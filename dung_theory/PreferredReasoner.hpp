//=======================================================================
// Copyright 2015 Tsinghua University.
// Authors: Fuan Pu (Pu.Fuan@gmail.com)
// 
// Dung's abstract argumentation framework
//=======================================================================

#ifndef DUNG_PREFERRED_REASONER_HPP
#define DUNG_PREFERRED_REASONER_HPP

#include <iostream>
#include <string>
#include <utility>   
#include <algorithm>   

//boost
#include <boost/graph/graph_traits.hpp> 
#include <boost/graph/adjacency_list.hpp>

#include "config/config.hpp"
#include "ArgumentProperty.hpp"
#include "AttackProperty.hpp"
#include "bitmatrix/bitvector.hpp"
#include "bitmatrix/bitmatrix.hpp"
#include "DungAF.hpp"
#include "Reasoner.hpp"


namespace argumatrix{

using namespace std;

/**
* This reasoner for Dung theories performs inference on the preferred extension.
* Computes the (unique) grounded extension, i.e., the least fixpoint of 
* the characteristic function.
*/
class PreferredReasoner : public Reasoner {
public:
	PreferredReasoner(const DungAF& daf, streambuf* osbuff = std::cout.rdbuf())
		: Reasoner(daf, osbuff) {
		// m_argNum = m_daf.getNumberOfArguments();
		// m_BmAtkMtx = m_daf.getAttackMatrix();
		m_attackedBy = m_BmAtkMtx.transpose();
	}

	/**
	 * Compute all extensions given a specific semantics. Each extension is a set 
	 * of arguments. Here, we use a bitvector to represent an extension. The results
	 * are stored in m_extensions. When a new extension bv is computed, it can be added
	 * to m_extensions by m_extensions.push_back(bv).
	 * @return no return. The results are stored in m_extensions.
	 */
	void computeExtensions();

	/**
	 * Get attackers of arguments in _bv, R^-(S) 
	 * R^-(S) = {x|x attacks some argument in S}.
	 * R^-(S_bv) = D^T*S_bv 
	 * @param _bv the bitvector of a set of arguments S.
	 * @return a set of arguments in bitvector form.
	 */
	bitvector getAttackers(const bitvector& _bv);

private:
	enum SELECT_TYPE { TYPE_A, TYPE_B };
	enum LABELS { BLANK = 0, _IN_, _OUT_, MUST_OUT, UNDEC, Label_Num };

	/**
	 * Before computing the preferred extension, we will preprocess all arguments
	 * by assigning initial labels to some arguments for reducing search space. 
	 * The preprocessing operations contain (1) setting all self-attacking arguments
	 * with OUT label; (2) setting all unattacked arguments with IN label; 
	 * (3) setting all arguments attacked by IN-label arguments with OUT label; 
	 * Lastly, all arguments labeled IN or OUT will be removed from BLANK.
	 */
	void preprocessing(bitvector& _blank, bitvector& _in, bitvector& _out, bitvector& _undec);

	void findPreferredExtensions(bitvector& _blank, bitvector& _in, 
			bitvector& _out, bitvector& _undec, bitvector& _must_out);

	bool lookAhead(bitvector& _blank_new, bitvector& _tmp_must_out);

