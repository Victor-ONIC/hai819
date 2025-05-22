#include "Camera.h"
#include "Chunk.h"
#include "ChunkBuilder.h"
#include "Constants.h"
#include "Param.h"
#include "Cube.h"
#include "GameEngine.h"
#include "Noise.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "TextureManager.h"
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
#include <random>
#include <vector>

namespace C = Constants;
namespace P = Param;

static inline void mouse_callback(GLFWwindow *window, double xpos,
                                  double ypos) {
  Camera *cam = static_cast<Camera *>(glfwGetWindowUserPointer(window));

  float xoffset, yoffset;
  if (cam->firstMouse) {
    cam->lastX = xpos;
    cam->lastY = ypos;
    cam->firstMouse = false;
  }

  xoffset = xpos - cam->lastX;
  yoffset = cam->lastY - ypos; // inversé car y va de bas en haut

  cam->lastX = xpos;
  cam->lastY = ypos;

  cam->process_mouse_movement(xoffset, yoffset);
}

static inline void print_vram() {
  GLint total_mem_kb = 0;
  glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total_mem_kb);
  std::cout << "Mémoire vidéo totale : " << total_mem_kb / 1024 << " Mo"
            << std::endl;

  GLint current_mem_kb = 0;
  glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX,
                &current_mem_kb);
  std::cout << "Mémoire disponible : " << current_mem_kb / 1024 << " Mo"
            << std::endl
            << std::endl;
}

static inline void draw_chunk_faces(Chunk *chunk, Camera cam) {
  glm::mat4 mvp;
  static glm::vec4 light_pos = glm::vec4(0.0, 2000.0, 5000.0, 1.0);
  GameEngine &engine = GameEngine::getInstance();
  ShaderManager &shader_manager = ShaderManager::getInstance();
  TextureManager &texture_manager = TextureManager::getInstance();
  std::shared_ptr<Texture> texture;

  std::shared_ptr<Shader> shader = shader_manager.getShader("mapDrawFaces");
  shader->use();

  texture = texture_manager.getTexture("grass_tex");
  texture->bind(0);
  texture = texture_manager.getTexture("water_tex");
  texture->bind(1);
  texture = texture_manager.getTexture("cobble_tex");
  texture->bind(2);
  texture = texture_manager.getTexture("bedrock_tex");
  texture->bind(3);
  texture = texture_manager.getTexture("lava_tex");
  texture->bind(4);
  texture = texture_manager.getTexture("sand_tex");
  texture->bind(5);
  shader->set_uniform("grass_tex", 0);
  shader->set_uniform("water_tex", 1);
  shader->set_uniform("cobble_tex", 2);
  shader->set_uniform("bedrock_tex", 3);
  shader->set_uniform("lava_tex", 4);
  shader->set_uniform("sand_tex", 5);
  mvp = cam.get_proj() * cam.get_view();
  glBindVertexArray(chunk->get_vao_faces()); // Associer VAO
  shader->set_uniform("Lp", light_pos);
  shader->set_uniform("MVP", mvp);
  shader->set_uniform("view", cam.get_view());
  shader->set_uniform("proj", cam.get_proj());
  shader->set_uniform("offsetx", float(chunk->get_xz()[0]));
  shader->set_uniform("offsetz", float(chunk->get_xz()[1]));
  shader->set_uniform("cam", cam.get_pos());
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, chunk->get_faces_buffer());
  // Dessiner les instances
  glDrawArraysInstanced(GL_TRIANGLES, 0, 6, chunk->get_counter_faces());

  shader->stop();
  glBindVertexArray(0);
  glUseProgram(0);
}

static inline void update_chunk_state(Chunk *chunk, Camera cam) {
  if (!(chunk->renderable) && chunk->m_fence) {
    GLenum res =
        glClientWaitSync(chunk->m_fence, 0, 0); // 0 timeout = non bloquant
    if (res == GL_ALREADY_SIGNALED || res == GL_CONDITION_SATISFIED) {
      glDeleteSync(chunk->m_fence);
      chunk->m_fence = nullptr;
      chunk->renderable = true;
      chunk->is_being_processed = false;
    }
  }
}
bool is_chunk_visible(Camera &cam, int chunk_x, int chunk_z) {
    float marge_fov = 30.0f;
    float min_dist = 1.5f * C::CHUNK_WIDTH; // chunks trop proches toujours visibles

    glm::vec3 cam_pos = cam.get_pos();
    glm::vec3 cam_dir = glm::normalize(cam.get_look_at() - cam_pos);

    float chunk_center_x = chunk_x * C::CHUNK_WIDTH + C::CHUNK_WIDTH * 0.5f;
    float chunk_center_z = chunk_z * C::CHUNK_DEPTH + C::CHUNK_DEPTH * 0.5f;

    float dist2 = (chunk_center_x - cam_pos.x) * (chunk_center_x - cam_pos.x) +
                  (chunk_center_z - cam_pos.z) * (chunk_center_z - cam_pos.z);

    if (dist2 < min_dist * min_dist)
        return true;

    glm::vec2 to_chunk = glm::normalize(glm::vec2(
        chunk_center_x - cam_pos.x,
        chunk_center_z - cam_pos.z
    ));

    glm::vec2 cam_forward = glm::normalize(glm::vec2(cam_dir.x, cam_dir.z));

    float dot = glm::dot(to_chunk, cam_forward);

    float angle_rad = glm::radians(P::fov * 0.5f + marge_fov);
    float threshold = cos(angle_rad);

    return dot > threshold;
}

