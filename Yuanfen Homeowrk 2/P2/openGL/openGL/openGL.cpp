//
//  main.cpp
//  OpenGL4Test
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//
#include "stdafx.h"
// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"

float* vertices;
float *colors;


float vertices_of_obj[19][3] = {
	{0.0f, 0.0f, 0.0f},  //1
	{3.0f, 0.0f, 0.0f},
	{3.0f, 1.0f, 0.0f},
	{1.0f, 2.0f, 0.0f},
	{0.0f, 2.0f, 0.0f},  //5
	{0.0f, 2.0f, 1.0f},
	{0.0f, 0.0f, 1.0f},
	{3.0f, 0.0f, 1.0f},
	{3.0f, 1.0f, 1.0f},
	{1.0f, 2.0f, 1.0f},  //10
	{1.0f, 1.0f, 1.0f},
	{0.0f, 1.0f, 1.0f},
	{0.0f, 0.0f, 1.0f},
	{1.0f, 0.0f, 1.0f},
	{1.0f, 0.0f, 3.0f},  //15
	{1.0f, 1.0f, 3.0f},
	{0.0f, 1.0f, 3.0f},
	{0.0f, 0.0f, 3.0f},  //18
	{0.0f, 1.0f, 0.0f}, //19 actually there is only 18 points, the 7 and 13 is the same````
};
float color_blue[3] = {0.0f, 0.0f, 1.0f};


using namespace std;

static const string vs_string =
    "#version 410 core                                                 \n"
    "                                                                   \n"
    "uniform mat4 projectionMatrix;                                    \n"
    "uniform mat4 viewMatrix;                                           \n"
    "uniform mat4 modelMatrix;                                          \n"
    "in vec3 in_Position;                                               \n"
    "                                                                   \n"
    "in vec3 in_Color;                                                  \n"
    "out vec3 pass_Color;                                               \n"
    "                                                                  \n"
    "void main(void)                                                   \n"
    "{                                                                 \n"
    "    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
    "    pass_Color = in_Color;                                         \n"
    "}                                                                 \n";

// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
static const string fs_string  =
    "#version 410 core                                                 \n"
    "                                                                  \n"
    "in vec3 pass_Color;                                                 \n"
    "out vec4 color;                                                    \n"
    "void main(void)                                                   \n"
    "{                                                                 \n"
    "    color = vec4(pass_Color, 1.0);                               \n"
    "}                                                                 \n";


glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix; // Store the view matrix
glm::mat4 modelMatrix; // Store the model matrix


// The handle to the window object
GLFWwindow*         window;


// Define some of the global variables we're using for this sample
GLuint program;
GLuint vao;
unsigned int vbaID[2];
//unsigned int vaoID[2]; // Our Vertex Array Object
unsigned int vboID[4]; // Our Vertex Buffer Object


// This is the callback we'll be registering with GLFW for errors.
// It'll just print out the error to the STDERR stream.
void error_callback(int error, const char* description) {
    fputs(description, stderr);
}


// This is the callback we'll be registering with GLFW for keyboard handling.
// The only thing we're doing here is setting up the window to close when we press ESC
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

inline void assign_vec3f(float* array_, int id_, float a, float b, float c)
{
	array_[id_*3] = a;
	array_[id_*3+1] = b;
	array_[id_*3+2] = c;
}
inline void assign_vec3f(float* array_, int id_, float* vec3f)
{
	array_[id_*3] = vec3f[0];
	array_[id_*3+1] = vec3f[1];
	array_[id_*3+2] = vec3f[2];
	//std::cout<<vec3f[0]<<","<<vec3f[1]<<","<<vec3f[2]<<std::endl;
}

