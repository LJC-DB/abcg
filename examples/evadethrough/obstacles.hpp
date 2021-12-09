#ifndef OBSTACLES_HPP_
#define OBSTACLES_HPP_

#include <deque>
#include <random>

#include "abcg.hpp"
#include "model.hpp"

class Obstacles {
 public:
  void initializeGL(GLuint program, std::string path);
  void paintGL(glm::mat4* viewMatrix);
  void update(float forwardspeed);
  void terminateGL();
  void reset();

  [[nodiscard]] std::deque<glm::vec3> getPos() const { return positions; }

 private:
  Model m_model;

  GLint m_modelMatrixLoc{};
  GLint normalMatrixLoc;
  GLint KaLoc;
  GLint KdLoc;
  GLint KsLoc;
  GLint shininessLoc;

  std::deque<glm::vec3> positions{};

  std::default_random_engine m_randomEngine;

  float posZ{};

  int n{8};
};

#endif
