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
#include "camera.h"
#include "mousePicker.h"

//--Evil Global variables
int w = 800, h = 800;// Window size
GLuint program;// The GLSL program handle
GLuint vbo_geometry;// VBO handle for our geometry
int NUM_OF_VERTICIES = 0;
Camera camera;
MousePicker mousePicker;

float gameCounter = 0.0;

bool LEFTGOAL = false;
bool RIGHTGOAL = false;

enum GameState{
    STARTUP = 0,
    PLAY,
    PAUSE,
    SCOREBOARD,
    FINISHED
} state = STARTUP;

//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLuint loc_texture;

GLD midWall; 

std::vector<GLD*> allObjects;

string top10File;
//transform matrices
glm::mat4 model;//obj->world each object should have its own model matrix
glm::mat4 view;//world->eye
glm::mat4 projection;//eye->clip
glm::mat4 mvp;//premultiplied modelviewprojection

//--GLUT Callbacks
void render();
void update();
void reshape(int n_w, int n_h);

//Game Menu Call Backs
void render_Menu();
void render_Top10();
void render_END();
void win_Menu();

//--Resource management
bool initialize();
void cleanUp();

//--Random time things
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;

//--I/O callbacks
void keyboard(unsigned char key, int x_pos, int y_pos);
void special_keyboard(int key, int x_pos, int y_pos);
void mouse(int x_pos, int y_pos);
void menu_options( int id );
float getDT();

GameDriver mainGame;

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

    // HardCode top10File
    top10File = "../bin/scoreboard";

    /* changes options...  
    GLUT_DOUBLE enables double buffering (drawing to a background buffer while another buffer is displayed), 
    GLUT_DEPTH bit mask to select a window with a depth buffer */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); 
    glutInitWindowSize(w, h);

    // Name and create the Window
    glutCreateWindow("Air Hockey");

    glutCreateMenu(menu_options);
    glutAddMenuEntry("Start Game", 1);
    glutAddMenuEntry("Exit Game", 2);
    glutAddMenuEntry("Turn on AI", 3);
    glutAddMenuEntry("Turn off AI", 4);
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
    //glutDisplayFunc(win_Menu);
    glutDisplayFunc(render_Menu);// Called continuously by GLUT internal loop when its time to display

    glutReshapeFunc(reshape);// Called if the window is resized
    glutIdleFunc(update);// Called if there is nothing else to do
    glutKeyboardFunc(keyboard);// Called if there is keyboard input
    glutSpecialFunc(special_keyboard);
    glutMotionFunc(mouse);

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

    midWall.initialize("../bin/middle_wall.obj", "../bin/ah_final_texture.png", true, TRIMESH, STATIC);
    allObjects.push_back(&midWall);

    mousePicker.initialize(camera, projection, view);
    mainGame.initGame();
    allObjects = mainGame.getAllObjects();

    // add physics where needed & and add to world
    for (unsigned int objectNdx = 0; objectNdx < allObjects.size(); ++objectNdx)
        {
            if (allObjects[objectNdx]->getShape() != NONE){
                allObjects[objectNdx]->addPhysics();
                dynamicsWorld->addRigidBody(allObjects[objectNdx]->getRigidBody());
            }
        }

    mainGame.initTableAttributes();
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

    //write the player scores to the screen before anything else happens
    mainGame.printScores();
    
    //enable the shader program
    glUseProgram(program);

    //get the most recent camera data
    view = camera.getViewMatrix();
    mousePicker.update(camera, view);

    for( unsigned int objIndex = 0; objIndex < allObjects.size(); objIndex++ ){
        //draw only the objects we want to see and skip elseways
        if( !allObjects[objIndex]->isDrawable() ){
            continue;
        }

        //premultiply the matrix for this example
        model = allObjects[objIndex]->getModel();
        mvp = projection * view * model;

        //upload the matrix to the shader
        glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));

        //Bind each texture to the corresponding object
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, allObjects[objIndex]->getPicTexture() );

        //set up the Vertex Buffer Object so it can be drawn
        glEnableVertexAttribArray(loc_position);
        glEnableVertexAttribArray(loc_texture);

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

        glDrawArrays(GL_TRIANGLES, 0, (allObjects[objIndex]->getNumOfVerticies()));//mode, starting index, count

        //clean up
        glDisableVertexAttribArray(loc_position);
        glDisableVertexAttribArray(loc_texture);
    }
                           
    //swap the buffers
    glutSwapBuffers();
}

