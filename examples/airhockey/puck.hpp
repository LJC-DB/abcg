#ifndef PUCK_HPP_
#define PUCK_HPP_

#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "player.hpp"

class OpenGLWindow;

class Puck {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(Player &m_player, const GameData &gameData, float deltaTime);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};

  glm::vec3 m_color{.4f};
  float m_scale{.04f};
  glm::vec2 m_translation{};
  glm::vec2 m_velocity{};

  std::default_random_engine m_randomEngine;

  int sides{20};
};

#endif
