#ifndef RESISTOR_HPP
#define RESISTOR_HPP

#include <iostream>
#include "component.hpp"

class resistor: public component
{
private:
  double resistance;

public:
  resistor(const std::string& name = "", const std::string& node1 = "", const std::string& node2 ="", double resistance=0)
    : component(name, node1, node2, component_type::R), resistance(resistance) {}
    double get_conductance() override {return 1/resistance;} //returns 1/resistance
//  void set_resistance(const std::string& r);
/*  double get_current() override //returns the current (parameter probably should be the nodes)
  {

  } */
  resistor* clone() override
  {
    return new resistor(*this);
  }


};
#endif