static inline void draw_chunks_below(Camera cam) {
  World &world = World::getInstance();
  ChunkBuilder chunkbuilder = ChunkBuilder();
  Chunk *chunk;
  int cx = floor(cam.get_pos()[0] / C::CHUNK_WIDTH);
  int cz = floor(cam.get_pos()[2] / C::CHUNK_DEPTH);
  int radius = P::render_distance;
  int margin = P::margin_distance_erase_chunks;
  for (int dz = -radius; dz <= radius; ++dz) {
    for (int dx = -radius; dx <= radius; ++dx) {
      // distance au chunk central
      float dist = sqrt(dx * dx + dz * dz);
      if (dist > radius)
        continue; // on ignore les chunks hors rayon

      int chunk_x = cx + dx;
      int chunk_z = cz + dz;

      Chunk *chunk = world.tryGetChunk(chunk_x, chunk_z);
      if (chunk)
        update_chunk_state(chunk, cam);
      else{
        world.initChunk(chunk_x, chunk_z);
        continue;
      }

      if (chunk && chunk->renderable && is_chunk_visible(cam, chunk_x, chunk_z))
        draw_chunk_faces(chunk, cam);
    }
  }
  world.delete_chunks(cam, radius + margin);
  //world.print_nb_chunks();
}

static inline int generate_seed() {
  static std::random_device rd;
  static std::mt19937 gen(rd()); // bonne entropie
  static std::uniform_int_distribution<int> dis(
      -100000, 100000); // bornes selon ton besoin
  return dis(gen);
}

