//=======================================================================
// Copyright 2015 Tsinghua University.
// Authors: Fuan Pu (Pu.Fuan@gmail.com)
// 
// Dung's abstract argumentation framework
//=======================================================================

#ifndef ARGUMATRIX_PARSER_HPP
#define ARGUMATRIX_PARSER_HPP

#include <iostream>
#include <string>
#include <utility>   
#include <algorithm>   
// #include <regex>
#include <boost/regex.hpp>

#include "config/config.hpp"
#include "dung_theory/ArgumentProperty.hpp"
#include "dung_theory/AttackProperty.hpp"
#include "dung_theory/DungAF.hpp"

namespace argumatrix{

using namespace std;

class parser
{
public:
	/**
	 * @brief Read Aspartix format from a file to DungAF. We have implemented
	 * three approaches  to read aspartix format. stl::regex, boost::regex and
	 * the approach from ArgSemSAT.1.0rc3. By testing, we choose boost::regex
	 * since it is more stable than stl::regex, and slightly faster than ArgSemSAT.
	 * @param std::string filePath -- the file path of the Aspartix format file.
	 * @param DungAF& daf -- the output abstract argumentation framework
	 * @return bool. If the translation is successful return true, else return false.
	 */
	static bool Aspartix2DungAF(const std::string& filePath, DungAF& daf);
	
