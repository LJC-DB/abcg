#include "obstacles.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>

void Obstacles::initializeGL(GLuint program, std::string path) {
  m_model.loadObj(path + "box.obj");
  m_model.setupVAO(program);
  m_model.m_Ka = {0.2f, 0.1f, 0.1f, 1.0f};
  m_model.m_Kd = {0.5f, 0.4f, 0.4f, 1.0f};
  m_model.m_Ks = {0.2f, 0.2f, 0.2f, 1.0f};

  // Save location of uniform variables
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  normalMatrixLoc = abcg::glGetUniformLocation(program, "normalMatrix");
  KaLoc = abcg::glGetUniformLocation(program, "Ka");
  KdLoc = abcg::glGetUniformLocation(program, "Kd");
  KsLoc = abcg::glGetUniformLocation(program, "Ks");
  shininessLoc = abcg::glGetUniformLocation(program, "shininess");
  // m_colorLoc = abcg::glGetUniformLocation(program, "color");

  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  reset();
}

void Obstacles::paintGL(glm::mat4* viewMatrix) {
  // abcg::glUniform4f(m_colorLoc, 0.0f, 0.0f, 1.0f, 1.0f);

  abcg::glUniform1f(shininessLoc, m_model.m_shininess);
  abcg::glUniform4fv(KaLoc, 1, &m_model.m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_model.m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_model.m_Ks.x);

  for (const auto pos : positions) {
    // Set model matrix
    glm::mat4 model{1.0f};
    model = glm::translate(model, pos);
    model = glm::scale(model, glm::vec3(0.4f));

    abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

    const auto modelViewMatrix{glm::mat3((*viewMatrix) * model)};
    glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
    abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

    m_model.render();
  }
}

void Obstacles::update(float forwardspeed) {
  posZ -= forwardspeed;

  if (posZ <= -4) {
    positions.pop_front();

    for (auto& pos : positions) pos.z += 4;

    auto& re{m_randomEngine};
    std::uniform_real_distribution<float> randPos(-1.45f, 1.45f);
    positions.push_back(
        glm::vec3(randPos(re), 0.11547f, -2.0f - 4.0f * (n - 1) + randPos(re)));

    posZ += 4;
  }
}

void Obstacles::reset() {
  posZ = 0.0f;
  auto& re{m_randomEngine};
  std::uniform_real_distribution<float> randPos(-1.45f, 1.45f);
  positions.clear();
  for (auto i : iter::range(n))
    positions.push_back(
        glm::vec3(randPos(re), 0.2309f, -2.0f - 4.0f * i + randPos(re)));
}

void Obstacles::terminateGL() { m_model.terminateGL(); }
