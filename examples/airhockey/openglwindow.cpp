#include "openglwindow.hpp"

#include <imgui.h>

#include <string>

#include "abcg.hpp"

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN)
      m_gameData.m_input.set(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
    if (event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(static_cast<size_t>(Input::W));
    if (event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.set(static_cast<size_t>(Input::S));
    if (event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::A));
    if (event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::D));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_UP)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
    if (event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::W));
    if (event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(static_cast<size_t>(Input::S));
    if (event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::A));
    if (event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::D));
  }
}

void OpenGLWindow::initializeGL() {
  // Load a new font
  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  // Create program to render the other objects
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");

  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  restart();
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;

  m_player.initializeGL(m_objectsProgram);
  m_puck.initializeGL(m_objectsProgram);
  m_board.initializeGL(m_objectsProgram);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
    m_gameData.m_score.fill(0);
    restart();
    return;
  }

  m_player.update(m_gameData, deltaTime);
  m_puck.update(m_player, m_gameData, deltaTime);

  if (m_gameData.m_state == State::Playing) {
    checkGoals();
    checkWinCondition();
  }
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_player.paintGL(m_gameData);
  m_puck.paintGL(m_gameData);
  m_board.paintGL(m_gameData);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    ImVec2 size;
    ImVec2 position;
    if (m_gameData.m_state == State::Playing) {
      size = ImVec2(180, 80);
      position = ImVec2((m_viewportWidth - size.x) / 2.0f,
                        (m_viewportHeight - size.y) / 4.0f);
    } else {
      size = ImVec2(400, 85);
      position = ImVec2((m_viewportWidth - size.x) / 2.0f,
                        (m_viewportHeight - size.y) / 2.0f);
    }

    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::WinP1) {
      ImGui::Text("Player 1 Wins!");
    } else if (m_gameData.m_state == State::WinP2) {
      ImGui::Text("Player 2 Wins!");
    } else {
      std::string text = std::to_string(m_gameData.m_score.at(0)) +
                         std::string("    ") +
                         std::to_string(m_gameData.m_score.at(1));
      ImGui::Text(text.c_str());
    }

    ImGui::PopFont();
    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  abcg::glDeleteProgram(m_objectsProgram);

  m_player.terminateGL();
  m_puck.terminateGL();
  m_board.terminateGL();
}

void OpenGLWindow::checkGoals() {
  auto pos_x = abs(m_puck.m_translation.x) + m_puck.m_scale;
  auto pos_y = abs(m_puck.m_translation.y) + m_puck.m_scale;
  if (pos_x >= .9f && pos_y < .3f) {
    if (m_puck.m_translation.x > 0)
      m_gameData.m_score.at(0)++;
    else
      m_gameData.m_score.at(1)++;
    restart();
  }
}

void OpenGLWindow::checkWinCondition() {
  if (std::any_of(std::begin(m_gameData.m_score), std::end(m_gameData.m_score),
                  [=](int n) { return n >= 5; })) {
    if (m_gameData.m_score.at(0) >= 5)
      m_gameData.m_state = State::WinP1;
    else
      m_gameData.m_state = State::WinP2;
    m_restartWaitTimer.restart();
  }
}
