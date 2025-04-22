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


static inline void draw_chunk_faces(Chunk * chunk, Camera cam) {
  glm::mat4 mvp;
  glm::vec4 light_pos = glm::vec4(0.0, 50000.0, 10000.0, 1.0);
  GameEngine &engine = GameEngine::getInstance();
  ShaderManager &shader_manager = ShaderManager::getInstance();

  std::shared_ptr<Shader> shader = shader_manager.getShader("mapDrawFaces");
  shader->use();
  // TODO l'ordre des 4 lignes ci-dessous doit être garder sinon ça casse, ce
  // qui n'est pas normal
  Texture grass("../res/textures/grass.jpg"); // TODO avoir un Texture Manager
  Texture water("../res/textures/water.jpg");
  Texture cobble("../res/textures/cobble.jpg");
  grass.bind(0);
  water.bind(1);
  cobble.bind(2);
  shader->set_uniform("grass_tex", 0);
  shader->set_uniform("water_tex", 1);
  shader->set_uniform("cobble_tex", 2);
  mvp = cam.get_proj() * cam.get_view();
  glBindVertexArray(chunk->get_vao_faces()); // Associer VAO
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->set_uniform("Lp", light_pos);
  shader->set_uniform("MVP", mvp);
  shader->set_uniform("view", cam.get_view());
  shader->set_uniform("offsetx", float(chunk->get_xz()[0]));
  shader->set_uniform("offsetz", float(chunk->get_xz()[1]));

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, chunk->get_faces_buffer());
  // Dessiner les instances
  glDrawArraysInstanced(GL_TRIANGLES, 0, 6, chunk->get_counter_faces());
  //glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 36);


  shader->stop();
  glBindVertexArray(0);
  glUseProgram(0);
}

static inline void draw_all_chunk(Camera cam){
  World &world = World::getInstance();
  for (auto &[key, chunkPtr] : world.m_chunks) {
    if (chunkPtr) {
      draw_chunk_faces(chunkPtr.get(), cam);
    }
  }
}

