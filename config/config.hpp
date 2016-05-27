#ifndef ARGUMATRIX_CONFIG_HPP
#define ARGUMATRIX_CONFIG_HPP

#include <iostream>
#include <fstream>
#include <string>

// For x86 system, sizeof(unsigned long long)=4. This means that each block can accommodate 32 bits;
// For x64 system, sizeof(unsigned long long)=8. Each block can accommodate 64 bits.
typedef unsigned long long block_type;  

// Using to define the type of the number of bits or arguments.
typedef std::size_t size_type;

// The data type of weight
typedef float weight_type;

// left limiter "["
const std::string LEFT_LIMITER = "[";

// right limiter "]"
const std::string RIGHT_LIMITER = "]";

// Delimiter ","
const std::string DELIMITER = ",";

// Indent "\t" = two spaces
const std::string INDENT = "  ";

// YES or TURE
// const std::string STR_YES = "YES";
const std::string STR_YES = "YES";

// NO or FALSE 
// const std::string STR_NO = "NO";
const std::string STR_NO = "NO";

namespace argumatrix{
using namespace std;

string trim(string& s)
{
	size_type init = s.find_first_not_of(" ");
	size_type end = s.find_last_not_of(" ");

	if (init == string::npos)
		init = 0;

	if (end == string::npos)
		end = s.size();

	return s.substr(init, end - init + 1);
}

} // end namespace argumatrix

#endif // ARGUMATRIX_CONFIG_HPP
