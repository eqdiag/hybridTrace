#version 330 core


const vec2 vertices[3] = vec2[]
(
	vec2(-1.0f,-1.0f),
	vec2(3.0f,-1.0f),
	vec2(-1.0f,3.0f)
);

out vec2 uv;

void main(){
	vec2 pos = vertices[gl_VertexID];
	uv = (pos + 1.0)/2.0;
	gl_Position = vec4(pos,0.0,1.0);
}