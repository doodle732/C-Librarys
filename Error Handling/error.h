/* Generated by CodeDescriptor 1.5.0.0907 */
/*
* Project Name      -> Error Handling
* Version           -> 1.0.0.0528
* Author            -> Hm @ Workstadion.: QP-01-02
* Build Date        -> 28.05.2018 06:38:46
* Description       -> Description
*
*	Aufbau des Fehlerspeichers.:
*
*
*	z.B.:	SLOT ( max. ERROR_SLOTS )	|	Errorcode ( max. ERROR_BUFFER_MASK )
*		   +------------------------+	|  +-----------------------------------+
*			0							| 	1 , 4 , 2 , 5 , 1
*			1							| 	5 , 7 , 8 , 32 , 212
*			...							|
*
*
*	- Jeder einzelne Slot arbeitet nach dem "LIFO" Prinzip		
*
*
*
*/

#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdint.h>



#define ERROR_SLOTS				10

/*
*	Muss das vielfache von n� sein!
*/
#define ERROR_BUFFER_LENG		16
#define ERROR_BUFFER_MASK		( ERROR_BUFFER_LENG - 1 )



/*<-----------------------------------------Enumerations----------------------------------------->*/

enum Error_Return_Codes_Enum
{
	ERROR_RETURN_SLOTS_OK,
	ERROR_RETURN_SLOTS_EMPTY,
	ERROR_RETURN_SLOTS_FULL,	
	ERROR_RETURN_SLOTS_OVF,
};

/*<-----------------------------------------Ende Enumerations----------------------------------------->*/

/*<-----------------------------------------Strukturen----------------------------------------->*/

typedef struct
{
	uint8_t uiWrite;
	uint8_t	uiRead;
	uint8_t pBuffer[ERROR_BUFFER_LENG];
}Error_t;

/*<-----------------------------------------Ende Strukturen----------------------------------------->*/

/*<-----------------------------------------Variabeln----------------------------------------->*/

static Error_t sError[ERROR_SLOTS];

/*<-----------------------------------------Ende Variabeln----------------------------------------->*/


/*<-----------------------------------------Prototypen----------------------------------------->*/

void ErrorInit( void );

enum Error_Return_Codes_Enum ErrorWrite( uint8_t uiSlot , uint8_t uiError );

enum Error_Return_Codes_Enum ErrorRead( uint8_t uiSlot , uint8_t *pByte );

/*<-----------------------------------------Ende Prototypen----------------------------------------->*/

#endif // __ERROR_H__
