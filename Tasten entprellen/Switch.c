/* Generated by CodeDescriptor 1.5.0.0907 */
/*
* Project Name      -> Taster lesen
* Version           -> 1.0.0.0328
* Author            -> Hm @ Workstadion.: QP-01-02
* Build Date        -> 28.03.2019 07:37:32
* Description       -> **DEBUG**
*
*
*
*/

#include <avr/io.h>
#include "Switch.h"

/* Nur für die Bibliothek gedacht, nicht für "extern"
*	@Info: Sollte optimal in einer ISR hochgezählt werden..
*/
volatile static uint16_t Repeat;

/* Prototypen ******************************************************************************************************/

void SwitchInit( volatile uint8_t *SwitchInPort , uint8_t SwitchMask , Switch_t *Switch )
{
	PORT_DDR_ADDR(SwitchInPort) &= ~( SwitchMask ); // Daten Richtungs Register konfigurieren
	Switch->Old = 0;
	Switch->New = 0;
	Switch->Info = 0;
	Switch->Repeat.Info = 0;
	Switch->Mask = SwitchMask; // Tasten Maskierung speichern
}

void SwitchRead( Switch_t *Switch , volatile uint8_t *SwitchInPort )
{	
	static uint8_t RepeatInfo = 0;
	
	Switch->New = ( ( PORT_PIN_ADDR( SwitchInPort ) & Switch->Mask ) ^ Switch->Mask );
	
	if ( Switch->New != Switch->Old) 
	{
		Switch->Info = ( Switch->Info | ( Switch->New & ( Switch->Old ^ Switch->New ) ) );
		RepeatInfo = Switch->Info;
		Repeat = SWITCH_BEGINN_FIRST_REPEAT;
	}
	
	Switch->Old = Switch->New;
	
	if ( RepeatInfo & Switch->New )
	{
		if ( --Repeat == 0 )
		{
			Repeat = SWITCH_BEGINN_NEXT_REPEAT;
			Switch->Repeat.Info = RepeatInfo;
		}
	}
}

void SwitchClear( Switch_t *Switch )
{
	Switch->Old = 0;
	Switch->New = 0;
	Switch->Info = 0;
	Switch->Repeat.Info = 0;
}

/* Ende Prototypen ******************************************************************************************************/