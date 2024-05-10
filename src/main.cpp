#include "../include/main.hpp"

float red=1.0f, blue=1.0f, green=1.0f;
float angle = 1.0f;
Object* obj;

void resize(int w, int h) {
    if (h == 0)
        h = 1;
    float ratio = 1.0 * w /h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

void    processNormalKeys(unsigned char key, int x, int y){
    (void)x;
    (void)y;
    if (key == 27)
        exit(0);
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

int main(int argc, char** argv) {

    if (argc != 2){
        return 1;
    }
    string av = argv[1];
    obj = new Object(av);
    glInit(argc, argv);
    int window = glutCreateWindow("scop ltressen");
    window += 0;
    glutDisplayFunc(display);
    glutReshapeFunc(resize);

    glutIdleFunc(display);

    glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
    glutMainLoop();
    return 0;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(  0.0f, 0.0f, 10.0f,
			    0.0f, 0.0f,  0.0f,
			    0.0f, 1.0f,  0.0f);
    float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f;
    int vertexCount = 0;
    for(std::map<int,Vertex*>::iterator it = obj->getVertices().begin(); it != obj->getVertices().end();it++){
        centerX += (*it).second->getX();
        centerY += (*it).second->getY();
        centerZ += (*it).second->getZ();
        vertexCount++;
    }
    centerX /= vertexCount;
    centerY /= vertexCount;
    centerZ /= vertexCount;
    glTranslatef(-centerX, -centerY, -centerZ);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
        for (std::vector<Triangle*>::iterator it = obj->getTriangles().begin() ; it != obj->getTriangles().end(); it++){
            glColor3f((*it)->_red, (*it)->_green, (*it)->_blue); 
            glVertex3f((*it)->getA()->getX(),(*it)->getA()->getY(),(*it)->getA()->getZ());
            glVertex3f((*it)->getB()->getX(),(*it)->getB()->getY(),(*it)->getB()->getZ());
            glVertex3f((*it)->getC()->getX(),(*it)->getC()->getY(),(*it)->getC()->getZ());
        }
    glEnd();
    angle += 0.5f;

    glutSwapBuffers();
}