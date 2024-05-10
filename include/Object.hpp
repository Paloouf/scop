#pragma once
#include "main.hpp"

class Triangle;
class Vertex;

class Object{
    public:
        Object(std::string);
        ~Object();
        int parseObj(std::string);
        bool isObjFile(const std::string);
        void storePoint(std::string, int);
        void storeFace(std::string);
        std::vector<Triangle*> &getTriangles(){return _triangles;};
        std::map<int, Vertex*> &getVertices(){return _vertices;};

    private:
        Object();
        
        std::string  _filename;
        std::map<int, Vertex*> _vertices;
        std::vector<Triangle*> _triangles;
};