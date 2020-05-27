#ifndef INDUCTOR_HPP
#define INDUCTOR_HPP

#include <iostream>
#include "component.hpp"

class inductor
    :public component
{
private:
    double inductance;
    double previous_current;
    //double next_current;
public:
  inductor(const std::string& name="", const std::string& node1="", const std::string& node2="", double inductance=0)
    : component(name, node1, node2, component_type::L), inductance(inductance), previous_current(0)
    {}
  double get_inductance(); //returns the inductance (do we need set?)

  double get_previous_current(); //returns the previous current
  void set_previous_current(double i); //sets the prev. current

  double get_next_current(); //returns the next (current) current (is it necessary?)
  void set_next_current(double i); //sets the next current

/*  double get_current() override; //returns the current*/

};

#endif
