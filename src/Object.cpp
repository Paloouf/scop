#include "../include/Object.hpp"

Object::Object(string filename) :_filename(filename){
    srand(time(NULL));
    angle = 0;
    parseObj(filename);
    centerCoords = Vec3::findCentroid(vertices);
    InitContext();
    parseShader();
    createBuffers();
}

Object::Object(){}

Object::~Object(){
    //i need to free some shit probably
    glDeleteBuffers(1, &vbo->getVbo());
    glDeleteBuffers(1, &ibo->getIbo());
    glDeleteVertexArrays(1, &VAO);
    if (textureID) {
        glDeleteTextures(1, &textureID);
    }
}

void Object::loadTexture(){
    int width, height;
    std::vector<unsigned char> imageData;
    if (!loadBMP("textures/wood.bmp", width, height, imageData)) {
        std::cerr << "Failed to load BMP texture." << std::endl;
    }

    // Generate and bind the texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the texture data to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    //glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "texture1"), 0);
}

void Object::createBuffers(){
    vbo = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
    ibo = new IndexBuffer(indices.data(), indices.size());
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    vbo->Bind();
    ibo->Bind();
    // Position attribute (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute (3 floats, offset by 3 floats from the start of each vertex)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute (2 floats, offset by 6 floats from the start of each vertex)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    cout << ibo->GetCount() <<std::endl;
}

void Object::draw(){
    glUseProgram(program);
    //translate first to the center coordinates every draw call then make it rotate at speed angle
    Mat4 translation = Mat4::translate(xOffset, yOffset,zOffset);

    // Combine the translation, rotation, and centering transformations
    Mat4 model = translation
               * Mat4::translate(-centerCoords.x, -centerCoords.y, -centerCoords.z)
               * Mat4::rotateY(angle);

    GLuint modelLoc = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.value_ptr());

    // Set texture only if in use
    if (useTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(glGetUniformLocation(program, "useTexture"), 1);
    } else {
        glUniform1i(glGetUniformLocation(program, "useTexture"), 0);
    }
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

void applyTexture(std::vector<float>& vertices, int totalVertices) {
    int index = 0;
    while (index < totalVertices) {
        // Bottom-left corner
        vertices[index + 6] = 0.0f;
        vertices[index + 7] = 0.0f;
        index += 8;

        // Top-left corner
        vertices[index + 6] = 0.0f;
        vertices[index + 7] = 1.0f;
        index += 8;

        // Top-right corner
        vertices[index + 6] = 1.0f;
        vertices[index + 7] = 1.0f;
        index += 8;

        // Bottom-right corner
        vertices[index + 6] = 1.0f;
        vertices[index + 7] = 0.0f;
        index += 8;
    }
}

void Object::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Object* obj = static_cast<Object*>(glfwGetWindowUserPointer(window));
    if (!obj) return;
    (void)mods;
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            obj->mousePressed = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else if (action == GLFW_RELEASE) {
            obj->mousePressed = false;
            obj->firstMouse = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Reset firstMouse to avoid jumps
        }
    }
}



void Object::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Object* obj = static_cast<Object*>(glfwGetWindowUserPointer(window));
    if (!obj) return;
    if (!obj->mousePressed) return;

    if (obj->firstMouse) { // Ignore the first mouse movement to avoid jump
        obj->lastX = xpos;
        obj->lastY = ypos;
        obj->firstMouse = false;
    }

    float xOffset = xpos - obj->lastX;
    float yOffset = obj->lastY - ypos; // Reversed: y-coordinates go from bottom to top
    obj->lastX = xpos;
    obj->lastY = ypos;

    xOffset *= obj->mouseSensitivity;
    yOffset *= obj->mouseSensitivity;

    obj->yaw += xOffset;
    obj->pitch += yOffset;

    // Constrain the pitch to avoid gimbal lock
    if (obj->pitch > 89.0f) obj->pitch = 89.0f;
    if (obj->pitch < -89.0f) obj->pitch = -89.0f;
}


