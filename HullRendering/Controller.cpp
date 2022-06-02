/**
 * @file Controller.cpp
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief This file computes the MVP matrix based on user input.
 * @note A lot of the code is based of OpenGL 3.3 tutorial (https://www.opengl-tutorial.org/) which is WTFPL. 
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#include "Controller.h"

using namespace HullRendering;
using namespace glm;

#define SPEED 0.9f
#define MOUSE_SPEED 0.03f
#define ZOOM_SPEED 15.0f
#define INITIAL_POSITION vec3(0.5f, 0.5f, 1.5f)
#define INITIAL_FOV 90.0f
#define INITIAL_PITCH 0.0f
#define INITIAL_HEADING -M_PI

/**
 * @brief Construct a new Controller object.
 */
Controller::Controller(std::string dir, int duration)
:pitch(INITIAL_PITCH),heading(INITIAL_HEADING),position(INITIAL_POSITION),fov(INITIAL_FOV),
duration(duration),dir(dir){
    lastUpdateTime = glfwGetTime();
    modelMatrix = mat4(1.0f);
    showContours = 0;

    c_wasPressed  = z_wasPressed = x_wasPressed = 0;
    currentFrame = 0;

    // in case of animation
    if (duration > 0) {
        animationModel = new Loader(dir + ((dir.back() == '/') ? "" : "/") + "animation_0.obj");
    }
}

/**
 * @brief This function gets the Projection View Model (MVP) matrix.
 * @return mat4 MVP matrix.
 */
mat4 Controller::getMVP(){
    return projectionMatrix * viewMatrix * modelMatrix;
}

/**
 * @brief This function updates the internal projection and viewMatrix based on user input.
 * @param window The GLFW window of the application.
 */
void Controller::updateCamera(GLFWwindow *window) {
    auto t = float(currentTime - lastUpdateTime);

    double x, y;
    int w, h;
    glfwGetCursorPos(window, &x, &y);
    glfwGetWindowSize(window, &w, &h);
    glfwSetCursorPos(window, w/2, h/2);

    heading += MOUSE_SPEED * t * float(w/2 - x);
    pitch += MOUSE_SPEED * t * float(h/2 - y);
    pitch = min(max((double) pitch, M_PI/-2), M_PI/2);

    vec3 forward(cos(pitch) * sin(heading), sin(pitch), cos(pitch) * cos(heading));
    vec3 right(sin(heading - M_PI/2.0f), 0, cos(heading - M_PI/2.0f));
    vec3 up = cross(right, forward);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position += forward * t * SPEED;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position -= forward * t * SPEED;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position += right * t * SPEED;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position -= right * t * SPEED;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) position += up * t * SPEED;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) position -= up * t * SPEED;
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) fov += t * ZOOM_SPEED;
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) fov -= t * ZOOM_SPEED;
    
    projectionMatrix = perspective(radians(fov), (float) h/w, 0.1f, 100.0f);
    viewMatrix = lookAt(position, position + forward, up);
    viewVector = forward;
} 

/**
 * @brief This function updates the options in the controller based on user input. 
 * @param window The GLFW window of the application.
 */
void Controller::updateOptions(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
        fov = INITIAL_FOV;
        pitch = INITIAL_PITCH;
        heading = INITIAL_HEADING;
        position = INITIAL_POSITION;
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) c_wasPressed = 1;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE && c_wasPressed) showContours = !showContours;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) c_wasPressed = 0;

    if (duration < 1) return ;

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) z_wasPressed = 1;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE && z_wasPressed) {
        currentFrame = (--currentFrame + duration) % duration;
        animationModel = new Loader(dir + ((dir.back() == '/') ? "" : "/") + "animation_" + std::to_string(currentFrame) + ".obj");
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE) z_wasPressed = 0;
    
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) x_wasPressed = 1;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE && x_wasPressed) {
        currentFrame = (++currentFrame) % duration;
        animationModel = new Loader(dir + ((dir.back() == '/') ? "" : "/") + "animation_" + std::to_string(currentFrame) + ".obj");
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE) x_wasPressed = 0;
}


/**
 * @brief This function listens for and processes user input.
 * @param window The application window.
 */
void Controller::update(GLFWwindow *window) {
    currentTime = glfwGetTime();
    updateOptions(window);
    updateCamera(window);
    lastUpdateTime = currentTime;
}

/**
 * @brief This function returns the Model matrix (M).
 * @return mat4 matrix M.
 */
mat4 Controller::getM() {
    return modelMatrix;
}

/**
 * @brief This function returns the View matrix (W).
 * @return mat4 matrix M.
 */
mat4 Controller::getV() {
    return viewMatrix;
}
