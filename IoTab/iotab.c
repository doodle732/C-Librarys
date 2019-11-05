/*************************************************************
*|
*|	\@file  	iotab.h
*|	\@brief
*|	\@author 	J.H - Elec(C)
*|
*|	\@project	Strukturierte Handhabung der Ein. bzw. Ausg�nge
*|
*|	\@date		30/01/2019 - first implementation
*|
*|	\@todo 		nothing
*|	\@test		not tested
*|	\@bug		no bug known
*|
*|
*|	\@version	1.0.301019
*|
*|**************************************************************/

#ifndef __IOTAB_H__
 #include "../includes/iotab.h"
#endif





/*!<-- functions -- >*/
/*****************************************************************/

void IoTabInit( const sIO_t *sTab )
{
	for ( uint8_t x = 0 ; sTab[x].pPort != NULL ; x++ )
	{
		switch ( ( sTab[x].uiFuncBitPos & IO_TAB_PIN_FUNC_MASK  ) )
		{
			case _FUNCTION_INPUT:
			{
				*( sTab[x].pPort - 1 ) &= ~( 1 << ( sTab[x].uiFuncBitPos & IO_TAB_PIN_MASK ) ); // - 1 f�r das DDR Register
			}break;
			
			case _FUNCTION_INPUT_PULLUP:
			{
				*( sTab[x].pPort - 1 ) &= ~( 1 << ( sTab[x].uiFuncBitPos & IO_TAB_PIN_MASK ) ); // - 1 f�r das DDR Register
				*( sTab[x].pPort ) |=  ( 1 << ( sTab[x].uiFuncBitPos & IO_TAB_PIN_MASK ) );
			}break;
			
			case _FUNCTION_OUTPUT:
			{
				*( sTab[x].pPort - 1 ) |= ( 1 << ( sTab[x].uiFuncBitPos & IO_TAB_PIN_MASK ) ); // - 1 f�r das DDR Register
				
// 				if ( sTab[x].uiFuncBitPos & IO_TAB_DEFAULT_MASK )
// 				{
// 					*( sTab[x].pPort ) |= ( 1 << ( sTab[x].uiFuncBitPos & IO_TAB_PIN_MASK ) );
// 				}else
// 				{
// 					*( sTab[x].pPort ) &= ~( 1 << ( sTab[x].uiFuncBitPos & IO_TAB_PIN_MASK ) );
// 				}
				
			}break;
		}
	}
}

void IoTabSetLow( const sIO_t *sTab )
{
	for ( uint8_t x = 0 ; sTab[x].pPort != NULL ; x++ )
	{
		if ( ( sTab[x].uiFuncBitPos & IO_TAB_PIN_FUNC_MASK ) == _FUNCTION_OUTPUT )
		{
			*( sTab[x].pPort ) &= ~( 1 << ( sTab[x].uiFuncBitPos & 0x0F ) );
		}
	}
}

void IoTabSetHigh( const sIO_t *sTab )
{
	for ( uint8_t x = 0 ; sTab[x].pPort != NULL  ; x++ )
	{
		if ( ( sTab[x].uiFuncBitPos & IO_TAB_PIN_FUNC_MASK ) == _FUNCTION_OUTPUT )
		{
			*( sTab[x].pPort ) |= ( 1 << ( sTab[x].uiFuncBitPos & 0x0F ) );
		}
	}
}

void IoTabxHigh( const sIO_t *sTab , uint8_t uiMember )
{
	if ( sTab[uiMember].pPort == NULL )
	{
		return; // Ung�ltig!
	}
	
	*(sTab[uiMember].pPort) |= ( 1 << ( sTab[uiMember].uiFuncBitPos & 0x0F ) );
}

void IoTabxLow( const sIO_t *sTab , uint8_t uiMember )
{
	if ( sTab[uiMember].pPort == NULL )
	{
		return; // Ung�ltig!
	}
	
	*(sTab[uiMember].pPort) &= ~( 1 << ( sTab[uiMember].uiFuncBitPos & 0x0F ) );
}

/*****************************************************************/
// end of file
