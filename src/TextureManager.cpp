// Permet de sauvegarder et récupérer des textures
#include "TextureManager.h"

void TextureManager::loadTexture(const std::string& name, const std::string& computePath) {
    m_textures[name] = std::make_shared<Texture>(computePath);
}

std::shared_ptr<Texture> TextureManager::getTexture(const std::string& name) {
    auto it = m_textures.find(name);
    if (it != m_textures.end()) {
        return it->second;  // Retourne un pointeur partagé vers la Texture
    } else {
        std::cerr << "Texture " << name << " not found!" << std::endl;
        throw std::runtime_error("Texture not found");  // Lance une exception si la texture n'est pas trouvée
    }
}
