// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "LoRa.h"

#include <iostream>
#include <thread> // yield
#include <bcm2835.h>
using namespace std;

// registers
#define REG_FIFO                 0x00
#define REG_OP_MODE              0x01

#define REG_FRF_MSB              0x06
#define REG_FRF_MID              0x07
#define REG_FRF_LSB              0x08

#define REG_PA_CONFIG            0x09
#define REG_OCP                  0x0b
#define REG_LNA                  0x0c

#define REG_FIFO_ADDR_PTR        0x0d
#define REG_FIFO_TX_BASE_ADDR    0x0e
#define REG_FIFO_RX_BASE_ADDR    0x0f
#define REG_FIFO_RX_CURRENT_ADDR 0x10

#define REG_IRQ_FLAGS            0x12
#define REG_RX_NB_BYTES          0x13

#define REG_PKT_SNR_VALUE        0x19
#define REG_PKT_RSSI_VALUE       0x1a
#define REG_RSSI_VALUE           0x1b
#define REG_MODEM_CONFIG_1       0x1d
#define REG_MODEM_CONFIG_2       0x1e
#define REG_PREAMBLE_MSB         0x20
#define REG_PREAMBLE_LSB         0x21
#define REG_PAYLOAD_LENGTH       0x22
#define REG_MODEM_CONFIG_3       0x26
#define REG_FREQ_ERROR_MSB       0x28
#define REG_FREQ_ERROR_MID       0x29
#define REG_FREQ_ERROR_LSB       0x2a
#define REG_RSSI_WIDEBAND        0x2c
#define REG_DETECTION_OPTIMIZE   0x31
#define REG_INVERTIQ             0x33
#define REG_DETECTION_THRESHOLD  0x37
#define REG_SYNC_WORD            0x39
#define REG_INVERTIQ2            0x3b
#define REG_DIO_MAPPING_1        0x40
#define REG_VERSION              0x42
#define REG_PA_DAC               0x4d

// modes
#define MODE_LONG_RANGE_MODE     0x80
#define MODE_SLEEP               0x00
#define MODE_STDBY               0x01
#define MODE_TX                  0x03
#define MODE_RX_CONTINUOUS       0x05
#define MODE_RX_SINGLE           0x06

// PA config
#define PA_BOOST                 0x80

// IRQ masks
#define IRQ_TX_DONE_MASK           0x08
#define IRQ_PAYLOAD_CRC_ERROR_MASK 0x20
#define IRQ_RX_DONE_MASK           0x40

#define RF_MID_BAND_THRESHOLD    525E6
#define RSSI_OFFSET_HF_PORT      157
#define RSSI_OFFSET_LF_PORT      164

#define MAX_PKT_LENGTH           255

// #define ISR_PREFIX

// Constants
#define B111  (0b0111)
#define B1000 (0b1000)

#define LORA_DEFAULT_SS_PIN        RPI_GPIO_P1_24
#define LORA_DEFAULT_RESET_PIN     RPI_GPIO_P1_22
#define LORA_DEFAULT_DIO0_PIN      RPI_GPIO_P1_18

#define DEBUG

#ifdef DEBUG
  #include <iomanip>
  #include <fstream>

  ofstream logfile("log.txt");
  // log.open("log.txt");
  // log << str << endl;
  // log.close();
  #define DEBUG_MSG(str) (logfile << str << std::endl)
#else
  #define DEBUG_MSG(str) 
#endif

/**********************************************************
 * GPIO Functions
 * *******************************************************/
/**
 * @brief Write a HIGH or a LOW value to a digital pin.
 * @param pin: pin number.
 * @param val: HIGH or LOW.
 * @return none
 */
#define digitalWrite(_pin_, _val_) bcm2835_gpio_write(_pin_, _val_)

/**
 * @brief Configures the specified pin to behave either as an input or an output.
 * @param pin: pin number to set the mode of.
 * @param mode: INPUT or OUTPUT
 * @return none
 */
#define INPUT   BCM2835_GPIO_FSEL_INPT
#define OUTPUT  BCM2835_GPIO_FSEL_OUTP
#define pinMode(_pin_,_mode_) bcm2835_gpio_fsel(_pin_, _mode_)

