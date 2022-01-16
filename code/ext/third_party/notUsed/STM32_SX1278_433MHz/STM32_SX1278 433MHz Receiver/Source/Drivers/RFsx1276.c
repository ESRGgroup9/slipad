#include "RFsx1276.h"
#include "delay.h"
#include "main.h"
#include "stm32f10x.h"
#include "stm32f10x_spi.h"

#define SysTick_CTRL_ENABLE_Pos             0                                             /*!< SysTick CTRL: ENABLE Position */
#define SysTick_CTRL_ENABLE_Msk            (1ul << SysTick_CTRL_ENABLE_Pos)   

#define LORA     1  

RF rf;
tSX1276* SX1276;
tSX1276LR* SX1276LR;
uint8_t SX1276Regs[0x70];
static uint16_t TxPacketSize = 0;
static uint16_t RxPacketSize = 0;
static uint8_t RxGain = 1;
static uint8_t TxRxFlag=0;
static double RxPacketRssiValue;
static int8_t RxPacketSnrEstimate;
static uint32_t RxTimeoutTimer = 0;
static uint32_t TxTimeoutTimer = 0;
static uint8_t RFBuffer[RF_BUFFER_SIZE];
static uint8_t RFLRState = RFLR_STATE_IDLE;
static uint8_t RFState = RF_STATE_IDLE;
static bool LoRaOn = false;
static bool LoRaOnState = false;
volatile uint32_t TickCounter = 0;

#define RF_SEND_CYCLE                300 

tLoRaSettings LoRaSettings =
{
    433000000,        
    20,               
    6,                
                      
    7,                
    2,                
    true,             
    false,            
    1,                
    0,                
    4,                
    RF_SEND_CYCLE,    
    RF_SEND_CYCLE,    
    6,              
};

tFskSettings FskSettings = 
{
    870000000,      
    9600,           
    50000,          
    20,             
    100000,         
    150000,         
    true,           
    true,           
    255             
};
const double RssiOffsetLF[] =
{   
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
    -155.0,
};
const double RssiOffsetHF[] =
{   
    -150.0,
    -150.0,
    -150.0,
    -150.0,
    -150.0,
    -150.0,
    -150.0,
    -150.0,
    -150.0,
    -150.0,
};

const double SignalBwLog[] =
{
    3.8927900303521316335038277369285,  // 7.8 kHz
    4.0177301567005500940384239336392,  // 10.4 kHz
    4.193820026016112828717566631653,   // 15.6 kHz
    4.31875866931372901183597627752391, // 20.8 kHz
    4.4948500216800940239313055263775,  // 31.2 kHz
    4.6197891057238405255051280399961,  // 41.6 kHz
    4.795880017344075219145044421102,   // 62.5 kHz
    5.0969100130080564143587833158265,  // 125 kHz
    5.397940008672037609572522210551,   // 250 kHz
    5.6989700043360188047862611052755   // 500 kHz
};

const int32_t HoppingFrequencies[] =
{
    916500000,
    923500000,
    906500000,
    917500000,
    917500000,
    909000000,
    903000000,
    916000000,
    912500000,
    926000000,
    925000000,
    909500000,
    913000000,
    918500000,
    918500000,
    902500000,
    911500000,
    926500000,
    902500000,
    922000000,
    924000000,
    903500000,
    913000000,
    922000000,
    926000000,
    910000000,
    920000000,
    922500000,
    911000000,
    922000000,
    909500000,
    926000000,
    922000000,
    918000000,
    925500000,
    908000000,
    917500000,
    926500000,
    908500000,
    916000000,
    905500000,
    916000000,
    903000000,
    905000000,
    915000000,
    913000000,
    907000000,
    910000000,
    926500000,
    925500000,
    911000000,
};


void SX1276InitIo( void )
{
   GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE );

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // Configure NSS as output
    GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_SET );
    GPIO_InitStructure.GPIO_Pin = NSS_PIN;
    GPIO_Init( NSS_IOPORT, &GPIO_InitStructure );
		
    // Configure radio DIO as inputs
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    // Configure DIO0
    GPIO_InitStructure.GPIO_Pin =  DIO0_PIN;
    GPIO_Init( DIO0_IOPORT, &GPIO_InitStructure );
    
    // Configure DIO1
    GPIO_InitStructure.GPIO_Pin =  DIO1_PIN;
    GPIO_Init( DIO1_IOPORT, &GPIO_InitStructure );
    
    // Configure DIO2
    GPIO_InitStructure.GPIO_Pin =  DIO2_PIN;
    GPIO_Init( DIO2_IOPORT, &GPIO_InitStructure );
    
    // Configure DIO3 as input
    GPIO_InitStructure.GPIO_Pin =  DIO3_PIN;
    GPIO_Init( DIO3_IOPORT, &GPIO_InitStructure );
    
    // Configure DIO5 as input
    GPIO_InitStructure.GPIO_Pin =  DIO5_PIN;
    GPIO_Init( DIO5_IOPORT, &GPIO_InitStructure );
}


u8 SpiInOut(uint8_t input)
{
	 	/* Loop while DR register in not emplty */
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI2, input);

  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);	
	

}
void SX1276SetReset( uint8_t state )
{
    GPIO_InitTypeDef GPIO_InitStructure;

    if( state == RADIO_RESET_ON )
    {
        // Set RESET pin to 0
        GPIO_WriteBit( RESET_IOPORT, RESET_PIN, Bit_RESET );
        // Configure RESET as output
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Pin = RESET_PIN;
        GPIO_Init( RESET_IOPORT, &GPIO_InitStructure );
    }
    else
    {
        // Configure RESET as input
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Pin =  RESET_PIN;
        GPIO_Init( RESET_IOPORT, &GPIO_InitStructure );
    }
}

void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;
    //NSS = 0;
    GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_RESET );
    SpiInOut( addr | 0x80 );
    for( i = 0; i < size; i++ )
    {
        SpiInOut( buffer[i] );
    }
    //NSS = 1;
    GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_SET );
}

void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;
    //NSS = 0;
    GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_RESET );

    SpiInOut( addr & 0x7F );

    for( i = 0; i < size; i++ )
    {
        buffer[i] = SpiInOut( 0 );
    }
    GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_SET );
}

void SX1276Write( uint8_t addr, uint8_t data )
{
    SX1276WriteBuffer( addr, &data, 1 );
}

void SX1276Read( uint8_t addr, uint8_t *data )
{
    SX1276ReadBuffer( addr, data, 1 );
}
void SX1276SetIdleState(void)
{
 RFState=RF_STATE_IDLE;
}
void SX1276WriteFifo( uint8_t *buffer, uint8_t size )
{
    SX1276WriteBuffer( 0, buffer, size );
}

void SX1276ReadFifo( uint8_t *buffer, uint8_t size )
{
    SX1276ReadBuffer( 0, buffer, size );
}

inline uint8_t SX1276ReadDio0( void )
{
    return GPIO_ReadInputDataBit( DIO0_IOPORT, DIO0_PIN );
}

