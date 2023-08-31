#version 330 core

in vec3 Position;
in vec3 Normal;

out vec4 outColor;

struct Material{
	vec3 diffuse;
	vec3 specular;
	float a; //Specular exponent
};

struct PointLight{
	vec3 position;
	vec3 intensity;
};

uniform vec3 eye;
uniform Material material;
uniform PointLight light;
uniform samplerCube skybox;
uniform bool skyboxToggle;


void main()
{
	vec3 eyeDir = normalize(Position - eye);
	vec3 reflectDir = reflect(eyeDir,Normal);


	vec3 lightDir = normalize(light.position - Position.xyz);
	vec3 viewVec = normalize(eye - Position);
	vec3 halfVec = normalize(viewVec + lightDir);

	//float diffuseAmt = max(0.0,dot(Normal,lightDir));
	float specAmt = max(0.0,pow(dot(Normal,halfVec),material.a));
	
	vec3 color;

	if(skyboxToggle){
		color = light.intensity*(texture(skybox, reflectDir).xyz*material.diffuse + specAmt*material.specular);
	}else{
		color = light.intensity*(material.diffuse + specAmt*material.specular);
	}
	//vec3 color = material.diffuse*diffuseAmt;
	outColor = vec4(color,1.0);

}