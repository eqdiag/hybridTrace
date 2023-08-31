#version 330 core

#define MAX_DIST 100000000
#define MAX_NUM_SPHERES 100

in vec2 uv;
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

struct Sphere{
	vec3 center;
	float radius;
	Material material;
};


struct Ray{
	vec3 point;
	vec3 dir;
};

struct Hit{
	vec3 point;
	vec3 normal;
	vec3 incoming;
	float t;
	Material mat;
};


//Light params
uniform PointLight light;
uniform samplerCube skybox;
uniform bool skyboxToggle;

//Camera params
uniform mat4 proj;
uniform mat4 view;
uniform vec3 eye;

//Geometry params
uniform int numSpheresDisplayed;
uniform int numBounces;
uniform Sphere spheres[MAX_NUM_SPHERES];

bool getSphereHit(Ray ray,Sphere sphere,out Hit hit){
	vec3 v = normalize(ray.dir);
	vec3 dx = ray.point - sphere.center;
	float B = 2.0*dot(dx,v);
	float C = dot(dx,dx) - sphere.radius*sphere.radius;
	float discrim = B*B - 4.0*C;
	if(discrim < 0) return false;
	float t = (-B - sqrt(discrim))*0.5;
	if(t > 0){
		hit.point = ray.point + t*v;
		hit.normal = normalize(hit.point - sphere.center);
		hit.t = t;
		hit.incoming = v;
		hit.mat = sphere.material;
		return true;
	}
	t = (-B + sqrt(discrim))*0.5;
	if(t > 0){
		hit.point = ray.point + t*v;
		hit.normal = normalize(hit.point - sphere.center);
		hit.t = t;
		hit.incoming = v;
		hit.mat = sphere.material;
		return true;
	}
	return false;
}

//Assumes all t > 0 returned from objects in world
bool getClosestHit(Ray ray,out Hit closestHit){

	bool hitFound = false;
	closestHit.t = MAX_DIST;

	for(int i = 0;i < numSpheresDisplayed;i++){
		Hit hit;
		if(getSphereHit(ray,spheres[i],hit)){
			if(hit.t < closestHit.t){
				closestHit = hit;
				hitFound = true;
			}
		}
	}
	return hitFound;
}

vec3 Shade(Hit hit){
	vec3 eyeDir = normalize(hit.point - eye);
	vec3 reflectDir = reflect(eyeDir,hit.normal);

	vec3 lightDir = normalize(light.position - hit.point);
	vec3 viewVec = normalize(eye - hit.point);
	vec3 halfVec = normalize(viewVec + lightDir);

	float diffuseAmt = max(0.0,dot(hit.normal,lightDir));
	float specAmt = max(0.0,pow(dot(hit.normal,halfVec),hit.mat.a));

	Ray shadowRay = Ray(hit.point + 0.01*lightDir,lightDir);

	Hit temp;

	//Not in shadow
	if(!getClosestHit(shadowRay,temp)){
		return light.intensity*(hit.mat.diffuse*diffuseAmt + specAmt*hit.mat.specular);
	}else{ //In shadow
		return vec3(0.0);
	}
}

vec3 trace(Ray eyeRay){


	Hit hit;
	if(getClosestHit(eyeRay,hit)){
		vec3 color = Shade(hit);

		Ray bounceRay;
		vec3 spec = hit.mat.specular;

		//For multiple bounces
		for(int i = 0;i< numBounces;i++){

			//Create reflected ray
			vec3 reflectDir = reflect(hit.incoming,hit.normal);
			bounceRay = Ray(hit.point + 0.05*reflectDir,reflectDir);

			//Shoot into scene
			if(getClosestHit(bounceRay,hit)){
				vec3 spec = hit.mat.specular;
				color += Shade(hit) * vec3(pow(spec.x,i),pow(spec.y,i),pow(spec.z,i));
			}else{
				if(skyboxToggle){
					color += texture(skybox, bounceRay.dir).rgb * pow(.1,i)*.3;
				}
			}
		}
		return color;
	}else{
		if(skyboxToggle){
			texture(skybox, eyeRay.dir).rgb;
		}else{
			return vec3(0);
		}
	}
}


void main()
{
	vec4 v = inverse(proj) * vec4(2.0*uv.x - 1.0,2.0*uv.y - 1.0,-1.0,1.0);
	v = inverse(view) * vec4(v.x,v.y,-1.0,0.0);

	Ray eyeRay = Ray(eye,v.xyz);

	vec3 color = trace(eyeRay);

	outColor = vec4(color,1.0);
}