inline uint8_t SX1276ReadDio1( void )
{
    return GPIO_ReadInputDataBit( DIO1_IOPORT, DIO1_PIN );
}

inline uint8_t SX1276ReadDio2( void )
{
    return GPIO_ReadInputDataBit( DIO2_IOPORT, DIO2_PIN );
}

inline uint8_t SX1276ReadDio3( void )
{
	  return GPIO_ReadInputDataBit( DIO3_IOPORT, DIO3_PIN );
}

inline uint8_t SX1276ReadDio4( void )
{
	  return GPIO_ReadInputDataBit( DIO4_IOPORT, DIO4_PIN );
}

inline uint8_t SX1276ReadDio5( void )
{
	  return GPIO_ReadInputDataBit( DIO5_IOPORT, DIO5_PIN );
}

inline void SX1276WriteRxTx( uint8_t txEnable )
{

}

void SX1276Reset( void )
{
    SX1276SetReset( RADIO_RESET_ON );
    delay_ms(1);
    SX1276SetReset( RADIO_RESET_OFF );
    delay_ms(6);
}


void SX1276LoRaSetOpMode( uint8_t opMode )
{
    static uint8_t opModePrev = RFLR_OPMODE_STANDBY;
    static bool antennaSwitchTxOnPrev = true;
    bool antennaSwitchTxOn = false;

    opModePrev = SX1276LR->RegOpMode & ~RFLR_OPMODE_MASK;

    if( opMode != opModePrev )
    {
        if( opMode == RFLR_OPMODE_TRANSMITTER )
        {
            antennaSwitchTxOn = true;
        }
        else
        {
            antennaSwitchTxOn = false;
        }
        if( antennaSwitchTxOn != antennaSwitchTxOnPrev )
        {
            antennaSwitchTxOnPrev = antennaSwitchTxOn;
            RXTX( antennaSwitchTxOn ); // Antenna switch control
        }
        SX1276LR->RegOpMode = ( SX1276LR->RegOpMode & RFLR_OPMODE_MASK ) | opMode;

        SX1276Write( REG_LR_OPMODE, SX1276LR->RegOpMode );        
    }
}

void SX1276SetLoRaOn( bool enable )
{
    if( LoRaOnState == enable )
    {
        return;
    }
    LoRaOnState = enable;
    LoRaOn = enable;

    if( LoRaOn == true )
    {
        SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );
        
        SX1276LR->RegOpMode = ( SX1276LR->RegOpMode & RFLR_OPMODE_LONGRANGEMODE_MASK ) | RFLR_OPMODE_LONGRANGEMODE_ON;
        SX1276Write( REG_LR_OPMODE, SX1276LR->RegOpMode );
        
        SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
                                        // RxDone               RxTimeout                   FhssChangeChannel           CadDone
        SX1276LR->RegDioMapping1 = RFLR_DIOMAPPING1_DIO0_00 | RFLR_DIOMAPPING1_DIO1_00 | RFLR_DIOMAPPING1_DIO2_00 | RFLR_DIOMAPPING1_DIO3_00;
                                        // CadDetected          ModeReady
        SX1276LR->RegDioMapping2 = RFLR_DIOMAPPING2_DIO4_00 | RFLR_DIOMAPPING2_DIO5_00;
        SX1276WriteBuffer( REG_LR_DIOMAPPING1, &SX1276LR->RegDioMapping1, 2 );
        
        SX1276ReadBuffer( REG_LR_OPMODE, SX1276Regs + 1, 0x70 - 1 );
    }
    else
    {
        SX1276LoRaSetOpMode( RFLR_OPMODE_SLEEP );
        
        SX1276LR->RegOpMode = ( SX1276LR->RegOpMode & RFLR_OPMODE_LONGRANGEMODE_MASK ) | RFLR_OPMODE_LONGRANGEMODE_OFF;
        SX1276Write( REG_LR_OPMODE, SX1276LR->RegOpMode );
        
        SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
        
        SX1276ReadBuffer( REG_OPMODE, SX1276Regs + 1, 0x70 - 1 );
    }
}

