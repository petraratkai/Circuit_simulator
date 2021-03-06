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
  //header of the output file:
  std::cout<<"time";
  for(int i = 0; i<nodes.size(); i++) {
    std::cout<< ","<<nodes[i].get_name() ; //writing out the name of each node
  }
  for(int i = 0; i<components.size(); i++){
    std::cout<<","<<components[i]->get_name(); //writing out the name of each component
  }
  //finding the operating point
  std::cout << std::endl;
  circuit dc; //dc equivalent of the circuit
  make_dc(dc); //copies components into dc, but capacitors are open circuit, inductors short circuit
  MatrixXd conductance_mx (dc.nodes.size(), dc.nodes.size()); //conductance matrix for finding the operating point
  dc.set_up_matrix(conductance_mx); //setting up the matrix for nodal analysis
  VectorXd vector (dc.nodes.size()); //vector of currents + voltages
  dc.set_up_vector(0, vector);//setting up the vector for the dc analysis
  VectorXd solution = conductance_mx.colPivHouseholderQr().solve(vector); //solving the matrix for the vector of nodes
//  set the nodes + voltages in dc, and then in the original circuit
  dc.set_voltages(solution); //copies the voltages from the solution to the nodes vector of dc
  set_voltages(dc); //assigns the voltages from dc to the appropriate nodes in the original circuit
  dc.set_currents(0); //calculates the currents in dc
  update_circuit(dc, true); //copies currents from dc, calculates them for C and L
  std::cout<<"0"; //time column of the first line
  write_out(std::cout); //write out the circuit at t=0
  //start transient analysis:
  circuit dc2; //linear equivalent of the original circuit
  make_linear(dc2); //replaces capacitors and inductors with their linear companion
  MatrixXd conductance_mx2 (dc2.nodes.size(), dc2.nodes.size());
  conductance_mx2.fill(0); //initialize the matrix
  dc2.set_up_matrix(conductance_mx2); //conductance matrix for the transient analysis
  VectorXd vector2 (dc2.nodes.size());  //vector of currents and voltages for transient analysis
  VectorXd solution2 (dc2.nodes.size()); //solution vector

  for(double i = timestep; i<=stoptime; i+=timestep)
  {
    std::cout<<i; //first column of the csv
    dc2.refresh_dc(*this); //refreshes the values of the voltage and current sources corresponding to capacitors and inductors
    vector2.fill(0);//reset the vector
    dc2.set_up_vector(i,vector2);
    solution2 = conductance_mx2.colPivHouseholderQr().solve(vector2);
    //set voltages and currents in dc, then in the original
    dc2.set_voltages(solution2);
    set_voltages(dc2);
    dc2.set_currents(i); //calculate currents in the linear circuit
    //dc2.write_out(std::cerr);
    update_circuit(dc2, false); //update the original
    write_out(std::cout); //write out original
  }
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

/*void circuit::refresh_LC()
{
  for(int i = 0; i<components.size(); i++)
  {
    if(components[i]->is_capacitor())
      components[i]->set_previous_voltage(components[i]->get_next_voltage());
    else if(components[i]->is_inductor()){
      components[i]->set_previous_current(components[i]->get_current());
  }
  }

}*/

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
      v1 = 0; v2 = 0;
      std::string node1, node2;
      node1 = components[i]->get_node1();
      node2 = components[i]->get_node2();
      if(node1!="0")
        v1 = dc.nodes[dc.find_node_index(node1)].get_voltage();
      if(node2!="0")
        v2 = dc.nodes[dc.find_node_index(node2)].get_voltage();
      components[i]->set_next_voltage(v1-v2); //or should it be v2-v1?
      //have to also set current
      dc.find_comp_indexes(components[i]->get_name(), index1, index2);
      components[i]->set_current(dc.components[index1]->get_current());
      components[i]->set_previous_voltage(components[i]->get_next_voltage());

    }
    else if(components[i]->is_inductor()) //have to set next current
    {
      dc.find_comp_indexes(components[i]->get_name(), index1, index2);
      i1=dc.components[index1]->get_current();
      if(!is_dc) i2 = dc.components[index2]->get_current();
      components[i]->set_current(i1+i2);
      components[i]->set_previous_current(components[i]->get_current());
      //std::cerr<<i1 << "+" <<i2<<","<<std::endl;
    }
    else
    {
      dc.find_comp_indexes(components[i]->get_name(), index1, index2);
      components[i]->set_current(dc.components[index1]->get_current());
    }
  }
}

