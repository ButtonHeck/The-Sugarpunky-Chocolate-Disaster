#include "LandFacade"

LandFacade::LandFacade(Shader &renderShader) noexcept
  :
    shader(renderShader),
    generator(),
    renderer(generator)
{}

void LandFacade::setup(const map2D_f &shoreMap)
{
  generator.setup(shoreMap);
}

void LandFacade::serialize(std::ofstream &output)
{
  generator.serialize(output);
}

void LandFacade::deserialize(std::ifstream &input)
{
  generator.deserialize(input);
}

void LandFacade::draw(const glm::vec3 &lightDir,
                      const std::array<glm::mat4, NUM_SHADOW_LAYERS> &lightSpaceMatrices,
                      const glm::mat4& projectionView,
                      bool useShadows)
{
  shader.update(lightDir, lightSpaceMatrices, projectionView, useShadows);
  renderer.render();
}

const map2D_f &LandFacade::getMap() const noexcept
{
  return generator.getMap();
}

void LandFacade::updateCellsIndirectBuffer(const Frustum &frustum)
{
  generator.updateCellsIndirectBuffer(frustum);
}
