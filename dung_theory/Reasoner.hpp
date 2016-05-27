//=======================================================================
// Copyright 2015 Tsinghua University.
// Authors: Fuan Pu (Pu.Fuan@gmail.com)
// 
// Dung's abstract argumentation framework
//=======================================================================

#ifndef DUNG_REASONER_HPP
#define DUNG_REASONER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <utility>   
#include <algorithm>   
#include <iosfwd>

//boost
#include <boost/graph/graph_traits.hpp> 
#include <boost/graph/adjacency_list.hpp>

#include "config/config.hpp"
#include "ArgumentProperty.hpp"
#include "AttackProperty.hpp"
#include "bitmatrix/bitvector.hpp"
#include "bitmatrix/bitmatrix.hpp"
#include "DungAF.hpp"


namespace argumatrix{

using namespace std;

class Reasoner {
public:
	Reasoner(const DungAF& daf, streambuf* osbuff = std::cout.rdbuf());

	/**
	 * @brief Get the attacked arguments by arguments in _bv, \f$R^+(S)\f$ 
	 * \f$R^+(S) = {x|x is attacked by S}\f$.
	 * \f$R^+(S_bv) = D*S_bv\f$
	 * @param _bv the bitvector with respect to the set \f$S\f$.
	 * @return a set of arguments in bitvector form.
	 */
	bitvector getAttacked(const bitvector& _bv);
	//bitvector R_plus(const bitvector& _bv);	

	/**
	 * @brief The characteristic function of an abstract argumentation framework: 
	 * \f$F_{AF}(S) = {A|A is acceptable wrt. S}\f$.
	 * F_AF(S_bv) = not{ R^+[ not( R^+(S_bv) ) ] }
	 * @param extension an extension (a set of arguments), the default is empty set.
	 * @return a set of arguments in bitvector form.
	 */
	bitvector characteristic(const bitvector& _bv);

	/**
	 * @brief The characteristic function of an abstract argumentation framework with
	 * the initialization of empty set
	 * @return a set of arguments in bitvector form.
	 */
	bitvector characteristic();

	/**
	 * @brief The neutrality function of an abstract argumentation framework: 
	 * N_AF(S) = {A| All arguments that not attacked by S}.
	 * N_AF(S_bv) = not( R^+(S_bv) )
	 * @param extension an extension (a set of arguments).
	 * @return a set of arguments in bitvector form.
	 */
	bitvector neutrality(const bitvector& _bv);

	/**
	 * @brief Argument set S is conflict-free? 
	 * S is said to be conflict-free iff for any two arguments a,b in S such
	 * that a does not attack b
	 * @param extension an extension (a set of arguments).
	 * @return true if S is conflict-free.
	 */
	bool is_conflict_free(const bitvector& _bv);
	bool is_conflict_free(const set<string>& argset);

	/**
	 * @brief Argument set A is acceptable w.r.t S? Alternately, S defends A? 
	 * A can be an argument or an argument set. 
	 * S defends argument (set) A iff for any argument x in X, if attacks A
	 * (or a in A) then there is an argument y in S such that y attacks x.
	 * @param S: a set of arguments.
	 * @param A: an argument or an argument set
	 * @return true if S defends A.
	 */
	bool is_acceptable(const bitvector& S, const bitvector& A);

	/**
	 * @brief Argument a is self-attacking iff it attacks itself.
	 * @param idx: the index of the argument w.r.t the attack matrix
	 * @return true if S defends A.
	 */
	bool is_self_attacking(size_type idx);

	/**
	 * @brief To decide whether a set of arguments (with the form of bitvector) is 
	 * an admissible extension.
	 * $S$ is an admissible extension iff $S \subseteq F(S) \cap N(S)$.
	 * @return true if _bv is admissible; otherwise returns false.
	 */
	bool is_admissible(const bitvector& _bv);


	/**
	 * @brief To decide whether a set of arguments (with the form of bitvector) is 
	 * a complete extension.
	 * $S$ is a complete extension iff $S == F(S) \cap N(S)$.
	 * @return true if _bv is complete; otherwise returns false.
	 */
	bool is_complete(const bitvector& _bv);

