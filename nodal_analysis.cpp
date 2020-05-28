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
  circuit dc = make_dc();
  MatrixXd conductance_mx (dc.nodes.size());
  dc.set_up_matrix(conductance_mx);
  VectorXd vector (dc.nodes.size());
  dc.set_up_vector(0, vector);
  VectorXd solution (dc.nodes.size()) = conductance_mx.colPivHouseholderQr().solve(vector);
  //set the nodes + voltages in dc, and then in the original circuit
  dc.set_voltages(solution);
  set_voltages(solution);

  //print original out
  //start transient analysis:
  dc= make_linear();
  MatrixXd conductance_mx2 (dc.nodes.size());
  dc.set_up_matrix(conductance_mx2);
  VectorXd vector2 (dc.nodes.size());
  VectorXd solution2 (dc.nodes.size());
  for(int i = timestep; i<stoptime; i+=i)
  {
    dc.set_up_vector(i,vector2);
    solution = conductance_mx2.colPivHouseholderQr().solve(vector2);
    //set voltages and currents in dc, then in the original
    //print out original
  }


}

void circuit::set_voltages(VectorXd& voltages)
{
  for(int i=0; i<nodes.size(); i++)
  {
    nodes[i].set_voltage(voltages[i]);
  }

}

void circuit::update_circuit(circuit& dc)
{
  double v1, v2, i1, i2;
  bool r_found,i_found;
  for(int i = 1; i<components.size(); i++)
  { r_found= false;
    i_found = false;
    if(components[i]->is_capacitor()) //need to set next voltage
    {
      v1 = nodes[find_node_index(compononents[i]->get_node1())].get_voltage();
      v2 = nodes[find_node_index(compononents[i]->get_node2())].get_voltage();
      components[i].set_next_voltage(v1-v2); //or should it be v2-v1?
      //have to also set current
    }
    else if(components[i]->is_inductor) //have to set next current
    {

    }
  }
}

void circuit::find_comp_indexes(std::string& name, int& index1, int& index2)
{
  bool found_index1 = false;
  bool found_index2 = false;
  for(int i = 0; i<components.size()&&!(found_index1&found_index2); i++)
  {
    if(components[i]==name)
    {
      if(!found_index1)
      {
        index1 = i;
        found_index1 = true;
      }
      else
      {
        index2 = i;
        found_index2 = true;
      }
    }
  }

}

void circuit::set_currents()
{
  for(int i = 0; i<components.size(); i++)
  {

  }
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
  int n1, n2, n1new, n2new;
  std::string node1, node2;
  double conductance;
  bool node1connectedto0;
  bool node2connectedto0;
  for(int i = 0; i<components.size(); i++)
  {
    node1 = components[i]->get_node1();
    node2 = components[i]->get_node2();
    n1 = find_node_index(node1);
    n2 = find_node_index(node2); //-1 if node is 0
//std::cout << n1 << " " << n2 << " ";
    n1new = -1;
    n2new = -1;
    node1connectedto0 = false;
    node2connectedto0 = false;
    if(components[i]->is_resistor())
    {
      conductance = static_cast<resistor*>(components[i])->get_conductance();


      /* find in supernodes, find the name of the last element of that supernode, find the index of that*/

      if(n1!=-1)
      {
        if (nodes[n1].is_connnectedtov())
        {
            // node1 is the reference node

            node1 = find_supernode_name(nodes[n1].get_name());
            if(node1!="") //node1 is not connected to the reference node via a voltage source
              n1new = find_node_index(node1); //last element of the supernode vector containing node1
            else {n1new=-1; node1connectedto0 = true;}
            }
          }

      if(n2!=-1)
        {
          if (nodes[n2].is_connnectedtov())
          {
            node2=find_supernode_name(nodes[n2].get_name());
            if(node2!="")
              n2new = find_node_index(node2);
            else {n2new=-1; node2connectedto0 = true;}
          }
        }
        if(n1!=-1 && n2!=-1)
        {
          if(n1new!=-1)
            n1=n1new;
          if(n2new!=-1)
            n2= n2new;

            if(!node1connectedto0) mx(n1,n2)-=conductance;
            if(n1!=n2) //can this even happen???
            {
             if(!node1connectedto0) mx(n1,n1)+=conductance;
             if(!node2connectedto0) mx(n2,n2)+=conductance;
            if(!node2connectedto0) mx(n2,n1)-=conductance;
            }
          }
        else if (n1==-1)
        {
          if(n2new!=-1)
            n2=n2new;
          mx(n2,n2)+=conductance;
        }
        else if(n2==-1)
        {
          if(n1new!=-1)
            n1=n1new;
            mx(n1,n1)+=conductance;
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
void circuit::set_up_vector(double t, VectorXd& vec)
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
        //std::cout<<n1 << " " << n2;
        if(n1!=-1)

          vec(n1)-=static_cast<current*>(components[i])->get_current(t);

        if(n2!=-1)
          vec(n2)+=static_cast<current*>(components[i])->get_current(t);
          //std::cout<<static_cast<current*>(components[i])->get_current(t)<<std::endl;
    }
    else if(components[i]->is_voltage())
    {
      node1 = components[i]->get_node1();
      node2 = components[i]->get_node2();
      n1 = find_node_index(node1);
      n2 = find_node_index(node2);
      if(n2!=-1)
        vec(n2)=+static_cast<voltage*>(components[i])->get_voltage(t);
      if(n1!=-1)
        vec(n1)=static_cast<voltage*>(components[i])->get_voltage(t);

    }

  }
}
