#include "../include/Object.hpp"

Object::Object(string filename) :_filename(filename){
    srand(time(NULL));
    angle = 0;
    parseObj(filename);
    centerCoords = Vec3::findCentroid(vertices);
    InitContext();
    parseShader();
    createBuffers();
    //renderer();
}

Object::Object(){}

Object::~Object(){
    //i need to free some shit probably
}

void Object::createBuffers(){
    vbo = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
    ibo = new IndexBuffer(indices.data(), indices.size());
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    vbo->Bind();
    ibo->Bind();
    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    cout << ibo->GetCount() <<std::endl;
}

void Object::draw(){
    glUseProgram(program);
    //translate first to the center coordinates every draw call then make it rotate at speed angle
    Mat4 rotation = Mat4::translate(-centerCoords.x,-centerCoords.y,-centerCoords.z) * Mat4::rotateY(angle);
    GLuint rotationLoc = glGetUniformLocation(program, "rotation");
    glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, rotation.value_ptr());
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);
    //glDisableVertexAttribArray(0);
}

void Object::print_fps_counter(GLFWwindow *window) {
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
    Mat4 projection = Mat4::perspective(90.0f * (M_PI / 180), (float)WIDTH/(float)HEIGHT, 1.0f, 100.0f);

    Mat4 view = Mat4::lookAt(Vec3(3,1,3), Vec3(0,0,0), Vec3(0,1,0)); 
    //first vec to place the camera in a 3D world
    //second vec is actually where you look at, so origin here
    //third vec is what is up, in our case, Y axis, so we rotateY() in draw call

    Mat4 model;
    
    GLuint modelLoc = glGetUniformLocation(program, "model");
    GLuint viewLoc = glGetUniformLocation(program, "view");
    GLuint projectionLoc = glGetUniformLocation(program, "projection");

    glUseProgram(program);
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.value_ptr());
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.value_ptr());
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.value_ptr());

    while (!glfwWindowShouldClose(window))
	{
		print_fps_counter(window);
		glClearColor(0.1f, 0.1f, 0.1f, 0.6f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw();
        angle += 0.0005f; //jdois faire en sorte que ca soit smooth et pas link aux fps, c'est nul la
        if (angle >= 360.0f)
            angle = 0;
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
    //code found on learnopengl.com, good tutorials just follow that stuff
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
    //code on learnopengl.com again, explains in detail over there
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
    //my lame parsing with some debug code in there
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

    //adding random colors at the back of it
    vertices.push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
    vertices.push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
    vertices.push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));

}

void Object::storeFace(string line){
    stringstream facedata(line);
    string f, a, b, c, d, e;
    facedata >> f >> a >> b >> c >> d >> e;
    
    //cout << facedata.str() << std::endl;
    //the -1 is there to start the indices at 0, obj starts at 1, opengl at 0 so unless
    //you want stuff to be all over the place, its necessary;
    if (!d.empty()){
        indices.push_back(stoi(a) - 1);
        indices.push_back(stoi(c) - 1);
        indices.push_back(stoi(d) - 1);
    }
    if (!e.empty()){
        indices.push_back(stoi(a) - 1);
        indices.push_back(stoi(d) - 1);
        indices.push_back(stoi(e) - 1);
    }
    indices.push_back(stoi(a) - 1);
    indices.push_back(stoi(b) - 1);
    indices.push_back(stoi(c) - 1);
}


