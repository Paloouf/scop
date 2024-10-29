#include "../include/main.hpp"

float red=1.0f, blue=1.0f, green=1.0f;
float angle = 1.0f;
//Object* obj;
bool isRunning = 1;


void perspective(float fov, float aspect, float near, float far) {
    float f = 1.0f / tan(fov * 0.5f * M_PI / 180.0f);
    float rangeInv = 1.0f / (near - far);

    GLfloat matrix[16] = {
        f / aspect, 0.0f, 0.0f,                              0.0f,
        0.0f,       f,    0.0f,                              0.0f,
        0.0f,       0.0f, (far + near) * rangeInv,          -1.0f,
        0.0f,       0.0f, (2.0f * far * near) * rangeInv,    0.0f
    };

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(matrix);
    glMatrixMode(GL_MODELVIEW); 
}

void resize(int w, int h) {
    if (h == 0)
        h = 1;
    float ratio = 1.0 * w /h;
    float fov = 50.0f;
    float nearPlan = 1.0f;
    float farPlan = 100.0f;

    glLoadIdentity();
    glViewport(0, 0, w, h);
    perspective(fov, ratio, nearPlan, farPlan);
}

void freeAll(){
    std::cout << "on est la i guess\n";
}

void    processNormalKeys(unsigned char key, int x, int y){
    (void)x;
    (void)y;
    if (key == 27){
        //delete obj;
        freeAll();
        exit(0);
    }
    else if (key=='r') {
		int mod = glutGetModifiers();
		if (mod == GLUT_ACTIVE_ALT)
			red = 0.0;
		else
			red = 1.0;
	}
}

void    processSpecialKeys(int key, int x, int y){
    (void)x;
    (void)y;
    switch(key) {
		case GLUT_KEY_F1 :
				red = 1.0;
				green = 0.0;
				blue = 0.0; break;
		case GLUT_KEY_F2 :
				red = 0.0;
				green = 1.0;
				blue = 0.0; break;
		case GLUT_KEY_F3 :
				red = 0.0;
				green = 0.0;
				blue = 1.0; break;
	}
}

void glInit(int ac, char** av){
    glutInit(&ac, av);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(WIDTH/2, HEIGHT/2);
}

void    displayFunc(Object obj){
    while (1){
        display(obj);  
    }
    return;
}

void customMainLoop(Object &obj) {
    while (isRunning)
        display(obj);    
}

int main(int argc, char** argv) {

    if (argc != 2){
        return 1;
    }
    string av = argv[1];
    Object* obj = new Object(av);
    glInit(argc, argv);
    int window = glutCreateWindow("scop ltressen");
    window += 0;
    //displayFunc(*obj);
    // glutDisplayFunc(display); // how to make this on my own
    // glutReshapeFunc(resize); // same

    //glutIdleFunc(idle); // same lmao

    resize(WIDTH, HEIGHT);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    customMainLoop(*obj);
    //glutMainLoop();
    return 0;
}

void display(Object &obj) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;
    int vertexCount = 0;
    for(std::map<int,Vertex*>::iterator it = obj.getVertices().begin(); it != obj.getVertices().end();it++){
        centerX += (*it).second->getX();
        centerY += (*it).second->getY();
        centerZ += (*it).second->getZ();
        vertexCount++;
    }
    centerX /= vertexCount;
    centerY /= vertexCount;
    centerZ /= vertexCount;
    // gluLookAt(  0.0f, 0.0f, 10.0f,
	// 		    0.0f, 0.0f,  0.0f,
	// 		    0.0f, 1.0f,  0.0f); //this needs to be remade too
    
    glTranslatef(-centerX, -centerY, -centerZ);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
        for (std::vector<Triangle*>::iterator it = obj.getTriangles().begin() ; it != obj.getTriangles().end(); it++){
            glColor3f((*it)->_red, (*it)->_green, (*it)->_blue); 
            glVertex3f((*it)->getA()->getX(),(*it)->getA()->getY(),(*it)->getA()->getZ());
            glVertex3f((*it)->getB()->getX(),(*it)->getB()->getY(),(*it)->getB()->getZ());
            glVertex3f((*it)->getC()->getX(),(*it)->getC()->getY(),(*it)->getC()->getZ());
        }
    glEnd();
    angle += 0.5f;
    if (angle == 360.0f)
        angle = 0;
    glutSwapBuffers();
}