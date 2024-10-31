#pragma once
#include "main.hpp"

class vec3{
	public:
		vec3(float x,float y, float z) :x(x),y(y),z(z){};
		~vec3();
		float x,y,z;
};