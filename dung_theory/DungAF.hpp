//=======================================================================
// Copyright 2015 Tsinghua University.
// Authors: Fuan Pu (Pu.Fuan@gmail.com)
// 
// Dung's abstract argumentation framework
//=======================================================================

#ifndef DUNG_ARGUMENTATION_FRAMEWORK_HPP
#define DUNG_ARGUMENTATION_FRAMEWORK_HPP

//boost
#include <boost/config.hpp>
//#include <boost/graph/graph_traits.hpp> 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>
//#include <boost/graph/named_graph.hpp> 
//#include <boost/property_map/property_map.hpp>

#include <iostream>
#include <string>
#include <utility>   
#include <algorithm>   

#include "config/config.hpp"
#include "ArgumentProperty.hpp"
#include "AttackProperty.hpp"
#include "bitmatrix/bitvector.hpp"
#include "bitmatrix/bitmatrix.hpp"


/************************************************************************/
/* Use the label of ArgumentProperty as a key for indexing Arguments    */
/* in a graph. This approach is referred from:                          */
/* [Named vertices](http://svn.boost.org/svn/boost/trunk/libs/graph/test/named_vertices_test.cpp) */
/* and [Name Graph](http://www.boost.org/doc/libs/1_49_0/boost/graph/named_graph.hpp) */ 
/************************************************************************/
namespace boost{
	namespace graph {

		/// Use the label of ArgumentProperty as a key for indexing Arguments in a graph
		template<>
		struct internal_vertex_name<argumatrix::ArgumentProperty>
		{
			typedef multi_index::member<argumatrix::ArgumentProperty, 
				std::string, &argumatrix::ArgumentProperty::label> type;
		};

		/// Allow the graph to build Arguments given only their labels (filling in
		/// the defaults for fields).
		template<>
		struct internal_vertex_constructor<argumatrix::ArgumentProperty>
		{
			typedef vertex_from_name<argumatrix::ArgumentProperty> type;
		};
	} // end namespace boost::graph
}  // end namespace boost



namespace argumatrix{

using namespace std;
using namespace boost;

/**
* An abstract argumentation framework is a directed graph, called Argument Graph.
* Here, we use the boost graph to define the argument graph. 
*   typedef adjacency_list
*   < setS, // edge container
*   listS, // vertex container
*   undirectedS,
*   boost::no_property,  // vertex property
*   boost::no_property // edge property
*   > graph;
*/
typedef boost::adjacency_list<boost::setS, boost::vecS,
	boost::directedS, ArgumentProperty, AttackProperty> ArgumentGraph;

// The definition of the argument (descriptor)
typedef boost::graph_traits<ArgumentGraph>::vertex_descriptor Argument;

// The definition of the attack relation (descriptor)
typedef boost::graph_traits<ArgumentGraph>::edge_descriptor Attack;

// Iterators
typedef boost::graph_traits<ArgumentGraph>::vertex_iterator ArgumentIterator;
typedef boost::graph_traits<ArgumentGraph>::edge_iterator AttackIterator;
typedef boost::graph_traits<ArgumentGraph>::adjacency_iterator AdjacencyIterator;
typedef boost::graph_traits<ArgumentGraph>::out_edge_iterator OutEdgeIterator;

// The index property of the vertices. Using to get the index of an vertex.
typedef boost::property_map<ArgumentGraph, boost::vertex_index_t, size_type>::type IdxMapType;

/************************************************************************/
/* The concepts of Dung's argumentation framework.                       */
/************************************************************************/
class DungAF
{
public:
	DungAF();
	~DungAF();


	// Note that: this method just copy the properties of an argument
	// Add a new Argument to the argument graph and copy ap into property 
	// object for the new argument.
	Argument addArgument(const ArgumentProperty& _argProp);
	Argument addArgument(string _label, string _title="", string _description="");

	// Add an attack to argument graph with an AttackProperty. Here _arg1 attacks _arg2.
	Attack addAttack(const std::string& _arg1_label, const std::string& _arg2_label, const AttackProperty& _attProp = AttackProperty());
	Attack addAttack(const Argument& _arg1, const std::string& _arg2_label, const AttackProperty& _attProp = AttackProperty());
	Attack addAttack(const std::string& _arg1_label, const Argument& _arg2, const AttackProperty& _attProp = AttackProperty());
	Attack addAttack(const Argument& _arg1, const Argument& _arg2, const AttackProperty& _attProp = AttackProperty());
	Attack addAttack(const std::pair<Argument&, Argument&>& _edge, const AttackProperty& _attProp = AttackProperty());


