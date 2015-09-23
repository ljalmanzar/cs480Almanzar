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
glm::mat4 model;//obj->world each object should have its own model matrix
glm::mat4 view;//world->eye
glm::mat4 projection;//eye->clip
glm::mat4 mvp;//premultiplied modelviewprojection

GLfloat ANGLE = 45;

string objFile; 

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

// count the verticies
int countVerticies(string fileName);
int countFaces(string fileName);

// Load vertex func 
/*returns the number of faces for the object
  needs file name for reading object file
  the vertex struct for the geometry*/
void readOBJ (string fileName, Vertex* theGeometry, int sizeOfVerticies);
void getColor(string fileName, string name, float* storage);

//I/O
void special_keyboard(int key, int x_pos, int y_pos);

/********
--MAIN--
********/

int main(int argc, char **argv)
{
    // Initialize glut
    glutInit(&argc, argv); // just initializes

    // get object file
    objFile = argv[1];

    /* changes options...  
    GLUT_DOUBLE enables double buffering (drawing to a background buffer while another buffer is displayed), 
    GLUT_DEPTH bit mask to select a window with a depth buffer */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); 
    glutInitWindowSize(w, h);

    // Name and create the Window
    glutCreateWindow("My Model Loader");

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

    glutSpecialFunc(special_keyboard);// Called for arrow keys

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

    // get how many verticies to draw
    int numOfVerticiesToDraw = countFaces(objFile);
    numOfVerticiesToDraw *= 3;
    glDrawArrays(GL_TRIANGLES, 0, numOfVerticiesToDraw);//mode, starting index, count

    //clean up
    glDisableVertexAttribArray(loc_position);
    glDisableVertexAttribArray(loc_color);
                           
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

bool initialize()
{
    // count the number of verticies
    int numOfVerticies = countVerticies(objFile);
    int numOfFaces = countFaces(objFile);

    std::cout << "Faces: " << numOfFaces << endl;
    std::cout << "Number of Verticies: " << numOfVerticies << endl;

    //this defines a cube, this is why a model loader is nice
    //you can also do this with a draw elements and indices, try to get that working
    Vertex geometry[numOfFaces*3];

    readOBJ(objFile, geometry, numOfVerticies);

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
   float ret;
   t2 = std::chrono::high_resolution_clock::now();
   ret = std::chrono::duration_cast< std::chrono::duration<float> >(t2-t1).count();
   t1 = std::chrono::high_resolution_clock::now();
   return ret;
}

int countVerticies(string fileName)
{
  string compareString;
  string garbageString;

  int vertexCount = 0;

  ifstream fin(fileName.c_str());

  if (!fin.good())
    {
      std::cout << "ERROR: Unable to open object file!" << endl;
    }

  fin >> compareString;
  while(fin.good())
    {
      if (compareString == "v")
        {
          vertexCount++;
          getline(fin, garbageString);
        }
      else
        {
          getline(fin, garbageString);
        }

      fin >> compareString;
    }
    
    return vertexCount;
}

int countFaces(string fileName)
{
  string compareString;
  string garbageString;

  int faceCount = 0;

  ifstream fin(fileName.c_str());

  if (!fin.good())
    {
      std::cout << "ERROR: Unable to open object file!" << endl;
    }

  fin >> compareString;
  while(fin.good())
    {
      if (compareString == "f")
        {
          faceCount++;
          getline(fin, garbageString);
        }
      else
        {
          getline(fin, garbageString);
        }

      fin >> compareString;
    }
    
    return faceCount;
}

void readOBJ (string fileName, Vertex* theGeometry, int sizeOfVerticies)
{
  // where to get the needed vertex from
  int vertexLocation;

  // for unnecessary file members
  char garbageStuff;
  string garbageString;

  // holds all the verticies for easy access
  Vertex vertexHolder[sizeOfVerticies];

  // for saving the x,y, or z
  GLfloat coordinate = 0;

  // for saving the material file
  string materialFile;

  // for saving name of material color
  string colorName;

  // for storing the current material color 
  float RGBs[] = {0,0,0};

  // to move through the geometry array
  int geometryNdx = 0;

  // to move through the temp array that saves all the coordinates
  int vertexCount = 0;

  // string to see if i need the line
  string compareString;

  // open file
  ifstream fin(fileName.c_str());

  if (!fin.good())
    {
      std::cout << "ERROR: Unable to open object file!" << endl;
    }

  // read in the front line char (prime the loop)
  fin >> compareString;
  while (fin.good())
    {
      // if it is a vertex, save it
      if (compareString == "v")
        {
          // save the three coordinates of the vertex into the vertex
          for(int i = 0; i<3; ++i)
            {
              fin >> coordinate;
              vertexHolder[vertexCount].position[i] = coordinate;
            } 

          vertexCount++;
        }
      // save the material file
      else if(compareString == "mtllib")
        {
          fin >> materialFile;
          materialFile = "../bin/" + materialFile;
        }

      else if(compareString == "usemtl")
        {
          fin >> colorName;
          getColor(materialFile, colorName, RGBs);

        }
      /* if it is a face, get the vertex from the "vertexHolder"
         save it in the geometry Vertex array */
      else if(compareString == "f")
        {
          // for the 3 verticies
          for (int i = 0; i < 3; ++i)
            {
              // grab which vertex & ignore the three chars after that
              fin >> vertexLocation;
              fin >> garbageStuff;
              fin >> garbageStuff;
              fin >> garbageStuff;

              for (int j = 0; j<3; ++j)
                { 
                  theGeometry[geometryNdx].position[j] = vertexHolder[vertexLocation-1].position[j];
                  theGeometry[geometryNdx].color[j] = RGBs[j];
                }

              geometryNdx++;
            }
        }
      // if not v or f get the line and move on
      else
        {
          getline(fin, garbageString);
        }

      // read in the front line char
      fin >> compareString;
    }
}

void getColor(string fileName, string name, float* storage)
{
  // for checking line
  string compareString;

  // to save the color in the file
  string nameOfColor;

  // to ignore unnecessary stuff in the file
  string garbageString;

  // open file
  ifstream fin(fileName.c_str());

  if (!fin.good())
    {
      std::cout << "ERROR: Unable to open material file!!!!";
    }

  // prime
  fin >> compareString;
  while(fin.good())
    { 
      // find the newmtl
      if (compareString == "newmtl")
        {
          // get color name
          fin >> nameOfColor;
          // see if colors match
          if (name == nameOfColor)
            {
              // find kd 
              fin >> compareString;
              while (compareString != "Kd")
                { 
                  getline(fin, garbageString); 
                  fin >> compareString;
                }

              // save RGB values
              for (int i = 0; i < 3; ++i)
                {
                  fin >> storage[i];
                }

              return;
            }
        }
      // move on
      else
        {
          getline(fin, garbageString);
        }

      // check next line  
      fin >> compareString;
    }
}

void special_keyboard(int key, int x_pos, int y_pos)
{
   switch(key)
      {
         case GLUT_KEY_LEFT:
         break;

         case GLUT_KEY_RIGHT:
         break;
      }
}