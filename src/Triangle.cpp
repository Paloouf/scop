#include "../include/Triangle.hpp"

Triangle::Triangle(Vertex* A, Vertex* B, Vertex* C): _A(A), _B(B), _C(C){
    cout << "Triangle:" << endl;
    cout << "A: " << _A->getX() << " " << _A->getY() << " " << _A->getZ() << endl;
    cout << "B: " << _B->getX() << " " << _B->getY() << " " << _B->getZ() << endl;
    cout << "C: " << _C->getX() << " " << _C->getY() << " " << _C->getZ() << endl;
    _red = static_cast<float>(rand()) / RAND_MAX;     // Random value between 0 and 1
    _green = static_cast<float>(rand()) / RAND_MAX;   // Random value between 0 and 1
    _blue = static_cast<float>(rand()) / RAND_MAX;    // Random value between 0 and 1
};

Triangle::Triangle(){}

Triangle::~Triangle(){}