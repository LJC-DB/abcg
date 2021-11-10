#include "player.hpp"

#include <cppitertools/itertools.hpp>
#include <vector>

void Player::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  players.at(1).m_type = Type::P2;

  for (auto &plyr : players) {
    plyr.m_velocity = glm::vec2(0);
    if (plyr.m_type == Type::P1) {
      plyr.m_color = glm::vec3(1, 0, 0);
      plyr.m_translation = glm::vec2(-.75, 0);
    } else {
      plyr.m_color = glm::vec3(0, 0, 1);
      plyr.m_translation = glm::vec2(.75, 0);
    }
  }

  std::vector<glm::vec2> positions(0);
  positions.emplace_back(0, 0);
  const auto step{M_PI * 2 / sides};
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
  }
  positions.push_back(positions.at(1));

  positions.emplace_back(0, 0);
  for (const auto angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(.4 * std::cos(angle), .4 * std::sin(angle));
  }
  positions.push_back(positions.at(sides + 3));

  // Generate VBO
  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Player::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  for (const auto &plyr : players) {
    abcg::glBindVertexArray(m_vao);

    abcg::glUniform1f(m_scaleLoc, m_scale);
    abcg::glUniform2fv(m_translationLoc, 1, &plyr.m_translation.x);

    auto color = glm::vec4{plyr.m_color, 1};
    abcg::glUniform4fv(m_colorLoc, 1, &color.r);
    abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, sides + 2);

    color = glm::vec4{.4f * plyr.m_color, 1};
    abcg::glUniform4fv(m_colorLoc, 1, &color.r);
    abcg::glDrawArrays(GL_TRIANGLE_FAN, sides + 2, sides + 2);

    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);
}

void Player::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Player::update(const GameData &gameData, float deltaTime) {
  if (gameData.m_state != State::Playing) return;
  for (auto &plyr : players) {
    plyr.m_velocity = glm::vec2{0};

    if (plyr.m_type == Type::P1) {
      if (gameData.m_input[static_cast<size_t>(Input::W)]) {
        plyr.m_velocity += glm::vec2{0.0f, 1.0f};
      }
      if (gameData.m_input[static_cast<size_t>(Input::S)]) {
        plyr.m_velocity += glm::vec2{0.0f, -1.0f};
      }
      if (gameData.m_input[static_cast<size_t>(Input::D)]) {
        plyr.m_velocity += glm::vec2{1.0f, 0.0f};
      }
      if (gameData.m_input[static_cast<size_t>(Input::A)]) {
        plyr.m_velocity += glm::vec2{-1.0f, 0.0f};
      }
    } else {
      if (gameData.m_input[static_cast<size_t>(Input::Up)]) {
        plyr.m_velocity += glm::vec2{0.0f, 1.0f};
      }
      if (gameData.m_input[static_cast<size_t>(Input::Down)]) {
        plyr.m_velocity += glm::vec2{0.0f, -1.0f};
      }
      if (gameData.m_input[static_cast<size_t>(Input::Right)]) {
        plyr.m_velocity += glm::vec2{1.0f, 0.0f};
      }
      if (gameData.m_input[static_cast<size_t>(Input::Left)]) {
        plyr.m_velocity += glm::vec2{-1.0f, 0.0f};
      }
    }

    if (plyr.m_velocity != glm::vec2{0})
      plyr.m_velocity = glm::normalize(plyr.m_velocity);

    plyr.m_translation += plyr.m_velocity * deltaTime;
    if (plyr.m_type == Type::P1)
      plyr.m_translation.x =
          std::clamp(plyr.m_translation.x, -.9f + m_scale, -.1f - m_scale);
    else
      plyr.m_translation.x =
          std::clamp(plyr.m_translation.x, .1f + m_scale, .9f - m_scale);

    plyr.m_translation.y =
        std::clamp(plyr.m_translation.y, -.9f + m_scale, .9f - m_scale);
  }
}
