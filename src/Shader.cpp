#include "Shader.h"

Shader::Shader(const std::string& VS_path, const std::string& FS_path)
{
    /* 
     * Les abbréviations suivantes sont utilisées:
     * VS: Vertex Shader
     * FS: Fragment Shader
     * 
     * 1. Lire les fichiers dans des const char*;
     * 2. Créer les shaders, leur donner le code et les compiler;
     * 3. Créer le programme, y attacher les shaders, et le linker;
     * 4. Supprimer les shaders.
    */

    int success;

    // 1. Lire les fichiers dans des const char*.
    std::string VS_code_string;
    std::string FS_code_string;

    std::ifstream VS_file(VS_path);
    if (!VS_file)
    {
        std::cout << "ERREUR::SHADER - Fichier vertex shader\n";
    }

    std::ifstream FS_file(FS_path);
    if (!FS_file)
    {
        std::cout << "ERREUR::SHADER - Fichier fragment shader\n";
    }

    std::stringstream VS;
    std::stringstream FS;

    VS << VS_file.rdbuf();
    FS << FS_file.rdbuf();
    VS_file.close();
    FS_file.close();

    VS_code_string = VS.str();
    FS_code_string = FS.str();

    const char* VS_code = VS_code_string.c_str();
    const char* FS_code = FS_code_string.c_str();

    // 2. Créer les shaders, leur donner le code et les compiler.
    // Créer les deux shaders.
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // Injecter le code source.
    glShaderSource(vertex_shader, 1, &VS_code, NULL);
    glShaderSource(fragment_shader, 1, &FS_code, NULL);

    // Compiler et afficher les erreurs s'il y en a.
    success = 0;
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        int length;
        glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];

        glGetShaderInfoLog(vertex_shader, length, NULL, message);
        std::cout << "ERREUR::SHADER::COMPILATION - Vertex shader:\n";
        std::cout << message << std::endl;

        delete[] message;
    }

    success = 0;
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        int length;
        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];

        glGetShaderInfoLog(fragment_shader, length, NULL, message);
        std::cout << "ERREUR::SHADER::COMPILATION - Fragment shader:\n";
        std::cout << message << std::endl;

        delete[] message;
    }

    // 3. Créer le programme, y attacher les shaders, et le linker.
    ID = glCreateProgram();
    glAttachShader(ID, vertex_shader);
    glAttachShader(ID, fragment_shader);

    success = 0;
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        int length;
        glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];

        glGetProgramInfoLog(ID, length, NULL, message);
        std::cout << "ERREUR::SHADER::LINK - Link programme shader:\n";
        std::cout << message << std::endl;

        delete[] message;
    }

    // 4. Supprimer les shaders maintenant qu'on a le programme.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::set_uniform(const std::string& name, int i)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), i);
}

void Shader::set_uniform(const std::string& name, float f)
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), f);
}

void Shader::set_uniform(const std::string& name, float f1, float f2, float f3)
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), f1, f2, f3);
}

void Shader::set_uniform(const std::string& name, float f1, float f2, float f3, float f4)
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), f1, f2, f3, f4);
}

void Shader::set_uniform(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
