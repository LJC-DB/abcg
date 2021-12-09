#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcg.hpp"

class Ground {
 public:
  void initializeGL(GLuint program);
  void paintGL(glm::mat4* viewMatrix);
  void terminateGL();

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};

  GLint m_modelMatrixLoc{};
  GLint normalMatrixLoc;
  GLint KaLoc;
  GLint KdLoc;
  GLint KsLoc;
  GLint shininessLoc;

  glm::vec4 m_Ka{0.1f, 0.1f, 0.1f, 1.0f};
  glm::vec4 m_Kd{0.7f, 0.7f, 0.7f, 1.0f};
  glm::vec4 m_Ks{0.5f, 0.5f, 0.5f, 1.0f};
  float m_shininess{25.0f};
  // GLint m_colorLoc{};
};

#endif
