#ifndef NOISE_H_
#define NOISE_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Noise {
public:
  Noise(size_t height);
  std::vector<uint8_t> m_buffer;

private:
};

#endif
