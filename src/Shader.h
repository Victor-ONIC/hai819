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

// Scalaires
void set_uniform(const std::string &name, int i);
void set_uniform(const std::string &name, unsigned int ui);
void set_uniform(const std::string &name, float f);
void set_uniform(const std::string &name, double d);

// Vecteurs 2D
void set_uniform(const std::string &name, int i1, int i2);
void set_uniform(const std::string &name, unsigned int ui1, unsigned int ui2);
void set_uniform(const std::string &name, float f1, float f2);
void set_uniform(const std::string& name, const glm::vec2& vec);

// Vecteurs 3D
void set_uniform(const std::string &name, int i1, int i2, int i3);
void set_uniform(const std::string &name, unsigned int ui1, unsigned int ui2, unsigned int ui3);
void set_uniform(const std::string &name, float f1, float f2, float f3);
void set_uniform(const std::string& name, const glm::vec3& vec);

// Vecteurs 4D
void set_uniform(const std::string &name, int i1, int i2, int i3, int i4);
void set_uniform(const std::string &name, unsigned int ui1, unsigned int ui2, unsigned int ui3, unsigned int ui4);
void set_uniform(const std::string &name, float f1, float f2, float f3,
                         float f4);
void set_uniform(const std::string& name, const glm::vec4& vec);

// Matrice 2x2
void set_uniform(const std::string &name, const glm::mat2 &matrix);

// Matrice 3x3
void set_uniform(const std::string &name, const glm::mat3 &matrix);

// Matrice 4x4
void set_uniform(const std::string &name, const glm::mat4 &matrix);

// Parcours m_uniforms et set_uniform sur chacun de ses éléments
void set_all_uniforms();

// getID
GLuint getID() const { return m_ID; }

private:
  std::string read_shader(const std::string &path);
  void compile_shader(GLuint shader_id);
  void link_program();
  GLuint m_ID;
  C::UniformMap m_uniforms;
};

#endif