static inline void init(Camera cam) {
  GameEngine &engine = GameEngine::getInstance();
  TextureManager &texture_manager = TextureManager::getInstance();
  ShaderManager &shader_manager = ShaderManager::getInstance();
  std::shared_ptr<Shader> shader;
  std::shared_ptr<Texture> texture;

  // Compute Shader - Map Compute Height
  shader_manager.loadShader("mapComputeHeight", "../res/shaders/map.comp");
  shader = shader_manager.getShader("mapComputeHeight");
  shader->use();
  shader->set_uniform("u_seed", generate_seed());
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->set_num_groups(C::CHUNK_WIDTH / 8, C::CHUNK_HEIGHT / 8,
                         C::CHUNK_DEPTH / 8);
  shader->set_uniform("cell_size", C::CELL_SIZE);
  shader->stop();

  // Compute Shader - Map Compute 3D
  shader_manager.loadShader("mapCompute3D", "../res/shaders/map3D.comp");
  shader = shader_manager.getShader("mapCompute3D");
  shader->use();
  shader->set_uniform("u_seed", generate_seed());
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->set_num_groups(C::CHUNK_WIDTH / 8, C::CHUNK_HEIGHT / 8,
                         C::CHUNK_DEPTH / 8);
  shader->stop();
  //
  // Compute Shader - Grass Compute
  shader_manager.loadShader("grassCompute", "../res/shaders/grass.comp");
  shader = shader_manager.getShader("grassCompute");
  shader->use();
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->set_num_groups(C::CHUNK_WIDTH / 8, C::CHUNK_HEIGHT / 8,
                         C::CHUNK_DEPTH / 8);
  shader->stop();

  // Compute Shader - Water Compute
  shader_manager.loadShader("waterCompute", "../res/shaders/water.comp");
  shader = shader_manager.getShader("waterCompute");
  shader->use();
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->set_num_groups(C::CHUNK_WIDTH / 8, C::CHUNK_HEIGHT / 8,
                         C::CHUNK_DEPTH / 8);
  shader->stop();

  // Compute Shader - Map Compute Voronoi
  shader_manager.loadShader("mapComputeVoronoi", "../res/shaders/voronoi.comp");
  shader = shader_manager.getShader("mapComputeVoronoi");
  shader->use();
  shader->set_uniform("u_seed", generate_seed());
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->set_num_groups(C::CHUNK_WIDTH / 8, C::CHUNK_HEIGHT / 8,
                         C::CHUNK_DEPTH / 8);
  shader->set_uniform("cell_size", C::CELL_SIZE);
  shader->stop();

  // Compute Shader - Map Compute Biomes
  shader_manager.loadShader("mapComputeBiomes", "../res/shaders/biomes.comp");
  shader = shader_manager.getShader("mapComputeBiomes");
  shader->use();
  shader->set_uniform("u_seed", generate_seed());
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->set_num_groups(C::CHUNK_WIDTH / 8, C::CHUNK_HEIGHT / 8,
                         C::CHUNK_DEPTH / 8);
  shader->set_uniform("cell_size", C::CELL_SIZE);
  shader->stop();

  // Compute Shader - Gen Vertices
  shader_manager.loadShader("genVertices", "../res/shaders/gen_vertices.comp");
  shader = shader_manager.getShader("genVertices");
  shader->use();
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  shader->set_num_groups(C::CHUNK_WIDTH / 8, C::CHUNK_HEIGHT / 8,
                         C::CHUNK_DEPTH / 8);
  shader->stop();

  // Uint Map Draw Shader
  shader_manager.loadShader("mapDrawUint", "../res/shaders/voxels.vert",
                            "../res/shaders/voxels.frag",
                            "../res/shaders/voxels.geom");

  // Faces Map Draw Shader
  shader_manager.loadShader("mapDrawFaces", "../res/shaders/faces.vert",
                            "../res/shaders/faces.frag");
  shader = shader_manager.getShader("mapDrawFaces");
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  texture_manager.loadTexture("grass_tex", "../res/textures/grass.jpg");
  texture_manager.loadTexture("water_tex", "../res/textures/water.jpg");
  texture_manager.loadTexture("cobble_tex", "../res/textures/cobble.jpg");
  texture_manager.loadTexture("bedrock_tex", "../res/textures/bedrock.jpg");
  texture_manager.loadTexture("lava_tex", "../res/textures/lava.jpg");
  texture_manager.loadTexture("sand_tex", "../res/textures/sand.jpg");

  // Cube Repère Shader
  shader_manager.loadShader("cubeRepere", "../res/shaders/cube_repere.vert",
                            "../res/shaders/cube_repere.frag");

  World &world = World::getInstance();
  ChunkBuilder chunkbuilder = ChunkBuilder();
  int radius = P::render_distance;
  for (int dz = -radius; dz <= radius; ++dz) {
    for (int dx = -radius; dx <= radius; ++dx) {
      // distance au chunk central
      float dist = sqrt(dx * dx + dz * dz);
      if (dist > radius) continue;
      world.initChunk(dx, dz);
      chunkbuilder.build(world.tryGetChunk(dx, dz));
    }
  }

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
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glClearColor(0.3f, 0.4f, 0.8f, 1.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // draw_cube_repere(cam);
  draw_chunks_below(cam);
}

static inline void camera_settings(Camera &cam, float current_time) {
  static GLfloat angle = 6.0;
  GLfloat dist = 0.0;
  GLfloat vit = 0.1;
  //current_time = 0.0;

  /*
  cam.update(
      glm::vec3(-10.5 * dist * cos(vit * current_time), 170.0,
                10.2 * dist * sin(vit * current_time)),
      glm::vec3((GLfloat)C::CHUNK_WIDTH * 1, 0.0, (GLfloat)C::CHUNK_DEPTH * 1),
      glm::vec3(0.0, 1.0, 0.0));
      */
  cam.update(
            glm::vec3(10.0 * dist * cos(vit * current_time), 300.0, 10.0 * dist * sin(vit * current_time)),
            glm::vec3(10.0 * dist * cos(vit * current_time), 0.0, 10 * dist * sin(vit * current_time)),
            //glm::vec3(0.0, 0.0, 0.0),
            glm::vec3(0.0, 1.0, 1.0));
}

static inline void tick_chunkbuilder(Camera cam) {
  int i = 0;
  int max_chunks_per_tick = 1;
  ChunkBuilder chunkbuilder = ChunkBuilder();
  World &world = World::getInstance();
  for (auto &[key, chunk] : world.m_chunks) {
    if (chunk && !(chunk->is_being_processed) && !(chunk->renderable)) {
      if ((++i) > max_chunks_per_tick)
        break;
      chunkbuilder.build(chunk.get());
      print_vram();
    }
  }
}

int main() {
  static float deltaTime = 0.0;
  static float lastFrame = 0.0;
  static float currentFrame = 0.0;
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

  Camera cam =
  Camera(glm::vec3(0, 200, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  //camera_settings(cam, 0.0);
  init(cam);
  auto lastTime = std::chrono::high_resolution_clock::now();
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetWindowUserPointer(window, &cam);

  while (glfwGetKey(window, GLFW_KEY_L) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float>(currentTime - lastTime).count();
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    //camera_settings(cam, dt);
    cam.process_keyboard(window, deltaTime);
    tick_chunkbuilder(cam);
    draw(cam);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
