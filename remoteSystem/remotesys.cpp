#include <iostream>
#include "CRemoteSystem.h"
using namespace std;

int main(int argc, char *argv[])
{
	if(argc < 2)
		return 1;

	CRemoteSystem remoteSys(atoi(argv[1]));
	// CRemoteSystem remoteSys;

	remoteSys.run();
	
	cout << endl << "RemoteSys: Exited main()" << endl;
	return 0;
}