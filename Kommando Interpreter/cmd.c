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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cmd.h"


char	*cmd_ = NULL;


static char			*cmdSearch			( char *inBuff , char *srchCmd )
{
	/*
	*	Zeiger deklarieren).
	*/
	char *inBuffPtr		= inBuff;
	char *srchCmdPtr	= srchCmd;
	char *cmdBeginnPtr 	= NULL;
	char *cmdEndPtr		= NULL;

	if ( inBuffPtr == NULL || srchCmdPtr == NULL )
	{
		return NULL;
	}

	cmdBeginnPtr = strstr( inBuffPtr , srchCmdPtr );
	cmdEndPtr	 = strchr( inBuffPtr , CMD_DATA_END );

	if ( cmdEndPtr == NULL || cmdBeginnPtr == NULL )
	{
		return NULL;
	}

	cmd_ = cmdBeginnPtr;

	return cmdBeginnPtr;
}

static int8_t		cmdGetIndex			( char *inBuff )
{
	uint8_t i;

	cmd_t 	*cmdPtr			= &cmd;
	char  	*cmdSearchPtr 	= NULL;
	char	*inputPtr		= inBuff;

	for ( i = 0 ; i < cmdPtr->tabLen ; i++ )
	{
		cmdSearchPtr = cmdSearch( inputPtr , ( char* ) cmdPtr->table[i].instruction );
		if ( cmdSearchPtr != NULL )
		{
			return i;
		}
	}

	return -1;
}


void				cmdInit				( const cmdTable_t *tab , cmdRaw_t *raw , size_t tableSize )
{
	cmd.table	= tab;
	cmd.tabLen	= tableSize;
	cmd.raw		= raw;
	
	cmd.raw->cmdPtr		= NULL;
	cmd.raw->paraNumb	= 0;
}

uint8_t 			cmdCntPara			( char *stream )
{
	uint8_t		x					= 0;
	char		*cmdPtr				= stream;
	char		*beginnPtr			= stream;

	beginnPtr = strchr( beginnPtr , CMD_RAW_DATA_BEGINN[0] ); // Erster Parameter
	if ( beginnPtr != NULL )
	{
		if ( *( beginnPtr + 1 ) != CMD_DATA_END && *( beginnPtr + 1 ) != CMD_RAW_PARA_DELIMITER[0] )
		{
			x++;
		}
	}
	
	for ( ; cmdPtr != NULL ;  )
	{
		cmdPtr = strchr( cmdPtr + 1 , CMD_RAW_PARA_DELIMITER[0] ); // weitere Parameter

		if ( cmdPtr != NULL )
		{
			if ( *( cmdPtr + 1 ) != CMD_DATA_END && *( cmdPtr + 1 ) != CMD_RAW_PARA_DELIMITER[0] )
			{
				x++;
			}
		}
	}

	cmd.raw->paraNumb = x;

	return x;
}

const char			*cmdGetInstruction	( char *input )
{
	int8_t i = cmdGetIndex( input );

	if ( i != (int8_t) -1 )
	{
		return cmd.table[i].instruction;
	}

	return NULL;
}

const char			*cmdGetName			( char *input )
{
	int8_t ret = cmdGetIndex( input );

	if ( ret != (int8_t) -1 )
	{
		return cmd.table[ret].name;
	}

	return NULL;
}

void				*cmdGetFunc			( char *input )
{
	int8_t ret = cmdGetIndex( input );
	if ( ret != (int8_t) -1 )
	{
		return cmd.table[ret].fnc;
	}
	return NULL;
}