void circuit::find_comp_indexes(const std::string& name, int& index1, int& index2)
{
  bool found_index1 = false;
  bool found_index2 = false;
  for(int i = 0; i<components.size()&&!(found_index1&&found_index2); i++)
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
  { //std::cerr<<std::endl;
    supernode_connectedto0 = false;
    for(int j = 0; j<supernodes[i].size(); j++)
    { //std::cerr<<supernodes[i][j]<< " ";
      if(supernodes[i][j]=="0")
        supernode_connectedto0 = true;
      else if(supernodes[i][j] == n)
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
  int n1, n2, n1new, n2new; //n1 is the index of node1, n2 is the index of node2,
            // n1new is a valid index if node1 is in a supernode, n2new is valid if node2 is in a supernode
  std::string node1, node2;
  double conductance;
  bool supernode1_connectedto0, supernode2_connectedto0;
  reset_nodesDone(); //setting the done parameter of the nodes to false

  for(int i = 0; i<components.size(); i++) //going through the vector of components
  {
    node1 = components[i]->get_node1(); //node1 of the current component
    node2 = components[i]->get_node2(); //node2 of the current component
    n1 = find_node_index(node1); //n1 is the index of node1 in the nodes vector
    n2 = find_node_index(node2); //n2 is the index of node2 in the nodes vector
    //n1 is -1 if node1 is the reference node, n2 is -1 if node2 is the reference node

    n1new = -1; //these are needed for the supernodes
    n2new = -1;
    supernode1_connectedto0 = false;
    supernode2_connectedto0 = false;
    if(components[i]->is_resistor()) //if the components is a resistor, its conductance has to be added to
            //the proper places of the matrix
    {
      conductance = components[i]->get_conductance();
      /* find in supernodes, find the name of the last element of that supernode, find the index of that*/
      if(n1!=-1) //node1 is not the reference node
      {
        if (nodes[n1].is_connectedtov())
        {
            //if node1 is connected to a voltage source, node1 belongs to a supernode, so
            // a new index needs to be found
            //new index is the index of the last node in the supernode vector
            node1 = find_supernode_name(node1, supernode1_connectedto0); //node1 is the name of the
                                                                                        //last node in the supernode
            //supernode1_connectedto0 will e true if the supernode contains the reference node
            if(!supernode1_connectedto0) //node1 is not connected to the reference node via voltage sources
              n1new = find_node_index(node1); //last element of the supernode vector containing node1
            else {n1new=-1;} //otherwise make sure n1new is invalid
        }
      }
      //doing the same for node2:
      if(n2!=-1) //node2 is not the reference node
        {
          if (nodes[n2].is_connectedtov()) //node2 is in a supernode
          {
            node2=find_supernode_name(node2, supernode2_connectedto0); //node2 is the name of last
                                                                                      //node of the supernode
            //supernode2_connectedto0 will be true if the supernode contains the reference node
            if(!supernode2_connectedto0) //if the supernode does not contain the reference node,new index is needed
              n2new = find_node_index(node2);//new index is the index of last element in the supernode
            else n2new=-1; //otherwise make sure n2new invalid
          }
        }
        if(n1!=-1 && n2!=-1)
        {
          if(!supernode1_connectedto0) //supernode is not connected to 0, so row with KCL is needed for node1
          {
            if(n1new!=-1)
            { //if new index was needed, adding and subtracting from the new row
              mx(n1new,n2)-=conductance;
              mx(n1new,n1)+=conductance;
            }
            else
            { //if new index was not needed, adding and subtracting from the original row
              mx(n1,n2)-=conductance;
              mx(n1,n1)+=conductance;
            }
          }
          if(!supernode2_connectedto0) //supernode is not connected to 0, so row with Kcl is needed for node2
          {
            if(n2new!=-1)
            { //if new index was needed, adding and subtracting from the new row
              mx(n2new,n2)+=conductance;
              mx(n2new,n1)-=conductance;
            }
            else
            { //if new index was not needed, adding and subtracting from the original row
              mx(n2,n2)+=conductance;
              mx(n2,n1)-=conductance;
            }
          }
        }
        else if (n1==-1) //node 1 is the reference node
        {
          if(!supernode2_connectedto0) //supernode not connected to reference node, KCL needed
          {
            if(n2new!=-1) //if new row needed
              mx(n2new,n2)+=conductance; //add to new row
              else
              mx(n2,n2)+=conductance; //add to original otherwise
          }
        }
        else if(n2==-1) //node2 is the reference node
        {
          if(!supernode1_connectedto0) //supernode not connected to reference node, KCL needed
          {
          if(n1new!=-1) //if new row index needed
            mx(n1new,n1)+=conductance; //add to new row
          else
           mx(n1, n1)+=conductance; //add to original otherwise
        }
      }
    }
    else if(components[i]->is_voltage()) //if the component is a voltage
    {
      int line = find_lineIndex(components[i]->get_node1()); //find the next index that can be
          //used for expressing the voltage difference
      if(n1!=-1) //if node1 is not the reference node
        mx(line,n1)= 1;
      if(n2!=-1) //of node2 is not the reference node
        mx(line, n2) = -1;
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
        if(n1!=-1) //node1 is not the reference node
        {
          if(nodes[n1].is_connectedtov())
          {
            node1 = find_supernode_name(node1, supernode1_connectedto0);
            if(!supernode1_connectedto0)
            {
              n1= find_node_index(node1);
              vec(n1)-=components[i]->get_current(t);
            }
          }
          else
            vec(n1)-=components[i]->get_current(t);
        }
          if(n2!=-1) //node2 is not the reference node
          {
            if(nodes[n2].is_connectedtov())
            {
              node2 = find_supernode_name(node2, supernode2_connectedto0);
              if(!supernode2_connectedto0)
              {
                n2= find_node_index(node2);
                vec(n2)+=components[i]->get_current(t);
              }

            } else   vec(n2)+=components[i]->get_current(t);
          }
    }
    else if(components[i]->is_voltage())
    {
        int line = find_lineIndex(components[i]->get_node1());
        vec(line) = components[i]->get_voltage(t);
    }

  }
}
