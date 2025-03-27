#include "Shader.h"
#include <any>

namespace C = Constants;

Shader::Shader(const std::string& cs_path)
{
    std::string tmp = this->read_shader(cs_path);
    const char* cs_code = tmp.c_str();

    GLuint cs_id = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(cs_id, 1, &cs_code, NULL);
    this->compile_shader(cs_id);
    
    m_ID = glCreateProgram();
    glAttachShader(m_ID, cs_id);
    this->link_program();

    glDeleteShader(cs_id);
}

Shader::Shader(const std::string& vs_path, const std::string& fs_path, const std::string& gs_path)
{
    std::string tmp;
    const char *vs_code, *fs_code, *gs_code;
    GLuint vs_id, fs_id, gs_id;

    m_ID = glCreateProgram();

    tmp = this->read_shader(vs_path);
    vs_code = tmp.c_str();
    vs_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs_id, 1, &vs_code, NULL);
    this->compile_shader(vs_id);
    glAttachShader(m_ID, vs_id);
    
    tmp = this->read_shader(fs_path);
    fs_code = tmp.c_str();
    fs_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs_id, 1, &fs_code, NULL);
    this->compile_shader(fs_id);
    glAttachShader(m_ID, fs_id);

    if (!gs_path.empty())
    {
        tmp = this->read_shader(gs_path);
        gs_code = tmp.c_str();
        gs_id = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gs_id, 1, &gs_code, NULL);
        this->compile_shader(gs_id);
        glAttachShader(m_ID, gs_id);
    }

    this->link_program();

    glDeleteShader(vs_id);
    glDeleteShader(fs_id);
}

std::string Shader::read_shader(const std::string& path)
{
    std::string code_string;
    std::ifstream shader_file(path);
    if (!shader_file)
        std::cout << "ERREUR::SHADER - Fichier " << path << '\n';

    // Extraire le texte du fichier
    std::stringstream ss;
    ss << shader_file.rdbuf();
    shader_file.close();

    return ss.str();
}

void Shader::compile_shader(GLuint shader_id)
{
    int success = 0;
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        int length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];

        glGetShaderInfoLog(shader_id, length, NULL, message);
        std::cout << "ERREUR::SHADER::COMPILATION - Vertex shader:\n";
        std::cout << message << std::endl;

        delete[] message;
    }
}

void Shader::link_program()
{
    int success = 0;
    glLinkProgram(m_ID);
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        int length;
        glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];

        glGetProgramInfoLog(m_ID, length, NULL, message);
        std::cout << "ERREUR::SHADER::LINK - Link programme shader:\n";
        std::cout << message << std::endl;

        delete[] message;
    }
}

Shader::~Shader()
{
    glDeleteProgram(m_ID);
}

void Shader::use()
{
    glUseProgram(m_ID);
}

void Shader::stop()
{
    glUseProgram(0);
}

// Ci-dessous tout les set_uniform correspondant aux types ValueType définit
// dans Constants.h
//
// Scalaires
void Shader::set_uniform(const std::string &name, int i) {
  m_uniforms[name] = i;
  glUniform1i(glGetUniformLocation(m_ID, name.c_str()), i);
}
void Shader::set_uniform(const std::string &name, unsigned int ui) {
  m_uniforms[name] = ui;
  glUniform1ui(glGetUniformLocation(m_ID, name.c_str()), ui);
}
void Shader::set_uniform(const std::string &name, float f) {
  m_uniforms[name] = f;
  glUniform1f(glGetUniformLocation(m_ID, name.c_str()), f);
}

void Shader::set_uniform(const std::string &name, double d) {
  m_uniforms[name] = d;
  glUniform1f(glGetUniformLocation(m_ID, name.c_str()), d);
}

// Vecteurs 2D
void Shader::set_uniform(const std::string &name, int i1, int i2) {
    m_uniforms[name] = std::pair(i1, i2);
  glUniform2i(glGetUniformLocation(m_ID, name.c_str()), i1, i2);
}
void Shader::set_uniform(const std::string &name, unsigned int ui1, unsigned int ui2) {
    m_uniforms[name] = std::pair(ui1, ui2);
  glUniform2ui(glGetUniformLocation(m_ID, name.c_str()), ui1, ui2);
}
void Shader::set_uniform(const std::string &name, float f1, float f2) {
    m_uniforms[name] = std::pair(f1, f2);
  glUniform2f(glGetUniformLocation(m_ID, name.c_str()), f1, f2);
}
void Shader::set_uniform(const std::string& name, const glm::vec2& vec) {
    m_uniforms[name] = vec;
    glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(vec));
}

// Vecteurs 3D
void Shader::set_uniform(const std::string &name, int i1, int i2, int i3) {
    m_uniforms[name] = std::tuple(i1, i2, i3);
  glUniform3i(glGetUniformLocation(m_ID, name.c_str()), i1, i2, i3);
}
void Shader::set_uniform(const std::string &name, unsigned int ui1, unsigned int ui2, unsigned int ui3) {
    m_uniforms[name] = std::tuple(ui1, ui2, ui3);
  glUniform3ui(glGetUniformLocation(m_ID, name.c_str()), ui1, ui2, ui3);
}
void Shader::set_uniform(const std::string &name, float f1, float f2, float f3) {
    m_uniforms[name] = std::tuple(f1, f2, f3);
  glUniform3f(glGetUniformLocation(m_ID, name.c_str()), f1, f2, f3);
}
void Shader::set_uniform(const std::string& name, const glm::vec3& vec) {
    m_uniforms[name] = vec;
    glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(vec));
}

