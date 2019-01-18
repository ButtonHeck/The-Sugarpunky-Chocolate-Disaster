#include "game/world/ambience/lensFlare/LensFlareFacade.h"

LensFlareFacade::LensFlareFacade(Shader& shader, TextureLoader &textureLoader)
  :
    shader(shader),
    basicGLBuffers(VAO | VBO),
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
    {
      std::string uniformName("u_flares[" + std::to_string(i) + "]");
      BindlessTextureManager::emplaceBack(uniformName, flares[i].getTextureID(), BINDLESS_TEXTURE_LENS_FLARE);
    }

  basicGLBuffers.bind(VAO | VBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
}

void LensFlareFacade::draw(const glm::vec3& sunWorldPosition,
                           const glm::mat4& projectionView,
                           float theSunVisibility)
{
  glm::vec4 sunWorldPosTransformed = projectionView * glm::vec4(sunWorldPosition, 1.0f);
  if (sunWorldPosTransformed.w > 0)
    {
      float x = (sunWorldPosTransformed.x / sunWorldPosTransformed.w + 1) / 2.0f;
      float y = 1.0f - ((sunWorldPosTransformed.y / sunWorldPosTransformed.w + 1) / 2.0f);
      glm::vec2 sunScreenPos(x, y);
      glm::vec2 sunToCenter = glm::vec2(0.5f) - sunScreenPos;
      float sunToCenterLength = glm::length(sunToCenter);
      float brightnessFlares = glm::max((1 - sunToCenterLength / 0.55f) * theSunVisibility, 0.0f);
      float brightnessHalo = glm::max((1 - sunToCenterLength / 1.25f) * theSunVisibility, 0.0f);
      updatePositions(sunScreenPos, sunToCenter);
      shader.use();
      shader.setFloat("u_brightnessFlare", brightnessFlares);
      shader.setFloat("u_brightnessHalo", brightnessHalo);
      renderer.draw(NUM_LENS_FLARES);
    }
}

void LensFlareFacade::adjustFlaresPointSize(float pointSizeDivisor)
{
  for (LensFlareElement& flare : flares)
    flare.adjustPointSize(pointSizeDivisor);
}

void LensFlareFacade::updatePositions(glm::vec2 &sunScreenPosition, glm::vec2 &sunToCenter)
{
  for (unsigned int i = 0; i < flares.size(); i++)
    {
      flares[i].setPosition(sunScreenPosition + FLARES_SPACING * i * sunToCenter);
      vertices[i*3 + 0] = flares[i].getPosition().x;
      vertices[i*3 + 1] = flares[i].getPosition().y;
      vertices[i*3 + 2] = flares[i].getPointSize();
    }
  basicGLBuffers.bind(VAO | VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}
