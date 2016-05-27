//=======================================================================
// Copyright 2015 Tsinghua University.
// Authors: Fuan Pu (Pu.Fuan@gmail.com)
// 
// SWI-Prolog wrapper
//=======================================================================

#ifndef SWI_PROLOG_UTIL_HPP
#define SWI_PROLOG_UTIL_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <utility>   
#include <algorithm>   
#include <stdlib.h>

//boost
#include <boost/graph/graph_traits.hpp> 
#include <boost/graph/adjacency_list.hpp>


#include "bitmatrix/bitvector.hpp"
#include "bitmatrix/bitmatrix.hpp"

// SWI-Prolog
#include <SWI-cpp.h>

namespace argumatrix{

using namespace std;

void bitmatrix2plmatrixTerm(const bitmatrix& bm, term_t bmtx);
void printBoolListTerm(const term_t blst, std::ostream &output = std::cout);
void printBoolMatrixTerm(const term_t bmtx, std::ostream &output = std::cout);
void createBlListTerm(const vector<int>& vecI, term_t blst);
void createBlListTerm(const bitvector& vecB, term_t blst);


/**
* class: ArgumatrixPlEngine
* @brief We rewrite the PlEngine since we do not want the PlEngine to print the 
* welcome and version information. These information can not be printed if we use command
* "-g true" or "--quiet" or "-q". Note that both PlEngine and ArgumatrixPlEngine 
* must be defined as a static instance, since multiple engines exist may lead 
* to crash when one of the engins is closed (i.e., invoking PL_cleanup) then 
* all other running engines are closed. Define PlEngine as a static instance
* can solve this problem since static instances are global and stored in static
* variable area. They are initialized before the main function and released 
* when the program exits. 
* @see The definition of ArgumatrixPlEngine is at the beginning of [PlReasoner.hpp].
*/
class ArgumatrixPlEngine {
public:
	/**
	 * Method:    ArgumatrixPlEngine
	 * FullName:  public  argumatrix::ArgumatrixPlEngine::ArgumatrixPlEngine
	 * @brief  Create the Prolog engine.    
	 * @param plArgc: the number of arguments
	 * @param plArgv: a list of options. 
	 * Options:
	 * * -x state         Start from state (must be first)
	 * * -[LGT]size[KMG]  Specify {Local,Global,Trail} limits
	 * * -t toplevel      Toplevel goal
	 * * -g goal          Initialisation goal
	 * * -f file          User initialisation file
	 * * -F file          System initialisation file
	 * * -l file          Script source file
	 * * -s file          Script source file
	 * * -p alias=path    Define file search path 'alias'
	 * * [+/-]tty         Allow tty control
	 * * -O               Optimised compilation
	 * * --nosignals      Do not modify any signal handling
	 * * --nodebug        Omit generation of debug info
	 * * --quiet          Quiet operation (also -q)
	 * * --traditional    Disable extensions of version 7
	 * * --home=DIR       Use DIR as SWI-Prolog home
	 * * --pldoc[=port]   Start PlDoc server [at port]
	 * * --win_app     Behave as Windows application
	 * @note We implement it by a cross-platform and robust solution, which
	 * can handle the ERROR:
	 * **[FATAL ERROR: Could not find system resources]**
	 * when invokes the function _PL_initialise_, or when Prolog is 
	 * embedded in a C/C++/Java/... application.
	 */
	ArgumatrixPlEngine(int argc, char **argv)
	{   /*
		* A cross-platform and robust solution to handle the ERROR:
		* **[FATAL ERROR: Could not find system resources]**
		* when invokes the function *PL_initialise*, or when Prolog is 
		* embedded in a C/C++/Java/... application.
		**/
	#if defined(__linux) || defined(__APPLE__)
		// use putenv() to put an appropriate path into the environment 
		// before calling PL_initialise().
		putenv("SWI_HOME_DIR=/usr/lib/swi-prolog");
	#elif defined(_WIN32)
		/* On Windows, it suffices to leave libswipl.dll in the installation
		 * tree (i.e., do not copy it elsewhere) and add the bin directory
		 * of the installation tree to %PATH%. We have completed these 
		 * configuration in this project.
		 * 1 C/C++ -> General -> Include file ../swipl/include
		 * 1 Linker -> General -> Additional libraries ../swipl/lib
		 * 1 Linker -> Input -> Additional Dependence ../swipl/lib/libswipl.dll.a
		 */
		// _putenv("SWI_HOME_DIR=/usr/lib/swi-prolog");
	#endif

		if ( !PL_initialise(argc, argv) )
			throw PlError("ArgumatrixPlEngine failed to initialise");
	}

