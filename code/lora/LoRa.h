// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef LORA_H
#define LORA_H

#include <cstdint> // uint8_t
#include <cstddef> // size_t
#include <string>  // string
#include <bcm2835.h> // BCM2835_SPI_CS0

// #define LORA_DEFAULT_SPI          SPI
#define LORA_DEFAULT_SPI_CS        BCM2835_SPI_CS0

// if you change this see LoraClass::setSPI
#define LORA_DEFAULT_SPI_FREQUENCY 8E6

// lora default pins
// CE0 when SPI0 in use   RPI_GPIO_P1_24
// CE1 when SPI0 in use   RPI_GPIO_P1_26

// CLK when SPI0 in use   RPI_GPIO_P1_23
// MISO when SPI0 in use  RPI_GPIO_P1_21
// MOSI when SPI0 in use  RPI_GPIO_P1_19

#define LORA_DEFAULT_SS_PIN        10
#define LORA_DEFAULT_RESET_PIN     9
#define LORA_DEFAULT_DIO0_PIN      2  

#define PA_OUTPUT_RFO_PIN          0
#define PA_OUTPUT_PA_BOOST_PIN     1

typedef struct
{
  int recvAddr;     // receiver address
  int sendAddr;     // sender address

  int msgID;        // message ID
  size_t msgLength; // message length
  std::string msg;  // message
} LoRaMessage;

enum class ErrorLoraRecv
{
  MSGOK = 0,  // Message OK
  ENOMSGR,    // No message received
  ENOTME,     // Message received is not for this device
  EBADLMSG    // Message received lengths does not match
};

class LoRaClass {
public:
  LoRaClass(int localAddress);

  int begin(long frequency);
  void end();

  // setup lora pins
  void setup(int ss = LORA_DEFAULT_SS_PIN, int reset = LORA_DEFAULT_RESET_PIN,
             int dio0 = LORA_DEFAULT_DIO0_PIN, int cs = LORA_DEFAULT_SPI_CS);
  // set SPI
  void setSPI(void);
  
  // user functions to send and recv Lora messages
  void sendTo(std::string msg, int destination);
  ErrorLoraRecv receive(LoRaMessage &loraMsg);

  int getLocalAddress(void) const { return localAddress; }

  // packet creation functions
  int beginPacket(int implicitHeader = false);
  int endPacket(bool async = false);

  // write functions
  size_t write(uint8_t byte);
  size_t write(const uint8_t *buffer, size_t size);


  int parsePacket(int size = 0);
  int packetRssi();
  float packetSnr();
  long packetFrequencyError();

  int rssi();

  void idle();
  void sleep();

  void setTxPower(int level, int outputPin = PA_OUTPUT_PA_BOOST_PIN);
  void setFrequency(long frequency);
  void setSpreadingFactor(int sf);
  void setSignalBandwidth(long sbw);
  void setCodingRate4(int denominator);
  void setPreambleLength(long length);
  void setSyncWord(int sw);

  void enableCrc();
  void disableCrc();
  void enableInvertIQ();
  void disableInvertIQ();
  
  // Over Current Protection control
  void setOCP(uint8_t mA);
  // Set LNA gain
  void setGain(uint8_t gain);

  // deprecated
  void crc()    { enableCrc(); }
  void noCrc()  { disableCrc(); }

  uint8_t random();
  int available();
  int read();
  int peek();
  void flush();

  void onReceive(void(*callback)(int));
  void onTxDone(void(*callback)());
  void recv(int size = 0);

private:
  void explicitHeaderMode();
  void implicitHeaderMode();

  void handleDio0Rise();
  static void onDio0Rise();

  bool isTransmitting();
  int getSpreadingFactor();
  long getSignalBandwidth();
  void setLdoFlag();

  uint8_t readRegister(uint8_t address);
  void writeRegister(uint8_t address, uint8_t value);
  uint8_t singleTransfer(uint8_t address, uint8_t value);

private:
  // Lora Module Pins
  int _cs;
  int _ss;
  int _reset;
  int _dio0;

  // Local address to be used in comunications with other modules
  int localAddress;

  long _frequency;
  int _packetIndex;
  int _implicitHeaderMode;
  
  // receive/transmitt done callbacks
  void (*_onReceive)(int);
  void (*_onTxDone)();
};

extern LoRaClass LoRa;

#endif