/**
 * @brief Writes a bit of a numeric variable.
 * @param var: the numeric variable to which to write.
 * @param bit: which bit of the number to write, starting at 0 for the LSB.
 * @param val: the value to write to the bit (0 or 1).
 * @return none
 */
#define bitRead(_var_, _bit_)   (((_var_) >> (_bit_)) & 0x01)
#define bitSet(_var_, _bit_)    ((_var_) |= (1UL << (_bit_)))
#define bitClear(_var_, _bit_)  ((_var_) &= ~(1UL << (_bit_)))
#define bitWrite(_var_, _bit_, _val_) ((_val_) ? bitSet(_var_, _bit_) : bitClear(_var_, _bit_))

/**
 * @brief Digital Pins With Interrupts
 * @param int: the number of the interrupt. Allowed data types: int.
 * @param isr: the ISR to call when the interrupt occurs;
 * @param mode: defines when the interrupt should be triggered.
 *  LOW to trigger the interrupt whenever the pin is low,
    CHANGE to trigger the interrupt whenever the pin changes value
    RISING to trigger when the pin goes from low to high,
    FALLING for when the pin goes from high to low.
 * @return none
 *
 * Recommended usage: attachInterrupt(digitalPinToInterrupt(pin), ISR, mode)
 * Where, pin: the pin number.
 */
#define attachInterrupt(_int_,_isr_,_mode_) (cout << "attachInterrupt()" << endl)

/**
 * @brief Turns off the given interrupt.
 * @param int: the number of the interrupt to disable
 * @return none
 *
 * Recommended usage: detachInterrupt(digitalPinToInterrupt(pin)). Where
 * pin: the pin number of the interrupt to disable
 */
#define detachInterrupt(_int_) (cout << "detachInterrupt()" << endl)


/**
 * @brief Translate the actual digital pin to the specific interrupt number.
 * @param pin: the pin number.
 * @return the number of the interrupt.
 */
#define digitalPinToInterrupt(_pin_) ((_pin_) == 2 ? 0 : ((_pin_) == 3 ? 1 : -1))

/**********************************************************
 * LoRaMsg << operator overload
 * *******************************************************/
ostream& operator<<(ostream& os, const LoRaMsg& msg)
{
  cout << "from[0x"   << hex << msg.sendAddr   << "]" << endl;
  cout << "to[0x"   << hex << msg.recvAddr   << "]" << endl;
  cout << "msgID["      << msg.msgID      << "]" << endl;
  cout << "msgLength["  << dec << msg.msgLength  << "]" << endl;
  cout << "msg["        << msg.msg        << "]" << endl;
  return os;
}

/**********************************************************
 * Functions Implementation
 * *******************************************************/
LoRaClass::LoRaClass(int localAddress) :
  _ss(LORA_DEFAULT_SS_PIN),
  _reset(LORA_DEFAULT_RESET_PIN),
  _dio0(LORA_DEFAULT_DIO0_PIN),
  
  _frequency(0),
  _packetIndex(0),
  _implicitHeaderMode(0),
  _onReceive(NULL),
  _onTxDone(NULL)
{
  this->localAddress = localAddress;

  if(!bcm2835_init())
  {
    cout << "Error BCM2835 init" << endl;
    exit(1);
  }
}

