#include "Options.h"

Options::Options()
{
  options.assign(
  {
    {OPT_TREES_SHADOW_EMPHASIZE, true},
    {OPT_DRAW_TREES, true},
    {OPT_ANIMATE_WATER, true},
    {OPT_DRAW_DEBUG_TEXT, true},
    {OPT_RECREATE_TERRAIN_REQUEST, false},
    {OPT_SAVE_REQUEST, false},
    {OPT_LOAD_REQUEST, false},
    {OPT_SHOW_CURSOR, false},
    {OPT_DRAW_BUILDABLE, false},
    {OPT_HILLS_CULLING, false},
    {OPT_WATER_CULLING, false},
    {OPT_USE_MULTISAMPLING, true},
    {OPT_USE_SHADOWS, true},
    {OPT_CREATE_SHADOW_MAP_REQUEST, true},
    {OPT_DRAW_FLAT_TERRAIN, true},
    {OPT_DRAW_WATER, true},
    {OPT_MODELS_FLAT_BLENDING, true},
    {OPT_MODELS_PHONG_SHADING, true},
    {OPT_POLYGON_LINE, false}
  });
}

bool Options::get(OPTION value)
{
  return options[value].second;
}

void Options::set(OPTION value, bool flag)
{
  options[value].second = flag;
}

void Options::toggle(OPTION value)
{
  options[value].second = !options[value].second;
}
