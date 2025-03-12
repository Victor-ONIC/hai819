#ifndef CHUNK_H_
#define CHUNK_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define GLEW_STATIC
#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Chunk {
public:
  Chunk(size_t x, size_t z);
  ~Chunk();

private:
};

#endif // CHUNK_H_
