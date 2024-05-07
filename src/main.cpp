#include "../include/main.hpp"

float red=1.0f, blue=1.0f, green=1.0f;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(  0.0f, 0.0f, 10.0f,
			    0.0f, 0.0f,  0.0f,
			    0.0f, 1.0f,  0.0f);
    
    glBegin(GL_TRIANGLES);
        glVertex3f(-2,-2,-5.0);
		glVertex3f(2,0.0,-5.0);
		glVertex3f(0.0,2,-5.0);
    glEnd();


    glutSwapBuffers();
}

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

bool isObjFile(const string filename){
    size_t dotPosition = filename.find_last_of('.');
    if (dotPosition != string::npos && dotPosition < filename.length() - 1){
        string extension = filename.substr(dotPosition + 1);
        if (extension == "obj")
            return true;
    }
    return false;
}

void storePoint(string line){
    (void)line;
}

void storeFace(string line){
    (void)line;
}

int parseObj(string resource)
{
    if (!isObjFile(resource)){
        cout << "Not an obj file\n";
        return 0;
    }
    ifstream file(resource.c_str());
    if (!file.is_open()){
        cerr << "probleme open" << endl;
        return 0;
    }
    string line;
    int vertexNum = 0;
    int faceNum = 0;
    while (getline(file, line)){
        cout << line << endl;
        if (line[0] == 'v')
            storePoint(line);
        if (line[0] == 'f')
            storeFace(line);
    }
    cout << "vertexnum: " << vertexNum << " , faceNum: " << faceNum << endl;
    file.close();
    return 1;
}

int main(int argc, char** argv) {

    if (argc != 2){
        return 1;
    }
    if (!parseObj(argv[1]))
        return 1;
    glutInit(&argc, argv);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(WIDTH/2, HEIGHT/2);
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