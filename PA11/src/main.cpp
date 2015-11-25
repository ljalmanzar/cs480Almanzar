#define GLM_FORCE_RADIANS
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

#include "GLD.cpp"
#include "camera.h"
#include "light.cpp"
#include "gameDriver.cpp"

//--Evil Global variables
int w = 800, h = 800;// Window size
GLuint program;// The GLSL program handle
GLuint vbo_geometry;// VBO handle for our geometry
int NUM_OF_VERTICIES = 0;
Camera camera;
GameDriver maingame;

//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLuint loc_texture;
GLint loc_normal;
GLint loc_lightpos;
GLint loc_diffuse;

vector<GLD*> allObjects;

// Suggested by gunnar
Light theLight; // 0 ambient, 1 distant, 2 point, 3 spot

enum GameState{
    AMBIENT = 0,
    DISTANT,
    POINT,
    SPOT
} lightType = AMBIENT;

//transform matrices
glm::mat4 model;//obj->world each object should have its own model matrix
glm::mat4 view;//world->eye
glm::mat4 projection;//eye->clip
glm::mat4 mvp;//premultiplied modelviewprojection

//--GLUT Callbacks
void render();
void update();
void reshape(int n_w, int n_h);

//--Resource management
bool initialize();
void cleanUp();

//--Random time things
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;

//--I/O callbacks
void keyboard(unsigned char key, int x_pos, int y_pos);
void special_keyboard(int key, int x_pos, int y_pos);
float getDT();

// BULLET STUFF
//check for collisions, (broad or narrow, broad eliminates the non-colliding objs)
btBroadphaseInterface *broadphase;
// collision algorithm (heavy lifting)
btDefaultCollisionConfiguration *collisionConfiguration;
// param: collisionConfig pointer, used for collision algorithm, sends events to obj
btCollisionDispatcher *dispatcher; 
// makes everything work well, forces & shit. world & obj 
btSequentialImpulseConstraintSolver *solver;
// the world
btDiscreteDynamicsWorld *dynamicsWorld;

/********
--MAIN--
********/

int main(int argc, char **argv)
{
    // Initialize Magick
    Magick::InitializeMagick(*argv);

    // Initialize glut
    glutInit(&argc, argv); // just initializes

    /* changes options...  
    GLUT_DOUBLE enables double buffering (drawing to a background buffer while another buffer is displayed), 
    GLUT_DEPTH bit mask to select a window with a depth buffer */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); 
    glutInitWindowSize(w, h);

    // Name and create the Window
    glutCreateWindow("Lighting");

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

bool initialize()
{
    //bullet allocating stuff
    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration); 
    solver = new btSequentialImpulseConstraintSolver();

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase,
                                                solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0.0f,-9.8f,0.0f));

    //light
    theLight.position = glm::vec4(5.0,5.0,5.0,0.0);
    theLight.diffuse = glm::vec4(1.0,1.0,1.0,0.0);

/*
    allObjects[0].initialize("../bin/peeps_model.obj","../bin/ice.jpg");
    allObjects[0].translate(glm::vec3(5,0,0));
    allObjects[1].initialize("../bin/planet.obj","../bin/ice.jpg");
    allObjects[1].translate(glm::vec3(-5,0,0));
*/

    maingame.initGame();

    // Creation of shaders
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER); 
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // Load the shaders
    std::string vs = shaderLoader::insertLoader("../bin/p_vertex_light.txt");   
    std::string fs = shaderLoader::insertLoader("../bin/pass_through_fragment.txt");

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
        std::cerr << "[F] UV NOT FOUND" << std::endl;
        return false;
    }

    loc_normal = glGetAttribLocation(program,
                    const_cast<const char*>("v_normal"));
    if(loc_normal == -1){
        std::cerr << "[F] NORMAL NOT FOUND" << std::endl;
        return false;
    }

    loc_lightpos = glGetUniformLocation(program,
                    const_cast<const char*>("l_lightpos"));
    if(loc_lightpos == -1){
        std::cerr << "[F] LIGHT POS NOT FOUND" << std::endl;
        return false;
    }

    loc_diffuse = glGetUniformLocation(program,
                    const_cast<const char*>("l_diffuse"));
    if(loc_diffuse == -1){
        std::cerr << "[F] LIGHT DIFFUSE NOT FOUND" << std::endl;
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
    view = glm::lookAt( glm::vec3(0.0, 8.0, -8.0), //Eye Position
                        glm::vec3(0.0, 0.0, 0.0), //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

    projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                   float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                   0.01f, //Distance to the near plane, normally a small value like this
                                   100.0f); //Distance to the far plane, 

    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //and its done
    return true;
}