int LoRaClass::begin(long frequency)
{
  // setup pins
  pinMode(_ss, OUTPUT);

  // set SS high
  digitalWrite(_ss, HIGH);

  if (_reset != -1)
  {
    pinMode(_reset, OUTPUT);

    // perform reset
    digitalWrite(_reset, LOW);
    // sleep(10);
    bcm2835_delay(10);
    digitalWrite(_reset, HIGH);
    // sleep(10);
    bcm2835_delay(10);
  }

  // start SPI
  if (!bcm2835_spi_begin())
  {
    cout << "bcm2835_spi_begin failed. Are you running as root??\n";
    exit(1);
  }

  DEBUG_MSG("[SPI] begin");
  setSPI();
  DEBUG_MSG("[SPI] setup done");

  // check version
  uint8_t version = readRegister(REG_VERSION);

  if (version != 0x12)
  {
    cout << "[begin] version: 0x" << hex << static_cast<int>(version) << endl;
    // DEBUG_MSG("[begin] version: 0x" << hex << static_cast<int>(version));
    return 0;
  }

  // put in sleep mode
  sleep();

  // set frequency
  setFrequency(frequency);

  // set base addresses
  writeRegister(REG_FIFO_TX_BASE_ADDR, 0);
  writeRegister(REG_FIFO_RX_BASE_ADDR, 0);

  // set LNA boost
  writeRegister(REG_LNA, readRegister(REG_LNA) | 0x03);

  // set auto AGC
  writeRegister(REG_MODEM_CONFIG_3, 0x04);

  // set output power to 17 dBm
  setTxPower(17);

  // put in standby mode
  idle();

  return 1;
}

void LoRaClass::end()
{
  // put in sleep mode
  sleep();

  // stop SPI
  // _spi->end();
  bcm2835_spi_end();
}

void LoRaClass::setPins(int ss, int reset, int dio0)
{
  _ss = ss;
  _reset = reset;
  _dio0 = dio0;
}

void LoRaClass::setSPI(void)
{
  // bcm2835_spi_begin();

  // Initialize SPI interface with default values
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);

  // BCM2835 runs at 250 MHz. The clock divider necessary to run SPI
  // at LORA_DEFAULT_SPI_FREQUENCY is:
  //    250MHz / LORA_DEFAULT_SPI_FREQUENCY ~= 32
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);
  // bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
  // bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
}

LoRaMsg LoRaClass::sendTo(std::string msg, uint8_t destination)
{
  // create static message count as message identifier
  static uint8_t msgCount = 0;
  LoRaMsg loraMsg;

  beginPacket();

  // DEBUG_MSG(endl << "[sendTo] send destination addr");
  // add destination address  
  write(destination);

  // DEBUG_MSG(endl << "[sendTo] send sender addr");
  // // add sender address
  write(localAddress);

  // DEBUG_MSG(endl << "[sendTo] msg id");
  // // add message ID
  write(msgCount);

  // DEBUG_MSG(endl << "[sendTo] msg length");
  // // add message length
  write(msg.length());

  // DEBUG_MSG(endl << "[sendTo] msg");
  // // add message; convert string to const uint8_t*
  write(reinterpret_cast<const uint8_t*>(&msg[0]), msg.length());

  endPacket();

  // return received message
  loraMsg.recvAddr = destination;
  loraMsg.sendAddr = localAddress;
  loraMsg.msgID = msgCount;
  loraMsg.msgLength = msg.length();
  loraMsg.msg = msg;

  // update message counter
  msgCount++;

  return loraMsg;
}

LoRaError LoRaClass::receive(LoRaMsg &loraMsg) 
{
  if(parsePacket() == 0)
    // no message received
    return LoRaError::ENOMSGR;
 
  // parse packet
  // read recipient address
  int recipient = read();
  
  // check the message recipient
  if ((recipient != localAddress) && (recipient != 0xFF))
    // this message is not for me
    return LoRaError::ENOTME;
  
  // read sender address
  uint8_t sender = read();
  // read message ID
  uint8_t incomingMsgId = read();
  
  // read message length
  uint8_t incomingLength = read();
  // read message
  std::string msg = "";
  while (available())
    msg += static_cast<char>(read());
 
  // check length for error
  if(incomingLength != msg.length())
    // error: message length does not match the supposed length
    return LoRaError::EBADLMSG;                        

  // return received message using loraMsg
  loraMsg.recvAddr = recipient;
  loraMsg.sendAddr = sender;
  loraMsg.msgID = incomingMsgId;
  loraMsg.msgLength = incomingLength;
  loraMsg.msg = msg;

  // message received
  return LoRaError::MSGOK;
}


size_t LoRaClass::write(uint8_t byte)
{
  return write(&byte, sizeof(byte));
}

