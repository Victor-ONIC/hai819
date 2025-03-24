#include "ChunkBuilder.h"
#include "Chunk.h"
#include "Constants.h"
#include "ShaderManager.h"
#include "Noise.h"
#include "Texture.h"

//Constructeur temporaire où l'on ajoute forcément le programme "computeMap" à la pipeline
ChunkBuilder::ChunkBuilder() { addShader("mapCompute"); }

//Permet d'ajouter un programme GPU à la pipeline de build
void ChunkBuilder::addShader(const std::string &name) {
  ShaderManager &shader_manager = ShaderManager::getInstance();
  m_shaders_pipeline.push_back(shader_manager.getShader(name));
}


ChunkBuilder::~ChunkBuilder() {
}

// Prend un Chunk et exécute la pipeline pour le build
void ChunkBuilder::build(Chunk *chunk) {
  Noise noise = Noise(256);
  Texture tex = Texture(noise.m_buffer, 256, 256);
  tex.bind(0);
  for (const auto &shader : m_shaders_pipeline) { // Référence constante, pas de copie
    shader->set_uniform("permTexture", 0);
    glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(GLuint),
                          (const void *)0);
    shader->use();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, chunk->get_buffer());
    shader->set_uniform("map_width", C::CHUNK_WIDTH);
    shader->set_uniform("map_height", C::CHUNK_HEIGHT);
    shader->set_uniform("map_depth", C::CHUNK_DEPTH);
    glDispatchCompute(C::BLOCKS_PER_CHUNK / 1024, 1, 1);
    shader->stop();
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  }
}
/*
  ShaderManager &shader_manager = ShaderManager::getInstance();
  std::shared_ptr<Shader> shader = shader_manager.getShader("mapCompute");
  Noise noise = Noise(256);
  Texture tex = Texture(noise.m_buffer, 256, 256);
  tex.bind(0);
  shader->set_uniform("permTexture", 0);

  std::vector<GLuint> data(C::BLOCKS_PER_CHUNK);
  // Génération des buffers
  glGenBuffers(1, _buffer);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, _buffer[0]);
  glBufferData(GL_SHADER_STORAGE_BUFFER, C::BLOCKS_PER_CHUNK * sizeof(GLuint),
  data.data(), GL_DYNAMIC_DRAW);
  //
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _buffer[0]);
  glBufferData(GL_ARRAY_BUFFER, C::BLOCKS_PER_CHUNK * sizeof(GLuint),
  data.data(), GL_DYNAMIC_DRAW); glVertexAttribPointer(0, 1, GL_UNSIGNED_INT,
  GL_FALSE, sizeof(GLuint), (const void *)0);
  // Compute Shader
  shader->use();
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _buffer[0]);
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  glDispatchCompute(_nb_threads_x, 1, 1);
  shader->stop();
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
*/
