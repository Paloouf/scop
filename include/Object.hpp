#pragma once
#include "main.hpp"

class Triangle;
class Vertex;
class IndexBuffer;
class VertexBuffer;

class Object{
    public:
        Object(std::string);
        ~Object();
        int parseObj(std::string);
        bool isObjFile(const std::string);
        void storePoint(std::string);
        void storeFace(std::string);
        void parseShader();
        void InitContext();
        void renderer();
        void draw();
        void createBuffers();
        std::vector<Triangle*> &getTriangles(){return _triangles;};
        std::map<int, Vertex*> &getVertices(){return _vertices;};

        GLFWwindow*         window;
        GLuint				program;
	    GLuint				vtx_shader;
	    GLuint				frag_shader;
        std::vector<float>  vertices;
        std::vector<uint>    indices;

        VertexBuffer*    vbo;
        IndexBuffer*     ibo;
    private:
        Object();
        
        std::string  _filename;
        std::map<int, Vertex*> _vertices;
        std::vector<Triangle*> _triangles;
};