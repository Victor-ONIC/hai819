#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <unordered_map>
#include <string>
#include <memory>  // Pour std::unique_ptr et std::shared_ptr
#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"

class ShaderManager {
public:
    static ShaderManager& getInstance() { //  Pour avoir une instance unique et statique à la class
        static ShaderManager instance;
        return instance;
    }

        void loadShader(const std::string& name,
                        const std::string& vertexPath,
                        const std::string& fragmentPath,
                        const std::string& geomPath = "");
    void loadShader(const std::string& name, const std::string& computePath);
    std::shared_ptr<Shader> getShader(const std::string& name);

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;

    // Empêche la copie
    ShaderManager() = default;
    ~ShaderManager() = default; // Plus besoin de destructeur manuel

    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;
};
#endif
