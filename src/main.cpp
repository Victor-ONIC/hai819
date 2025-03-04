#include <GL/glew.h>  // first
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Shader.h"
#include "Texture.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 675

int main()
{

    // GLFW
    if (glfwInit() != GLFW_TRUE)
    {
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Minecraft lite - Moteur de jeux", NULL, NULL);
    if (!window) 
    {
        glfwTerminate();
        return -100;
    }
    glfwMakeContextCurrent(window);

    // GLEW
    if (glewInit() != GLEW_OK) 
    {
        glfwTerminate();
        return -200;
    }

    // DONNÉES
    float cube[] =
    {
        // 6 faces, chaque face 2 triangles, chaque triangle 3 sommets, chaque sommets 5 floats.
        // position           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
 
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
 
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 positions_cubes[] =
    {
        // 10 cubes
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // OpenGL API
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // 1. Créer un VAO, et le bind.
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // 2. Créer un VBO, le bind à GL_ARRAY_BUFFER, et injecter les données de sommets.
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    // 3. Créer un EBO, le bind à GL_ELEMENT_ARRAY_BUFFER, et injecter les données d'indices.
    // Pas d'EBO ici.

    // 4. Dire à OpenGL comment interpréter les données de sommet.
    // Autrement dit, quel est le format des données contenues dans le buffer ?
    // Attribut 0: position:
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    // Attribut 1: coordonnées de texture:
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));

    // Noter que les actions de 2., 3., et 4. sont stockées dans le VAO.
    // C'est pour ça qu'on bind le VAO en 1., et à chaque fois qu'on
    // va bind ce VAO, c'est comme si on faisait 2., 3., et 4. !


    // PROGRAMME SHADER
    // Utiliser ma classe Shader pour créer et utiliser un programme shader.
    Shader programme_shader("../res/shaders/vertex_shader.vert", "../res/shaders/fragment_shader.frag");
    programme_shader.use();


    // TEXTURES
    Texture container("../res/textures/container.jpg");
    Texture face("../res/textures/awesomeface.png");

    container.bind(0);
    face.bind(1);

    programme_shader.set_uniform("texture1", 0);
    programme_shader.set_uniform("texture2", 1);


    // MATRICES
    // Matrice de modèle -> boucle de rendu.

    // Matrice de vue -> CAMÉRA.

    // Matrice de projection (perspective).
    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 1000.0f/750.0f, 1.0f, 100.0f);
    programme_shader.set_uniform("projection", projection);


    // CAMÉRA -> boucle de rendu.

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float radius = 6;
        float X = sin(glfwGetTime()) * radius;
        float Z = cos(glfwGetTime()) * radius;
        glm::mat4 view = glm::lookAt(glm::vec3(X,0,Z), glm::vec3(0,0,0), glm::vec3(0,1,0));
        programme_shader.set_uniform("view", view);

        for (int i = 0; i < 10; i++)
        {
            glm::mat4 model(1.0f);
            model = glm::translate(model, positions_cubes[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
            programme_shader.set_uniform("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}
