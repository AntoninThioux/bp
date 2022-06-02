/**
 * @file Loader.h
 * @author Antonin Thioux (antonin.thioux@gmail.com)
 * @brief Header file to Loader.cpp
 * @date last modified at 2022-06-02
 * @version 1.0
 */

#ifndef BP_LOADER_H
#define BP_LOADER_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>
#include <sstream>
#include <fstream>

namespace HullRendering {
    class Loader {
    private:
        std::vector<unsigned int> indicesData;
        std::vector<glm::vec3> vertexData;
        std::vector<glm::vec3> normalData;
        std::vector<glm::vec3> colorData;
        GLuint vertexBuffer;
        GLuint normalBuffer;
        GLuint colorBuffer;
        GLuint elementBuffer;
        void readFile(std::string filepath);
        void makeBuffers();
    public:
        Loader(std::string filepath);
        // Loader();
        void drawModel();
        ~Loader();
    };
}

#endif
