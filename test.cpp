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

int main()
{

  circuit c; //create circuit
  c.read_in(std::cin); //take in the netlist
  c.analyse();//run transient analysis

}
