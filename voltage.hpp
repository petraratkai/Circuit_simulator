#ifndef VOLTAGE_HPP
#define VOLTAGE_HPP

#include <iostream>
#include "component.hpp"
#include <cmath>

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
  double get_voltage(double t) override {
    return dc_offset + amplitude*sin(2*M_PI*frequency*t);
  } //returns voltage at t time
  //double get_current()override;
  void set_dc_offset(double v) override {dc_offset = v;}

  voltage* clone() override
  {
    return new voltage(*this);
  }
  bool is_voltage() override {return true;}
};

#endif
