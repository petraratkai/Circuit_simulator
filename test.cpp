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

  circuit c;
    //std::cout << "Yes";
  //  top row of the output, names of the nodes and components.
    
  c.read_in(std::cin);

  //  std::cout << "Yes";
    //c.write_out(std::cout);
  //MatrixXd mx(3, 3);
  //  std::cout << "Yes";
  //mx.fill(0);
c.analyse();
  /*c.set_up_matrix(mx);
  std::cout<<mx << std::endl;
  VectorXd vec(3);
  vec.fill(0);
  c.set_up_vector(0, vec);
  std::cout<<vec<<std::endl;
  Vector3d x = mx.colPivHouseholderQr().solve(vec);*/
//std::cout << x;
  //c.write_out(std::cout);
}
