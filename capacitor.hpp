#ifndef CAPACITOR_HPP
#define CAPACITOR_HPP

#include <iostream>
#include "component.hpp"

class capacitor
    :public component
{
private:
    double capacitance;
    double previous_voltage;
    double next_voltage;
public:
  double get_capacitance(); //returns the inductance (do we need set_capacitance?)
  void set_capacitance(const std::string& c); //sets the capacitance

  double get_previous_voltage(); //returns the previous current
  void set_previous_voltage(double i); //sets the prev. current

  double get_next_voltage(); //returns the next (current) current (is it necessary?)
  void set_next_voltage(double i); //sets the next current

  double get_current() override; //calculates and returns the current

};

#endif
