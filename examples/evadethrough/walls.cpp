#include "walls.hpp"

#include <cppitertools/itertools.hpp>

void Walls::initializeGL(GLuint program, std::string path) {
  m_model.loadObj(path + "box.obj");
  m_model.setupVAO(program);

  // Save location of uniform variables
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(program, "color");
}

void Walls::paintGL() {
  abcg::glUniform4f(m_colorLoc, 0.0f, 0.0f, 1.0f, 1.0f);

  for (const auto z : iter::range(4))
    for (const auto x : {-width, width}) {
      // Set model matrix
      glm::mat4 model{1.0f};
      model = glm::translate(model, glm::vec3(x, 0.29f, -z * len));
      model = glm::scale(model, glm::vec3(0.5f, 0.5f, len));

      abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

      m_model.render();
    }
}

void Walls::terminateGL() { m_model.terminateGL(); }
