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

double circuit::calculate_current(component *comp, double t, bool node1tonode2)
{
  //this function can only be called on circuits that only contain resistors, current sources and voltage sources
  std::string node1_name = comp->get_node1(); //name of node1 of comp
  std::string node2_name = comp->get_node2(); //name of node2 of comp
  int node1_index=find_node_index(node1_name); //find the index of node1 in the nodes vector
  int node2_index = find_node_index(node2_name); //find the index of node2 in the nodes vector

  if(comp->is_resistor()) //base case1: if the component is a resistor
  {
    double v1, v2;
    if(node1_name!="0")
      v1 = nodes[node1_index].get_voltage(); //finding the voltage at node1
    else
      v1 = 0;
    if(node2_name!="0")
      v2 = nodes[node2_index].get_voltage(); //finding the voltage at node2
    else v2 = 0;
    if(node1tonode2) return (v1-v2)*(comp->get_conductance()); //current is (voltage1-voltage2)*conductance when the
                      //positive current is defined to flow from node1 to node2
    return (v2-v1)*comp->get_conductance(); //if positive current is defined to flow from node2 to node1

  }

  if(comp->is_current()) //base case2: component is a current source
  {
    if(node1tonode2)
      return comp->get_current(t); //if + current flows from node1 to node2
    return -comp->get_current(t); //if + current flows from node2 to node1

  }
//  if the component is a voltage:

    /* find all the components connected to node2, get the current through them, return the sum of them*/
    double sum = 0;
    if(node1tonode2)
    {
      for(int i= 0; i< components.size(); i++)
      {
        if(components[i]!=comp &&(components[i]->get_node2() == node2_name ||node2_name==components[i]->get_node1()))
          //all the other components that are connected to node1 of comp:
        {
          if(node2_name == components[i]->get_node1()) //if node1 of components[i] is connected to comp
            sum += calculate_current(components[i], t, true); //need to calculate the current of components[i] from node1 to node2
          if(node2_name == components[i]->get_node2()) //if node2 of components[i] is connected to comp
            sum += calculate_current(components[i], t,false); //need to calculate the current of components[i] from node2 to node1
        }

      }
    }
  else //calcultating current from node2 to node1
  {
    for(int i= 0; i< components.size(); i++)
    {
      if(components[i]!=comp&&(components[i]->get_node2() == node1_name ||node1_name==components[i]->get_node1()))
      { //find all components connected to node1 of comp
        if(node1_name == components[i]->get_node1()) //have to calculate the current from node1 to node2
          sum += calculate_current(components[i],t, true);
        if(node1_name == components[i]->get_node2()) //have to calculate the current from node2 to node1
          sum += calculate_current(components[i],t, false);
      }
    }
  }

return sum;
}
