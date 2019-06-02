#ifndef OPTIONS_H
#define OPTIONS_H
#include <bitset>
#include "../../src/game/OptionsUnits.h"

class Options
{
public:
  Options();
  std::bitset<OPTIONS_COUNT>::reference operator[](OPTION value);
  void toggle(OPTION value);

private:
  std::bitset<OPTIONS_COUNT> options;
};

#endif // OPTIONS_H
