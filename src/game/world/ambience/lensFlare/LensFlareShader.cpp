#include "game/world/ambience/lensFlare/LensFlareShader.h"

LensFlareShader::LensFlareShader(Shader &renderShader)
  :
    renderShader(renderShader)
{}

void LensFlareShader::update(float brightnessFlares, float brightnessHalo)
{
  renderShader.use();
  renderShader.setFloat("u_brightnessFlare", brightnessFlares);
  renderShader.setFloat("u_brightnessHalo", brightnessHalo);
}
