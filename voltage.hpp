#ifndef VOLTAGE_HPP
#define VOLTAGE_HPP

#include <iostream>
#include "component.hpp"

class voltage :public component //voltage source, can be either DC or sinusoidal
{
private:
  double dc_offset;
  double amplitude;
  double frequency;
public:
  double get_voltage(double t); //returns voltage at t time
  double get_current()override;
};

#endif
