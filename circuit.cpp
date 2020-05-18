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

void circuit::add_component(const component& c)
{
  //is the parameter the right type? does the data get sliced off? No, it should be fine
  /*
  check the type, create the appropriate type of component with using new
  push back to the vector of components
  add the nodes the component is connected to to the vector of nodes if necessary
*/
}
