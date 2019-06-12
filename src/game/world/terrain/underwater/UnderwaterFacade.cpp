#include "UnderwaterFacade"
#include "BenchmarkTimer"

UnderwaterFacade::UnderwaterFacade(Shader &renderShader) noexcept
  :
    shader(renderShader),
    generator(),
    renderer(generator)
{}

void UnderwaterFacade::draw(const glm::vec3 &lightDir, const glm::mat4& projectionView)
{
  BENCHMARK("UnderwaterRenderer: draw", true);
  shader.update(lightDir, projectionView);
  renderer.render();
}
