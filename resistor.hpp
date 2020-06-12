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
    : component(name, node1, node2), resistance(resistance) {}
    double get_conductance() override {return 1/resistance;} //returns 1/resistance

  resistor* clone() override
  {
    return new resistor(*this);
  }
  bool is_resistor() override {return true;}


};
#endif
