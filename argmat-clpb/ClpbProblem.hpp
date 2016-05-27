/*!
 * @file ClpbProblem.hpp
 * @date 2016/05/04 15:20
 *
 * @author Fuan Pu
 * Contact: Pu.Fuan@gmail.com
 *
 * @brief 
 *
 * TODO: long description
 *
 * @note
*/
#ifndef ARGUMATRIX_CLPB_PROBLEM_HPP
#define ARGUMATRIX_CLPB_PROBLEM_HPP

#include <map>
#include <string>
#include <iostream>

#include "dung_theory/DungAF.hpp"
#include "dung_theory/GroundedReasoner.hpp"
#include "config/config.hpp"
#include "parser/parser.hpp"
#include "PlReasoner/CompletePlReasoner.hpp"
#include "PlReasoner/ConflictfreePlReasoner.hpp"
#include "PlReasoner/StablePlReasoner.hpp"
#include "PlReasoner/AdmissiblePlReasoner.hpp"

namespace argumatrix{

using namespace std;


/**
* @brief Print the version information
*/
void printVersionInfo();


/**
* @brief Print the help/usage information
*/
void printHelpInfo();

/**
* @brief Print the ProblemOptions information
*/
void printProblemOptions();

/**
* @brief Print the FileFormatOptions information
*/
void printFileFormatOptions();


struct ClpbProblem {
	string m_problemType;
	string m_fileFmt;
	string m_inputFile;
	string m_outputFile;
	set<string> m_otherArgs;
	bool m_timeFlag;

	DungAF daf;

	// Default Constructor
	ClpbProblem();

	void parseParams(int argc, char* argv[]);

	// Solve()
	void solve();

	// Load argument graph from a files;
	void loadDungAF();


	void reasoning(Reasoner* rsner, const string& task);


	/**
	 * Method:    parseAddtionalParams
	 * FullName:  public  argumatrix::ClpbProblem::parseAddtionalParams
	 * @brief     interpret additional arguments into argument set. The format
	 * of input is arg1[,arg2,arg3,...], e.g., a1,a2. The interpretted results
	 * are stored in the member variable *set<string> otherArgs*.
	 * @param     const string & oargs
	 * @see
	 */
	void parseAddtionalParams(const string& oargs);

