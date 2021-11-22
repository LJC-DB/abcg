#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include "abcg.hpp"
#include "camera.hpp"
#include "car.hpp"
#include "ground.hpp"
#include "walls.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_program{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  Camera m_camera;
  float m_latSpeed{};
  float m_forwardSpeed{1.0f};

  Ground m_ground;
  Walls m_walls;
  Car m_car;

  GLint viewMatrixLoc{};
  GLint projMatrixLoc{};

  void update();
};

#endif