void SX1276LoRaSetDefaults( void )
{
    SX1276Read( REG_LR_VERSION, &SX1276LR->RegVersion );
}
void SX1276LoRaSetRFFrequency( uint32_t freq )
{
    LoRaSettings.RFFrequency = freq;

    freq = ( uint32_t )( ( double )freq / ( double )FREQ_STEP );
    SX1276LR->RegFrfMsb = ( uint8_t )( ( freq >> 16 ) & 0xFF );
    SX1276LR->RegFrfMid = ( uint8_t )( ( freq >> 8 ) & 0xFF );
    SX1276LR->RegFrfLsb = ( uint8_t )( freq & 0xFF );
    SX1276WriteBuffer( REG_LR_FRFMSB, &SX1276LR->RegFrfMsb, 3 );
}
void SX1276LoRaSetNbTrigPeaks( uint8_t value )
{
    SX1276Read( 0x31, &SX1276LR->RegTestReserved31 );
    SX1276LR->RegTestReserved31 = ( SX1276LR->RegTestReserved31 & 0xF8 ) | value;
    SX1276Write( 0x31, SX1276LR->RegTestReserved31 );
}
void SX1276LoRaSetSpreadingFactor( uint8_t factor )
{

    if( factor > 12 )
    {
        factor = 12;
    }
    else if( factor < 6 )
    {
        factor = 6;
    }

    if( factor == 6 )
    {
        SX1276LoRaSetNbTrigPeaks( 5 );
    }
    else
    {
        SX1276LoRaSetNbTrigPeaks( 3 );
    }

    SX1276Read( REG_LR_MODEMCONFIG2, &SX1276LR->RegModemConfig2 );    
    SX1276LR->RegModemConfig2 = ( SX1276LR->RegModemConfig2 & RFLR_MODEMCONFIG2_SF_MASK ) | ( factor << 4 );
    SX1276Write( REG_LR_MODEMCONFIG2, SX1276LR->RegModemConfig2 );    
    LoRaSettings.SpreadingFactor = factor;
}
void SX1276LoRaSetErrorCoding( uint8_t value )
{
    SX1276Read( REG_LR_MODEMCONFIG1, &SX1276LR->RegModemConfig1 );
    SX1276LR->RegModemConfig1 = ( SX1276LR->RegModemConfig1 & RFLR_MODEMCONFIG1_CODINGRATE_MASK ) | ( value << 1 );
    SX1276Write( REG_LR_MODEMCONFIG1, SX1276LR->RegModemConfig1 );
    LoRaSettings.ErrorCoding = value;
}
void SX1276LoRaSetPacketCrcOn( bool enable )
{
    SX1276Read( REG_LR_MODEMCONFIG2, &SX1276LR->RegModemConfig2 );
    SX1276LR->RegModemConfig2 = ( SX1276LR->RegModemConfig2 & RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK ) | ( enable << 2 );
    SX1276Write( REG_LR_MODEMCONFIG2, SX1276LR->RegModemConfig2 );
    LoRaSettings.CrcOn = enable;
}
void SX1276LoRaSetSignalBandwidth( uint8_t bw )
{
    SX1276Read( REG_LR_MODEMCONFIG1, &SX1276LR->RegModemConfig1 );
    SX1276LR->RegModemConfig1 = ( SX1276LR->RegModemConfig1 & RFLR_MODEMCONFIG1_BW_MASK ) | ( bw << 4 );
    SX1276Write( REG_LR_MODEMCONFIG1, SX1276LR->RegModemConfig1 );
    LoRaSettings.SignalBw = bw;
}
void SX1276LoRaSetImplicitHeaderOn( bool enable )
{
    SX1276Read( REG_LR_MODEMCONFIG1, &SX1276LR->RegModemConfig1 );
    SX1276LR->RegModemConfig1 = ( SX1276LR->RegModemConfig1 & RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK ) | ( enable );
    SX1276Write( REG_LR_MODEMCONFIG1, SX1276LR->RegModemConfig1 );
    LoRaSettings.ImplicitHeaderOn = enable;
}
void SX1276LoRaSetSymbTimeout( uint16_t value )
{
    SX1276ReadBuffer( REG_LR_MODEMCONFIG2, &SX1276LR->RegModemConfig2, 2 );

    SX1276LR->RegModemConfig2 = ( SX1276LR->RegModemConfig2 & RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK ) | ( ( value >> 8 ) & ~RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK );
    SX1276LR->RegSymbTimeoutLsb = value & 0xFF;
    SX1276WriteBuffer( REG_LR_MODEMCONFIG2, &SX1276LR->RegModemConfig2, 2 );
}
void SX1276LoRaSetPayloadLength( uint8_t value )
{
    SX1276LR->RegPayloadLength = value;
    SX1276Write( REG_LR_PAYLOADLENGTH, SX1276LR->RegPayloadLength );
    LoRaSettings.PayloadLength = value;
}
void SX1276LoRaSetLowDatarateOptimize( bool enable )
{
    SX1276Read( REG_LR_MODEMCONFIG3, &SX1276LR->RegModemConfig3 );
    SX1276LR->RegModemConfig3 = ( SX1276LR->RegModemConfig3 & RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_MASK ) | ( enable << 3 );
    SX1276Write( REG_LR_MODEMCONFIG3, SX1276LR->RegModemConfig3 );
}
void SX1276LoRaSetPAOutput( uint8_t outputPin )
{
    SX1276Read( REG_LR_PACONFIG, &SX1276LR->RegPaConfig );
    SX1276LR->RegPaConfig = (SX1276LR->RegPaConfig & RFLR_PACONFIG_PASELECT_MASK ) | outputPin;
    SX1276Write( REG_LR_PACONFIG, SX1276LR->RegPaConfig );
}
void SX1276LoRaSetPa20dBm( bool enale )
{
    SX1276Read( REG_LR_PADAC, &SX1276LR->RegPaDac );
    SX1276Read( REG_LR_PACONFIG, &SX1276LR->RegPaConfig );

    if( ( SX1276LR->RegPaConfig & RFLR_PACONFIG_PASELECT_PABOOST ) == RFLR_PACONFIG_PASELECT_PABOOST )
    {    
        if( enale == true )
        {
            SX1276LR->RegPaDac = 0x87;
        }
    }
    else
    {
        SX1276LR->RegPaDac = 0x84;
    }
    SX1276Write( REG_LR_PADAC, SX1276LR->RegPaDac );
}
void SX1276LoRaSetRFPower( int8_t power )
{
    SX1276Read( REG_LR_PACONFIG, &SX1276LR->RegPaConfig );
    SX1276Read( REG_LR_PADAC, &SX1276LR->RegPaDac );
    
    if( ( SX1276LR->RegPaConfig & RFLR_PACONFIG_PASELECT_PABOOST ) == RFLR_PACONFIG_PASELECT_PABOOST )
    {
        if( ( SX1276LR->RegPaDac & 0x87 ) == 0x87 )
        {
            if( power < 5 )
            {
                power = 5;
            }
            if( power > 20 )
            {
                power = 20;
            }
            SX1276LR->RegPaConfig = ( SX1276LR->RegPaConfig & RFLR_PACONFIG_MAX_POWER_MASK ) | 0x70;
            SX1276LR->RegPaConfig = ( SX1276LR->RegPaConfig & RFLR_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 5 ) & 0x0F );
        }
        else
        {
            if( power < 2 )
            {
                power = 2;
            }
            if( power > 17 )
            {
                power = 17;
            }
            SX1276LR->RegPaConfig = ( SX1276LR->RegPaConfig & RFLR_PACONFIG_MAX_POWER_MASK ) | 0x70;
            SX1276LR->RegPaConfig = ( SX1276LR->RegPaConfig & RFLR_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 2 ) & 0x0F );
        }
    }
    else
    {
        if( power < -1 )
        {
            power = -1;
        }
        if( power > 14 )
        {
            power = 14;
        }
        SX1276LR->RegPaConfig = ( SX1276LR->RegPaConfig & RFLR_PACONFIG_MAX_POWER_MASK ) | 0x70;
        SX1276LR->RegPaConfig = ( SX1276LR->RegPaConfig & RFLR_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power + 1 ) & 0x0F );
    }
    SX1276Write( REG_LR_PACONFIG, SX1276LR->RegPaConfig );
    LoRaSettings.Power = power;
}
void SX1276LoRaInit( void )
{
    RFLRState = RFLR_STATE_IDLE;

    SX1276LoRaSetDefaults( );
    
    SX1276ReadBuffer( REG_LR_OPMODE, SX1276Regs + 1, 0x70 - 1 );
    
    SX1276LR->RegLna = RFLR_LNA_GAIN_G1;

    SX1276WriteBuffer( REG_LR_OPMODE, SX1276Regs + 1, 0x70 - 1 );
	  //debug
    SX1276ReadBuffer( REG_LR_OPMODE, SX1276Regs + 1, 0x70 - 1 );
	
    // set the RF settings 
    SX1276LoRaSetRFFrequency( LoRaSettings.RFFrequency );
    SX1276LoRaSetSpreadingFactor( LoRaSettings.SpreadingFactor ); // SF6 only operates in implicit header mode.
    SX1276LoRaSetErrorCoding( LoRaSettings.ErrorCoding );
    SX1276LoRaSetPacketCrcOn( LoRaSettings.CrcOn );
    SX1276LoRaSetSignalBandwidth( LoRaSettings.SignalBw );

    SX1276LoRaSetImplicitHeaderOn( LoRaSettings.ImplicitHeaderOn );
    SX1276LoRaSetSymbTimeout( 0x3FF );
    SX1276LoRaSetPayloadLength( LoRaSettings.PayloadLength );
    SX1276LoRaSetLowDatarateOptimize( true );
		//debug
    SX1276ReadBuffer( REG_LR_OPMODE, SX1276Regs + 1, 0x70 - 1 );
		
#if( ( MODULE_SX1276RF1IAS == 1 ) || ( MODULE_SX1276RF1KAS == 1 ) )
    if( LoRaSettings.RFFrequency > 860000000 )
    {
        SX1276LoRaSetPAOutput( RFLR_PACONFIG_PASELECT_RFO );
        SX1276LoRaSetPa20dBm( false );
        LoRaSettings.Power = 14;
        SX1276LoRaSetRFPower( LoRaSettings.Power );
    }
    else
    {
        SX1276LoRaSetPAOutput( RFLR_PACONFIG_PASELECT_PABOOST );
        SX1276LoRaSetPa20dBm( true );
        LoRaSettings.Power = 20;
        SX1276LoRaSetRFPower( LoRaSettings.Power );
    } 
#elif( MODULE_SX1276RF1JAS == 1 )
    if( LoRaSettings.RFFrequency > 860000000 )
    {
        SX1276LoRaSetPAOutput( RFLR_PACONFIG_PASELECT_PABOOST );
        SX1276LoRaSetPa20dBm( true );
        LoRaSettings.Power = 20;
        SX1276LoRaSetRFPower( LoRaSettings.Power );
    }
    else
    {
        SX1276LoRaSetPAOutput( RFLR_PACONFIG_PASELECT_RFO );
        SX1276LoRaSetPa20dBm( false );
        LoRaSettings.Power = 14;
        SX1276LoRaSetRFPower( LoRaSettings.Power );
    } 
#endif

    SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
}