unsigned int createTriangleStripModel(void)
{
    float* vertices = new float[99];  // Vertices for our square
    float *colors = new float[99]; // Colors for our vertices

	//Top sqaure
	//Top sqaure TOP
	vertices[0] = 0; vertices[1] = 0; vertices[2] = 3; // Top right  corner
	colors[0] = 0.0; colors[1] = 0.0; colors[2] = 1.0; // Top left corner

	vertices[3] = 1; vertices[4] = 1; vertices[5] = 3; // Bottom right  corner
	colors[3] = 0.0; colors[4] = 0.0; colors[5] = 1.0; // Bottom left corner

	vertices[6] = 0; vertices[7] = 1; vertices[8] = 3; // Top left  corner
	colors[6] = 0.0; colors[7] = 0.0; colors[8] = 1.0; // Top left corner

	vertices[9] = 1; vertices[10] = 0; vertices[11] = 3; // Bottom left  corner
	colors[9] = 0.0; colors[10] = 0.0; colors[11] = 1.0; // Bottom left corner
    
    // Top sqaure FRONT
 
    vertices[12] = 1; vertices[13] = 0; vertices[14] = 1; // Bottom left corner
    colors[12] = 0.0; colors[13] = 0.0; colors[14] = 1.0; // Bottom left corner
    
    vertices[15] = 1; vertices[16] = 1; vertices[17] = 3; // Top Right corner
    colors[15] = 0.0; colors[16] = 0.0; colors[17] = 1.0; // Top Right corner
    
    vertices[18] = 1; vertices[19] = 1; vertices[20] = 1; // Bottom right corner
    colors[18] = 0.0; colors[19] = 0.0; colors[20] = 1.0; // Bottom right corner
    
    // Top sqaure RIGHT SIDE
    vertices[21] = 0; vertices[22] = 1; vertices[23] = 3; // Top right corner
    colors[21] = 0.0; colors[22] = 0.0; colors[23] = 1.0; // Top right corner
    
    vertices[24] = 0; vertices[25] = 1; vertices[26] = 1; // Bottom right corner
    colors[24] = 0.0; colors[25] = 0.0; colors[26] = 1.0; // Bottom right corner
    
    // Top sqaure BACK
    vertices[27] = 0; vertices[28] = 0; vertices[29] = 3; // Top left  corner
    colors[27] = 0.0; colors[28] = 0.0; colors[29] = 1.0; // Top left corner
    
    vertices[30] = 0; vertices[31] = 0; vertices[32] = 1; // Bottom left  corner
    colors[30] = 0.0; colors[31] = 0.0; colors[32] = 1.0; // Bottom left corner
    
    // Top sqaure LEFT
    vertices[33] = 1; vertices[34] = 0; vertices[35] = 3; // Top left  corner
    colors[33] = 0.0; colors[34] = 0.0; colors[35] = 1.0; // Top left corner
    
    vertices[36] = 1; vertices[37] = 0; vertices[38] = 1; // Bottom left  corner
    colors[36] = 0.0; colors[37] = 0.0; colors[38] = 1.0; // Bottom left corner



	//Bottom pentagon
	//Bottom pentagon  top
	vertices[39] = 0; vertices[40] = 0; vertices[41] = 1; 
	colors[39] = 0.0; colors[40] = 0.0; colors[41] = 1.0; 

	vertices[42] = 0; vertices[43] = 2; vertices[44] = 1; 
	colors[42] = 0.0; colors[43] = 0.0; colors[44] = 1.0; 

	vertices[45] = 3; vertices[46] = 0; vertices[47] = 1; 
    colors[45] = 0.0; colors[46] = 0.0; colors[47] = 1.0; 

	vertices[48] = 1; vertices[49] = 2; vertices[50] = 1; 
	colors[48] = 0.0; colors[49] = 0.0; colors[50] = 1.0; 

	vertices[51] = 3; vertices[52] = 1; vertices[53] = 1; 
	colors[51] = 0.0; colors[52] = 0.0; colors[53] = 1.0; 


    //Bottom pentagon  side 1												

	vertices[54] = 3; vertices[55] = 1; vertices[56] = 0; 
	colors[54] = 0.0; colors[55] = 0.0; colors[56] = 1.0; 

	vertices[57] = 1; vertices[58] = 2; vertices[59] = 1; 
	colors[57] = 0.0; colors[58] = 0.0; colors[59] = 1.0; 

	vertices[60] = 1; vertices[61] = 2; vertices[62] = 0; 
	colors[60] = 0.0; colors[61] = 0.0; colors[62] = 1.0; 

	//Bottom pentagon  side 2												  
	vertices[63] = 0; vertices[64] = 2; vertices[65] = 1; 
	colors[63] = 0.0; colors[64] = 0.0; colors[65] = 1.0; 

	vertices[66] = 0; vertices[67] = 2; vertices[68] = 0; 
	colors[66] = 0.0; colors[67] = 0.0; colors[68] = 1.0; 

 // Bottom pentagon  side 3
	vertices[69] = 0; vertices[70] = 0; vertices[71] = 1; 
	colors[69] = 0.0; colors[70] = 0.0; colors[71] = 1.0; 

	vertices[72] = 0; vertices[73] = 0; vertices[74] = 0; 
	colors[72] = 0.0; colors[73] = 0.0; colors[74] = 1.0; 

//Bottom pentagon  side 4								
	vertices[75] = 3; vertices[76] = 0; vertices[77] = 1; 
	colors[75] = 0.0; colors[76] = 0.0; colors[77] = 1.0; 

	vertices[78] = 3; vertices[79] = 0; vertices[80] = 0; 
	colors[78] = 0.0; colors[79] = 0.0; colors[80] = 1.0; 

//Bottom pentagon  side 5
	vertices[81] = 3; vertices[82] = 1; vertices[83] = 1; 
	colors[81] = 0.0; colors[82] = 0.0; colors[83] = 1.0; 

	vertices[84] = 3; vertices[85] = 1; vertices[86] = 0; 
	colors[84] = 0.0; colors[85] = 0.0; colors[86] = 1.0; 

//Bottom pentagon  bottom

	vertices[87] = 3; vertices[88] = 0; vertices[89] = 0; 
	colors[87] = 0.0; colors[88] = 0.0; colors[89] = 1.0; 

	vertices[90] = 1; vertices[91] = 2; vertices[92] = 0; 
	colors[90] = 0.0; colors[91] = 0.0; colors[92] = 1.0; 

	vertices[93] = 0; vertices[94] = 0; vertices[95] = 0; 
    colors[93] = 0.0; colors[94] = 0.0; colors[95] = 1.0; 

	vertices[96] = 0; vertices[97] = 2; vertices[98] = 0; 
	colors[96] = 0.0; colors[97] = 0.0; colors[98] = 1.0; 

	

    //glGenVertexArrays(2, &vaoID[0]); // Create our Vertex Array Object
    glBindVertexArray(vbaID[0]); // Bind our Vertex Array Object so we can use it
     
    
    glGenBuffers(2, vboID); // Generate our Vertex Buffer Object
    
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, 99 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
  
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(0); // Disable our Vertex Array Object
    
    
    //Color
    glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, 99 * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(1); // Enable the second vertex attribute array
    
    glBindVertexArray(0); // Disable our Vertex Buffer Object
    
    
    delete [] vertices; // Delete our vertices from memory

	return 33;
}

