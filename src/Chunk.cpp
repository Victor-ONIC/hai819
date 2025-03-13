#include "Chunk.h"

Chunk::Chunk(int x, int z){
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
  glBufferData(GL_ARRAY_BUFFER, C::BLOCKS_PER_CHUNK * sizeof(GLuint), m_data.data(),
               GL_DYNAMIC_DRAW);
  //glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(GLuint),
                        //(const void *)0);

}

Chunk::~Chunk() {
  if (m_buffer != 0) {
    glDeleteBuffers(1, &m_buffer);
  }
  if (m_vao != 0) {
    glDeleteVertexArrays(1, &m_vao);
  }
}
