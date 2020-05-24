#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "circuit.hpp"
#include "node.hpp"
#include "component.hpp"
#include "capacitor.hpp"
#include "resistor.hpp"
#include "inductor.hpp"
#include "voltage.hpp"
#include "current.hpp"

using namespace std;

circuit& make_dc() { //returns the dc version of the circuit, this is for the operating point
    for (int j = 0; j < components.size(); ++j) {
        if (components[j].is_capacitor()){
            string node1 = components[i].node1;
            string node2 = components[i].node2;
            current* i = new current(components[i].name, node1, node2, 0);
            add_component(i);
        }
        else if (components[j].is_inductor()) {
            string node1 = components[i].node1;
            string node2 = components[i].node2;
            voltage* v = new voltage(components[i].name, node1, node2, 0);
            add_component(v);
        }
    }
}

circuit& make_linear() { //returns the circuit, linear components instead of C and L

}