unsigned int createMyModel(void)
{
	float* vertices = new float[120];  // Vertices for our square
    float *colors = new float[120]; // Colors for our vertices

	for(int i=0; i<40; i++)
		assign_vec3f(colors, i, color_blue);

	//Top sqaure
	//Top sqaure TOP, front, and right
	assign_vec3f(vertices, 0, vertices_of_obj[14]); // Top left  corner
	
	assign_vec3f(vertices, 1, vertices_of_obj[13]); // Bottom left  corner
	
	assign_vec3f(vertices, 2, vertices_of_obj[12]);

	assign_vec3f(vertices, 3, vertices_of_obj[17]);

	assign_vec3f(vertices, 4, vertices_of_obj[16]);

	assign_vec3f(vertices, 5, vertices_of_obj[15]);

	assign_vec3f(vertices, 6, vertices_of_obj[10]);

	assign_vec3f(vertices, 7, vertices_of_obj[13]);

    
    // Top back triangle strip
 
    assign_vec3f(vertices, 8, vertices_of_obj[17]); // top right corner
    
    
    assign_vec3f(vertices, 9, vertices_of_obj[12]); // bottom right corner
    
    
    assign_vec3f(vertices, 10, vertices_of_obj[16]); // top left corner
   

    assign_vec3f(vertices, 11, vertices_of_obj[11]); // topbottom

    
    // Top sqaure LEFT
    assign_vec3f(vertices, 12, vertices_of_obj[16]); // Top right  corner
    
    assign_vec3f(vertices, 13, vertices_of_obj[15]); // Bottom right  corner
    
	 assign_vec3f(vertices, 14, vertices_of_obj[11]); // Top right  corner

	 assign_vec3f(vertices, 15, vertices_of_obj[10]); // Top right  corner

	//Bottom pentagon
	//Bottom pentagon  top and tow sides  triangle fan
	assign_vec3f(vertices, 16, vertices_of_obj[6]);
	//colors[39] = 0.0; colors[40] = 0.0; colors[41] = 1.0; 

	assign_vec3f(vertices, 17, vertices_of_obj[0]);
	//colors[42] = 0.0; colors[43] = 0.0; colors[44] = 1.0; 

	assign_vec3f(vertices, 18, vertices_of_obj[1]);
   // colors[45] = 0.0; colors[46] = 0.0; colors[47] = 1.0; 

	assign_vec3f(vertices, 19, vertices_of_obj[7]);
	//colors[48] = 0.0; colors[49] = 0.0; colors[50] = 1.0; 

	assign_vec3f(vertices, 20, vertices_of_obj[8]); 
	//colors[51] = 0.0; colors[52] = 0.0; colors[53] = 1.0; 

	assign_vec3f(vertices, 21, vertices_of_obj[9]);

	assign_vec3f(vertices, 22, vertices_of_obj[5]);

	assign_vec3f(vertices, 23, vertices_of_obj[4]);

	assign_vec3f(vertices, 24, vertices_of_obj[0]);


	//Bottom pentagon  bottom and one sides  triangle fan
	assign_vec3f(vertices, 25, vertices_of_obj[3]);
	//colors[39] = 0.0; colors[40] = 0.0; colors[41] = 1.0; 

	assign_vec3f(vertices, 26, vertices_of_obj[9]);
	//colors[42] = 0.0; colors[43] = 0.0; colors[44] = 1.0; 

	assign_vec3f(vertices, 27, vertices_of_obj[5]);
   // colors[45] = 0.0; colors[46] = 0.0; colors[47] = 1.0; 

	assign_vec3f(vertices, 28, vertices_of_obj[4]);
	//colors[48] = 0.0; colors[49] = 0.0; colors[50] = 1.0; 

	assign_vec3f(vertices, 29, vertices_of_obj[0]); 
	//colors[51] = 0.0; colors[52] = 0.0; colors[53] = 1.0; 

	assign_vec3f(vertices, 30, vertices_of_obj[1]);

	assign_vec3f(vertices, 31, vertices_of_obj[2]);

	//Bottom pentagon  side 4
    assign_vec3f(vertices, 32, vertices_of_obj[7]); // Top right  corner
    
    assign_vec3f(vertices, 33, vertices_of_obj[1]); // Bottom right  corner
    
	 assign_vec3f(vertices, 34, vertices_of_obj[8]); // Top right  corner

	 assign_vec3f(vertices, 35, vertices_of_obj[2]); // Top right  corner

	 //Bottom pentagon  side 5
    assign_vec3f(vertices, 36, vertices_of_obj[8]); // Top right  corner
    
    assign_vec3f(vertices, 37, vertices_of_obj[9]); // Bottom right  corner
    
	 assign_vec3f(vertices, 38, vertices_of_obj[2]); // Top right  corner

	 assign_vec3f(vertices, 39, vertices_of_obj[3]); // Top right  corner

   



	
    //glGenVertexArrays(2, &vaoID[1]); // Create our Vertex Array Object
    glBindVertexArray(vbaID[1]); // Bind our Vertex Array Object so we can use it
     
    
    glGenBuffers(2, &vboID[2]); // Generate our Vertex Buffer Object
    
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, vboID[2]); // Bind our Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, 120 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
  
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(0); // Disable our Vertex Array Object
    
    
    //Color
    glBindBuffer(GL_ARRAY_BUFFER, vboID[3]); // Bind our second Vertex Buffer Object
    glBufferData(GL_ARRAY_BUFFER, 120 * sizeof(GLfloat), colors, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW
    
    glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
    glEnableVertexAttribArray(1); // Enable the second vertex attribute array
    
    glBindVertexArray(0); // Disable our Vertex Buffer Object
    
    
    delete [] vertices; // Delete our vertices from memory

	return 33;
}

