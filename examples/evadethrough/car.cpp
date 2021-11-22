#include "car.hpp"

#include <algorithm>

void Car::loadObj(std::string path, bool standardize) {
  Model::loadObj(path + "chamferbox.obj", standardize);
}

void Car::setupVAO(GLuint m_program) {
  Model::setupVAO(m_program);
  modelMatrixLoc = abcg::glGetUniformLocation(m_program, "modelMatrix");
  colorLoc = abcg::glGetUniformLocation(m_program, "color");
}

void Car::render(int numTriangles) {
  glm::mat4 m_modelMatrix{1.0f};
  m_modelMatrix = glm::translate(m_modelMatrix, position);
  m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(0.5f, 0.3f, 0.5f));
  // m_modelMatrix =
  //     glm::rotate(m_modelMatrix, glm::radians(-90.0f), glm::vec3(0, 1, 0));

  abcg::glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);

  Model::render(numTriangles);
}

void Car::update(float forwardspeed, float lateralspeed) {
  const glm::vec3 change{glm::vec3(lateralspeed, 0.0f, -forwardspeed)};

  position += change;

  if (position.z <= -4) position.z += 4;
  position.x = std::clamp(position.x, -2 + 0.58f, 2 - 0.58f);
}