//--Implementations
void render()
{
    //--Render the scene

    //clear the screen
    glClearColor(0.174, 0.167, 0.159, 1.0); // sets color for clearing the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    
    //enable the shader program
    glUseProgram(program);

    //get the most recent camera data
    view = camera.getViewMatrix();
    //give information to shades for static lights
    glUniform4fv(loc_lightpos, 1, &theLight.position[0]);
    glUniform4fv(loc_diffuse, 1, &theLight.diffuse[0]);

    for( unsigned int objIndex = 0; objIndex < allObjects.size(); objIndex++ ){
        //draw only the objects we want to see and skip elseways
        if( !allObjects[objIndex]->isDrawable() ){
            continue;
        }

        //premultiply the matrix for this example
        model = maingame.getMasterTransform() * allObjects[objIndex]->getModel();
        mvp = projection * view * model;

        //upload the matrix to the shader
        glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));

        //Bind each texture to the corresponding object
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, allObjects[objIndex]->getPicTexture() );

        //set up the Vertex Buffer Object so it can be drawn
        glEnableVertexAttribArray(loc_position);
        glEnableVertexAttribArray(loc_texture);
        glEnableVertexAttribArray(loc_normal);

        glBindBuffer(GL_ARRAY_BUFFER, allObjects[objIndex]->getVBO());

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

        
        glVertexAttribPointer( loc_normal,
                                3,
                                GL_FLOAT,
                                GL_FALSE,
                                sizeof(Vertex),
                                (void*)offsetof(Vertex,normal));


        glDrawArrays(GL_TRIANGLES, 0, (allObjects[objIndex]->getNumOfVerticies()));//mode, starting index, count

        //clean up
        glDisableVertexAttribArray(loc_position);
        glDisableVertexAttribArray(loc_texture);
        glDisableVertexAttribArray(loc_normal);
    }


                           
    //swap the buffers
    glutSwapBuffers();
}

void update()
{
    dynamicsWorld->stepSimulation(getDT(), 10);
    allObjects = maingame.getAllObjects();
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


void cleanUp()
{
    // Clean up, Clean up
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_geometry);
    
    delete dynamicsWorld;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;
}

float getDT()
{
    float ret;
    t2 = std::chrono::high_resolution_clock::now();
    ret = std::chrono::duration_cast< std::chrono::duration<float> >(t2-t1).count();
    t1 = std::chrono::high_resolution_clock::now();
    return ret;
}

void keyboard(unsigned char key, int x_pos, int y_pos)
{
    // Handle keyboard input
    if(key == 27){ //ESC
        exit(0);
    }
    else if(key == '+'){
        camera.zoom( 1 );     
    }
    else if(key == '-'){
        camera.zoom( -1 );     
    }
    else {
        switch( key ){
            case '[':
                theLight.position[0] += 0.25;
                break;
            case ']':
                theLight.position[0] -= 0.25;
                break;
            case '8':
                camera.pivot( P_UP );
                break;
            case '2':
                camera.pivot( P_DOWN );
                break;
            case '4':
                camera.pivot( P_LEFT );
                break;
            case '6':
                camera.pivot( P_RIGHT );
                break;
            case 'p':
            case 'P':
                glutDisplayFunc(render);
                break;
            case 'a':
            case 'A':
            theLight.position[3] = AMBIENT;
            theLight.diffuse[3] = AMBIENT;
                break;
            case 's':
            case 'S':
            theLight.position[3] = SPOT;
            theLight.diffuse[3] = SPOT;
                break;
            case 'd':
            case 'D':
            theLight.position[3] = DISTANT;
            theLight.diffuse[3] = DISTANT;
                break;
            case 'f':
            case 'F':
            theLight.position[3] = POINT;
            theLight.diffuse[3] = POINT;
                break;
        }        
    }
    glutPostRedisplay();
}

void special_keyboard(int key, int x_pos, int y_pos){
    switch(key){
        case GLUT_KEY_UP:
            maingame.tiltOnX( 1.0f );
            break;
        case GLUT_KEY_DOWN:
            maingame.tiltOnX( -1.0f );
            break;
        case GLUT_KEY_LEFT:
            maingame.tiltOnZ( 1.0f );
            break;
        case GLUT_KEY_RIGHT:
            maingame.tiltOnZ( -1.0f );
            break;
    }
    glutPostRedisplay();
}