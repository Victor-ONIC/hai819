#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 look_at, glm::vec3 head){
   m_pos = pos;
   m_look_at = look_at;
   m_head = head;
   m_view = glm::lookAt(m_pos, m_look_at, m_head);
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

glm::mat4 Camera::get_view(){
  return m_view;
}

Camera::~Camera()
{
}
