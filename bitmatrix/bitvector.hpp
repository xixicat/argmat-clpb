#ifndef BIT_VECTOR_HPP
#define BIT_VECTOR_HPP

#include <assert.h>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <iostream>

#include "config/config.hpp"

#include <boost/dynamic_bitset/dynamic_bitset.hpp>



namespace boost {
	/**
	 * The multiplication of two dynamic_bitsets. 
	 * Assume A=[a_1, a_2, ..., a_n] and B=[b_1, b_2, ..., b_n], the multiplication
	 * of A*B = (a_1*b_1)+(a_2*b_2)+...+(a_n*b_n). This operation is equivalent 
	 * in function to determine whether A intersects with B.
	 * @param two dynamic_bitsets a and b
	 * @return the bool value of A*B.
	 */
	
	template <typename Block, typename Allocator>
	inline bool operator*(const dynamic_bitset<Block, Allocator>& a,
		const dynamic_bitset<Block, Allocator>& b)
	{
		assert(a.size() == b.size());

		/************************************************************************/
		/* Methods one, needing to add friend function in source file           */
		/************************************************************************/
		//typedef typename dynamic_bitset<Block, Allocator>::size_type size_type;

		//for (size_type ii = a.num_blocks(); ii > 0; --ii) {
		//  size_type i = ii-1;
		//  if (a.m_bits[i] & b.m_bits[i])
		//    return true;
		//}
		//return false;
		// 

		/************************************************************************/
		/* Method two, a quite easy way!!!                                      */
		/************************************************************************/

		return a.intersects(b);
	}

} // boost namespace end



namespace argumatrix {  // argumatrix

using namespace boost;
class bitvector: public dynamic_bitset<block_type>
{
public:
	// Constructor
	bitvector(): dynamic_bitset<block_type>() { }
	bitvector(size_type _sz, unsigned long value = 0): dynamic_bitset<block_type>(_sz, value) { }
	bitvector(const std::string& _s): dynamic_bitset<block_type>(_s) { }
	bitvector(const dynamic_bitset<block_type>& _db): dynamic_bitset<block_type>(_db) { } 
	
public:
	/**
	 * The multiplication of two bitvector. 
	 * Assume A=[a_1, a_2, ..., a_n] and B=[b_1, b_2, ..., b_n], the multiplication
	 * of A*B = (a_1*b_1)+(a_2*b_2)+...+(a_n*b_n). This operation is equivalent 
	 * in function to determine whether A intersects with B.
	 * @param two bitvector a and b
	 * @return the bool value of A*B.
	 */
	bool operator*(const bitvector& _bv);

	/**
	 * @brief To decide whether the bitvector of an argument set is empty-set? 
	 * If all entries of this bitvector are 0's, then it is an empty set, and
	 * this function return true, else return false. we must discriminate this
	 * function from the function empty(), which is used to determine the size of
	 * the bitvector is zero. This function is the same as the function none().
	 * @return true if all entries of the bitvector are 0's, otherwise return false.
	 */
	bool is_emptyset();

	/**
	 * @brief To decide whether the bitvector of an argument set is universal? 
	 * If all entries of this bitvector are 1's, then it is an universal set, and
	 * this function return true, else return false. 
	 */
	bool is_universal();

	/**
	 * @brief Create an empty set (all 0's) or a universal set (all 1's) under a given size. 
	 * @param _sz the size of the empty set or the universal set
	 * @return the bitvector of the empty set or the universal set.
	 */
	static bitvector EmptySet(size_type _sz);
	static bitvector UniversalSet(size_type _sz);

	/**
	 * increaser the bit vector with 1. 
	 * @param _sz the size of the empty set or the universal set
	 * @return the bitvector of the empty set or the universal set.
	 */
	bool Increase();
};

__inline
bool bitvector::operator*(const bitvector& _bv)
{
	return intersects(_bv);
}

inline
bool bitvector::is_emptyset()
{
	return !any();
}

__inline
argumatrix::bitvector bitvector::EmptySet(size_type _sz)
{
	return bitvector(_sz, 0);
}


argumatrix::bitvector bitvector::UniversalSet(size_type _sz)
{
	//bitvector universal_bv(_sz, 0);
	//universal_bv.set();  // set all 0's to all 1's
	//return universal_bv;

	return bitvector(_sz, 0).set();
}

bool bitvector::Increase()
{
	return true;
}

__inline 
bool bitvector::is_universal()
{
	return all();
	// return bitvector(this->size(), 0).set() == *this;
}

} // namespace argumatrix
#endif
