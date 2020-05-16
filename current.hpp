#ifndef CURRENT_HPP
#define CURRENT_HPP

#include <iostream>
#include "component.hpp"

class current :public component //current source, can be either DC or sinusoidal
{
private:
  double dc_offset;
  double amplitude;
  double frequency;
public:
  double get_current(double t) ; //returns current at t time, it should be the override though, think about it
};

#endif
