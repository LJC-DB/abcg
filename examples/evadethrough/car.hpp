#ifndef CAR_HPP_
#define CAR_HPP_

#include <string>
#include <vector>

#include "abcg.hpp"
#include "model.hpp"

class Car : public Model {
 public:
  void setupVAO(GLuint program);
  void loadObj(std::string path, bool standardize = true);
  void render(glm::mat4* viewMatrix, int numTriangles = -1);
  void update(float forwardspeed, float lateralspeed);
  void reset() { position = glm::vec3(0.0f, 0.14f, 0.0f); }

  [[nodiscard]] glm::vec3 getPos() const { return position; }

 private:
  GLint modelMatrixLoc;
  GLint normalMatrixLoc;
  GLint KaLoc;
  GLint KdLoc;
  GLint KsLoc;
  // GLint IsLoc;
  // GLint IdLoc;
  // GLint IaLoc;
  GLint shininessLoc;
  // GLint colorLoc;

  glm::vec3 position{0.0f, 0.14f, 0.0f};
};

#endif
