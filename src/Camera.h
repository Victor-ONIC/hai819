#ifndef CAMERA_H_
#define CAMERA_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


class Camera
{
  unsigned int ID;
 public:
  glm::vec3 m_pos;
  glm::vec3 m_look_at;
  glm::vec3 m_head;
  glm::mat4 m_view;

  Camera(glm::vec3 pos, glm::vec3 look_at, glm::vec3 head);

  void update(glm::vec3 pos, glm::vec3 look_at, glm::vec3 head);
  void pos_update(glm::vec3 pos);
  void look_at_update(glm::vec3 look_at);
  void head_update(glm::vec3 head);
};

#endif // CAMERA_H_