void SX1276Init( void )
{
    // Initialize FSK and LoRa registers structure
    SX1276 = ( tSX1276* )SX1276Regs;
    SX1276LR = ( tSX1276LR* )SX1276Regs;
    SX1276InitIo( );
    SX1276Reset( );
    // REMARK: After radio reset the default modem is 
    LoRaOn = true;
    SX1276SetLoRaOn( LoRaOn );
    SX1276LoRaInit( );
}
void SX1276LoRaSetRFState( uint8_t state )
{
    RFLRState = state;
}
uint8_t SX1276LoRaReadRxGain( void )
{
    SX1276Read( REG_LR_LNA, &SX1276LR->RegLna );
    return( SX1276LR->RegLna >> 5 ) & 0x07;
}

void SX1276StartRx( void )
{
  SX1276LoRaSetRFState( RFLR_STATE_RX_INIT );
}

void SX1276LoRaGetRxPacket( void *buffer, uint16_t *size )
{
    *size = RxPacketSize;
    RxPacketSize = 0;
    memcpy( ( void * )buffer, ( void * )RFBuffer, ( size_t )*size );
}

void SX1276GetRxPacket( void *buffer, uint16_t *size )
{
   SX1276LoRaGetRxPacket( buffer, size );
}
void SX1276LoRaSetTxPacket( const void *buffer, uint16_t size )
{
    if( LoRaSettings.FreqHopOn == false )
    {
        TxPacketSize = size;
    }
    else
    {
        TxPacketSize = 255;
    }
    memcpy( ( void * )RFBuffer, buffer, ( size_t )TxPacketSize ); 

    RFLRState = RFLR_STATE_TX_INIT;
}
void SX1276SetTxPacket( const void *buffer, uint16_t size )
{
  SX1276LoRaSetTxPacket( buffer, size );
}
uint8_t SX1276FskReadRxGain( void )
{
    SX1276Read( REG_LNA, &SX1276->RegLna );
    return( SX1276->RegLna >> 5 ) & 0x07;
}

void SX1276_Rx_Count_Time(void)
{
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;  
	SysTick->LOAD=(u32)9000*(LoRaSettings.RxPacketTimeout);
	SysTick->VAL =0x00;    
  SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;
}

