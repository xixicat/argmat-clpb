#ifndef ARGUMENT_HPP
#define ARGUMENT_HPP

#include <iostream>
#include <string>

#include "config/config.hpp"



namespace argumatrix{

using namespace std;

/************************************************************************/
/* This class captures an abstract argument.                            */
/************************************************************************/
struct ArgumentProperty{
	std::string label;   // Each argument has a unique label.
	std::string title;
	std::string description;

	ArgumentProperty()
		:label("no_label"), title(""), description("") 
	{ }

	ArgumentProperty(string _uniq_label, string _title="", string _description="")
		: label(_uniq_label), title(_title), description(_description)
	{ }

	ArgumentProperty(const ArgumentProperty& _ap);
	std::string getTitle() const { return title; }
	void setTitle(std::string val) { title = val; }
};


ArgumentProperty::ArgumentProperty(const ArgumentProperty& _ap)
{
	label = _ap.label;
	title = _ap.title;
	description = _ap.description;
}

} // namespace argumatrix

#endif  //ARGUMENT_HPP
