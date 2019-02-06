#include "game/world/ambience/lensFlare/LensFlareFacade.h"

LensFlareFacade::LensFlareFacade(Shader& shader, TextureLoader &textureLoader, const ScreenResolution &screenResolution)
  :
    BRIGHTNESS_HALO(1.25f),
    BRIGHTNESS_FLARES(0.55f),
    basicGLBuffers(VAO | VBO),
    shader(shader),
    renderer(basicGLBuffers)
{
  flares.reserve(NUM_LENS_FLARES);
  flares.emplace_back(1300.0f, textureLoader.loadTexture("lensFlares/flare11.png", 0, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false, true, true));
  flares.emplace_back(220.0f, textureLoader.loadTexture("lensFlares/flare2.png", 0, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false, true, true));
  flares.emplace_back(150.0f, textureLoader.loadTexture("lensFlares/flare5.png", 0, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false, true, true));
  flares.emplace_back(75.0f, textureLoader.loadTexture("lensFlares/flare6.png", 0, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false, true, true));
  flares.emplace_back(160.0f, textureLoader.loadTexture("lensFlares/flare1.png", 0, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false, true, true));
  flares.emplace_back(180.0f, textureLoader.loadTexture("lensFlares/flare9.png", 0, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false, true, true));
  flares.emplace_back(340.0f, textureLoader.loadTexture("lensFlares/flare3.png", 0, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, false, true, true));

  for (unsigned int i = 0; i < NUM_LENS_FLARES; i++)
    BindlessTextureManager::emplaceBack("u_flares[" + std::to_string(i) + "]", flares[i].getTextureID(), BINDLESS_TEXTURE_LENS_FLARE);

  basicGLBuffers.bind(VAO | VBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

  float pointSizeDivisorX = screenResolution.getWidthRatioToReference();
  float pointSizeDivisorY = screenResolution.getHeightRatioToReference();
  float pointSizeDivisor = (pointSizeDivisorX + pointSizeDivisorY) / 2;
  for (LensFlareElement& flare : flares)
    flare.adjustPointSize(pointSizeDivisor);
}

void LensFlareFacade::draw(const glm::vec3& sunWorldPosition, const glm::mat4& projectionView, float theSunVisibility)
{
  glm::vec4 sunCameraSpacePosition = projectionView * glm::vec4(sunWorldPosition, 1.0f);
  if (sunCameraSpacePosition.w > 0)
    {
      //map x,y from NDC[-1;1] to the texture coordinates[0;1] with y-axis reflected
      float x = (sunCameraSpacePosition.x / sunCameraSpacePosition.w + 1) / 2.0f;
      float y = 1.0f - ((sunCameraSpacePosition.y / sunCameraSpacePosition.w + 1) / 2.0f);
      glm::vec2 sunScreenPosition(x, y);
      glm::vec2 sunScreenPositionToScreenCenter = glm::vec2(0.5f) - sunScreenPosition;
      float sunToCenterLength = glm::length(sunScreenPositionToScreenCenter);
      float brightnessFlares = glm::max((1 - sunToCenterLength / BRIGHTNESS_FLARES) * theSunVisibility, 0.0f);
      float brightnessHalo = glm::max((1 - sunToCenterLength / BRIGHTNESS_HALO) * theSunVisibility, 0.0f);
      updateFlaresPositions(sunScreenPosition, sunScreenPositionToScreenCenter);
      shader.update(brightnessFlares, brightnessHalo);
      renderer.draw(NUM_LENS_FLARES);
    }
}

void LensFlareFacade::updateFlaresPositions(const glm::vec2 &sunScreenPosition, const glm::vec2 &sunScreenPositionToScreenCenter)
{
  for (unsigned int flareIndex = 0; flareIndex < flares.size(); flareIndex++)
    {
      flares[flareIndex].setPosition(sunScreenPosition + flareIndex * FLARES_SPACING * sunScreenPositionToScreenCenter);
      vertices[flareIndex*3 + 0] = flares[flareIndex].getPosition().x;
      vertices[flareIndex*3 + 1] = flares[flareIndex].getPosition().y;
      vertices[flareIndex*3 + 2] = flares[flareIndex].getPointSize();
    }
  basicGLBuffers.bind(VAO | VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}
