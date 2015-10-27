#version 410 core                                                 

uniform sampler2D texture_background; //this is the texture
uniform sampler2D texture_foreground; //this is the texture
uniform sampler2D texture_middle; //this is the texture

in vec2 pass_TexCoord; //this is the texture coord
in vec4 pass_Color;
out vec4 color;

uniform int texture_blend;

void main(void)                                                   
{
    // This function finds the color component for each texture coordinate. 
    vec4 tex_landscape =  texture(texture_background, pass_TexCoord);
    
    vec4 tex_gradient =  texture(texture_foreground, pass_TexCoord);

    vec4 tex_penguin =  texture(texture_middle, pass_TexCoord);
    
    // This mixes the background color with the texture color.
    // The GLSL shader code replaces the former envrionment. It is now up to us
    // to figure out how we like to blend a texture with the background color.
    if(texture_blend == 0)
    {
        //color =  0.1 * pass_Color + tex_gradient + tex_landscape + tex_penguin;
    	color =  0.1 * pass_Color + 0.5*tex_gradient+0.6*tex_landscape +0.7*tex_penguin;
	//color =  pass_Color + 0.5*tex_gradient+0.6*tex_landscape +0.7*tex_penguin;
    }
    else if(texture_blend == 1)
    {
       //color = (tex_gradient*0.5)*tex_landscape + tex_penguin;
 //color = tex_landscape * tex_gradient * tex_penguin;
//color =pass_Color*0.1 + tex_landscape * tex_gradient * tex_penguin;
color =pass_Color*0.1 + tex_landscape * tex_gradient * tex_penguin +0.4* tex_gradient.b - 0.4*tex_gradient.r;
    }
    else if(texture_blend == 2)
    {
        color = (tex_gradient.g)*tex_landscape * tex_penguin;
    }
    else
    {
        color =0.1 * pass_Color + tex_landscape + tex_penguin;
    }
    
}