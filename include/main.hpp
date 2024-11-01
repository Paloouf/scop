#pragma once

class Object;
#define WIDTH 720
#define HEIGHT 480
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
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

void display(Object& obj);

using namespace std;