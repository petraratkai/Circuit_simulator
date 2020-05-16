/*#include <iostream>
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

class component { //will make this an abstract class
    :public circuit
protected:
    string component_name;
    string node_1;
    string node_2;
    double current;
};

class inductor {
    :public component
private:
    double inductance;
};

class conductor {
    :public component
private:
    double conductance;
};

class resistor {
    :public component
private:
    double resistance;
};

class voltage {
    :public component
private:
    double dc_offset;
    double amplitude;
    double frequency;
};

class current {
    :public component
private:
    double dc_offset;
    double amplitude;
    double frequency;
};
*/
