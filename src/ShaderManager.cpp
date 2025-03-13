#include "ShaderManager.h"

void ShaderManager::loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geomPath) {
    m_shaders[name] = std::make_shared<Shader>(vertexPath, fragmentPath, geomPath);
}

void ShaderManager::loadShader(const std::string& name, const std::string& computePath) {
    m_shaders[name] = std::make_shared<Shader>(computePath);
}

std::shared_ptr<Shader> ShaderManager::getShader(const std::string& name) {
    auto it = m_shaders.find(name);
    if (it != m_shaders.end()) {
        return it->second;  // Retourne un pointeur partagé vers le Shader
    } else {
        std::cerr << "Shader " << name << " not found!" << std::endl;
        throw std::runtime_error("Shader not found");  // Lance une exception si le shader n'est pas trouvé
    }
}
