#ifndef __CLDR_H__
#define __CLDR_H__

enum luxState_enum{
	DAY = 0,
	NIGHT,
	UNDEF
};

// Light Sensor Class
class CLdr
{
public:
	CLdr(void);
	~CLdr(void);

	luxState_enum getLuxState(void);

protected:

private:
	luxState_enum luxstate;
};
// end CLdr Class definition

#endif //!__CLDR_H__