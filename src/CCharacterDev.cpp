#include "CCharacterDev.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "utils.h"
using namespace std;

#define IOCTL_PID 1

CCharacterDev::CCharacterDev(string devName)
{
	pid_t pid;
	devStr = "/dev/";

	devStr.append(devName);
	dev = open(devStr.c_str(), O_RDWR);
	if(dev < 0)
	{	
		string output = devName;
		//output.assign(devName);
		output.append("Device driver not found.\n");
		panic(output.c_str());
	}
	
	pid = getpid();
	if(ioctl(dev, IOCTL_PID, &pid))
	{
		close(dev);
		string output = devName;
		//output.assign(devName);
		output.append("Failed system call. Closing device driver.\n");
		panic(output.c_str());
	}

}

CCharacterDev::~CCharacterDev()
{
	close(dev);

	// panic("[PIR] Exiting\n");
	string output = devStr;
	//output.assign(devName);
	output.append("Exiting\n");
	panic(output.c_str());
}
