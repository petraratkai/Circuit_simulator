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
  std::cout<<"time";
  for(int i = 0; i<nodes.size(); i++) {
    std::cout<< ","<<nodes[i].get_name() ;
  }
  for(int i = 0; i<components.size(); i++){
    std::cout<<","<<components[i]->get_name();

  }
  std::cout << std::endl;
  circuit dc;
  make_dc(dc);
  MatrixXd conductance_mx (dc.nodes.size(), dc.nodes.size());
  dc.set_up_matrix(conductance_mx);
  VectorXd vector (dc.nodes.size());
  dc.set_up_vector(0, vector);
  VectorXd solution = conductance_mx.colPivHouseholderQr().solve(vector);
//  set the nodes + voltages in dc, and then in the original circuit
//  std::cout<<conductance_mx;
//  std::cout<<vector;
  dc.set_voltages(solution);
  set_voltages(dc);
  dc.set_currents(0);
  update_circuit(dc, true);
  std::cout<<"0";
  write_out(std::cout);

  //print original out
  //start transient analysis:
  circuit dc2;
  make_linear(dc2);
  MatrixXd conductance_mx2 (dc2.nodes.size(), dc2.nodes.size());
  dc2.set_up_matrix(conductance_mx2);
  VectorXd vector2 (dc2.nodes.size());
  VectorXd solution2 (dc2.nodes.size());

  for(double i = timestep; i<=stoptime; i+=timestep)
  {
    //update dc??
    std::cout<<i;


    refresh_LC();
    dc2.refresh_dc(*this);
     //wrong, have to look into it!!!
    vector2.fill(0);
    dc2.set_up_vector(i,vector2);

    solution2 = conductance_mx2.colPivHouseholderQr().solve(vector2);
    //set voltages and currents in dc, then in the original
    dc2.set_voltages(solution2);
    set_voltages(dc2);
    dc2.set_currents(i);
    update_circuit(dc2, false);
    write_out(std::cout); //write out original




  }
  //circuit c;
  //  std::cout << "Yes";
  //c.read_in(std::cin);
  //  std::cout << "Yes";
    //c.write_out(std::cout);



}

void circuit::set_voltages(VectorXd& voltages)
{
  for(int i=0; i<nodes.size(); i++)
  {
    nodes[i].set_voltage(voltages[i]);
  }

}
void circuit::set_voltages(circuit& dc)
{
  for(int i = 0; i<nodes.size(); i++)
  {
    for(int j = 0; j<dc.nodes.size(); j++)
    {
      if(nodes[i].get_name()==dc.nodes[j].get_name())
        nodes[i].set_voltage(dc.nodes[j].get_voltage());
    }
  }
}

void circuit::refresh_LC()
{
  for(int i = 0; i<components.size(); i++)
  {
    if(components[i]->is_capacitor())
      components[i]->set_previous_voltage(components[i]->get_next_voltage());
    else if(components[i]->is_inductor()){
      components[i]->set_previous_current(components[i]->get_current());
  }
  }

}

void circuit::refresh_dc(circuit& original)
{
  int index1, index2;
  for(int i = 0; i<original.components.size(); i++)
  {
    if(original.components[i]->is_capacitor())
    {
      find_comp_indexes(original.components[i]->get_name(), index1, index2);
      components[index1]->set_dc_offset(original.components[i]->get_previous_voltage());
    }
    else if(original.components[i]->is_inductor())
    {
      find_comp_indexes(original.components[i]->get_name(), index1, index2);
      components[index1]->set_dc_offset(original.components[i]->get_previous_current());
    }

  }
}

void circuit::update_circuit(circuit& dc, bool is_dc)
{
  double v1, v2, i1, i2;
  int index1, index2;

  for(int i = 0; i<components.size(); i++)
  {
    i2 = 0;
    if(components[i]->is_capacitor()) //need to set next voltage
    {
      v1 = nodes[find_node_index(components[i]->get_node1())].get_voltage();
      v2 = nodes[find_node_index(components[i]->get_node2())].get_voltage();
      static_cast<capacitor*>(components[i])->set_next_voltage(v1-v2); //or should it be v2-v1?
      //have to also set current
      dc.find_comp_indexes(components[i]->get_name(), index1, index2);
      components[i]->set_current(dc.components[index1]->get_current());
    }
    else if(components[i]->is_inductor()) //have to set next current
    {
      dc.find_comp_indexes(components[i]->get_name(), index1, index2);
      i1=dc.components[index1]->get_current();
      if(!is_dc) i2 = dc.components[index2]->get_current();
      components[i]->set_current(i1+i2);
    //  std::cerr<<i1 << "+" <<i2<<",";
    }
    else
    {
      find_comp_indexes(components[i]->get_name(), index1, index2);
      components[i]->set_current(dc.components[index1]->get_current());
    }
  }
}

