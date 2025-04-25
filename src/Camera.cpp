#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 look_at, glm::vec3 head){
   m_pos = pos;
   m_look_at = look_at;
   m_head = head;
   m_view = glm::lookAt(m_pos, m_look_at, m_head);
   //TODO ne plus avoir de valeurs en dur 1920-1080, mais d'où on prend ces valeurs ?
   m_proj = glm::perspective(glm::radians(45.0f),
                             (float)1920 / (float)1080, 0.1f,
                             9000.0f);
}

void Camera::update(glm::vec3 pos, glm::vec3 look_at, glm::vec3 head){
     m_pos = pos;
     m_look_at = look_at;
     m_head = head;
     m_view = glm::lookAt(pos, look_at, head);
}

void Camera::pos_update(glm::vec3 pos){
     m_pos = pos;
  }

void Camera::look_at_update(glm::vec3 look_at){
     m_look_at = look_at;
  }

void Camera::head_update(glm::vec3 head){
     m_head = head;
  }

glm::vec3 Camera::get_pos(){
  return m_pos;
}

glm::mat4 Camera::get_view(){
  return m_view;
}

glm::mat4 Camera::get_proj(){
  return m_proj;
}

Camera::~Camera()
{
}
