#include "puck.hpp"

#include <cppitertools/itertools.hpp>
#include <vector>

void Puck::initializeGL(GLuint program) {
  terminateGL();

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  auto &re{m_randomEngine};
  std::uniform_real_distribution<float> initialVelocity(-.5f, .5f);

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_translation = glm::vec2{0};
  m_velocity = glm::vec2{initialVelocity(re), initialVelocity(re)};

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

void Puck::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, sides + 2);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Puck::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Puck::update(Player &m_player, const GameData &gameData, float deltaTime) {
  if (gameData.m_state != State::Playing) return;
  if (m_velocity == glm::vec2{0}) return;

  bool collided{false};

  float diff{};

  m_translation += m_velocity * deltaTime;
  diff = abs(m_translation.x) + m_scale - .9f;
  auto pos_y = abs(m_translation.y) + m_scale;

  if (diff >= 0 && pos_y >= .3f) {
    collided = true;
    m_velocity.x *= -1;
    m_translation.x =
        std::clamp(m_translation.x, -.9f + m_scale, .9f - m_scale);
    m_translation.x += copysign(diff, m_velocity.x);
    m_velocity *= .95;
  }

  diff = abs(m_translation.y) + m_scale - .9f;
  if (diff >= 0) {
    collided = true;
    m_velocity.y *= -1;
    m_translation.y =
        std::clamp(m_translation.y, -.9f + m_scale, .9f - m_scale);
    m_translation.y += copysign(diff, m_velocity.y);
    m_velocity *= .95;
  }

  for (auto &plyr : m_player.players) {
    auto dist{glm::distance(m_translation, plyr.m_translation)};
    auto rdist{m_scale + m_player.m_scale};
    if (dist <= rdist) {
      collided = true;
      auto n = glm::normalize(m_translation - plyr.m_translation);

      if (glm::dot(m_velocity, n) <= 0)
        m_velocity = glm::reflect(m_velocity, n);

      auto cp{n * rdist + plyr.m_translation};

      m_translation = cp + (rdist - dist) * glm::normalize(m_velocity);

      m_velocity += plyr.m_velocity;
      m_velocity *= .95;
    }
    if (m_velocity.x == 0) m_velocity.x = copysign(.1f, m_velocity.y);
    auto v{glm::length(m_velocity)};
    if (v > 2.0f)
      m_velocity = glm::normalize(m_velocity) * 2.0f;
    else if (v < .2f)
      m_velocity = glm::normalize(m_velocity) * .2f;
  }

  if (collided == false)
    m_velocity -= glm::normalize(m_velocity) * .1f * deltaTime;
}
