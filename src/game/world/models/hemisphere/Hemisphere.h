#ifndef HEMISPHERE_H
#define HEMISPHERE_H
#include "game/world/models/Model.h"

class Hemisphere
{
public:
  Hemisphere(const std::string& path);
  void draw();
private:
  Model model;
  glm::mat4 transform;
};

#endif // HEMISPHERE_H
