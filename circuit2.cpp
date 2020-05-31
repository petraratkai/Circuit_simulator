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
      *comp = *(static_cast<current*> (components[i]));
      dc_equiv.add_component(comp);
    }
    else if(components[i]->is_voltage())
    {
      voltage *comp = new voltage;
      *comp = *(static_cast<voltage*>(components[i]));
      dc_equiv.add_component(comp);
    }
    else if(components[i]->is_resistor())
    {
      resistor *comp = new resistor;
      *comp = *(static_cast<resistor*>(components[i]));
      dc_equiv.add_component(comp);
    }
    else if(components[i]->is_inductor()) //have to make inductors short circuits.
    {
      voltage *comp = new voltage;
      *comp = voltage(components[i]->get_name(), components[i]->get_node1(), components[i]->get_node2());
      dc_equiv.add_component(comp);

    }
    else
    {
      current *comp = new current;
      *comp = current(components[i]->get_name(), components[i]->get_node1(), components[i]->get_node2());
      dc_equiv.add_component(comp);
    
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


circuit circuit::make_linear()  //returns the circuit, linear components instead of C and L later try to change it so that we don't copy the circuit
{
    circuit linear_eq; // returning linear_eq which has type circuit
    double resistance;
    for(int i = 0; i<components.size(); i++) {

        if (components[i]->is_current()) {
            current *comp = new current;
            *comp = *(static_cast<current *> (components[i]));
            linear_eq.add_component(comp);
        } else if (components[i]->is_voltage()) {
            voltage *comp = new voltage;
            *comp = *(static_cast<voltage *>(components[i]));
            linear_eq.add_component(comp);
        } else if (components[i]->is_resistor()) {
            resistor *comp = new resistor;
            *comp = *(static_cast<resistor *>(components[i]));
            linear_eq.add_component(comp);
        } else if (components[i]->is_inductor()) //replace inductors with current source and resistor in parallel
        {
            resistance = static_cast<inductor*>(components[i])->get_inductance()/timestep; //still don't know how to do this
            current *comp = new current(components[i]->get_name(), components[i]->get_node1(),components[i]->get_node2(), static_cast<inductor*>(components[i])->get_previous_current());
            resistor *comp2 = new resistor(components[i]->get_name(), components[i]->get_node1(),components[i]->get_node2(), resistance); //still don't know how to do this
            linear_eq.add_component(comp);
            linear_eq.add_component(comp2);
        } else //replace capacitors with voltage source and resistor in series
        {
            double resistance = timestep/static_cast<capacitor*> (components[i])->get_capacitance(); /*how to calc current?*/ //still don't know how to do this
            voltage *comp = new voltage(components[i]->get_name(), components[i]->get_node1(),components[i]->get_name(), static_cast<capacitor*>(components[i])->get_previous_voltage());
            resistor *comp2 = new resistor(components[i]->get_name(), components[i]->get_name(),components[i]->get_node2(), resistance); //still don't know how to do this
            linear_eq.add_component(comp);
            linear_eq.add_component(comp2);
        }
        return linear_eq;
    }
}
