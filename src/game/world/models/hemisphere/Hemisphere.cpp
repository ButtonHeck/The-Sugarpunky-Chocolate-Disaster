#include "game/world/models/hemisphere/Hemisphere.h"

Hemisphere::Hemisphere(const std::string &path)
  :
    model(path, false)
{}

void Hemisphere::draw()
{
  model.drawDirect();
}
