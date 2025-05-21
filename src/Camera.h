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
#include <GLFW/glfw3.h>


class Camera
{
public:
  Camera(glm::vec3 pos, glm::vec3 look_at, glm::vec3 head);

  void process_keyboard(GLFWwindow* window, float deltaTime);
  void process_mouse_movement(float xoffset, float yoffset, bool constrainPitch = true);
  void update_vectors();
  void update(glm::vec3 pos, glm::vec3 look_at, glm::vec3 head);
  void pos_update(glm::vec3 pos);
  void look_at_update(glm::vec3 look_at);
  void head_update(glm::vec3 head);
  glm::vec3 get_pos();
  glm::mat4 get_view();
  glm::mat4 get_proj();
  float yaw = -90.0f;
  float pitch = -25.0f;
  float lastX = 960.0f, lastY = 540.0f;
  bool firstMouse = true;
  float speed = 50.0f;
  glm::vec3 front, right, up;
  ~Camera();

private:
  GLuint m_ID;
  glm::vec3 m_pos;
  glm::vec3 m_look_at;
  glm::vec3 m_head;
  glm::mat4 m_view;
  glm::mat4 m_proj;
};

#endif // CAMERA_H_
