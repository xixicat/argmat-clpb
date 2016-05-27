#ifndef BIT_MATRIX_HPP
#define BIT_MATRIX_HPP

#include <assert.h>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <iostream>

#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include "config/config.hpp"
#include "bitvector.hpp"
#include <utility>


namespace argumatrix {  // argumatrix
using namespace std;

class bitmatrix{ 	
 public:
	// constructors
	bitmatrix(size_type num_size);
	bitmatrix(size_type num_rows, size_type num_columns);
	// copy constructor
	bitmatrix(const bitmatrix& _bm);
	bitmatrix(): m_nRow(0), m_nColumn(0) {}  // default constructor 
	
	void show();
	bitvector& operator[](size_type pos);	
	const bitvector& operator[](size_type pos) const { return m_bitData[pos]; }
	//const bitvector& operator[](const size_type pos);	
	bitvector operator*(const bitvector& _bv);
	bitmatrix operator*(const bitmatrix& _bm);
	bitmatrix operator=(const bitmatrix& _bm);

	// set the row bitvector of bitmatrix at pos with _bv
	void setBitvector(const bitvector& _bv, size_type pos);  
	
	// get a copy of the row bitvector of bitmatrix at pos
	bitvector getBitvector(size_type pos);

	bitmatrix transpose() const;
	size_type sizeR() const { return m_nRow; }
	size_type sizeC() const { return m_nColumn; }

	void push_back(bitvector& _bv);

	/**
	 * Get the diagonal elements of a bitmatrix
	 * @return the bitvector of the diagonal elements
	 */
	bitvector diag();

private:
	size_type m_nRow;
	size_type m_nColumn;
	std::vector< bitvector > m_bitData;
};

bitmatrix::bitmatrix(size_type num_rows, size_type num_columns )
	:m_nRow(num_rows),
	 m_nColumn(num_columns)
{
	m_bitData.resize(m_nRow);
	
	for(size_type i=0; i < m_nRow; i++){
		m_bitData[i].resize(m_nColumn, false);
	}
}

bitmatrix::bitmatrix(size_type num_size)
	:m_nRow(num_size),
	 m_nColumn(num_size)
{
	m_bitData.resize(m_nRow);
	
	for(size_type i=0; i < m_nRow; i++){
		m_bitData[i].resize(m_nColumn, false);
	}
}

bitmatrix::bitmatrix(const bitmatrix& _bm)
{
	m_nRow = _bm.m_nRow;
	m_nColumn = _bm.m_nColumn;
	m_bitData = _bm.m_bitData;
}


void bitmatrix::show(){
	for(size_type i=0; i<m_nRow; i++){
		std::cout<< m_bitData[i] << std::endl;
	}
}

bitvector& bitmatrix::operator[](size_type pos) {
	return m_bitData[pos];
}

bitvector bitmatrix::operator*(const bitvector& _bv){
	assert(_bv.size()==m_nColumn);
	bitvector bv1(_bv.size());
	for(size_type i=0; i<m_nColumn; i++){
		bv1[i] = m_bitData[i]*_bv;
	}
	return bv1;
}

bitmatrix bitmatrix::transpose() const {
	assert(m_nColumn>0);
	assert(m_nRow>0);
	
	bitmatrix bm(m_nColumn, m_nRow);
	
	for(size_type i=0; i<m_nColumn; i++){
		for(size_type j=0; j<m_nRow; j++){
			bm[i][j] = m_bitData[j][i];
		} 
	}
	
	return bm;
}

bitmatrix bitmatrix::operator=(const bitmatrix& _bm){
	m_nRow = _bm.m_nRow;
	m_nColumn = _bm.m_nColumn;
	m_bitData = _bm.m_bitData;
	
	return *this;
}

bitmatrix bitmatrix::operator*(const bitmatrix& _bm_r){
	assert( m_nColumn == _bm_r.m_nRow);
	bitmatrix result_bm(m_nRow, _bm_r.m_nColumn);
	bitmatrix _bm_r_t = _bm_r.transpose();
	
	for(size_type i=0; i<m_nRow; i++){
		for(size_type j=0; j<_bm_r_t.m_nRow; j++){
			result_bm[i][j] = (m_bitData[i]*_bm_r_t.m_bitData[j]);
		} 
	}
	
	return result_bm;
}

void bitmatrix::setBitvector(const bitvector& _bv, size_type pos)
{
	assert(pos >=0 && pos < m_nRow);
	assert(m_nColumn == _bv.size());
	
	m_bitData[pos] = _bv;
}

bitvector bitmatrix::getBitvector(size_type pos)
{
	assert(pos >=0 && pos < m_nRow);

	return m_bitData[pos];
}

void bitmatrix::push_back(bitvector& _bv)
{
	// The _bv must none empty, i.e., _bv.size()>0
	assert(!_bv.empty());

	// if the bitmatrix is empty, _bv will be the first row of it
	if (m_nRow == 0){
		m_nColumn = _bv.size();
	}

	assert( _bv.size() == m_nColumn );
	
	m_bitData.push_back(_bv);

	++m_nRow;
}

bitvector bitmatrix::diag()
{
	size_type _sz = min(m_nRow, m_nColumn);
	bitvector _bv( _sz );

	for (size_type i=0; i<_sz; i++)
		_bv[i] = m_bitData[i][i];

	return _bv;
}

} // namespace argumatrix
#endif
