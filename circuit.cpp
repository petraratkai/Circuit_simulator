#include <iostream>
#include <string>
#include <vector>

#include "circuit.hpp"
#include "node.hpp"
#include "component.hpp"
#include "capacitor.hpp"
#include "resistor.hpp"
#include "inductor.hpp"
#include "voltage.hpp"
#include "current.hpp"

void circuit::read_in(std::istream& is)
{
  std::string line;
  while(  std::getline(is, line))//reads in a line
  {
    std::vector<std::string> strings;
    std::string s;
    for(int i =0; i<line.size(); i++) { //split the line into strings, store them in a vector of strings
      if(line[i]!==' ' ){
        s.push_back(line[i]);
      } else{
        strings.push_back(s);
        s="";
      }
    }
    if(strings[0][0]=="."){
      assert(strings[0]==".tran" && string.size()==2);
      set_stoptime(strings[1]);
    }
    else
    {
    switch (strings[0][0])
    {
      case 'R':
        resistor* r = new resistor;
      /*  r.set_name(strings[0]);
        r.set_node1(strings[1]);
        r.set_node2(strings[2]);
        r.set_resistance(strings[3]);*/
        *r = resistor(strings[0], strings[1], strings[2], strings[3]);
        add_component(r);

      case 'C':
        capacitor* c = new capacitor;
        c.set_name(strings[0]);
        c.set_node1(strings[1]);
        c.set_node2(strings[2]);
        c.set_capacitance(strings[3]);
        add_component(c);
      case 'L':
        inductor* ind = new inductor;
        ind.set_name(strings[0]);
        ind.set_node1(strings[1]);
        ind.set_node2(strings[2]);
        ind.set_capacitance(strings[3]);
        add_component(ind);
      case 'V':
        voltage* v = new voltage;
        ind.set_name(strings[0]);
        ind.set_node1(strings[1]);
        ind.set_node2(strings[2]);
        ind.set_capacitance(strings[3]);
        add_component(ind);
      case 'I':
}
}
  }
}
