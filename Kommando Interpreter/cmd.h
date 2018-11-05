/* Generated by CodeDescriptor 1.5.0.0907 */
/*
* Project Name      -> Kommando Interpreter
* Version           -> 1.0.1.1124
* Author            -> Hm @ Workstadion.: QP-01-02
* Build Date        -> 20.09.2017 07:50:01
* Description       -> Description
*
*
*
*/

#ifndef __CMD_H__
#define __CMD_H__

#include <stdint.h>
#include <stdlib.h>



enum Communication_Header_Enum
{
	CMD_HEADER_START_BYTE1, // Kommando Start Byte 1
	CMD_HEADER_START_BYTE2, // .. Byte 2
	CMD_HEADER_LENGHT, 		// L�nge des ganzen Streams
	CMD_HEADER_DATA_LENGHT, // L�nge der Roh Daten
	CMD_HEADER_DATA_TYP, 	// (u)char , (u)int8 , (u)int16 , (u)int32
	CMD_HEADER_ID, 			// Stream ID
	CMD_HEADER_EXITCODE,	// Exitkode aus Funktionen
	CMD_HEADER_CRC, 		// Checksumme von der Message

	__CMD_HEADER_ENTRYS__
};



typedef struct
{
	uint8_t msgLen;
	uint8_t dataLen;
	uint8_t dataTyp;
	uint8_t id;
	uint8_t exitcode;	
	uint8_t inCrc;
	uint8_t outCrc;
	
	uint8_t *dataPtr;

}cmd_t;

typedef struct 
{
	uint8_t (*fnc)( cmd_t *);	
}cmdFuncTab_t;



void	cmdInit				( cmd_t *c );					

int8_t	cmdGetStartIndex	( uint8_t *rx );					

uint8_t	cmdGetEndIndex		( uint8_t *rx );					

uint8_t	cmdParse			( uint8_t *rx , cmd_t *c );		

uint8_t	cmdCrc8StrCCITT		( uint8_t *str , uint8_t leng );

uint8_t	*cmdBuildHeader		( cmd_t *a );					

void	cmdSendAnswer		( cmd_t *a );					



#endif
