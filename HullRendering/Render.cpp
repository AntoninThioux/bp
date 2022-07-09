/**
 * @file Render.cpp
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief This file contains the logic for rendering the Spatio-Temporal hulls.
 * @note A lot of the code is based of OpenGL 3.3 tutorial (https://www.opengl-tutorial.org/) which is WTFPL. 
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#include "Render.h"

using namespace HullRendering;
using namespace glm;

/**
 * @brief This function gets shader code from a shader file.
 * @param path The path to the file.
 * @return std::string The code as a string ready to compile.
 */
std::string Render::getShaderCode(const char *path) {
    using namespace std;
    string code;

    ifstream shaderStream(path, ios::in);
    if (shaderStream.is_open()) {
        stringstream sstr;
        sstr << shaderStream.rdbuf();
        code = sstr.str();
        shaderStream.close();
    } else {
        cerr << "Could not open ./" << path << "!" << endl;
        exit(4);
    }

    return code;
}

/**
 * @brief This function compiles shader code.
 * @param code The code as a string of the shader that should be compiled.
 * @param shaderID The shader source.
 */
void Render::compileShader(std::string code, GLuint shaderID) {
    using namespace std;
    
    // Compile Shader
    char const *vertexSourcePointer = code.c_str();
    glShaderSource(shaderID, 1, &vertexSourcePointer, nullptr);
    glCompileShader(shaderID);

    // Check Shader
    GLint result = GL_FALSE;
    int infoLogLength;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0){
		vector<char> errorMessage(infoLogLength + 1);
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, &errorMessage[0]);
        cerr << &errorMessage[0] << endl;
        exit(5);
	}
}

/**
 * @brief This function loads a pair of vertex and fragment shaders.
 * @param vertexPath The path to the vertex shader.
 * @param fragmentPath The path to fragment shader.
 */
GLuint Render::loadShaders(const char *vertexPath, const char *fragmentPath) {
    using namespace std;

    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    compileShader(getShaderCode(vertexPath), vertexShaderID);
    compileShader(getShaderCode(fragmentPath), fragmentShaderID);

    // Link the program
    cout << "Linking program" << endl;
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program
    GLint result = GL_FALSE;
    int infoLogLength;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0){
		vector<char> errorMessage(infoLogLength + 1);
		glGetShaderInfoLog(programID, infoLogLength, NULL, &errorMessage[0]);
        cerr << &errorMessage[0] << endl;
        exit(5);
    }
	
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

    return programID;
}

/**
 * @brief This prints the average rendering time of the frames in the last second.
 * @param time The current time.
 */
void Render::printRenderTime(double time) {
    frames++;
    if (time - startTime >= 1.0) {
        std::cout << "avg rendering time: " << (time - startTime) * 1000.0 / frames << "ms" << std::endl;
        frames = 0;
        startTime = time;
    }
}

/**
 * @brief This function is called when a shader needs to be used.
 * @param isContour Which shader should be used? Contour shader or standard.
 */
void Render::useShaders(int isContour) {
    if (isContour) {
        glUseProgram(programID1);
        glUniformMatrix4fv(mvpID1, 1, GL_FALSE, &controller->getMVP()[0][0]);
        glUniformMatrix4fv(mID1, 1, GL_FALSE, &controller->getM()[0][0]);
        glUniformMatrix4fv(vID1, 1, GL_FALSE, &controller->getV()[0][0]);
    } else {
        glUseProgram(programID0);
        glUniformMatrix4fv(mvpID0, 1, GL_FALSE, &controller->getMVP()[0][0]);
        glUniformMatrix4fv(mID0, 1, GL_FALSE, &controller->getM()[0][0]);
        glUniformMatrix4fv(vID0, 1, GL_FALSE, &controller->getV()[0][0]);
    }
}

/**
 * @brief This function is called every frame to paint the spatio-temporal hulls.
 */
void Render::paint() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // get user input
    controller->update(window);

    // draw hulls with contour or standard shaders
    useShaders(controller->showContours);
    hulls->drawModel();

    if (duration != 0) {  // draw animation with standard shaders
        useShaders(0);
        controller->animationModel->drawModel();
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

/**
 * @brief This function starts the rendering of the Spatio-Temporal hulls.
 */
void Render::start(){
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

    hulls = new Loader(filepath);

    controller = new Controller(animationDir, duration);
    startTime = glfwGetTime();
    frames = 0;

    do {
        paint();
        printRenderTime(glfwGetTime());
    } while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS);
}

/**
 * @brief Construct a new Render object.
 * @param filepath The file path to the hull.obj file.
 */
Render::Render(std::string filepath, std::string animationDir, int duration)
:filepath(filepath),animationDir(animationDir),duration(duration){
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(1280, 960, "Spatio-Temporal Hulls", nullptr, nullptr);

    if (!window) {
        std::cerr << "creating window failed" << std::endl;
        glfwTerminate();
        exit(3);
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cerr << "intialization GLEW failed" << std::endl;
		glfwTerminate();
		exit(6);
	}

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.94f, 1.0f, 0.94f, 0.0f);

    programID0 = loadShaders("HullRendering/shaders/standard.vertexshader",
                            "HullRendering/shaders/standard.fragmentshader");
    mvpID0 = glGetUniformLocation(programID0, "MVP");
    mID0 = glGetUniformLocation(programID0, "M");
    vID0 = glGetUniformLocation(programID0, "V");

    programID1 = loadShaders("HullRendering/shaders/contour.vertexshader",
                            "HullRendering/shaders/contour.fragmentshader");
    mvpID1 = glGetUniformLocation(programID1, "MVP");
    mID1 = glGetUniformLocation(programID1, "M");
    vID1 = glGetUniformLocation(programID1, "V");
}

/**
 * @brief Destroy the Render object.
 */
Render::~Render() {
    glDeleteProgram(programID0);
    glDeleteProgram(programID1);
    glDeleteVertexArrays(1, &vertexArrayID);
	glfwTerminate();
}
