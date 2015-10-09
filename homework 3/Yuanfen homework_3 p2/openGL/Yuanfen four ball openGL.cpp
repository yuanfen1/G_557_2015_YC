//
//  openGL.cpp
//  HCI 557 homework 3
//
//  Created by Yuanfen Chen on 10/8/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "GLSphereDirect.h"
#include "GLSphereSpot.h"




using namespace std;


// The handle to the window object
GLFWwindow*         window;

// Define some of the global variables we're using for this sample
GLuint program;

/* A trackball to move and rotate the camera view */
extern Trackball trackball;



int main(int argc, const char * argv[])
{
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
    
    
    // Init the glew api
    initGlew();
    
    

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Create some models
    GLSpotLightSource spotlight;
	GLLightSource directlight;
	GLMaterial material;
    // coordinate system
    CoordSystem* cs = new CoordSystem(40.0);
    
    
    GLSphereDirect* sphere1 = new GLSphereDirect(-12.0, 0.0, 0.0, 3, 90, 50); //create sphere 1
	GLSphereDirect* sphere2 = new GLSphereDirect(-4, 0.0, 0.0, 3, 90, 50);    //create sphere 2
	GLSphereSpot* sphere3 = new GLSphereSpot(4.0, 0.0, 0.0, 3, 90, 50);      //create sphere 3
	GLSphereSpot* sphere4 = new GLSphereSpot(12.0, 0.0, 0.0, 3.0, 90, 50);     //create sphere 4

	// Sphere1 Red set the light position, intensity, and material 

	directlight._lightPos = glm::vec4(15.0,0.0,10.0, 0.0); //z was 0;15  y was 15
	directlight._ambient_intensity = 0.3;
    directlight._specular_intensity = 1.0;// was 0.5
    directlight._diffuse_intensity = 1.0; //was 1.0
    directlight._attenuation_coeff = 0.02;// was 0.02
	sphere1-> SetLightSource(directlight);

	material._diffuse_material = glm::vec3(0.7, 0.0, 0.0); // bright red in the front
    material._ambient_material = glm::vec3(0.5, 0.0, 0.0);//dark red in the back
    material._specular_material = glm::vec3(1.0, 1.0, 1.0);
    material._shininess = 70.0;//was 1;50;70
	material._emissive_material=glm::vec3(0.0,0.0,0.0);
	sphere1->set_material(material);
	sphere1->init();

	// Sphere2  blue  set the light position, intensity, and material 

	directlight._lightPos = glm::vec4(15,0.0,10.0, 0.0);//z was 0;15 y was 15
	directlight._ambient_intensity = 0.3;//was 0.3
    directlight._specular_intensity = 0.0;//was 0.5
    directlight._diffuse_intensity = 1.0;
    directlight._attenuation_coeff = 0.02;
	sphere2-> SetLightSource(directlight);

	material._diffuse_material = glm::vec3(0.0, 0.0, 0.65); // bright red in the front
    material._ambient_material = glm::vec3(0.0, 0.0, 0.2);//// dark red in the back;
    material._specular_material = glm::vec3(1.0, 1.0, 1.0);
    material._shininess = 1.0;
	sphere2->set_material(material);
	sphere2->init();

	// Sphere3  black  set the light position, intensity, and material 

	spotlight._lightPos = glm::vec4(4.0,0.0,9.0,1.0);//was (4.0,0.0,9.0,1.0)
	spotlight._ambient_intensity = 0.3;//was 0.3
    spotlight._specular_intensity = 1.5;// was 0.5;0.7;1.0
    spotlight._diffuse_intensity = 10.0;//was 1.0;2.0
    spotlight._attenuation_coeff = 0.01;//was 0.02; 0.2;0.1;0.01;0.005  0.01
    spotlight._cone_angle = 22.0; // in degree was 90;10;80;30;50;35
	spotlight._smooth_angle = 23.0; // 1 degree smooth angle to get rid of the zipzap edge effect
    spotlight._cone_direction = glm::vec3(0.0, 1.0,-5.0); // was (-1, 1.8,-9) (0.0, 1.0,-5.0)
	sphere3-> SetSpotLightSource(spotlight);

	material._diffuse_material = glm::vec3(0.0, 1.0, 0.0); //(0.5, 1.0, 0.0); green spotlight
    material._ambient_material = glm::vec3(0.0, 0.0, 0.0);//dark ambient
    material._specular_material = glm::vec3(1.0, 1.0, 1.0);
    material._shininess = 40.0;//was 1
	sphere3->set_material(material);
	sphere3->init();


	// Sphere4  milk  set the light position, intensity, and material 

	spotlight._lightPos = glm::vec4(12,0.0,9.0, 1.0); //was (12,0.0,9.0, 1.0)
	spotlight._ambient_intensity = 0.5;
    spotlight._specular_intensity = 0.25;
    spotlight._diffuse_intensity = 0.9;
    spotlight._attenuation_coeff = 0.01;
    spotlight._cone_angle = 22.0; // in degree
    spotlight._cone_direction = glm::vec3(0.0, 1.0, -5); // this must be aligned with the object and light position.
	spotlight._smooth_angle = 28.0; // smooth angle in degree
	sphere4-> SetSpotLightSource(spotlight);

	material._diffuse_material = glm::vec3(1.0, 1.0, 0.2); //(0.5, 1.0, 0.0);
    material._ambient_material = glm::vec3(0.953, 0.945, 0.4);//(0.5, 0.5, 0.0);
    material._specular_material = glm::vec3(1.0, 1.0, 1.0);
    material._shininess = 40.0;
	sphere4->set_material(material);
	sphere4->init();

    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector. 
    SetViewAsLookAt(glm::vec3(15.6f, 15.6f, 20.15f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //move the camera far way to reduce the egg shape
    
    
    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    //sphere->enableNormalVectorRenderer();
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
    
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects
        
        // Set the trackball locatiom
        SetTrackballLocation(trackball.getRotationMatrix());
        
        // draw the objects
        cs->draw();
        
        sphere1->draw();
		sphere2->draw();
		sphere3->draw();
		sphere4->draw();
        
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    delete cs;


}