/*!
 This function creates the two models
 */
void setupScene(void) {
    glGenVertexArrays(2, &vbaID[0]); // Create our Vertex Array Object
    createTriangleStripModel();
    createMyModel();
}

/*!
 ADD YOUR CODE TO RENDER THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
void renderTriangleStripModel(void)
{
	// Bind the buffer and switch it to an active buffer
    glBindVertexArray(vbaID[0]);
	// Draw the triangles
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 33);
    // Unbind our Vertex Array Object
    glBindVertexArray(0);
}

/*!
 ADD YOUR CODE TO RENDER YOUR MODEL TO THIS FUNCTION
 */
void renderMyModel(void)
{
    // Bind the buffer and switch it to an active buffer
    glBindVertexArray(vbaID[1]);
	// Draw the triangles fan and triangles
    glDrawArrays(GL_TRIANGLE_FAN, 0, 8);

	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);

	glDrawArrays(GL_TRIANGLE_FAN, 16, 9);

	glDrawArrays(GL_TRIANGLE_FAN, 25, 7);

	glDrawArrays(GL_TRIANGLE_STRIP, 32, 4);

	glDrawArrays(GL_TRIANGLE_STRIP, 36, 4);

    // Unbind our Vertex Array Object
    glBindVertexArray(0);
}