	/**
	* Get the Property of the Argument or the Attack
	* Note that the return value must be const since all external changes
	* on properties are not safe. Let the label of argument a is "A" and assume
	* the returned property is not constant, for instance, if an argument b, 
	* whose label is 'B', then we can modify the label of argument b as "A", which
	* may contradict with the restriction that each argument has a unique label. 
	* The return value of constant may avoid this issue. We also provide a series 
	* of methods to change the property of some argument or attack with safety-check.
	* 
	* @param Argument or Attack, or the unique argument label
	* @return no return. The results are stored in m_extensions.
	*/
	const ArgumentProperty& getArgumentProperty(const Argument& _arg) const;
	const ArgumentProperty& getArgumentProperty(const std::string& _arg_label) const;
	size_type getArgumentIdx(const std::string& _arg_label) const;
	size_type getArgumentIdx(const Argument& _arg) const;
	const AttackProperty& getAttackProperty(const Attack& _atk) const;
	
	// Get a vector of argument labels, vector[i] is the label of the argument indexed by i
	std::vector<std::string> getArgumentLabels() const;

	// Clear the argument graph
	void clear();

	// Update the bit matrix when changing (of Argument and Attack) occurs
	//void update();


	/**
	 * Method:    set2bv
	 * FullName:  argumatrix::DungAF::set2bv
	 * Access:    public 
	 * @param     const set<Argument> & sa
	 * @return    argumatrix::bitvector
	 */
	bitvector set2bv(const set<Argument>& sa) const;
	set<Argument> bv2set(const bitvector& bv) const;
	set<string> bv2label_set(const bitvector& bv) const;
	bitvector labelSet2bv(const set<string>& _ls) const;
	set<bitvector> label_ext2bv_ext(const set< set<string> >& _le) const;

	std::string toString() const;

	bitmatrix getAttackMatrix() const;

	size_type getNumberOfArguments() const;

	void showSet(const set<Argument>& as, std::ostream& os = std::cout) const; 

	/**
	* Output a bitvector. A bitvector represent a set of arguments in bool 
	* vector form. This function will output a bitvector in string, but not 
	* the string with 0 and 1. 
	* @param bitvector _bv: A bitvector.
	* @param ostream& os: The out put stream. The default value is cout.
	* @return no return. The results are stored in m_extensions.
	*/
	void outputBv(const bitvector& _bv, std::ostream& os = std::cout) const; 

