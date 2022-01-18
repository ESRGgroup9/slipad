#include "CLocalSystem.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	CLocalSystem localsys;

	localsys.run();

	cout << endl << "Exited main() local system" << endl;
	return 0;
}