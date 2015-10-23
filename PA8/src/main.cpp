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
#include "assimpLoader.cpp"

#include "btBulletDynamicsCommon.h"
#include <stdio.h>

//--Evil Global variables
//Just for this example!
int w = 800, h = 800;// Window size
GLuint program;// The GLSL program handle
GLuint vbo_geometry[2];// VBO handle for our geometry
char * model_filename;
char * texture_filename;
int NUM_OF_VERTICIES[3];


//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLuint loc_texture;
GLuint pic_textures[2];

//transform matrices
glm::mat4 model[2];//obj->world each object should have its own model matrix
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
float getDT();
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
// triangle mesh 
btTriangleMesh *objTriMesh[3];
// the rigid body..
btRigidBody *rigidBody[3];

#define BIT(x) (1<<(x))
enum collisiontypes{
	COL_NOTHING = 0,
	COL_TABLE = BIT(0),
	COL_BALL = BIT(1)
};

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
    model_filename = new char [filenamelength+1];
    strcpy( model_filename, argv[1] );

    // Saving texture
    filenamelength = strlen( argv[2] );
    texture_filename = new char [filenamelength+1];
    strcpy( texture_filename, argv[2] );

    /* changes options...  
    GLUT_DOUBLE enables double buffering (drawing to a background buffer while another buffer is displayed), 
    GLUT_DEPTH bit mask to select a window with a depth buffer */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); 
    glutInitWindowSize(w, h);

    // Name and create the Window
    glutCreateWindow("Bullet Part I");

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
    delete broadphase;
    broadphase = NULL;

    delete collisionConfiguration;
    collisionConfiguration = NULL;
    
    delete dispatcher;
    dispatcher = NULL; 
    
    delete solver;
	solver = NULL;
    
    cleanUp();
    return 0;
}

//--Implementations
bool initialize()
{
	//bullet allocating stuff
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration); 
	solver = new btSequentialImpulseConstraintSolver;
	objTriMesh[0] = new btTriangleMesh();
	objTriMesh[1] = new btTriangleMesh();
	objTriMesh[2] = new btTriangleMesh();

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase,
												solver, collisionConfiguration);	
	dynamicsWorld->setGravity(btVector3(0,-9.81,0));

    // Initialize basic geometry and shaders for this example
    assimpLoader AI_Obj( model_filename, texture_filename ); //

    assimpLoader second_Obj("../bin/casket_uv.obj","../bin/metal.jpg");

    assimpLoader third_Obj("../bin/ramp.obj", "../bin/ice.jpg");

    AI_Obj.orderVertices( objTriMesh[0] );
    second_Obj.orderVertices( objTriMesh[1] );
    third_Obj.orderVertices( objTriMesh[2] );

    // V is where we keep all our info for the object
    std::vector<Vertex> v;
    v = AI_Obj.getOrderedVertices();

    std::vector<Vertex> u;
    u = second_Obj.getOrderedVertices();

    std::vector<Vertex> v;
    w = third_Obj.orderVertices();

    NUM_OF_VERTICIES[0] = v.size();
    NUM_OF_VERTICIES[1] = u.size();
    NUM_OF_VERTICIES[2] = w.size();

    // collision pointer
    btCollisionShape *shape = new btBvhTriangleMeshShape(objTriMesh[0], true);
    btCollisionShape *shape2 = new btBvhTriangleMeshShape(objTriMesh[1], true);
    btCollisionShape *shape3 = new btBvhTriangleMeshShape(objTriMesh[2], true);
    btCollisionShape *groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
    btCollisionShape *fallShape = new btSphereShape(1);

    btScalar mass(1);
    btVector3 inertia(1, 2, 10);
   
    // object characteristics & creation of the rigid body
    btDefaultMotionState *shapeMotionState = NULL;
    shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(1, 1, 1, 1), btVector3(2.8, 5, 0)));
    fallShape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, fallShape, inertia);
    rigidBody[0] = new btRigidBody(shapeRigidBodyCI);
   
    btDefaultMotionState *groundMotionState = NULL;
    groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    shape2->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI2(0, groundMotionState, shape2, inertia);
    rigidBody[1] = new btRigidBody(shapeRigidBodyCI2);

	btDefaultMotionState *triangleMotionState = NULL;
    triangleMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    shape3->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI3(mass, groundMotionState, shape3, inertia);
    rigidBody[2] = new btRigidBody(shapeRigidBodyCI3);    

    // dynamicsWorld->addRigidBody(rigidBody,COLLIDE_MASK, CollidesWith); COLLIDE_MASK & Collision... IDK
    /*
    int ballCollideWith = COL_TABLE;
    int tableCollideWith = COL_BALL;
    dynamicsWorld->addRigidBody(rigidBody[0], COL_BALL, ballCollideWith);
    dynamicsWorld->addRigidBody(rigidBody[1], COL_TABLE, tableCollideWith);
    */
    dynamicsWorld->addRigidBody(rigidBody[0]);
    dynamicsWorld->addRigidBody(rigidBody[1]);

    // Create a Vertex Buffer object to store this vertex info on the GPU
    glGenBuffers(1, &vbo_geometry[0]); // 1st param-how many to create 2nd-address of array of GLuints
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry[0]);
    glBufferData(GL_ARRAY_BUFFER,
                v.size() * sizeof(Vertex),
                &v.front(),
                GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_geometry[1]); // 1st param-how many to create 2nd-address of array of GLuints
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry[1]);
    glBufferData(GL_ARRAY_BUFFER,
                u.size() * sizeof(Vertex),
                &u.front(),
                GL_STATIC_DRAW);

    // Text loading
    AI_Obj.mapTextures(pic_textures[0]);
    second_Obj.mapTextures(pic_textures[1]);

    //get picture texture location
    pic_textures[0] = AI_Obj.getLocTexture();
    pic_textures[1] = second_Obj.getLocTexture();

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

void render()
{
    //--Render the scene

    //clear the screen
    glClearColor(0.174, 0.167, 0.159, 1.0); // sets color for clearing the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    for( int i = 0; i < 2; i++ ){    
        //premultiply the matrix for this example
        mvp = projection * view * model[i];

        //enable the shader program
        glUseProgram(program);

        //upload the matrix to the shader
        glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));

        //Bind each texture to the corresponding object
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, pic_textures[i] );

        //set up the Vertex Buffer Object so it can be drawn
        glEnableVertexAttribArray(loc_position);
        glEnableVertexAttribArray(loc_texture);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry[i]);
        glBindTexture( GL_TEXTURE_2D, loc_texture );

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

        glDrawArrays(GL_TRIANGLES, 0, NUM_OF_VERTICIES[i]*3);//mode, starting index, count
    }

    //clean up
    glDisableVertexAttribArray(loc_position);
    glDisableVertexAttribArray(loc_texture);
                           
    //swap the buffers
    glutSwapBuffers();
}

void update()
{
	btTransform trans;
	btScalar m[16];
	dynamicsWorld->stepSimulation(getDT(), 10);
	
    rigidBody[0]->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(m);
    model[0] = glm::make_mat4(m);
    
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
    projection = glm::perspective(45.0f, float(w)/float(h), 0.01f, 100.0f);
}

void cleanUp()
{
    // Clean up, Clean up
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_geometry[0]);
}

//returns the time delta
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
		/*
         case GLUT_KEY_LEFT:
             model[1] = glm::rotate(model[1, (.2f), glm::vec3(0.0,1.0,0.0));
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
		*/
      }

    glutPostRedisplay();
}
