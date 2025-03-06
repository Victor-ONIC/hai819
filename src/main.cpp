#include <GL/glew.h>  // first
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Shader.h"
#include "Noise.h"
#include "Texture.h"
#include "Camera.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 675
static GLuint _cube_buffer[2] = {0};
static GLuint _cube_vao = 0;

void draw_cube(Shader prog, glm::mat4 mvp) {
  // Cube de simu
  //glUseProgram(prog.m_ID);
  prog.use();
  glLineWidth(1.0f);

  prog.set_uniform("MVP", mvp);
  glBindBuffer(GL_ARRAY_BUFFER, _cube_buffer[0]);
  glBindVertexArray(_cube_vao);
  glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT,
                 (const GLvoid *)(0 * sizeof(GLuint)));
  glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT,
                 (const GLvoid *)(4 * sizeof(GLuint)));
  glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT,
                 (const GLvoid *)(8 * sizeof(GLuint)));
  glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT,
                 (const GLvoid *)(12 * sizeof(GLuint)));
  glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT,
                 (const GLvoid *)(16 * sizeof(GLuint)));
  glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT,
                 (const GLvoid *)(20 * sizeof(GLuint)));
  glBindVertexArray(0);
  glUseProgram(0);
}

void init(){
  //srand(time(NULL));

  GLfloat cube[32] = // 8 sommets * 4 coord
      {/* sommet  0 */ -10, 10,  10,  0,
       /* sommet  1 */ 10,  10,  10,  0,
       /* sommet  2 */ 10,  10,  -10, 0,
       /* sommet  3 */ -10, 10,  -10, 0,
       /* sommet  4 */ -10, -10, 10,  0,
       /* sommet  5 */ 10,  -10, 10,  0,
       /* sommet  6 */ 10,  -10, -10, 0,
       /* sommet  7 */ -10, -10, -10, 0};
  GLuint icube[24] = {/* face haut du cube unitaire */
                      0, 1, 2, 3,
                      /* face bas du cube unitaire  */
                      4, 7, 6, 5,
                      /* face avant du cube unitaire  */
                      0, 4, 5, 1,
                      /* face arriere du cube unitaire  */
                      7, 3, 2, 6,
                      /* face gauche du cube unitaire  */
                      4, 0, 3, 7,
                      /* face droite du cube unitaire  */
                      5, 6, 2, 1};


  // CUBE
  glGenVertexArrays(1, &_cube_vao);
  glBindVertexArray(_cube_vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glGenBuffers(2, _cube_buffer);
  /* Lier le VBO-ARRAY_BUFFER à l'identifiant du premier VBO généré */
  glBindBuffer(GL_ARRAY_BUFFER, _cube_buffer[0]);
  /* Transfert des données VBO-ARRAY_BUFFER */
  glBufferData(GL_ARRAY_BUFFER, sizeof cube, cube, GL_STATIC_DRAW);
  /* Paramétrage 2 premiers indices d'attribut de sommet */
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof *cube,
                        (const void *)0);
  /* Lier le VBO-ELEMENT_ARRAY_BUFFER à l'identifiant du second VBO généré */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _cube_buffer[1]);
  /* Transfert des données d'indices VBO-ELEMENT_ARRAY_BUFFER */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof icube, icube, GL_STATIC_DRAW);
  /* dé-lier le VAO puis les VAO */
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

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
    // clang-format on

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // Attribut 1: coordonnées de texture:
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Noter que les actions de 2., 3., et 4. sont stockées dans le VAO.
    // C'est pour ça qu'on bind le VAO en 1., et à chaque fois qu'on
    // va bind ce VAO, c'est comme si on faisait 2., 3., et 4. !

    // PROGRAMME SHADER
    // Compute Shader
    Shader programme_cs("../res/shaders/map.comp");
    // Cube repère
    Shader programme_cube_repere("../res/shaders/cube_repere.vert",
                            "../res/shaders/cube_repere.frag");
    // Utiliser ma classe Shader pour créer et utiliser un programme shader.
    Shader programme_shader("../res/shaders/vertex_shader.vert",
                            "../res/shaders/fragment_shader.frag");
    programme_shader.use();

    // TEXTURES
    //Texture container("../res/textures/container.jpg");
    //Texture face("../res/textures/awesomeface.png");
    //
    Noise noise = Noise(256);
    Texture tex = Texture(noise.m_buffer, 256, 256);
    // TEXTURES

    //container.bind(0);
    //face.bind(0);
    tex.bind(0);
    programme_shader.set_uniform("permTexture", 0);

    init();
    // MATRICES
    // Matrice de modèle -> boucle de rendu.

    // Matrice de vue -> CAMÉRA.
    Camera cam = Camera(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,1,0));

    // Matrice de projection (perspective).
    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f), 1000.0f / 750.0f, 1.0f, 100.0f);
    programme_shader.set_uniform("projection", projection);

    // CAMÉRA -> boucle de rendu.

    while (glfwGetKey(window, GLFW_KEY_L) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0)
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float radius = 6;
        float X = sin(glfwGetTime()) * radius;
        float Z = cos(glfwGetTime()) * radius;
        Camera cam = Camera(glm::vec3(X,0,Z), glm::vec3(0,0,0), glm::vec3(0,1,0));
        programme_shader.set_uniform("view", cam.m_view);

        glm::mat4 cube_model = glm::mat4(1.0f);
        glm::mat4 mvp = projection * cam.m_view * cube_model; // Remember, matrix multiplication is the other way around
        draw_cube(programme_cube_repere, mvp);
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
