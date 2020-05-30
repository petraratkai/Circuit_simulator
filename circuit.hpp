#ifndef CIRCUIT_HPP
#define CIRCUIT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Dense>

#include "component.hpp"
#include "node.hpp"

//enum class component_type {R, I, V, C, L};

using namespace Eigen;

class circuit {
private:            //protected?
    double timestep;
    double stoptime;
    std::vector<component*> components; //vector of all the components in the circuit, heterogeneous collection
    std::vector<node> nodes; //vector of nodes in the circuit
  //  void set_stoptime(const std::string& t); //converts t to double, and sets stoptime to that value
    std::vector <std::vector<std::string>> supernodes; //name of the nodes that are connected via a voltage source
public:

    circuit(){} //why do I need this???

    void analyse(); //creates the matrix, runs the simulation several times, prints the circuit at each time step?

    void set_up_matrix(MatrixXd& mx); //mx is the conductance matrix, vec is the current/voltage vector, t is time

    void set_up_vector(double t, VectorXd& vec);
    std::string find_supernode_name(std::string n, bool& supernode_connectedto0);

    void add_component(component* c); //not sure if needed

    void read_in(std::istream& is); //parses in the circuit from the netlist

    void write_out(std::ostream& os);

    int find_node_index(std::string name);

    //double get_current(component& comp);

    circuit& operator=(const circuit& rhs);

    circuit(const circuit& c);

    circuit make_dc(); //returns the dc version of the circuit, this is for the operating point

    circuit make_linear(); //returns the circuit, linear components instead of C and L

    void set_voltages(VectorXd& voltages); //sets the voltages of the nodes to the values of voltages

    void set_currents(double t); //only on circuits containing R, V and I

    void update_circuit(circuit& dc, bool is_dc);

    void find_comp_indexes(const std::string& name, int& index1, int& index2);

    double calculate_current(component *comp, double t, bool node1tonode2);

    void refresh_LC();

    void refresh_dc(circuit& original);

    ~circuit();

};

#endif
