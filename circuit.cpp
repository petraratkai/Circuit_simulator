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

void circuit::add_component(component* c)
{
/* c already allocated dynamically

  push back to the vector of components
  add the nodes the component is connected to to the vector of nodes if necessary
*/
  components.push_back(c);
  bool node1 = false; //turns true if we found it in the nodes vector
  bool node2 = false; //turns true if we found it in the nodes vector
  for(int i = 0; i<nodes.size()&& !node1 && !node2; i++) {
    if (nodes[i].get_name() == c->get_node1() && c->get_node1()!="0")
      node1 = true;
    if(nodes[i].get_name()==c->get_node2()&& c->get_node2()!="0")
      node2 = true;
  }
  bool connected_to_v = false;
  if(c->is_voltage()==true) connected_to_v = true;
  if(!node1)
    nodes.push_back(node(c->get_node1(), connected_to_v));
  if(!node2)
    nodes.push_back(node(c->get_node2(), connected_to_v));
  return;
}

int circuit::find_node_index(std::string name)
{
  for(int i=0; i<nodes.size(); i++)
  {
    if(nodes[i].get_name()==name)
      return i;
  }
  return 0;
 //assumes the node is in the vector!!!
}

double circuit::get_current(component& comp) //returns the value of the current through comp
{
  int node1_index = find_node_index(comp.get_node1()); //node1 of comp
  int node2_index = find_node_index(comp.get_node2()); //node2 of comp
  if(comp.is_resistor())
    return (nodes[node1_index].get_voltage()-nodes[node2_index].get_voltage())*comp.get_conductance();
    //(v1-v2)/R
//  if(comp.is_voltage())
        //find the sum of currents entering node1(or node2?) of the component

    //if(comp.is_current()) return comp.get_current();
    //if(comp.is_capacitor()) find the current through the corresponding resistor
    //if(comp.is_inductor()) find the sum of the currents flowing through the corr. resistor and current source
}
circuit::~circuit()
{
  for(int i=0; i<components.size(); i++)
  {
    delete components[i];
  }
}
