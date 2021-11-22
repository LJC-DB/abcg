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
  void render(int numTriangles = -1);
  void update(float forwardspeed, float lateralspeed);

 private:
  GLint modelMatrixLoc;
  GLint colorLoc;

  glm::vec3 position{0.0f, 0.18f, 0.0f};
};

#endif
