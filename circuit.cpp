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

void circuit::add_component(const component* c)
{
/* c already allocated dynamically

  push back to the vector of components
  add the nodes the component is connected to to the vector of nodes if necessary
*/
  components.push_back(c);
  bool node1 = false; //turns true if we found it in the nodes vector
  bool node2 = false; //turns true if we found it in the nodes vector
  for(int i = 0; i<nodes.size()&& !node1 && !node2; i++) {
    if (nodes[i]==(c->node1))
      node1 = true;
    if(nodes[i]==(c->node2))
      node2 = true;
  }
  if(!node1)
    nodes.push_back(c->node1);
  if(!node2)
    nodes.push_back(c->node2);
  return;
}
