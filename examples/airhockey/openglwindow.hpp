#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include "abcg.hpp"
#include "board.hpp"
#include "player.hpp"
#include "puck.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_objectsProgram{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  GameData m_gameData;

  Player m_player;
  Board m_board;
  Puck m_puck;

  abcg::ElapsedTimer m_restartWaitTimer;

  ImFont* m_font{};

  void checkGoals();
  void checkWinCondition();

  void restart();
  void update();
};

#endif
