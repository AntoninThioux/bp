/**
 * @file Controller.h
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief Header file to Controller.cpp.
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#ifndef SPATIO_TEMPORAL_HULLS_CONTROLLER_H
#define SPATIO_TEMPORAL_HULLS_CONTROLLER_H

#include "Loader.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace HullRendering {
    class Controller {
    private:
        glm::vec3 position;
        float pitch;
        float heading;
        float fov;
        double currentTime;
        double lastUpdateTime;
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::vec3 viewVector;
        void updateCamera(GLFWwindow *window);
        int c_wasPressed;
        int z_wasPressed;
        int x_wasPressed;
        int duration;
        int currentFrame;
        std::string dir;
        void updateOptions(GLFWwindow *window);

    public:
        Controller(std::string dir, int duration);
        glm::mat4 getMVP();
        glm::mat4 getM();
        glm::mat4 getV();
        void update(GLFWwindow *window);
        int showContours;
        Loader *animationModel;
    };
}


#endif
