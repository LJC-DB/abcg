#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;
class Puck;

enum class Type { P1, P2 };

class Player {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(const GameData &gameData, float deltaTime);

 private:
  friend OpenGLWindow;
  friend Puck;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};

  struct Plyr {
    Type m_type{Type::P1};
    glm::vec3 m_color{};
    glm::vec2 m_translation{};
    glm::vec2 m_velocity{};
  };

  std::array<Plyr, 2> players{};

  float m_scale{0.075f};

  int sides{20};
};

#endif
