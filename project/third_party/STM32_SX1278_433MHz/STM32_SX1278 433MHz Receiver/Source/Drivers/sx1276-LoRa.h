/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND 
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) SEMTECH S.A.
 */
/*! 
 * \file       sx1276-LoRa.h
 * \brief      SX1276 RF chip driver mode LoRa
 *
 * \version    2.0.B2 
 * \date       May 6 2013
 * \author     Gregory Cristian
 *
 * Last modified by Miguel Luis on Jun 19 2013
 */
#ifndef __SX1276_LORA_H__
#define __SX1276_LORA_H__
   
/*   
  #define LED_TX           PD_ODR_ODR7
#define LED_RX           PD_ODR_ODR6

#define MODE_CTRL1           PB_ODR_ODR2
#define MODE_CTRL2           PB_ODR_ODR1

#define STATUS_AUX           PB_ODR_ODR0

#define RF_RESET             PD_ODR_ODR5
#define RF_IRQ             PC_ODR_ODR4
#define RF_NSS             PD_ODR_ODR4



#define RF_RST        PD_ODR_ODR5//pc5--rf rst
#define RF_SCK        PB_ODR_ODR5//
#define RF_MISO       PB_IDR_IDR7 //INPUT
#define RF_MOSI       PB_ODR_ODR6 
#define RF_NSEL_PIN   PD_ODR_ODR4//pb4 ---片选
#define RF_RXTXSWITH  PB_ODR_ODR3//天线开关


#define SCK        PB_ODR_ODR5
#define MISO       PB_IDR_IDR7 //INPUT
#define MOSI       PB_ODR_ODR6 
#define nCS   PD_ODR_ODR4
#define RF_RXTXSWITH  PB_ODR_ODR3


#define RF_IRQ_DIO0       PC_IDR_IDR4


 */
#define	RF_GPIO	GPIOB
#define	RF_SEL_PIN	GPIO_Pin_12
#define	RF_IRQ_PIN	 GPIO_Pin_10
#define  RF_RST	 GPIO_Pin_8        //XL1278-SD01 REST
#define	RF_RST_1  GPIO_Pin_11     //xl1278-smt rest 

   
   



//-----------------------------------------------------------------------------
// 子程序声明
//-----------------------------------------------------------------------------
void RF_GpioInt(void);
void SpiWriteAddressData(unsigned char address, unsigned char data1);
unsigned char SpiReadAddressData(unsigned char u8Addr);
void SX1276LoRaInit( void );

//-----------------------------------------------------------------------------
// define MCU GPIO
//-----------------------------------------------------------------------------

typedef unsigned char u8;
typedef unsigned char uint8_t;
typedef unsigned char INT8U;
u8 SPIRead(u8 adr);
void SPIBurstRead(u8 adr, u8 *ptr, u8 length);
void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size );
void RFM96_LoRaEntryRx(void);
u8 RFM96_LoRaRxPacket(u8 *buf);
u8 RFM96_LoRaEntryTx(u8 packet_length);
u8 RFM96_LoRaTxPacket(u8 *buf,u8 len);
void delayms(unsigned int t);
/*!
 * SX1276 Internal registers Address
 */
//RFM96 Internal registers Address
#define LR_RegFifo                                  0x0000//FIFO read/write access
// Common settings
#define LR_RegOpMode                                0x0100//Operating mode & LoRaTM / FSK selection
#define LR_RegFrMsb                                 0x0600//RF Carrier Frequency, Most Significant Bits
#define LR_RegFrMid                                 0x0700//RF Carrier Frequency, Most Significant Bits
#define LR_RegFrLsb                                 0x0800//RF Carrier Frequency, Most Significant Bits
// Tx settings
#define LR_RegPaConfig                              0x0900//PA selection and Output Power control
#define LR_RegPaRamp                                0x0A00//Control of PA ramp time, low phase noise PLL
#define LR_RegOcp                                   0x0B00//Over Current Protection control
// Rx settings
#define LR_RegLna                                   0x0C00//LNA settings
// LoRa registers
#define LR_RegFifoAddrPtr                           0x0D00//FIFO SPI pointer
#define LR_RegFifoTxBaseAddr                        0x0E00//Start Tx data
#define LR_RegFifoRxBaseAddr                        0x0F00//Start Rx data
#define LR_RegFifoRxCurrentaddr                     0x1000//Start address of last packet received 
#define LR_RegIrqFlagsMask                          0x1100//Optional IRQ flag mask
#define LR_RegIrqFlags                              0x1200//IRQ flags
#define LR_RegRxNbBytes                             0x1300//Number of received bytes
#define LR_RegRxHeaderCntValueMsb                   0x1400//Number of valid headers received
#define LR_RegRxHeaderCntValueLsb                   0x1500//
#define LR_RegRxPacketCntValueMsb                   0x1600//Number of valid packets received
#define LR_RegRxPacketCntValueLsb                   0x1700
#define LR_RegModemStat                             0x1800//Live LoRa TM  modem status
#define LR_RegPktSnrValue                           0x1900//Espimation of last packetSNR
#define LR_RegPktRssiValue                          0x1A00//RSSI of last packet
#define LR_RegRssiValue                             0x1B00//Current RSSI
#define LR_RegHopChannel                            0x1C00//FHSS start channel
#define LR_RegModemConfig1                          0x1D00//Modem PHY config 最重要的寄存器包括Signal bandwidth:Error coding rate，Explicit Header mode
#define LR_RegModemConfig2                          0x1E00//SF rate,
#define LR_RegSymbTimeoutLsb                        0x1F00//Receiver timeout value
#define LR_RegPreambleMsb                           0x2000//Size of preamble
#define LR_RegPreambleLsb                           0x2100//
#define LR_RegPayloadLength                         0x2200//LoRa TM  payload length
#define LR_RegMaxPayloadLength                      0x2300//LoRaTM maximum pay-load length
#define LR_RegHopPeriod                             0x2400//FHSS Hop period
#define LR_RegFifoRxByteAddr                        0x2500//Address of last bytewritten in FIFO
#define LR_RegModemConfig3                         0x2600//Modem PHY config 3

// I/O settings
#define REG_LR_DIOMAPPING1                          0x4000
#define REG_LR_DIOMAPPING2                          0x4100
// Version
#define REG_LR_VERSION                              0x4200
// Additional settings
#define REG_LR_PLLHOP                               0x4400
#define REG_LR_TCXO                                 0x4B00//TCXO or XTAL input setting
#define REG_LR_PADAC                                0x4D00//Higher power settings of the PA
#define REG_LR_FORMERTEMP                           0x5B00//Stored temperature during the former IQ Calibration

#define REG_LR_AGCREF                               0x6100//Adjustment of the AGC thresholds
#define REG_LR_AGCTHRESH1                           0x6200
#define REG_LR_AGCTHRESH2                           0x6300
#define REG_LR_AGCTHRESH3                           0x6400

#endif //__SX1276_LORA_H__

