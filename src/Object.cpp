#include "../include/Object.hpp"

Object::Object(string filename) :_filename(filename){
    parseObj(filename);
    InitContext();
    parseShader();
    createBuffers();
    renderer();
}

Object::Object(){}

Object::~Object(){

}

void Object::createBuffers(){
    // for (float i: vertices)
    //     std::cout << i << ' ';
    // cout << "vertex puis index\n";
    // for (uint i: indices)
    //     cout << i << ' ';
    vbo = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
    ibo = new IndexBuffer(indices.data(), indices.size());
    //unsigned int VAO;
    vbo->Bind();
    ibo->Bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //To show with lines only
}

void Object::draw(){
    glUseProgram(program);
    glBindVertexArray(vbo->getVbo());
    glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
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

void Object::renderer(){
    cout << "now\n";
    while (!glfwWindowShouldClose(window))
	{
		print_fps_counter(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw();
        glfwSwapBuffers(window);
		glfwPollEvents();
	}
    //free_all(); fonction a faire
}

void    framebuffer_size_callback(GLFWwindow *window,int width, int height)
{
	(void)window;
    (void)width;
    (void)height;
	glViewport(0, 0, WIDTH, HEIGHT);
}

void Object::InitContext(){
    if(!glfwInit()){
        cout << "problem init\n";
        return ;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);
    
    window = glfwCreateWindow(WIDTH, HEIGHT, "scop ltressen", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
	    cout << "Failed to initialize GLAD\n";
        return;
    }
}

void Object::parseShader(){
    this->frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    this->vtx_shader = glCreateShader(GL_VERTEX_SHADER);

    //Lire le shader vertex et stocker pour la compilation apres
    std::string vert_glsl_code;
    std::ifstream vert_stream("shaders/vert.glsl", std::ios::in);
    if (vert_stream.is_open()){
        std::stringstream sstr;
        sstr << vert_stream.rdbuf();
        vert_glsl_code = sstr.str();
        vert_stream.close();
    }
    else{
        std::cout << "probleme open vert.glsl, check directory\n";
        return;
    }

    //la meme pour le shader fragment
    std::string frag_glsl_code;
    std::ifstream frag_stream("shaders/frag.glsl", std::ios::in);
    if (frag_stream.is_open()){
        std::stringstream sstr;
        sstr << frag_stream.rdbuf();
        frag_glsl_code = sstr.str();
        frag_stream.close();
    }
    else{
        std::cout << "probleme open frag.glsl, check directory\n";
        return;
    }

    GLint Result = GL_FALSE;
	int InfoLogLength;

    printf("Compiling vertex shader\n");
    char const * VertexSourcePointer = vert_glsl_code.c_str();
	glShaderSource(vtx_shader, 1, &VertexSourcePointer , NULL);
	glCompileShader(vtx_shader);

    // Check vertex Shader
    glGetShaderiv(vtx_shader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vtx_shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(vtx_shader, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}


	printf("Compiling fragment shader\n");
	char const * FragmentSourcePointer = frag_glsl_code.c_str();
	glShaderSource(frag_shader, 1, &FragmentSourcePointer , NULL);
	glCompileShader(frag_shader);

	// Check Fragment Shader
	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(frag_shader, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

    //link le programme shader a l'objet
    this->program = glCreateProgram();
    glAttachShader(this->program, vtx_shader);
	glAttachShader(this->program, frag_shader);
	glLinkProgram(this->program);

    // Check the program
	glGetProgramiv(this->program, GL_LINK_STATUS, &Result);
	glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(this->program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

    glDetachShader(this->program, vtx_shader);
	glDetachShader(this->program, frag_shader);
	
	glDeleteShader(vtx_shader);
	glDeleteShader(frag_shader);
}

int Object::parseObj(string resource)
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
    int vertexIndex = 1;
    while (getline(file, line)){
        if (line[0] == 'v'){
            storePoint(line);
            vertexIndex++;
        }
    }
    // for (uint i = 0; i < vertices.size(); i++){
    //     if (i % 3 == 0){
    //         cout << "index:" << (i/3)+1 <<  "[ x:" << vertices[i];
    //         cout << ", y:" << vertices[i+1];
    //         cout << ", z:" << vertices[i+2] << "]" << std::endl;
    //     }
    // }
    ifstream filetwo(resource.c_str());
    while(getline(filetwo,line)){
        if (line[0] == 'f')
            storeFace(line);
    }
    // for (uint i = 0; i < indices.size(); i++){
    //     cout << "index[" << i << "]:" <<indices[i] << std::endl;
    // }
    filetwo.close();
    file.close();
    return 1;
}

bool Object::isObjFile(const string filename){
    size_t dotPosition = filename.find_last_of('.');
    if (dotPosition != string::npos && dotPosition < filename.length() - 1){
        string extension = filename.substr(dotPosition + 1);
        if (extension == "obj")
            return true;
    }
    return false;
}

void Object::storePoint(string line){
    stringstream pointdata(line);
    string v, x, y, z;
    pointdata >> v >> x >> y >> z;
    //cout << pointdata.str() << std::endl;
    //_vertices.insert(make_pair(vertexIndex, new Vertex(stod(x), stod(y), stod(z))));
    vertices.push_back(std::stof(x));
    vertices.push_back(std::stof(y));
    vertices.push_back(std::stof(z));

}

void Object::storeFace(string line){
    stringstream facedata(line);
    string f, a, b, c, d, e;
    facedata >> f >> a >> b >> c >> d >> e;
    
    //cout << facedata.str() << std::endl;
    if (!d.empty()){
        indices.push_back(stoi(a));
        indices.push_back(stoi(c));
        indices.push_back(stoi(d));
        _triangles.push_back(new Triangle(_vertices[stoi(a)], _vertices[stoi(c)], _vertices[stoi(d)]));
    }
    if (!e.empty()){
        indices.push_back(stoi(a));
        indices.push_back(stoi(d));
        indices.push_back(stoi(e));
        _triangles.push_back(new Triangle(_vertices[stoi(a)], _vertices[stoi(d)], _vertices[stoi(e)]));
    }
    indices.push_back(stoi(a));
    indices.push_back(stoi(b));
    indices.push_back(stoi(c));
    _triangles.push_back(new Triangle(_vertices[stoi(a)], _vertices[stoi(b)], _vertices[stoi(c)]));
}


