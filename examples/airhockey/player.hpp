#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

// class Asteroids;
// class Bullets;
class OpenGLWindow;
// class StarLayers;

enum class Type { P1, P2 };

class Player {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(const GameData &gameData, float deltaTime);

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
  // GLuint m_ebo{};

  Type m_type{Type::P1};

  glm::vec3 m_color{};
  // float m_rotation{};
  float m_scale{0.075f};
  glm::vec2 m_translation{};
  glm::vec2 m_velocity{};

  int sides{20};

  // abcg::ElapsedTimer m_trailBlinkTimer;
  // abcg::ElapsedTimer m_bulletCoolDownTimer;
};

#endif
