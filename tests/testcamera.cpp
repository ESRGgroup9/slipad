#include"CCamera.h"

int main(int agrc, char **argv)
{
	CCamera camera("video0");

	camera.captureFrame();

	return 0;
}