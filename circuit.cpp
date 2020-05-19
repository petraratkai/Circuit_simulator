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
  if(!node1)
    nodes.push_back(c->get_node1());
  if(!node2)
    nodes.push_back(c->get_node2());
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

circuit::~circuit()
{
  for(int i=0; i<components.size(); i++)
  {
    delete components[i];
  }
}
