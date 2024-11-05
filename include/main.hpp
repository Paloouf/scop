#pragma once

class Object;
#define WIDTH 720
#define HEIGHT 480
const float CAMERA_XY_LIMIT = 100.0f;  // Maximum movement in the x and y axes
const float CAMERA_Z_LIMIT = 500.0f;   // Maximum distance along the z-axis
const float MIN_CAMERA_DISTANCE = 1.5f; // Minimum camera distance to avoid clipping
const float MAX_CAMERA_DISTANCE = 1000.0f; // Maximum zoom out distance
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include "../glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <map>
#include <cmath>
#include <sstream>
#include "Object.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Vec3.hpp"
#include "Mat4.hpp"
#include "BMPStruct.hpp"

void calculateUVsForTriangles(std::vector<float>& vertices);
bool loadBMP(const std::string& filename, int& width, int& height, std::vector<unsigned char>& data);
void display(Object& obj);

using namespace std;