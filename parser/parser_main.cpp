#include <iostream>
#include <fstream>
#include <string.h>
#include <iostream> 

#include "bitmatrix/bitmatrix.hpp"
#include "dung_theory/ArgumentProperty.hpp"
#include "dung_theory/AttackProperty.hpp"
#include "dung_theory/DungAF.hpp"
#include "dung_theory/Reasoner.hpp"
#include "dung_theory/GroundedReasoner.hpp"
#include "dung_theory/PreferredReasoner.hpp"
#include "config/cptimer.hpp"
#include "parser.hpp"


using namespace std;
using namespace boost;
using namespace argumatrix;


int main(int argc, char* argv[])
{
	uint64_t BegainTime ;     
	uint64_t EndTime ;     

	
	//std::string inputFile("../datasets/test_cases2/apx/145703__300__11_12_17__95.apx");
	//std::string inputFile("../datesets/barabasi/60/Bar_n60a20_91.dl");
	//std::string inputFile("../datasets/MyExamples/hunter.dl");
	std::string inputFile("../datasets/kleinberg/9/Kle_n3e0.9_01.dl");
	//std::string inputFile("../datasets/iccma15_testcases/tgf/rdm20_30.tgf");
	//std::string inputFile("../datesets/barabasi/30/Bar_n30a20_100.dl");
	//argumatrix::parser::TrivialGraph2DungAF(inputFile, daf);

	
	BegainTime = clock_ns(); 

	for(int i=0; i<1000; i++)
	{
		DungAF daf;
		argumatrix::parser::Aspartix2DungAF(inputFile, daf);
	}

	EndTime = clock_ns();  

	// cout << endl << "DAF: " << daf.toString() << endl;cout << endl << "DAF: " << daf.toString() << endl;

	cout << "Running Time(ms): " <<(double)( EndTime - BegainTime )/1000 <<endl;    

	system("PAUSE");

	return 0;
}


