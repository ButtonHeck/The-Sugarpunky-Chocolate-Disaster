#pragma once

#include "OptionsUnits"

#include <bitset>

class Options
{
public:
  Options() noexcept;
  std::bitset<OPTIONS_COUNT>::reference operator[](OPTION value);
  void toggle(OPTION value);

private:
  std::bitset<OPTIONS_COUNT> options;
};
