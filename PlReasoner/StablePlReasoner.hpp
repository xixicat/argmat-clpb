//=======================================================================
// Copyright 2015 Tsinghua University.
// Authors: Fuan Pu (Pu.Fuan@gmail.com)
// 
// Compute Dung's semantics by SWI-Prolog
// -- Can tackle the following computational problems for stable semantics
// 
//=======================================================================

#ifndef PROLOG_STABLE_REASONER_HPP
#define PROLOG_STABLE_REASONER_HPP

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

class StablePlReasoner : public PlReasoner {
public:
	StablePlReasoner(const DungAF& daf, const string& sm_task = "ST", streambuf* osbuff = std::cout.rdbuf()); 

	/**
	 * Problem [EE-\f$\sigma\f$]
	 * Print all extensions (with a vector of integers {0,1,2})
	 * @param no argument
	 * @return no return.
	 */
	void task_EE();
	void task_EX();

	/**
	 * Problem [EC-\f$\sigma\f$]
	 * @brief Given an \f$\textit{AF}=\left< \mathcal{X}, \mathcal{R}\right>\f$ and 
	 * an argument \f$s \in \mathcal{X}\f$ (respectively, a set of arguments 
	 * \f$S\subseteq \mathcal{X}\f$), enumerate all sets \f$E\subseteq \mathcal{X}\f$
	 * such that \f$E \in \mathcal{E}_\sigma(AF)\f$ and \f$s\in E\f$ (respectively,
	 * \f$S \subseteq E\f$).
	 * @param set<string>, or a Boolean vector: a set of argument
	 * @return no return.
	 */
	void task_EC(const std::set<string>& argset);

	/**
	 * Problem [SC-\f$\sigma\f$]
	 * @brief Given an \f$\textit{AF}=\left< \mathcal{X}, \mathcal{R}\right>\f$ and an 
	 * argument \f$s \in \mathcal{X}\f$ (respectively, a set of arguments 
	 * \f$S\subseteq \mathcal{X}\f$), enumerate some set \f$E\subseteq \mathcal{X}\f$
	 * such that \f$E \in \mathcal{E}_\sigma(AF)\f$ and \f$s\in E\f$ (respectively, 
	 * \f$S \subseteq E\f$).
	 * @param a set of arguments.
	 * @return no return.
	 */
	void task_SC(const std::set<string>& argset);
	

	/**
	 * Problem [SE-\f$\sigma\f$]
	 * @brief Given an \f$\textit{AF}=\left< \mathcal{X}, \mathcal{R}\right>\f$, enumerate
	 * some set \f$E\subseteq \mathcal{X}\f$ that are in \f$\mathcal{E}_\sigma(AF)\f$.
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
	 * Problem [DE-\f$\sigma\f$]
	 * @brief Given an \f$AF = \left<X,R\right>\f$ and a set of arguments \f$S \subseteq X\f$. Decide whether S is
	 * a \f$\sigma\f$-extension of \f$AF\f$, i.e., \f$S \in E_\sigma(AF)\f$.
	 * @param a set of arguments.
	 * @return true if argset is a \sigma-extension; otherwise return false.
	 */
	void task_DE(const std::set<string>& argset);

	/**
	 * Problem [DN-\f$\sigma\f$]
	 * @brief Given an \f$AF = \left<X,R\right>\f$ and a set of arguments \f$S \subseteq X\f$. Decide whether
	 * there exist a non-empty \sigma-extension for AF
	 * @return true if there exist a non-empty \f$\sigma\f$-extension; otherwise return false.
	 */
	void task_DN();

	/**
	 * Problem [DC-\f$\sigma\f$]
	 * @brief Given an \f$AF = \left<X,R\right>\f$ and an argument \f$s \in X\f$ (respectively, a set of 
	 * arguments \f$S \subseteq X\f$). Decide whether \$s\$ contained (respectively, \f$S\f$ included)
	 * in some \f$E \in E_\sigma(AF)\f$ (i.e., credulously justified).
	 */
	void task_DC(const std::set<string>& argset);
	