void circuit::find_comp_indexes(const std::string& name, int& index1, int& index2)
{
  bool found_index1 = false;
  bool found_index2 = false;
  for(int i = 0; i<components.size()&&!(found_index1&found_index2); i++)
  {
    if((components[i]->get_name())==name)
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

void circuit::set_currents(double t)
{
  double current;
  for(int i = 0; i<components.size(); i++)
  {
    current = calculate_current(components[i],t, true);
    components[i]->set_current(current);
  }
}

std::string circuit::find_supernode_name(std::string n, bool& supernode_connectedto0)
{
  std::string result ="";
  bool found = false;
  for(int i= 0; i<supernodes.size()&& !found; i++)
  {
    supernode_connectedto0 = false;
    for(int j = 0; j<supernodes[i].size(); j++)
    {
      if(supernodes[i][j]=="0")
        supernode_connectedto0 = true;
      if(supernodes[i][j] == n)
      {
        found = true;
        if(supernodes[i].back()=="0")
          result = supernodes[i][supernodes[i].size()-2];
        else
          result=supernodes[i].back();
      }
    }
  }
  //assert(0);
  return result;
}

void circuit::set_up_matrix(MatrixXd& mx) //this function can only be called on circuits containing only resistors, voltage sources and current sources
{
  int n1, n2, n1new, n2new;
  std::string node1, node2;
  double conductance;
  //bool node1connectedto0;
  //bool node2connectedto0;
  bool supernode1_connectedto0, supernode2_connectedto0;
  reset_nodesDone();
  for(int i = 0; i<components.size(); i++)
  {
    node1 = components[i]->get_node1();
    node2 = components[i]->get_node2();
    n1 = find_node_index(node1);
    n2 = find_node_index(node2); //-1 if node is 0
//std::cout << n1 << " " << n2 << " ";
    n1new = -1;
    n2new = -1;
    supernode1_connectedto0 = false;
    supernode2_connectedto0 = false;
    if(components[i]->is_resistor())
    {
      conductance = components[i]->get_conductance();


      /* find in supernodes, find the name of the last element of that supernode, find the index of that*/

      if(n1!=-1) //node1 is not the reference node
      {
        if (nodes[n1].is_connectedtov())
        {
            // node1 is the reference node

            node1 = find_supernode_name(nodes[n1].get_name(), supernode1_connectedto0);
            if(!supernode1_connectedto0) //node1 is not connected to the reference node via a voltage source
              n1new = find_node_index(node1); //last element of the supernode vector containing node1
            else {n1new=-1; /*node1connectedto0 = true;*/}
            }
          }

      if(n2!=-1)
        {
          if (nodes[n2].is_connectedtov())
          {
            node2=find_supernode_name(nodes[n2].get_name(), supernode2_connectedto0);
            if(!supernode2_connectedto0)
              n2new = find_node_index(node2);
            else {n2new=-1; /*node2connectedto0 = true;*/}
          }
        }
        if(n1!=-1 && n2!=-1)
        {
          if(n1new!=-1)
            n1=n1new;
          if(n2new!=-1)
            n2= n2new;

            if(!supernode1_connectedto0) mx(n1,n2)-=conductance;
            if(n1!=n2) //can this even happen???
            {
             if(!supernode1_connectedto0) mx(n1,n1)+=conductance;
             if(!supernode2_connectedto0) mx(n2,n2)+=conductance;
            if(!supernode2_connectedto0) mx(n2,n1)-=conductance;
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
      int line = find_lineIndex(components[i]->get_node1());
    //  std::cerr<<line;
      if(n1!=-1)
        mx(line,n1)= 1;
      if(n2!=-1)
        mx(line, n2) = -1;
      /*if(n1==-1)
        mx(n2, n2) = -1;
      else if(n2==-1)
        mx(n1,n1) = 1;
      else
      {
        mx(n1,n1)=1;
        mx(n1,n2)=-1;
      }*/

    }
  }
}
void circuit::set_up_vector(double t, VectorXd& vec)
{
  int n1, n2, n1new, n2new;
  std::string node1, node2;
  bool supernode1_connectedto0, supernode2_connectedto0;
  reset_nodesDone();
  for(int i = 0; i<components.size(); i++) {
    if(components[i]->is_current())
    {
        node1 = components[i]->get_node1();
        node2 = components[i]->get_node2();
        n1 = find_node_index(node1);
        n2 = find_node_index(node2);
        supernode1_connectedto0=false;
        supernode2_connectedto0=false;
        n1new = -1;
        n2new =-1;

        //std::cout<<n1 << " " << n2;
        if(n1!=-1)
        {
          if(nodes[n1].is_connectedtov())
            node1 = find_supernode_name(node1, supernode1_connectedto0);
          if(!supernode1_connectedto0)
            n1= find_node_index(node1);

          vec(n1)-=components[i]->get_current(t);
        }
        if(n2!=-1)
        {
          if(nodes[n2].is_connectedtov())
            node2 = find_supernode_name(node2, supernode2_connectedto0);
          if(!supernode2_connectedto0)
            n2= find_node_index(node2);
          vec(n2)+=components[i]->get_current(t);

        }
          //std::cout<<static_cast<current*>(components[i])->get_current(t)<<std::endl;
    }
    else if(components[i]->is_voltage())
    {
      node1 = components[i]->get_node1();
      node2 = components[i]->get_node2();
      n1 = find_node_index(node1);
      n2 = find_node_index(node2);
    /*  if(n2!=-1)
        vec(n2)=+components[i]->get_voltage(t);
      if(n1!=-1)
        vec(n1)=components[i]->get_voltage(t);*/
        int line = find_lineIndex(components[i]->get_node1());
        vec(line) = components[i]->get_voltage(t);
        //std::cerr<<line;

    }

  }
}
