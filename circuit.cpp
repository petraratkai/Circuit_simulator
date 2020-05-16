#include <iostream>
#include <string>
#include <vector>

using namespace std;

class circuit {
private:            //protected?
    double timestep;
    double stoptime;
    vector<component> components;
    vector<node> nodes;
public:
    void analyse(double timestep, double stoptime);
    void add_component(component& c);
};

class component {
    :public circuit
protected:
    string component_name;
    string node_1;
    string node_2;
    double current;
};

class node {
    :public circuit
protected:
    string node_name;
    double voltage;
public:
    double get_voltage();
    double get_name();
    void set_voltage(double volts);
};


