/* Generated by CodeDescriptor 1.5.0.0907 */
/*
* Project Name      -> stringHandler
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


char	*srchCmd		( char *inBuff , char *srchCmd )	
{
	/*
	*	Zeiger deklarieren).
	*/
	char *cmdBeginnPtr    	= NULL;
	char *cmdEndPtr		 	= NULL;
	char *cmdBeginnRawPtr	= NULL;
	
	/*
	*	Wurde als Parameter ein "NULL" Zeiger übergeben,
	*	wird die Funktion vorzeitig beendet
	*/

	if ( inBuff == NULL || srchCmd == NULL)
	{
		return NULL;
	}

	cmdBeginnPtr 	= strstr( inBuff , srchCmd );
	cmdEndPtr		= strchr( inBuff , CMD_DATA_END );	      

	if ( cmdEndPtr == NULL || cmdBeginnPtr == NULL)
	{
		return NULL;
	}
	
	/*
	*	Kommando terminieren
	*/
	*cmdEndPtr = '\0';		
	
	/*
	*	Benutzer Parameter empfangen?
	*/	
	cmdBeginnRawPtr = strchr( inBuff , CMD_RAW_DATA_BEGINN );
	if ( cmdBeginnRawPtr == NULL )
	{
		cmdBeginnPtr[ strlen( srchCmd ) ] = '\0';
		return cmdBeginnPtr;
	}
	
	/*
	*	Kommandotrenner überspringen
	*/
	cmdBeginnRawPtr++;
	
	return cmdBeginnRawPtr;
}

cmd_struct *getCmd		( cmd_struct __flash *tab , char *input , char *rawRX )
{
	uint8_t i;
	char *ptr;

	for( i = 0 ; i < CMD_SIZE_OF_TAB ; i++ )
	{
		ptr = srchCmd( input , tab[i].instruction );
		if ( ptr != NULL )
		{
			strcpy( rawRX , ptr );
			return tab + i;
		}
	}
	return NULL;
}

int8_t	cmpCmd		( char *strOne , char *strTwo )						
{
	uint8_t strLenOne = strlen( strOne );
	uint8_t strLenTwo = strlen( strTwo );
	
	if ( strLenOne != strLenTwo )
	{
		return -1;
	}
	
	
	if( strcmp( strOne , strTwo ) == 0 )
	{
		return 0;
	}
	else
	{
		return -2;
	}
	
	return -3;
}

uint8_t removeChar	( char *str , uint8_t wanted )						
{
    char * p = NULL;
	static uint8_t removed = 0;
	
    while ( *str )
    {
        while ( *str == wanted )
        {
			removed++;
            for ( p = str ; *p ; p++ )
            {
                *p = *( p + 1 );
            }
        }
        str++;
    }
	return removed;
}


cmd_t cmd =																
{
	.search		= srchCmd,
	.compare	= cmpCmd,
	.removeChar = removeChar,
};
