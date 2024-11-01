#pragma once
#include "main.hpp"
#include "Vec3.hpp"

class IndexBuffer;
class VertexBuffer;
class Vec3;

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
        void print_fps_counter(GLFWwindow *window);
        Vec3                centerCoords;

        GLFWwindow*         window;
        GLuint				program;
	    GLuint				vtx_shader;
	    GLuint				frag_shader;
        std::vector<float>  vertices;
        std::vector<uint>    indices;

        VertexBuffer*    vbo;
        IndexBuffer*     ibo;
        unsigned int     VAO;
        float          angle;
    private:
        Object();
        
        std::string  _filename;
};