void render_Menu(){
    // clear the screen
    glClearColor(0.174, 0.167, 0.159, 1.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // set the color
    glColor3f( 1.0, 1.0, 1.0 );
    // no program needed to print
    glUseProgram(0);

    //set the text i want to say
    char * tempStr;
    int cursor = 0;
    std::string information[] = {
        "Air Hockey", 
        "by Gunnar, Luis and Daniel (GLD)",
        "Press 'P'     to play a New Game",
        "Press 'SPACE' to pause the game",
        "Press 'T'     to view the the 10 scoreboard", 
        "Press 'ESC'   to quit"
    };

    //print the title
    glRasterPos2f(-.1, .5);
    tempStr = &information[0][0];
    while( tempStr[cursor] ){
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, tempStr[cursor++] );
    }
    cursor = 0;

    //print the subtitle
    glRasterPos2f(-.25, .35);
    tempStr = &information[1][0];
    while( tempStr[cursor] ){
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, tempStr[cursor++] );
    }
    cursor = 0;

    //print out the instructions
    for( int i = 2; i < 6; i++ ){
        glRasterPos2f(-.2, -float(i)/20);
        tempStr = &information[i][0];
        while( tempStr[cursor] ){
            glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, tempStr[cursor++] );
        }
        cursor = 0;
    }

    glutSwapBuffers();
}

void render_Top10(){
    // clear the screen
    glClearColor(0.174, 0.167, 0.159, 1.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // set the color
    glColor3f( 1.0, 1.0, 1.0 );
    // no program needed to print
    glUseProgram(0);

    //set the text i want to say

    char * tempStr;
    int cursor = 0;
    std::string information[] = {
        "Scoreboard", 
        "Quickest Games (Time Measurement - Some computer time)",
        "1) ",
        "2) ",
        "3) ", 
        "4) ",
        "5) ",
        "6) ",
        "7) ", 
        "8) ",
        "9) ",
        "10) "
    };

    //print stuff out
    for( int i = 0; i < 12; i++ ){
        glRasterPos2f(-.2, -float(i)/20);
        tempStr = &information[i][0];
        while( tempStr[cursor] ){
            glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, tempStr[cursor++] );
        }
        cursor = 0;
    }

    glutSwapBuffers();
}

void render_END(){
    glClearColor(0.174, 0.167, 0.159, 1.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // set the color
    glColor3f( 1.0, 1.0, 1.0 );
    // no program needed to print
    glUseProgram(0);

    //set the text i want to say
    char * tempStr;
    int cursor = 0;

    std::string endGameText[3];

    if (mainGame.getPlayer1()->getScore() > mainGame.getPlayer2()->getScore()){
        endGameText[0] = "PLAYER ONE WINS!";
        endGameText[1] = "Thanks for playing!";
        endGameText[2] = "Press 'ESC' to quit";
       
    } else{
        endGameText[0] = "PLAYER TWO WINS!";
        endGameText[1] = "Thanks for playing!";
        endGameText[2] = "Press 'ESC' to quit";
    }
    
     //print the title
    glRasterPos2f(-.1, .5);
    tempStr = &endGameText[0][0];
    while( tempStr[cursor] ){
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, tempStr[cursor++] );
    }
    cursor = 0;

    //print out the instructions
    for( int i = 1; i < 3; i++ ){
        glRasterPos2f(-.2, -float(i)/20);
        tempStr = &endGameText[i][0];
        while( tempStr[cursor] ){
            glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, tempStr[cursor++] );
        }
        cursor = 0;
    }

    glutSwapBuffers();
}

void win_Menu(){
    glutSwapBuffers();
}

