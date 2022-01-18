/**
 * @file
 * @author Diogo Fernandes, Tomás Abreu
 * @date
 *
 * @brief Class to control the luminosity of the lamp
 * 
 * Detailed Description
 * PWM Generation:
 * Oscillator 54 MHz. With a divider of 16 in MARKSPACE mode, and a 
 * pulse repetition frequency of 50 Hz:
 * 		54 MHz / 16 = 3,375 MHz
 *  	RANGE = 3,375MHz / 50 Hz
 * 		RANGE = 67500
 */

#include "CLamp.h"

#include "utils.h"
#include <bcm2835.h>

/**
 * PWM Defines
 */
#define PWM_PIN		(RPI_GPIO_P1_12)
#define PWM_CHANNEL (0)
#define RANGE 		(67500)
#define PWM_MAX 	(100)
#define PWM_OFF 	(0)

CLamp::CLamp(int timoutSecs) :
    // create timer in as non periodic - single expire
    timLampOnSecs(timoutSecs, NULL, false)
{
	if (!bcm2835_init())
		panic("Error bcm2835 init");

	pwmVal = 0;

	// PWM Generation Initialization
	// Set the output pin to Alt Fun 5, to allow PWM channel 0 to be output there
    bcm2835_gpio_fsel(PWM_PIN, BCM2835_GPIO_FSEL_ALT5);
    // set clock divider
    bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
    //CTL reg
    bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);
    //RNG1/2 reg
    bcm2835_pwm_set_range(PWM_CHANNEL, RANGE);

    if(pthread_mutex_init(&mutChangePWM, NULL) != 0)
        panic("CLamp::CLamp(): Mutex init");
}

CLamp::~CLamp()
{

}

/**
 * @brief Sets PWM in channel 0 to the specified value
 * @param lux: duty-cycle value in percentage
 * */
void CLamp::setBrightness(uint8_t lux)
{
	float duty = lux/100;

	pthread_mutex_lock(&mutChangePWM);
	pwmVal = lux;
	bcm2835_pwm_set_data(PWM_CHANNEL, (duty*RANGE));
    pthread_mutex_unlock(&mutChangePWM);

	if(pwmVal == PWM_MAX)
        timLampOnSecs.start();
}

uint8_t CLamp::getBrightness(void) const
{
    return pwmVal;
}

void CLamp::on(uint8_t lux)
{
	setBrightness(lux);
}

void CLamp::off(void)
{
	setBrightness(PWM_OFF);
}
