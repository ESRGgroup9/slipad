#include <bcm2835.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	if(argc < 2)
		return 1;

	if(!bcm2835_init())
	{
		cout << "Error BCM2835 init" << endl;
		exit(1);
	}

  if (!bcm2835_spi_begin())
  {
    cout << "bcm2835_spi_begin failed. Are you running as root??\n";
    return 1;
  }

	// Initialize SPI interface with default values
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);

	// SPI at ~8 MHz
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

	// Send a byte to the slave and simultaneously read a byte back from the slave
  // If you tie MISO to MOSI, you should read back what was sent

	// uint8_t send_data = 0x42 & 0x7f;
	uint8_t send_data = strtol(argv[1], NULL, 16);
	uint8_t read_data = bcm2835_spi_transfer(send_data);

	cout << "Sent[0x" << hex << static_cast<int>(send_data) << "]" << endl;
	cout << "Read[0x" << hex << static_cast<int>(read_data) << "]" << endl;

	if (send_data != read_data)
	  cout << "Do you have the loopback from MOSI to MISO connected?\n";

	bcm2835_spi_end();
	bcm2835_close();

	return 0;
}