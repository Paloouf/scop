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

// void    framebuffer_size_callback(GLFWwindow *window,int width, int height)
// {
// 	(void)window;
//     (void)width;
//     (void)height;
// 	glViewport(0, 0, WIDTH, HEIGHT);
// }




int main(int argc, char** argv) {

    if (argc != 2){
        return 1;
    }
    string av = argv[1];
    Object* obj = new Object(av);
    (void)obj;
    return 0;
}

// void display(Object &obj) {
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     glUseProgram(obj.program);
//     glLoadIdentity();

//     updateVBO();

//     //glBindTexture(GL_TEXTURE_2D, textureID);

//     obj.ibo->Bind();
//     obj.vbo->Bind();
//     glEnableVertexAttribArray(0); // Vertex positions
    
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

//    // glEnableVertexAttribArray(1); // Vertex textures
//     //tbo->Bind();
//     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

// 	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //To show with lines only

//     glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, nullptr);

//     glDisableVertexAttribArray(0);
//     //glDisableVertexAttribArray(1);

//     obj.vbo->Unbind();
//     //tbo->Unbind();
//     obj.ibo->Unbind();
//     //drawFrustum(invViewProj);

// }

// REWRITE WITH PROPER FUNCTIONS LIKE DRAWELEMENTS SO USE VAOS AND VBOS, SHADERS AND FRAGMENT SHADERS U STUPID 