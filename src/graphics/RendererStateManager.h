#pragma once

#include <GL/glew.h>

namespace RendererStateManager
{
  void setInitialRenderingState(bool useMultisample);
  void setAmbienceRenderingState(bool isOn);
};
