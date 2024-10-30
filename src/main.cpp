#include "../include/main.hpp"

float red=1.0f, blue=1.0f, green=1.0f;
float angle = 1.0f;


// void perspective(float fov, float aspect, float near, float far) {
//     float f = 1.0f / tan(fov * 0.5f * M_PI / 180.0f);
//     float rangeInv = 1.0f / (near - far);

//     GLfloat matrix[16] = {
//         f / aspect, 0.0f, 0.0f,                              0.0f,
//         0.0f,       f,    0.0f,                              0.0f,
//         0.0f,       0.0f, (far + near) * rangeInv,          -1.0f,
//         0.0f,       0.0f, (2.0f * far * near) * rangeInv,    0.0f
//     };

//     glMatrixMode(GL_PROJECTION);
//     glLoadMatrixf(matrix);
//     glMatrixMode(GL_MODELVIEW); 
// }

// void resize(int w, int h) {
//     if (h == 0)
//         h = 1;
//     float ratio = 1.0 * w /h;
//     float fov = 50.0f;
//     float nearPlan = 1.0f;
//     float farPlan = 100.0f;

//     glLoadIdentity();
//     glViewport(0, 0, w, h);
//     perspective(fov, ratio, nearPlan, farPlan);
// }

// void freeAll(){
//     std::cout << "on est la i guess\n";
// }

// void    processNormalKeys(unsigned char key, int x, int y){
//     (void)x;
//     (void)y;
//     if (key == 27){
//         //delete obj;
//         isRunning = false;
//         std::cout << "wtf man\n";
//         freeAll();
//         exit(0);
//     }
//     else if (key=='r') {
// 		int mod = glfwGetModifiers();
// 		if (mod == GLFW_ACTIVE_ALT)
// 			red = 0.0;
// 		else
// 			red = 1.0;
// 	}
// }

void    processSpecialKeys(int key, int x, int y){
    (void)x;
    (void)y;
    switch(key) {
		case GLFW_KEY_F1 :
				red = 1.0;
				green = 0.0;
				blue = 0.0; break;
		case GLFW_KEY_F2 :
				red = 0.0;
				green = 1.0;
				blue = 0.0; break;
		case GLFW_KEY_F3 :
				red = 0.0;
				green = 0.0;
				blue = 1.0; break;
	}
}

void    framebuffer_size_callback(GLFWwindow *window,int width, int height)
{
	(void)window;
    (void)width;
    (void)height;
	glViewport(0, 0, WIDTH, HEIGHT);
}


void print_fps_counter(GLFWwindow *window) {
    static double previousSeconds = glfwGetTime();
    static int frameCount = 0;
    double currentSeconds = glfwGetTime();
    double elapsedSeconds = currentSeconds - previousSeconds;

    // Increment frame count every frame
    frameCount++;

    // Update the FPS once per second
    if (elapsedSeconds >= 1.0) {
        double fps = (double)frameCount / elapsedSeconds;

        // Reset the counter and timer
        previousSeconds = currentSeconds;
        frameCount = 0;

        // Create a new window title with the FPS included
        std::ostringstream oss;
        oss.precision(2);
        oss << std::fixed << "scop ltressen - FPS: " << fps;

        // Set the new title
        glfwSetWindowTitle(window, oss.str().c_str());
    }
}

int main(int argc, char** argv) {

    if (argc != 2){
        return 1;
    }
    string av = argv[1];
    //Object* obj = new Object(av);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "scop ltressen", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
	    cout << "Failed to initialize GLAD\n";
        return -1;
    }
    while (!glfwWindowShouldClose(window)){
        print_fps_counter(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //display(*obj);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}

// void display(Object &obj) {
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     glLoadIdentity();
//     float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;
//     int vertexCount = 0;
//     for(std::map<int,Vertex*>::iterator it = obj.getVertices().begin(); it != obj.getVertices().end();it++){
//         centerX += (*it).second->getX();
//         centerY += (*it).second->getY();
//         centerZ += (*it).second->getZ();
//         vertexCount++;
//     }
//     centerX /= vertexCount;
//     centerY /= vertexCount;
//     centerZ /= vertexCount;
//     glTranslatef(-centerX, -centerY, -centerZ);
//     glRotatef(angle, 0.0f, 1.0f, 0.0f);
//     glBegin(GL_TRIANGLES);
//         for (std::vector<Triangle*>::iterator it = obj.getTriangles().begin() ; it != obj.getTriangles().end(); it++){
//             glColor3f((*it)->_red, (*it)->_green, (*it)->_blue); 
//             glVertex3f((*it)->getA()->getX(),(*it)->getA()->getY(),(*it)->getA()->getZ());
//             glVertex3f((*it)->getB()->getX(),(*it)->getB()->getY(),(*it)->getB()->getZ());
//             glVertex3f((*it)->getC()->getX(),(*it)->getC()->getY(),(*it)->getC()->getZ());
//         }
//     glEnd();
//     angle += 0.5f;
//     if (angle == 360.0f)
//         angle = 0;
//     glfwSwapBuffers();
// }

// REWRITE WITH PROPER FUNCTIONS LIKE DRAWELEMENTS SO USE VAOS AND VBOS, SHADERS AND FRAGMENT SHADERS U STUPID 