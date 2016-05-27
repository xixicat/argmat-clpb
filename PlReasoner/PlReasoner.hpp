//=======================================================================
// Copyright 2015 Tsinghua University.
// Authors: Fuan Pu (Pu.Fuan@gmail.com)
// 
// Compute Dung's semantics by SWI-Prolog
//=======================================================================

#ifndef PROLOG_REASONER_HPP
#define PROLOG_REASONER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <utility>   
#include <algorithm>   
#include <iosfwd>

// argumatrix
#include "config/config.hpp"
#include "bitmatrix/bitvector.hpp"
#include "bitmatrix/bitmatrix.hpp"
#include "dung_theory/DungAF.hpp"
#include "dung_theory/ArgumentProperty.hpp"
#include "dung_theory/AttackProperty.hpp"
#include "dung_theory/Reasoner.hpp"

// SWI-prolog
#include <SWI-cpp.h>
#include "swipl-util.hpp"



// #define ARG_PROLOG_FILE	"./argmat-clpb.pl"
#define ARG_PROLOG_FILE	"../PlReasoner/argmat-clpb"

namespace argumatrix{

using namespace std;


/**
 * @brief  The ArgumatrixPlEngine. Note that the ArgumatrixPlEngine must 
 * be defined in front of all SWI-Prolog term or object, since all these 
 * object are initialized based on this engine. We set *ArgumatrixPlEngine*
 * as a static and globe variable since in C++ a static variable is always
 * initialized before the main function.    
 */
static ArgumatrixPlEngine argmatrixPlEngine((char*)"PlReasoner");


// typedef PlTimeQry ClpbQry;
typedef PlQuery ClpbQry;


class PlReasoner : public Reasoner {
public:
	PlReasoner(const DungAF& daf, streambuf* osbuff = std::cout.rdbuf());
	~PlReasoner();


	/**
	 * Method:    consultPlFile
	 * @brief   Loading the Prolog source file [argmat-clpb]   
	 * @param     std::string plFile
	 * @note SWI-Prolog supports compilation of individual or multiple Prolog
	 * source files into ‘Quick Load Files’. A ‘Quick Load File’ (.qlf file)
	 * stores the contents of the file in a precompiled format. These files
	 * load considerably faster than source files and are normally more compact.
	 * They are machine-independent and may thus be loaded on any implementation
	 * of SWI-Prolog. Note, however, that clauses are stored as virtual 
	 * machine instructions. Changes to the compiler will generally make old
	 * compiled files unusable. Quick Load Files are created using qcompile/1. 
	 * They are loaded using consult/1 or one of the other file-loading 
	 * predicates described in section 4.3. If consult/1 is given an 
	 * explicit .pl file, it will load the Prolog source. When given a .qlf
	 * file, it will load the file. When no extension is specified, it will
	 * load the .qlf file when present and the .pl file otherwise.
	 */
	void consultPlFile(std::string plFile = ARG_PROLOG_FILE);

	void Test_time();

	/**
	 * Problem [EE-$\sigma$]
	 * @brief Print all extensions with respect to semantic predct
	 * @param no argument
	 * @return no return.
	 */
	void printAllExts(const std::string& predct);
	void printAllExts2(const std::string& predct);

	/**
	 * Method:    fetchAllExts
	 * FullName:  public  argumatrix::PlReasoner::fetchAllExts
	 * @brief      
	 * @param     const std::string & predct
	 * @return    void
	 * @retval
	 * @note
	 * @see
	 */
	void fetchAllExts(const std::string& predct);

	/**
	 * Problem [EC-$\sigma$]
	 * @brief Given an $\textit{AF}=\left< \mathcal{X}, \mathcal{R}\right>$ and 
	 * an argument $s \in \mathcal{X}$ (respectively, a set of arguments 
	 * $S\subseteq \mathcal{X}$), enumerate all sets $E\subseteq \mathcal{X}$
	 * such that $E \in \mathcal{E}_\sigma(AF)$ and $s\in E$ (respectively,
	 * $S \subseteq E$).
	 * @param set<string>, or a Boolean vector: a set of argument
	 * @return no return.
	 */
	void printAllExts(const std::string& predct, const std::set<string>& argset);
	void printAllExts(const std::string& predct, const std::vector<int>& vecii);


