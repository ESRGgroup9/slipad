#include "CLocalSystem.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	CLocalSystem localsys;

	localsys.run();

	cout << endl << "LocalSys: Exited main()" << endl;
	return 0;
}