GLFWwindow* initWindow(void)
{
    // The handle to the window object
    GLFWwindow*         window = NULL;
    
    // Initialize GLFW, and if it fails to initialize for any reason, print it out to STDERR.
    if (!glfwInit()) {
        fprintf(stderr, "Failed initialize GLFW.");
        exit(EXIT_FAILURE);
    }
    
    // Set the error callback, as mentioned above.
    glfwSetErrorCallback(error_callback);
    
    // Set up OpenGL options.
    // Use GLFW verion 3.3,
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    // Create a window to put our stuff in.
    window = glfwCreateWindow(800, 600, "Homework 2", NULL, NULL);
    
    // If the window fails to be created, print out the error, clean up GLFW and exit the program.
    if( window == NULL ){
        cout <<  "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible." << endl;
        glfwTerminate();
        system("pause");
        exit(-1);
    }
    
    // Use the window as the current context (everything that's drawn will be place in this window).
    glfwMakeContextCurrent(window);
    
    
    // Set the keyboard callback so that when we press ESC, it knows what to do.
    glfwSetKeyCallback(window, key_callback);
    
    return window;
}


/*!
 This function initializes glew
 */
bool initGlew(void)
{
    
    // Makes sure all extensions will be exposed in GLEW and initialize GLEW.
    glewExperimental = GL_TRUE;
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        cout <<  "Failed to initialize GLEW\n" << endl;
        system("pause");
        return false;
    }
    
    cout <<  "OpenGL version supported by this platform " << glGetString(GL_VERSION) <<  endl;
	return true;
}

/*!
 This function updates the virtual camera
 */
bool updateCamera()
{
    // Compute the MVP matrix from keyboard and mouse input
    computeMatricesFromInputs();
    viewMatrix = getViewMatrix(); // get the current view matrix
    
    return true;
}



int _tmain(int argc, const char * argv[])
{
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
    
    
    // Init the glew api
    initGlew();
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// The Shader Program starts here
    
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
    static const string vertex_code = vs_string;
    static const char * vs_source = vertex_code.c_str();
    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    static const string fragment_code = fs_string;
    static const char * fs_source = fragment_code.c_str();
    
    // This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
    program = glCreateProgram();
    
    // We create a shader with our fragment shader source code and compile it.
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    
    // We create a shader with our vertex shader source code and compile it.
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    
    // We'll attach our two compiled shaders to the OpenGL program.
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    glLinkProgram(program);
    
    // We'll specify that we want to use this program that we've attached the shaders to.
    glUseProgram(program);
    
    //// The Shader Program ends here
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    /// IGNORE THE NEXT PART OF THIS CODE
    /// IGNORE THE NEXT PART OF THIS CODE
    /// IGNORE THE NEXT PART OF THIS CODE
    // It controls the virtual camera
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    
    projectionMatrix = glm::perspective(1.1f, (float)800 / (float)600, 0.1f, 100.f);
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    viewMatrix = glm::lookAt(glm::vec3(1.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    int projectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
    int viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
    int modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader
    
    
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
    
    
    glBindAttribLocation(program, 0, "in_Position");
    glBindAttribLocation(program, 1, "in_Color");
    
    //// The Shader Program ends here
    //// START TO READ AGAIN
    //// START TO READ AGAIN
    //// START TO READ AGAIN
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    // this creates the scene
    setupScene();
    
    

    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        
        // update the virtual camera
        // ignore this line since we did not introduced cameras.
        updateCamera();
        
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This generate the object
        // Enable the shader program
        glUseProgram(program);
        
        // this changes the camera location
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // send the view matrix to our shader
        
        // This moves the model to the right
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
        
        renderTriangleStripModel();
        
        // This moves the model to the left
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, -0.0f, 0.0f));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
        
        renderMyModel();
        
        
        glUseProgram(0);
        //// This generate the object
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    // Program clean up when the window gets closed.
    glDeleteVertexArrays(2, vbaID);
    glDeleteProgram(program);

	return 0;
}