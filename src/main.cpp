#include "Camera.h"
#include "Chunk.h"
#include "ChunkBuilder.h"
#include "World.h"
#include "Constants.h"
#include "Cube.h"
#include "GameEngine.h"
#include "Noise.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "glm/gtx/transform.hpp"
#include <GL/glew.h> // first
#include <GLFW/glfw3.h>
#include <chrono>
#include <functional>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <math.h>
#include <vector>

namespace C = Constants; //  Pour ne pas à avoir à écrire Constants:: à chaque fois

//TODO stocker dans des objets
static GLuint _buffer[1] = {0};
static std::vector<GLuint> _data(C::BLOCKS_PER_CHUNK, 0); // Initialise avec 0 par défaut
static const size_t _nb_threads_x = C::BLOCKS_PER_CHUNK / 1024;
static GLuint _vao = 0;

static inline void process_map() {
  GameEngine &engine = GameEngine::getInstance();
  ShaderManager &shader_manager = ShaderManager::getInstance();
  World &world = World::getInstance();
  ChunkBuilder chunkbuilder = ChunkBuilder();
  world.initChunk(0, 0);
  chunkbuilder.build(world.getChunk(0, 0));
  return;

  std::shared_ptr<Shader> shader = shader_manager.getShader("mapCompute");
  Noise noise = Noise(256);
  Texture tex = Texture(noise.m_buffer, 256, 256);
  tex.bind(0);
  shader->set_uniform("permTexture", 0);

  std::vector<GLuint> data(C::BLOCKS_PER_CHUNK, 0);
  // Génération des buffers
  glGenBuffers(1, _buffer);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, _buffer[0]);
  glBufferData(GL_SHADER_STORAGE_BUFFER, C::BLOCKS_PER_CHUNK * sizeof(GLuint), data.data(), GL_DYNAMIC_DRAW);
  //
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _buffer[0]);
  glBufferData(GL_ARRAY_BUFFER, C::BLOCKS_PER_CHUNK * sizeof(GLuint), data.data(),
               GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(GLuint),
                        (const void *)0);
  // Compute Shader
  shader->use();
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _buffer[0]);
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  glDispatchCompute(_nb_threads_x, 1, 1);
  shader->stop();
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

static inline void init() {
  GameEngine &engine = GameEngine::getInstance();
  ShaderManager &shader_manager = ShaderManager::getInstance();

  // Compute Shader - Map Compute
  shader_manager.loadShader("mapCompute", "../res/shaders/map.comp");
  // Map Draw Shader
  shader_manager.loadShader("mapDraw", "../res/shaders/voxels.vert",
                            "../res/shaders/voxels.frag",
                            "../res/shaders/voxels.geom");
  // Cube Repère Shader
  shader_manager.loadShader("cubeRepere", "../res/shaders/cube_repere.vert",
                            "../res/shaders/cube_repere.frag");

  process_map();
  glEnable(GL_DEPTH_TEST);
}
static inline void draw_map(Camera cam) {
  glm::vec4 light_pos = glm::vec4(300.0, 500.0, 200.0, 1.0);
  GameEngine &engine = GameEngine::getInstance();
  ShaderManager &shader_manager = ShaderManager::getInstance();
  World &world = World::getInstance();
  std::shared_ptr<Shader> shader = shader_manager.getShader("mapDraw");
  shader->use();
  //DRAW
  Texture grass("../res/textures/grass.jpg");
  Texture water("../res/textures/water.jpg");
  grass.bind(0);
  water.bind(1);
  shader->set_uniform("grass_tex", 0);
  shader->set_uniform("water_tex", 1);


  glm::mat4 mvp = cam.get_proj() * cam.get_view();
  glBindVertexArray(world.getChunk(0, 0).get_vao()); // Associer VAO
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->set_uniform("Lp", light_pos);
  shader->set_uniform("MVP", mvp);
  shader->set_uniform("view", cam.get_view());

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, world.getChunk(0, 0).get_buffer());
  glDrawArrays(GL_POINTS, 0, C::BLOCKS_PER_CHUNK); // 1 vertex par bloc

  shader->stop();
  glBindVertexArray(0);
  glUseProgram(0);
}

static inline void draw_cube_repere(Camera cam) {
  GameEngine &engine = GameEngine::getInstance();
  ShaderManager &shader_manager = ShaderManager::getInstance();
  std::shared_ptr<Shader> shader = shader_manager.getShader("cubeRepere");
  Cube c = Cube(shader);
  c.setPosition(glm::vec3(0, 0, 0));
  c.draw(cam.get_proj(), cam.get_view());
  draw_map(cam);
  glBindVertexArray(0);
  shader->stop();
}

static inline void draw(Camera cam){
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  draw_cube_repere(cam);
  draw_map(cam);
}

static inline void camera_settings(Camera &cam, float current_time) {
  static GLfloat angle = 6.0;
  GLfloat dist = 100.0;
  GLfloat vit = 1.0;
  cam.update(
      glm::vec3(1.2 * dist * sin(vit * current_time), dist * 0.5, dist * cos(vit * current_time)),
      glm::vec3(0.2 * dist, 0.1 * dist, 0.2 * dist), glm::vec3(0.0, 1.0, 0.0));
}

int main() {
  // GLFW
  if (glfwInit() != GLFW_TRUE) {
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glEnable(GL_MULTISAMPLE);
  GLFWwindow *window =
      glfwCreateWindow(C::WINDOW_WIDTH, C::WINDOW_HEIGHT,
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
  glViewport(0, 0, C::WINDOW_WIDTH, C::WINDOW_HEIGHT);

  init();
  Camera cam = Camera(glm::vec3(50, 20, 30), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
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
