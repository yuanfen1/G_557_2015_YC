#version 410 core                                                 

// Transformations for the projections
uniform mat4 projectionMatrixBox;                                    
uniform mat4 viewMatrixBox;                                          
uniform mat4 modelMatrixBox;     
uniform struct Material
{
	// The material parameters 
	vec3 diffuse_color;                                        
	vec3 ambient_color;                                         
	vec3 specular_color; 
	float shininess;  
	
} allMaterials[1];
uniform struct Light
{
	// Position of the light source
	vec4 light_position;   

	// The intensity values for the reflection equations
	float diffuse_intensity;                                          
	float ambient_intensity;                                        
	float specular_intensity;                                       
	float attenuationCoefficient;
	float cone_angle;
	vec3  cone_direction;
} allLights[2];                                    
                                             
// The vertex buffer input                                    
in vec3 in_Color; 
in vec3 in_Position; 
in vec3 in_Normal;  

// The output color
out vec3 pass_Color;                                            
                                                                 
                                                               
                                                                                                                               
void main(void)                                                 
{                                                               
    vec3 normal = normalize(in_Normal);                                                                   
    vec4 transformedNormal =  normalize(transpose(inverse(modelMatrixBox)) * vec4( normal, 1.0 ));            
    vec4 surfacePostion = modelMatrixBox * vec4(in_Position, 1.0);                          
    
    vec4 surface_to_light[2];                                                                                                        
    surface_to_light[0] =   normalize( allLights[0].light_position -  surfacePostion );  
	surface_to_light[1] =   normalize( allLights[1].light_position -  surfacePostion );                     
                                                                                                            
    // Diffuse color
	float diffuse_coefficient[2];                                                                                          
    diffuse_coefficient[0] = max( dot(transformedNormal, surface_to_light[0]), 0.0);  
	diffuse_coefficient[1] = max( dot(transformedNormal, surface_to_light[1]), 0.0);                        
    vec3 out_diffuse_color[2];
	out_diffuse_color[0] = allMaterials[0].diffuse_color  * diffuse_coefficient[0] * allLights[0].diffuse_intensity;   
	out_diffuse_color[1] = allMaterials[0].diffuse_color  * diffuse_coefficient[1] * allLights[1].diffuse_intensity;                        
                                                                                                              
    // Ambient color                                                                                         
    vec3 out_ambient_color[2];
	out_ambient_color[0] = vec3(allMaterials[0].ambient_color) * allLights[0].ambient_intensity;     
	out_ambient_color[1] = vec3(allMaterials[0].ambient_color) * allLights[1].ambient_intensity;                                    
                                                                                                             
    // Specular color                                                                                        
    vec3 incidenceVector[2];
	incidenceVector[0] = -surface_to_light[0].xyz;   
	incidenceVector[1] = -surface_to_light[1].xyz;                                                           
    vec3 reflectionVector[2];
	reflectionVector[0] = reflect(incidenceVector[0], transformedNormal.xyz); 
	reflectionVector[1] = reflect(incidenceVector[1], transformedNormal.xyz);  
	                                
    vec3 cameraPosition = vec3( -viewMatrixBox[3][0], -viewMatrixBox[3][1], -viewMatrixBox[3][2]); 
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePostion.xyz); 
	                                  
    float cosAngle[2];
	cosAngle[0] = max( dot(surfaceToCamera, reflectionVector[0]), 0.0); 
	cosAngle[1] = max( dot(surfaceToCamera, reflectionVector[1]), 0.0);                                     
    float specular_coefficient[2];
	specular_coefficient[0] = pow(cosAngle[0], allMaterials[0].shininess);  
	specular_coefficient[1] = pow(cosAngle[1], allMaterials[0].shininess);                                                  
    vec3 out_specular_color[2];
	out_specular_color[0] = allMaterials[0].specular_color * specular_coefficient[0] * allLights[0].specular_intensity; 
	out_specular_color[1] = allMaterials[0].specular_color * specular_coefficient[1] * allLights[1].specular_intensity;                  
        
	//attenuation
    float distanceToLight[2];
	distanceToLight[0] = length(allLights[0].light_position.xyz - surfacePostion.xyz);
	distanceToLight[1] = length(allLights[1].light_position.xyz - surfacePostion.xyz);
    float attenuation[2];
	attenuation[0] = 1.0 / (1.0 + allLights[0].attenuationCoefficient * pow(distanceToLight[0], 2));
	attenuation[1] = 1.0 / (1.0 + allLights[1].attenuationCoefficient * pow(distanceToLight[1], 2));
	
	//////////////////////////////////////////////////////////////////////////////////////////////        
    // Directional light
    //
	for( int i = 0; i < 2; i++)
	{
		if(allLights[i].light_position.w == 0.0) {
 			// this is a directional light.

			// 1. the values that we store as light position is our light direction.
  			vec3 light_direction = normalize(allLights[i].light_position.xyz);
  		
  			// 2. We check the angle of our light to make sure that only parts towards our light get illuminated
  			float light_to_surface_angle = dot(light_direction, transformedNormal.xyz);
  		
  			// 3. Check the angle, if the angle is smaller than 0.0, the surface is not directed towards the light. 
  			if(light_to_surface_angle > 0.0)attenuation[i] = 1.0;
  			else attenuation[i] = 0.0;	
		} 
			// Spotlight

		else {
	      
					// 1. Normalize the cone direction
					 vec3 cone_direction_norm = normalize(allLights[i].cone_direction); 
    
					// 2. Calculate the ray direction. We already calculated the surface to light direction.
					// 	  All what we need to do is to inverse this value
					 vec3 ray_direction = -surface_to_light[i].xyz;   
    
					 // 3. Calculate the angle between light and surface using the dot product again. 
					//    To simplify our understanding, we use the degrees
					 float light_to_surface_angle = degrees(acos(dot(ray_direction, cone_direction_norm))) ; 
    
					 // 4. Last, we compare the angle with the current direction and 
					 //    reduce the attenuation to 0.0 if the light is outside the angle. 
						if(light_to_surface_angle > allLights[i].cone_angle){
  						attenuation[i] = 0.0;
					}
				 
	
		}
	}

	// Writes the color
	pass_Color = vec3(out_diffuse_color[0] + attenuation[0]*(out_ambient_color[0] + out_specular_color[0])) 
				+ vec3(out_diffuse_color[1] +attenuation[1] *( out_ambient_color[1] + out_specular_color[1]));  
	
	
	// Passes the projected position to the fragment shader / rasterization process. 
    gl_Position = projectionMatrixBox * viewMatrixBox * modelMatrixBox * vec4(in_Position, 1.0);             
                                                                                                             
                          
}                                                                                                            