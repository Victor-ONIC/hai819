#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader
{
    // Identifiant du programme shader pour OpenGL;
    unsigned int ID;

public:
    // Le constructeur lit le code source depuis deux fichiers et construit le programme shader.
    Shader(const std::string& VS_path, const std::string& FS_path);
    ~Shader();

    // Dire à OpenGL d'utiliser ce programme pour le rendu.
    void use();

    // Donner une valeur à un uniforme.
    void set_uniform(const std::string& name, int i);                                    // 1 int
    void set_uniform(const std::string& name, float f);                                  // 1 float
    void set_uniform(const std::string& name, float f1, float f2, float f3);             // 3 floats
    void set_uniform(const std::string& name, float f1, float f2, float f3, float f4);   // 4 floats
    void set_uniform(const std::string& name, const glm::mat4& matrix);                  // 1 matrice 4x4
};

#endif
