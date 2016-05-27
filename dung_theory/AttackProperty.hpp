#ifndef ATTACK_HPP
#define ATTACK_HPP

#include <iostream>
#include <string>

#include "config/config.hpp"
#include "ArgumentProperty.hpp"

namespace argumatrix{

using namespace std;

/************************************************************************/
/* The property of attack relation between two arguments                */
/************************************************************************/
class AttackProperty{ 
private:	
	string label;         // The label of this attack.
	weight_type weight;   // The weight of this attack.
public:
	AttackProperty(string _l="no_label", weight_type _w=0.0) : label(_l), weight(_w) { }

	std::string getLabel() const { return label; }
	void setLabel(std::string val) { label = val; }

	weight_type getWeight() const { return weight; }
	void setWeight(weight_type val) { weight = val; }

}; // end class AttackProperty

} // namespace argumatrix



#endif