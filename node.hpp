#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <string>

class node {

private:
    std::string name;
    double voltage;
public:
    node(std::string name, double voltage = 0): name(name), voltage(voltage){}
    double get_voltage(); //returns the voltage of the node
    std::string get_name(){
      return name;
    } //returns the name of the node
    void set_voltage(double v); //sets the voltage to v Volts
};

#endif