size_t LoRaClass::write(const uint8_t *buffer, size_t size)
{
  // if(size == 1)
  //   DEBUG_MSG("[write] buffer[" << static_cast<int>(buffer[0]) << "] size[" << size << "]");
  // else
  //   DEBUG_MSG("[write] buffer[" << buffer << "] size[" << size << "]");

  if(buffer == NULL)
    return -1;

  int currentLength = readRegister(REG_PAYLOAD_LENGTH);

  // check size
  if ((currentLength + size) > MAX_PKT_LENGTH)
    size = MAX_PKT_LENGTH - currentLength;

  // write data
  for (size_t i = 0; i < size; i++)
  {
    DEBUG_MSG("[write] send(" << static_cast<int>(buffer[i]) << ") from buff[" << i << "]");
    writeRegister(REG_FIFO, buffer[i]);
  }

  // update length
  writeRegister(REG_PAYLOAD_LENGTH, currentLength + size);
  
  return size;
}

int LoRaClass::beginPacket(int implicitHeader)
{
  DEBUG_MSG("[beginPacket] enter");

  if (isTransmitting())
    return 0;

  // put in standby mode
  idle();

  if (implicitHeader)
    implicitHeaderMode();
  else
    explicitHeaderMode();

  // reset FIFO address and payload length
  writeRegister(REG_FIFO_ADDR_PTR, 0);
  writeRegister(REG_PAYLOAD_LENGTH, 0);

  return 1;
}

