#define GLM_FORCE_RADIANS
#define MAX_FRAME 240   
#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <GL/freeglut.h> // extension to glut
#include <iostream>
#include <chrono>
#include "vertex.cpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include "shaderLoader.cpp"
#include "assimpLoader.cpp"
#include "solarSystem.cpp"
#include "planet.cpp"

//--Evil Global variables
//Just for this example!
int w = 800, h = 800;// Window size
GLuint program;// The GLSL program handle
GLuint vbo_geometry[100];// VBO handle for our geometry
char * master_planet_filename;
int NUM_OF_VERTICIES = 0;
bool dt_flag = true;

//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLuint loc_texture;

//transform matrices
glm::mat4 model;//obj->world each object should have its own model matrix
glm::mat4 view;//world->eye
glm::mat4 projection;//eye->clip
glm::mat4 mvp;//premultiplied modelviewprojection

// Solar System stuff
SolarSystem solarsystem;

#include "camera_struct.cpp"

//--GLUT Callbacks
void render();
void update();
void reshape(int n_w, int n_h);

//--Resource management
bool initialize();
void cleanUp();

//--Random time things
float getDT();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;

//--I/O callbacks
void keyboard(unsigned char key, int x_pos, int y_pos);
void special_keyboard(int key, int x_pos, int y_pos);
void menu_options( int id );
void draw_orbit_rings( glm::mat4, float radius );

/********
--MAIN--
********/

int main(int argc, char **argv)
{
    // Initialize Magick
    Magick::InitializeMagick(*argv);

    // Initialize glut
    glutInit(&argc, argv); // just initializes
    // Saving obj file
    int filenamelength = strlen( argv[1] );
    master_planet_filename = new char [filenamelength+1];
    strcpy( master_planet_filename, argv[1] );

    /* changes options...  
    GLUT_DOUBLE enables double buffering (drawing to a background buffer while another buffer is displayed), 
    GLUT_DEPTH bit mask to select a window with a depth buffer */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); 
    glutInitWindowSize(w, h);

    // Name and create the Window
    glutCreateWindow("Solar System");

    // Create menus
    // sub menu creation
    glutCreateMenu(menu_options);
    glutAddMenuEntry("Start/Stop Movement", 1);
    glutAddMenuEntry("Exit Program", 2);
    glutAttachMenu( GLUT_RIGHT_BUTTON );


    // Now that the window is created the GL context is fully set up
    // Because of that we can now initialize GLEW to prepare work with shaders
    GLenum status = glewInit(); // initializes GLEW & checks for error (must be after GLUT)
    if( status != GLEW_OK)
    {
        std::cerr << "[F] GLEW NOT INITIALIZED: ";
        std::cerr << glewGetErrorString(status) << std::endl;
        return -1;
    }

    // Set all of the callbacks to GLUT that we need
    glutDisplayFunc(render);// Called continuously by GLUT internal loop when its time to display
    glutReshapeFunc(reshape);// Called if the window is resized
    glutIdleFunc(update);// Called if there is nothing else to do
    glutKeyboardFunc(keyboard);// Called if there is keyboard input
    glutSpecialFunc(special_keyboard);

    // Initialize all of our resources(shaders, geometry)
    bool init = initialize();
    if(init)
    {
        t1 = std::chrono::high_resolution_clock::now();
        glutMainLoop();
    }

    // Clean up after ourselves
    cleanUp();
    return 0;
}

//--Implementations
void render()
{
    //--Render the scene

    //clear the screen
    glClearColor(0.0, 0.0, 0.0, 1.0); // sets color for clearing the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    //enable the shader program
    glUseProgram(program);

 
    glm::mat4 sun_trans = solarsystem.getPlanetPointer(0) -> getModel();

    //go through and premultiple matricies
    for( int i = 0; i < solarsystem.getNumOfPlanets(); i++ ){
        //premultiply the matrix for this example
        model = solarsystem.getPlanetPointer(i) -> getModel();
        mvp = projection * view * model;

        //upload the matrix to the shader
        glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));
        
        if( i == 0 ){
            for( int j = 1; j < 10; j++ ){       
                draw_orbit_rings(sun_trans, solarsystem.getPlanetPointer(j)->getOrbitRadius()*3.2265);
            }
        }

        //Bind each texture to the corresponding object
        int local_texture = solarsystem.getPlanetPointer(i)->getLocTexture();
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, local_texture );

        //set up the Vertex Buffer Object so it can be drawn
        glEnableVertexAttribArray(loc_position);
        glEnableVertexAttribArray(loc_texture);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry[i]);

        //set pointers into the vbo for each of the attributes(position and color)
        glVertexAttribPointer( loc_position,//location of attribute
                               3,//number of elements
                               GL_FLOAT,//type
                               GL_FALSE,//normalized?
                               sizeof(Vertex),//stride
                               0);//offset

        glVertexAttribPointer( loc_texture,
                                2,
                                GL_FLOAT,
                                GL_FALSE,
                                sizeof(Vertex),
                                (void*)offsetof(Vertex,uv));

        glDrawArrays(GL_TRIANGLES, 0, 
        solarsystem.getPlanetPointer(i) -> getGeometry().size()*3);//mode, starting index, count

        //clean up
        glDisableVertexAttribArray(loc_position);
        glDisableVertexAttribArray(loc_texture);
    }

                           
    //swap the buffers
    glutSwapBuffers();
}

