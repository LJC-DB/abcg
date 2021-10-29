#include "player.hpp"

#include <fmt/core.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <vector>

void Player::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  // m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // m_rotation = 0.0f;
  // m_translation = glm::vec2(0);
  m_velocity = glm::vec2(0);
  if (m_type == Type::P1) {
    m_color = glm::vec3(1, 0, 0);
    m_translation = glm::vec2(-.75, 0);
  } else {
    m_color = glm::vec3(0, 0, 1);
    m_translation = glm::vec2(.75, 0);
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

  // for (const auto p : positions) {
  //   fmt::print("({},{})\n", p.x, p.y);
  // }

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

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  auto color = glm::vec4{m_color, 1};
  abcg::glUniform4fv(m_colorLoc, 1, &color.r);
  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, sides + 2);

  color = glm::vec4{.4f * m_color, 1};
  abcg::glUniform4fv(m_colorLoc, 1, &color.r);
  abcg::glDrawArrays(GL_TRIANGLE_FAN, sides + 2, sides + 2);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Player::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  // abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Player::update(const GameData &gameData, float deltaTime) {
  if (gameData.m_state != State::Playing) return;
  m_velocity = glm::vec2{0};

  if (m_type == Type::P1) {
    if (gameData.m_input[static_cast<size_t>(Input::W)]) {
      m_velocity += glm::vec2{0.0f, 1.0f};
    }
    if (gameData.m_input[static_cast<size_t>(Input::S)]) {
      m_velocity += glm::vec2{0.0f, -1.0f};
    }
    if (gameData.m_input[static_cast<size_t>(Input::D)]) {
      m_velocity += glm::vec2{1.0f, 0.0f};
    }
    if (gameData.m_input[static_cast<size_t>(Input::A)]) {
      m_velocity += glm::vec2{-1.0f, 0.0f};
    }
  } else {
    if (gameData.m_input[static_cast<size_t>(Input::Up)]) {
      m_velocity += glm::vec2{0.0f, 1.0f};
    }
    if (gameData.m_input[static_cast<size_t>(Input::Down)]) {
      m_velocity += glm::vec2{0.0f, -1.0f};
    }
    if (gameData.m_input[static_cast<size_t>(Input::Right)]) {
      m_velocity += glm::vec2{1.0f, 0.0f};
    }
    if (gameData.m_input[static_cast<size_t>(Input::Left)]) {
      m_velocity += glm::vec2{-1.0f, 0.0f};
    }
  }

  if (m_velocity != glm::vec2{0})
    m_velocity = glm::normalize(m_velocity) * deltaTime;

  m_translation += m_velocity;
  if (m_type == Type::P1)
    m_translation.x =
        std::clamp(m_translation.x, -.9f + m_scale, -.1f - m_scale);
  else
    m_translation.x = std::clamp(m_translation.x, .1f + m_scale, .9f - m_scale);

  m_translation.y = std::clamp(m_translation.y, -.9f + m_scale, .9f - m_scale);
  // fmt::print("p:({},{}), v:{},{})\n", m_translation.x, m_translation.y,
  //            m_velocity.x, m_velocity.y);
}
