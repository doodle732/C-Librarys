/* Generated by CodeDescriptor 1.5.0.0907 */
/*
* Project Name      -> STPM32
* Version           -> 1.0.0.0621
* Author            -> Hm @ Workstadion.: QP-01-02
* Build Date        -> 21.06.2018 07:49:56
* Description       -> Description
*
*
*
*/

#include "../Headers/wsq3000_def.h"

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../Headers/stpm32.h"


/*
*	Datenstruktur
*/
stpm32_t stpm32;





static void				stpm32Select	( void )											
{
	__STPM32_PIN_LOW__( STPM32_SCS_PORT , STPM32_SCS_bp );
}

static void				stpm32Deselect	( void )											
{
	__STPM32_PIN_HIGH__( STPM32_SCS_PORT , STPM32_SCS_bp );
}

static  inline uint8_t	stpm32CalcCRC	( uint8_t inCrc , uint8_t inData )					
{
	uint8_t   i;
	uint8_t   data;

	data = inCrc ^ inData;

	for ( i = 0; i < 8; i++ )
	{
		if ( data & 0x80 )
		{
			data <<= 1;
			data ^= 0x07;
		}
		else
		{
			data <<= 1;
		}
	}
	return data;
}

static inline void		stpm32SpiWrite	( uint8_t *buff , size_t leng )						
{
	uint8_t x;
	
	stpm32Select();
	for ( x = 0 ; x < leng ; x++ )
	{
		spiMasterWriteRead( buff[x] );
	}

	for ( x = 0 ; x < leng ; x++ )
	{
		spiMasterWriteRead( 0xFF );
	}
	stpm32Deselect();
}

static inline void		stpm32SpiRead	( uint8_t *buff , uint8_t useCrc )					
{	
	uint8_t frame[5] = { buff[0] , 0xFF , 0xFF , 0xFF , 0x00 }; // buff[0] = Register Adresse
		
	__STPM32_PIN_LOW__( STPM32_SYNC_PORT , STPM32_SYNC_bp );
  	_delay_ms(5);
  	__STPM32_PIN_HIGH__( STPM32_SYNC_PORT , STPM32_SYNC_bp );
  	_delay_ms(15);		
	
	stpm32Select();
	for ( uint8_t i = 0 ; i < ( STPM32_FRAME_LENG - 1 ) + useCrc ; i++ )
	{
		spiMasterWriteRead( frame[i] );
	}		

	for ( uint8_t i = 0 ; i < ( STPM32_FRAME_LENG - 1 ) + useCrc ; i++ )
	{
		*buff++ = spiMasterWriteRead( 0xFF );
	}
	stpm32Deselect();
}

static inline void		stpm32Reset		( void )											
{
	for ( uint8_t x = 0 ; x < 3 ; x++ )
	{
		__STPM32_PIN_LOW__( STPM32_SYNC_PORT , STPM32_SYNC_bp );
		_delay_ms( 10 );
		__STPM32_PIN_HIGH__( STPM32_SYNC_PORT , STPM32_SYNC_bp );
		_delay_ms( 10 );
	}
	
	__STPM32_PIN_LOW__( STPM32_SYNC_PORT , STPM32_SYNC_bp );
	
	spiMasterWrite( 0x00 );
	
	_delay_ms( 100 );
	
	__STPM32_PIN_HIGH__( STPM32_SYNC_PORT , STPM32_SYNC_bp );
	
}

static inline void		stpm32StartUp	( void )											
{	
	__STPM32_PIN_LOW__( STPM32_EN_PORT , STPM32_EN_bp );
  	_delay_ms(50);

	__STPM32_PIN_LOW__( STPM32_SCS_PORT , STPM32_SCS_bp );
  	_delay_ms(50);

	__STPM32_PIN_HIGH__( STPM32_EN_PORT , STPM32_EN_bp );
}

static void				stpm32Write		( uint8_t addr , uint16_t cmd , uint8_t useCrc )	
{		
	/*
	*	Kommando Byte bauen und mit Daten versehen
	*/
	uint8_t buff[STPM32_COM_PROTOCOL] = 
	{
		0xFF						, // STPM32_READ_ADDR
		addr						, // STPM32_WRITE_ADDR
		( cmd & 0x00FF )       		, // STPM32_CMD_LSB
		( ( cmd & 0xFF00 ) >> 8 ) 	, // STPM32_CMD_MSB
		0							, // STPM32_CRC
	};
	
	for ( uint8_t x = 0 ; x < STPM32_FRAME_WITHOUT_CRC ; x++ )
	{
		buff[STPM32_CRC] = stpm32CalcCRC( buff[STPM32_CRC] , *(buff + x) );
	}
	
	stpm32SpiWrite( buff , ( STPM32_FRAME_WITHOUT_CRC + useCrc ) ); 

}

static void				stpm32Write_	( uint8_t addr , uint32_t cmd , uint8_t useCrc )	
{
	uint8_t frameLow	[]	= { 0xFF , addr		, ( cmd & 0x000000FF ) , ( ( cmd & 0x0000FF00) >> 8 ) , 0x00 };
	uint8_t frameHigh	[]	= { 0xFF , addr + 1 , ( ( cmd & 0x00FF0000 ) >> 16 ) , ( ( cmd & 0xFF000000) >> 24 ) , 0x00 };
	
	for ( uint8_t x = 0 ; x < STPM32_FRAME_WITHOUT_CRC ; x++ )
	{
		frameLow[STPM32_CRC] = stpm32CalcCRC( frameLow[STPM32_CRC] , frameLow[x] );
	}
	stpm32SpiWrite( frameLow , ( STPM32_FRAME_WITHOUT_CRC + useCrc ) );
	stpm32.lastTxCrc[STPM32_TX_CRC_LOW_WORD] = frameLow[STPM32_CRC];
	
	for ( uint8_t x = 0 ; x < STPM32_FRAME_WITHOUT_CRC ; x++ )
	{
		frameHigh[STPM32_CRC] = stpm32CalcCRC( frameHigh[STPM32_CRC] , frameHigh[x] );
	}
	stpm32SpiWrite( frameHigh , ( STPM32_FRAME_WITHOUT_CRC + useCrc ) );
	stpm32.lastTxCrc[STPM32_TX_CRC_HIGH_WORD] = frameHigh[STPM32_CRC];
}

