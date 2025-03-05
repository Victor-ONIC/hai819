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

class Shader {
public:
    Shader() = delete;
    Shader(const std::string& cs_path);
    Shader(const std::string& vs_path, 
           const std::string& fs_path,
           const std::string& gs_path = "");
    ~Shader();

    void use();
    void stop();

    void set_uniform(const std::string& name, int i);                                    // 1 int
    void set_uniform(const std::string& name, float f);                                  // 1 float
    void set_uniform(const std::string& name, float f1, float f2, float f3);             // 3 floats
    void set_uniform(const std::string& name, float f1, float f2, float f3, float f4);   // 4 floats
    void set_uniform(const std::string& name, const glm::mat4& matrix);                  // 1 matrice 4x4

private:
    const char* read_shader(const std::string& path);
    void compile_shader(GLuint shader_id);
    void link_program();

    GLuint m_ID;
};

#endif
