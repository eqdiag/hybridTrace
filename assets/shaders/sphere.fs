#version 330 core

in vec3 Position;
in vec3 Normal;

out vec4 outColor;


uniform vec3 eye;
uniform samplerCube skybox;


void main()
{
	vec3 eyeDir = normalize(Position - eye);
	vec3 reflectDir = reflect(eyeDir,Normal);
	vec3 normalColor = (Normal + vec3(1.0))*0.5;
	outColor = texture(skybox, reflectDir);
	//outColor = vec4(normalColor,1.0);
}