static int8_t			stpm32Read		( uint8_t addr , uint8_t *buff , uint8_t useCrc )	
{
	uint8_t crcRX = 0;
	int8_t	ret = 0;
	
	*buff = addr;
	stpm32SpiRead( buff , useCrc );
	
	for ( uint8_t x = 0 ; x < ( STPM32_FRAME_LENG - 1 ) ; x++ )
	{
		crcRX = stpm32CalcCRC( crcRX , buff[x] );
	}	
	
	stpm32.lastRxCrc = buff[STPM32_FRAME_LENG - 1]; // Empfangene CRC sichern

	if ( useCrc == 1 )
	{
		if ( stpm32.lastRxCrc != crcRX )
		{
			stpm32.crcErr++;
			return -1;
		}
	}

	return ret;
}

static uint16_t			stpm32CalcVrms	( uint32_t ad )										
{
	return ( ad & 0x7FFF ) * STPM32_VLSB;
}

static float			stpm32CalcIrms	( uint32_t ad )										
{
	return ( ( ad & 0xFFFF8000 ) >> 15 ) * STPM32_ILSB;
}

static uint32_t			stpm32CalcXv	( uint16_t volt )									
{
	return ( ( volt / STPM32_VLSB * 1000 ) );
}

static uint32_t			stpm32CalcXi	( uint16_t data	)									
{
	return ( data / STPM32_ILSB );
}

static inline uint32_t	stpm32CalcCHV	( uint16_t vCal , uint32_t vADRead )				
{
	return ( ( 14336 *  stpm32CalcXv( vCal ) / ( vADRead & 0x7FFF ) - 12288 ) );
}

static inline uint32_t	stpm32CalcCHC	( uint8_t iCal , uint32_t iADRead )					
{
	return ( ( 14336 *  stpm32CalcXi( iCal ) /  ( iADRead ) - 12288 ) ) ;
}



void		stpm32SpiInit		( void )								
{
	__STPM32_PIN_OUTPUT__( STPM32_SYNC_PORT , STPM32_SYNC_bp );
	__STPM32_PIN_OUTPUT__( STPM32_SCS_PORT , STPM32_SCS_bp );
	__STPM32_PIN_OUTPUT__( STPM32_EN_PORT , STPM32_EN_bp );
	
	stpm32StartUp();
	
	stpm32Reset();
	
}

uint16_t	stpm32SetCalV		( uint16_t voltageAD , uint8_t useCrc )		
{
	uint16_t chv = stpm32CalcCHV( STPM32_CAL_VOLTAGE , voltageAD );
	
	stpm32Write_( DSP_CR5 , chv , useCrc );
	
	return chv;
}

uint16_t	stpm32SetCalI		( uint16_t currentAD , uint8_t useCrc )		
{
	uint16_t chc = stpm32CalcCHC( STPM32_CAL_CURRENT , currentAD );
	
	stpm32Write_( DSP_CR6 , chc , useCrc );
	
	return chc;	
}

void		stpm32useCrcable	( uint8_t useCrc )							
{
	/*
	*	Siehe Datenblatt Seite.: 69
	*/
	uint8_t buff[] =
	{
		0x24,
		0x24,
		0x07,
		0x40,
		0xD2, // CRC
	};
	
	stpm32SpiWrite( buff , ( STPM32_FRAME_WITHOUT_CRC + useCrc ) ); // Kommando wird mit CRC gesendet
}
	
void		stpm32CrcDisable	( uint8_t useCrc )							
{
	/*
	*	Siehe Datenblatt Seite.: 69
	*/
	uint8_t buff[] =
	{
		0x24,
		0x24,
		0x07,
		0x00,
		0x15, // CRC
	};
	
	stpm32SpiWrite( buff , ( STPM32_FRAME_WITHOUT_CRC + useCrc ) ); 
}

void		stpm32CrcSetPoly	( uint8_t poly , uint8_t useCrc )			
{
	/*
	*	Siehe Datenblatt Seite.: 69
	*/	
	stpm32Write( 0x24 , poly , 1 ); 	
}

uint8_t		stpm32Online		( uint8_t useCrc )							
{
	uint8_t buff[5];
	stpm32Write_( 0x1E , 0b11111111000000001111111100000000  , useCrc );
	stpm32Read	( 0x1E , buff , useCrc );
	
	stpm32Write_( 0x1E , 0  , useCrc );
	
	if ( BUILD_UINT32( buff ) == 0b11111111000000001111111100000000 )
	{
		return 0;
	}
	
	return 1;
}

uint16_t	stpm32GetVoltage	( uint8_t useCrc )							
{
	uint8_t buff[5] = "";
	stpm32Read( 0x48 , buff , useCrc );
	
	return ( stpm32CalcVrms( BUILD_UINT32( buff ) ) );
}

float		stpm32GetCurrent	( uint8_t useCrc )							
{
	uint8_t buff[5] = "";
	stpm32Read( 0x48 , buff , useCrc );
	
	return ( stpm32CalcIrms( ( BUILD_UINT32( buff ) & 0xFFFF8000 ) >> 15 ) );
}