	/**
	 * Method:    Constructor: ArgumatrixPlEngine
	 * @brief Initialize the Prolog engine with some default settings.
	 * @param     char * program
	 * @return    
	 */
	ArgumatrixPlEngine(char *program = (char*)"ArgumatrixPlEngine")
	{ 
		/**
		* Two variables used to initialize the SWI-Prolog Engine.
		* @param plArgc: the number of arguments
		* @param plArgv: a list of arguments
		* Options:
		* -x state         Start from state (must be first)
		* -[LGT]size[KMG]  Specify {Local,Global,Trail} limits
		* -t toplevel      Toplevel goal
		* -g goal          Initialisation goal
		* -f file          User initialisation file
		* -F file          System initialisation file
		* -l file          Script source file
		* -s file          Script source file
		* -p alias=path    Define file search path 'alias'
		* [+/-]tty         Allow tty control
		* -O               Optimised compilation
		* --nosignals      Do not modify any signal handling
		* --nodebug        Omit generation of debug info
		* --quiet          Quiet operation (also -q)
		* --traditional    Disable extensions of version 7
		* --home=DIR       Use DIR as SWI-Prolog home
		* --pldoc[=port]   Start PlDoc server [at port]
		* --win_app     Behave as Windows application
		*/
		int		plArgc = 0;
		char*	plArgv[10];


		plArgv[plArgc++] = program;
		// plArgv[plArgc++] = "--quiet";  // "-q"
		plArgv[plArgc++] = "-g";
		plArgv[plArgc++] = "true";
		// plArgv[plArgc++] = "--home=D:/swipl/"; // SWI_HOME_DIR for linux
		// plArgv[plArgc++] = "-O";
		plArgv[plArgc] = NULL;

		// Using the *placement new* technique to construct an object on a 
		// pre-allocated buffer. This is useful when building a memory pool,
		// a garbage collector or simply when performance and exception 
		// safety are paramount (there's no danger of allocation failure
		// since the memory has already been allocated, and constructing
		// an object on a pre-allocated buffer takes less time): 
		new (this)ArgumatrixPlEngine(plArgc, plArgv);
	}

	~ArgumatrixPlEngine()
	{ 
		PL_cleanup(0);
	}
}; // END class ArgumatrixPlEngine


/*!
 * @class PlTimeQry
 *
 * @brief Create a *time* query which can output the time information.
 *
 * @date May 2016
 */
class PlTimeQry
{
public:
	qid_t qid;

	/**
	 * Method:    Constructor: PlTimeQry
	 * @brief   Create a *time* query where name defines the name of the 
	 * predicate and av the argument vector. The arity is deduced from av.size 
	 * The predicate is located in the Prolog module user.    
	 * @param     const char * name
	 * @param     const PlTermv & av
	 * @note  A Time Query is a query to measure the time of this query. It 
	 * is equivalence to the query: time(Goal), in which the Goal is the real
	 * body of the query and is a compound term PlCompound(name, av). 
	 */
	PlTimeQry(const char *name, const PlTermv &av) { 
		// The default module is user. This module forms the initial 
		// working space of the user.
		new (this)PlTimeQry("user", name, av);		
	}


	/**
	 * Method:    Constructor: PlTimeQry
	 * @brief Same, but performs the time query lookup in the indicated module. 
	 * @param     const char * module
	 * @param     const char * name
	 * @param     const PlTermv & av
	 */
	PlTimeQry(const char *module, const char *name, const PlTermv &av) { 
		atom_t mdl_atom = PL_new_atom(module);
		atom_t time_atom = PL_new_atom("time");
		module_t mdl = PL_new_module(mdl_atom);

		// the Goal
		PlTermv cp_term(PlCompound(name, av));

		predicate_t p = PL_pred(PL_new_functor(time_atom, cp_term.size), mdl);

		PL_unregister_atom(mdl_atom);
		PL_unregister_atom(time_atom);

		//PL_Q_NORMAL
		//Normal operation. The debugger inherits its settings from the environment. If an excep-
		//tion occurs that is not handled in Prolog, a message is printed and the tracer is started to
		//debug the error.4
		//PL_Q_NODEBUG
		//Switch off the debugger while executing the goal. This option is used by many calls
		//to hook-predicates to avoid tracing the hooks.
		//An example is print/1 calling
		//portray/1 from foreign code.
		//PL_Q_CATCH EXCEPTION
		//If an exception is raised while executing the goal, do not report it, but make it available
		//for PL exception().
		//PL_Q_PASS EXCEPTION
		//As PL_Q_CATCH EXCEPTION, but do not invalidate the exception-term while calling
		//PL_close_query(). This option is experimental.
		qid = PL_open_query(mdl, PL_Q_CATCH_EXCEPTION, p, cp_term.a0);
		if ( !qid )
			throw PlResourceError();
	}

	~PlTimeQry() { 
		PL_cut_query(qid);
	}