char 				*cmdGetPara 		( char *out , char *in , uint8_t num )
{
	const char 	*rawPtr		= NULL;
	uint8_t x;

	memset( (char*)out , 0 , strlen(out) );

    if ( cmdCntPara( in ) < num )
    {
        return NULL;
    }

	for ( x = 0 ; x < cmd.tabLen ; x++ )
	{
		rawPtr = cmdSearch( in , ( char * ) cmd.table[x].instruction );
		if ( rawPtr != NULL )
		{
			break;
		}
	}

	if ( rawPtr == NULL )
	{
		return NULL;
	}

	char *streamPtr = in;

	streamPtr = strchr( in , CMD_RAW_DATA_BEGINN[0] ) + 1;
    if ( *streamPtr == CMD_DATA_END || *streamPtr == CMD_RAW_PARA_DELIMITER[0] || streamPtr == NULL )
    {
        return NULL;
    }

	uint8_t i;
	for ( i = 0 ; i < num ; i++ )
	{
		streamPtr = strchr( streamPtr , CMD_RAW_PARA_DELIMITER[0] ) + 1;
		if ( *streamPtr == CMD_DATA_END || *streamPtr == CMD_RAW_PARA_DELIMITER[0] )
        {
            return NULL;
        }
	}

	char *outPtr = out;
	while( *streamPtr != '\0' && *streamPtr != CMD_RAW_PARA_DELIMITER[0] && *streamPtr != CMD_DATA_END )
	{
		if ( *( streamPtr )  == CMD_CRC_BEGINN )
		{
			return NULL;
		}
        *outPtr++ = *streamPtr++;
	}

	return out;
}

char 				*cmdGetCRC 			( char *out , char *stream )
{
	char *crcPtr = stream;
	char *outPtr = out;

	crcPtr = strchr( crcPtr , ( char ) CMD_DATA_END );
	if ( crcPtr == NULL )
	{
		return NULL;
	}
	
	crcPtr = strchr( crcPtr , ( char ) CMD_CRC_BEGINN );
	if ( crcPtr != NULL && *( crcPtr + 1 ) != '\0' )
	{
		return crcPtr + 1;
	}

	return NULL;
}

char				*cmdGetCmdStr		( char *out , char *stream )
{
	uint8_t i;

	cmd_t	*cmdPtr			= &cmd;
	char	*outPtr			= out;
	char  	*cmdSearchPtr 	= NULL;
	char	*inputPtr		= stream;

	if ( cmdGetCRC( out , stream ) == NULL )
	{
		return NULL;
	}

	for ( i = 0 ; i < cmdPtr->tabLen ; i++ )
	{
		cmdSearchPtr = cmdSearch( inputPtr , ( char* ) cmdPtr->table[i].instruction );
		if ( cmdSearchPtr != NULL )
		{
			while( *cmdSearchPtr != '#' )
			{
				*out++ = *cmdSearchPtr++;
			}
			
			return outPtr;
		}
	}
	
	return NULL;	
}

char				*cmdHelp			( char *helpBuff )
{
 	strcpy( ( char * ) helpBuff , "Kommando Syntax: " );
 	strcat( ( char * ) helpBuff , "[KOMMANDO WORD][PARAMETER START][PARAMETER][KOMMANDO ENDE]\r\n" );

	strcat( ( char * ) helpBuff , "'" );
	strcat( ( char * ) helpBuff , CMD_RAW_DATA_BEGINN );
	strcat( ( char * ) helpBuff , "' ->[PARA. START]\r\n" );

	strcat( ( char * ) helpBuff , "'" );
	strcat( ( char * ) helpBuff , CMD_RAW_PARA_DELIMITER );
	strcat( ( char * ) helpBuff , "' ->[PARA. TRENNER]\r\n" );

	strcat( ( char * ) helpBuff , "'" );

	uint8_t pos = strlen( helpBuff );

	if ( CMD_DATA_END == '\0' )
	{
		strcat( ( char * ) helpBuff , "\\0" );
	}
	else
	{
		helpBuff[ pos ] = CMD_DATA_END;
	}
	strcat( ( char * ) helpBuff , "' ->[KOMMANDO ENDE]\r\n\n" );


 	strcat( ( char * ) helpBuff , "Kommandos:\r\n\n" );

	uint8_t x = 0;
	for ( ; x < cmd.tabLen ; x++ )
	{
		strcat( ( char * ) helpBuff , cmd.table[x].name );
		strcat( ( char * ) helpBuff , " " );
		strcat( ( char * ) helpBuff , cmd.table[x].instruction );
		strcat( ( char * ) helpBuff , cmd.table[x].syntax );
		strcat( ( char * ) helpBuff , "\r\n" );
	}

	return helpBuff;
}
