#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#define GLEW_STATIC
#include "Constants.h"
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace C = Constants;

class Shader {
public:
  Shader() = delete;
  Shader(const std::string &cs_path);
  Shader(const std::string &vs_path, const std::string &fs_path,
         const std::string &gs_path = "");
  ~Shader();

  void use();
  void stop();


// getID
GLuint getID() const { return m_ID; }

template <typename T>

void set_uniform(const std::string& name, T&& value) {
    if constexpr (std::is_same_v<std::decay_t<T>, int>) {
        m_uniforms[name] = value;
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, unsigned int>) {
        m_uniforms[name] = value;
        glUniform1ui(glGetUniformLocation(m_ID, name.c_str()), value);
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, float>) {
        m_uniforms[name] = value;
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, double>) {
        m_uniforms[name] = value;
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, std::pair<int, int>>) {
        m_uniforms[name] = value;
        glUniform2i(glGetUniformLocation(m_ID, name.c_str()), value.first, value.second);
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, std::pair<unsigned int, unsigned int>>) {
        m_uniforms[name] = value;
        glUniform2ui(glGetUniformLocation(m_ID, name.c_str()), value.first, value.second);
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, std::pair<float, float>>) {
        m_uniforms[name] = value;
        glUniform2f(glGetUniformLocation(m_ID, name.c_str()), value.first, value.second);
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, glm::vec2>) {
        m_uniforms[name] = value;
        glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, std::array<int, 3>>) {
        m_uniforms[name] = value;
        glUniform3i(glGetUniformLocation(m_ID, name.c_str()), value[0], value[1], value[2]);
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, std::array<unsigned int, 3>>) {
        m_uniforms[name] = value;
        glUniform3ui(glGetUniformLocation(m_ID, name.c_str()), value[0], value[1], value[2]);
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, std::array<float, 3>>) {
        m_uniforms[name] = value;
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value[0], value[1], value[2]);
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, glm::vec3>) {
        m_uniforms[name] = value;
        glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, std::array<int, 4>>) {
        m_uniforms[name] = value;
        glUniform4i(glGetUniformLocation(m_ID, name.c_str()), value[0], value[1], value[2], value[3]);
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, std::array<unsigned int, 4>>) {
        m_uniforms[name] = value;
        glUniform4ui(glGetUniformLocation(m_ID, name.c_str()), value[0], value[1], value[2], value[3]);
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, std::array<float, 4>>) {
        m_uniforms[name] = value;
        glUniform4f(glGetUniformLocation(m_ID, name.c_str()), value[0], value[1], value[2], value[3]);
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, glm::vec4>) {
        m_uniforms[name] = value;
        glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, glm::mat2>) {
        m_uniforms[name] = value;
        glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, glm::mat3>) {
        m_uniforms[name] = value;
        glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }
    else if constexpr (std::is_same_v<std::decay_t<T>, glm::mat4>) {
        m_uniforms[name] = value;
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }
}

void set_all_uniforms(){
    for (const auto& pair : m_uniforms) {
        std::cout << "Clé: " << pair.first << std::endl;
        set_uniform(pair.first, pair.second);
    }
}

private:
  std::string read_shader(const std::string &path);
  void compile_shader(GLuint shader_id);
  void link_program();
  GLuint m_ID;
  C::UniformMap m_uniforms;
};

#endif
