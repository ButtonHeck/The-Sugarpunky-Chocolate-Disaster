#pragma once

class BuildableGenerator;

class BuildableRenderer
{
public:
  BuildableRenderer(BuildableGenerator& generator) noexcept;
  void renderBuildable();
  void renderSelected();

private:
  BuildableGenerator& generator;
};
