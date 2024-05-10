#pragma once
#include "main.hpp"

class Vertex;

class Triangle{
    public:
        Triangle(Vertex*, Vertex*, Vertex*);
        ~Triangle();
        Vertex* &getA(){return _A;};
        Vertex* &getB(){return _B;};
        Vertex* &getC(){return _C;};
        float _red;
        float _green;
        float _blue;
    private:
        Triangle();
        Vertex* _A;
        Vertex* _B;
        Vertex* _C;
};