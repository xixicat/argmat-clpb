//=======================================================================
// Copyright 2015 Tsinghua University.
// Authors: Fuan Pu (Pu.Fuan@gmail.com)
// 
// Compute Dung's semantics by SWI-Prolog
// -- Can tackle the following computational problems for stable semantics
// 
//=======================================================================

#ifndef PROLOG_CONFLICT_FREE_REASONER_HPP
#define PROLOG_CONFLICT_FREE_REASONER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <utility>   
#include <algorithm>   

// argumatrix
#include "config/config.hpp"
#include "bitmatrix/bitvector.hpp"
#include "bitmatrix/bitmatrix.hpp"
#include "dung_theory/DungAF.hpp"
#include "dung_theory/ArgumentProperty.hpp"
#include "dung_theory/AttackProperty.hpp"

// SWI-prolog
#include <SWI-cpp.h>
#include "swipl-util.hpp"
#include "PlReasoner.hpp"


namespace argumatrix{

using namespace std;

class ConflictfreePlReasoner : public PlReasoner {
public:
	ConflictfreePlReasoner(const DungAF& daf, const string& sm_task = "CF", streambuf* osbuff = std::cout.rdbuf()); 

	/**
	 * Problem [EE-$\sigma$]
	 * Print all extensions (with a vector of integers {0,1,2})
	 * @param no argument
	 * @return no return.
	 */
	void task_EE();
	void task_EX();

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
	void task_EC(const std::set<string>& argset);

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
	void task_SC(const std::set<string>& argset);
	

	/**
	 * Problem [SE-$\sigma$]
	 * @brief Given an $\textit{AF}=\left< \mathcal{X}, \mathcal{R}\right>$, enumerate
	 * some set $E\subseteq \mathcal{X}$ that are in $\mathcal{E}_\sigma(AF)$.
	 * @param a set of arguments.
	 * @return no return.
	 */
	void task_SE();

	/**
	 * Find all extensions and store them in *m_extensions* with the form of
	 * bitvector.
	 * @param no argument.
	 * @return no return. The results are added into *m_extensions*.
	 */
	void findAllExts();


	/**
	 * Problem [DE-$\sigma$]
	 * @brief Given an $AF = \left<X,R\right>$ and a set of arguments 
	 * $S \subseteq X$. Decide whether S is a \sigma-extension of AF, i.e.,
	 * S \in E_\sigma(AF). 
	 * @param a set of arguments.
	 * @return true if argset is a \sigma-extension; otherwise return false.
	 */
	void task_DE(const std::set<string>& argset);

	/**
	 * Problem [DN-$\sigma$]
	 * @brief Given an $AF = \left<X,R\right>$ and a set of arguments $S \subseteq X$. Decide whether
	 * there exist a non-empty \sigma-extension for AF
	 * @return true if there exist a non-empty \sigma-extension; otherwise return false.
	 * @note If an AF does not exist non-empty conflict-free extension iff all its
	 * arguments are self-attacking.
	 */
	void task_DN();

	/**
	 * Problem [DC-$\sigma$]
	 * @brief Given an $AF = \left<X,R\right>$ and an argument s \in X (respectively, a set of 
	 * arguments $S \subseteq X$). Decide whether s contained (respectively, S included)
	 * in some E \in E_\sigma(AF) (i.e., credulously justified).
	 */
	void task_DC(const std::set<string>& argset);
	
