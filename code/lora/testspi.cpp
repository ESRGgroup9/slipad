#include <iostream>
#include <string>
#include <bcm2835.h>
using namespace std;

int main(int argc, char *argv[])
{
	uint8_t i = 90;
	uint8_t response;

	if(bcm2835_init() == 0)
	{
		cerr << "Error bcm init" << endl;
		return 1;
	}

	bcm2835_spi_begin();
	// Initialize SPI interface with default values
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);

	// BCM2835 runs at 250 MHz. The clock divider necessary to run SPI
	// at LORA_DEFAULT_SPI_FREQUENCY is:
	//    250MHz / LORA_DEFAULT_SPI_FREQUENCY ~= 32
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

	while(1)
	{
		cout << "Send[" << static_cast<int>(i) << "]" << endl;
		response = bcm2835_spi_transfer(i);
		cout << "Response[" << static_cast<int>(response) << "]" << endl;
		
		cout << "Delay..." << endl;
		bcm2835_delay(5000);
	}
	
	return 0;
}
