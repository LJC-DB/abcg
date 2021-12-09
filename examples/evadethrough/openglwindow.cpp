#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>

void OpenGLWindow::handleEvent(SDL_Event& ev) {
  if (ev.type == SDL_KEYDOWN) {
    if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a)
      m_latSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d)
      m_latSpeed = 1.0f;
  }
  if (ev.type == SDL_KEYUP) {
    if ((ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) &&
        m_latSpeed < 0)
      m_latSpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) &&
        m_latSpeed > 0)
      m_latSpeed = 0.0f;
  }
}

void OpenGLWindow::initializeGL() {
  // Load a new font
  ImGuiIO& io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }
  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  const auto path{getAssetsPath() + "shaders/" + "texture"};
  m_program = createProgramFromFile(path + ".vert", path + ".frag");

  viewMatrixLoc = abcg::glGetUniformLocation(m_program, "viewMatrix");
  projMatrixLoc = abcg::glGetUniformLocation(m_program, "projMatrix");
  lightPosLoc = abcg::glGetUniformLocation(m_program, "lightPos");
  IaLoc = abcg::glGetUniformLocation(m_program, "Ia");
  IdLoc = abcg::glGetUniformLocation(m_program, "Id");
  IsLoc = abcg::glGetUniformLocation(m_program, "Is");
  diffuseTexLoc = abcg::glGetUniformLocation(m_program, "diffuseTex");
  mappingModeLoc = abcg::glGetUniformLocation(m_program, "mappingMode");

  m_mappingMode = 0;  // "From mesh" option

  m_ground.initializeGL(m_program);

  m_walls.initializeGL(m_program, getAssetsPath());

  m_obstacles.initializeGL(m_program, getAssetsPath());

  m_car.loadObj(getAssetsPath());
  m_car.setupVAO(m_program);
  // m_car.loadDiffuseTexture(getAssetsPath() + "maps/" + "Futuristic_Car.png");
  m_car.loadDiffuseTexture(getAssetsPath() + "maps/" +
                           "roman_lamp_diffuse.jpg");
}

void OpenGLWindow::paintGL() {
  update();

  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  abcg::glUseProgram(m_program);

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);

  abcg::glUniform4fv(lightPosLoc, 1, &m_lightPos.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);

  m_mappingMode = 2;
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);
  m_ground.paintGL(&m_camera.m_viewMatrix);

  m_mappingMode = 1;
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);
  m_walls.paintGL(&m_camera.m_viewMatrix);

  m_obstacles.paintGL(&m_camera.m_viewMatrix);

  m_mappingMode = 0;
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);
  m_car.render(&m_camera.m_viewMatrix);

  abcg::glUseProgram(0);
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() {
  m_ground.terminateGL();
  m_walls.terminateGL();
  m_obstacles.terminateGL();
  m_car.terminateGL();
  abcg::glDeleteProgram(m_program);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  const float forwardspeed = m_forwardSpeed * deltaTime;
  const float lateralspeed = m_latSpeed * deltaTime;

  m_camera.update(forwardspeed, lateralspeed);
  m_car.update(forwardspeed, lateralspeed);
  m_obstacles.update(forwardspeed);

  checkCollision();

  m_score += forwardspeed;

  m_lightPos.z = m_car.getPos().z + 0.1f;
  // m_lightPos.z = m_car.getPos().z + 1.0f;
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    ImVec2 size;
    ImVec2 position;
    size = ImVec2(180, 80);
    position = ImVec2((m_viewportWidth - size.x) / 2.0f,
                      (m_viewportHeight - size.y) / 20.0f);

    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    ImGui::Text(std::to_string(m_score).c_str());

    ImGui::PopFont();
    ImGui::End();
  }
}

void OpenGLWindow::checkCollision() {
  auto obs = m_obstacles.getPos().front();
  auto car = m_car.getPos();
  auto disX = abs(obs.x - car.x);
  auto disZ = abs(obs.z - car.z);
  if (disX < 0.46f && disZ < 0.4f) restart();
}
void OpenGLWindow::restart() {
  m_score = 0;
  m_obstacles.reset();
  m_camera.reset();
  m_car.reset();
}
