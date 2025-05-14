// Permet de sauvegarder et récupérer des textures
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <iostream>
#include <memory> // Pour std::unique_ptr et std::shared_ptr
#include <string>
#include <unordered_map>

#define GLEW_STATIC
#include "GL/glew.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class TextureManager {
public:
  static TextureManager &
  getInstance() { //  Pour avoir une instance unique et statique à la class
    static TextureManager instance;
    return instance;
  }

  void loadTexture(const std::string& name, const std::string& computePath);
  std::shared_ptr<Texture> getTexture(const std::string &name);

private:
  std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;

  // Empêche la copie
  TextureManager() = default;
  ~TextureManager() = default; // Plus besoin de destructeur manuel

  TextureManager(const TextureManager &) = delete;
  TextureManager &operator=(const TextureManager &) = delete;
};
#endif
