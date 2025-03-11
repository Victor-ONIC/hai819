#include "Shader.h"

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

void Shader::set_uniform(const std::string& name, int i)
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), i);
}

void Shader::set_uniform(const std::string& name, float f)
{
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), f);
}

void Shader::set_uniform(const std::string& name, float f1, float f2, float f3)
{
    glUniform3f(glGetUniformLocation(m_ID, name.c_str()), f1, f2, f3);
}

void Shader::set_uniform(const std::string& name, float f1, float f2, float f3, float f4)
{
    glUniform4f(glGetUniformLocation(m_ID, name.c_str()), f1, f2, f3, f4);
}

void Shader::set_uniform(const std::string& name, const glm::vec4& vec) {
    glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::set_uniform(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