	pair<size_type, SELECT_TYPE> selectArgument(const bitvector& _blank, const bitvector& _in, 
		const bitvector& _out, const bitvector& _undec, const bitvector& _must_out);

private:
	/**
	 * The transpose of the attack matrix, it provides an effective way to
	 * access all attacked arguments of a given argument. The attacked arguments
	 * of the argument with index i is m_attackedBy[i].	
	 */
	bitmatrix m_attackedBy;

};  // class GroundedReasoner

void PreferredReasoner::computeExtensions()
{
	m_extensions.clear();

	// We using five bitvector to encode the labellings of all arguments. 
	// Each argument is merely labeled with one of the following five labellings.
	bitvector _blank = bitvector::UniversalSet(m_argNum);  // At beginning, all arguments are labeled BLANK
	bitvector _out = bitvector::EmptySet(m_argNum);   // Empty set
	bitvector _in = bitvector::EmptySet(m_argNum);
	bitvector _must_out = bitvector::EmptySet(m_argNum);
	bitvector _undec = bitvector::EmptySet(m_argNum);

	// Preprocessing
	preprocessing(_blank, _in, _out, _undec);

	findPreferredExtensions(_blank, _in, _out, _undec, _must_out);
}

void PreferredReasoner::preprocessing(bitvector& _blank, bitvector& _in, bitvector& _out, bitvector& _undec)
{
	// setting all self-attacking arguments with OUT label
	// _out |= m_attackMatrix.diag();
	_undec |= getSelfAttackingArguments();

	// getting all unattacked arguments by characteristic function
	_in |= characteristic(bitvector::EmptySet(m_argNum));

	// setting all arguments, attacked by IN-label arguments, with OUT label; 
	_out |= getAttacked(_in);

	// Removing IN and OUT from BLANK
	// _blank ^= (_in|_out);
	_blank -= _in;
	_blank -= _out;
	_blank -= _undec;
}

void PreferredReasoner::findPreferredExtensions(bitvector& _blank, 
		bitvector& _in, bitvector& _out, bitvector& _undec, bitvector& _must_out)
{
	size_type i;
	SELECT_TYPE _s_type;

	boost::tie(i, _s_type) = selectArgument(_blank, _in, _out, _undec, _must_out);

	while (i != bitvector::npos)
	{
		bitvector _blank_new(_blank);
		bitvector _out_new(_out);
		bitvector _in_new(_in);
		bitvector _must_out_new(_must_out);
		bitvector _undec_new(_undec);

		_in_new[i] = true; _blank_new[i] = false;  // label i with IN

		_out_new |= m_attackedBy[i];
		_blank_new -= m_attackedBy[i];
		_undec_new -= m_attackedBy[i];
		_must_out_new -= m_attackedBy[i];

		// get argument i's attackers which are labeled by BLANK or UNDEC
		bitvector _tmp_must_out( m_BmAtkMtx[i]&(_blank_new|_undec_new) );
		_must_out_new |= _tmp_must_out;
		_blank_new -= _tmp_must_out;
		_undec_new -= _tmp_must_out;

		//bool forwardCheck = lookAhead(_blank_new, _tmp_must_out);
		bool forwardCheck = lookAhead(_blank_new, _must_out_new);
		if ( forwardCheck )
		{
			findPreferredExtensions(_blank_new, _in_new, _out_new, _undec_new, _must_out_new);
		}
		
		if (_s_type == TYPE_B)
		{
			_undec[i] = true;  _blank[i] = false;
			if ( !lookAhead(_blank, _must_out) )
				return;
		}
		else   // TYPE_A
		{
			if ( !forwardCheck )
				return;

			_blank = _blank_new;
			_in = _in_new;
			_out = _out_new;
			_must_out = _must_out_new;
			_undec = _undec_new;
		}

		boost::tie(i, _s_type) = selectArgument(_blank, _in, _out, _undec, _must_out);
	}

	// if there are no argument labeled MUST_OUT, then inserts the argument 
	// set labeled IN into extensions
	if ( _must_out.is_emptyset() )
	{
		std::set<bitvector>::iterator sa_itr;

		for (sa_itr = m_extensions.begin(); sa_itr != m_extensions.end(); sa_itr++)
		{
			if (_in.is_subset_of( *sa_itr ))
			{
				//cout << "duplicate: " << _in << "is subset of" << *sa_itr << endl;
				return;
			}
		}

		m_extensions.insert( _in );
	}
}

bool PreferredReasoner::lookAhead(bitvector& _blank_new, bitvector& _tmp_must_out)
{
	for (size_type i = _tmp_must_out.find_first(); i != bitvector::npos; i = _tmp_must_out.find_next(i))
	{
		if ( !m_BmAtkMtx[i].intersects(_blank_new) )
			return false;
	}

	return true;
}

pair<size_type, PreferredReasoner::SELECT_TYPE> PreferredReasoner::selectArgument(
	const bitvector& _blank, const bitvector& _in, const bitvector& _out,
	const bitvector& _undec, const bitvector& _must_out) 
{
	size_type i = bitvector::npos;
	for (size_type j = _blank.find_first(); j != bitvector::npos; j = _blank.find_next(j))
	{
		//bool _m_in = true;
		if( m_BmAtkMtx[j].is_subset_of(_out|_must_out) )
		//if ( !m_attackMatrix[j].intersects(_blank|_undec) )
		{
			return make_pair(j, TYPE_A);
		}
		
		if ( i == bitvector::npos) 	{
			i = j;
		} else if (m_attackedBy[j].count()+m_BmAtkMtx[j].count() >
			m_attackedBy[i].count()+m_BmAtkMtx[i].count()) {
			i = j;
		}
	}

	return make_pair(i, TYPE_B);
}

bitvector PreferredReasoner::getAttackers(const bitvector& _bv)
{
	assert( _bv.size() == m_argNum );

	return m_attackedBy * _bv;
}

} // namespace argumatrix



#endif  //DUNG_PREFERRED_REASONER_HPP