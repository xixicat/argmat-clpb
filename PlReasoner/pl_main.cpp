#include <iostream>
#include <fstream>

#include "bitmatrix/bitvector.hpp"
#include "bitmatrix/bitmatrix.hpp"
#include "dung_theory/ArgumentProperty.hpp"
#include "dung_theory/AttackProperty.hpp"
#include "dung_theory/DungAF.hpp"
#include "parser/parser.hpp"
#include "config/cptimer.hpp"

// SWI-prolog
//#include <SWI-cpp.h>
//#include "swipl-util.hpp"
#include "PlReasoner.hpp"
#include "CompletePlReasoner.hpp"
#include "StablePlReasoner.hpp"
#include "AdmissiblePlReasoner.hpp"
#include "ConflictfreePlReasoner.hpp"
#include "dung_theory/GroundedReasoner.hpp"


using namespace std;
using namespace boost;
using namespace argumatrix;


void printExts(DungAF& daf)
{
	CompletePlReasoner cplr(daf);

	//ofstream ofile("output.txt");
	//cplr.setOutput(ofile.rdbuf());

	//cplr.printAllExts2("conflict_free");
	//cplr.printAllExts2("stable1");
	//cplr.printAllExts2("admissible1");
	//cplr.printAllExts2("complete2");
	//cout << "Grounded: " << endl;
	//cplr.printGroundedExt();
	//cout << endl;

	cplr.Test_time();

	//cplr.setOutput();

	//ofile.close();
}

void demo1()
{
	DungAF daf1;
	daf1.addArgument("a");
	daf1.addArgument("b");
	daf1.addArgument("c");
	daf1.addArgument("d");

	daf1.addAttack("a","b");
	daf1.addAttack("b","a");
	daf1.addAttack("b","c");
	daf1.addAttack("c","d");
	daf1.addAttack("d","a");

	printExts(daf1);
}


void demo2()
{
	DungAF daf2;
	daf2.addArgument("x1");
	daf2.addArgument("x2");
	daf2.addArgument("x3");
	daf2.addArgument("x4");

	daf2.addAttack("x2","x1");
	daf2.addAttack("x4","x2");
	daf2.addAttack("x3","x2");
	daf2.addAttack("x2","x3");
	daf2.addAttack("x3","x3");

	printExts(daf2);
}


void demo3()
{
	DungAF daf3;
	daf3.addArgument("a");
	daf3.addArgument("b");
	daf3.addArgument("c");

	daf3.addAttack("a","b");
	daf3.addAttack("b","a");
	daf3.addAttack("c","a");
	//daf3.addAttack("c","b");
	daf3.addAttack("b","c");

	printExts(daf3);
}

void demo4()
{
	DungAF daf4;
	daf4.addArgument("a");
	daf4.addArgument("b");
	daf4.addArgument("c");
	daf4.addArgument("d");
	daf4.addArgument("e");

	daf4.addAttack("a","b");
	daf4.addAttack("a","c");
	daf4.addAttack("b","d");
	daf4.addAttack("c","d");
	daf4.addAttack("d","e");
	daf4.addAttack("e","a");

	printExts(daf4);
}


void demo5()
{
	// AAAI 15
	cout << "-----------------------------------" << endl;
	cout << "Demo 5, from AAAI-15" << endl;
	DungAF daf5;
	daf5.addArgument("a");
	daf5.addArgument("b");
	daf5.addArgument("c");
	daf5.addArgument("d");
	daf5.addArgument("e");
	daf5.addArgument("f");

	
	daf5.addAttack("a","e");
	daf5.addAttack("a","b");
	daf5.addAttack("b","c");
	daf5.addAttack("b","a");
	daf5.addAttack("c","e");
	//daf5.addAttack("c","f");
	daf5.addAttack("d","a");
	daf5.addAttack("e","d");
	daf5.addAttack("f","c");
	//daf5.addAttack("a","d");

	cout << daf5.toString() << endl;
	printExts(daf5);
}

int main(int argc, char* argv[])
{
	DungAF daf;

	uint64_t time_begin, time_end;
	time_begin = clock_ns();

	std::string inputFile("../datasets/kleinberg/9/Kle_n3e0.9_02.dl");
	//std::string inputFile("../datasets/kleinberg/16/Kle_n4e0.9_01.dl");
	// std::string inputFile("../datasets/iccma15_testcases1/apx/rdm50_0.apx");
	// std::string inputFile("../datasets/MyExamples/hunter.dl");
	//std::string inputFile("../datasets/iccma15_testcases1/apx/rdm1000_1.apx");
	// std::string inputFile("../datasets/test_cases2/apx/119290__200__21_22_51__46.apx");

	std::string outputFile("output.res");
	ofstream ofs(outputFile);

	argumatrix::parser::Aspartix2DungAF(inputFile, daf);

	cout << daf.toString() << endl;
	//PlReasoner plr(daf);
	//plr.printGroundedExt();
	//plr.Test_time();
	//demo1();
	//demo2();
	//demo3();
	//demo4();
	//demo5();
	Reasoner* rsner = new AdmissiblePlReasoner(daf, "AD2");
	// rsner->setOutput(ofs);

	cout << "EE:" << endl;
	rsner->task_EE();

	cout << "DN:" << endl;
	rsner->task_DN();
	//cplr.isSkepticallyJustified(argset);
	time_end = clock_ns();
	cout << endl << "The time is : " << time_end - time_begin << endl;

	// rsner->Test_time();

	cout << "================================" << endl;
	//rsner->fetchAllExts("stable2");

	cout << "END!" << endl;
	system("pause");

	return 0;
}