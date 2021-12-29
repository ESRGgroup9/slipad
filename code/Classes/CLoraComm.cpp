#include "CLoraComm.h"

void CLoraComm::setSPI(void)
{
  bcm2835_spi_begin();
  // Initialize SPI interface with default values
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);

  /*
    BCM2835 runs at 250 MHz. The clock divider necessary to run SPI
    at LORA_DEFAULT_SPI_FREQUENCY is:

       250MHz / LORA_DEFAULT_SPI_FREQUENCY ~= 32
  */
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
  bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
}


void CLoraComm::digitalWrite(uint8_t pin, uint8_t val)
{
	if(val == 0)
		bcm2835_gpio_clr(pin);
	else
		bcm2835_gpio_set(pin);
}

void CLoraComm::pinMode()
{
	bcm2835_gpio_fsel(_pin_, _mode_);
}