void SX1276_Tx_Count_Time(void)   
{
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;  
	SysTick->LOAD=(u32)9000*(LoRaSettings.TxPacketTimeout);
	SysTick->VAL =0x00;        
  SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;
}
uint32_t SX1276LoRaProcess1( void )
{
    uint32_t result = RF_BUSY;
    
    switch( RFLRState )
			{
				case RFLR_STATE_IDLE:
				break;
				case RFLR_STATE_RX_INIT:  
								SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
								SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |RFLR_IRQFLAGS_VALIDHEADER |RFLR_IRQFLAGS_TXDONE |RFLR_IRQFLAGS_CADDONE |RFLR_IRQFLAGS_CADDETECTED;
								SX1276Write( REG_LR_IRQFLAGSMASK, SX1276LR->RegIrqFlagsMask );
								if( LoRaSettings.FreqHopOn == true )
										{
												SX1276LR->RegHopPeriod = LoRaSettings.HopPeriod;
												SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
												SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
										}
								else
										{
												SX1276LR->RegHopPeriod = 255;
										}
								SX1276Write( REG_LR_HOPPERIOD, SX1276LR->RegHopPeriod );		
								SX1276LoRaSetOpMode( RFLR_OPMODE_RECEIVER_SINGLE );
								memset( RFBuffer, 0, ( size_t )RF_BUFFER_SIZE );
								SX1276_Rx_Count_Time();
								RFLRState = RFLR_STATE_RX_RUNNING;
				break;
				case RFLR_STATE_RX_RUNNING:
								SX1276Read(REG_LR_IRQFLAGS,&TxRxFlag );
								if((TxRxFlag&0x40)==0x40)
										{	
											TxRxFlag=0X00;								
											if( LoRaSettings.FreqHopOn == true )
													{
															SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
															SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
													}
											SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_RXDONE  );					
											RFLRState = RFLR_STATE_RX_DONE;
										}
								if((TxRxFlag&0x02)==0x02)
										{ 
											TxRxFlag=0X00;
											if( LoRaSettings.FreqHopOn == true )
													{
															SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
															SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
													}
											SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL );// Clear Irq
										}
								else
										{
											TxRxFlag=0X00;
											RxTimeoutTimer=SysTick->CTRL;
											if((RxTimeoutTimer&0x01&&!(RxTimeoutTimer&(1<<16)))==0)
													{
														SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
														SysTick->VAL =0X00;                     
														RFLRState = RFLR_STATE_RX_TIMEOUT;
													}
										}
				break;
				case RFLR_STATE_RX_DONE:
								SX1276Read( REG_LR_IRQFLAGS, &SX1276LR->RegIrqFlags );
								if( ( SX1276LR->RegIrqFlags & RFLR_IRQFLAGS_PAYLOADCRCERROR ) == RFLR_IRQFLAGS_PAYLOADCRCERROR )
											{
													SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_PAYLOADCRCERROR  );
													RFLRState = RFLR_STATE_RX_INIT;
												  break;
											}
								
								{
										uint8_t rxSnrEstimate;
										SX1276Read( REG_LR_PKTSNRVALUE, &rxSnrEstimate );
										if( rxSnrEstimate & 0x80 ) 
												{
														RxPacketSnrEstimate = ( ( ~rxSnrEstimate + 1 ) & 0xFF ) >> 2;
														RxPacketSnrEstimate = -RxPacketSnrEstimate;
												}
										else
												{
														RxPacketSnrEstimate = ( rxSnrEstimate & 0xFF ) >> 2;
												}
								}
								
								if( LoRaSettings.RFFrequency < 860000000 )  
										{    
												if( RxPacketSnrEstimate < 0 )
														{
																RxPacketRssiValue = NOISE_ABSOLUTE_ZERO + 10.0 * SignalBwLog[LoRaSettings.SignalBw] + NOISE_FIGURE_LF + ( double )RxPacketSnrEstimate;
														}
												else
														{    
																SX1276Read( REG_LR_PKTRSSIVALUE, &SX1276LR->RegPktRssiValue );
																RxPacketRssiValue = RssiOffsetLF[LoRaSettings.SignalBw] + ( double )SX1276LR->RegPktRssiValue;
														}
										}
								else                                        // HF
										{    
												if( RxPacketSnrEstimate < 0 )
														{
																RxPacketRssiValue = NOISE_ABSOLUTE_ZERO + 10.0 * SignalBwLog[LoRaSettings.SignalBw] + NOISE_FIGURE_HF + ( double )RxPacketSnrEstimate;
														}
												else
														{    
																SX1276Read( REG_LR_PKTRSSIVALUE, &SX1276LR->RegPktRssiValue );
																RxPacketRssiValue = RssiOffsetHF[LoRaSettings.SignalBw] + ( double )SX1276LR->RegPktRssiValue;
														}
										}
								SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoRxBaseAddr;
								SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
								if( LoRaSettings.ImplicitHeaderOn == true )
										{
												RxPacketSize = SX1276LR->RegPayloadLength;
												SX1276ReadFifo( RFBuffer, SX1276LR->RegPayloadLength );
										}
								else
										{
												SX1276Read( REG_LR_NBRXBYTES, &SX1276LR->RegNbRxBytes );
												RxPacketSize = SX1276LR->RegNbRxBytes;
												SX1276ReadFifo( RFBuffer, SX1276LR->RegNbRxBytes );
										}
								SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; 
								SysTick->VAL =0X00;                      
								RFLRState = RFLR_STATE_RX_INIT;
								result = RF_RX_DONE;
				break;
				case RFLR_STATE_RX_TIMEOUT:
								RFLRState = RFLR_STATE_RX_INIT;
								result = RF_RX_TIMEOUT;
				break;
				case RFLR_STATE_TX_INIT:
								SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
								if( LoRaSettings.FreqHopOn == true )
										{
												SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |RFLR_IRQFLAGS_RXDONE |RFLR_IRQFLAGS_PAYLOADCRCERROR |RFLR_IRQFLAGS_VALIDHEADER |RFLR_IRQFLAGS_CADDONE | RFLR_IRQFLAGS_CADDETECTED;
												SX1276LR->RegHopPeriod = LoRaSettings.HopPeriod;
												SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
												SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
										}
								else
										{
												SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |RFLR_IRQFLAGS_RXDONE |RFLR_IRQFLAGS_PAYLOADCRCERROR |RFLR_IRQFLAGS_VALIDHEADER |RFLR_IRQFLAGS_CADDONE |RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |RFLR_IRQFLAGS_CADDETECTED;
												SX1276LR->RegHopPeriod = 0;
										}
								SX1276Write( REG_LR_HOPPERIOD, SX1276LR->RegHopPeriod );
								SX1276Write( REG_LR_IRQFLAGSMASK, SX1276LR->RegIrqFlagsMask );
								SX1276LR->RegPayloadLength = TxPacketSize;
								SX1276Write( REG_LR_PAYLOADLENGTH, SX1276LR->RegPayloadLength );
								SX1276LR->RegFifoTxBaseAddr = 0x00; // Full buffer used for Tx
								SX1276Write( REG_LR_FIFOTXBASEADDR, SX1276LR->RegFifoTxBaseAddr );
								SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoTxBaseAddr;
								SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
								SX1276WriteFifo( RFBuffer, SX1276LR->RegPayloadLength );
								SX1276LoRaSetOpMode( RFLR_OPMODE_TRANSMITTER );
								SX1276_Tx_Count_Time();
								RFLRState = RFLR_STATE_TX_RUNNING;
				break;
				case RFLR_STATE_TX_RUNNING:
								SX1276Read(REG_LR_IRQFLAGS,&TxRxFlag );
								if((TxRxFlag&0x08)==0x08)
										{	
											TxRxFlag=0X00;								
											SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_TXDONE  );
											SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
											SysTick->VAL =0X00;                     
											RFLRState = RFLR_STATE_TX_DONE;   
										}
							 if((TxRxFlag&0x02)==0x02)
										{ 
											TxRxFlag=0X00;
											if( LoRaSettings.FreqHopOn == true )
													{
															SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
															SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
													}
											SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL );
										}
								else
										{
											TxRxFlag=0X00;
											TxTimeoutTimer=SysTick->CTRL;
											if((TxTimeoutTimer&0x01&&!(TxTimeoutTimer&(1<<16)))==0)
													{
														SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
														SysTick->VAL =0X00;                    
														RFLRState = RFLR_STATE_TX_TIMEOUT;
													}
										}
				break;
				case RFLR_STATE_TX_DONE:
								SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
								RFLRState = RFLR_STATE_RX_INIT;
								result = RF_TX_DONE;
				break;
				case RFLR_STATE_TX_TIMEOUT:
					      SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
								RFLRState = RFLR_STATE_RX_INIT;
								result = RF_TX_TIMEOUT;
				break;
			} 
   return result;
}

/*用户读RSSI*/
double SX1276LoRaReadRssi( void )
{
    // Reads the RSSI value
    SX1276Read( REG_LR_PKTRSSIVALUE, &SX1276LR->RegPktRssiValue );

    if( LoRaSettings.RFFrequency < 860000000 )  // LF
    {
        return RssiOffsetLF[LoRaSettings.SignalBw] + ( double )SX1276LR->RegPktRssiValue;		
    }
    else
    {
        return RssiOffsetHF[LoRaSettings.SignalBw] + ( double )SX1276LR->RegPktRssiValue;
    }
}
uint8_t  usr_ReadRssi( void )
{
    // Reads the RSSI value
    SX1276Read( REG_LR_PKTRSSIVALUE, &SX1276LR->RegPktRssiValue );
    return  SX1276LR->RegPktRssiValue ; // for debug 
}
/*用户读RSSI*/

