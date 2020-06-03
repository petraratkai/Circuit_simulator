#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <iostream>


enum class component_type {R, I, V, C, L};


class component { //abstract class

private:
    std::string name;
    std::string node1;
    std::string node2;
    double current;
    component_type c_type;


public:
    component(std::string name="", std::string node1="", std::string node2="", component_type c_type=component_type::R)
      :name(name), node1(node1), node2(node2), current(0), c_type(c_type) {}

  //  virtual double get_current() = 0; //purely virtual function, because the method
    //of getting the current is different for different types of components

    bool is_capacitor() //returns true if the component is a capacitor, otherwise false
    {
      return c_type == component_type::C;
    }
    bool is_inductor() //returns true if the component is an inductor, otherwise false
    {
      return c_type == component_type::L;
    }
    bool is_resistor() //returns true if the component is a resistor, otherwise false
    {
      return c_type == component_type::R;
    }
    bool is_voltage() //returns true if the component is a voltage source, otherwise false
    {
      return c_type == component_type::V;
    }
    bool is_current() //returns true if the component is a capacitor, otherwise false
    {
      return c_type == component_type::I;
    }
    std::string get_name() {return name;}//returns the name of the component
    void set_name(const std::string& s);
    std::string get_node1() //returns node1
    {
      return node1;
    }
    void set_node1(const std::string& n1);
    std::string get_node2()
    {
      return node2;
    }
    void set_node2(const std::string& n2); //returns node2

    void set_current(double i) {current = i;}

    double get_current() {return current;}

    virtual component* clone() = 0;

    virtual double get_previous_voltage() {std::cerr<<"wrong type";}

    virtual void set_previous_voltage(double v) {std::cerr<<"wrong type";}

    virtual double get_next_voltage() {std::cerr<<"wrong type";}

    virtual double get_previous_current() {std::cerr<<"wrong type";}

    virtual  void set_previous_current(double i) {std::cerr<<"wrong type";}

    virtual void set_dc_offset(double v) {std::cerr<<"wrong type";}

    virtual double get_conductance() {std::cerr<<"wrong type";}

    virtual double get_current(double t) {std::cerr<<"wrong type";}

    virtual double get_voltage(double t) {std::cerr<<"wrong type";}

    virtual ~component() = default;

    //virtual double get_conductance();
};

#endif
