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

void circuit::make_dc(circuit& dc_equiv) //returns the dc version of the circuit, this is for the operating point
{
  //circuit dc_equiv; //this is what we are going to return
  for(int i = 0; i<components.size(); i++)
  {
    if(components[i]->is_inductor()) //have to make inductors short circuits.
    {
      voltage *comp = new voltage;
      *comp = voltage(components[i]->get_name(), components[i]->get_node1(), components[i]->get_node2());
      dc_equiv.add_component(comp);

    }
    else if(components[i]->is_capacitor())
    {
      current *comp = new current;
      *comp = current(components[i]->get_name(), components[i]->get_node1(), components[i]->get_node2());
      dc_equiv.add_component(comp);

    }
    else
    {
      dc_equiv.add_component(components[i]->clone());
    }
  }
}



void circuit::make_linear(circuit &linear_eq)  //returns the circuit, linear components instead of C and L
{
  //  circuit linear_eq; // returning linear_eq which has type circuit
  double resistance;
  for(int i = 0; i<components.size(); i++) {

    if (components[i]->is_inductor()) //replace inductors with current source and resistor in parallel
    {
      resistance = components[i]->get_inductance()/timestep; //resistor must have a value of L/dt
      current *comp = new current(components[i]->get_name(), components[i]->get_node1(),
        components[i]->get_node2(), components[i]->get_previous_current());
        //creating the current, nodes are same as for the original component
      resistor *comp2 = new resistor(components[i]->get_name(),
        components[i]->get_node1(),components[i]->get_node2(), resistance);
        //creating the resistor, nodes are same as for original component
      //adding the resistor and current source into the new circuit:
      linear_eq.add_component(comp);
      linear_eq.add_component(comp2);
    }
    else if(components[i]->is_capacitor())//replace capacitors with voltage source and resistor in series
    {
      double resistance = timestep/components[i]->get_capacitance(); //resistance must be dt/C
      //creating a voltage source and a resistor in series, new node must to be created,
      //new node has the same nam as the component:
      voltage *comp = new voltage(components[i]->get_name(), components[i]->get_node1(),
        components[i]->get_name(), components[i]->get_previous_voltage());
      resistor *comp2 = new resistor(components[i]->get_name(),
        components[i]->get_name(),components[i]->get_node2(), resistance);
      //adding the new components
      linear_eq.add_component(comp);
      linear_eq.add_component(comp2);
    }
    else
        {
          //the other components must be copied into the linear equivalent
          linear_eq.add_component(components[i]->clone());
        }

    }

}
