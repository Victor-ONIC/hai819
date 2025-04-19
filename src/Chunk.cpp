#include "Chunk.h"
#include "Constants.h"
#include "Noise.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Texture.h"

Chunk::Chunk(int x, int z) {
  // std::fill(m_data.begin(), m_data.end(), 0);
  //   Indices du chunk
  m_x = x;
  m_z = z;
  //  Creation du buffer SSBO blocktype
  glGenBuffers(1, &m_buffer_blocktype);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_buffer_blocktype);
  glBufferData(GL_SHADER_STORAGE_BUFFER, C::BLOCKS_PER_CHUNK * sizeof(GLuint),
               nullptr, GL_DYNAMIC_DRAW);

  //  Creation du VAO correspondant au buffer SSBO blocktype
  glGenVertexArrays(1, &m_vao_blocktype);
  glBindVertexArray(m_vao_blocktype);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer_blocktype);
  glBufferData(GL_ARRAY_BUFFER, C::BLOCKS_PER_CHUNK * sizeof(GLuint),
               // m_data.data(), GL_DYNAMIC_DRAW);
               nullptr, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(GLuint),
                        (const void *)0);

  //  Creation du buffer SSBO faces
  glGenBuffers(1, &m_buffer_faces);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_buffer_faces);
  glBufferData(GL_SHADER_STORAGE_BUFFER, C::BLOCKS_PER_CHUNK * 6 * sizeof(face),
               nullptr, GL_DYNAMIC_DRAW);

  //  Creation du VAO correspondant au buffer SSBO faces
  glGenVertexArrays(1, &m_vao_faces);
  glBindVertexArray(m_vao_faces);
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer_faces);
  glBufferData(GL_ARRAY_BUFFER, C::BLOCKS_PER_CHUNK * 6 * sizeof(face), nullptr,
               GL_DYNAMIC_DRAW);

  // blockype (as uint)
  glEnableVertexAttribArray(0);
  glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(face), (void *)0);
  glVertexAttribDivisor(0, 1);

  // vert[0]
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(face), (void *)(16));
  glVertexAttribDivisor(1, 1);

  // vert[1]
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(face), (void *)(32));
  glVertexAttribDivisor(2, 1);

  // vert[2]
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(face), (void *)(48));
  glVertexAttribDivisor(3, 1);

  // vert[3]
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(face), (void *)(64));
  glVertexAttribDivisor(4, 1);

  // normal
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(face), (void *)(80));
  glVertexAttribDivisor(5, 1);

  // coord_tex
  glEnableVertexAttribArray(6);
  glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(face), (void *)(96));
  glVertexAttribDivisor(6, 1);
}

GLuint Chunk::get_buffer() { return m_buffer_blocktype; }
GLuint Chunk::get_vao() { return m_vao_blocktype; }

Chunk::~Chunk() {
  if (m_buffer_blocktype != 0) {
    glDeleteBuffers(1, &m_buffer_blocktype);
  }
  if (m_vao_blocktype != 0) {
    glDeleteVertexArrays(1, &m_vao_blocktype);
  }
}
