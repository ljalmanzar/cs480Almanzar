#define GLM_FORCE_RADIANS
#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <GL/freeglut.h> // extension to glut
#include <iostream>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include "loader.cpp"

//--Data types
//This object will define the attributes of a vertex(position, color, etc...)
struct Vertex
{
    GLfloat position[3];
    GLfloat color[3];
};

//--Evil Global variables
//Just for this example!
int w = 640, h = 480;// Window size
GLuint program;// The GLSL program handle
GLuint vbo_geometry;// VBO handle for our geometry

//uniform locations
GLint loc_mvpmat;// Location of the modelviewprojection matrix in the shader

//attribute locations
GLint loc_position;
GLint loc_color;

//transform matrices
glm::mat4 moon_model;
glm::mat4 model;//obj->world each object should have its own model matrix
glm::mat4 view;//world->eye
glm::mat4 projection;//eye->clip
glm::mat4 mvp;//premultiplied modelviewprojection

//direction change int
int PLANET_ROTAION_DIRECTION = 1;
int PLANET_ORBIT_DIRECTION = 1;

int MOON_ROTAION_DIRECTION = 1;
int MOON_ORBIT_DIRECTION = 1;

// spin pause flag
bool PAUSED = false; 

//--GLUT Callbacks
void render();
void update();
void reshape(int n_w, int n_h);
void keyboard(unsigned char key, int x_pos, int y_pos);
void special_keyboard(int key, int x_pos, int y_pos);

//--Resource management
bool initialize();
void cleanUp();

//--Random time things
float getDT();
std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;

//--I/O callbacks
void menu_options(int id);
void mouse_options(int button, int state, int x_pos, int y_pos);


/********
--MAIN--
********/

int main(int argc, char **argv)
{
    // Initialize glut
    glutInit(&argc, argv); // just initializes
    /* changes options...  
    GLUT_DOUBLE enables double buffering (drawing to a background buffer while another buffer is displayed), 
    GLUT_DEPTH bit mask to select a window with a depth buffer */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); 
    glutInitWindowSize(w, h);

    // Name and create the Window
    glutCreateWindow("Adding I/O");

    // Create menus
    // sub menu creation
    GLuint sub_menu;
    sub_menu = glutCreateMenu(menu_options);
    glutAddMenuEntry("start all movement", 1); // common sense
    glutAddMenuEntry("stop all movement", 2);

    GLuint direction_change;
    direction_change = glutCreateMenu(menu_options);
    glutAddMenuEntry("toggle planet orbit", 3); 
    glutAddMenuEntry("toggle planet rotation", 4);
    glutAddMenuEntry("toggle moon orbit", 5); 
    glutAddMenuEntry("toggle moon rotation", 6);

    // main menu creation
    glutCreateMenu(menu_options);
    glutAddSubMenu("start/stop", sub_menu);
    glutAddSubMenu("changing rotations & orbits", direction_change);
    glutAddMenuEntry("quit program", 7);
    glutAttachMenu(GLUT_RIGHT_BUTTON); // attached menu to right click

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
    glutSpecialFunc(special_keyboard);// Called for arrow keys

    glutMouseFunc(mouse_options); //Called if there is a mouse input

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

    //premultiply the matrix for this example
    mvp = projection * view * model;

    //enable the shader program
    glUseProgram(program);

    //upload the matrix to the shader
    glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));

    //set up the Vertex Buffer Object so it can be drawn
    glEnableVertexAttribArray(loc_position);
    glEnableVertexAttribArray(loc_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    
    //set pointers into the vbo for each of the attributes(position and color)
    glVertexAttribPointer( loc_position,//location of attribute
                           3,//number of elements
                           GL_FLOAT,//type
                           GL_FALSE,//normalized?
                           sizeof(Vertex),//stride
                           0);//offset

    glVertexAttribPointer( loc_color,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           sizeof(Vertex),
                           (void*)offsetof(Vertex,color));

    glDrawArrays(GL_TRIANGLES, 0, 36);//mode, starting index, count

    //second block created
    moon_model = glm::scale(moon_model, glm::vec3(.5));

    mvp = projection * view * moon_model;

    glUniformMatrix4fv(loc_mvpmat, 1, GL_FALSE, glm::value_ptr(mvp));

    glDrawArrays(GL_TRIANGLES, 0, 36);//mode, starting index, 

   //supposed to be for text but didn't work
   //unsigned char word [5] = "left";
   //glRasterPos2i(2, 2);
   //glColor4b(1.0f, 1.0f, 1.0f, 1.0f);
   //glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, word);

    //clean up
    glDisableVertexAttribArray(loc_position);
    glDisableVertexAttribArray(loc_color);
                           
    //swap the buffers
    glutSwapBuffers();
}

