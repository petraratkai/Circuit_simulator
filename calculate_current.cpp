#include <iostream>
#include <string>
#include <vector>
#include <cassert>
//#include <Eigen/Dense>

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
  std::string node1_name = comp->get_node1();
  std::string node2_name = comp->get_node2();
  int node1_index=find_node_index(node1_name);
  int node2_index = find_node_index(node2_name);

  if(comp->is_resistor())
  {
    double v1, v2;
    v1 = nodes[node1_index].get_voltage();
    v2 = nodes[node2_index].get_voltage();
    return (v1-v2)*static_cast<resistor*>(comp)->get_conductance();

  }
  if(comp->is_current())
  {
    return static_cast<current*>comp->get_current(t);

  }
  if(comp->is_voltage())
  {
    /* find all the components connected to node2, get the current through them, return sum*/
    double sum = 0;
    for(int i= 0; i< components.size(); i++)
    {
      if(components[i]->get_name()!=comp->get_name()&&(components[i]->get_node2() == node2_index ||node2_index==components[i]->get_node1()))
        if(components[i]->is_voltage())
        {
          if(node2_name == components[i]->get_node1())
            return calculate_current(components[i]);
          if()
        }
        else if(node2_name == components[i]->get_node1())
          sum+=calculate_current(components[i],t, true);
        else if(node2_name == components[i]->get_node2())
          sum-=calculate_current(components[i], t, true);
/* deal with resistor with the same name*/

    }

  }
}