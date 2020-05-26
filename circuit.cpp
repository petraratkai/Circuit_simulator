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
//std::cout<<"Add component";
  components.push_back(c);
  bool node1 = false; //turns true if we found it in the nodes vector
  bool node2 = false; //turns true if we found it in the nodes vector
  std::string node1_name = c->get_node1();
  std::string node2_name = c->get_node2();
  for(int i = 0; i<nodes.size()&& !(node1 && node2); i++) {
    if (nodes[i].get_name() == node1_name)
      node1 = true;
    if(nodes[i].get_name() == node2_name)
      node2 = true;
  }
  bool connected_to_v = false;
  if(c->is_voltage()==true)
    connected_to_v = true;
  /*if(!node1 && node1_name!="0")
    nodes.push_back(node(node1_name, connected_to_v));

  if(!node2 && node2_name!="0")
    nodes.push_back(node(node2_name, connected_to_v));*/
  if(node1_name!="0")
  {
    if(!node1)
      nodes.push_back(node(node1_name, connected_to_v));
    else if(connected_to_v)
      nodes[find_node_index(node1_name)].set_connectedtov(connected_to_v);
  }
  if(node2_name!="0")
  {
    if(!node2)
      nodes.push_back(node(node2_name, connected_to_v));
    else if(connected_to_v)
      nodes[find_node_index(node2_name)].set_connectedtov(connected_to_v);
  }
  //nodes[find_node_index(node1_name)].set_connectedtov(connected_to_v);
  //nodes[find_node_index(node2_name)].set_connectedtov(connected_to_v);

  //store supernode
  if(connected_to_v && node1_name!="0" && node2_name!="0")
  {
    node1 = false;
    node2 = false;
    std::vector<std::string> supernode;
    int supernode1_index;
    int supernode2_index;
    for(int i = 0; i<supernodes.size() && !(node1 && node2); i++)
    {
      for(int j = 0; j<(supernodes[i]).size() && !(node1 && node2); j++)
      {
        if(supernodes[i][j] == node1_name)
        {
         node1 = true; //found node1
         supernode1_index = i;
        }
        if(supernodes[i][j] == node2_name)
         {
           node2 = true; //found node1
           supernode2_index = i;
         }
       }
    }

    if(node1 && !node2)
      supernodes[supernode1_index].push_back(node2_name);
    else if(!node1 && node2)
      supernodes[supernode2_index].push_back(node1_name);
    else if(!node1 && !node2)
    {
      supernode.push_back(node1_name);
      supernode.push_back(node2_name);
      supernodes.push_back(supernode);
    }
    else if(supernode1_index!=supernode2_index)//we found both
    {
      supernodes[supernode1_index].insert(supernodes[supernode1_index].end(),
        supernodes[supernode2_index].begin(), supernodes[supernode2_index].end()); //append the two vectors
      supernodes.erase(supernodes.begin()+supernode2_index); //erase the vector that contained node2
    }

    }


  return;
}

int circuit::find_node_index(std::string name)
{
  for(int i=0; i<nodes.size(); i++)
  {
    if(nodes[i].get_name()==name)
      return i;
  }
  return -1;
 //assumes the node is in the vector!!! or it's the reference node
}

circuit::circuit(const circuit& c)
{
  stoptime = c.stoptime;
  timestep = c.timestep;
  for(int i = 0; i<c.components.size(); i++) {
    //component *comp = new component;
    //*comp = *(c.components[i]);
    if(components[i]->is_current())
    {
      current *comp = new current;
      *comp = *(static_cast<current*> (components[i]));
      components.push_back(comp);
    }
    else if(components[i]->is_voltage())
    {
      voltage *comp = new voltage;
      *comp = *(static_cast<voltage*>(components[i]));
      components.push_back(comp);
    }
    else if(components[i]->is_resistor())
    {
      resistor *comp = new resistor;
      *comp = *(static_cast<resistor*>(components[i]));
      components.push_back(comp);
    }
    else if(components[i]->is_inductor())
    {
      inductor *comp = new inductor;
      *comp = *(static_cast<inductor*>(components[i]));
      components.push_back(comp);
    }
    else
    {
      capacitor *comp = new capacitor;
      *comp = *(static_cast<capacitor*>(components[i]));
      components.push_back(comp);
    }
  }
  nodes = c.nodes;
  supernodes = c.supernodes;
}

circuit& circuit::operator=(const circuit& rhs)
{
  if(components!=rhs.components)
  {
    for(int i = 0 ; i<components.size(); i++)
    {
      delete components[i];
    }
    components.clear();
    stoptime = rhs.stoptime;
    timestep = rhs.timestep;
    for(int i = 0; i<rhs.components.size(); i++) {
      if(components[i]->is_current())
      {
        current *comp = new current;
        *comp = *(static_cast<current*> (components[i]));
        components.push_back(comp);
      }
      else if(components[i]->is_voltage())
      {
        voltage *comp = new voltage;
        *comp = *(static_cast<voltage*>(components[i]));
        components.push_back(comp);
      }
      else if(components[i]->is_resistor())
      {
        resistor *comp = new resistor;
        *comp = *(static_cast<resistor*>(components[i]));
        components.push_back(comp);
      }
      else if(components[i]->is_inductor())
      {
        inductor *comp = new inductor;
        *comp = *(static_cast<inductor*>(components[i]));
        components.push_back(comp);
      }
      else
      {
        capacitor *comp = new capacitor;
        *comp = *(static_cast<capacitor*>(components[i]));
        components.push_back(comp);
      }
       //only these are allocated dynamically
    }
    nodes = rhs.nodes;
    supernodes = rhs.supernodes;
  }
  return *this;
}

/*double circuit::get_current(component& comp) //returns the value of the current through comp
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
}*/
circuit::~circuit()
{
  for(int i=0; i<components.size(); i++)
  {
    delete components[i];
  }
}
