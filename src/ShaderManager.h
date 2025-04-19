// Permet de sauvegarder et récupérer des shaders
#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <iostream>
#include <memory> // Pour std::unique_ptr et std::shared_ptr
#include <string>
#include <unordered_map>

#define GLEW_STATIC
#include "GL/glew.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class ShaderManager {
public:
  static ShaderManager &
  getInstance() { //  Pour avoir une instance unique et statique à la class
    static ShaderManager instance;
    return instance;
  }

  void loadShader(const std::string &name, const std::string &vertexPath,
                  const std::string &fragmentPath,
                  const std::string &geomPath = "");
  void loadShader(const std::string &name, const std::string &computePath);
  std::shared_ptr<Shader> getShader(const std::string &name);

private:
  std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;

  // Empêche la copie
  ShaderManager() = default;
  ~ShaderManager() = default; // Plus besoin de destructeur manuel

  ShaderManager(const ShaderManager &) = delete;
  ShaderManager &operator=(const ShaderManager &) = delete;
};
#endif
