#include "walls.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>

void Walls::initializeGL(GLuint program, std::string path) {
  m_model.loadObj(path + "box.obj");
  m_model.setupVAO(program);
  m_model.m_Ka = {0.0f, 0.0f, 0.1f, 1.0f};
  m_model.m_Kd = {0.0f, 0.0f, 0.7f, 1.0f};
  m_model.m_Ks = {0.2f, 0.2f, 0.2f, 1.0f};

  // Save location of uniform variables
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  normalMatrixLoc = abcg::glGetUniformLocation(program, "normalMatrix");
  KaLoc = abcg::glGetUniformLocation(program, "Ka");
  KdLoc = abcg::glGetUniformLocation(program, "Kd");
  KsLoc = abcg::glGetUniformLocation(program, "Ks");
  shininessLoc = abcg::glGetUniformLocation(program, "shininess");
  // m_colorLoc = abcg::glGetUniformLocation(program, "color");
}

void Walls::paintGL(glm::mat4* viewMatrix) {
  // abcg::glUniform4f(m_colorLoc, 0.0f, 0.0f, 1.0f, 1.0f);

  abcg::glUniform1f(shininessLoc, m_model.m_shininess);
  abcg::glUniform4fv(KaLoc, 1, &m_model.m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_model.m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_model.m_Ks.x);

  for (const auto x : {-width, width}) {
    // Set model matrix
    glm::mat4 model{1.0f};
    model = glm::translate(model, glm::vec3(x, 0.29f, -(len / 2) + 1.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, (len + 1) / 1.1547));
    // model = glm::scale(model, glm::vec3(0.5f, 0.5f, len / 1.18f));

    abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

    const auto modelViewMatrix{glm::mat3((*viewMatrix) * model)};
    glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
    abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

    m_model.render();
  }

  // for (const auto z : iter::range(4))
  //   for (const auto x : {-width, width}) {
  //     // Set model matrix
  //     glm::mat4 model{1.0f};
  //     model = glm::translate(model, glm::vec3(x, 0.29f, -z * len));
  //     model = glm::scale(model, glm::vec3(0.5f, 0.5f, 3.4641f));
  //     // model = glm::scale(model, glm::vec3(0.5f, 0.5f, len / 1.18f));

  //     abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

  //     const auto modelViewMatrix{glm::mat3((*viewMatrix) * model)};
  //     glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  //     abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE,
  //                              &normalMatrix[0][0]);

  //     m_model.render();
  //   }
}

void Walls::terminateGL() { m_model.terminateGL(); }
