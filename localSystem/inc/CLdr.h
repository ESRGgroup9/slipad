#ifndef __CLDR_H__
#define __CLDR_H__

enum class LuxState
{
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

	void enable(void);
	void disable(void);

	LuxState getLuxState(void);

private:
	LuxState luxstate;
};
// end CLdr Class definition

#endif //!__CLDR_H__