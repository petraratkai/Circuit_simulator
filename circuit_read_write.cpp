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

double convert_string_to_param(const std::string& string); //put it elsewhere later

void circuit::read_in(std::istream& is)
{
  std::string line; //current line of the netlist
  bool tran = false; //have we gotten the .tran line?
  //std::string sine = "SINE";
  while(  std::getline(is, line))//reads in a line
  {
    std::vector<std::string> strings; //the words (separated by whitespace)the line consists of
    std::string s;
    for(int i =0; i<line.size(); i++) { //split the line into strings, store them in a vector of strings
      if(line[i]!=' ' ){
        s.push_back(line[i]);
      } else{
        strings.push_back(s); //if we got a whitespace, we got the first parameter, so we can push back
        s=""; //set it to empty string
      }
    }
    if(strings[0][0]=='.'){ //if it starts with a '.', it must be the .tran line
      assert(strings[0]==".tran" && strings.size()==2); //check if line in correct form
      stoptime=convert_string_to_param(strings[1]); //set the stop time
      tran = true; //do we assume there is only one .tran line? is it at the end?
    }
    else
    {
    switch (strings[0][0])
    {
      case 'R':
      {
        assert(strings.size()==4);
        resistor r(strings[0],strings[1], strings[2], convert_string_to_param(strings[3]));
        add_component(r);
        break;
      }
      case 'C':
      {
        assert(strings.size()==4);
        capacitor c(strings[0],strings[1], strings[2], convert_string_to_param(strings[3]));
        add_component(c);
        break;
      }
      case 'L':
      {
        assert(strings.size()==4);
        inductor ind(strings[0],strings[1], strings[2], convert_string_to_param(strings[3]));
        add_component(ind);
        break;
      }
      case 'V':
      {
        if(strings.size()==4)
        {
          voltage v(strings[0],strings[1], strings[2], convert_string_to_param(strings[3]));
          add_component(v);
        } else if(strings.size()==6 && strings[4].rfind("SINE(", 0)==0)
        {
          /*voltage v(strings[0],strings[1], strings[2], convert_string_to_param(strings[3]));
          add_component(v);*/
          //have to deal with sine conversion
        }
        else assert(0); //voltage not valid in the netlist
        break;
      }
      case 'I':
      {
        if(strings.size()==4)
        {
          voltage v(strings[0],strings[1], strings[2], convert_string_to_param(strings[3]));
          add_component(v);
        } else if(strings.size()==6 && strings[4].rfind("SINE(", 0)==0)
        {
          /*voltage v(strings[0],strings[1], strings[2], convert_string_to_param(strings[3]));
          add_component(v);*/
          //have to deal with sine conversion
        }
        else assert(0); //current not valid in the netlist
        break;
      }
      }
    }
  }
  if(tran==false) std::cerr <<"no .tran line in netlist";
}