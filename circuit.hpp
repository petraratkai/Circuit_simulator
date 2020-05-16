#ifndef CIRCUIT_HPP
#define CIRCUIT_HPP

#include <iostream>
#include <string>
#include <vector>

#include "component.hpp"
#include "node.hpp"


class circuit {
private:            //protected?
    double timestep;
    double stoptime;
    std::vector<component> components; //vector of all the components in the circuit, heterogeneous collection
    std::vector<node> nodes; //vector of nodes in the circuit
public:
    void analyse(double timestep, double stoptime); //should this print the circuit out or
                                                    //should we have another function for that
    void add_component(component& c); //not sure if needed
};

#endif
