#include "LandRenderer"
#include "LandGenerator"
#include "BenchmarkTimer"

LandRenderer::LandRenderer(LandGenerator &generator) noexcept
  :
    generator(generator)
{}

void LandRenderer::render()
{
  //square chunks are better to render without frustum culling
  {
    BENCHMARK("LandRenderer: draw chunks", true);
    generator.basicGLBuffers.bind(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, VERTICES_PER_QUAD, GL_UNSIGNED_BYTE, 0, generator.tiles.size());
  }

  //these ones should probably be culled with multiDrawIndirect
  {
    BENCHMARK("LandRenderer: draw cells", true);
    generator.cellBuffers.bind(VAO | DIBO);
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_BYTE, 0, generator.cellPrimitiveCount, 0);
  }
}