	/**
	 * @brief To decide whether a set of arguments (with the form of bitvector) is 
	 * a stable extension.
	 * $S$ is a stable extension iff $S == N(S)$.
	 * @return true if _bv is stable; otherwise returns false.
	 */
	bool is_stable(const bitvector& _bv);

	/**
	 * @brief To decide whether a set of arguments (with the form of bitvector) is 
	 * a grounded extension. $S$ is a grounded extension iff it is the least
	 * fixed point of the characteristic function F.
	 * @return true if _bv is grounded; otherwise returns false.
	 */
	bool is_grounded(const bitvector& _bv);

	/**
	 * @brief Get all arguments which are self-attacking. Obviously, if an argument 
	 * i attacks itself, the entry[i][i] of the attack matrix must be 1 (true).
	 * Therefore to get the set of self-attacking arguments is to get the diagonal
	 * elements of the attack matrix.
	 * @return a set of arguments in bitvector form, if bitvector[i] = true 
	 * representing the argument (with index) i is a self-attacking argument,
	 * otherwise is not.
	 */
	bitvector getSelfAttackingArguments();

	/**
	 * @brief Get all extensions given a specific semantics in format of bitvector. 
	 * Each extension is a set of arguments which can "survive the conflict
	 * together". Here, we use a bitvector to represent an extension. 
	 * Therefore, all extensions are formed an set of bit vectors, i.e., 
	 * set<bitvector>. Each bitvector in set is an extension w.r.t. 
	 * a given semantics. The computed extensions are stored in m_extensions, 
	 * therefore, to get all extensions, it must first invoke the function 
	 * computeExtension() 
	 * @return a set of bitvector, i.e., set<bitvector>. 
	 */
	const set<bitvector>& getBvExtensions();


	/**
	 * @brief Get a vector of integers {0, 1, 2}: 2 -- Unknown, 1 -- in grounded
	 * extension, and 0 -- attacked by the grounded extension.
	 * @return a vector of integers {0, 1, 2}.
	 */
	vector<int> getGroundedIntVector();

	/**
	 * @brief Redirect the output stream to streambuf* strbuf or ostream& os. If strbuf = cout.rdbuf(),
	 * then the output is standard output. It can also redirect the output to 
	 * a file by the following codes:
	 * ~~~~~{.cpp}
	 *   ofstream ofile("output.txt");
	 *   streambuf* oldsb = xxx.setOutput(ofile.rdbuf()); 
	 * ~~~~~
	 * or
	 * ~~~~~{.cpp}
	 *   streambuf* oldsb = xxx.setOutput(ofile); 
	 * ~~~~~
	 * @param strbuf is a streambuf pointer.
	 * @return return the old streambuf, which can be used to redirect the old
	 * streambuf.
	 */
	streambuf* setOutput(streambuf* strbuf);


	/**
	 * Method:    setOutput
	 * FullName:  public  argumatrix::Reasoner::setOutput
	 * @see       streambuf* setOutput(streambuf* strbuf = std::cout.rdbuf());
	 * @param     ostream & os
	 * @return    streambuf*
	 */
	streambuf* setOutput(ostream& os = std::cout);

	/**
	 * @brief Print an extension with the form of bitvector. Assume the bitvector 
	 * is [0, 1, 0, 1], the arguments corresponding to entry 1 will
	 * be print. The member *m_output* will determine where to print.
	 * @param bitvector& bv_ext : The bv_ext is a bitvector, which represents
	 * an extension.
	 * @return no return. 
	 * @see [setOutput]
	 */
	void printLabSet(const bitvector& bv_ext);

	/**
	 * @brief Print a set of arguments. The member *m_output* will determine
	 * where to print.
	 * @param const std::set<string>& labset
	 * @return no return. 
	 * @see setOutput(streambuf* strbuf = std::cout.rdbuf());
	 */
	void printLabSet(const std::set<string>& labset);

