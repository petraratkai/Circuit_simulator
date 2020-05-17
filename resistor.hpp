#ifndef RESISTOR_HPP
#define RESISTOR_HPP

#include <iostream>
#include "component.hpp"

class resistor: public component
{
private:
  double resistance;

public:
  double get_conductance(); //returns 1/resistance
  double get_current() override; //returns the current (parameter probably should be the nodes)

};
#endif