void update()
{
    //if this is the main menu state
        //draw the main menu
        //listen for input
    //if this is the play state
        //do all the upkeep
    //if this is the top 10 scoreboard state
    allObjects = mainGame.getAllObjects();

    dynamicsWorld->stepSimulation(getDT(), 10);
    
    for (unsigned int currentObj = 0; currentObj < allObjects.size(); ++currentObj){
        allObjects[currentObj]->updateObjectAndPhysics();
    }

    // check if time for power up and then move box
    mainGame.getPU();

    // check if powerup has been hit
    if (mainGame.isPowerupActive())
        mainGame.checkForMysteryBox(dynamicsWorld);


    if( mainGame.isGameOver()){
        glutDisplayFunc(win_Menu);
    }

    mainGame.checkForMidBoundry();

    mainGame.checkForGoal(dynamicsWorld); 
    if (LEFTGOAL || RIGHTGOAL){

    }

    // ai stuff
    if (mainGame.isAiActive()){
        int puckNdx = mainGame.findPuck();
        glm::mat4 puckModel = allObjects[puckNdx]->getModel();
        glm::vec3 positionOfPuck = glm::vec3(puckModel[3]);

        std::cout << positionOfPuck[0]<< " " <<positionOfPuck[1]<<" "<< positionOfPuck[2] << endl;

        glm::mat4 p2PaddleModel = allObjects[1]->getModel();
        glm::vec3 positionOfPaddle = glm::vec3(p2PaddleModel[3]);

        if (positionOfPuck[2] > positionOfPaddle[2]){
            allObjects[1]->setVelocity(0,0,5);
        }   
        else{
            allObjects[1]->setVelocity(0,0,-5);
        } 

        /*
        if(positionOfPuck[0] < 0){
                allObjects[1]->setVelocity(5,0,0);
            }
        */
    }

    if (mainGame.isGameOver()){
        glutDisplayFunc(render_END);
    }

    gameCounter+= .05;

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
    if( state != PLAY ){
        t2 = t1;
    }
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
            case 'w':
                mainGame.setP1PaddlePos(D_UP, &camera);
                break;
            case 'a':
                  mainGame.setP1PaddlePos(D_LEFT, &camera);
                break;
            case 's':
                 mainGame.setP1PaddlePos(D_DOWN, &camera);
                break;
            case 'd':
                mainGame.setP1PaddlePos(D_RIGHT, &camera);
                break;
            case 'p':
            case 'P':
                glutDisplayFunc(render);
                state = PLAY;
                break;
            case 'm':
            case 'M':
                glutDisplayFunc(render_Menu);
                state = STARTUP;
                break;
            case 't':
            case 'T':
                glutDisplayFunc(render_Top10);
                state = SCOREBOARD;
                break;
            case ' ':
                if( state == PAUSE )
                    state = PLAY;
                else if (state == PLAY)
                    state = PAUSE;
                break;
        }        
    }
    glutPostRedisplay();
}

void mouse(int x_pos, int y_pos){
    mousePicker.calculateMouseRay(x_pos, y_pos, w, h);
    mainGame.setP1PaddleMouse(mousePicker,&camera);
}

void special_keyboard(int key, int x_pos, int y_pos)
{
    switch(key){
        case GLUT_KEY_UP:
            mainGame.setP2PaddlePos(D_UP, &camera);
            break;
        case GLUT_KEY_LEFT:
            mainGame.setP2PaddlePos(D_LEFT, &camera);
            break;
        case GLUT_KEY_DOWN:
            mainGame.setP2PaddlePos(D_DOWN, &camera);
            break;
        case GLUT_KEY_RIGHT:
            mainGame.setP2PaddlePos(D_RIGHT, &camera);
    }
    glutPostRedisplay();
}

void menu_options( int id ){
    //commands for each selection
    switch( id ){
        case 1:
             break;
        case 2:
            exit(0);
            break;
        case 3:
            mainGame.setAi(true);
            break;
        case 4:
            mainGame.setAi(false);
            break;
    }
}
