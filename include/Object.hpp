#pragma once
#include "main.hpp"

class Triangle;

class Object{
    public:
        Object();
        ~Object();
    private:
        vector<Triangle*> _triangles;
};