	/**
	 * Read Trivial Graph Format from a file to DungAF
	 * @param std::string filePath -- the file path of the Aspartix format file.
	 * @param DungAF& daf -- the output abstract argumentation framework
	 * @return bool. If the translation is successful return true, else return false.
	 */
	static bool TrivialGraph2DungAF(const std::string& filePath, DungAF& daf);
};


//bool parser::Aspartix2DungAF(const std::string& fileName, DungAF& daf)
//{
//	bool _is_successful = true;
//
//	// The argument and attack regex expressions of Aspartix format
//	// arg(a1).
//	// att(a1, a2).
//
//	// Before parsing, we need to clear the daf.
//	daf.clear();
//
//	std::ifstream _infile;
//	_infile.open(fileName, ios_base::in);
//
//	//check the file
//	if(!_infile)
//	{
//		cerr << "Can not open the file: " << fileName << endl;
//		exit(1);
//	}
//
//	size_type start;  
//	size_type _line_num = 0;
//	std::string _line_str;
//
//	while ( getline(_infile, _line_str) )
//	{	
//		// add attacks
//		if ( (start =_line_str.find("att(") != string::npos) ) 
//		{
//			start += 3;
//
//			size_type comma = _line_str.find(",", start);
//			string arg_src = trim(_line_str.substr(start, comma - start));
//			
//			++comma;
//			size_type end_ = _line_str.find(")", comma);
//			string arg_tag = trim(_line_str.substr(comma, end_ - comma));
//
//			daf.addAttack(arg_src, arg_tag);
//			//cout << arg_src << "->" << arg_tag << endl;
//		} 
//		else if ((start =_line_str.find("arg(") != string::npos))
//		{
//			start += 3;
//			size_type end_ = _line_str.find(")", start);
//			string arg1 = trim(_line_str.substr(start, end_ - start));
//
//			daf.addArgument(arg1);
//			//cout << arg1 << endl;
//		}
//	}
//
//	_infile.close();
//
//	return _is_successful;
//} // Aspartix2DungAF(const std::string& fileName, DungAF& daf)



//// The std::regex may result regex_error on linux systems. Thus, we using 
//// boost::regex to substitute.
bool parser::Aspartix2DungAF(const std::string& fileName, DungAF& daf)
{
	bool _is_successful = true;

	// The argument and attack regex expressions of Aspartix format
	// arg(a1).
	// att(a1, a2).
	const boost::regex rgx_arg("arg\\(\\s*([a-zA-Z0-9_]+)\\s*\\)");
	const boost::regex rgx_att("att\\(\\s*([a-zA-Z0-9_]+)\\s*,\\s*([a-zA-Z0-9_]+)\\s*\\)");

	// Before parsing, we need to clear the daf.
	daf.clear();

	std::ifstream _infile;
	_infile.open(fileName, ios_base::in);

	//check the file
	if(!_infile)
	{
		cerr << "Can not open the file: " << fileName << endl;
		exit(1);
	}

	std::string _line_str;
	size_type   _line_num = 0;
	boost::cmatch _result;

	while ( getline(_infile, _line_str) )
	{
		//_infile.getline()
		//_infile >> _line_str;
		++_line_num;

		if(_line_str.length()<=4)
			continue;

		if (boost::regex_search(_line_str.c_str(), _result, rgx_att))  // Find Attack
		{
			daf.addAttack(_result[1], _result[2]);
			//cout << "Adding Attack: (" << _result[1] << "," << _result[2] << ")" << endl;
		}
		else if (boost::regex_search(_line_str.c_str(), _result, rgx_arg)) // Find Argument
		{
			daf.addArgument( _result[1] );
			//cout << "Adding Argument: " << _result[1] << endl;
		}
		else
		{
			std::cerr << "Ignorance: " << fileName << endl
				<< "     Line number(" << _line_num << "): " << _line_str << endl;
			_is_successful = false;
		}
	}

	_infile.close();

	return _is_successful;
} // Aspartix2DungAF(const std::string& fileName, DungAF& daf)


/////////////////////
// Implemented by stl::regex
///////////////////// 
//bool parser::Aspartix2DungAF(const std::string& fileName, DungAF& daf)
//{
//	bool _is_successful = true;
//
//	// The argument and attack regex expressions of Aspartix format
//	// arg(a1).
//	// att(a1, a2).
//	const std::regex rgx_arg("arg\\(\\s*([a-zA-Z0-9]+)\\s*\\)");
//	const std::regex rgx_att("att\\(\\s*([a-zA-Z0-9]+)\\s*,\\s*([a-zA-Z0-9]+)\\s*\\)");
//
//	// Before parsing, we need to clear the daf.
//	daf.clear();
//
//	std::ifstream _infile;
//	_infile.open(fileName);
//
//	//check the file
//	if(!_infile)
//	{
//		cerr << "Can not open the file: " << fileName << endl;
//		exit(1);
//	}
//
//	std::string _line_str;
//	size_type   _line_num = 0;
//	std::match_results<std::string::const_iterator> _result;
//
//	while ( getline(_infile, _line_str) )
//	{
//		//_infile.getline()
//		//_infile >> _line_str;
//		++_line_num;
//
//		if(_line_str.length()<=4)
//			continue;
//
//		if (std::regex_search(_line_str, _result, rgx_att))  // Find Attack
//		{
//			daf.addAttack(_result[1], _result[2]);
//			//cout << "Adding Attack: (" << _result[1] << "," << _result[2] << ")" << endl;
//		}
//		else if (std::regex_search(_line_str, _result, rgx_arg)) // Find Argument
//		{
//			daf.addArgument( _result[1] );
//			//cout << "Adding Argument: " << _result[1] << endl;
//		}
//		else
//		{
//			std::cout << "Ignorance: " << fileName << endl
//				      << "     Line number(" << _line_num << "): " << _line_str << endl;
//			_is_successful = false;
//		}
//	}
//
//	_infile.close();
//
//	return _is_successful;
//} // Aspartix2DungAF(const std::string& fileName, DungAF& daf)


bool parser::TrivialGraph2DungAF(const std::string& fileName, DungAF& daf)
{
	bool _is_successful=true;
	string arg1,arg2;

	// Before parsing, we need to clear the daf.
	daf.clear();

	std::ifstream _infile;
	_infile.open(fileName, ios_base::in);

	//check the file
	if(!_infile)
	{
		cerr << "Can not open the file: " << fileName << endl;
		exit(1);
	}

	// read arguments
	while(_infile>>arg1)
	{	
		if(arg1=="#")
		{
			break;
		}
		else
		{
			daf.addArgument(arg1);
			// cout << arg1 << endl;
		}
	}

	// read attackes
	while(_infile>>arg1>>arg2)
	{  
		daf.addAttack(arg1,arg2);	
		// cout << arg1 << " -> " << arg2 << endl;
	}

	_infile.close();

	return _is_successful;
} // TrivialGraph2DungAF(const std::string& fileName, DungAF& daf)

} // namespace argumatrix



#endif  //ARGUMATRIX_PARSER_HPP