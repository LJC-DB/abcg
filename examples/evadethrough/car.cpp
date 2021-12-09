#include "car.hpp"

#include <algorithm>
#include <glm/gtc/matrix_inverse.hpp>

void Car::loadObj(std::string path, bool standardize) {
  Model::loadObj(path + "Futuristic_Car.obj", standardize);

  m_Ka = {0.5f, 0.5f, 0.5f, 1.0f};
  m_Kd = {0.3f, 0.3f, 0.3f, 1.0f};
}

void Car::setupVAO(GLuint m_program) {
  Model::setupVAO(m_program);
  modelMatrixLoc = abcg::glGetUniformLocation(m_program, "modelMatrix");
  normalMatrixLoc = abcg::glGetUniformLocation(m_program, "normalMatrix");
  KaLoc = abcg::glGetUniformLocation(m_program, "Ka");
  KdLoc = abcg::glGetUniformLocation(m_program, "Kd");
  KsLoc = abcg::glGetUniformLocation(m_program, "Ks");
  shininessLoc = abcg::glGetUniformLocation(m_program, "shininess");
  // colorLoc = abcg::glGetUniformLocation(m_program, "color");
}

void Car::render(glm::mat4* viewMatrix, int numTriangles) {
  glm::mat4 m_modelMatrix{1.0f};
  m_modelMatrix = glm::translate(m_modelMatrix, position);
  m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(0.5f));
  // m_modelMatrix =
  //     glm::rotate(m_modelMatrix, glm::radians(45.0f), glm::vec3(0, 1, 0));
  // m_modelMatrix =
  //     glm::rotate(m_modelMatrix, glm::radians(-90.0f), glm::vec3(1, 0, 0));
  // m_modelMatrix =
  //     glm::rotate(m_modelMatrix, glm::radians(180.0f), glm::vec3(0, 0, 1));

  // abcg::glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);
  const auto modelViewMatrix{glm::mat3((*viewMatrix) * m_modelMatrix)};
  glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  abcg::glUniform1f(shininessLoc, m_shininess);
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

  Model::render(numTriangles);
}

void Car::update(float forwardspeed, float lateralspeed) {
  const glm::vec3 change{glm::vec3(lateralspeed, 0.0f, -forwardspeed)};

  position += change;

  if (position.z <= -4) position.z += 4;
  position.x = std::clamp(position.x, -2 + 0.52f, 2 - 0.52f);
}
