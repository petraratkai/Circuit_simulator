#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <Eigen/Dense>

#include "circuit.hpp"
#include "node.hpp"
#include "component.hpp"
#include "capacitor.hpp"
#include "resistor.hpp"
#include "inductor.hpp"
#include "voltage.hpp"
#include "current.hpp"

using namespace Eigen;
void circuit::analyse()
{
  /*
  create dc equivalent for operating point
  call set_up_matrix()
  solve the matrix
  print out
  create equivalent for transient simulation
  set up the new cond. matrix,
  for loop: change the current source values and voltage source values, solve the matrix again
  each time, calculate the current through each component in the replacement circuits
  from the equivalent, find the currents and voltages in the original circuit
  */
}

std::string circuit::find_supernode_name(std::string n)
{
  for(int i= 0; i<supernodes.size(); i++)
  {
    for(int j = 0; j<supernodes[i].size(); j++)
    {
      if(supernodes[i][j] == n)
      {
        return supernodes[i].back();
      }
    }
  }
  //assert(0);
  return "";
}

void circuit::set_up_matrix(MatrixXd& mx) //this function can only be called on circuits containing only resistors, voltage sources and current sources
{
  int n1, n2;
  std::string node1, node2;
  double capacitance;
  for(int i = 0; i<components.size(); i++)
  {
    node1 = components[i]->get_node1();
    node2 = components[i]->get_node2();
    n1 = find_node_index(node1);
    n2 = find_node_index(node2); //-1 if node is 0

    if(components[i]->is_resistor())
    {
      capacitance = static_cast<resistor*>(components[i])->get_conductance();


      /* find in supernodes, find the name of the last element of that supernode, find the index of that*/
      if(n1==-1) //node1 is the reference node
        mx(n2,n2)+=capacitance;
      else if(n2==-1) //node2 is the reference node
        mx(n1,n1)+=capacitance;
      else
      {
        if (nodes[n1].is_connnectedtov())
        {
          n1 = find_node_index(find_supernode_name(nodes[n1].get_name()));
        }
        else if (nodes[n2].is_connnectedtov())
        {
          n2 = find_node_index(find_supernode_name(nodes[n2].get_name()));
        }

        mx(n1,n2)+=capacitance;
        if(n1!=n2) //can this even happen???
        {
          mx(n1,n1)+=capacitance;
          mx(n2,n2)+=capacitance;
          mx(n2,n1)+=capacitance;
        }
      }
    }
    else if(components[i]->is_voltage())
    {
      if(n1==-1)
        mx(n2, n2) = 1;
      else if(n2==-1)
        mx(n1,n1) = 1;
      else
      {
        mx(n1,n1)=1;
        mx(n1,n2)=-1;
      }
    }
  }
}
void circuit::set_up_vector(double t, VectorXd vec)
{
  int n1, n2;
  std::string node1, node2;
  for(int i = 0; i<components.size(); i++) {
    if(components[i]->is_current())
    {
        node1 = components[i]->get_node1();
        node2 = components[i]->get_node2();
        n1 = find_node_index(node1);
        n2 = find_node_index(node2);
        std::cout<<n1 << " " << n2;
        if(n1!=-1)
          vec(n1)-=static_cast<current*>(components[i])->get_current(t);
        if(n2!=-1)
          vec(n2)+=static_cast<current*>(components[i])->get_current(t);
    }
    else if(components[i]->is_voltage())
    {
      if(n2!=-1)
        vec(n2)=-static_cast<voltage*>(components[i])->get_voltage(t);
      if(n1!=-1)
        vec(n1)=static_cast<voltage*>(components[i])->get_voltage(t);

    }
  }
}