void update()
{
    float dt = 0.0;
    dt += getDT() * .1f;
    //SolarSystem::
    solarsystem.update( dt );
    CameraAnimation.updateCamera();
    glutPostRedisplay();
}

void reshape(int n_w, int n_h)
{
    w = n_w;
    h = n_h;
    //Change the viewport to be correct
    glViewport( 0, 0, w, h);

    //Update the projection matrix as well
    //See the init function for an explaination
    projection = glm::perspective(45.0f, float(w)/float(h), 0.01f, 200.0f);
}

bool initialize()
{
    // file parsing from the data files for the planet
    solarsystem.initialize( master_planet_filename );

    // V is where we keep all our info for the object
    for( int i = 0; i < solarsystem.getNumOfPlanets(); i++ ){
        std::vector<Vertex> v;
        v = solarsystem.getPlanetPointer(i)->getGeometry();

        NUM_OF_VERTICIES = v.size();

        // Create a Vertex Buffer object to store this vertex info on the GPU
        glGenBuffers(1, &vbo_geometry[i]); // 1st param-how many to create 2nd-address of array of GLuints
        glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry[i]);
        glBufferData(GL_ARRAY_BUFFER,
                    v.size() * sizeof(Vertex),
                    &v.front(),
                    GL_STATIC_DRAW);
    }

    // Creation of shaders
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER); 
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // Load the shaders
    std::string vs = shaderLoader::insertLoader("../bin/vrtxshdr.txt");   
    std::string fs = shaderLoader::insertLoader("../bin/frgshdr.txt");

    //compile the shaders
    GLint shader_status;

    // Vertex shader first NOTE::Casted VS & FS
    glShaderSource(vertex_shader, 1, (const GLchar**)&vs, NULL);
    glCompileShader(vertex_shader);
    //check the compile status
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] FAILED TO COMPILE VERTEX SHADER!" << std::endl;
        return false;
    }

    // Now the Fragment shader
    glShaderSource(fragment_shader, 1, (const GLchar**)&fs, NULL);
    glCompileShader(fragment_shader);
    //check the compile status
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] FAILED TO COMPILE FRAGMENT SHADER!" << std::endl;
        return false;
    }

    //Now we link the 2 shader objects into a program
    //This program is what is run on the GPU
    program = glCreateProgram(); // program object, link all shaders together to this
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    //check if everything linked ok
    glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
    if(!shader_status)
    {
        std::cerr << "[F] THE SHADER PROGRAM FAILED TO LINK" << std::endl;
        return false;
    }

    //Now we set the locations of the attributes and uniforms
    //this allows us to access them easily while rendering
    loc_position = glGetAttribLocation(program,
                    const_cast<const char*>("v_position"));
    if(loc_position == -1)
    {
        std::cerr << "[F] POSITION NOT FOUND" << std::endl;
        return false;
    }

    loc_texture = glGetAttribLocation(program,
                    const_cast<const char*>("v_uv"));
    if(loc_texture < 0)
    {
        std::cerr << "[F] V_COLOR NOT FOUND" << std::endl;
        return false;
    }

    loc_mvpmat = glGetUniformLocation(program,
                    const_cast<const char*>("mvpMatrix"));
    if(loc_mvpmat == -1)
    {
        std::cerr << "[F] MVPMATRIX NOT FOUND" << std::endl;
        return false;
    }
    
    

    //--Init the view and projection matrices
    //  if you will be having a moving camera the view matrix will need to more dynamic
    //  ...Like you should update it before you render more dynamic 
    //  for this project having them static will be fine
    view = glm::lookAt( CameraPosition, //Eye Position
                        CameraFocus, //Focus point
                        CameraYaw); //Positive Y is up

    projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                   float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                   0.01f, //Distance to the near plane, normally a small value like this
                                   100.0f); //Distance to the far plane, 

    CameraDirection = glm::normalize( CameraPosition - CameraFocus );
    Camera_RIGHT = glm::normalize( glm::cross(CameraYaw, CameraDirection) );
    Camera_UP = glm::cross(CameraDirection, Camera_RIGHT);

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //and its done
    return true;
}

void cleanUp()
{
    // Clean up, Clean up
    glDeleteProgram(program);
    for( int i = 0; i < solarsystem.getNumOfPlanets(); i++ ){
        glDeleteBuffers(1, &vbo_geometry[i]);
    }
}

