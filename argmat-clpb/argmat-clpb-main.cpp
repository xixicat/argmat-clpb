#include <boost/program_options.hpp>
//#include "boost/program_options/options_description.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include "ClpbProblem.hpp"


using namespace std;
using namespace argumatrix;
using namespace boost;
namespace po = boost::program_options;



int main(int argc, char* argv[])
{
	if(argc==1){		
		printVersionInfo();
		printHelpInfo();
		return 0;
	}
	if(strcmp(argv[1],"--version")==0){
		printVersionInfo();
		return 0;
	}
	if(strcmp(argv[1],"--help")==0){
		printHelpInfo();
		return 0;
	}
	if(strcmp(argv[1],"--formats")==0){
		printFileFormatOptions();
		return 0;
	}
	if(strcmp(argv[1],"--problem")==0){
		printProblemOptions();
		return 0;
	}

	ClpbProblem problem;
	problem.parseParams(argc, argv);

	problem.solve();

	return 0;
}


