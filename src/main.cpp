#include "Camera.h"
#include "GameEngine.h"
#include "Noise.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "Cube.h"
#include "glm/gtx/transform.hpp"
#include <GL/glew.h> // first
#include <GLFW/glfw3.h>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <math.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

static inline void init() {
  GameEngine &engine = GameEngine::getInstance();
  ShaderManager &shader_manager = ShaderManager::getInstance();

  // Compute Shader - Map Compute
  shader_manager.loadShader("mapCompute", "../res/shaders/map.comp");
  // Cube Repère Shader
  shader_manager.loadShader("cubeRepere", "../res/shaders/cube_repere.vert", "../res/shaders/cube_repere.frag");

  glEnable(GL_DEPTH_TEST);
}

static inline void draw(Camera cam) {
  glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 9000.0f);

  glm::mat4 view = glm::lookAt(
      glm::vec3(30, 20, 20),
      glm::vec3(0, 0, 0),
      glm::vec3(0, 1, 0));
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GameEngine &engine = GameEngine::getInstance();
  ShaderManager &shader_manager = ShaderManager::getInstance();
  std::shared_ptr<Shader> shader = shader_manager.getShader("cubeRepere");
  Cube c = Cube(shader);
  c.setPosition(glm::vec3(0, 0, 0));
  c.draw(proj, cam.get_view());
}

static inline void camera_settings(Camera cam, float current_time) {
  static GLfloat angle = 6.0;
  GLfloat dist = 10.0;
  cam.update(glm::vec3(dist * sin(angle), dist, dist * cos(angle)),
             glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  cam.update(glm::vec3(100.0, 100.0, 100.0), glm::vec3(0.0, 0.0, 0.0), glm::);
  angle += 0.008;
}

int main() {
  // GLFW
  if (glfwInit() != GLFW_TRUE) {
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow *window =
      glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                       "Minecraft lite - Moteur de jeux", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -100;
  }
  glfwMakeContextCurrent(window);

  // GLEW
  if (glewInit() != GLEW_OK) {
    glfwTerminate();
    return -200;
  }

  // OpenGL API
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  Noise noise = Noise(256);
  Texture tex = Texture(noise.m_buffer, 256, 256);
  tex.bind(0);

  init();
  Camera cam =
      Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

  auto lastTime = std::chrono::high_resolution_clock::now();
  while (glfwGetKey(window, GLFW_KEY_L) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float>(currentTime - lastTime).count();
    camera_settings(cam, dt);
    draw(cam);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
