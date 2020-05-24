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

circuit circuit::make_dc() //returns the dc version of the circuit, this is for the operating point
{
  circuit dc_equiv; //this is what we are going to return
  for(int i = 0; i<components.size(); i++)
  {
    if(components[i]->is_current())
    {
      current *comp = new current;
      *comp = *(dynamic_cast<current*> (components[i]));
      dc_equiv.add_component(comp);
    }
    else if(components[i]->is_voltage())
    {
      voltage *comp = new voltage;
      *comp = *(dynamic_cast<voltage*>(components[i]));
      dc_equiv.add_component(comp);
    }
    else if(components[i]->is_resistor())
    {
      resistor *comp = new resistor;
      *comp = *(dynamic_cast<resistor*>(components[i]));
      dc_equiv.add_component(comp);
    }
    else if(components[i]->is_inductor()) //have to make inductors short circuits.
    {
      voltage *comp = new voltage(components[i]->get_name(), components[i]->get_node1(), components[i]->get_node2());
      dc_equiv.add_component(comp);
      components.push_back(comp);
    }
    else
    {
      current *comp = new current(components[i]->get_name(), components[i]->get_node1(), components[i]->get_node2());
      dc_equiv.add_component(comp);
      components.push_back(comp);
    }
  }
  return dc_equiv;
}
    /*for (int j = 0; j < components.size(); ++j) {
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
    }*/


circuit& make_linear()  //returns the circuit, linear components instead of C and L
{

}