	/**
	 * @brief Output all extensions given a specific semantics with an ostream.  
	 * Each extension is a set of arguments which can "survive the conflict
	 * together". Here, we return a string to represent all
	 * extensions. For example, if there are two extensions, 
	 * [a,b] and [b, d], for some problem w.r.t. a given semantics, then we 
	 * return string "[[a,b],[d,c]]". If extensions is not existing, the string
	 * "[]" will return.
	 * @param ostream& os = std::cout, output the resluts into the ostream os. 
	 * @return a set of bitvector, i.e., set<bitvector>. 
	 */
	void printBvExts();

	/**
	 * @brief Get the grounded extension
	 * @return a set of arguments in bitvector form.
	 */
	bitvector getGroundedExtension();

	
	/**
	 * @brief Print the grounded extension
	 * @return no return. 
	 */
	void printGroundedExt();

	/**
	 * @brief Convert a set of argument (string) labels to an integer vector of {0,1,2},
	 * All arguments in these set, the indices is 1; otherwise 2 (representing unknown)
	 * @return no return. 
	 */
	vector<int> labelSet2IntVector(const std::set<string>& label_set);

public:
	/**
	 * @brief Problem [EE-$\sigma$]
	 * Print all extensions
	 * @param no argument
	 * @return no return.
	 */
	virtual void task_EE() { cerr << "Unimplemented!" << endl; }
	virtual void task_EX() { cerr << "Unimplemented!" << endl; }

	/**
	 * @brief Problem [EC-$\sigma$]
	 * Given an $\textit{AF}=\left< \mathcal{X}, \mathcal{R}\right>$ and 
	 * an argument $s \in \mathcal{X}$ (respectively, a set of arguments 
	 * $S\subseteq \mathcal{X}$), enumerate all sets $E\subseteq \mathcal{X}$
	 * such that $E \in \mathcal{E}_\sigma(AF)$ and $s\in E$ (respectively,
	 * $S \subseteq E$).
	 * @param set<string>, or a Boolean vector: a set of argument
	 * @return no return.
	 */
	virtual void task_EC(const std::set<string>& argset)
	{ cerr << "Unimplemented!" << endl; }

	/**
	 * @brief Problem [SC-$\sigma$]
	 * Given an $\textit{AF}=\left< \mathcal{X}, \mathcal{R}\right>$ and an 
	 * argument $s \in \mathcal{X}$ (respectively, a set of arguments 
	 * $S\subseteq \mathcal{X}$), enumerate some set $E\subseteq \mathcal{X}$
	 * such that $E \in \mathcal{E}_\sigma(AF)$ and $s\in E$ (respectively, 
	 * $S \subseteq E$).
	 * @param a set of arguments.
	 * @return no return.
	 */
	virtual void task_SC(const std::set<string>& argset) 
	{ cerr << "Unimplemented!" << endl; }

	/**
	 * @brief Problem [SE-$\sigma$]
	 * Given an $\textit{AF}=\left< \mathcal{X}, \mathcal{R}\right>$, enumerate
	 * some set $E\subseteq \mathcal{X}$ that are in $\mathcal{E}_\sigma(AF)$.
	 * @param a set of arguments.
	 * @return no return.
	 */
	virtual void task_SE() { cerr << "Unimplemented!" << endl; }

	/**
	 * @brief Problem [DE-$\sigma$]
	 * Given an $AF = \left<X,R\right>$ and a set of arguments $S \subseteq X$. Decide whether S is
	 * a \sigma-extension of AF, i.e., S \in E_\sigma(AF).
	 * @param a set of arguments.
	 * @return true if argset is a \sigma-extension; otherwise return false.
	 * @note For this task, the *set<string>& argset* can be empty, which means
	 * to decide whether the empty set is a $\sigma$-extension of AF. This indicates
	 * that the option -a is not necessary, then the default *set<string>& argset* 
	 * is empty.
	 */
	virtual void task_DE(const std::set<string>& argset)
	{ cerr << "Unimplemented!" << endl; }

	/**
	 * @brief Problem [DN-$\sigma$]
	 * Given an $AF = \left<X,R\right>$ and a set of arguments $S \subseteq X$. Decide whether
	 * there exist a non-empty \sigma-extension for AF
	 * @return true if there exist a non-empty \sigma-extension; otherwise return false.
	 */

