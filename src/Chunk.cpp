#include "Chunk.h"
#include "Constants.h"
#include "Noise.h"
#include "Texture.h"
#include "Shader.h"
#include "ShaderManager.h"

/*
Chunk::Chunk(int x, int z) {
  std::fill(m_data.begin(), m_data.end(), 2);
  //  Indices du chunk
  m_x = x;
  m_z = z;
  //  Creation du buffer SSBO
  glGenBuffers(1, &m_buffer);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_buffer);
  glBufferData(GL_SHADER_STORAGE_BUFFER, C::BLOCKS_PER_CHUNK * sizeof(GLuint),
               m_data.data(), GL_DYNAMIC_DRAW);

  //  Creation du VAO correspondant au buffer SSBO
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
  glBufferData(GL_ARRAY_BUFFER, C::BLOCKS_PER_CHUNK * sizeof(GLuint),
               m_data.data(), GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(GLuint),
                        (const void *)0);
}
*/

Chunk::Chunk(int x, int z) {
  ShaderManager &shader_manager = ShaderManager::getInstance();
  std::fill(m_data.begin(), m_data.end(), 2);
  //  Indices du chunk
  m_x = x;
  m_z = z;
  //
  std::shared_ptr<Shader> shader = shader_manager.getShader("mapCompute");
  Noise noise = Noise(256);
  Texture tex = Texture(noise.m_buffer, 256, 256);
  tex.bind(0);
  shader->set_uniform("permTexture", 0);
  //  Creation du buffer SSBO
  glGenBuffers(1, &m_buffer);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_buffer);
  glBufferData(GL_SHADER_STORAGE_BUFFER, C::BLOCKS_PER_CHUNK * sizeof(GLuint),
               m_data.data(), GL_DYNAMIC_DRAW);

  //  Creation du VAO correspondant au buffer SSBO
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
  glBufferData(GL_ARRAY_BUFFER, C::BLOCKS_PER_CHUNK * sizeof(GLuint),
               m_data.data(), GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(GLuint),
                        (const void *)0);

  // Compute Shader
  shader->use();
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_buffer);
  shader->set_uniform("map_width", C::CHUNK_WIDTH);
  shader->set_uniform("map_height", C::CHUNK_HEIGHT);
  shader->set_uniform("map_depth", C::CHUNK_DEPTH);
  glDispatchCompute(C::BLOCKS_PER_CHUNK / 1024, 1, 1);
  shader->stop();
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void Chunk::bind() {
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_buffer);
  glBindVertexArray(m_vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
  glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(GLuint),
                        (const void *)0);
}

GLuint Chunk::get_buffer() { return m_buffer; }
GLuint Chunk::get_vao() { return m_vao; }

Chunk::~Chunk() {
  if (m_buffer != 0) {
    glDeleteBuffers(1, &m_buffer);
  }
  if (m_vao != 0) {
    glDeleteVertexArrays(1, &m_vao);
  }
}