	/**
	* Output a set of bitvector set<bitvector>. We know that one bitvector 
	* represent a set of arguments in bool vector form. Therefore, a set of bitvector 
	* may represent a set of a set of arguments. This function will output a set
	* of bitvector in string.
	* @param set< bitvector >& _bv_set: A set of bitvector.
	* @param ostream& os: The out put stream. The default value is cout.
	* @return no return. The results are stored in m_extensions.
	*/
	void outputBvSet(const std::set< bitvector >& _bv_set, std::ostream& os = std::cout) const;

private:
	/* Dung'a abstract argumentation framework can be seen as a direct graph.
	 * */
	ArgumentGraph	m_ag;
	IdxMapType	m_idxMap;
	//bitmatrix	m_bm;  
}; // end DungAF

DungAF::DungAF()
{
	m_idxMap = boost::get(boost::vertex_index, m_ag);
}

DungAF::~DungAF()
{
	
}

__inline
Argument DungAF::addArgument(const ArgumentProperty& _ap)
{
	return boost::add_vertex(_ap, m_ag);
}

__inline
Argument DungAF::addArgument(string _label, string _title/*=""*/, string _description/*=""*/)
{
	//// Approach 1
	//Argument arg = boost::add_vertex( _label, m_ag );
	//ArgumentProperty& _ap = m_ag[arg];
	//_ap.description = _description;
	//_ap.title = _title;

	//return arg;

	// This approach maybe more efficient.
	// Argument arg = boost::add_vertex(m_ag);
	// ArgumentProperty& _ap = m_ag[arg];
	// _ap.setLabel(_attack_label);
	// _ap.setTitle(_title);
	// _ap.setDescription(_description);

	//return arg;

	// Approch 3 
	ArgumentProperty _ap(_label, _title, _description);
	return addArgument(_ap);
}

__inline
Attack DungAF::addAttack(const std::pair<Argument&, Argument&>& _edge,
			             const AttackProperty& _attProp /*= AttackProperty()*/)
{
	Attack atk;
	bool  inserted = false;
	boost::tie(atk, inserted) = boost::add_edge(_edge.first, _edge.second, _attProp, m_ag);

	assert(inserted);  // Was this attack inserted?

	return atk;
}

__inline
Attack DungAF::addAttack(const Argument& _arg1, const std::string& _arg2_label,
						 const AttackProperty& _attProp /*= AttackProperty()*/)
{
	Attack atk;
	bool  inserted = false;
	boost::tie(atk, inserted) = add_edge(_arg1, _arg2_label, _attProp, m_ag);

	assert(inserted);  // Was this attack inserted?

	return atk;
}

__inline
Attack DungAF::addAttack(const std::string& _arg1_label, const Argument& _arg2,
						 const AttackProperty& _attProp /*= AttackProperty()*/)
{
	Attack atk;
	bool  inserted = false;
	boost::tie(atk, inserted) = add_edge(_arg1_label, _arg2, _attProp, m_ag);

	assert(inserted);  // Was this attack inserted?

	return atk;
}

__inline
Attack DungAF::addAttack(const Argument& _arg1, const Argument& _arg2, 
						 const AttackProperty& _attProp /*= AttackProperty()*/)
{
	Attack atk;
	bool  inserted = false;
	boost::tie(atk, inserted) = boost::add_edge(_arg1, _arg2, _attProp, m_ag);

	assert(inserted);  // Was this attack inserted?

	return atk;
}

__inline
Attack DungAF::addAttack(const std::string& _arg1_label, const std::string& _arg2_label, 
						 const AttackProperty& _attProp /*= AttackProperty()*/)
{
	Attack atk;
	bool  inserted = false;
	boost::tie(atk, inserted) = add_edge(_arg1_label, _arg2_label, _attProp, m_ag);

	assert(inserted);  // Was this attack inserted?

	return atk;
}

inline
const ArgumentProperty& DungAF::getArgumentProperty(const Argument& _arg) const
{
	return m_ag[_arg];
}

__inline
const ArgumentProperty& DungAF::getArgumentProperty(const std::string& _arg_label) const
{
	return m_ag[*find_vertex(_arg_label, m_ag)];
}

inline
const AttackProperty& DungAF::getAttackProperty(const Attack& _atk) const
{
	return m_ag[_atk];
}

bitvector DungAF::set2bv(const set<Argument>& sa) const
{
	bitvector bv( boost::num_vertices(m_ag), 0 );

	set<Argument>::iterator sa_itr;
	for (sa_itr = sa.begin(); sa_itr != sa.end(); sa_itr++)	{
		bv.set(m_idxMap[*sa_itr], true);
	}

	return bv;
}

set<Argument> DungAF::bv2set(const bitvector& bv) const
{
	assert(bv.size() == boost::num_vertices(m_ag));
	set<Argument> sa;
	for (size_type i=bv.find_first(); i < bitvector::npos; i=bv.find_next(i)) {
		sa.insert(boost::get(m_idxMap, i));
	}

	return sa;
}

std::string DungAF::toString() const
{
	std::string s("[");

	// arguments to string
	s += "{";
	std::pair<ArgumentIterator, ArgumentIterator> ai = vertices(m_ag);
	if ( ai.first != ai.second )
	{
		ArgumentIterator ait = ai.first;
		s +=  m_ag[*ait].label; 

		for ( ++ait; ait!=ai.second; ++ait) {
			s += ( "," + m_ag[*ait].label); 
		}
	}
	s += "},";

	// Attacks to string
	s += "{";
	std::pair<AttackIterator, AttackIterator> ei = edges(m_ag);
	if( ei.first != ei.second )
	{
		AttackIterator eit = ei.first;
		s += "(" + m_ag[source(*eit, m_ag)].label + ","
			 + m_ag[target(*eit, m_ag)].label + ")";

		for (++eit; eit != ei.second; ++eit) {
			s += ",(" + m_ag[source(*eit, m_ag)].label + ","
				 + m_ag[target(*eit, m_ag)].label + ")";
		}
	}
	s += "}";

	s += "]"; 

	return s;
}

bitmatrix DungAF::getAttackMatrix() const
{
	bitmatrix bm( boost::num_vertices(m_ag) );

	std::pair<AttackIterator, AttackIterator> ei = edges(m_ag);
	for (AttackIterator eit = ei.first; eit != ei.second; ++eit) {
		size_type i = m_idxMap[target(*eit, m_ag)];
		size_type j = m_idxMap[source(*eit, m_ag)];

		bm[i][j] = true;		
	}

	return bm;
}

inline
size_type DungAF::getNumberOfArguments() const
{
	return boost::num_vertices(m_ag);
}

void DungAF::showSet(const set<Argument>& sa, std::ostream& os /*= std::cout*/) const
{
	std::set<Argument>::iterator sa_itr;
	bool is_first = true;

	os << LEFT_LIMITER;
	for (sa_itr = sa.begin(); sa_itr != sa.end(); sa_itr++)
	{
		if (is_first)
		{
			os<< m_ag[*sa_itr].label;
			is_first = false;
		}
		else
		{
			os << DELIMITER << m_ag[*sa_itr].label;
		}
	}

	os << RIGHT_LIMITER << std::endl;
}

void DungAF::outputBv(const bitvector& _bv, std::ostream& os /*= std::cout*/) const
{
	bool first = true;

	os << LEFT_LIMITER;
	for ( size_type i = _bv.find_first(); 
		i != bitvector::npos; 
		i = _bv.find_next(i) )
	{
		if(first){
			first = false;
		}else{
			os << DELIMITER; // ","
		}
		os << m_ag[boost::get(m_idxMap, i)].label;
	}
	os << RIGHT_LIMITER;
}

void DungAF::outputBvSet(const std::set< bitvector >& _bv_set, std::ostream& os /*= std::cout*/) const
{
	os << LEFT_LIMITER;

	bool first = true;

	std::set<bitvector>::iterator _bv_set_itr = _bv_set.begin();	

	for ( ; _bv_set_itr != _bv_set.end(); ++_bv_set_itr) {
		if(first){
			first = false;
		}else{
			os << DELIMITER; // ","
		}
		outputBv(*_bv_set_itr, os);
	}

	os << RIGHT_LIMITER;
}

inline
void DungAF::clear()
{
	m_ag.clear();
}

set<string> DungAF::bv2label_set(const bitvector& _bv) const
{
	set<string> _ss;
	for (size_type i = _bv.find_first(); i != bitvector::npos; i = _bv.find_next(i))
	{
		_ss.insert( m_ag[boost::get(m_idxMap, i)].label );
	}

	return _ss;
}

bitvector DungAF::labelSet2bv(const set<string>& _ls) const
{
	bitvector _bv( getNumberOfArguments(), 0); // bitvector with 0s
	set<string>::iterator _ls_itr;
	for (_ls_itr = _ls.begin(); _ls_itr != _ls.end(); ++_ls_itr)
	{
		_bv[getArgumentIdx(*_ls_itr)] = true;

		// This approach is not safe, and can be solved by boost::optional.
		// _bv[ m_idxMap[*find_vertex(*_ls_itr, m_ag)] ] = true;
	}

	return _bv;
}

set<bitvector> DungAF::label_ext2bv_ext(const set< set<string> >& _le) const
{
	set<bitvector> _sb;
	set< set<string> >::iterator _le_itr;
	for (_le_itr = _le.begin(); _le_itr != _le.end(); ++_le_itr)
	{
		_sb.insert( labelSet2bv( *_le_itr ) );
	}

	return _sb;
}

std::vector<std::string> DungAF::getArgumentLabels() const
{
	vector<string> _label_vector;
	for (size_type i=0; i<getNumberOfArguments(); ++i)
	{
		_label_vector.push_back(m_ag[boost::get(m_idxMap, i)].label);
	}

	return _label_vector;
}

size_type DungAF::getArgumentIdx(const std::string& _arg_label) const
{
	// argumatrix::IdxMapType::npos;
	// boost::vertex_index_t::
	boost::optional<Argument> opt_arg = find_vertex(_arg_label, m_ag);
	if ( opt_arg ) { return m_idxMap[*opt_arg]; }
	else {
		std::cerr << "Argument [" << _arg_label << "] is not in this framework." << endl;
		exit(1);
	}
}

size_type DungAF::getArgumentIdx(const Argument& _arg) const
{
	return m_idxMap[_arg];
}

} // namespace argumatrix



#endif  //DUNG_ARGUMENTATION_FRAMEWORK_HPP