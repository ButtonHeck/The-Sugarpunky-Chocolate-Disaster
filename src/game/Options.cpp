#include "game/Options.h"

Options::Options()
{
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
  options[OPT_DRAW_LAND] = true;
  options[OPT_DRAW_WATER] = true;
  options[OPT_MODELS_LAND_BLENDING] = true;
  options[OPT_MODELS_PHONG_SHADING] = true;
  options[OPT_POLYGON_LINE] = false;
  options[OPT_DEBUG_RENDER] = false;
  options[OPT_SHADOW_CAMERA_FIXED] = false;
  options[OPT_USE_SHADOW_CAMERA_MATRIX] = false;
  options[OPT_SHOW_SHADOW_VOLUME_VISUALIZATION] = true;
}

std::bitset<OPTIONS_COUNT>::reference Options::operator[](OPTION value)
{
  return options[value];
}

void Options::toggle(OPTION value)
{
  options.flip(value);
}

