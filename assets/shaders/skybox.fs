#version 330 core


in vec3 texCoords;

out vec4 outColor;

uniform samplerCube skybox;
uniform bool skyToggle;


void main()
{    
    if(skyToggle){
        outColor = texture(skybox, texCoords);
    }else{
        outColor = vec4(0.0);
    }
    //outColor = vec4(1.0,0.0,0.0,0.0);
}