	/**
	 * Problem [DS-$\sigma$]
	 * @brief Given an $AF = \left<X,R\right>$ and an argument s \in X (respectively, a set of 
	 * arguments $S \subseteq X$). Decide whether s contained (respectively, S included)
	 * in each E \in E_\sigma(AF) (i.e., skeptically justified).
	 */
	void task_DS(const std::set<string>& argset);
protected:
	string m_predicate;
};


ConflictfreePlReasoner::ConflictfreePlReasoner(
	const DungAF& daf, const string& sm_task /*= "CF"*/, 
	streambuf* osbuff /*= std::cout.rdbuf()*/) :
	PlReasoner(daf, osbuff)
{	
	if (sm_task == "CF") {   // The default predicate
		m_predicate = "conflict_free1";
	} else
	if (sm_task == "CF1") {
		m_predicate = "conflict_free1";
	} else
	if (sm_task == "CF2") {
		m_predicate = "conflict_free2";
	} else
	if (sm_task == "CF3") {
		m_predicate = "conflict_free3";
	} else {
		cerr << "Error: in ConflictfreePlReasoner, unsupported predicate!" << endl;
		cerr << "  The supported options [CF,CF1,CF2,CF3]" << endl;
		exit(1);
	}
}

__inline
void ConflictfreePlReasoner::task_EE()
{
	// For conflict-free semantics, it can not be optimized by initializing
	// the grounded extension.
	PlReasoner::printAllExts(m_predicate);
}

__inline
void ConflictfreePlReasoner::task_EC(const std::set<string>& argset)
{
	if(argset.empty()) {
		cerr << "The task EC needs additional argument (see option -a)" << endl;
		exit(1);
	}

	PlReasoner::printAllExts(m_predicate, labelSet2IntVector(argset) );
}

__inline
void ConflictfreePlReasoner::findAllExts()
{
	PlReasoner::findAllExts(m_predicate);
}

__inline
void ConflictfreePlReasoner::task_DE(const std::set<string>& argset)
{
	//if(argset.empty()) {
	//	cerr << "The task DC needs additional argument (see option -a)" << endl;
	//	exit(1);
	//}

	if ( is_conflict_free(argset)) {
		m_output << STR_YES << endl;
	} else 	{
		m_output << STR_NO << endl;
	}
}


void ConflictfreePlReasoner::task_DN()
{
	//if( verifyNonemptyExt(m_predicate))	{
	//	m_output << STR_YES << endl;
	//}
	//else {
	//	m_output << STR_NO << endl;
	//}
	// If an AF does not exist non-empty conflict-free extension iff all its
	// arguments are self-attacking.
	bitvector self_att_args = getSelfAttackingArguments();
	if (self_att_args.is_universal()) { 
		m_output << STR_NO << endl;
	} else {
		m_output << STR_YES << endl;
	}
}

__inline
void ConflictfreePlReasoner::task_SC(const std::set<string>& argset)
{
	if(argset.empty()) {
		cerr << "The task DC needs additional argument (see option -a)" << endl;
		exit(1);
	}

	// If argset is conflict-free, directly output it.
	if (is_conflict_free(argset)){ 
		printLabSet( argset );
	} else {
		m_output << STR_NO << endl;
	}
}

__inline
void ConflictfreePlReasoner::task_SE()
{
	// The empty set is always conflict-free.
	m_output << LEFT_LIMITER << RIGHT_LIMITER << endl;
}

__inline
void ConflictfreePlReasoner::task_DC(const std::set<string>& argset)
{
	if(argset.empty()) {
		cerr << "The task DC needs additional argument (see option -a)" << endl;
		exit(1);
	}
	// If *argset* is conflict-free, then it is credulously justified.
	if (is_conflict_free(argset)){ 
		m_output << STR_YES << endl;
	} else {
		m_output << STR_NO << endl;
	}
}

__inline
void ConflictfreePlReasoner::task_DS(const std::set<string>& argset)
{
	if(argset.empty()) {
		cerr << "The task DC needs additional argument (see option -a)" << endl;
		exit(1);
	}

	// All non-empty sets are not skeptically justified.
	m_output << STR_NO << endl;
}

inline
void ConflictfreePlReasoner::task_EX()
{
	fetchAllExts(m_predicate);
}


} // namespace argumatrix


#endif  // PROLOG_CONFLICT_FREE_REASONER_HPP