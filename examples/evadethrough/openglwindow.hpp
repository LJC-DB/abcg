#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include "abcg.hpp"
#include "camera.hpp"
#include "car.hpp"
#include "ground.hpp"
#include "obstacles.hpp"
#include "walls.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_program{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  Camera m_camera;
  float m_latSpeed{};
  float m_forwardSpeed{4.0f};

  Ground m_ground;
  Walls m_walls;
  Car m_car;
  Obstacles m_obstacles;

  int m_mappingMode{};

  glm::vec4 m_lightPos{0.0f, 0.4f, 0.0f, 1.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};

  ImFont* m_font{};

  GLint viewMatrixLoc{};
  GLint projMatrixLoc{};
  GLint lightPosLoc{};
  GLint diffuseTexLoc{};
  GLint mappingModeLoc{};

  GLint IsLoc;
  GLint IdLoc;
  GLint IaLoc;

  float m_score{};

  void update();
  void restart();

  void checkCollision();
};

#endif
