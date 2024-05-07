#pragma once
#include "main.hpp"

class Point;

class Triangle{
    public:
        Triangle();
        ~Triangle();
    private:
        Point* _A;
        Point* _B;
        Point* _C;
};