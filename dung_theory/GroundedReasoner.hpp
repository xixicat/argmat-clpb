//=======================================================================
// Copyright 2015 Tsinghua University.
// Authors: Fuan Pu (Pu.Fuan@gmail.com)
// 
// Dung's abstract argumentation framework
//=======================================================================

#ifndef DUNG_GROUNDED_REASONER_HPP
#define DUNG_GROUNDED_REASONER_HPP

#include <iostream>
#include <string>
#include <utility>   
#include <algorithm>   
#include <iosfwd>

#include "config/config.hpp"
#include "bitmatrix/bitvector.hpp"
#include "bitmatrix/bitmatrix.hpp"
#include "DungAF.hpp"
#include "Reasoner.hpp"


namespace argumatrix{

using namespace std;

/**
* This reasoner for Dung theories performs inference on the grounded extension.
* Computes the (unique) grounded extension, i.e., the least fixpoint of 
* the characteristic function.
*/
class GroundedReasoner : public Reasoner {
public:
	GroundedReasoner(const DungAF& daf, streambuf* osbuff = std::cout.rdbuf())
		: Reasoner(daf, osbuff) { }

public:
	// Problems
	/**
	* Problem [EE-\f$\textsf{GR}\f$]
	* Print all extensions
	* @param no argument
	* @return no return.
	*/
	void task_EE();

	/**
	* Problem [EC-\f$\textsf{GR}\f$]
	* @brief Given an \f$\textit{AF}=\left< \mathcal{X}, \mathcal{R}\right>\f$ and 
	* an argument \f$s \in \mathcal{X}\f$ (respectively, a set of arguments 
	* \f$S\subseteq \mathcal{X}\f$), enumerate all sets \f$E\subseteq \mathcal{X}\f$
	* such that \f$E \in \mathcal{E}_\textsf{GR}(AF)\f$ and \f$s\in E\f$ (respectively,
	* \f$S \subseteq E\f$).
	* @param set<string>, or a Boolean vector: a set of argument
	* @return no return.
	*/
	void task_EC(const std::set<string>& argset);

	/**
	* Problem [SC-\f$\textsf{GR}\f$]
	* @brief Given an \f$\textit{AF}=\left< \mathcal{X}, \mathcal{R}\right>\f$ and an 
	* argument \f$s \in \mathcal{X}\f$ (respectively, a set of arguments 
	* \f$S\subseteq \mathcal{X}\f$), enumerate some set \f$E\subseteq \mathcal{X}\f$
	* such that \f$E \in \mathcal{E}_\textsf{GR}(AF)\f$ and \f$s\in E\f$ (respectively, 
	* \f$S \subseteq E\f$).
	* @param a set of arguments.
	* @return no return.
	*/
	void task_SC(const std::set<string>& argset);

	/**
	* Problem [SE-\f$\textsf{GR}\f$]
	* @brief Given an \f$\textit{AF}=\left< \mathcal{X}, \mathcal{R}\right>\f$, enumerate
	* some set \f$E\subseteq \mathcal{X}\f$ that are in \f$\mathcal{E}_\textsf{GR}(AF)\f$.
	* @param a set of arguments.
	* @return no return.
	*/
	void task_SE();

	/**
	* Problem [DE-\f$\textsf{GR}\f$]
	* @brief Given an \f$AF = \left<X,R\right>\f$ and a set of arguments \f$S \subseteq X\f$. Decide whether S is
	* a \textsf{GR}-extension of AF, i.e., S \in E_\textsf{GR}(AF).
	* @param a set of arguments.
	* @return true if argset is a \textsf{GR}-extension; otherwise return false.
	* @note For this task, the *set<string>& argset* can be empty, which means
	* to decide whether the empty set is a \f$\textsf{GR}\f$-extension of AF. This indicates
	* that the option -a is not necessary, then the default *set<string>& argset* 
	* is empty.
	*/
	void task_DE(const std::set<string>& argset);

	/**
	* Problem [DN-\f$\textsf{GR}\f$]
	* @brief Given an \f$AF = \left<X,R\right>\f$ and a set of arguments \f$S \subseteq X\f$. Decide whether
	* there exist a non-empty \textsf{GR}-extension for AF
	* @return true if there exist a non-empty \textsf{GR}-extension; otherwise return false.
	*/

	void task_DN();

