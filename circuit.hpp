#ifndef CIRCUIT_HPP
#define CIRCUIT_HPP

#include <iostream>
#include <string>
#include <vector>

#include "component.hpp"
#include "node.hpp"

//enum class component_type {R, I, V, C, L};

class circuit {
private:            //protected?
    double timestep;
    double stoptime;
    std::vector<component*> components; //vector of all the components in the circuit, heterogeneous collection
    std::vector<node> nodes; //vector of nodes in the circuit
  //  void set_stoptime(const std::string& t); //converts t to double, and sets stoptime to that value
    std::vector <std::vector<std::string>> supernodes; //name of the nodes that are connected via a voltage source
public:
    void analyse(); //should this print the circuit out or
                                                    //should we have another function for that
    void add_component(component* c); //not sure if needed

    void read_in(std::istream& is); //parses in the circuit from the netlist

    void write_out(std::ostream& os);

    int find_node_index(std::string name);

    double get_current(component& comp);





    ~circuit();

};

#endif
