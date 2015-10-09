//
//  GLSphereExt.hpp
//  HCI557_Spotlight
//
//  Created by Rafael Radkowski on 9/26/15.
//
//
#pragma once


// stl include
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// locals
#include "GLObject.h"
#include "GLSphere.h"
#include "Shaders.h"

#include "HCI557Datatypes.h"





class GLSphereDirect : public GLSphere
{
public:
    
    GLSphereDirect(float center_x, float center_y, float center_z, float radius, int rows = 10, int segments = 10 );
    ~GLSphereDirect();
    void SetLightSource(GLLightSource& lightsource)
	{
		// define the position of the light and send the light position to your shader program
		_light_source1._lightPos = lightsource._lightPos;//glm::vec4(20.0,20.0,0.0, 1.0);
		_light_source1._ambient_intensity = lightsource._ambient_intensity;//0.3;
		_light_source1._specular_intensity = lightsource._specular_intensity;//5.0;
		_light_source1._diffuse_intensity = lightsource._diffuse_intensity;//8.0;
		_light_source1._attenuation_coeff = lightsource._attenuation_coeff;//0.02;
	}
    
protected:
    
    
    
    /*
     Inits the shader program for this object
     */
    virtual void initShader(void);
    
    
    // The spotlight object
    GLLightSource           _light_source1;
    


};