/* Generated by CodeDescriptor 1.5.0.0907 */
/*
* Project Name      -> STPM32
* Version           -> 1.0.0.0621
* Author            -> Hm @ Workstadion.: QP-01-02
* Build Date        -> 21.06.2018 07:49:56
* Description       -> Description
*
*	Startbedingungen:
*
*	SCS Pin :		Um in den SPI Modus zu booten muss der Pin "SCS" beim Startvorgang ( einschalten der Versorgungsspannung )
*					auf "low" gezogen sein, sonst wird im USART Modus gebootet.
*					Wenn der Kommunikations Modus gew�hlt wurde, wird der "SCS" Pin als Chip Select Signal benutzt.
*
*	Enable Pin :	Wenn der Pin beim Startvorgang auf "High" ist und direkt danach auf "Low" gezogen wird ist der Chip startklar
*
*/


#ifndef __STPM32_H__
#define __STPM32_H__

#define BUILD_UINT32( BUFF )	( ((uint32_t)BUFF[3]<<24UL | (uint32_t)BUFF[2]<<16UL | (uint32_t)BUFF[1]<<8UL | (uint32_t)BUFF[0]) )

#include "xmega_spi.h"


/*	Hier bitte die "SPI Routinen" eintragen!
*	
*	**STPM32_SLAVE_SELECT**.: Eine Funktion ohne R�ckgabeparameter und
*	ohne �bergabeparameter. Die Funktion muss lediglich den "Slave Select" Pin
*	vom "STPM32" auf "low" ziehen.
*
*	**STPM32_SLAVE_SELECT**.: Eine Funktion ohne R�ckgabeparameter und
*	ohne �bergabeparameter. Die Funktion muss lediglich den "Slave Select" Pin
*	vom "STPM32" auf "high" ziehen.
*
*
*	**STPM32_TX_RX**.: Eine Funktion die ein "Byte( 8 Bit )" als �bergabeparameter
*	erwartet und als R�ckgabeparameter das empfangene Byte wieder zur�ck gibt.
*
*/
#define STPM32_SYNC_PORT		PORTC
#define STPM32_SYNC_bp			0

#define STPM32_SCS_PORT			PORTC
#define STPM32_SCS_bp			1

#define STPM32_EN_PORT			PORTC
#define STPM32_EN_bp			2


#define __STPM32_PIN_OUTPUT__( _port , _pin )	( _port.DIRSET = 1<<_pin )
#define __STPM32_PIN_HIGH__( _port , _pin )		( _port.OUTSET = 1<<_pin )
#define __STPM32_PIN_LOW__( _port , _pin )		( _port.OUTCLR = 1<<_pin )

enum stpm32CommunicationProtocol
{
	#define STPM32_FRAME_LENG			5
	#define STPM32_FRAME_WITHOUT_CRC	( STPM32_FRAME_LENG - 1 )
	
	STPM32_READ_ADDR,
	STPM32_WRITE_ADDR,
	STPM32_CMD_LSB,
	STPM32_CMD_MSB,
	STPM32_CRC,
	
	STPM32_COM_PROTOCOL
};

enum stpm32Regs
{
	DSP_CR1		= 0x00,
	DSP_CR2		= 0x02,
	DSP_CR3		= 0x04,
	DSP_CR4		= 0x06,
	DSP_CR5		= 0x08,
	DSP_CR6		= 0x0A,
	DSP_CR7		= 0x0C,
	DSP_CR8		= 0x0E,
	DSP_CR9		= 0x10,
	DSP_CR10	= 0x12,
	DSP_CR11	= 0x14,
	DSP_CR12	= 0x16,
	
	DFE_CR1		= 0x18,
	DFE_CR2		= 0x1A,
	
	DSP_IRQ1	= 0x1C,
	DSP_IRQ2	= 0x1E,
	
	DSP_SR1		= 0x20,
	DSP_SR2		= 0x22,
	
	US_REG1		= 0x24,
	US_REG2		= 0x26,
	US_REG3		= 0x28,
	
	DSP_EV1		= 0x2A,
	DSP_EV2		= 0x2C,
	
	DSP_REG1	= 0x2E,
	DSP_REG2	= 0x30,
	DSP_REG3	= 0x32,
	DSP_REG4	= 0x34,
	DSP_REG5	= 0x36,
	DSP_REG6	= 0x38,
	DSP_REG7	= 0x3A,
	DSP_REG8	= 0x3C,
	DSP_REG9	= 0x3E,
	DSP_REG10	= 0x40,
	DSP_REG11	= 0x42,
	DSP_REG12	= 0x44,
	DSP_REG13	= 0x46,
	DSP_REG14	= 0x48,
	DSP_REG15	= 0x4A,
	DSP_REG16	= 0x4C,
	DSP_REG17	= 0x4E,
	DSP_REG18	= 0x50,
	DSP_REG19	= 0x52,	
	
	PH1_REG1	= 0x54,
	PH1_REG2	= 0x56,
	PH1_REG3	= 0x58,
	PH1_REG4	= 0x5A,
	PH1_REG5	= 0x5C,
	PH1_REG6	= 0x5E,
	PH1_REG7	= 0x60,
	PH1_REG8	= 0x62,
	PH1_REG9	= 0x64,
	PH1_REG10	= 0x66,
	PH1_REG11	= 0x68,
	PH1_REG12	= 0x6A,
	
