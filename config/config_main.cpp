#include <cstdio>
#include <iostream>
#include <fstream>
#include <string.h>
#include "config.hpp"
#include "cptimer.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	cout<< "sizeof(block_type) = "<< sizeof(block_type) << endl;
	cout<< "sizeof(size_type) = "<< sizeof(size_type) << endl;

	uint64_t counter = 10000;

	uint64_t time_begin, time_end;
	time_begin = clock_ns();
	while(counter--);
	time_end = clock_ns();

	cout << "The time is :" << time_end - time_begin << endl;

	system("pause");

	return 0;
}




