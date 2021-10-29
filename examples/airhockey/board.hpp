#ifndef BOARD_HPP_
#define BOARD_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

// class Asteroids;
// class Bullets;
class OpenGLWindow;
// class StarLayers;

class Board {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  // void update(const GameData &gameData, float deltaTime);

 private:
  // friend Asteroids;
  // friend Bullets;
  friend OpenGLWindow;
  // friend StarLayers;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  // GLint m_rotationLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  glm::vec3 m_color{.7f};
  // float m_rotation{};
  float m_scale{1};
  glm::vec2 m_translation{};

  // abcg::ElapsedTimer m_trailBlinkTimer;
  // abcg::ElapsedTimer m_bulletCoolDownTimer;
};

#endif