	virtual void task_DN() { cerr << "Unimplemented!" << endl; }

	/**
	 * @brief Problem [DC-$\sigma$]
	 * Given an $AF = \left<X,R\right>$ and an argument s \in X (respectively, a set of 
	 * arguments $S \subseteq X$). Decide whether s contained (respectively, S included)
	 * in some E \in E_\sigma(AF) (i.e., credulously justified).
	 */
	virtual void task_DC(const std::set<string>& argset) 
	{ cerr << "Unimplemented!" << endl; }

	/**
	 * @brief Problem [DS-$\sigma$]
	 * Given an $AF = \left<X,R\right>$ and an argument s \in X (respectively, a set of 
	 * arguments $S \subseteq X$). Decide whether s contained (respectively, S included)
	 * in each E \in E_\sigma(AF) (i.e., skeptically justified).
	 */
	virtual void task_DS(const std::set<string>& argset) 
	{ cerr << "Unimplemented!" << endl; }

protected:
	bitmatrix m_BmAtkMtx;  /**< The bitmatrix of the Dung Abstract argumentation framework. We can 
	* access all attackers of an argument. The attackers of the argument 
	* with index i is m_BmAtkMtx[i].
	*/

	size_type m_argNum; /**< The number of arguments */


	const DungAF&	  m_daf;  /**< Dung's abstract argumentation framework */

	set< bitvector > m_extensions;

	vector< std::string > m_argLabels; /**< Dung's abstract argumentation framework */

