#include "Cube.h"
#include "Shader.h"
#include "ShaderManager.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Cube::Cube(std::shared_ptr<Shader> shader) {
  shaderProgram = shader->getID();  // Utilise l'opérateur -> pour accéder à l'ID

  GLfloat cubeVertices[32] = {
    -10, 10,  10,  0, // 0
    10,  10,  10,  0, // 1
    10,  10,  -10, 0, // 2
    -10, 10,  -10, 0, // 3
    -10, -10, 10,  0, // 4
    10,  -10, 10,  0, // 5
    10,  -10, -10, 0, // 6
    -10, -10, -10, 0  // 7
  };

  GLuint cubeIndices[24] = {
    0, 1, 2, 3, // Face haut
    4, 7, 6, 5, // Face bas
    0, 4, 5, 1, // Face avant
    7, 3, 2, 6, // Face arrière
    4, 0, 3, 7, // Face gauche
    5, 6, 2, 1  // Face droite
  };

  // Génération des buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}

// Destructeur
Cube::~Cube() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

// Fonction de rendu
void Cube::draw(glm::mat4 proj, glm::mat4 view) {
  glLineWidth(1.0f);
  glUseProgram(shaderProgram);

  glm::mat4 mvp = proj * view * model;
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE,
                     &mvp[0][0]);

  glBindVertexArray(VAO);

  for (int i = 0; i < 6; i++) {
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT,
                   (const void *)(i * 4 * sizeof(GLuint)));
  }

  glBindVertexArray(0);
  glUseProgram(0);
}

void Cube::setPosition(const glm::vec3 &position) {
  model = glm::translate(glm::mat4(1.0f), position);
}