	// ....
	friend ostream & operator<<(ostream & out, ClpbProblem& problem);
};  // struct problem


// Default Constructor
ClpbProblem::ClpbProblem() : 
	m_problemType(""), 
	m_fileFmt(""),
	m_inputFile(""),
	m_outputFile(""),
	m_timeFlag(false)
{
	
}

void ClpbProblem::parseParams(int argc, char* argv[])
{
	for(int i=1; i<argc; i++){
		if(strcmp(argv[i],"-p")==0){
			if((i+1)<argc){
				m_problemType = argv[++i];
				continue;
			}
		} else 
		if(strcmp(argv[i],"-f")==0){
			if((i+1)<argc){
				m_inputFile = argv[++i];
				continue;
			}			
		} else
		if(strcmp(argv[i],"-fo")==0){
			if((i+1)<argc){
				m_fileFmt = argv[++i];
				continue;
			}
		} else
		if(strcmp(argv[i],"-o")==0){
			if((i+1)<argc){
				m_outputFile = argv[++i];
				continue;
			}
		} else
		if(strcmp(argv[i],"-a")==0){
			if((i+1)<argc){
				parseAddtionalParams(argv[++i]);
				continue;
			} else {

			}
		} else
		if(strcmp(argv[i],"-t")==0){
			m_timeFlag = true;
				continue;
		}
	}
}

void ClpbProblem::loadDungAF()
{
	if("" == m_inputFile) {
		cerr << "No input file!" << endl;
		exit(1);
	}
	if("" == m_fileFmt) {
		cerr << "The file format must be given!" << endl;
		printFileFormatOptions();
		exit(1);
	}

	if(m_fileFmt=="apx") {
		if(!(argumatrix::parser::Aspartix2DungAF(m_inputFile, daf))) {
			cerr<< "apx file loading failed." <<endl;
			exit(1);
		}
	} else 
	if(m_fileFmt=="tgf") {
		if(!(argumatrix::parser::TrivialGraph2DungAF(m_inputFile, daf))){
			cerr<< "tgf file loading failed."<<endl;
			exit(1);
		}
	} else {
		cerr << "The file format ["<< m_fileFmt<<"] is not supported!"<< endl;
		printFileFormatOptions();
		exit(1);
	}
}

void ClpbProblem::solve()
{
	loadDungAF();

	if(m_problemType.size() < 5) {
		cerr << "The problem type is invalid!" << endl;
		printProblemOptions();
		exit(1);
	}

	// Osbuff
	streambuf* osbuff = NULL;
	ofstream ofs;
	// if no out file is specified, then the results are displayed using 
	// standard output.
	if ("" == m_outputFile) {  
		osbuff = std::cout.rdbuf();
	} else {
		ofs.open(m_outputFile, ios_base::out);
		if(!ofs) {
			cerr << "Can not open the file: " << m_outputFile << endl;
			exit(1);
		}
		osbuff = ofs.rdbuf();
	}

	string sm_type = m_problemType.substr(3,2);  // Get the semantics option
	string sm_full = m_problemType.substr(3);
	string tsk_type = m_problemType.substr(0,2); // Get the Task option
	
	Reasoner* rsner = NULL;
	
	if (sm_type == "CF") {
		rsner = new ConflictfreePlReasoner(daf, sm_full, osbuff);
	} else 
	if (sm_type == "ST") {
		rsner = new StablePlReasoner(daf, sm_full, osbuff);
	} else
	if (sm_type == "AD") {
		rsner = new AdmissiblePlReasoner(daf, sm_full, osbuff);
	} else
	if (sm_type == "CO") {
		rsner = new CompletePlReasoner(daf, sm_full, osbuff);
	} else
	if (sm_type == "GR") {
		rsner = new GroundedReasoner(daf, osbuff);
	} else
	if (sm_type == "PR") {
		//reasoning<CompletePlReasoner>(TODO, TODO);
	} 
	else {
		cerr << "The semantics [" << sm_type << "] "
			 << "is not supported!" << endl;
		printProblemOptions();
		exit(1);
	}

	// Do reason TASK
	if (rsner != NULL) {
		reasoning(rsner, tsk_type);

		delete rsner;
	}
}

void ClpbProblem::reasoning(Reasoner* rsner, const string& task)
{
	if (task == "EE") {
		rsner->task_EE();
	} else 
	if (task == "EX") {
		rsner->task_EX();
	} else 
	if (task == "EC") {
		rsner->task_EC(m_otherArgs);
	} else
	if (task == "SC") {
		rsner->task_SC(m_otherArgs);
	} else
	if (task == "SE") {
		rsner->task_SE();
	} else
	if (task == "DE")
	{
		rsner->task_DE(m_otherArgs);
	} else
	if (task == "DN")
	{
		rsner->task_DN();
	} else 
	if (task == "DC")
	{
		rsner->task_DC(m_otherArgs);
	} else
	if (task == "DS")
	{
		rsner->task_DS(m_otherArgs);
	} else {
		cerr << "The task [" << task << "] "
			<< "is not supported!" << endl;
		printProblemOptions();
		exit(1);
	}
}

void ClpbProblem::parseAddtionalParams(const string& oargs)
{
	m_otherArgs.clear();
	if (oargs.length()==0){
		return;
	}
	string sptor = ",";  //The separator
	size_type bgn=0, nd=0;
	string arg;
	for (nd = oargs.find_first_of(sptor); 
		 nd != string::npos; 
		 nd = oargs.find_first_of(sptor, bgn))
	{
		if (nd-bgn==0) { // avoid the case ,, and the , at the beginning.
			cerr << "Wrong additional argument : " << oargs << endl;
			cerr << "The right input for additional argument is: " << endl
				<< "  arg1[,arg2,arg3,...]" << endl;
			exit(1);		
		}
		
		arg = oargs.substr(bgn, nd - bgn);

		m_otherArgs.insert(arg);
		bgn = nd + 1;
	}
	
	// insert the last element
	arg = oargs.substr(bgn);
	if (arg.length()>0)
		m_otherArgs.insert(arg);
	else {
		cerr << "Wrong additional argument : " << oargs << endl;
		cerr << "The right input for additional argument is: " << endl
			 << "  arg1[,arg2,arg3,...]" << endl;
		exit(1);	
	}
}


ostream & operator<<(ostream & out, ClpbProblem& problem)
{
	if (problem.m_problemType == "") {
		out << "The problem is not initialized!" << endl;
		return out;
	}

	out << "The problem is:" << endl;
	out	<< INDENT << "Computational problem: " << problem.m_problemType << endl;
	out	<< INDENT << "FILE_FORMAT: " << problem.m_fileFmt << endl;
	out	<< INDENT << "Input file: " << problem.m_inputFile << endl;
	// out	<< INDENT << "Additional args: " << problem.m_otherArgs << endl;

	return out;
}

void printHelpInfo()
{
	/*print the help information.*/
	std::cout<< "Usage: " <<endl;
	std::cout<< "argmat-clpb " << "-p <problem> "
		<< "-f <file> " << "-fo <fileformat> "
		<< "[-a <additional_parameter>] " 
		<< "[-o <file>] " << "[-t]" << endl;
	std::cout<< "<problem>" << endl
		<< INDENT;
	printProblemOptions();

	std::cout<< "<fileformat>" << endl
		<< INDENT;
	printFileFormatOptions();


	/*how to display version information.*/
	std::cout<< "argmat-clpb --version" <<endl
		<< INDENT << "Display the version information." <<endl;

	/*how to display the problem information. */
	std::cout<< "argmat-clpb --problem "<<endl
		<< INDENT <<"Display the problem information. "<<endl;

	/*how to display the format information. */
	std::cout<< "argmat-clpb --format "<<endl
		<< INDENT <<"Display the format information. "<<endl;

	/*how to display the help information. */
	std::cout<< "argmat-clpb --help "<<endl
		<< INDENT <<"Display the help information. "<<endl;

	std::cout<< "argmat-clpb"<<endl
		<< INDENT <<"Display the all the information. "<<endl;
}

void printVersionInfo()
{
	/*Print the version information.*/
	cout << "argmat-clpb: version 0.1" << endl << endl
		<< "Dung's abstract argumentation framework" << endl
		<< "Copyright @ 2015 Tsinghua University." << endl
		<< "East Main Building 11-323, Tsinghua University," << endl
		<< "Haidian District, Beijing, China 100084." << endl
		<< "Authors:" << endl
		<< INDENT << "Fuan pu (Pu.Fuan@gmail.com)" << endl;
}

void printFileFormatOptions()
{
	cout << "The supported file formats:" << endl;
	cout << INDENT << "[apx,tgf]" <<  endl;
}

void printProblemOptions()
{
	cout << "[DC-CO]" <<  endl;
}

} // namespace argumatrix

#endif //  ARGUMATRIX_CLPB_PROBLEM_HPP