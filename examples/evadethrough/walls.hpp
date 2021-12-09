#ifndef WALLS_HPP_
#define WALLS_HPP_

#include "abcg.hpp"
#include "model.hpp"

class Walls {
 public:
  void initializeGL(GLuint program, std::string path);
  void paintGL(glm::mat4* viewMatrix);
  void terminateGL();

 private:
  Model m_model;

  GLint m_modelMatrixLoc{};
  GLint normalMatrixLoc;
  GLint KaLoc;
  GLint KdLoc;
  GLint KsLoc;
  GLint shininessLoc;
  // GLint m_colorLoc{};

  int len{35};
  int width{2};
};

#endif
