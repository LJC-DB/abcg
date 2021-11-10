#ifndef BOARD_HPP_
#define BOARD_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class Board {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  glm::vec3 m_color{.7f};
  float m_scale{1};
  glm::vec2 m_translation{};
};

#endif
