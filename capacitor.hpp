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
  capacitor(const std::string& name="", const std::string& node1="", const std::string& node2="", double capacitance=0)
    : component(name, node1, node2, component_type::L), capacitance(capacitance), previous_voltage(0), next_voltage(0)
    {}
  double get_capacitance() //returns the inductance (do we need set_capacitance?)
  {return capacitance;}
  void set_capacitance(const std::string& c); //sets the capacitance

  double get_previous_voltage() //returns the previous current
  {return previous_voltage;  }
  void set_previous_voltage(double i); //sets the prev. current

  double get_next_voltage(); //returns the next (current) current (is it necessary?)
  void set_next_voltage(double v) //sets the next current needs to be virtual
  {
  next_voltage = v;
  }

//  double get_current() override; //calculates and returns the current

};

#endif