	/**
	 * Problem [SC-$\sigma$]
	 * @brief Given an $\textit{AF}=\left< \mathcal{X}, \mathcal{R}\right>$ and an 
	 * argument $s \in \mathcal{X}$ (respectively, a set of arguments 
	 * $S\subseteq \mathcal{X}$), enumerate some set $E\subseteq \mathcal{X}$
	 * such that $E \in \mathcal{E}_\sigma(AF)$ and $s\in E$ (respectively, 
	 * $S \subseteq E$).
	 * @param a set of arguments.
	 * @return no return.
	 */
	void printSomeExt(const std::string& predct, const std::set<string>& argset);
	void printSomeExt(const std::string& predct, const vector<int>& vecii);

	/**
	 * Problem [DN-$\sigma$]
	 * @brief Given an $AF = \left<X,R\right>$ and a set of arguments $S \subseteq X$. Decide whether
	 * there exist a non-empty $\sigma$-extension for AF
	 * @return true if there exist a non-empty \sigma-extension; otherwise return false.
	 */
	bool verifyNonemptyExt(const std::string& predct);
	
	/**
	 * Problem [DE-$\sigma$]
	 * @brief Given an $AF = \left<X,R\right>$ and a set of arguments $S \subseteq X$. Decide whether S is
	 * a $\sigma$-extension of AF, i.e., $S \in E_\sigma(AF)$.
	 * @param a set of arguments.
	 * @return true if argset is a \sigma-extension; otherwise return false.
	 */
	bool verifyExtension(const std::string& predct, std::set<string>& argset);
	bool verifyExtension(const std::string& predct, bitvector& bvecii);

	/**
	 * Problem [DC-$\sigma$]
	 * @brief Given an $AF = \left<X,R\right>$ and an argument s \in X (respectively, a set of 
	 * arguments $S \subseteq X$). Decide whether s contained (respectively, S included)
	 * in some E \in E_\sigma(AF) (i.e., credulously justified).
	 */
	bool isCredulouslyJustified(const std::string& predct, const std::set<string>& argset);
	
	/**
	 * Problem [DS-$\sigma$]
	 * @brief Given an $AF = \left<X,R\right>$ and an argument s \in X (respectively, a set of 
	 * arguments $S \subseteq X$). Decide whether s contained (respectively, S included)
	 * in each E \in E_\sigma(AF) (i.e., skeptically justified).
	 */
	bool isSkepticallyJustified(const std::string& predct, const std::set<string>& argset);
protected:

	/**
	 * @brief Find all extensions and store them in *m_extensions* with the form of
	 * bitvector.
	 * @param string& predct: The predicate.
	 * @return no return. The results are added into *m_extensions*.
	 */
	void findAllExts(const std::string& predct);

	/**
	 * @brief Create the attack matrix with the form of PlTerm, which is an input of
	 * SWI-Prolog.
	 * @return no return. The result is stored in member variable *m_PlAtkMtx*.
	 */
	void createPlAttackMatrix();

	/**
	 * @brief Print an extension with the form of the bool list term. Assume a bool
	 * list term is [0, 1, 0, 1], the arguments corresponding to entry 1 will
	 * be print. The member *m_output* will determine where to print.
	 * @param PlTerm& plt : The PlTerm is a bool list term, which represents
	 * an extension.
	 * @return no return. 
	 */
	void printLableExtByBlistTerm(const PlTerm& plt);