	PH2_REG1	= 0x6C,
	PH2_REG2	= 0x6E,
	PH2_REG3	= 0x70,
	PH2_REG4	= 0x72,
	PH2_REG5	= 0x74,
	PH2_REG6	= 0x76,
	PH2_REG7	= 0x78,
	PH2_REG8	= 0x7A,
	PH2_REG9	= 0x7C,
	PH2_REG10	= 0x7E,
	PH2_REG11	= 0x80,
	PH2_REG12	= 0x82,
	
	TOT_REG1	= 0x84,
	TOT_REG2	= 0x86,
	TOT_REG3	= 0x88,
	TOT_REG4	= 0x8A,
};

enum tx_crc
{
	STPM32_TX_CRC_LOW_WORD,
	STPM32_TX_CRC_HIGH_WORD,	
	STPM32_TX_CRC_SIZE,
};

/*	CRC Modus
*	Soll CRC f�r die �bertragung genutzt werden?
*	Wenn es nicht genutzt werden sollte, muss es noch dem "STPM32" mitgeteilt werden.
*	Default erwartet der "STPM32" eine Checksumme!
*/
#define STPM32_CRC_MODE		1

#if !STPM32_CRC_MODE
	#warning STPM32_CRC_DISABLE!
#endif

/*
*	Werte wurden bereits berechnet, damit wir sp�ter keine unn�tige CPU auslastung verursachen..
*	K�nnen jedoch mit #define STPM32_WITHOUT_FIX_CALC "frisch" berechnet werden.	
*
*	VLSB (VRMS): Vref * ( 1 + R1 / R2 ) / calv * Av * 2^15 
*	ILSB (VRMS): Vref / cali * Ai * 2^17 * ks * kint
*/

//#define STPM32_WITHOUT_FIX_CALC

#ifdef STPM32_WITHOUT_FIX_CALC
	#define STPM32_VLSB		( float ) ( ( STPM32_VREF * ( 1 + ( STPM32_R1 / STPM32_R2 ) ) ) / ( STPM32_CALV * STPM32_AV * 32768 ) )
	#define STPM32_ILSB		( float ) ( ( STPM32_VREF ) / ( STPM32_CALI * STPM32_AI * 131072 * STPM32_KS * STPM32_KINT ) )
	#warning STPM32 VLSB & ILSB testen
#else
	/*	Werte stammen aus der Excel Tabelle vom Hersteller
	*/
	#define STPM32_VLSB			( float )		0.036085
	#define STPM32_ILSB			( float )		0.00218
	#define STPM32_PERIOD_LSB	( float )		0.000008 // 8�s
	#define STPM32_ZCR_FCLK		( uint32_t )	125000 // 125kHz
	#define STPM32_POWER_LSB	( float )		0.001259 //W
	#define STPM32_ENERGY_LSB	( float )		0.000330 //Ws
	#define STPM32_PHASE_LSB	( float ) 

#endif

#define STPM32_CAL_VOLTAGE	230	// Angabe in Volt
#define STPM32_CAL_CURRENT	5	// Angabe in Ampere

#define STPM32_CALC_FREQUENCY( period )	 (float)( period * STPM32_PERIOD_LSB )


typedef struct
{
	struct  
	{
		uint8_t lastRx;
		uint16_t cnt;
	}crc;
	
	struct  
	{
		float period;
		float phaseAngel;
		float apparentRmsPower;
		float apparentEnergy;
	}ch1;
	
	struct  
	{
		float period;
		float phaseAngel;
		float apparentRmsPower;
		float apparentEnergy;
	}ch2;
	
}stpm32_t;
stpm32_t stpm32;


void		stpm32Init			( void );									

void		stpm32CrcEnable		( void );							
	
void		stpm32CrcDisable	( void );							

void		stpm32CrcSetPoly	( uint8_t poly );			

uint8_t		stpm32Online		( void );							

uint32_t	stpm32GetVoltageAD	( void );									

uint32_t	stpm32GetCurrentAD	( void );									

uint32_t	stpm32CalcCHV		( uint16_t vCal , uint32_t vADRead );		

void		stpm32SetCHV		( uint16_t val );			

uint32_t	stpm32CalcCHC		( uint8_t iCal , uint32_t iADRead );			

void		stpm32SetCHC		( uint16_t val );			

uint16_t	stpm32GetVoltage	( void );							

float		stpm32GetCurrent	( void );							

void		stpm32GetPeriod		( stpm32_t *s );

void		stpm32GetPhaseAngel ( stpm32_t *s );

void		stpm32GetApparentRMSPower ( stpm32_t *s );

void		stpm32GetApparentEnergy( stpm32_t *s );

typedef enum stpm32_error_enum
{
	STPM32_ERROR_ALL_OK,
	STPM32_ERROR_CHV_FAIL,
	STPM32_ERROR_CHC_FAIL,
}stpm32_error_enum_t;

stpm32_error_enum_t		stpm32StartCalibration	( uint16_t *chv , uint16_t *chc );

#endif