void Object::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Object* obj = static_cast<Object*>(glfwGetWindowUserPointer(window));
    (void)scancode;
    (void)mods;
    if (!obj){
        cout << "obj is null\n";
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        obj->paused = !obj->paused; // Toggle pause on 'P' key press
    }
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        if (obj->polygonMode == 0) {
            obj->polygonMode = 1;
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode
        } else if (obj->polygonMode == 1) {
            obj->polygonMode = 2;
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); // Point (pixel) mode
            glPointSize(2.0f); // Adjust point size as desired
        } else {
            obj->polygonMode = 0;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Filled mode
        }
    }
    if (key == GLFW_KEY_ESCAPE){
        exit(1);

        //free_all();
    }
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_W) {
            obj->yOffset += obj->translationSpeed; // Move up
        }
        if (key == GLFW_KEY_S) {
            obj->yOffset -= obj->translationSpeed; // Move down
        }
        if (key == GLFW_KEY_A) {
            obj->xOffset -= obj->translationSpeed; // Move left
        }
        if (key == GLFW_KEY_D) {
            obj->xOffset += obj->translationSpeed; // Move right
        }
        if (key == GLFW_KEY_Q) {
            obj->zOffset += obj->translationSpeed; // Move forward (increase Z)
        }
        if (key == GLFW_KEY_E) {
            obj->zOffset -= obj->translationSpeed; // Move backward (decrease Z)
        }
        if (key == GLFW_KEY_Z) {
            obj->cameraDistance -= obj->zoomSpeed; // Zoom in by moving camera closer
            if (obj->cameraDistance < 1.5f) obj->cameraDistance = 1.5f; // Prevent too close
        }
        if (key == GLFW_KEY_X) {
            obj->cameraDistance += obj->zoomSpeed; // Zoom out by moving camera farther
        }
    }
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        obj->useTexture = !obj->useTexture;  // Toggle texture use
        if (obj->useTexture) {
            applyTexture(obj->vertices, obj->vertices.size());
        } else {
            // Optionally reset or clear UVs if disabling textures
            for (uint i = 6; i < obj->vertices.size(); i += 8) {
                obj->vertices[i] = 0.0f; // Reset U
                obj->vertices[i + 1] = 0.0f; // Reset V
            }
        }

        // Re-upload the modified vertex data
        glBindBuffer(GL_ARRAY_BUFFER, obj->vbo->getVbo());
        glBufferData(GL_ARRAY_BUFFER, obj->vertices.size() * sizeof(float), obj->vertices.data(), GL_STATIC_DRAW);
    }
}


