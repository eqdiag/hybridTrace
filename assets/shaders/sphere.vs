#version 330 core

in vec3 position;
in vec3 normal;

out vec3 Position;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){
	Position = (model * vec4(position,1.0)).xyz;
	Normal = normalize((inverse(transpose(model)) * vec4(normal,0.0)).xyz);
	gl_Position = proj * view * model * vec4(position,1.0);
}