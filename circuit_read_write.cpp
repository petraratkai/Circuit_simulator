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
  bool end = false;
  //std::string sine = "SINE";

  while(std::getline(is, line) && !end)//reads in a line
  {
    //std::cout<< line;
    std::vector<std::string> strings; //the words (separated by whitespace)the line consists of
    std::string s;
    for(int i =0; i<line.size(); i++) { //split the line into strings, store them in a vector of strings
      if(line[i]!=' '){
        s.push_back(line[i]);
      } else{
        strings.push_back(s); //if we got a whitespace, we got the first parameter, so we can push back
        s.clear(); //set it to empty string
      }

    }
    strings.push_back(s);

    if(strings[0]==".tran"){ //if it starts with a '.', it must be the .tran line
      if(!strings.size()==5) //check if line in correct form
      {
        std::cerr<<"too many or too few parameters for .tran";
      }
      stoptime=convert_string_to_param(strings[2]); //set the stop time
      timestep = convert_string_to_param(strings[4]);
      tran = true; //do we assume there is only one .tran line? is it at the end?
      //std::cout<<"tran";
    }
    else if (strings[0]==".end") {
      end = true;
    }
    else
    {
    if(strings[1][0]=='C' || strings[2][0]=='C')
    {
      std::cerr << "node name starts with C:" <<strings[0];
    }

    switch (strings[0][0])
    {
      case 'R':
      {
        assert(strings.size()==4);
        resistor* r = new resistor;
        *r = resistor(strings[0],strings[1], strings[2], convert_string_to_param(strings[3]));
        add_component(r);
        break;
      }
      case 'C':
      {
        assert(strings.size()==4);
        capacitor* c= new capacitor;
        *c = capacitor(strings[0],strings[1], strings[2], convert_string_to_param(strings[3]));
        add_component(c);

        break;
      }
      case 'L':
      {
        assert(strings.size()==4);
        inductor* ind = new inductor;
        *ind = inductor(strings[0],strings[1], strings[2], convert_string_to_param(strings[3]));
        add_component(ind);
        break;
      }
      case 'V':
      {
        if(strings.size()==4)
        {
          voltage* v = new voltage;
          *v =voltage(strings[0],strings[1], strings[2], convert_string_to_param(strings[3]));
          add_component(v);
        } else if(strings.size()==6 && strings[3].rfind("SINE(", 0)==0)
        {
          voltage* v = new voltage;
          *v = voltage(strings[0],strings[1], strings[2], convert_string_to_param(strings[3].substr(5)),convert_string_to_param(strings[4]), convert_string_to_param(strings[5]));
          add_component(v);
          //have to deal with sine conversion
        }
        //else assert(0); //voltage not valid in the netlist
        break;
      }
      case 'I':
      {
        if(strings.size()==4)
        {
          current* i = new current;
          *i = current(strings[0],strings[1], strings[2], convert_string_to_param(strings[3]));
          add_component(i);
        } else if(strings.size()==6 && strings[3].rfind("SINE(", 0)==0)
        {
          current *i = new current;
          *i = current(strings[0],strings[1], strings[2], convert_string_to_param(strings[3].substr(5)),convert_string_to_param(strings[4]), convert_string_to_param(strings[5]));
          add_component(i);
          //have to deal with sine conversion
        }
        else assert(0); //current not valid in the netlist
        break;
      }
      case '*': //if the line is a comment, it must be ignored
      {break;}
      default:
        {
          std::cerr<< "invalid component type";
          assert(0);
        }
    }
    }
  }
  if(!tran) std::cerr <<"no .tran line in netlist";
  if(!end) std::cerr<<"no .end line in the netlist";

}

void circuit::write_out(std::ostream& os)
{
  //CSV format

  for(int i = 0; i< nodes.size(); i++){ //only if not the additional node of a capacitor!
    os<<","<<nodes[i].get_voltage();
    //os<<nodes[i].get_name();
  //  if(i<(nodes.size()-1))

  }
  for(int i = 0; i<components.size(); i++){//if capacitor or inductor, it will be different!
    os<<","<<components[i]->get_current();

  }
  os<<std::endl;
  return;

}
