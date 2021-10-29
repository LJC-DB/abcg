#include "board.hpp"

#include <fmt/core.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <vector>

void Board::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  // m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  std::array<glm::vec2, 24> positions{
      glm::vec2{-1, -1},   glm::vec2{-.9f, -1},
      glm::vec2{-1, -.3f}, glm::vec2{-.9f, -.3f},

      glm::vec2{-1, 1},    glm::vec2{-.9f, 1},
      glm::vec2{-1, .3f},  glm::vec2{-.9f, .3f},

      glm::vec2{1, -1},    glm::vec2{.9f, -1},
      glm::vec2{1, -.3f},  glm::vec2{.9f, -.3f},

      glm::vec2{1, 1},     glm::vec2{.9f, 1},
      glm::vec2{1, .3f},   glm::vec2{.9f, .3f},

      glm::vec2{-1, -1},   glm::vec2{-1, -.9f},
      glm::vec2{1, -1},    glm::vec2{1, -.9f},

      glm::vec2{-1, 1},    glm::vec2{-1, .9f},
      glm::vec2{1, 1},     glm::vec2{1, .9f},
  };

  // std::vector<glm::vec2> indices(0);
  std::array<int, 36> indices{0};
  for (size_t i = 0; i < 6; i++) {
    auto x = i * 6;
    auto y = i * 4;
    indices.at(x) = y;
    indices.at(x + 1) = y + 1;
    indices.at(x + 2) = y + 2;
    indices.at(x + 3) = y + 1;
    indices.at(x + 4) = y + 2;
    indices.at(x + 5) = y + 3;
  }

  // for (const auto i : indices)
  //   fmt::print("{} - ({},{})\n", i, positions.at(i).x, positions.at(i).y);

  // Generate VBO
  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Board::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  // abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, sides + 2);
  abcg::glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Board::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

// void Board::update(const GameData &gameData, float deltaTime) {}
