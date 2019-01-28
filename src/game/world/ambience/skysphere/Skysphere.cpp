#include "game/world/ambience/skysphere/Skysphere.h"

Skysphere::Skysphere(const std::string &path)
  :
    model(path, false)
{}

void Skysphere::draw()
{
  model.drawDirect();
}
