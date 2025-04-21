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

  // Counter face atomic set upGLuint atomicCounterBuffer;
    GLuint zero = 0;
  m_visibleFaceCounter = 0;
  glGenBuffers(1, &m_bufferVisibleFaceCounter);
  glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_bufferVisibleFaceCounter);
  glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &zero);
  // Lie le buffer à l’unité 2 (comme dans ton shader: `binding = 2`)
  glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, m_bufferVisibleFaceCounter);


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

  // coord_tex[0]
  glEnableVertexAttribArray(6);
  glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(face), (void *)(96));
  glVertexAttribDivisor(6, 1);

  // coord_tex[1]
  glEnableVertexAttribArray(7);
  glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(face), (void *)(104));
  glVertexAttribDivisor(7, 1);

  // coord_tex[2]
  glEnableVertexAttribArray(8);
  glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(face), (void *)(112));
  glVertexAttribDivisor(8, 1);

  // coord_tex[3]
  glEnableVertexAttribArray(9);
  glVertexAttribPointer(9, 2, GL_FLOAT, GL_FALSE, sizeof(face), (void *)(120));
  glVertexAttribDivisor(9, 1);
}

GLuint Chunk::get_blocktype_buffer() { return m_buffer_blocktype; }
GLuint Chunk::get_faces_buffer() { return m_buffer_faces; }
GLuint Chunk::get_buffer_counter_faces() { return m_bufferVisibleFaceCounter; }
GLuint Chunk::get_vao_blocktype() { return m_vao_blocktype; }
GLuint Chunk::get_vao_faces() { return m_vao_faces; }

void Chunk::reset_counter_faces(){
    m_visibleFaceCounter = 0;
}

GLuint Chunk::get_counter_faces() {
  glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);
  glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_bufferVisibleFaceCounter);
  glGetBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &m_visibleFaceCounter);
  return m_visibleFaceCounter;
}

void Chunk::print_counter_faces(){
  std::cout << "Nombre de faces visibles : " << m_visibleFaceCounter << std::endl;
}

void Chunk::print_faces(){
  GLuint ssbo = get_faces_buffer();
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

  // Mapper les données en lecture seule
  face * data = (face *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0,
                                        6 * sizeof(face), GL_MAP_READ_BIT);
  if (data) {
    for (int i = 0; i < 6; ++i) {
      std::cout << "Face #" << i << "\n";
      std::cout << "Blocktype: " << data[i].blocktype << "\n";
      std::cout << "Normal: " << data[i].normal[0] << ",  " << data[i].normal[1] << ",  " << data[i].normal[2] << "\n";
      for (int v = 0; v < 4; ++v) {
        std::cout << "  Vertex " << v << "--->      " << data[i].vert[v][0] << ",  " << data[i].vert[v][1] << ",  " << data[i].vert[v][2]
                  << "\n";
        std::cout << "  UV     " << v << ": "
                  << data[i].coord_tex[v][0] << ",  " << data[i].coord_tex[v][1] << "\n";
      }
      std::cout << "----------------------\n";
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  } else {
    std::cerr << "Erreur : impossible de mapper le SSBO.\n";
  }

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

Chunk::~Chunk() {
  if (m_buffer_blocktype != 0) {
    glDeleteBuffers(1, &m_buffer_blocktype);
  }
  if (m_vao_blocktype != 0) {
    glDeleteVertexArrays(1, &m_vao_blocktype);
  }
}
