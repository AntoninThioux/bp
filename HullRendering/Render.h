/**
 * @file Render.h
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief Header file to Render.cpp
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#ifndef BP_RENDER_H
#define BP_RENDER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "Controller.h"
#include "Loader.h"

namespace HullRendering {
    class Render
    {
    private:
        std::string filepath, animationDir;
        int duration;
        GLFWwindow *window;
        int frames;
        double startTime;
        GLuint programID0, mvpID0, mID0, vID0;
        GLuint programID1, mvpID1, mID1, vID1;
        GLuint vertexArrayID;
        Controller *controller;
        Loader *hulls;
        std::string getShaderCode(const char *path);
        GLuint loadShaders(const char *vertexPath, const char *fragmentPath);
        void compileShader(std::string code, GLuint shaderID);
        void printRenderTime(double time);
        void useShaders(int isContour);
        void paint();
    public:
        Render(std::string filepath, std::string animationDir, int duration);
        void start();
        ~Render();
    };
}

#endif
