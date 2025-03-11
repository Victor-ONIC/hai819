#ifndef CUBE_H
#define CUBE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string> // Ajouté ici
#include "ShaderManager.h"

class Cube {
public:
    Cube(std::shared_ptr<Shader> shader);
    ~Cube();

    void draw(glm::mat4 proj, glm::mat4 view);
    void setPosition(const glm::vec3& position);

private:
    GLuint shaderProgram;
    GLuint VAO, VBO, EBO;
    glm::mat4 model;
};

#endif
