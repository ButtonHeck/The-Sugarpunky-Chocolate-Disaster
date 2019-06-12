#pragma once

namespace RendererStateManager
{
  void setInitialRenderingState(bool useMultisample) noexcept;
  void setAmbienceRenderingState(bool isOn) noexcept;
};
