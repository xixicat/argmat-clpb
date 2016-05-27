#include <cstdio>
#include <iostream>
#include <fstream>
#include <string.h>
//#include <boost/dynamic_bitset/dynamic_bitset.hpp>

#include "bitmatrix.hpp"

using namespace std;
using namespace argumatrix;

int main(int argc, char* argv[])
{
	bitvector bv("00001001010");
	cout << bv << endl;
	//// Static bit matrix
	//argumatrix::bitvector bv[10];
	//bv[0].resize(100);
	//bv[1].resize(30, 1);
	//
	//std::cout << "my bitset: " << bv[0].size() << "bits, " << bv[0] << std::endl;
	//std::cout << "my bitset: " << bv[1].size() << "bits, " << bv[1] << std::endl;
	//
	//// Dynamic bit matrix
	//argumatrix::bitvector *bv1;	
	//bv1 = new argumatrix::bitvector[10];
	//bv1[0].resize(100);
	//
	//bv1[1].resize(20, true);
	////bv1[1].set(10, false);
	//
	//bv1[2].resize(20,false);
	////bv1[2].set(15, true);
	//
	//std::cout << "my bitset: " << bv1[0].size() << "bits, " << bv1[0] << std::endl;
	//std::cout << "my bitset: " << bv1[1].size() << "bits, " << bv1[1] << std::endl;
	//std::cout << "my bitset: " << bv1[2].size() << "bits, " << bv1[2] << std::endl;
	//std::cout << "bv1[1][10]: " << bv1[1][10] << std::endl;
	//std::cout << "bv1[1]*bv1[2]: " << (bv1[1]*bv1[2]) << std::endl;

	//// bit matrix test
	//argumatrix::bitmatrix bm(7,5);	
	//bm[0][0] = true;
	//bm[2][3] = 1;
	//bm.show();
	//
	//argumatrix::bitvector v2(5);
	//v2[1]=true;
	//
	//std::cout << "v2: " << v2 << std::endl;
	//std::cout << "bm[1]*bm[2]: " << bm[1]*bm[2] << std::endl;
	//
	//argumatrix::bitvector v3 = bm*v2;
	//
	//std::cout << "bm*v2: " << v3 << std::endl;
	//std::cout << "transpose of bm: " << std::endl;
	//argumatrix::bitmatrix bm1 = bm.transpose();
	//bm1.show();
	//
	//argumatrix::bitmatrix bm3 = bm;
	//bm[3][4] = 1;

	//std::cout << std::endl;
	//bm3.show();

	//std::cout << std::endl;
	//bm.show();
	//
	//std::cout << "bm*bm'*bm*bm'*bm*bm" << std::endl;
	//argumatrix::bitmatrix bm_t = bm.transpose();
	//argumatrix::bitmatrix bm4 = bm*bm_t*bm*bm_t*bm*bm_t;
	//
	//bm4.show();
	//
	////boost::bitmatrix bm2 = bm* (bm.transpose());
	////std::cout << "bm*bm': " << std::endl;
	////bm2.show();
	//
	//argumatrix::bitmatrix bm5(50);
	//bm5[0][1] = 1;
	//bm5[1][0] = 1;
	//bm5[2][3] = 1;
	//bm5[2][4] = 1;
	//
	//bm5.show();
	//
	//argumatrix::bitmatrix bm6 = bm5;
	//for(int i=0; i<2; i++){
	//	bm6 = bm6 * bm5;
	//	
	//	std::cout << "The " << i << "th iteration:" << std::endl;
	//	bm6.show();
	//}
	//
	//argumatrix::bitmatrix bm7(50, 1);
	//bm7[3][0] = 1;
	//
	//std::cout << "The bm7 = " << std::endl;
	//bm7.show();
	//
	//std::cout << "The bm5*bm7 = " << std::endl;
	//argumatrix::bitmatrix bm8 = bm5*bm7;
	//bm8.show();
	//
	//	
	//argumatrix::bitmatrix D(5);
	//D[1][0] = 1; D[1][3] = 1; D[1][4] = 1;
	//D[2][1] = 1;
	//D[3][2] = 1; D[3][3] = 1;
	//D[4][3] = 1;	
	//
	//D.show();
	//
	//argumatrix::bitvector g(5);
	//std::cout << "The initial bool vector: " << g << std::endl;

	//for(int i=0; i<1000; i++){
	//	argumatrix::bitvector g_bar = ~(D*(~(D*g)));
	//	
	//	std::cout << "The (" << i << ")th result: " << g_bar << std::endl;
	//	
	//	if(g_bar==g)
	//	{
	//		break;
	//	}
	//	g = g_bar;	
	//}

	system("pause");

	return 0;
}




