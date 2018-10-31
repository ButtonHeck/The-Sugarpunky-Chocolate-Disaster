#include "game/Options.h"

Options::Options()
{
  options[OPT_MODELS_SHADOW_EMPHASIZE] = true;
  options[OPT_DRAW_TREES] = true;
  options[OPT_ANIMATE_WATER] = true;
  options[OPT_DRAW_DEBUG_TEXT] = true;
  options[OPT_RECREATE_TERRAIN_REQUEST] = false;
  options[OPT_SAVE_REQUEST] = false;
  options[OPT_LOAD_REQUEST] = false;
  options[OPT_SHOW_CURSOR] = false;
  options[OPT_DRAW_BUILDABLE] = false;
  options[OPT_HILLS_CULLING] = false;
  options[OPT_WATER_CULLING] = false;
  options[OPT_USE_MULTISAMPLING] = true;
  options[OPT_USE_SHADOWS] = true;
  options[OPT_CREATE_SHADOW_MAP_REQUEST] = true;
  options[OPT_DRAW_LAND] = true;
  options[OPT_DRAW_WATER] = true;
  options[OPT_MODELS_FLAT_BLENDING] = true;
  options[OPT_MODELS_PHONG_SHADING] = true;
  options[OPT_POLYGON_LINE] = false;
}

bool Options::get(OPTION value)
{
  return options[value];
}

void Options::set(OPTION value, bool flag)
{
  options[value] = flag;
}

void Options::toggle(OPTION value)
{
  options[value] = !options[value];
}
