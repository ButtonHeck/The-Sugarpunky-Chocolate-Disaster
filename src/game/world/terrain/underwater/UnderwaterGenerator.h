#ifndef UNDERWATERGENERATOR_H
#define UNDERWATERGENERATOR_H
#include "Generator"

class UnderwaterGenerator : public Generator
{
public:
  UnderwaterGenerator();
  virtual ~UnderwaterGenerator() = default;
private:
  friend class UnderwaterRenderer;
};

#endif // UNDERWATERGENERATOR_H