	std::ostream m_output; /**< Where to output */
};

Reasoner::Reasoner(const DungAF& daf, streambuf* osbuff /*= std::cout.rdbuf()*/):
	m_daf(daf), m_output(osbuff)
{ 
	m_argNum = m_daf.getNumberOfArguments();
	m_BmAtkMtx = m_daf.getAttackMatrix();
	m_argLabels = m_daf.getArgumentLabels();
}

__inline
argumatrix::bitvector Reasoner::getAttacked(const bitvector& _bv)
{
	//assert( _bv.size() == m_argNum );

	return m_BmAtkMtx * _bv;
}

__inline
argumatrix::bitvector Reasoner::characteristic(const bitvector& _bv)
{
	// return ~(m_BmAtkMtx * (~(m_BmAtkMtx * _bv)));
	return neutrality(neutrality(_bv));
}

__inline
argumatrix::bitvector Reasoner::characteristic()
{
	return characteristic(bitvector::EmptySet(m_argNum));
}

__inline
bool Reasoner::is_conflict_free(const bitvector& _bv)
{
	// S is conflict-free iff $S \intersect R^+(S) = \emptyset$.
	return !_bv.intersects( getAttacked(_bv) );
}

__inline
bool Reasoner::is_conflict_free(const set<string>& argset)
{
	return is_conflict_free(m_daf.labelSet2bv(argset));
}

__inline
bool Reasoner::is_acceptable(const bitvector& S, const bitvector& A)
{
	return A.is_subset_of( characteristic(S) );
}

__inline
const set<bitvector>& Reasoner::getBvExtensions()
{
	return m_extensions;
}

__inline
bool Reasoner::is_self_attacking(size_type idx)
{
	// assert(idx < m_argNum);

	return m_BmAtkMtx[idx][idx];
}

__inline
argumatrix::bitvector Reasoner::getSelfAttackingArguments()
{
	//bitvector _bv(m_argNum, false);
	//for (size_type i=0; i<m_argNum; i++)
	//{
	//	_bv[i] = m_BmAtkMtx[i][i];
	//}

	//return _bv;

	return m_BmAtkMtx.diag();
}

__inline
void Reasoner::printBvExts()
{
	m_daf.outputBvSet(m_extensions, m_output);
}

void Reasoner::printLabSet(const bitvector& bv_ext)
{
	bool first = true;

	m_output << LEFT_LIMITER;
	for ( size_type i = bv_ext.find_first(); 
		i != bitvector::npos; 
		i = bv_ext.find_next(i) )
	{
		if(first){
			first = false;
		}else{
			m_output << DELIMITER; // ","
		}
		m_output << m_argLabels[i];
	}
	m_output << RIGHT_LIMITER;

	// m_daf.outputBv(bv_ext, m_output);
}

void Reasoner::printLabSet(const std::set<string>& labset)
{
	bool first = true;

	m_output << LEFT_LIMITER;
	set<string>::iterator itr = labset.begin();
	for ( ; itr != labset.end(); ++itr )
	{
		if(first){
			first = false;
		}else{
			m_output << DELIMITER; // ","
		}
		m_output << *itr;
	}
	m_output << RIGHT_LIMITER;
}

__inline
streambuf* Reasoner::setOutput(streambuf* strbuf)
{
	return m_output.rdbuf(strbuf);
}

__inline
streambuf* Reasoner::setOutput(ostream& os /*= std::cout*/)
{
	return m_output.rdbuf(os.rdbuf());
}

argumatrix::bitvector Reasoner::getGroundedExtension()
{
	bitvector _bv = bitvector::EmptySet(m_argNum);
	bitvector _bv_next = characteristic(_bv);

	while (_bv != _bv_next)
	{
		_bv = _bv_next;
		_bv_next = characteristic(_bv);
	}

	return _bv_next;
}


vector<int> Reasoner::getGroundedIntVector()
{
	// create m_argNum length vector with initial value 2 -- Unknown
	vector<int> vecii(m_argNum, 2);

	bitvector gr_ext = bitvector::EmptySet(m_argNum);
	bitvector gr_out;
	bitvector _bv_last;
	do {
		_bv_last = gr_ext;
		//gr_out = (m_BmAtkMtx * _bv_last); // S =  R^+(X)
		//gr_ext = ~(m_BmAtkMtx * (~gr_out)); // G = ~R^+(~S)
		gr_out = getAttacked( _bv_last ); // S =  R^+(X)
		gr_ext = neutrality( ~gr_out ); // G = ~R^+(~S)
	} while (gr_ext != _bv_last);

	// cout << "gr_ext" << gr_ext << endl;

	for(size_type i=0; i<m_argNum; ++i)
	{
		if( gr_ext[i] ) { vecii[i] = 1; }
		else if ( gr_out[i] ) { vecii[i] = 0; }
	}

	return vecii;
}

vector<int> Reasoner::labelSet2IntVector(const std::set<string>& label_set)
{
	vector<int> vecI( m_argNum, 2 );
	set<string>::iterator _ls_itr;
	for (_ls_itr = label_set.begin(); _ls_itr != label_set.end(); ++_ls_itr)
	{
		vecI[ m_daf.getArgumentIdx( *_ls_itr ) ] = 1;
	}

	return vecI;
}

__inline
void Reasoner::printGroundedExt()
{
	m_output << LEFT_LIMITER;  // [
	printLabSet( getGroundedExtension() );
	m_output << RIGHT_LIMITER; // ]
}

__inline
argumatrix::bitvector Reasoner::neutrality(const bitvector& _bv)
{
	return ~getAttacked(_bv);
}

__inline
bool Reasoner::is_admissible(const bitvector& _bv)
{
	// $S$ is an admissible extension iff $S \subseteq F(S) \cap N(S)$.
	bitvector neu_s = neutrality(_bv);
	bitvector f_s = neutrality( neu_s );
	f_s &= neu_s;

	return _bv.is_subset_of(f_s);
}

__inline
bool Reasoner::is_complete(const bitvector& _bv)
{
	// $S$ is a complete extension iff $S == F(S) \cap N(S)$.
	bitvector neu_s = neutrality(_bv);
	bitvector f_s = neutrality( neu_s );
	f_s &= neu_s;

	return _bv == f_s;
}

__inline
bool Reasoner::is_stable(const bitvector& _bv)
{
	// $S$ is a stable extension iff $S == N(S)$.
	bitvector neu_s = neutrality(_bv);

	return _bv == neu_s;
}

__inline
bool Reasoner::is_grounded(const bitvector& _bv)
{
	bitvector gr_ext = getGroundedExtension();

	return (_bv == gr_ext);
}

} // namespace argumatrix



#endif  //DUNG_REASONER_HPP