void update()
{
    //total time
    static float planet_orbit_angle = 0.0;
    static float planet_rotation_angle = 0.0;
    static float moon_orbit_angle = 0.0;
    static float moon_rotation_angle = 0.0;

    float dt = getDT();// if you have anything moving, use dt.

    // variables for the moon
    planet_orbit_angle += dt * (M_PI/2) * PLANET_ORBIT_DIRECTION; //move through 90 degrees a second
    planet_rotation_angle += dt * (M_PI/2) * PLANET_ROTAION_DIRECTION; 
    moon_orbit_angle += .01 * (M_PI/2) * MOON_ORBIT_DIRECTION; 
    moon_rotation_angle += dt * (M_PI/2) * MOON_ROTAION_DIRECTION; 

    model = glm::translate( glm::mat4(1.0f), glm::vec3(3.5 * sin(planet_orbit_angle), 0.0, 3.5 * cos(planet_orbit_angle)));

    model = glm::rotate(model, // what youre starting with
    planet_rotation_angle * 2, // angle of rotation (how much it rotates)
    glm::vec3(0.0,1.0,0.0)); // a three vector, axis

    // this controls the orbit of the "moon" around the orignal planet
    moon_model = glm::translate( model, glm::vec3(3.0 * sin(moon_orbit_angle), 0.0, 3.0 * cos(moon_orbit_angle)));

    // the rotation of the moon
    moon_model = glm::rotate(moon_model, moon_rotation_angle*2 , glm::vec3(0.0,1.0,0.0)); 

    // Update the state of the scene
    glutPostRedisplay();//call the display callback
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

bool initialize()
{
    // Initialize basic geometry and shaders for this example

    //this defines a cube, this is why a model loader is nice
    //you can also do this with a draw elements and indices, try to get that working
    Vertex geometry[] = { {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},
                          {{-1.0, -1.0, 1.0}, {0.0, 0.0, 1.0}},
                          {{-1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}},

                          {{1.0, 1.0, -1.0}, {1.0, 1.0, 0.0}},
                          {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},
                          {{-1.0, 1.0, -1.0}, {0.0, 1.0, 0.0}},
                          
                          {{1.0, -1.0, 1.0}, {1.0, 0.0, 1.0}},
                          {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},
                          {{1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}},
                          
                          {{1.0, 1.0, -1.0}, {1.0, 1.0, 0.0}},
                          {{1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}},
                          {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},

                          {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},
                          {{-1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}},
                          {{-1.0, 1.0, -1.0}, {0.0, 1.0, 0.0}},

                          {{1.0, -1.0, 1.0}, {1.0, 0.0, 1.0}},
                          {{-1.0, -1.0, 1.0}, {0.0, 0.0, 1.0}},
                          {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},

                          {{-1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}},
                          {{-1.0, -1.0, 1.0}, {0.0, 0.0, 1.0}},
                          {{1.0, -1.0, 1.0}, {1.0, 0.0, 1.0}},
                          
                          {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
                          {{1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}},
                          {{1.0, 1.0, -1.0}, {1.0, 1.0, 0.0}},

                          {{1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}},
                          {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
                          {{1.0, -1.0, 1.0}, {1.0, 0.0, 1.0}},

                          {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
                          {{1.0, 1.0, -1.0}, {1.0, 1.0, 0.0}},
                          {{-1.0, 1.0, -1.0}, {0.0, 1.0, 0.0}},

                          {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
                          {{-1.0, 1.0, -1.0}, {0.0, 1.0, 0.0}},
                          {{-1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}},

                          {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
                          {{-1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}},
                          {{1.0, -1.0, 1.0}, {1.0, 0.0, 1.0}}
                        };
    // Create a Vertex Buffer object to store this vertex info on the GPU
    glGenBuffers(1, &vbo_geometry); // 1st param-how many to create 2nd-address of array of GLuints
    glBindBuffer(GL_ARRAY_BUFFER, vbo_geometry);
    glBufferData(GL_ARRAY_BUFFER, sizeof(geometry), geometry, GL_STATIC_DRAW);

    //--Geometry done

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER); 
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    //Shader Sources

    // load shader info by calling function
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

    loc_color = glGetAttribLocation(program,
                    const_cast<const char*>("v_color"));
    if(loc_color == -1)
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
    view = glm::lookAt( glm::vec3(0.0, 8.0, -16.0), //Eye Position
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

void cleanUp()
{
    // Clean up, Clean up
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo_geometry);
}

//returns the time delta
float getDT()
{
   // check global "PAUSED" variable, if true make times equal
   if (PAUSED == true)
      t1 = std::chrono::high_resolution_clock::now();

   float ret;
   t2 = std::chrono::high_resolution_clock::now();
   ret = std::chrono::duration_cast< std::chrono::duration<float> >(t2-t1).count();
   t1 = std::chrono::high_resolution_clock::now();
   return ret;
}

void keyboard(unsigned char key, int x_pos, int y_pos)
{
    // Handle keyboard input
    if(key == 27)//ESC
       {
           exit(0);
       }

    if(key == 'r' || key == 'R')
      {
         // reverse spin 
         PLANET_ROTAION_DIRECTION *= -1;
      }
   if(key == ' ')
      {
         if (PAUSED == false)
            {
               PAUSED = true;
               t2 = t1;
            }
         else
            {
               PAUSED = false;
            }
      }
}

void special_keyboard(int key, int x_pos, int y_pos)
{
   switch(key)
      {
         case GLUT_KEY_LEFT:
         PLANET_ORBIT_DIRECTION = 1;
         break;

         case GLUT_KEY_RIGHT:
         PLANET_ORBIT_DIRECTION = -1;
         break;
      }
}

void menu_options(int id)
{
   // commands for corresponding selection
	switch(id)
		{
         case 1: // start rotation
            PAUSED = false;
            break;

         case 2: // stop rotation
            PAUSED = true;
            break;
         case 3:
            PLANET_ORBIT_DIRECTION *= -1;
            break;
         case 4: 
            PLANET_ROTAION_DIRECTION *= -1;
            break;
         case 5: 
            MOON_ORBIT_DIRECTION *= -1;
            break;
         case 6:
            MOON_ROTAION_DIRECTION *= -1;
            break;
         case 7:
            exit(0);
            break;
		}

   glutPostRedisplay(); // not sure what this does yet

}

void mouse_options(int button, int state, int x_pos, int y_pos)
{
   if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
         PLANET_ROTAION_DIRECTION *= -1;
}