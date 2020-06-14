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
///c already allocated dinamically
  components.push_back(c); //add the new component to the vector
  bool node1 = false; //turns true if we found it in the nodes vector
  bool node2 = false; //turns true if we found it in the nodes vector
  std::string node1_name = c->get_node1(); //node1 of c
  std::string node2_name = c->get_node2(); //node2 of c
  for(int i = 0; i<nodes.size()&& !(node1 && node2); i++) {
    if (nodes[i].get_name() == node1_name)
      node1 = true; //found node1
    if(nodes[i].get_name() == node2_name)
      node2 = true; //found node2
  }
  bool connected_to_v = false;
  if(c->is_voltage()==true)
    connected_to_v = true; //c is a voltage source
  if(node1_name!="0")
  {
    if(!node1) //if node1 hasn't been stored, it must be stored now
      nodes.push_back(node(node1_name, connected_to_v));
    else if(connected_to_v) //if node1 has been stored, but it is connected to
        //a voltage source, need to set the corresp. param.
      nodes[find_node_index(node1_name)].set_connectedtov(connected_to_v);
  }
  if(node2_name!="0")
  {
    if(!node2) //node2 was not stored before
      nodes.push_back(node(node2_name, connected_to_v)); //need to store node2
    else if(connected_to_v) //if necessary, set connected_to_v of node2 to true
      nodes[find_node_index(node2_name)].set_connectedtov(connected_to_v);
  }
  //store supernode
  if(connected_to_v)
  {
    node1 = false;
    node2 = false;
    std::vector<std::string> supernode;
    int supernode1_index;
    int supernode2_index;
    //need to check if any of the nodes had been added to any of the supernodes already
    //if yes, need to find the index of those supernodes
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
    //if found one of the node, the respective supernode_indexes are set
    if(node1 && !node2) //in this case node2 must be added to the supernode containing node1
      supernodes[supernode1_index].push_back(node2_name);
    else if(!node1 && node2)//in this case node1 must be added to the supernode containing node2
      supernodes[supernode2_index].push_back(node1_name);
    else if(!node1 && !node2) //if none of the nodes were found in the supernodes, a new supernode must be created
    //node1 and node2 must then be added to that new supernode, and the supernode must be added to the vector of supernodes
    {
      supernode.push_back(node1_name);
      supernode.push_back(node2_name);
      supernodes.push_back(supernode);
    }
    else if(supernode1_index!=supernode2_index)//we found both and they are in two different supernodes
    { //then those two supernodes create one big supernodes, so the two supernodes must be concatenated
      supernodes[supernode1_index].insert(supernodes[supernode1_index].end(),
        supernodes[supernode2_index].begin(), supernodes[supernode2_index].end()); //append the two vectors
      supernodes.erase(supernodes.begin()+supernode2_index); //erase the vector that contained node2
    }
    //if the nodes are in the same supernode, that means there is a voltage source or inductor loop: ERROR
    else{ std::cerr<<"voltage source or inductor loop found"; assert(0);}
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

circuit::~circuit()
{
  for(int i=0; i<components.size(); i++) //think about this!! do all data get deleted??
  {
    delete components[i];
  }
}
