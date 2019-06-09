#pragma once

class BuildableGenerator;

class BuildableRenderer
{
public:
  BuildableRenderer(BuildableGenerator& generator);
  void renderBuildable();
  void renderSelected();

private:
  BuildableGenerator& generator;
};