	/**
	 * Method:    next_solution
	 * FullName:  public  argumatrix::PlTimeQry::next_solution
	 * @brief     Generate the first (next) solution for the given query. 
	 * The return value is TRUE if a solution was found, or FALSE to indicate
	 * the query could not be proven. This function may be called repeatedly
	 * until it fails to generate all solutions to the query. 
	 * @return    int
	 */
	int next_solution();
};  // END class PlTimeQry


__inline 
int PlTimeQry::next_solution() { 
	int rval;
	if ( !(rval = PL_next_solution(qid)) ) { 
		term_t ex;

	if ( (ex = PL_exception(qid)) )
		PlException(ex).cppThrow();
	}
	return rval;
}


/**
* @brief Convert a bitmatrix into a Prolog Boolean matrix.
* @param bitvector& bv
* @param term_t bmtx
* @return no return. *term_t bmtx* is the output parameter.
*/
void bitmatrix2plmatrixTerm(const bitmatrix& bm, term_t bmtx)
{
	term_t h = PL_new_term_ref();
	PL_put_nil(bmtx); //initialize with the empty list
	for(int i = bm.sizeR()-1; i >= 0; --i){
		term_t blst = PL_new_term_ref();

		createBlListTerm(bm[i], blst);
		PL_put_term(h, blst);

		PL_cons_list(bmtx, h, bmtx); //add h in front of the bmtx
	}
}

/**
 * Method:    printBoolListTerm
 * FullName:  public argumatrix::printBoolListTerm
 * @brief     Output a Prolog Boolean list term. 
 * @param     const term_t blst
 * @param     std::ostream & output
 * @return    void
 */
void printBoolListTerm(const term_t blst, std::ostream &output /*= std::cout*/)
{
	//the list ref will get modified so copy it
	term_t tail = PL_copy_term_ref(blst);
	term_t head = PL_new_term_ref();
	int first = 1;
	int x;
	output << LEFT_LIMITER;
	while(PL_get_list(tail, head, tail)){
		if(first){
			first = 0;
		}else{
			output << DELIMITER; // ","
		}
		if ( PL_is_integer(head) ) {
			PL_get_integer(head, &x);
			output << x;
		}
		else { // Not a Number
			output << "NaN";
		}
		
	}
	output << RIGHT_LIMITER;
}


/**
* @brief  Output a Prolog Boolean list term.
* @param bitvector& bv
* @param term_t blst
* @return no return. 
*/
void printBoolMatrixTerm(const term_t bmtx, std::ostream &output /*= std::cout*/)
{
	//the list ref will get modified so copy it
	term_t tail = PL_copy_term_ref(bmtx);
	term_t head = PL_new_term_ref();
	//int first = 1;
	//output << LEFT_LIMITER;
	while(PL_get_list(tail, head, tail)){
		//if(first){
		//	first = 0;
		//}else{
		//	output << DELIMITER; // ","
		//}
		printBoolListTerm(head, output);
		output << endl;
	}
	//output << RIGHT_LIMITER << endl;
}


/**
* @brief Create a bool list term from a vector. There are three values in vecI,
* {0, 1, 2}: 0 -- bool 0, 1 -- bool 1, and 2 -- an unknown variable.
* @param vector& vecI : a vector of integers {0,1,2}.
* @param PlTerm& blst : The output parameter.
* @return no return. *term_t blst* is the output parameter.
*/
void createBlListTerm(const vector<int>& vecI, term_t blst)
{
	term_t tmp = PL_new_term_ref();
	PL_put_nil(blst); //initialize with the empty list
	for(int i = vecI.size() - 1; i >= 0; --i){
		if (2 == vecI[i]) {  // unknown
			PL_put_variable(tmp);
		} else {  /* 0 and 1 */
			PL_put_integer(tmp, vecI[i]);
			// PL_put_bool(tmp, vecI[i]);
		}
		
		PL_cons_list(blst, tmp, blst); //add tmp in front of the list
	}
}


/**
* @brief Create a bool list term from a bitvector. There are two values in bv,
* {0, 1}: 0 -- bool 0, 1 -- bool 1.
* @param bitvector& bv : a vector of bool {0,1}.
* @param PlTerm& blst : The output parameter.
* @return no return. *term_t blst* is the output parameter.
*/
void createBlListTerm(const bitvector& bv, term_t blst)
{
	term_t tmp = PL_new_term_ref();
	PL_put_nil(blst); //initialize with the empty list
	for(int i = bv.size() - 1; i >= 0; --i){ 
		//if (bv[i]) {
		//	PL_put_integer(tmp, 1);
		//} else {  /* 0 and 1 */
		//	PL_put_integer(tmp, 0);
		//}
		PL_put_integer(tmp, bv[i]);
		PL_cons_list(blst, tmp, blst); //add tmp in front of the list
	}
}

} // namespace argumatrix

#endif  //SWI_PROLOG_UTIL_HPP