void keyboard(unsigned char key, int x_pos, int y_pos)
{
    float cam_speed = 0.1;
    // Handle keyboard input
    if(key == 27)//ESC
       {
           exit(0);
       }
    else if ( key >= '0' and key <= '9' ){
        CameraAnimation.startingCameraPos = CameraPosition;
        CameraAnimation.startingCameraFocus = CameraFocus;
        //pan over to the front view, from wherever
        CameraAnimation.endingCameraFocus = glm::vec3(solarsystem.getPlanetPointer( key - '0' )->getModel() * glm::vec4(0.0, 0.0, 0.0, 1.0));
        CameraAnimation.endingCameraPos = CameraAnimation.endingCameraFocus + glm::vec3( 0.0, .5, -.5 );
        CameraAnimation.setAllFrames();   
    }
    else{
        switch( key ){
            case ')':
                CameraAnimation.planetToTrack = 0;
                break;
            case '!':
                CameraAnimation.planetToTrack = 1;
                break;
            case '@':
                CameraAnimation.planetToTrack = 2;
                break;
            case '#':
                CameraAnimation.planetToTrack = 3;
                break;
            case '$':
                CameraAnimation.planetToTrack = 4;
                break;
            case '%':
                CameraAnimation.planetToTrack = 5;
                break;
            case '^':
                CameraAnimation.planetToTrack = 6;
                break;
            case '&':
                CameraAnimation.planetToTrack = 7;
                break;
            case '*':
                CameraAnimation.planetToTrack = 8;
                break;
            case '(':
                CameraAnimation.planetToTrack = 9;
                break;
            //zoom in and out
            case '+':
                CameraPosition -= cam_speed * CameraDirection;
                CameraAnimation.planetToTrack = -1;
                break;
            case '-':
                CameraPosition += cam_speed * CameraDirection;
                CameraAnimation.planetToTrack = -1;
                break;
            //move forward
            case 'w':
                CameraPosition += cam_speed * Camera_UP;
                CameraAnimation.planetToTrack = -1;
                break;
            //move backward
            case 's':
                CameraPosition -= cam_speed * Camera_UP;
                CameraAnimation.planetToTrack = -1;
                break;
            //move left   
            case 'a':
                CameraPosition -= cam_speed * Camera_RIGHT;
                CameraAnimation.planetToTrack = -1;
                break;
            //move right
            case 'd':
                CameraPosition += cam_speed * Camera_RIGHT;
                CameraAnimation.planetToTrack = -1;
            default:
                break;
        }
    }
    view = glm::lookAt( CameraPosition, //Eye Position
                        CameraFocus, //Focus point
                        CameraYaw ); //Positive Y is up

    CameraDirection = glm::normalize( CameraPosition - CameraFocus );
    Camera_RIGHT = glm::normalize( glm::cross(CameraYaw, CameraDirection) );
    Camera_UP = glm::normalize( glm::cross(CameraDirection, Camera_RIGHT) );
   
    glutPostRedisplay();
}

void draw_orbit_rings( glm::mat4 origin, float radius ){
    if( radius <= 0 ){
        return;
    }
    glm::vec3 pos = glm::vec3( origin[3] );

    float twice_pi = 2.0 * M_PI;

    glBegin(GL_LINE_LOOP);

    for( int i = 0; i < 100; i++ ){
        glVertex3f(
            pos.x + (radius * cos(i * twice_pi / 100) ),
            0,
            pos.y + (radius * sin(i * twice_pi / 100) )
        );
    }
    glEnd();
}

void special_keyboard(int key, int x_pos, int y_pos)
{
    float cam_speed = 0.1f;
    glm::vec3 offsetVector;
    switch(key){
        case GLUT_KEY_LEFT:
            offsetVector = glm::vec3( cam_speed, 0, 0 );
            break;
        case GLUT_KEY_RIGHT:
            offsetVector = glm::vec3( -cam_speed, 0, 0 );
            break;
        case GLUT_KEY_UP:
            offsetVector = glm::vec3( 0, 0, cam_speed );
            break;
        case GLUT_KEY_DOWN:
            offsetVector = glm::vec3( 0, 0, -cam_speed );
            break;      
    }
    CameraPosition += offsetVector;
    CameraFocus += offsetVector;

    view = glm::lookAt( CameraPosition, //Eye Position
                        CameraFocus, //Focus point
                        CameraYaw ); //Positive Y is up

    CameraDirection = glm::normalize( CameraPosition - CameraFocus );
    Camera_RIGHT = glm::normalize( glm::cross(CameraYaw, CameraDirection) );
    Camera_UP = glm::normalize( glm::cross(CameraDirection, Camera_RIGHT) );

    glutPostRedisplay();
}

void menu_options( int id ){
    //commands for each selection
    switch( id ){
        case 1:
            dt_flag = !dt_flag;
            break;
        case 2:
            exit(0);
            break;
    }
}

float getDT()
{
    float ret;
    if( !dt_flag ) 
        t1 = std::chrono::high_resolution_clock::now();
    t2 = std::chrono::high_resolution_clock::now();
    ret = std::chrono::duration_cast< std::chrono::duration<float> >(t2-t1).count();
    t1 = std::chrono::high_resolution_clock::now();
    return ret;
}