	/**
	 * @brief Print an extension with the form of the bool matrix term. Assume a bool
	 * list term is [[0, 1, 0, 1], [0, 1, 0, 1]], the arguments corresponding to entry 1 will
	 * be print. The member *m_output* will determine where to print.
	 * @param PlTerm& plt : The PlTerm is a bool list term, which represents
	 * an extension.
	 * @return no return. 
	 */
	void printLableExtByBmatrixTerm(const PlTerm& pmtx);

	/**
	 * @brief Convert a bool list term into a bitvertor, and add it to extension.
	 * @param PlTerm& plt : The PlTerm is a bool list term, which represents
	 * an extension.
	 * @return no return. The results are added into m_extensions.
	 */
	void addBlTermToBvExts(const PlTerm& plt);


	/**
	 * @brief Given an $AF = \left<X,R\right>$ and a set of arguments $S \subseteq X$. Decide whether
	 * there exist a \sigma-extension that includes S
	 * @param a set of arguments.
	 * @return true if there exist a \sigma-extension that includes S; otherwise return false.
	 */
	bool verifyInclusion(const std::string& predct, const vector<int>& vecii);

	/**
	 * @brief Given an $AF = \left<X,R\right>$ and a set of arguments $S \subseteq X$. Decide whether
	 * there exist a \sigma-extension that excludes S
	 * @param a set of arguments.
	 * @return true if there exist a \sigma-extension that excludes S; otherwise return false.
	 */
	bool verifyExclusion(const std::string& predct, const bitvector& vecB);
protected:

