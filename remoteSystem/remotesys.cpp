#include <iostream>
#include "CRemoteSystem.h"
using namespace std;

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		cout << "Usage: " << argv[0] << " <portNum>" << endl;
		return 1;
	}

	CRemoteSystem remoteSys(atoi(argv[1]));

	remoteSys.run();
	
	cout << endl << "RemoteSys: Exited main()" << endl;
	return 0;
}