// Vecteurs 4D
void Shader::set_uniform(const std::string &name, int i1, int i2, int i3, int i4) {
    m_uniforms[name] = std::tuple(i1, i2, i3, i4);
  glUniform4i(glGetUniformLocation(m_ID, name.c_str()), i1, i2, i3, i4);
}
void Shader::set_uniform(const std::string &name, unsigned int ui1, unsigned int ui2, unsigned int ui3, unsigned int ui4) {
    m_uniforms[name] = std::tuple(ui1, ui2, ui3, ui4);
  glUniform4ui(glGetUniformLocation(m_ID, name.c_str()), ui1, ui2, ui3, ui4);
}
void Shader::set_uniform(const std::string &name, float f1, float f2, float f3,
                         float f4) {
    m_uniforms[name] = std::tuple(f1, f2, f3, f4);
  glUniform4f(glGetUniformLocation(m_ID, name.c_str()), f1, f2, f3, f4);
}
void Shader::set_uniform(const std::string& name, const glm::vec4& vec) {
    m_uniforms[name] = vec;
    glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(vec));
}

// Matrice 2x2
void Shader::set_uniform(const std::string &name, const glm::mat2 &matrix) {
    m_uniforms[name] = matrix;
  glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(matrix));
}

// Matrice 3x3
void Shader::set_uniform(const std::string &name, const glm::mat3 &matrix) {
    m_uniforms[name] = matrix;
  glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(matrix));
}

// Matrice 4x4
void Shader::set_uniform(const std::string &name, const glm::mat4 &matrix) {
    m_uniforms[name] = matrix;
  glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(matrix));
}

/*
void Shader::set_all_uniforms(){
    for (const auto& pair : m_uniforms) {
        std::cout << "Clé: " << pair.first << std::endl;
        set_uniform(pair.first, pair.second);
    }
}
*/
/*
void Shader::set_all_uniforms() {
    for (const auto& pair : m_uniforms) {
        std::cout << "Clé: " << pair.first << std::endl;

        // Utiliser std::visit pour déterminer le type et appeler la fonction correspondante
        std::visit([this, &pair](auto&& value) {
            using T = std::decay_t<decltype(value)>;

            // Traitement des scalaires
            if constexpr (std::is_same_v<T, int>) {
                set_uniform(pair.first, value);
            } else if constexpr (std::is_same_v<T, unsigned int>) {
                set_uniform(pair.first, value);
            } else if constexpr (std::is_same_v<T, float>) {
                set_uniform(pair.first, value);
            } else if constexpr (std::is_same_v<T, double>) {
                set_uniform(pair.first, value);
            }

            // Traitement des vecteurs 2D
            else if constexpr (std::is_same_v<T, std::pair<int, int>>) {
                set_uniform(pair.first, value.first, value.second);
            } else if constexpr (std::is_same_v<T, std::pair<unsigned int, unsigned int>>) {
                set_uniform(pair.first, value.first, value.second);
            } else if constexpr (std::is_same_v<T, std::pair<float, float>>) {
                set_uniform(pair.first, value.first, value.second);
            } else if constexpr (std::is_same_v<T, glm::vec2>) {
                set_uniform(pair.first, value);
            }

            // Traitement des vecteurs 3D
            else if constexpr (std::is_same_v<T, std::tuple<int, int, int>>) {
                set_uniform(pair.first, std::get<0>(value), std::get<1>(value), std::get<2>(value));
            } else if constexpr (std::is_same_v<T, std::tuple<unsigned int, unsigned int, unsigned int>>) {
                set_uniform(pair.first, std::get<0>(value), std::get<1>(value), std::get<2>(value));
            } else if constexpr (std::is_same_v<T, std::tuple<float, float, float>>) {
                set_uniform(pair.first, std::get<0>(value), std::get<1>(value), std::get<2>(value));
            } else if constexpr (std::is_same_v<T, glm::vec3>) {
                set_uniform(pair.first, value);
            }

            // Traitement des vecteurs 4D
            else if constexpr (std::is_same_v<T, std::tuple<int, int, int, int>>) {
                set_uniform(pair.first, std::get<0>(value), std::get<1>(value), std::get<2>(value), std::get<3>(value));
            } else if constexpr (std::is_same_v<T, std::tuple<unsigned int, unsigned int, unsigned int, unsigned int>>) {
                set_uniform(pair.first, std::get<0>(value), std::get<1>(value), std::get<2>(value), std::get<3>(value));
            } else if constexpr (std::is_same_v<T, std::tuple<float, float, float, float>>) {
                set_uniform(pair.first, std::get<0>(value), std::get<1>(value), std::get<2>(value), std::get<3>(value));
            } else if constexpr (std::is_same_v<T, glm::vec4>) {
                set_uniform(pair.first, value);
            }

            // Traitement des matrices 2x2
            else if constexpr (std::is_same_v<T, glm::mat2>) {
                set_uniform(pair.first, value);
            } else if constexpr (std::is_same_v<T, std::array<std::array<float, 2>, 2>>) {
                set_uniform(pair.first, value);
            }

            // Traitement des matrices 3x3
            else if constexpr (std::is_same_v<T, glm::mat3>) {
                set_uniform(pair.first, value);
            } else if constexpr (std::is_same_v<T, std::array<std::array<float, 3>, 3>>) {
                set_uniform(pair.first, value);
            }

            // Traitement des matrices 4x4
            else if constexpr (std::is_same_v<T, glm::mat4>) {
                set_uniform(pair.first, value);
            } else if constexpr (std::is_same_v<T, std::array<std::array<float, 4>, 4>>) {
                set_uniform(pair.first, value);
            }
        }, pair.second);
    }
}
*/
