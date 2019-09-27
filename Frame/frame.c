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

#ifndef _WIN32
    #include <avr/io.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "frame.h"


/**< Hier befinden sich die ganzen Informationen �ber die gesendeten Daten!
 *   Dieser wird bei jedem neuen Senden eines Frames
 *   mit gesendet.
 */
static uint8_t	    FrameHeader[ __FRAME_ENTRYS__ ] = "";

/**< Wird f�r die interne zwischen speicherung von Daten ben�tigt */
static sFrameDesc_t _sFrameDescInt =
{
    .eDataType    = 0,
    .eMessageID   = 0,
    .eExitcode    = 0,
    .pData        = NULL,
    .uiDataLength = 0
};

/**< Enth�lt die berechneten & empfangenen Checksummen */
static sCrc_t       sCrc;

/**< Status Variable, zum erkennen eines neuen Frames */
static int8_t       FrameStart = 0;

enum eAckState
{
    FRAME_ACK_NO_ACK,
    FRAME_ACK_ACK,

    __FRAME_ACK_ENTRYS__
};

typedef struct
{
    enum eAckState eState;
    uint8_t uiAckValue;
}sFrameAck_t;

 static sFrameAck_t _sFrameAck =
{
    .eState = FRAME_ACK_NO_ACK,
    .uiAckValue = 0xAC,
};

/** \brief  CallBack Funktion zum senden des Kommandos
 *          wird in "frame.h" hinterlegt (z.B uart_put).
 *
 * \param   *pData      -> Zeiger auf Byte(s) Anfang
 * \param   uiLength    -> L�nge der Bytes
 * \return  Es wird nichts zur�ck gegeben
 *
 */
void ( *pfncSendCallback )			    ( uint8_t *pData , uint8_t uiLength );



static uint8_t      Crc8Message	        ( uint8_t uiCrcStart , uint8_t *pData , uint8_t uiLength )
{
	static uint8_t uiData = 0;

	uiData = uiCrcStart;

	for ( uint8_t ui = 0 ; ui < uiLength ; ui++ )
    {
        uiData = ( uiData ^ pData[ui] );

        for ( uint8_t i = 0; i < 8; i++ )
        {
            if ( ( uiData & 0x80 ) != 0 )
            {
                uiData <<= 1;
                uiData ^= 0x07;
            }
            else
            {
                uiData <<= 1;
            }
        }
    }
	return uiData;
}

static int8_t       FrameSearch	        ( uint8_t *pReceive , uint8_t uiLength )
{
	for ( uint8_t x = 0 ; x < uiLength ; x++ )
	{
		if ( pReceive[x] == '-' )
		{
		  if ( pReceive[x+1] == '+' )
		  {
			return (int8_t)x + 2;
		  }
		  else
		  {
			  return -1;
		  }
		}
	}

	return -1;
}

static sFrame_t		FrameGet			( void )
{
	/*
	*	Muss als "static" deklariert sein, sonst k�nnen wir uns die
	*	reservierte Adresse von "puiFrame" bei einem erneuten aufruf nicht wieder freigeben.
	*	Das k�nnte in Verbindung mit "malloc()" Probleme verursachen!
	*/
	static sFrame_t sFrame;
	uint8_t ui = 0;

	if ( sFrame.puiFrame != NULL )
    {
		free( sFrame.puiFrame );
	}

	uint8_t	*puiFrame = malloc( __FRAME_ENTRYS__ + _sFrameDescInt.uiDataLength );

	if ( puiFrame == NULL )
    {
		return sFrame;
	}

	for ( ; ui < __FRAME_ENTRYS__ ; ui++ )
    {
		puiFrame[ui] = FrameHeader[ui];
	}

	for ( uint8_t ux = 0 ; ux < _sFrameDescInt.uiDataLength ; ux++ )
    {
		puiFrame[ui+ux] = _sFrameDescInt.pData[ux];
	}

	sFrame.puiFrame     = puiFrame;
	sFrame.uiLength     = __FRAME_ENTRYS__ + _sFrameDescInt.uiDataLength;
	sFrame.eDataType    = _sFrameDescInt.uiDataLength;

	return sFrame;
}

static uint8_t	    _FrameBuild		    ( sFrameDesc_t *psFrame )
{
	uint8_t uiInfo = 0;

	if ( psFrame->pData == NULL )
    {
		uiInfo = 1<<0; /**< Keine Nutzdaten vorhanden */
	}
	if ( psFrame == NULL )
	{
		uiInfo |= 1<<1; /**< Kein g�ltiger Zeiger */
		return uiInfo;
	}

	FrameHeader[FRAME_LENGTH_OF_FRAME]	= (__FRAME_ENTRYS__ + psFrame->uiDataLength);	// Laenge der ganzen Antwort
	FrameHeader[FRAME_DATA_TYP]		    = psFrame->eDataType;	// (u)char , (u)int8 , (u)int16 , (u)int32 usw.
	FrameHeader[FRAME_ID]				= psFrame->eMessageID; // 0..255
	FrameHeader[FRAME_EXITCODE]		    = psFrame->eExitcode;	// 0..255
	FrameHeader[FRAME_CRC]	            = 0;

	sCrc.uiInternal = 0;
	sCrc.uiInternal = Crc8Message( sCrc.uiInternal , FrameHeader , __FRAME_ENTRYS__ ); /**< Header */
	sCrc.uiInternal = Crc8Message( sCrc.uiInternal , psFrame->pData , psFrame->uiDataLength ); /**< Nutzdaten */

	FrameHeader[FRAME_CRC] = sCrc.uiInternal;

    _sFrameDescInt.pData         = psFrame->pData;
	_sFrameDescInt.uiDataLength  = psFrame->uiDataLength;

	return uiInfo;
}




