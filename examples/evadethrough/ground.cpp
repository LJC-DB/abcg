#include "ground.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>

void Ground::initializeGL(GLuint program) {
  // Unit quad on the xz plane
  // clang-format off
  std::array vertices{glm::vec3(-0.5f, 0.0f,  0.5f),
                      glm::vec3(-0.5f, 0.0f, -0.5f),
                      glm::vec3( 0.5f, 0.0f,  0.5f),
                      glm::vec3( 0.5f, 0.0f, -0.5f)};
  // clang-format on

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create VAO and bind vertex attributes
  abcg::glGenVertexArrays(1, &m_VAO);
  abcg::glBindVertexArray(m_VAO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

  const GLint posAttrib{abcg::glGetAttribLocation(program, "inPosition")};
  abcg::glEnableVertexAttribArray(posAttrib);
  abcg::glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  // const GLint normalAttribute{abcg::glGetAttribLocation(program,
  // "inNormal")}; abcg::glEnableVertexAttribArray(normalAttribute);
  // abcg::glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0,
  // nullptr);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);

  // Save location of uniform variables
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  normalMatrixLoc = abcg::glGetUniformLocation(program, "normalMatrix");
  KaLoc = abcg::glGetUniformLocation(program, "Ka");
  KdLoc = abcg::glGetUniformLocation(program, "Kd");
  KsLoc = abcg::glGetUniformLocation(program, "Ks");
  shininessLoc = abcg::glGetUniformLocation(program, "shininess");
  // m_colorLoc = abcg::glGetUniformLocation(program, "color");
}

void Ground::paintGL(glm::mat4* viewMatrix) {
  abcg::glUniform1f(shininessLoc, m_shininess);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

  // Draw a grid of tiles centered on the xz plane
  const int N{30};

  abcg::glBindVertexArray(m_VAO);
  for (const auto z : iter::range(-N, 2)) {
    for (const auto x : iter::range(-10, 10 + 1)) {
      // Set model matrix
      glm::mat4 model{1.0f};
      model = glm::translate(model, glm::vec3(x, 0.0f, z));
      abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

      const auto modelViewMatrix{glm::mat3((*viewMatrix) * model)};
      glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
      abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE,
                               &normalMatrix[0][0]);

      // Set color (checkerboard pattern)
      const float gray{(z + x) % 2 == 0 ? 1.0f : 0.5f};

      const glm::vec4 Ka{glm::vec3(m_Ka) * gray, 1.0f};
      const glm::vec4 Kd{glm::vec3(m_Kd) * gray, 1.0f};

      abcg::glUniform4fv(KaLoc, 1, &Ka.x);
      abcg::glUniform4fv(KdLoc, 1, &Kd.x);
      // abcg::glUniform4f(m_colorLoc, gray, gray, gray, 1.0f);

      abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
  }
  abcg::glBindVertexArray(0);
}

void Ground::terminateGL() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}
