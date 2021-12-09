#include "camera.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

void Camera::computeProjectionMatrix(int width, int height) {
  const auto aspect{static_cast<float>(width) / static_cast<float>(height)};
  // m_projMatrix = glm::ortho(-3.0f, 3.0f, -2f, 2f, 0.1f, 20.0f);
  m_projMatrix = glm::perspective(glm::radians(100.0f), aspect, 0.1f, 40.0f);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
  // m_viewMatrix = glm::mat4(1.0f);
}

void Camera::update(float forwardspeed, float lateralspeed) {
  const glm::vec3 change{glm::vec3(lateralspeed, 0.0f, -forwardspeed)};

  m_at += change;

  if (m_at.z <= -4) m_at.z += 4;
  m_at.x = std::clamp(m_at.x, -2 + 0.52f, 2 - 0.52f);

  // m_eye = m_at + glm::vec3(0.0f, 1.0f, 1.0f);
  m_eye = m_at + glm::vec3(0.0f, 0.6f, 1.0f);

  computeViewMatrix();
}
