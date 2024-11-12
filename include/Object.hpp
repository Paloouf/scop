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

        //Functions
        int parseObj(std::string);
        bool isObjFile(const std::string);
        void storePoint(std::string);
        void storeFace(std::string);
        void parseShader();
        void InitContext();
        void renderer();
        void draw();
        void createBuffers();
        void loadTexture();
        void applyMaterial();
        void parseMtlFile(const std::string& materialFile);
        void print_fps_counter(GLFWwindow *window);

        //static callback functions used for input management
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        //Values
        bool useTexture = false;
        int polygonMode = 0;
        bool paused = false;
        float xOffset = 0.0f;
        float yOffset = 0.0f;
        float zOffset = 0.0f;
        float cameraXOffset = 0.0f;
        float cameraYOffset = 0.0f;
        float cameraZOffset = 0.0f;
        bool mousePressed = false;
        float cameraDistance = 5.0f; // Initial distance of the camera from the origin
        float zoomSpeed = 0.5f; 
        float translationSpeed = 2.0f;
        float yaw = -90.0f;  // Initial yaw to look forward
        float pitch = 0.0f;  // Initial pitch
        float mouseSensitivity = 0.05f;
        bool firstMouse = true; // Track if it's the first mouse movement
        float lastX = WIDTH / 2.0f; // Initial last mouse X position
        float lastY = HEIGHT / 2.0f;
        unsigned int     VAO; //vertex array object, used in createBuffers()
        float          angle;

        //Used to find center coords of an object and translate accordingly to rotate it on its axis
        Vec3                centerCoords;

        //To read the .mtl file and store info, can be ignored tbh
        struct Material {
            std::string name;
            float diffuse[3] = {1.0f, 1.0f, 1.0f};
            float ambient[3] = {0.2f, 0.2f, 0.2f};
            float specular[3] = {1.0f, 1.0f, 1.0f};
            std::string textureFile;
        };

        Material currentMaterial;

        //OpenGL types
        GLFWwindow*         window;
        GLuint				program;
	    GLuint				vtx_shader;
	    GLuint				frag_shader;
        GLuint              textureID;

        //vectors used in my vertexbuffers and sent to gpu for opengl to treat basically
        std::vector<float>  vertices;
        std::vector<float>  og_vertices;
        std::vector<uint>    indices;

        VertexBuffer*    vbo;
        IndexBuffer*     ibo;


    private:
        Object();
        
        std::string  _filename;
};