#ifndef MATERIAL_H
#define MATERIAL_H

#include "math/vec.h"

struct Material {
	math::Vec3 diffuse;
	math::Vec3 specular;
	float specularExponent;
};

struct Light {
	math::Vec3 position;
	math::Vec3 intensity;
};

struct Sphere {
	math::Vec3 center;
	float radius;
	Material mat;
};

#endif