#include "CTCPclient.h"

#define localhost ("127.0.0.1")

int main(int argc, char *argv[])
{
	CTCPclient c1(localhost, 5000);

	return 0;
}