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

#include "gameDriver.cpp"
#include "player.cpp"
#include "powerup.cpp"
#include "GLD.cpp"

//--Evil Global variables
//Just for this example!
int w = 640, h = 480;// Window size
GLuint program;// The GLSL program handle
GLuint vbo_geometry;// VBO handle for our geometry
int NUM_OF_VERTICIES = 0;

//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLuint loc_texture;

std::vector<GLD> allObjects;

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

// BULLET STUFF
//check for collisions
btBroadphaseInterface *broadphase;
// collision algorithm
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
    glutCreateWindow("Air Hockey");

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
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase,
												solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0,-9.81,0));

	GLD objectController("../bin/peeps_model.obj","../bin/metal.jpg");
    GLD someotherObject("../bin/mario_mystery_box.obj", "../bin/Color_icon_yellow.png");
    glm::mat4 transformMatrix = glm::translate( 
        someotherObject.getModel(),
        glm::vec3(-4.0f, 0.0, 0.0) 
        );
    transformMatrix = glm::scale(
        transformMatrix,
        glm::vec3(.4f, .4f, .4f)
        );
    someotherObject.setModel( transformMatrix );

    GLD aThirdObject("../bin/feather.obj", "../bin/26-soft-white-nice-feather-texture.jpg");
    transformMatrix = glm::translate( 
        someotherObject.getModel(),
        glm::vec3(4.0f, 0.0, 0.0) 
        );
    transformMatrix = glm::scale(
        transformMatrix,
        glm::vec3(.4f, .4f, .4f)
        );
    someotherObject.setModel( transformMatrix );

    allObjects.push_back(objectController);
    allObjects.push_back(someotherObject);
    allObjects.push_back(aThirdObject);
    //objectController = new GLD("../bin/peeps_model.obj","../bin/metal.jpg");
    //objectController->addPhysics();
    
   // dynamicsWorld->addRigidBody( objectController->getRigidBody() );

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

    for( unsigned int objIndex = 0; objIndex < allObjects.size(); objIndex++ ){
        //premultiply the matrix for this example
        model = allObjects[objIndex].getModel();
        mvp = projection * view * model;

        //enable the shader program
        glUseProgram(program);

        //upload the matrix to the shader
        glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));

        //Bind each texture to the corresponding object
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, allObjects[objIndex].getPicTexture() );

        //set up the Vertex Buffer Object so it can be drawn
        glEnableVertexAttribArray(loc_position);
        glEnableVertexAttribArray(loc_texture);

        glBindBuffer(GL_ARRAY_BUFFER, allObjects[objIndex].getVBO());

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

        glDrawArrays(GL_TRIANGLES, 0, (allObjects[objIndex].getNumOfVerticies()));//mode, starting index, count

        //clean up
        glDisableVertexAttribArray(loc_position);
        glDisableVertexAttribArray(loc_texture);
    }
                           
    //swap the buffers
    glutSwapBuffers();
}

void update()
{

}

void reshape(int n_w, int n_h)
{
    w = n_w;
    h = n_h;
    //Change the viewport to be correct
    glViewport( 0, 0, w, h);

    //Update the projection matrix as well
    //See the init function for an explaination
    projection = glm::perspective(45.0f, float(w)/float(h), 0.01f, 100.0f);
}


void cleanUp()
{
    // Clean up, Clean up
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_geometry);
}

void keyboard(unsigned char key, int x_pos, int y_pos)
{
    static float CameraZoom = 8.0;

    // Handle keyboard input
    if(key == 27)//ESC
       {
           exit(0);
       }
    if(key == '+'){
        CameraZoom -= .5f;
        view = glm::lookAt( glm::vec3(0.0, CameraZoom, -CameraZoom), //Eye Position
                            glm::vec3(0.0, 0.0, 0.0), //Focus point
                            glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up        
    }
    if(key == '-'){
        CameraZoom += .5f;
        view = glm::lookAt( glm::vec3(0.0, CameraZoom, -CameraZoom), //Eye Position
                            glm::vec3(0.0, 0.0, 0.0), //Focus point
                            glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up        
    }
    glutPostRedisplay();
}

void special_keyboard(int key, int x_pos, int y_pos)
{
   switch(key)
      {
         case GLUT_KEY_LEFT:
         model = glm::rotate(model, (.2f), glm::vec3(0.0,1.0,0.0));
         break;

         case GLUT_KEY_RIGHT:
         model = glm::rotate(model, -(.2f), glm::vec3(0.0,1.0,0.0));
         break;

       case GLUT_KEY_UP:
         model = glm::rotate(model, (.2f), glm::vec3(1.0,0.0,0.0));
         break;

         case GLUT_KEY_DOWN:
         model = glm::rotate(model, -(.2f), glm::vec3(1.0,0.0,0.0));
         break;         
      }

    glutPostRedisplay();
}
