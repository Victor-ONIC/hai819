#ifndef CAMERA_H_
#define CAMERA_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <GLFW/glfw3.h>

#include "World.h"

struct Collision {
  Collision()
    : y_pos(false), y_neg(false), x_pos(false), x_neg(false), z_pos(false), z_neg(false) 
  {}

  bool y_pos, y_neg, x_pos, x_neg, z_pos, z_neg;
};

class Camera
{
private:

public:
  Camera(glm::vec3 pos, glm::vec3 look_at, glm::vec3 head);

  void process_keyboard(GLFWwindow* window, float deltaTime);
  int block_index(int x, int y, int z);
  Collision collide();
  void update(float dt);
  void process_mouse_movement(float xoffset, float yoffset, bool constrainPitch = true);
  void update_vectors();
  void update(glm::vec3 pos, glm::vec3 look_at, glm::vec3 head);
  void pos_update(glm::vec3 pos);
  void look_at_update(glm::vec3 look_at);
  void head_update(glm::vec3 head);
  glm::vec3 get_pos();
  glm::vec3& get_pos_modifiable();
  glm::mat4 get_view();
  glm::mat4 get_proj();
  float yaw = -90.0f;
  float pitch = 0.0f;
  float lastX = 960.0f, lastY = 540.0f;
  bool firstMouse = true;
  glm::vec3 front, right, up;
  ~Camera();

private:
  static const int walking_speed = 234;

  GLuint m_ID;
  glm::vec3 m_pos;
  glm::vec3 m_velocity;
  float m_speed;
  glm::vec3 m_look_at;
  glm::vec3 m_head;
  glm::mat4 m_view;
  glm::mat4 m_proj;
  Collision c;
};

#endif // CAMERA_H_