int LoRaClass::endPacket(bool async)
{
  DEBUG_MSG("[endPacket] enter");

  if ((async) && (_onTxDone))
  {
    DEBUG_MSG("[endPacket] DIO0 => TXDONE");
    // DIO0 => TXDONE
    writeRegister(REG_DIO_MAPPING_1, 0x40);
  }

  // put in TX mode
  DEBUG_MSG("[endPacket] put in TX mode");
  writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

  // send FIFO msgs in polling mode
  if (!async)
  {
    // wait for TX done
    DEBUG_MSG("[endPacket] waiting for TX done...");

    while ((readRegister(REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0)
    {
      // std::this_thread::yield();
    }
    
    // clear IRQ's
    writeRegister(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
  }

  DEBUG_MSG("[endPacket] packet end");
  return 1;
}

bool LoRaClass::isTransmitting()
{
  if ((readRegister(REG_OP_MODE) & MODE_TX) == MODE_TX)
  {
    DEBUG_MSG("[isTransmitting] mode TX");
    return true;
  }

  if (readRegister(REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK)
  {
    // clear IRQ's
    writeRegister(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
    DEBUG_MSG("[isTransmitting] TX done");
  }

  DEBUG_MSG("[isTransmitting] not transmitting");
  return false;
}

int LoRaClass::parsePacket(int size)
{
  int packetLength = 0;
  int irqFlags = readRegister(REG_IRQ_FLAGS);

  if (size > 0)
  {
    implicitHeaderMode();
    writeRegister(REG_PAYLOAD_LENGTH, size & 0xff);
  }
  else
    explicitHeaderMode();

  // clear IRQ's
  writeRegister(REG_IRQ_FLAGS, irqFlags);

  if ((irqFlags & IRQ_RX_DONE_MASK) &&
      (irqFlags & IRQ_PAYLOAD_CRC_ERROR_MASK) == 0)
  {
    DEBUG_MSG("[parsePacket] received a packet");
    // received a packet
    _packetIndex = 0;

    // read packet length
    if (_implicitHeaderMode)
      packetLength = readRegister(REG_PAYLOAD_LENGTH);
    else
      packetLength = readRegister(REG_RX_NB_BYTES);

    // set FIFO address to current RX address
    writeRegister(REG_FIFO_ADDR_PTR, readRegister(REG_FIFO_RX_CURRENT_ADDR));

    // put in standby mode
    idle();
  }
  else if (readRegister(REG_OP_MODE) != (MODE_LONG_RANGE_MODE | MODE_RX_SINGLE))
  {
    // not currently in RX mode
    // reset FIFO address
    writeRegister(REG_FIFO_ADDR_PTR, 0);

    // put in single RX mode
    writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_SINGLE);
  }

  return packetLength;
}

int LoRaClass::packetRssi()
{
  return (readRegister(REG_PKT_RSSI_VALUE) -
          (_frequency < RF_MID_BAND_THRESHOLD) ?
                        RSSI_OFFSET_LF_PORT : RSSI_OFFSET_HF_PORT);
}

float LoRaClass::packetSnr()
{
  return ((int8_t)readRegister(REG_PKT_SNR_VALUE)) * 0.25;
}

long LoRaClass::packetFrequencyError()
{
  int32_t freqError = 0;
  
  freqError = static_cast<int32_t>(readRegister(REG_FREQ_ERROR_MSB) & B111);
  freqError <<= 8L;
  freqError += static_cast<int32_t>(readRegister(REG_FREQ_ERROR_MID));
  freqError <<= 8L;
  freqError += static_cast<int32_t>(readRegister(REG_FREQ_ERROR_LSB));

  if(readRegister(REG_FREQ_ERROR_MSB) & B1000)
  {
    // Sign bit is on
    freqError -= 524288; // B1000'0000'0000'0000'0000
  }

  // FXOSC: crystal oscillator (XTAL) frequency
  // (2.5. Chip Specification, pag. 14)
  const float fXtal = 32E6;
  // (4.1.5 Frequency Error Indication, pag. 37)
  const float fError = ((static_cast<float>(freqError) * (1L << 24)) / fXtal) *
                        (getSignalBandwidth() / 500000.0f);

  return static_cast<long>(fError);
}

uint8_t LoRaClass::random()
{
  return readRegister(REG_RSSI_WIDEBAND);
}

int LoRaClass::rssi()
{
  return (readRegister(REG_RSSI_VALUE) - (_frequency < RF_MID_BAND_THRESHOLD ?
          RSSI_OFFSET_LF_PORT : RSSI_OFFSET_HF_PORT));
}

int LoRaClass::available()
{
  return (readRegister(REG_RX_NB_BYTES) - _packetIndex);
}

int LoRaClass::read()
{
  if (!available())
  {
    DEBUG_MSG("[read] not available");
    return -1;
  }

  // DEBUG_MSG("[read] reading");
  _packetIndex++;
  return readRegister(REG_FIFO);
}

int LoRaClass::peek()
{
  if (!available())
    return -1;

  // store current FIFO address
  int currentAddress = readRegister(REG_FIFO_ADDR_PTR);

  // read
  uint8_t b = readRegister(REG_FIFO);

  // restore FIFO address
  writeRegister(REG_FIFO_ADDR_PTR, currentAddress);

  return b;
}

void LoRaClass::flush()
{
}


void LoRaClass::onReceive(void(*callback)(int))
{
  _onReceive = callback;

  DEBUG_MSG("[onReceive] enter");
  if(callback)
  {
    pinMode(_dio0, INPUT);
    attachInterrupt(digitalPinToInterrupt(_dio0), LoRaClass::onDio0Rise, RISING);
  }
  else
  {
    detachInterrupt(digitalPinToInterrupt(_dio0));
  }
}

void LoRaClass::onTxDone(void(*callback)())
{
  _onTxDone = callback;

  DEBUG_MSG("[onTxDone] enter");
  if(callback)
  {
    pinMode(_dio0, INPUT);
    attachInterrupt(digitalPinToInterrupt(_dio0), LoRaClass::onDio0Rise, RISING);
  }
  else
  {
    detachInterrupt(digitalPinToInterrupt(_dio0));
  }
}

void LoRaClass::recv(int size)
{
  // DIO0 => RXDONE
  writeRegister(REG_DIO_MAPPING_1, 0x00);

  if (size > 0)
  {
    implicitHeaderMode();
    writeRegister(REG_PAYLOAD_LENGTH, size & 0xff);
  }
  else
    explicitHeaderMode();

  writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_CONTINUOUS);
}

void LoRaClass::idle()
{
  writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
}

void LoRaClass::sleep()
{
  writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
}

void LoRaClass::setTxPower(int level, int outputPin)
{
  if (PA_OUTPUT_RFO_PIN == outputPin)
  {
    // RFO
    if (level < 0)
      level = 0;
    else if(level > 14)
      level = 14;

    writeRegister(REG_PA_CONFIG, 0x70 | level);
  }
  else 
  {
    // PA BOOST
    if (level > 17)
    {
      if (level > 20)
        level = 20;

      // subtract 3 from level, so 18 - 20 maps to 15 - 17
      level -= 3;

      // High Power +20 dBm Operation (Semtech SX1276/77/78/79 5.4.3.)
      writeRegister(REG_PA_DAC, 0x87);
      setOCP(140);
    }
    else
    {
      if (level < 2) 
        level = 2;

      //Default value PA_HF/LF or +17dBm
      writeRegister(REG_PA_DAC, 0x84);
      setOCP(100);
    }

    writeRegister(REG_PA_CONFIG, PA_BOOST | (level - 2));
  }
}

void LoRaClass::setFrequency(long frequency)
{
  _frequency = frequency;

  uint64_t frf = ((uint64_t)frequency << 19) / 32000000;

  writeRegister(REG_FRF_MSB, (uint8_t)(frf >> 16));
  writeRegister(REG_FRF_MID, (uint8_t)(frf >> 8));
  writeRegister(REG_FRF_LSB, (uint8_t)(frf >> 0));
}

int LoRaClass::getSpreadingFactor()
{
  return readRegister(REG_MODEM_CONFIG_2) >> 4;
}

void LoRaClass::setSpreadingFactor(int sf)
{
  if (sf < 6)
    sf = 6;
  else if (sf > 12)
    sf = 12;

  if (sf == 6)
  {
    writeRegister(REG_DETECTION_OPTIMIZE, 0xc5);
    writeRegister(REG_DETECTION_THRESHOLD, 0x0c);
  } 
  else
  {
    writeRegister(REG_DETECTION_OPTIMIZE, 0xc3);
    writeRegister(REG_DETECTION_THRESHOLD, 0x0a);
  }

  writeRegister(REG_MODEM_CONFIG_2,
              (readRegister(REG_MODEM_CONFIG_2) & 0x0f) | ((sf << 4) & 0xf0));
  setLdoFlag();
}

long LoRaClass::getSignalBandwidth()
{
  uint8_t bw = (readRegister(REG_MODEM_CONFIG_1) >> 4);

  switch (bw)
  {
    case 0: return 7.8E3;
    case 1: return 10.4E3;
    case 2: return 15.6E3;
    case 3: return 20.8E3;
    case 4: return 31.25E3;
    case 5: return 41.7E3;
    case 6: return 62.5E3;
    case 7: return 125E3;
    case 8: return 250E3;
    case 9: return 500E3;
  }

  return -1;
}

void LoRaClass::setSignalBandwidth(long sbw)
{
  int bw;

  if (sbw <= 7.8E3) {
    bw = 0;
  } else if (sbw <= 10.4E3) {
    bw = 1;
  } else if (sbw <= 15.6E3) {
    bw = 2;
  } else if (sbw <= 20.8E3) {
    bw = 3;
  } else if (sbw <= 31.25E3) {
    bw = 4;
  } else if (sbw <= 41.7E3) {
    bw = 5;
  } else if (sbw <= 62.5E3) {
    bw = 6;
  } else if (sbw <= 125E3) {
    bw = 7;
  } else if (sbw <= 250E3) {
    bw = 8;
  } else /*if (sbw <= 250E3)*/ {
    bw = 9;
  }

  writeRegister(REG_MODEM_CONFIG_1, (readRegister(REG_MODEM_CONFIG_1) & 0x0f) | (bw << 4));
  setLdoFlag();
}

void LoRaClass::setLdoFlag()
{
  // Section 4.1.1.5
  long symbolDuration = 1000 / ( getSignalBandwidth() / (1L << getSpreadingFactor()) ) ;

  // Section 4.1.1.6
  bool ldoOn = symbolDuration > 16; 

  uint8_t config3 = readRegister(REG_MODEM_CONFIG_3);
  bitWrite(config3, 3, ldoOn);
  writeRegister(REG_MODEM_CONFIG_3, config3);
}

void LoRaClass::setCodingRate4(int denominator)
{
  if (denominator < 5) {
    denominator = 5;
  } else if (denominator > 8) {
    denominator = 8;
  }

  int cr = denominator - 4;

  writeRegister(REG_MODEM_CONFIG_1, (readRegister(REG_MODEM_CONFIG_1) & 0xf1) | (cr << 1));
}

void LoRaClass::setPreambleLength(long length)
{
  writeRegister(REG_PREAMBLE_MSB, (uint8_t)(length >> 8));
  writeRegister(REG_PREAMBLE_LSB, (uint8_t)(length >> 0));
}

void LoRaClass::setSyncWord(int sw)
{
  writeRegister(REG_SYNC_WORD, sw);
}

void LoRaClass::enableCrc()
{
  writeRegister(REG_MODEM_CONFIG_2, readRegister(REG_MODEM_CONFIG_2) | 0x04);
}

void LoRaClass::disableCrc()
{
  writeRegister(REG_MODEM_CONFIG_2, readRegister(REG_MODEM_CONFIG_2) & 0xfb);
}

void LoRaClass::enableInvertIQ()
{
  writeRegister(REG_INVERTIQ,  0x66);
  writeRegister(REG_INVERTIQ2, 0x19);
}

void LoRaClass::disableInvertIQ()
{
  writeRegister(REG_INVERTIQ,  0x27);
  writeRegister(REG_INVERTIQ2, 0x1d);
}

void LoRaClass::setOCP(uint8_t mA)
{
  uint8_t ocpTrim = 27;

  if (mA <= 120) {
    ocpTrim = (mA - 45) / 5;
  } else if (mA <=240) {
    ocpTrim = (mA + 30) / 10;
  }

  writeRegister(REG_OCP, 0x20 | (0x1F & ocpTrim));
}

void LoRaClass::setGain(uint8_t gain)
{
  // check allowed range
  if (gain > 6) {
    gain = 6;
  }
  
  // set to standby
  idle();
  
  // set gain
  if (gain == 0)
  {
    // if gain = 0, enable AGC
    writeRegister(REG_MODEM_CONFIG_3, 0x04);
  }
  else 
  {
    // disable AGC
    writeRegister(REG_MODEM_CONFIG_3, 0x00);
	
    // clear Gain and set LNA boost
    writeRegister(REG_LNA, 0x03);
	
    // set gain
    writeRegister(REG_LNA, readRegister(REG_LNA) | (gain << 5));
  }
}

void LoRaClass::explicitHeaderMode()
{
  _implicitHeaderMode = 0;

  writeRegister(REG_MODEM_CONFIG_1, readRegister(REG_MODEM_CONFIG_1) & 0xfe);
}

void LoRaClass::implicitHeaderMode()
{
  _implicitHeaderMode = 1;

  writeRegister(REG_MODEM_CONFIG_1, readRegister(REG_MODEM_CONFIG_1) | 0x01);
}

void LoRaClass::handleDio0Rise()
{
  int irqFlags = readRegister(REG_IRQ_FLAGS);

  // clear IRQ's
  writeRegister(REG_IRQ_FLAGS, irqFlags);

  if ((irqFlags & IRQ_PAYLOAD_CRC_ERROR_MASK) == 0)
  {
    if ((irqFlags & IRQ_RX_DONE_MASK) != 0)
    {
      // received a packet
      _packetIndex = 0;

      // read packet length
      int packetLength = _implicitHeaderMode ?
                readRegister(REG_PAYLOAD_LENGTH) : readRegister(REG_RX_NB_BYTES);

      // set FIFO address to current RX address
      writeRegister(REG_FIFO_ADDR_PTR, readRegister(REG_FIFO_RX_CURRENT_ADDR));

      if (_onReceive) {
        _onReceive(packetLength);
      }
    }
    else if ((irqFlags & IRQ_TX_DONE_MASK) != 0)
    {
      if (_onTxDone)
      {
        _onTxDone();
      }
    }
  }
}

uint8_t LoRaClass::readRegister(uint8_t address)
{
  uint8_t value = singleTransfer(address & 0x7f, 0x00);
  // DEBUG_MSG("[readRegister] read(" << hex << static_cast<int>(value) << ") from[0x" << hex << static_cast<int>(address & 0x7f) << "]");
  return value;
}

void LoRaClass::writeRegister(uint8_t address, uint8_t value)
{
  DEBUG_MSG("[writeRegister] write(" << hex << static_cast<int>(value) << ") at[0x" << hex << static_cast<int>(address | 0x80) << "]");
  singleTransfer(address | 0x80, value);
}

uint8_t LoRaClass::singleTransfer(uint8_t address, uint8_t value)
{
  uint8_t response;
  
  digitalWrite(_ss, LOW);
  // bcm2835_delayMicroseconds(1);

  bcm2835_spi_transfer(address);
  response = bcm2835_spi_transfer(value);
  
  // bcm2835_delayMicroseconds(1);
  digitalWrite(_ss, HIGH);


  // bcm2835_delay(10);
  // DEBUG_MSG("[SPI] transfer(" << static_cast<int>(response) <<
  //   ") addr[" << setw(3) << static_cast<int>(address) <<
  //   "] val[" << setw(3) << static_cast<int>(value) << "]");

  return response;
}

// ISR_PREFIX void LoRaClass::onDio0Rise()
void LoRaClass::onDio0Rise()
{
  LoRaClass LoRa(-1);
  LoRa.handleDio0Rise();
}

void LoRaClass::printFIFORegs(void)
{
  DEBUG_MSG("[testSPI] REG_FIFO                 (0x" << hex << static_cast<int>(readRegister(REG_FIFO)) << ")");
  DEBUG_MSG("[testSPI] REG_FIFO_ADDR_PTR        (0x" << hex << static_cast<int>(readRegister(REG_FIFO_ADDR_PTR)) << ")");
  DEBUG_MSG("[testSPI] REG_FIFO_TX_BASE_ADDR    (0x" << hex << static_cast<int>(readRegister(REG_FIFO_TX_BASE_ADDR)) << ")");
  DEBUG_MSG("[testSPI] REG_FIFO_RX_BASE_ADDR    (0x" << hex << static_cast<int>(readRegister(REG_FIFO_RX_BASE_ADDR)) << ")");
  DEBUG_MSG("[testSPI] REG_FIFO_RX_CURRENT_ADDR (0x" << hex << static_cast<int>(readRegister(REG_FIFO_RX_CURRENT_ADDR)) << ")");
  DEBUG_MSG("[testSPI] REG_RX_NB_BYTES (0x" << hex << static_cast<int>(readRegister(REG_RX_NB_BYTES)) << ")" << endl);

  uint8_t irqflags = readRegister(REG_IRQ_FLAGS);
  DEBUG_MSG("[testSPI] RX_DONE  (0x" << hex << static_cast<int>(irqflags & 0x40) << ")");
  DEBUG_MSG("[testSPI] TX_DONE  (0x" << hex << static_cast<int>(irqflags & 0x08) << ")");
}

void LoRaClass::testspi(uint8_t value)
{
  // uint8_t response;
  // uint8_t size;

  // check FIFO size
  // size = readRegister(REG_FIFO_ADDR_PTR) - readRegister(REG_FIFO_TX_BASE_ADDR);
  // DEBUG_MSG("[testSPI] FIFO tx bytes(" << static_cast<int>(size) << ")");
  printFIFORegs();

  // write
  writeRegister(REG_FIFO, value);
  DEBUG_MSG("[testSPI] write(" << static_cast<int>(value) << ")");
  
  DEBUG_MSG("[testSPI] delay...");
  bcm2835_delay(1000);

  printFIFORegs();
  // check FIFO size
  // size = readRegister(REG_FIFO_ADDR_PTR) - readRegister(REG_FIFO_TX_BASE_ADDR);
  // DEBUG_MSG("[testSPI] FIFO tx bytes(" << static_cast<int>(size) << ") after write" << endl);

  // read
  // response = readRegister(REG_FIFO);
  // DEBUG_MSG("[testSPI] read(" << static_cast<int>(response) << ")" << endl);


  
}
