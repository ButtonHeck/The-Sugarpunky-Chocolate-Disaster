#include "RendererStateManager.h"

namespace RendererStateManager
{
  void setInitialRenderingState(bool useMultisample)
  {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_DITHER);
    if (useMultisample)
      glEnable(GL_MULTISAMPLE);
    else
      glDisable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

  void setAmbienceRenderingState(bool isOn)
  {
    if (isOn)
      {
        glDisable(GL_MULTISAMPLE);
        glDisable(GL_CULL_FACE);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
      }
    else
      {
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);
      }
  }
}
