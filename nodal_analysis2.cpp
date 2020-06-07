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

int circuit::find_lineIndex(const std::string& node_name)
{
  bool found = false;
  int i, j;
  for(i = 0; i<supernodes.size()&&!found; i++)
  {
    for(j = 0; j<supernodes[i].size()&&!found; j++)
    {
      if(supernodes[i][j]==node_name)
        found = true;

    }
  }
  //need to find the first element in supernodes[i] that hasn't been is_done
//  bool found_NotDone = false;
  int index;
  for(int k = 0; k<supernodes[i-1].size(); k++)
  { //index = 1;
    //std::cout<<supernodes[i-1][k];
    index = find_node_index(supernodes[i-1][k]);
    if(index!=-1)
    {
      if(!nodes[index].is_done())
      {
        nodes[index].set_done(true);
        return index;
      }
    }
  }
  return -1;
}

void circuit::reset_nodesDone()
{
  for(int i = 0; i<nodes.size(); i++)
  {
    nodes[i].set_done(false);
  }
}
