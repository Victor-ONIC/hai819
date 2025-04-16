#include "Camera.h"
#include "Chunk.h"
#include "ChunkBuilder.h"
#include "Constants.h"
#include "Cube.h"
#include "GameEngine.h"
#include "Noise.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "World.h"
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

namespace C = Constants;

static inline void draw_chunk(Camera cam) {
  glm::vec4 light_pos = glm::vec4(300.0, 500.0, 200.0, 1.0);
  GameEngine &engine = GameEngine::getInstance();
  ShaderManager &shader_manager = ShaderManager::getInstance();
  World &world = World::getInstance();
  Chunk *chunk = world.tryGetChunk(0, 0);

  std::shared_ptr<Shader> shader = shader_manager.getShader("mapDraw");
  shader->use();
  // DRAW
  // TODO l'ordre des 4 lignes ci-dessous doit être garder sinon ça casse, ce
  // qui n'est pas normal
  Texture grass("../res/textures/grass.jpg"); // TODO avoir un Texture Manager
  Texture water("../res/textures/water.jpg");
  grass.bind(0);
  water.bind(1);
  shader->set_uniform("grass_tex", 0);
  shader->set_uniform("water_tex", 1);
  glm::mat4 mvp = cam.get_proj() * cam.get_view();
  glBindVertexArray(chunk->get_vao()); // Associer VAO
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->set_uniform("Lp", light_pos);
  shader->set_uniform("MVP", mvp);
  shader->set_uniform("view", cam.get_view());

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, chunk->get_buffer());
  glDrawArrays(GL_POINTS, 0, C::BLOCKS_PER_CHUNK); // 1 vertex par bloc

  shader->stop();
  glBindVertexArray(0);
  glUseProgram(0);
}

static inline void process_chunk(Chunk &chunk) {
  GameEngine &engine = GameEngine::getInstance();
  ShaderManager &shader_manager = ShaderManager::getInstance();
  ChunkBuilder chunkbuilder = ChunkBuilder();
  chunkbuilder.build(&chunk);
}

static inline void init() {
  GameEngine &engine = GameEngine::getInstance();
  ShaderManager &shader_manager = ShaderManager::getInstance();
  std::shared_ptr<Shader> shader;

  // Compute Shader - Map Compute
  shader_manager.loadShader("mapCompute", "../res/shaders/map.comp");
  shader = shader_manager.getShader("mapCompute");
  shader->use();
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->stop();

  // Compute Shader - Water Compute
  shader_manager.loadShader("waterCompute", "../res/shaders/water.comp");
  shader = shader_manager.getShader("waterCompute");
  shader->use();
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->stop();

  // Map Draw Shader
  shader_manager.loadShader("mapDraw", "../res/shaders/voxels.vert",
                            "../res/shaders/voxels.frag",
                            "../res/shaders/voxels.geom");
  // Cube Repère Shader
  shader_manager.loadShader("cubeRepere", "../res/shaders/cube_repere.vert",
                            "../res/shaders/cube_repere.frag");

  World &world = World::getInstance();
  ChunkBuilder chunkbuilder =
      ChunkBuilder(); // TODO Singleton Pattern ou un Manager
  world.initChunk(0, 0);
  chunkbuilder.build(world.tryGetChunk(0, 0));
  glEnable(GL_DEPTH_TEST);
}

static inline void draw_cube_repere(Camera cam) {
  GameEngine &engine = GameEngine::getInstance();
  ShaderManager &shader_manager = ShaderManager::getInstance();
  std::shared_ptr<Shader> shader = shader_manager.getShader("cubeRepere");
  Cube c = Cube(shader);
  c.setPosition(glm::vec3(0, 0, 0));
  c.draw(cam.get_proj(), cam.get_view());
  glBindVertexArray(0);
  shader->stop();
}

static inline void draw(Camera cam) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  draw_cube_repere(cam);
  draw_chunk(cam);
}

static inline void camera_settings(Camera &cam, float current_time) {
  static GLfloat angle = 6.0;
  GLfloat dist = 150.0;
  GLfloat vit = 0.1;
  cam.update(glm::vec3(1.2 * dist * sin(vit * current_time), dist * 0.4,
                       dist * cos(vit * current_time)),
             glm::vec3(0.3 * dist, 0.1 * dist, 0.4 * dist),
             glm::vec3(0.0, 1.0, 0.0));
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
  Camera cam =
      Camera(glm::vec3(50, 20, 30), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
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