	/**
	* Problem [DC-\f$\textsf{GR}\f$]
	* @brief Given an \f$AF = \left<X,R\right>\f$ and an argument s \in X (respectively, a set of 
	* arguments \f$S \subseteq X\f$). Decide whether s contained (respectively, S included)
	* in some E \in E_\textsf{GR}(AF) (i.e., credulously justified).
	*/
	void task_DC(const std::set<string>& argset);

	/**
	* Problem [DS-\f$\textsf{GR}\f$]
	* @brief Given an \f$AF = \left<X,R\right>\f$ and an argument s \in X (respectively, a set of 
	* arguments \f$S \subseteq X\f$). Decide whether s contained (respectively, S included)
	* in each E \in E_\textsf{GR}(AF) (i.e., skeptically justified).
	*/
	void task_DS(const std::set<string>& argset);

private:

};  // class GroundedReasoner

__inline
void GroundedReasoner::task_EE()
{
	printGroundedExt();
}

__inline
void GroundedReasoner::task_EC(const std::set<string>& argset)
{
	if(argset.empty()) {
		cerr << "The task DC needs additional argument (see option -a)" << endl;
		exit(1);
	}
	
	// Get the bitvector 
	bitvector bv_argset = m_daf.labelSet2bv(argset);

	bitvector gr_ext = getGroundedExtension();

	// The task EC is to enumerate all extensions that contain the *argset*.
	// The grounded extension is unique. Therefore, if *argset* is included
	// in the grounded extension, then we output the grounded extension; 
	// otherwise, output the emptyset.
	
	m_output << LEFT_LIMITER;  // [
	if (bv_argset.is_subset_of(gr_ext)){
		printLabSet( gr_ext );
	} 
	m_output << RIGHT_LIMITER; // ]
}


__inline
void GroundedReasoner::task_DE(const std::set<string>& argset)
{
	//if(argset.empty()) {
	//	cerr << "The task DC needs additional argument (see option -a)" << endl;
	//	exit(1);
	//}

	// The task DE for the grounded semantics is equivalence to decide whether
	// the *argset* is the grounded extension.
	bitvector bvec = m_daf.labelSet2bv(argset);
	if ( is_grounded(bvec)) {
		m_output << STR_YES << endl;
	} else 	{
		m_output << STR_NO << endl;
	}
}

__inline
void GroundedReasoner::task_DN()
{
	// The task DN for the grounded semantics is equivalence to decide whether
	// the grounded extension is empty.
	bitvector gr_ext = getGroundedExtension();

	if ( gr_ext.is_emptyset() ) {
		m_output << STR_NO << endl;
	} else {
		m_output << STR_YES << endl;
	}
}

__inline
void GroundedReasoner::task_SC(const std::set<string>& argset)
{
	if(argset.empty()) {
		cerr << "The task DC needs additional argument (see option -a)" << endl;
		exit(1);
	}

	// The task SC is to enumerate some extension that contains the *argset*.
	// The grounded extension is unique. Therefore, if *argset* is included
	// in the grounded extension, then we output the grounded extension; 
	// otherwise, output the "NO".

	// Get the bitvector 
	bitvector bv_argset = m_daf.labelSet2bv(argset);

	bitvector gr_ext = getGroundedExtension();

	if (bv_argset.is_subset_of(gr_ext)){
		printLabSet( gr_ext );
	} else {
		m_output << STR_NO << endl;
	}
}

__inline 
void GroundedReasoner::task_SE()
{
	printLabSet( getGroundedExtension() );
}

__inline
void GroundedReasoner::task_DC(const std::set<string>& argset)
{
	if(argset.empty()) {
		cerr << "The task DC/DS needs additional argument (see option -a)" << endl;
		exit(1);
	}

	// Since the grounded extension is unique, hence, the task DC just verify
	// whether the *argset* is included in the grounded extension.
	bitvector bv_argset = m_daf.labelSet2bv(argset);

	bitvector gr_ext = getGroundedExtension();

	if (bv_argset.is_subset_of(gr_ext)){
		m_output << STR_YES << endl;
	} else {
		m_output << STR_NO << endl;
	}
}

__inline
void GroundedReasoner::task_DS(const std::set<string>& argset)
{
	// For the grounded semantics, the task DS is the same as the task DC.
	task_DC(argset);
}


} // namespace argumatrix


#endif  //DUNG_GROUNDED_REASONER_HPP