uint32_t SX1276LoRaProcess2( void )
{
    uint32_t result = RF_BUSY;
    
    switch( RFLRState )
			{
					case RFLR_STATE_IDLE:
					break;
					case RFLR_STATE_RX_INIT:        
								SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
								SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |RFLR_IRQFLAGS_VALIDHEADER |RFLR_IRQFLAGS_TXDONE |RFLR_IRQFLAGS_CADDONE |RFLR_IRQFLAGS_CADDETECTED;
								SX1276Write( REG_LR_IRQFLAGSMASK, SX1276LR->RegIrqFlagsMask );
								if( LoRaSettings.FreqHopOn == true )
										{
												SX1276LR->RegHopPeriod = LoRaSettings.HopPeriod;
												SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
												SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
										}
									else
										{
												SX1276LR->RegHopPeriod = 255;
										}
								SX1276Write( REG_LR_HOPPERIOD, SX1276LR->RegHopPeriod );
																						// RxDone                    RxTimeout                   FhssChangeChannel           CadDone
								SX1276LR->RegDioMapping1 = RFLR_DIOMAPPING1_DIO0_00 | RFLR_DIOMAPPING1_DIO1_00 | RFLR_DIOMAPPING1_DIO2_00 | RFLR_DIOMAPPING1_DIO3_00;
																						// CadDetected               ModeReady
								SX1276LR->RegDioMapping2 = RFLR_DIOMAPPING2_DIO4_00 | RFLR_DIOMAPPING2_DIO5_00;
								SX1276WriteBuffer( REG_LR_DIOMAPPING1, &SX1276LR->RegDioMapping1, 2 );
								if( LoRaSettings.RxSingleOn == true ) // Rx single mode
										{
												SX1276LoRaSetOpMode( RFLR_OPMODE_RECEIVER_SINGLE );
										}
								else // Rx continuous mode
										{
												SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoRxBaseAddr;
												SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
												SX1276LoRaSetOpMode( RFLR_OPMODE_RECEIVER );
										}
								memset( RFBuffer, 0, ( size_t )RF_BUFFER_SIZE );
								SX1276_Rx_Count_Time();
								RFLRState = RFLR_STATE_RX_RUNNING;
					break;
					case RFLR_STATE_RX_RUNNING:
									if( DIO0 == 1 ) 
											{
													SX1276_Rx_Count_Time();
													if( LoRaSettings.FreqHopOn == true )
															{
																	SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
																	SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
															}
													SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_RXDONE  );
													RFLRState = RFLR_STATE_RX_DONE;
											}
									if( DIO2 == 1 ) 
											{
													SX1276_Rx_Count_Time();
													if( LoRaSettings.FreqHopOn == true )
															{
																	SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
																	SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
															}
													SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL );
											}
							
									if( LoRaSettings.RxSingleOn == true ) 
											{
												RxTimeoutTimer=SysTick->CTRL;
												if((RxTimeoutTimer&0x01&&!(RxTimeoutTimer&(1<<16)))==0)
														{
															SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
															SysTick->VAL =0X00;                     
															RFLRState = RFLR_STATE_RX_TIMEOUT;
														}
											}
					break;
					case RFLR_STATE_RX_DONE:
									SX1276Read( REG_LR_IRQFLAGS, &SX1276LR->RegIrqFlags );
									if( ( SX1276LR->RegIrqFlags & RFLR_IRQFLAGS_PAYLOADCRCERROR ) == RFLR_IRQFLAGS_PAYLOADCRCERROR )
											{	
													SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_PAYLOADCRCERROR  );					
													if( LoRaSettings.RxSingleOn == true ) 
														{
																RFLRState = RFLR_STATE_RX_INIT;
														}
													else
														{
																RFLRState = RFLR_STATE_RX_RUNNING;
														}
													break;
											}
									
									{
											uint8_t rxSnrEstimate;
											SX1276Read( REG_LR_PKTSNRVALUE, &rxSnrEstimate );
											if( rxSnrEstimate & 0x80 ) // The SNR sign bit is 1
											{
													// Invert and divide by 4
													RxPacketSnrEstimate = ( ( ~rxSnrEstimate + 1 ) & 0xFF ) >> 2;
													RxPacketSnrEstimate = -RxPacketSnrEstimate;
											}
											else
											{
													// Divide by 4
													RxPacketSnrEstimate = ( rxSnrEstimate & 0xFF ) >> 2;
											}
									}
									
									if( LoRaSettings.RFFrequency < 860000000 )  // LF
										{    
												if( RxPacketSnrEstimate < 0 )
													{
															RxPacketRssiValue = NOISE_ABSOLUTE_ZERO + 10.0 * SignalBwLog[LoRaSettings.SignalBw] + NOISE_FIGURE_LF + ( double )RxPacketSnrEstimate;
													}
												else
													{    
															SX1276Read( REG_LR_PKTRSSIVALUE, &SX1276LR->RegPktRssiValue );
															RxPacketRssiValue = RssiOffsetLF[LoRaSettings.SignalBw] + ( double )SX1276LR->RegPktRssiValue;
													}
										}
									else                                        // HF
										{    
												if( RxPacketSnrEstimate < 0 )
													{
															RxPacketRssiValue = NOISE_ABSOLUTE_ZERO + 10.0 * SignalBwLog[LoRaSettings.SignalBw] + NOISE_FIGURE_HF + ( double )RxPacketSnrEstimate;
													}
												else
													{    
															SX1276Read( REG_LR_PKTRSSIVALUE, &SX1276LR->RegPktRssiValue );
															RxPacketRssiValue = RssiOffsetHF[LoRaSettings.SignalBw] + ( double )SX1276LR->RegPktRssiValue;
													}
										}

									if( LoRaSettings.RxSingleOn == true ) // Rx single mode
										{
												SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoRxBaseAddr;
												SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
												if( LoRaSettings.ImplicitHeaderOn == true )
													{
															RxPacketSize = SX1276LR->RegPayloadLength;
															SX1276ReadFifo( RFBuffer, SX1276LR->RegPayloadLength );
													}
												else
													{
															SX1276Read( REG_LR_NBRXBYTES, &SX1276LR->RegNbRxBytes );
															RxPacketSize = SX1276LR->RegNbRxBytes;
															SX1276ReadFifo( RFBuffer, SX1276LR->RegNbRxBytes );
													}
										}
									else // Rx continuous mode
										{
												SX1276Read( REG_LR_FIFORXCURRENTADDR, &SX1276LR->RegFifoRxCurrentAddr );
												if( LoRaSettings.ImplicitHeaderOn == true )
													{
															RxPacketSize = SX1276LR->RegPayloadLength;
															SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoRxCurrentAddr;
															SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
															SX1276ReadFifo( RFBuffer, SX1276LR->RegPayloadLength );
													}
												else
													{
															SX1276Read( REG_LR_NBRXBYTES, &SX1276LR->RegNbRxBytes );
															RxPacketSize = SX1276LR->RegNbRxBytes;
															SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoRxCurrentAddr;
															SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
															SX1276ReadFifo( RFBuffer, SX1276LR->RegNbRxBytes );
													}
										}
									
									if( LoRaSettings.RxSingleOn == true ) // Rx single mode
										{
												RFLRState = RFLR_STATE_RX_INIT;
										}
									else // Rx continuous mode
										{
												RFLRState = RFLR_STATE_RX_RUNNING;
										}
									result = RF_RX_DONE;
					break;
					case RFLR_STATE_RX_TIMEOUT:
								RFLRState = RFLR_STATE_RX_INIT;
								result = RF_RX_TIMEOUT;
					break;
					case RFLR_STATE_TX_INIT:
									SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
									if( LoRaSettings.FreqHopOn == true )
										{
												SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |RFLR_IRQFLAGS_RXDONE |RFLR_IRQFLAGS_PAYLOADCRCERROR |RFLR_IRQFLAGS_VALIDHEADER |RFLR_IRQFLAGS_CADDONE |RFLR_IRQFLAGS_CADDETECTED;
												SX1276LR->RegHopPeriod = LoRaSettings.HopPeriod;
												SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
												SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
										}
									else
										{
												SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |RFLR_IRQFLAGS_RXDONE |RFLR_IRQFLAGS_PAYLOADCRCERROR |RFLR_IRQFLAGS_VALIDHEADER |RFLR_IRQFLAGS_CADDONE |RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |RFLR_IRQFLAGS_CADDETECTED;
												SX1276LR->RegHopPeriod = 0;
										}
									SX1276Write( REG_LR_HOPPERIOD, SX1276LR->RegHopPeriod );
									SX1276Write( REG_LR_IRQFLAGSMASK, SX1276LR->RegIrqFlagsMask );

									// Initializes the payload size
									SX1276LR->RegPayloadLength = TxPacketSize;
									SX1276Write( REG_LR_PAYLOADLENGTH, SX1276LR->RegPayloadLength );
									
									SX1276LR->RegFifoTxBaseAddr = 0x00; // Full buffer used for Tx
									SX1276Write( REG_LR_FIFOTXBASEADDR, SX1276LR->RegFifoTxBaseAddr );

									SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoTxBaseAddr;
									SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
									
									// Write payload buffer to LORA modem
									SX1276WriteFifo( RFBuffer, SX1276LR->RegPayloadLength );
																									// TxDone               RxTimeout                   FhssChangeChannel          ValidHeader         
									SX1276LR->RegDioMapping1 = RFLR_DIOMAPPING1_DIO0_01 | RFLR_DIOMAPPING1_DIO1_00 | RFLR_DIOMAPPING1_DIO2_00 | RFLR_DIOMAPPING1_DIO3_01;
																									// PllLock              Mode Ready
									SX1276LR->RegDioMapping2 = RFLR_DIOMAPPING2_DIO4_01 | RFLR_DIOMAPPING2_DIO5_00;
									SX1276WriteBuffer( REG_LR_DIOMAPPING1, &SX1276LR->RegDioMapping1, 2 );

									SX1276LoRaSetOpMode( RFLR_OPMODE_TRANSMITTER );
									 
									RFLRState = RFLR_STATE_TX_RUNNING;
					break;
					case RFLR_STATE_TX_RUNNING:
									if( DIO0 == 1 ) 
											{
													SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_TXDONE  );// Clear Irq
													RFLRState = RFLR_STATE_TX_DONE;   
											}
									if( DIO2 == 1 ) 
											{
													if( LoRaSettings.FreqHopOn == true )
														{
																SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
																SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
														}
													SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL );
											}
					break;
					case RFLR_STATE_TX_DONE:
									SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
									RFLRState = RFLR_STATE_RX_INIT;
									result = RF_TX_DONE;
					break;
			} 
    return result;
}
uint32_t SX1276LoRaProcess( void )
{
    uint32_t result = RF_BUSY;
    
    switch( RFLRState )
			{
				case RFLR_STATE_IDLE:
				break;
				case RFLR_STATE_RX_INIT:        
								SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
								SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |RFLR_IRQFLAGS_VALIDHEADER |RFLR_IRQFLAGS_TXDONE |RFLR_IRQFLAGS_CADDONE |RFLR_IRQFLAGS_CADDETECTED;
								SX1276Write( REG_LR_IRQFLAGSMASK, SX1276LR->RegIrqFlagsMask );
								if( LoRaSettings.FreqHopOn == true )
									{
											SX1276LR->RegHopPeriod = LoRaSettings.HopPeriod;
											SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
											SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
									}
								else
									{
											SX1276LR->RegHopPeriod = 255;
									}
								SX1276Write( REG_LR_HOPPERIOD, SX1276LR->RegHopPeriod );	
								SX1276LR->RegDioMapping1 = RFLR_DIOMAPPING1_DIO0_00 | RFLR_DIOMAPPING1_DIO1_00 | RFLR_DIOMAPPING1_DIO2_00 | RFLR_DIOMAPPING1_DIO3_00;// RxDone                    RxTimeout                   FhssChangeChannel           CadDone																		
								SX1276LR->RegDioMapping2 = RFLR_DIOMAPPING2_DIO4_00 | RFLR_DIOMAPPING2_DIO5_00;// CadDetected               ModeReady
								SX1276WriteBuffer( REG_LR_DIOMAPPING1, &SX1276LR->RegDioMapping1, 2 );
								SX1276LoRaSetOpMode( RFLR_OPMODE_RECEIVER_SINGLE );
								memset( RFBuffer, 0, ( size_t )RF_BUFFER_SIZE );
								SX1276_Rx_Count_Time();
								RFLRState = RFLR_STATE_RX_RUNNING;
				break;
				case RFLR_STATE_RX_RUNNING:
								if( DIO0 == 1 ) 
										{
											if( LoRaSettings.FreqHopOn == true )
													{
															SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
															SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
													}
												SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_RXDONE );// Clear Irq
												RFLRState = RFLR_STATE_RX_DONE;
										}
								if( DIO2 == 1 ) 
										{
											if( LoRaSettings.FreqHopOn == true )
													{
															SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
															SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
													}
												SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL );
										}
								else
										{
											RxTimeoutTimer=SysTick->CTRL;
											if((RxTimeoutTimer&0x01&&!(RxTimeoutTimer&(1<<16)))==0)
													{
														SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
														SysTick->VAL =0X00;                     
														RFLRState = RFLR_STATE_RX_TIMEOUT;
													}
										}
				break;
				case RFLR_STATE_RX_DONE:
								SX1276Read( REG_LR_IRQFLAGS, &SX1276LR->RegIrqFlags );
								if( ( SX1276LR->RegIrqFlags & RFLR_IRQFLAGS_PAYLOADCRCERROR ) == RFLR_IRQFLAGS_PAYLOADCRCERROR )
										{		
												SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_PAYLOADCRCERROR  );// Clear Irq
												RFLRState = RFLR_STATE_RX_INIT;
											  break;
										}
						
								{
										uint8_t rxSnrEstimate;
										SX1276Read( REG_LR_PKTSNRVALUE, &rxSnrEstimate );
										if( rxSnrEstimate & 0x80 ) // The SNR sign bit is 1
											{
													RxPacketSnrEstimate = ( ( ~rxSnrEstimate + 1 ) & 0xFF ) >> 2;// Invert and divide by 4
													RxPacketSnrEstimate = -RxPacketSnrEstimate;
											}
										else
											{
													RxPacketSnrEstimate = ( rxSnrEstimate & 0xFF ) >> 2;// Divide by 4
											}
								}
						
								if( LoRaSettings.RFFrequency < 860000000 )  // LF
									{    
										if( RxPacketSnrEstimate < 0 )
											{
													RxPacketRssiValue = NOISE_ABSOLUTE_ZERO + 10.0 * SignalBwLog[LoRaSettings.SignalBw] + NOISE_FIGURE_LF + ( double )RxPacketSnrEstimate;
											}
										else
											{    
													SX1276Read( REG_LR_PKTRSSIVALUE, &SX1276LR->RegPktRssiValue );
													RxPacketRssiValue = RssiOffsetLF[LoRaSettings.SignalBw] + ( double )SX1276LR->RegPktRssiValue;
											}
									}
								else                                        // HF
									{    
										if( RxPacketSnrEstimate < 0 )
											{
													RxPacketRssiValue = NOISE_ABSOLUTE_ZERO + 10.0 * SignalBwLog[LoRaSettings.SignalBw] + NOISE_FIGURE_HF + ( double )RxPacketSnrEstimate;
											}
										else
											{    
													SX1276Read( REG_LR_PKTRSSIVALUE, &SX1276LR->RegPktRssiValue );
													RxPacketRssiValue = RssiOffsetHF[LoRaSettings.SignalBw] + ( double )SX1276LR->RegPktRssiValue;
											}
									}

								SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoRxBaseAddr;
								SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
								if( LoRaSettings.ImplicitHeaderOn == true )
									{
											RxPacketSize = SX1276LR->RegPayloadLength;
											SX1276ReadFifo( RFBuffer, SX1276LR->RegPayloadLength );
									}
								else
									{
											SX1276Read( REG_LR_NBRXBYTES, &SX1276LR->RegNbRxBytes );
											RxPacketSize = SX1276LR->RegNbRxBytes;
											SX1276ReadFifo( RFBuffer, SX1276LR->RegNbRxBytes );
									}	
								SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; 
								SysTick->VAL =0X00;                      							
								RFLRState = RFLR_STATE_RX_INIT;
								result = RF_RX_DONE;
				break;
				case RFLR_STATE_RX_TIMEOUT:
								RFLRState = RFLR_STATE_RX_INIT;
								result = RF_RX_TIMEOUT;
				break;
				case RFLR_STATE_TX_INIT:
								SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
								if( LoRaSettings.FreqHopOn == true )
									{
											SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |RFLR_IRQFLAGS_RXDONE |RFLR_IRQFLAGS_PAYLOADCRCERROR |RFLR_IRQFLAGS_VALIDHEADER |RFLR_IRQFLAGS_CADDONE |RFLR_IRQFLAGS_CADDETECTED;
											SX1276LR->RegHopPeriod = LoRaSettings.HopPeriod;
											SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
											SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
									}
								else
									{
											SX1276LR->RegIrqFlagsMask = RFLR_IRQFLAGS_RXTIMEOUT |RFLR_IRQFLAGS_RXDONE |RFLR_IRQFLAGS_PAYLOADCRCERROR |RFLR_IRQFLAGS_VALIDHEADER |RFLR_IRQFLAGS_CADDONE |RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL |RFLR_IRQFLAGS_CADDETECTED;
											SX1276LR->RegHopPeriod = 0;
									}
								SX1276Write( REG_LR_HOPPERIOD, SX1276LR->RegHopPeriod );
								SX1276Write( REG_LR_IRQFLAGSMASK, SX1276LR->RegIrqFlagsMask );
								SX1276LR->RegPayloadLength = TxPacketSize;// Initializes the payload size
								SX1276Write( REG_LR_PAYLOADLENGTH, SX1276LR->RegPayloadLength );
								SX1276LR->RegFifoTxBaseAddr = 0x00; // Full buffer used for Tx
								SX1276Write( REG_LR_FIFOTXBASEADDR, SX1276LR->RegFifoTxBaseAddr );
								SX1276LR->RegFifoAddrPtr = SX1276LR->RegFifoTxBaseAddr;
								SX1276Write( REG_LR_FIFOADDRPTR, SX1276LR->RegFifoAddrPtr );
								SX1276WriteFifo( RFBuffer, SX1276LR->RegPayloadLength );// Write payload buffer to LORA modem																					
								SX1276LR->RegDioMapping1 = RFLR_DIOMAPPING1_DIO0_01 | RFLR_DIOMAPPING1_DIO1_00 | RFLR_DIOMAPPING1_DIO2_00 | RFLR_DIOMAPPING1_DIO3_01;// TxDone               RxTimeout                   FhssChangeChannel          ValidHeader         
								SX1276LR->RegDioMapping2 = RFLR_DIOMAPPING2_DIO4_01 | RFLR_DIOMAPPING2_DIO5_00;// PllLock              Mode Ready
								SX1276WriteBuffer( REG_LR_DIOMAPPING1, &SX1276LR->RegDioMapping1, 2 );
								SX1276LoRaSetOpMode( RFLR_OPMODE_TRANSMITTER );	
								SX1276_Rx_Count_Time();//2015.07.09修改							
								RFLRState = RFLR_STATE_TX_RUNNING;
				break;
				case RFLR_STATE_TX_RUNNING:
								if( DIO0 == 1 ) // TxDone
										{
												SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_TXDONE  );// Clear Irq
												RFLRState = RFLR_STATE_TX_DONE;   
										}
								if( DIO2 == 1 ) // FHSS Changed Channel
										{
												if( LoRaSettings.FreqHopOn == true )
													{
															SX1276Read( REG_LR_HOPCHANNEL, &SX1276LR->RegHopChannel );
															SX1276LoRaSetRFFrequency( HoppingFrequencies[SX1276LR->RegHopChannel & RFLR_HOPCHANNEL_CHANNEL_MASK] );
													}
												SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_FHSSCHANGEDCHANNEL );// Clear Irq
										}
								else
										{
											TxTimeoutTimer=SysTick->CTRL;
											if((TxTimeoutTimer&0x01&&!(TxTimeoutTimer&(1<<16)))==0)
													{
														SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
														SysTick->VAL =0X00;                     
														RFLRState = RFLR_STATE_TX_TIMEOUT;
													}
										}
				break;
				case RFLR_STATE_TX_DONE:
								SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
								SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; 
								SysTick->VAL =0X00;                      
								RFLRState = RFLR_STATE_RX_INIT;
								result = RF_TX_DONE;
				break;
				case RFLR_STATE_TX_TIMEOUT:
					      SX1276LoRaSetOpMode( RFLR_OPMODE_STANDBY );
								RFLRState = RFLR_STATE_RX_INIT;
								result = RF_TX_TIMEOUT;
				break;
			} 
    return result;
}
uint32_t SX1276Process( void )
{
  return SX1276LoRaProcess( );
}

void RF_Sx1276_Init(void)
{
 //Spi_Init();
 SX1276Init();
 SX1276SetIdleState();
}
void A(void)
{
	if(RxGain ==2)
	if(RFState==3)
	if(RxPacketRssiValue==4){}
}
