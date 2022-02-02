/**
 * @file 	CCharacterDev.h CCharacterDev.c
 * @author 	Diogo Fernandes, Tomas Abreu
 * @date 	20/01/2022
 *
 * @brief 	Class to deal with a character device
 */

#ifndef __CCHARACTERDEVICE_H__
#define __CCHARACTERDEVICE_H__

#include <string>

class CCharacterDev
{
public:
	CCharacterDev(std::string devName);
	~CCharacterDev();

	virtual void enable(void) = 0;
	virtual void disable(void) = 0;

	void Open();
	void Close();

private:
	int dev;
	std::string devStr;
};

#endif //!__CCHARACTERDEVICE_H__