static inline void draw_chunk_uint(Camera cam) {
  glm::vec4 light_pos = glm::vec4(300.0, 500.0, 200.0, 1.0);
  GameEngine &engine = GameEngine::getInstance();
  ShaderManager &shader_manager = ShaderManager::getInstance();
  World &world = World::getInstance();
  Chunk *chunk = world.tryGetChunk(0, 0);

  std::shared_ptr<Shader> shader = shader_manager.getShader("mapDrawUint");
  shader->use();
  // TODO l'ordre des 4 lignes ci-dessous doit être garder sinon ça casse, ce
  // qui n'est pas normal
  Texture grass("../res/textures/grass.jpg"); // TODO avoir un Texture Manager
  Texture water("../res/textures/water.jpg");
  Texture cobble("../res/textures/cobble.jpg");
  grass.bind(0);
  water.bind(1);
  cobble.bind(2);
  shader->set_uniform("grass_tex", 0);
  shader->set_uniform("water_tex", 1);
  shader->set_uniform("cobble_tex", 2);
  glm::mat4 mvp = cam.get_proj() * cam.get_view();
  glBindVertexArray(chunk->get_vao_blocktype()); // Associer VAO
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->set_uniform("Lp", light_pos);
  shader->set_uniform("MVP", mvp);
  shader->set_uniform("view", cam.get_view());

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, chunk->get_blocktype_buffer());
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

  // Compute Shader - Map Compute Height
  shader_manager.loadShader("mapComputeHeight", "../res/shaders/map.comp");
  shader = shader_manager.getShader("mapComputeHeight");
  shader->use();
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->set_num_groups(C::CHUNK_WIDTH/8, C::CHUNK_HEIGHT/8, C::CHUNK_DEPTH/8);
  shader->stop();

  // Compute Shader - Map Compute 3D
  shader_manager.loadShader("mapCompute3D", "../res/shaders/map3D.comp");
  shader = shader_manager.getShader("mapCompute3D");
  shader->use();
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->set_num_groups(C::CHUNK_WIDTH/8, C::CHUNK_HEIGHT/8, C::CHUNK_DEPTH/8);
  shader->stop();
  //
  // Compute Shader - Grass Compute
  shader_manager.loadShader("grassCompute", "../res/shaders/grass.comp");
  shader = shader_manager.getShader("grassCompute");
  shader->use();
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->set_num_groups(C::CHUNK_WIDTH/8, C::CHUNK_HEIGHT/8, C::CHUNK_DEPTH/8);
  shader->stop();

  // Compute Shader - Water Compute
  shader_manager.loadShader("waterCompute", "../res/shaders/water.comp");
  shader = shader_manager.getShader("waterCompute");
  shader->use();
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  //shader->set_num_groups(C::BLOCKS_PER_CHUNK/1024, 1, 1);
  shader->set_num_groups(C::CHUNK_WIDTH/8, C::CHUNK_HEIGHT/8, C::CHUNK_DEPTH/8);
  shader->stop();

  // Compute Shader - Gen Vertices
  shader_manager.loadShader("genVertices", "../res/shaders/gen_vertices.comp");
  shader = shader_manager.getShader("genVertices");
  shader->use();
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->set_num_groups(C::CHUNK_WIDTH/8, C::CHUNK_HEIGHT/8, C::CHUNK_DEPTH/8);
  shader->stop();

  // Uint Map Draw Shader
  shader_manager.loadShader("mapDrawUint", "../res/shaders/voxels.vert",
                            "../res/shaders/voxels.frag",
                            "../res/shaders/voxels.geom");

  // Faces Map Draw Shader
  shader_manager.loadShader("mapDrawFaces", "../res/shaders/faces.vert",
                            "../res/shaders/faces.frag");
  // Cube Repère Shader
  shader_manager.loadShader("cubeRepere", "../res/shaders/cube_repere.vert",
                            "../res/shaders/cube_repere.frag");

  World &world = World::getInstance();
  ChunkBuilder chunkbuilder = ChunkBuilder(); // TODO Singleton Pattern ou un Manager
  world.initChunk(0, 0);
  chunkbuilder.build(world.tryGetChunk(0, 0));

  /*
  world.initChunk(0, 1);
  chunkbuilder.build(world.tryGetChunk(0, 1));

  world.initChunk(1, 1);
  chunkbuilder.build(world.tryGetChunk(1, 1));

  world.initChunk(1, 0);
  chunkbuilder.build(world.tryGetChunk(1, 0));

  world.initChunk(1, 1);
  chunkbuilder.build(world.tryGetChunk(1, 1));

  world.initChunk(1, 2);
  chunkbuilder.build(world.tryGetChunk(1, 2));

  world.initChunk(2, 0);
  chunkbuilder.build(world.tryGetChunk(2, 0));

  world.initChunk(2, 1);
  chunkbuilder.build(world.tryGetChunk(2, 1));

  world.initChunk(2, 2);
  chunkbuilder.build(world.tryGetChunk(2, 2));
  */
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
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  draw_cube_repere(cam);
  //draw_chunk_uint(cam);
  //draw_chunk_faces(cam);
  draw_all_chunk(cam);
}

static inline void camera_settings(Camera &cam, float current_time) {
  static GLfloat angle = 6.0;
  GLfloat dist = 290.0;
  GLfloat vit = 0.4;
  /*
  cam.update(
            glm::vec3(-80.0, 250.0, -80.0),
            glm::vec3((GLfloat)C::CHUNK_WIDTH, 150.0, (GLfloat)C::CHUNK_DEPTH),
            glm::vec3(0.0, 1.0, 0.0));
            */
  cam.update(glm::vec3(1.5 * dist * sin(vit * current_time), 150.0,
                       1.2 * dist * cos(vit * current_time)),
             glm::vec3((GLfloat)C::CHUNK_WIDTH/2, 50.0, (GLfloat)C::CHUNK_DEPTH/2),
             glm::vec3(0.0, 1.0, 0.0));
}

int main() {
  srand(time(NULL));
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
      Camera(glm::vec3(100, 100, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
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
