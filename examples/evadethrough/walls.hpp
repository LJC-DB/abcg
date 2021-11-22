#ifndef WALLS_HPP_
#define WALLS_HPP_

#include "abcg.hpp"
#include "model.hpp"

class Walls {
 public:
  void initializeGL(GLuint program, std::string path);
  void paintGL();
  void terminateGL();

 private:
  Model m_model;

  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};

  int len{4};
  int width{2};
};

#endif
