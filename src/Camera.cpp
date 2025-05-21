#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 look_at, glm::vec3 head){
   m_pos = pos;
   m_speed = walking_speed;
   m_look_at = look_at;
   m_head = head;
   m_view = glm::lookAt(m_pos, m_look_at, m_head);
   //TODO ne plus avoir de valeurs en dur 1920-1080, mais d'où on prend ces valeurs ?
   m_proj = glm::perspective(glm::radians(50.0f),
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

// void Camera::process_keyboard(GLFWwindow* window, float deltaTime) {
//     float velocity = speed * deltaTime * 2.0;

//     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//         m_pos += glm::vec3(front[0], 0.0, front[2]) * velocity;

//     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//         m_pos -= glm::vec3(front[0], 0.0, front[2]) * velocity;

//     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//         m_pos -= glm::vec3(right[0], 0.0, right[2]) * velocity;

//     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//         m_pos += glm::vec3(right[0], 0.0, right[2]) * velocity;

//     if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
//         m_pos += glm::vec3(0.0, 1.0, 0.0) * (velocity * 2.0f);

//     if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
//         m_pos -= glm::vec3(0.0, 1.0, 0.0) * (velocity * 2.0f);

//     if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
//         speed = 12.0f;

//     if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
//         speed = 5.0f;

//     update_vectors();
// }
void Camera::process_keyboard(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_velocity += glm::vec3(front[0], 0.0, front[2]) * m_speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_velocity -= glm::vec3(front[0], 0.0, front[2]) * m_speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_velocity -= glm::vec3(right[0], 0.0, right[2]) * m_speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_velocity += glm::vec3(right[0], 0.0, right[2]) * m_speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_velocity += glm::vec3(0.0, 1.0, 0.0) * m_speed * deltaTime * 2.0f;

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_velocity -= glm::vec3(0.0, 1.0, 0.0) * m_speed * deltaTime * 2.0f;

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        m_speed = walking_speed * 2.0f;

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
        m_speed = walking_speed;

    update_vectors();
}

int Camera::block_index(int x, int y, int z)
{
    return x + y * C::CHUNK_WIDTH + z * (C::CHUNK_WIDTH * C::CHUNK_HEIGHT);;
}

Collision Camera::collide() {
    Collision res;

    World& w = World::getInstance();

    int x = floor(m_pos[0] / C::CHUNK_WIDTH);
    int z = floor(m_pos[2] / C::CHUNK_DEPTH);
    Chunk* c = w.tryGetChunk(x, z);
    if (!c) return res;

    GLuint ssbo = c->get_blocktype_buffer();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    void* ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    if (ptr) {
        GLuint* blocks = static_cast<GLuint*>(ptr);

        // coords caméra dans le chunk
        int chunk_x = static_cast<int>(m_pos[0]) % C::CHUNK_DEPTH;
        int chunk_y = static_cast<int>(m_pos[1]);
        int chunk_z = static_cast<int>(m_pos[2]) % C::CHUNK_WIDTH;

        // y_pos
        GLuint u = blocks[block_index(chunk_x, chunk_y + 1, chunk_z)];
        if (u != 0) {
            res.y_pos = true;
            std::cout << "Collision up" << std::endl;
        }

        // y_neg
        GLuint d = blocks[block_index(chunk_x, chunk_y - 2, chunk_z)];
        if (d != 0) {
            res.y_neg = true;
            std::cout << "Collision down" << std::endl;
        }

        // x_pos
        GLuint xp1 = blocks[block_index(chunk_x + 1, chunk_y - 1, chunk_z)];
        GLuint xp2 = blocks[block_index(chunk_x + 1, chunk_y, chunk_z)];
        if (xp1 != 0 || xp2 != 0) {
            res.x_pos = true;
            std::cout << "Collision x+" << std::endl;
        }

        // x_neg
        GLuint xn1 = blocks[block_index(chunk_x - 1, chunk_y - 1, chunk_z)];
        GLuint xn2 = blocks[block_index(chunk_x - 1, chunk_y, chunk_z)];
        if (xp1 != 0 || xp2 != 0) {
            res.x_neg = true;
            std::cout << "Collision x-" << std::endl;
        }

        // z_pos
        GLuint zp1 = blocks[block_index(chunk_x, chunk_y - 1, chunk_z + 1)];
        GLuint zp2 = blocks[block_index(chunk_x, chunk_y, chunk_z + 1)];
        if (zp1 != 0 || zp2 != 0) {
            res.z_pos = true;
            std::cout << "Collision z+" << std::endl;
        }

        // z_neg
        GLuint zn1 = blocks[block_index(chunk_x, chunk_y - 1, chunk_z - 1)];
        GLuint zn2 = blocks[block_index(chunk_x, chunk_y, chunk_z - 1)];
        if (zn1 != 0 || zn2 != 0) {
            res.z_neg = true;
            std::cout << "Collision z-" << std::endl;
        }
    }

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    return res;
}

void Camera::update(float dt) {
    this->c = this->collide();

    // Gravité
    if (!this->c.y_neg) {
        m_velocity -= glm::vec3(0.0, 1.0, 0.0) * m_speed * dt;
    }

    if (this->c.x_pos) {
        m_velocity[0] = m_velocity[0] > 0 ? 0 : m_velocity[0];
    }
    if (this->c.x_neg) {
        m_velocity[0] = m_velocity[0] <= 0 ? 0 : m_velocity[0];
    }

    if (this->c.z_pos) {
        m_velocity[2] = m_velocity[2] > 0 ? 0 : m_velocity[2];
    }
    if (this->c.z_neg) {
        m_velocity[2] = m_velocity[2] <= 0 ? 0 : m_velocity[2];
    }

    m_pos += m_velocity * dt;
    m_velocity *= pow(0.6f, dt * 60.0f);

    update_vectors();
}

void Camera::process_mouse_movement(float xoffset, float yoffset, bool constrainPitch) {
    float sensitivity = 0.03f;
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

glm::vec3& Camera::get_pos_modifiable()
{
    return this->m_pos;
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
