#ifndef SKYSPHERE_H
#define SKYSPHERE_H
#include "game/world/models/Model.h"

class Skysphere
{
public:
  Skysphere(const std::string& path);
  void draw();
private:
  Model model;
  glm::mat4 transform;
};

#endif // SKYSPHERE_H
