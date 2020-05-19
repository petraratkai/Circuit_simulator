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
  voltage (const std::string& name="", const std::string& node1="", const std::string& node2="", double dc_offset=0,
    double amplitude = 0, double frequency = 1)
    : component(name, node1, node2), dc_offset(dc_offset),
      amplitude(amplitude), frequency(frequency) {}
  double get_voltage(double t); //returns voltage at t time
  double get_current()override;
};

#endif
