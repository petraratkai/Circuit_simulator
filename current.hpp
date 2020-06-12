#ifndef CURRENT_HPP
#define CURRENT_HPP

#include <iostream>
#include "component.hpp"
#include <cmath>

class current :public component //current source, can be either DC or sinusoidal
{
private:
  double dc_offset;
  double amplitude;
  double frequency;
public:
  current(const std::string& name="", const std::string& node1="", const std::string& node2="", double dc_offset=0,
    double amplitude = 0, double frequency = 1)
    : component(name, node1, node2), dc_offset(dc_offset),
      amplitude(amplitude), frequency(frequency) {}
  double get_current(double t) override//returns current at t time, it should be the override though, think about it
  {
    return dc_offset + amplitude * sin(2*M_PI*frequency*t);
  }
  void set_dc_offset(double i) override {dc_offset = i;}

  current* clone() override
  {
    return new current(*this);
  }
  bool is_current() {return true;}

};

#endif
