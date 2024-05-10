#pragma once

#include "main.hpp"

class Vertex{
    public:
        Vertex(double x, double y, double z);
        ~Vertex();
        double getX(){return _x;};
        double getY(){return _y;};
        double getZ(){return _z;};
    private:
        Vertex();
        double _x;
        double _y;
        double _z;
};