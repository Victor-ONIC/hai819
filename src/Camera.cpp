#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 look_at, glm::vec3 head){
   m_pos = pos;
   m_look_at = look_at;
   m_head = head;
   m_view = glm::lookAt(m_pos, m_look_at, m_head);
   //TODO ne plus avoir de valeurs en dur 1920-1080, mais d'où on prend ces valeurs ?
   m_proj = glm::perspective(glm::radians(50.0f),
                             //(float)1920 / (float)1080, 0.1f,
                             (float)1920 / (float)1080, 0.1f,
                             2000.0f);
}

void Camera::update_vectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up    = glm::normalize(glm::cross(right, front));
    m_look_at = m_pos + front;
    m_view = glm::lookAt(m_pos, m_look_at, up);
}

void Camera::process_keyboard(GLFWwindow* window, float deltaTime) {
    float velocity = speed * deltaTime * 2.0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_pos += glm::vec3(front[0], 0.0, front[2]) * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_pos -= glm::vec3(front[0], 0.0, front[2]) * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_pos -= glm::vec3(right[0], 0.0, right[2]) * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_pos += glm::vec3(right[0], 0.0, right[2]) * velocity;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_pos += glm::vec3(0.0, 1.0, 0.0) * (velocity * 1.5f);
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        m_pos -= glm::vec3(0.0, 1.0, 0.0) * (velocity * 1.5f);
//FAST LIFE
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        m_pos += glm::vec3(9.0) * glm::vec3(front[0], 0.0, front[2]) * velocity;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        m_pos -= glm::vec3(9.0) * glm::vec3(front[0], 0.0, front[2]) * velocity;
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        m_pos -= glm::vec3(9.0) * glm::vec3(right[0], 0.0, right[2]) * velocity;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        m_pos += glm::vec3(9.0) * glm::vec3(right[0], 0.0, right[2]) * velocity;

    update_vectors();
}

void Camera::process_mouse_movement(float xoffset, float yoffset, bool constrainPitch) {
    float sensitivity = 0.01f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if (constrainPitch) {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    update_vectors();
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

glm::vec3 Camera::get_look_at(){
  return m_look_at;
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
