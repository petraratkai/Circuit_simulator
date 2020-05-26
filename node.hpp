#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <string>

class node {

private:
    std::string name;
    bool is_connected_to_v; //true if the node is connected to a voltage source
    double voltage;

public:
    node(std::string name, bool is_connected_to_v, double voltage = 0): name(name), is_connected_to_v(is_connected_to_v), voltage(voltage){}
    double get_voltage() //returns the voltage of the node
    { return voltage;}
    std::string get_name(){
      return name;
    } //returns the name of the node
    void set_voltage(double v); //sets the voltage to v Volts
    bool is_connnectedtov() {return is_connected_to_v;}
    void set_connectedtov(bool conn) {is_connected_to_v=conn;}
};

#endif