	/**
	 * Problem [DS-\f$\sigma\f$]
	 * @brief Given an \f$AF = \left<X,R\right>\f$ and an argument s \in X (respectively, a set of 
	 * arguments \f$S \subseteq X\f$). Decide whether s contained (respectively, S included)
	 * in each \f$E \in E_\sigma(AF)\f$ (i.e., skeptically justified).
	 */
	void task_DS(const std::set<string>& argset);
protected:
	string m_predicate;
};


StablePlReasoner::StablePlReasoner(
	const DungAF& daf, const string& sm_task /*= "CO"*/, 
	streambuf* osbuff /*= std::cout.rdbuf()*/) :
	PlReasoner(daf, osbuff)
{	
	if (sm_task == "ST") {   // The default predicate
		m_predicate = "stable2";
	} else
	if (sm_task == "ST1") {
		m_predicate = "stable1";
	} else
	if (sm_task == "ST2") {
		m_predicate = "stable2";
	} else {
		cerr << "Error: in StablePlReasoner, unsupported predicate!" << endl;
		cerr << "  The supported options [ST,ST1,ST2]" << endl;
		exit(1);
	}
}

__inline
void StablePlReasoner::task_EE()
{
	PlReasoner::printAllExts(m_predicate, getGroundedIntVector());
	// PlReasoner::printAllExts(m_predicate);
}

__inline
void StablePlReasoner::task_EC(const std::set<string>& argset)
{
	if(argset.empty()) {
		cerr << "The task DC needs additional argument (see option -a)" << endl;
		exit(1);
	}

	PlReasoner::printAllExts(m_predicate, labelSet2IntVector(argset) );
}

__inline
void StablePlReasoner::findAllExts()
{
	PlReasoner::findAllExts(m_predicate);
}

__inline
void StablePlReasoner::task_DE(const std::set<string>& argset)
{
	//if(argset.empty()) {
	//	cerr << "The task DC needs additional argument (see option -a)" << endl;
	//	exit(1);
	//}

	bitvector bvec = m_daf.labelSet2bv(argset);
	if ( is_stable(bvec)) {
		m_output << STR_YES << endl;
	} else 	{
		m_output << STR_NO << endl;
	}
}


void StablePlReasoner::task_DN()
{
	if( verifyNonemptyExt(m_predicate))	{
		m_output << STR_YES << endl;
	}
	else {
		m_output << STR_NO << endl;
	}
}

__inline
void StablePlReasoner::task_SC(const std::set<string>& argset)
{
	if(argset.empty()) {
		cerr << "The task DC needs additional argument (see option -a)" << endl;
		exit(1);
	}

	PlReasoner::printSomeExt(m_predicate, labelSet2IntVector(argset) );
}

__inline
void StablePlReasoner::task_SE()
{
	PlReasoner::printSomeExt(m_predicate, getGroundedIntVector() );
}

__inline
void StablePlReasoner::task_DC(const std::set<string>& argset)
{
	if(argset.empty()) {
		cerr << "The task DC needs additional argument (see option -a)" << endl;
		exit(1);
	}

	if ( isCredulouslyJustified(m_predicate, argset) ) {
		m_output << STR_YES << endl;
	} else {
		m_output << STR_NO << endl;
	}
}

__inline
void StablePlReasoner::task_DS(const std::set<string>& argset)
{
	if(argset.empty()) {
		cerr << "The task DC needs additional argument (see option -a)" << endl;
		exit(1);
	}

	if ( isSkepticallyJustified(m_predicate, argset) ) {
		m_output << STR_YES << endl;
	} else {
		m_output << STR_NO << endl;
	}
}

inline
void StablePlReasoner::task_EX()
{
	fetchAllExts(m_predicate);
}


} // namespace argumatrix


#endif  // PROLOG_STABLE_REASONER_HPP