#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <iostream>

class component { //abstract class

private:
    std::string name;
    std::string node1;
    std::string node2;
    double current;

public:
    component(std::string name, std::string node1, std::string node2)
      :name(name), node1(node1), node2(node2), current(0) {}

    virtual double get_current() = 0; //purely virtual function, because the method
    //of getting the current is different for different types of components

    bool is_capacitor(); //returns true if the component is a capacitor, otherwise false
    bool is_inductor(); //returns true if the component is an inductor, otherwise false
    bool is_resistor(); //returns true if the component is a resistor, otherwise false
    bool is_voltage(); //returns true if the component is a voltage source, otherwise false
    bool is_current(); //returns true if the component is a capacitor, otherwise false
    //not sure if these are needed

    std::string get_name(); //returns the name of the component
    void set_name(const std::string& s);
    std::string get_node1(); //returns node1
    void set_node1(const std::string& n1);
    std::string get_node2();
    void set_node2(const std::string& n2); //returns node2
};

#endif