void Object::renderer(){
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
    Mat4 projection = Mat4::perspective(90.0f * (M_PI / 180), (float)WIDTH/(float)HEIGHT, 1.0f, 1000.0f);

    Vec3 cameraPos(0.0f, 0.0f, cameraDistance);
    Vec3 target(0.0f, 0.0f, 0.0f); // Look at the origin
    Vec3 up(0.0f, 1.0f, 0.0f); 
    Mat4 view = Mat4::lookAt(cameraPos, target, up);
    //first vec to place the camera in a 3D world
    //second vec is actually where you look at, so origin here
    //third vec is what is up, in our case, Y axis, so we rotateY() in draw call

    Mat4 model;
    
    GLuint modelLoc = glGetUniformLocation(program, "model");
    GLuint viewLoc = glGetUniformLocation(program, "view");
    GLuint projectionLoc = glGetUniformLocation(program, "projection");
    
    

    glUseProgram(program);

    loadTexture();
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.value_ptr());
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.value_ptr());
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.value_ptr());
    applyMaterial();

    float lastTime = glfwGetTime();
    float rotationSpeed = 1.0f; // Rotation speed in degrees per second
    Vec3 front;
    front.x = cos(yaw * (M_PI / 180)) * cos(pitch * (M_PI / 180));
    front.y = sin(pitch * (M_PI / 180));
    front.z = sin(yaw * (M_PI / 180)) * cos(pitch * (M_PI / 180));
    front = front.normalize();
    while (!glfwWindowShouldClose(window))
	{

        
        //glfwSetKeyCallback(window, Object::keyCallback);
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime; // Calculate time difference
        lastTime = currentTime;

        // Update the angle based on delta time
        if (!paused) {
            angle += rotationSpeed * deltaTime;
            if (angle >= 360.0f) angle -= 360.0f;
        }

        cameraPos = Vec3(0.0f, 0.0f, cameraDistance);
        target = cameraPos + front; // Target based on the camera's orientation
        view = Mat4::lookAt(cameraPos, target, up);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.value_ptr());
        print_fps_counter(window);
		glClearColor(0.1f, 0.1f, 0.1f, 0.6f);
		
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
	glViewport(0, 0, width, height);

    // Update the projection matrix with the new aspect ratio
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    Mat4 projection = Mat4::perspective(90.0f * (M_PI / 180), aspectRatio, 1.0f, 1000.0f);

    // Get the projection uniform location and update it
    GLuint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, reinterpret_cast<GLint*>(&program)); // Get the active shader program
    GLuint projectionLoc = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.value_ptr());
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
    glfwSetWindowUserPointer(window, this);

    // Set the key callback to the static member function
    glfwSetMouseButtonCallback(window, Object::mouseButtonCallback);
    glfwSetCursorPosCallback(window, Object::mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, Object::keyCallback);
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

void Object::parseMtlFile(const std::string& materialFile) {
    std::ifstream mtlFile(materialFile);
    if (!mtlFile.is_open()) {
        std::cout << "Failed to open MTL file: " << materialFile << std::endl;
        return;
    }
    std::string line;
    while (std::getline(mtlFile, line)) {
        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;

        if (prefix == "newmtl") {
            // New material definition
            lineStream >> currentMaterial.name;
        } else if (prefix == "Kd") {
            // Diffuse color
            lineStream >> currentMaterial.diffuse[0] >> currentMaterial.diffuse[1] >> currentMaterial.diffuse[2];
        } else if (prefix == "Ka") {
            // Ambient color
            lineStream >> currentMaterial.ambient[0] >> currentMaterial.ambient[1] >> currentMaterial.ambient[2];
        } else if (prefix == "Ks") {
            // Specular color
            lineStream >> currentMaterial.specular[0] >> currentMaterial.specular[1] >> currentMaterial.specular[2];
        } else if (prefix == "map_Kd") {
            // Diffuse texture map
            lineStream >> currentMaterial.textureFile;
        }
    }
    mtlFile.close();
}

void Object::applyMaterial() {
    GLuint diffuseLoc = glGetUniformLocation(program, "Kd");
    glUniform3fv(diffuseLoc, 1, currentMaterial.diffuse);

    if (!currentMaterial.textureFile.empty()) {
        loadTexture(); // Load texture if specified
        glUniform1i(glGetUniformLocation(program, "useTexture"), 1);
    } else {
        glUniform1i(glGetUniformLocation(program, "useTexture"), 0);
    }
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
        //exit(1); et free aussi partout 
        return 0;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;
        if (prefix == "v") {
            storePoint(line);
        } else if (prefix == "f") {
            storeFace(line);
        } else if (prefix == "mtllib") {

            std::string materialFile;
            lineStream >> materialFile;
            std::string materialPath = "resources/" + materialFile;
            parseMtlFile(materialPath); // Parse the MTL file
        }
    }
    calculateUVsForTriangles(vertices);
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
    og_vertices = vertices;

    //adding random colors at the back of it
    float greyLevel = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    vertices.push_back(greyLevel);
    vertices.push_back(greyLevel);
    vertices.push_back(greyLevel);

    vertices.push_back(0.0f);    // Placeholder U
    vertices.push_back(0.0f);    // Placeholder V
    
    

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