	// The attack matrix, defined by Prolog *term_t*
	PlTerm m_PlAtkMtx;   
};


PlReasoner::PlReasoner(const DungAF& daf, streambuf* osbuff/* = std::cout.rdbuf()*/):
	Reasoner(daf, osbuff)
{
	// Load the logical engine
	consultPlFile();

	// Initialization
	createPlAttackMatrix();
}


void PlReasoner::createPlAttackMatrix()
{
	try	{ 
		bitmatrix2plmatrixTerm(m_BmAtkMtx, m_PlAtkMtx.ref);
	} catch ( PlException &ex ) { 
		cerr << (char *) ex << endl;
		exit(1);
	}
}

void PlReasoner::consultPlFile(std::string plFile /*= ARG_PROLOG_FILE*/)
{
	try {
		// PlCall("database", "consult", PlTermv(PlTerm(plFile.c_str())));
		predicate_t p_consult = PL_predicate("consult", 1, "database");
		term_t t = PL_new_term_ref();
		if ( !PL_put_string_chars(t, plFile.c_str()) )
			throw PlResourceError();
		if ( !PL_call_predicate(NULL, 0, p_consult, t) )
			throw PlResourceError();
	}
	catch (PlException* e)
	{
		cerr << "PlReasoner::consultPlFile loading argmat-clpb.pl failed!" 
			 << "   " << (char*)e << endl;
	}
}

void PlReasoner::printLableExtByBlistTerm(const PlTerm& plt)
{
	if ( !PL_is_list(plt.ref) )
	{
		throw PlTypeError("list", plt.ref);
		return;
	}

	//the list ref will get modified so copy it
	term_t tail = PL_copy_term_ref(plt.ref);
	term_t head = PL_new_term_ref();
	int index = 0;
	int x;
	bool first = true;
	m_output << LEFT_LIMITER;
	while(PL_get_list(tail, head, tail)){
		PL_get_integer(head, &x);
		if(x == 1) 
		{
			if(first){
				first = false;
			}else{
				m_output << DELIMITER; // ","
			}
			m_output << m_argLabels[index];
		}
		
		++index;
	}
	m_output << RIGHT_LIMITER;
}

void PlReasoner::addBlTermToBvExts(const PlTerm& plt)
{
	if ( !PL_is_list(plt.ref) )
	{
		throw PlTypeError("list", plt.ref);
		return;
	}

	//the list ref will get modified so copy it
	term_t tail = PL_copy_term_ref(plt.ref);
	term_t head = PL_new_term_ref();
	int x, index = 0;
	bitvector _bv(m_argNum, 0);  // all bits are 0s.
	while(PL_get_list(tail, head, tail)) {
		PL_get_integer(head, &x);
		if(x == 1) {
			_bv.set(index);
		}

		++index;
	}

	// The length of the list must be equal to the number of arguments.
	assert(m_argNum == index);

	m_extensions.insert( _bv );
}



PlReasoner::~PlReasoner()
{
	
}

void PlReasoner::printAllExts2(const string& predct)
{
	m_output << "Semantics - " << predct << ":" << endl;
	
	printAllExts(predct);
}

void PlReasoner::Test_time()
{
	m_output << "Time test :" << endl;

	try
	{ 
		// Building the constraints
		PlTermv clp_term(2);
		// The first term is attack matrix
		clp_term[0] = m_PlAtkMtx;  		

		PlTimeQry clp_query("complete5", clp_term);

		m_output << LEFT_LIMITER;  // [
		while( clp_query.next_solution() ) {
			// Find all assignments 
			PlTermv labeling_term(1);
			labeling_term[0] = clp_term[1];

			PlTimeQry labeling_query("clpb", "labeling", labeling_term);
			bool first = true;

			while( labeling_query.next_solution() ) {
				if(first) {
					first = false;
				} else {
					m_output << DELIMITER; // ","
				}
				// outputBoolListTerm((term_t)labeling_term[0]);
				printLableExtByBlistTerm(labeling_term[0]);
			}
		}
		m_output << RIGHT_LIMITER; // ]
	} catch ( PlException &ex ) { 
		cerr << (char *) ex << endl;
		exit(1);
	}
	m_output << endl;
}

__inline
void PlReasoner::printAllExts(const std::string& predct, const std::set<string>& argset)
{
	printAllExts(predct, labelSet2IntVector(argset));
}

void PlReasoner::printAllExts(const std::string& predct, const std::vector<int>& vecii)
{
	try
	{ 
		// Building the constraints
		PlTermv clp_term(2);
		// The first term is attack matrix
		clp_term[0] = m_PlAtkMtx;  
		// The second term is variable term, output
		createBlListTerm(vecii, clp_term[1].ref);  

		ClpbQry clp_query(predct.c_str(), clp_term);

		m_output << LEFT_LIMITER;  // [
		while( clp_query.next_solution() )  {
			// Find all assignments 
			PlTermv labeling_term(1);
			labeling_term[0] = clp_term[1];
			ClpbQry labeling_query("labeling", labeling_term);
			bool first = true;

			while( labeling_query.next_solution() ) {
				if(first){
					first = false;
				} else {
					m_output << DELIMITER; // ","
				}
				// outputBoolListTerm((term_t)labeling_term[0]);
				printLableExtByBlistTerm(labeling_term[0]);
			}
		}
		m_output << RIGHT_LIMITER << endl;

	} catch ( PlException &ex ) { 
		cerr << (char *) ex << endl;
		exit(1);
	}
}

void PlReasoner::printAllExts(const std::string& predct)
{
	try
	{ 
		// Building the constraints
		PlTermv clp_term(2);
		// The first term is attack matrix
		clp_term[0] = m_PlAtkMtx;  

		ClpbQry clp_query(predct.c_str(), clp_term);

		m_output << LEFT_LIMITER;  // [
		while( clp_query.next_solution() ) {
			// Find all assignments 
			PlTermv labeling_term(1);
			labeling_term[0] = clp_term[1];
			ClpbQry labeling_query("clpb", "labeling", labeling_term);
			bool first = true;

			while( labeling_query.next_solution() ) {
				if(first){
					first = false;
				}else{
					m_output << DELIMITER; // ","
				}
				// outputBoolListTerm((term_t)labeling_term[0]);
				printLableExtByBlistTerm(labeling_term[0]);
			}
		}
		m_output << RIGHT_LIMITER << endl;

	} catch ( PlException &ex ) { 
		cerr << (char *) ex << endl;
		exit(1);
	}
}

__inline
void PlReasoner::printSomeExt(const std::string& predct, const std::set<string>& argset)
{
	printSomeExt(predct, labelSet2IntVector(argset) );
}

void PlReasoner::printSomeExt(const std::string& predct, const vector<int>& vecii)
{
	try
	{ 
		// Building the constraints
		PlTermv clp_term(2);
		// The first term is attack matrix
		clp_term[0] = m_PlAtkMtx;  
		// The second term is variable term, output
		createBlListTerm(vecii, clp_term[1].ref);  

		ClpbQry clp_query(predct.c_str(), clp_term);

		while( clp_query.next_solution() )
		{
			// Find all assignments 
			PlTermv labeling_term(1);
			labeling_term[0] = clp_term[1];
			ClpbQry labeling_query("clpb", "labeling", labeling_term);

			if( labeling_query.next_solution() ) {
				printLableExtByBlistTerm(labeling_term[0]);
				m_output << endl;
				return;
			} else {
				m_output << STR_NO << endl;
				return;
			}
		}
		m_output << STR_NO << endl;

	} catch ( PlException &ex ) { 
		cerr << (char *) ex << endl;
		exit(1);
	}
}

bool PlReasoner::verifyNonemptyExt(const std::string& predct)
{
	try
	{ 
		PlTermv nonempy_term(3);
		nonempy_term[0] = PlTerm(predct.c_str());
		nonempy_term[1] = m_PlAtkMtx;
		ClpbQry nonempy_query("nonempty", nonempy_term);
		while( nonempy_query.next_solution() )
		{
			// Find all assignments 
			PlTermv labeling_term(1);
			labeling_term[0] = nonempy_term[2];
			ClpbQry labeling_query("clpb", "labeling", labeling_term);

			if( labeling_query.next_solution() ) {
				// printLableExtByBlistTerm(labeling_term[0]);
				// m_output << STR_YES << endl;
				return true;
			} else {
				// m_output << STR_NO << endl;
				return false;
			}	
		}
	} catch ( PlException &ex ) { 
		cerr << (char *) ex << endl;
		exit(1);
	}

	// m_output << STR_NO << endl;
	return false;
}

bool PlReasoner::verifyInclusion(const std::string& predct, const vector<int>& vecii)
{
	try{ 
		// Building the constraints
		PlTermv clp_term(2);
		// The first term is attack matrix
		clp_term[0] = m_PlAtkMtx;  
		// The second term is variable term, output
		createBlListTerm(vecii, clp_term[1].ref);  

		ClpbQry clp_query(predct.c_str(), clp_term);

		while( clp_query.next_solution() ){
			// Find all assignments 
			PlTermv labeling_term(1);
			labeling_term[0] = clp_term[1];
			ClpbQry labeling_query("clpb", "labeling", labeling_term);

			// If there exists a solution
			if( labeling_query.next_solution() ) {
				return true;
			} else {
				return false;
			}
		}
	} catch ( PlException &ex ) { 
		cerr << (char *) ex << endl;
		exit(1);
	}

	return false;
}

bool PlReasoner::verifyExclusion(const std::string& predct, const bitvector& vecB)
{
	try
	{ 
		PlTermv nonempy_term(4);
		nonempy_term[0] = PlTerm(predct.c_str());
		nonempy_term[1] = m_PlAtkMtx;
		createBlListTerm(vecB, nonempy_term[2].ref);
		ClpbQry nonempy_query("exclusion", nonempy_term);
		while( nonempy_query.next_solution() )
		{
			// Find all assignments 
			PlTermv labeling_term(1);
			labeling_term[0] = nonempy_term[3];
			ClpbQry labeling_query("clpb", "labeling", labeling_term);

			if( labeling_query.next_solution() )
			{
				// printLableExtByBlistTerm(labeling_term[0]);
				return true;
			} else {
				return false;
			}	
		}
	} catch ( PlException &ex ) { 
		cerr << (char *) ex << endl;
		exit(1);
	}

	return false;
}

__inline
bool PlReasoner::verifyExtension(const std::string& predct, std::set<string>& argset)
{
	bitvector _bv = m_daf.labelSet2bv(argset);
	return verifyExtension(predct, _bv );
}

bool PlReasoner::verifyExtension(const std::string& predct, bitvector& bvec)
{
	try
	{ 
		// Building the constraints
		PlTermv clp_term(2);
		// The first term is attack matrix
		clp_term[0] = m_PlAtkMtx;  
		// The second term is variable term, output
		createBlListTerm(bvec, clp_term[1].ref);  

		ClpbQry clp_query(predct.c_str(), clp_term);

		if ( clp_query.next_solution() )
		{
			// m_output << STR_YES << endl;
			return true;
		} else 	{
			// m_output << STR_NO << endl;
			return false;
		}
	} catch ( PlException &ex ) { 
		cerr << (char *) ex << endl;
		exit(1);
	}
}

bool PlReasoner::isCredulouslyJustified(const std::string& predct, const std::set<string>& argset)
{
	if (verifyInclusion(predct, labelSet2IntVector(argset)))
	{
		// m_output << STR_YES << endl;
		return true;
	} else {
		// m_output << STR_NO << endl;
		return false;
	}
}

bool PlReasoner::isSkepticallyJustified(const std::string& predct, const std::set<string>& argset)
{
	if (verifyInclusion(predct, labelSet2IntVector(argset)))
	{
		if ( verifyExclusion(predct, m_daf.labelSet2bv(argset) ) )
		{
			return false;
		} else {
			return true;
		}
	} else {
		return false;
	}
}

void PlReasoner::findAllExts(const std::string& predct)
{
	try
	{ 
		// Building the constraints
		PlTermv clp_term(2);
		// The first term is attack matrix
		clp_term[0] = m_PlAtkMtx;  
		// The second term is variable term, output
		createBlListTerm(getGroundedIntVector(), clp_term[1].ref);  

		ClpbQry clp_query(predct.c_str(), clp_term);

		while( clp_query.next_solution() )
		{
			// Find all assignments 
			PlTermv labeling_term(1);
			labeling_term[0] = clp_term[1];
			ClpbQry labeling_query("clpb", "labeling", labeling_term);

			while( labeling_query.next_solution() )
			{
				// outputBoolListTerm((term_t)labeling_term[0]);
				addBlTermToBvExts( labeling_term[0] );
			}
		}
	} catch ( PlException &ex ) { 
		cerr << (char *) ex << endl;
		exit(1);
	}
}

void PlReasoner::fetchAllExts(const std::string& predct)
{
	try
	{ 
		// fetchAll(complete1, AttM, E)
		PlTermv clp_term(3);
		
		clp_term[0] = PlTerm(predct.c_str()); // The first term is the goal	
		clp_term[1] = m_PlAtkMtx;  // The second term is attack matrix

		ClpbQry clp_query("fetchAll", clp_term);

		m_output << LEFT_LIMITER;
		while( clp_query.next_solution() )
		{
			printLableExtByBmatrixTerm(clp_term[2]);
		}
		m_output << RIGHT_LIMITER << endl;
	} catch ( PlException &ex ) { 
		cerr << (char *) ex << endl;
		exit(1);
	}
}

void PlReasoner::printLableExtByBmatrixTerm(const PlTerm& pmtx)
{
	//the list ref will get modified so copy it
	term_t tail = PL_copy_term_ref(pmtx.ref);
	term_t head = PL_new_term_ref();
	bool first = true;
	
	while(PL_get_list(tail, head, tail)){
		if(first){
			first = 0;
		}else{
			m_output << DELIMITER; // ","
		}
		printLableExtByBlistTerm(head);
	}
}


} // namespace argumatrix



#endif  //PROLOG_REASONER_HPP