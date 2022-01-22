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
	
	this->devStr = "/dev/" + devName;
	this->dev = open(devStr.c_str(), O_RDWR);

	if(dev < 0)
	{	
		panic(string(devStr + "Device driver not found.").c_str())
	}
	
	pid = getpid();
	if(ioctl(dev, IOCTL_PID, &pid))
	{
		close(dev);
		panic(string(devStr + "Failed system call. Closing device driver.").c_str());
	}
}

CCharacterDev::~CCharacterDev()
{
	close(dev);
}
