#include <iostream>
#include <bcm2835.h>
using namespace std;

#define PWM_PIN		(RPI_GPIO_P1_12)
#define PWM_CHANNEL 0
#define RANGE 		(67500)

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		cout << "Usage: " << argv[0] << " <duty_cycle>" << endl;
		return 1;
	}

	if (!bcm2835_init())
	{
		cout << "Error bcm2835 init" << endl;
    	return 1;
	}

    // Set the output pin to Alt Fun 5, to allow PWM channel 0 to be output there
    bcm2835_gpio_fsel(PWM_PIN, BCM2835_GPIO_FSEL_ALT5);
    
    // Oscillator 54 MHz. With a divider of 16 in MARKSPACE mode, and a
    // pulse repetition frequency of 50 Hz:

    // 54 MHz / 16 = 3,375 MHz
    // RANGE = 3,375MHz / 50 Hz
    // RANGE = 67500

    bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_16);
    //CTL reg
    bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1);
    //RNG1/2 reg
    bcm2835_pwm_set_range(PWM_CHANNEL, RANGE);

    float duty = atof(argv[1]);
    if(duty > 1)
   	{
   		cout << "Invalid duty cycle. Insert from 0 to 1" << endl;
   		return 1;
   	}

    cout << "Generating PWM wave on GPIO " << PWM_PIN << endl;
    cout << "PWM duty cycle at: " << duty*100 << " %" << endl;
    cout << "PWM data: " << duty*RANGE << endl;

    bcm2835_pwm_set_data(PWM_CHANNEL, (duty*RANGE));
    bcm2835_close();

	return 0;
}