void		FrameClear			( sFrameDesc_t *psFrame )
{
	psFrame->uiDataLength 	= 0;
	psFrame->eDataType		= 0;
	psFrame->eMessageID 	= 0;
	psFrame->eExitcode 	    = 0;
	psFrame->pData 		    = NULL;
}

void		FrameInit			( sFrameDesc_t *psFrame )
{
    #ifndef _WIN32
        pCmdSendCallback = _CMD_SEND_CB_FNC_PTR_;
	#endif

	if ( pfncSendCallback == NULL )
    {
		return;
	}

	if ( !psFrame )
	{
		FrameClear( psFrame );
	}

	sCrc.uiExternal		= 0;	// Extern resetten
	sCrc.uiInternal		= 0;	// Intern resetten
	FrameStart			= 0;	// Index eines Frames
}

uint8_t		FrameParse			( uint8_t *pReceive , sFrameDesc_t *psParsed , uint16_t uiBufferLength )
{
	FrameStart = FrameSearch( pReceive , uiBufferLength );

	if ( FrameStart == - 1 )
    {
		return 1;
	}

	psParsed->uiDataLength 	= pReceive[ FrameStart + FRAME_LENGTH_OF_FRAME  ] - __FRAME_ENTRYS__;
	psParsed->eDataType		= pReceive[ FrameStart + FRAME_DATA_TYP		    ];
	psParsed->eMessageID	= pReceive[ FrameStart + FRAME_ID				];
	psParsed->eExitcode		= pReceive[ FrameStart + FRAME_EXITCODE		    ];
	sCrc.uiExternal			= pReceive[ FrameStart + FRAME_CRC			 	];

	if ( psParsed->uiDataLength )
    {
		psParsed->pData = pReceive + ( FrameStart + __FRAME_ENTRYS__     );
	}
	else
	{
		psParsed->pData = NULL; // Keine Nutzdaten
	}

	sCrc.uiInternal = 0;

	pReceive[ FrameStart + FRAME_CRC ] = 0;

	/**< Checksumme vom empfangenen Frame-Header bilden */
	sCrc.uiInternal = Crc8Message( sCrc.uiInternal , &pReceive[FrameStart] , __FRAME_ENTRYS__ );

	/**< Checksumme von Nutzdaten bilden */
	sCrc.uiInternal = Crc8Message( sCrc.uiInternal , &pReceive[FrameStart + __FRAME_ENTRYS__ ] , psParsed->uiDataLength );

	/* Checksummen vergleichen */
	if ( sCrc.uiInternal != sCrc.uiExternal )
    {
		return 2;
	}

	return 0;
}

void		FrameBuild		    ( sFrameDesc_t *psFrame , enum eIdent eIdent , enum eDataType eDataType , enum eExitcodes eExitcode , uint8_t *pData, uint8_t DataLength )
{
	psFrame->eMessageID	    = eIdent;		// Beschreibt den Nachrichten Typ. Damit die gegenstelle die Nachrichten unterscheiden kann
	psFrame->eDataType		= eDataType;	// Gibt an um welchen Daten Typ es sich handelt
	psFrame->eExitcode		= eExitcode;	// Ruekgabewert einer Funktion
	psFrame->pData			= pData;		// Zeiger auf die Daten die gesendet werden sollen
	psFrame->uiDataLength	= DataLength;	// Anzahl der Bytes

    _FrameBuild( psFrame );
}

void		FrameSend		    ( sFrameDesc_t *psFrame )
{
	sFrame_t sFrame = FrameGet( );

    pfncSendCallback( sFrame.puiFrame , sFrame.uiLength );
}

#ifdef _WIN32
void        FrameShow             ( sFrameDesc_t *psFrame )
{
    sFrame_t sFrame = FrameGet();

    printf("*[CMD_HEADER_LENGTH_OF_FRAME]: 0x%02X\r\n" , sFrame.puiFrame[FRAME_LENGTH_OF_FRAME]);
    printf("*[CMD_HEADER_DATA_TYP       ]: 0x%02X\r\n" , sFrame.puiFrame[FRAME_DATA_TYP]);
    printf("*[CMD_HEADER_ID             ]: 0x%02X\r\n" , sFrame.puiFrame[FRAME_ID]);
    printf("*[CMD_HEADER_EXITCODE       ]: 0x%02X\r\n" , sFrame.puiFrame[FRAME_EXITCODE]);
    printf("*[CMD_HEADER_CRC            ]: 0x%02X\r\n" , sFrame.puiFrame[FRAME_CRC]);

    for ( uint8_t ui = 0 ; ui < (sFrame.uiLength - __FRAME_ENTRYS__) ; ui++ )
    {
         printf("*[Data                      ]: 0x%02X\r\n" , sFrame.puiFrame[__FRAME_ENTRYS__ + ui ]);
    }
}
#endif



/** \brief  Sucht das "Ack." im Empfangspuffer
 *
 * \param   pBuffer uint8_t*        -> Empfangspuffer
 * \param   uiBufferLength uint8_t  -> L�nge des Empfangspuffers
 * \param   uiWaitForX uint8_t      -> Auf welches Zeichen soll gewartet werden
 * \return  sFrameAck_t             -> Informationen
 *
 */
sFrameAck_t FrameSearchAck( uint8_t *pBuffer , uint8_t uiBufferLength , uint8_t uiWaitForX )
{
    for ( uint8_t ui = 0 ; ui < uiBufferLength ; ui++ )
    {

    }
}
