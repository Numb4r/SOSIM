#pragma once
#include <map>

class RAM {
  const int memMAX;

  // std::map<int,>
public:
  void reset();
  RAM(const int memMax);
  RAM();
};