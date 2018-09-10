/* Generated by CodeDescriptor 1.2.0.0713 */
/*
* Project Name      -> RX8564
* Version           -> 1.0.0.0714
* Author            -> Hm @ Workstadion.: QP-01-02
* Build Date        -> 14.07.2017 07:18:15
* Description       -> Device Driver
*
*
*
*/



#include <stdbool.h>
#include "rx8564.h"



 
 static uint8_t buff[11] = "";


inline uint8_t			rtcBcdToDec				( uint8_t val )						
{
	return ( ( ( val & 0xF0 ) >> 4 ) * 10 ) + ( val & 0x0F );
}

inline uint8_t			rtcDecToBcd				( uint8_t val )						
{	
	return ( ( ( val / 10 ) << 4 ) | ( val % 10 ) );
}

#ifdef __AVR__
static inline uint8_t	rtcRead					( uint8_t *buff , uint8_t leng )	
{
	
	if( i2c_start( RX8564_ADDR + I2C_WRITE ) )
	{
		
		#ifdef _WITH_ERROR_REPORT_
			errorWriteCircular( &err , _ERROR_RTC_I2C_ , ERROR_I2C_NO_ACK );
		#endif
	}
	
	if ( i2c_write( buff[0] ) )// Register Adresse
	{
		#ifdef _WITH_ERROR_REPORT_
			errorWriteCircular( &err , _ERROR_RTC_I2C_ , ERROR_I2C_ADDRESS_TX );
		#endif
	}
	
	if ( i2c_rep_start( RX8564_ADDR + I2C_READ ) )
	{
		#ifdef _WITH_ERROR_REPORT_
			errorWriteCircular( &err , _ERROR_RTC_I2C_ , ERROR_I2C_NO_ACK );
		#endif
		
		i2c_stop();
		
		return 1;
	}
	
	for ( uint8_t i = 0 ; i < leng ; i++ )
	{
		if ( leng == 1 )
		{
			*buff = i2c_readNak();
			return 0;
		}
		else
		{
			*buff++ = i2c_readAck();
		}
	}
	
	*buff = i2c_readNak();
	
	i2c_stop();
	
	return 0;
}

static inline uint8_t	rtcWrite				( uint8_t *buff , uint8_t leng )	
{
	i2c_start_wait( RX8564_ADDR + I2C_WRITE );

	for ( uint8_t i = 0 ; i < leng ; i++ )
	{
		if ( leng == 1 )
		{
			if ( i2c_write( *buff ) )
			{
				#ifdef _WITH_ERROR_REPORT_
					errorWriteCircular( &err , _ERROR_RTC_I2C_ , ERROR_I2C_BYTE_TX );	
				#endif
			}
			
			i2c_stop();
			
			return 0;
		}
		else
		{
			if ( i2c_write( *buff++ ) )
			{
				#ifdef _WITH_ERROR_REPORT_
					errorWriteCircular( &err , _ERROR_RTC_I2C_ , ERROR_I2C_BYTE_TX );
				#endif
			}
		}
	}
	
	i2c_stop();
	
	return 0;
}


void					rtcInit					( void )															
{
	buff[0] = RX8564_CONTROL_1;
	buff[1] = 0x00;
	rtcWrite( buff , sizeof( buff ) );
}

void					rtcSetTime				( uint8_t hour , uint8_t minutes , uint8_t seconds )				
{
	buff[0] = RX8564_SECONDS;
	buff[1] = rtcDecToBcd( seconds );
	buff[2] = rtcDecToBcd( minutes );
	buff[3] = rtcDecToBcd( hour );
	rtcWrite( buff , sizeof( buff ) );
} 
   
void					rtcSetDate				( uint8_t day , uint8_t weekDay , uint8_t month, uint16_t year )	
{
	buff[0] = RX8564_DAYS;
	buff[1] = rtcDecToBcd( day );
	buff[2] = rtcDecToBcd( weekDay );
	buff[3] = rtcDecToBcd( month );
	buff[4] = rtcDecToBcd( year - 2000 );
	rtcWrite( buff , sizeof( buff ) );    
}
  
void					rtcSetAlert				( uint8_t day , uint8_t weekDay , uint8_t hour, uint8_t minutes )	
{
	buff[0] = RX8564_MINUTE_ALERT;
	buff[1] = rtcDecToBcd( minutes );
	buff[2] = rtcDecToBcd( hour );
	buff[3] = rtcDecToBcd( day );
	buff[4] = rtcDecToBcd( weekDay );
	rtcWrite( buff , sizeof( buff ) );
 }
  
void					rtcSetClkOut			( uint8_t frequency )	
{    
	buff[0] = RX8564_CLKOUT_FREQUENCY;
	buff[1] = frequency;
	rtcWrite( buff , sizeof( buff ) );	
}
 
void					rtcGetData				( rx8564_t *buffer )	
{   
	buff[0] = RX8564_SECONDS;
	rtcRead( buff , sizeof( buff ) );
			
    buffer->second			= buff[0];
    buffer->minute			= buff[1];  
    buffer->hour			= buff[2];
    buffer->day				= buff[3];
    buffer->dayName			= buff[4];
    buffer->month			= buff[5];
    buffer->year			= buff[6];
    buffer->alrt_minute		= buff[7];
    buffer->alrt_hour		= buff[8];
    buffer->alrt_day		= buff[9];
    buffer->alrt_dayName	= buff[10];

 
    buffer->second			&= 0x7F;
    buffer->minute			&= 0x7F;
    buffer->hour			&= 0x3F;
          
    buffer->day				&= 0x3F;
    buffer->month			&= 0x1F;
    buffer->dayName			&= 0x07;
     
    buffer->alrt_minute		&= 0x7F;
    buffer->alrt_hour		&= 0x7F;
    buffer->alrt_day		&= 0x7F;
    buffer->alrt_dayName	&= 0x7F;
}
  
void					rtcSetCtrl2				( uint8_t mask )		
{
	buff[0] = RX8564_CONTROL_2;
	buff[1] = mask;
	rtcWrite( buff , sizeof( buff ) );
}
 
void					rtcSetTimerControl		( uint8_t mask )		
{
	buff[0] = RX8564_TIMER_CONTROL;
	buff[1] = mask;
	rtcWrite( buff , sizeof( buff ) );
}

uint8_t					rtcReadTimer			( void )				
{	
	buff[0] = RX8564_TIMER;
	rtcRead( buff , sizeof( buff ) );

	return buff[0];
}

#endif

bool					rtcIsLeapYear			( const uint16_t year )							
{
  // Die Regel lautet: Alles, was durch 4 teilbar ist, ist ein Schaltjahr.
  // Es sei denn, das Jahr ist durch 100 teilbar, dann ist es keins.
  // Aber wenn es durch 400 teilbar ist, ist es doch wieder eins.

	if ( ! (year % 400 ) )
	{
		return true;
	}
	else if ( ! ( year % 100 ) )
	{
		return false;
	}
	else if ( ! ( year % 4 ) )
	{
		return true;	
	}
    
  return false;
}                   

uint16_t				rtcGetNumOfDaysAtMonth	( const uint8_t month , const uint16_t year )	
{
  //                     ungueltig,Jan,Feb,Mrz,Apr,Mai,Jun,Jul,Aug,Sep,Okt,Nov,Dez
  uint8_t daysPerMonth[] = {  0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  if ( month == 2 )
  {
    // Februar: Schaltjahr unterscheiden
    if ( rtcIsLeapYear( year ) )
    {
    	return 29;
	}
    else
    {
    	return 28;
	} 
  }

  if ( ( month >= 1 ) && ( month <= 12 ) )
  {
  	return daysPerMonth[month];
  }
  else
  {
    return 0;
  }
  
}                   

uint16_t				rtcGetDays				( const uint16_t year )							
{
  return ( rtcIsLeapYear( year ) ) ? 366 : 365;
}                   

uint16_t				rtcGetWeekDay			( const uint8_t day , const uint8_t month , const uint16_t year )	
{
	//                       ungueltig Jan Feb Mrz Apr Mai Jun Jul Aug Sep Okt Nov Dez 
	uint8_t monthOffset[13] = {  0 ,  0 ,  3 ,  3 ,  6 ,  1 ,  4 ,  6 ,  2 ,  5 ,  0 ,  3 ,  5 };
	uint16_t result = 0;
	
	
	// Monat / Tag - Plausi pruefen:
	if ( ( day > 31 ) || ( month > 12 ) || ( month <= 0 ) || ( day <= 0 ) || ( year <= 0 ) )
	{
		return -1;
	}
	
	uint8_t dayDigit 		= ( day % 7 );
	uint8_t monthDigit 		= monthOffset[month];
	uint8_t yearDigit 		= ( ( year % 100 ) + ( ( year % 100 ) / 4 ) ) % 7;
	uint8_t centuryDigit 	= ( 3 - ( ( year / 100 ) % 4 ) ) * 2;
	
	// Schaltjahreskorrektur:
	if ( ( month <= 2 ) && ( rtcIsLeapYear( year ) ) )
	{
		dayDigit = dayDigit + 6;
	}
	
	result = ( dayDigit + monthDigit + yearDigit + centuryDigit ) % 7;
	
	// Ergebnis:
	// 0 = Sonntag
	// 1 = Montag
	// 2 = Dienstag
	// 3 = Mittwoch
	// 4 = Donnerstag
	// 5 = Freitag
	// 6 = Samstag
	return result;
}   

uint16_t				rtcGetDayofYear			( const uint8_t day , const uint8_t month , const uint16_t year )	
{
  // Der wievielte Tag des Jahres ist dieser Tag
  if ( ( month == 0 ) || ( month > 12 ) )
  {
    return -1;
  }

  uint8_t localDay = day;
  uint8_t localMonth = month;

  while ( localMonth > 1 )
  {
    localMonth--;
    localDay += rtcGetNumOfDaysAtMonth( localMonth , year );
  }

  return localDay;
}                   

uint16_t				rtcGetWeek				( uint16_t day , uint16_t month , uint16_t year )					
{
  // Berechnung erfolgt analog DIN 1355, welche besagt:
  // Der erste Donnerstag im neuen Jahr liegt immer in der KW 1.
  // "Woche" ist dabei definiert als [Mo, ..., So].

  uint16_t dayOfYear = rtcGetDayofYear( day , month , year );

  // Berechnen des Wochentags des 1. Januar:
  uint16_t weekDay1Jan = rtcGetWeekDay( 1 , 1 , year );

  // Sonderfaelle Freitag und Samstag
  if (weekDay1Jan >= 5) 
  {
  	weekDay1Jan = weekDay1Jan - 7;
  }
    
  // Sonderfaelle "Jahresanfang mit KW - Nummer aus dem Vorjahr"
  if ( ( dayOfYear + weekDay1Jan ) <= 1 )
  {
    return rtcGetWeek( 31 , 12 , year - 1 );
  }

  uint16_t week = ( ( dayOfYear + weekDay1Jan + 5 ) / 7 );

  // 53 Kalenderwochen hat grunds?tzlich nur ein Jahr, 
  // welches mit einem Donnerstag anf?ngt !
  // In Schaltjahren ist es auch mit einem Mittwoch moeglich, z.B. 1992
  // Andernfalls ist diese KW schon die KW1 des Folgejahres.
  if ( week == 53 )
  {
    bool leapYear = rtcIsLeapYear( year );

    if ( ( weekDay1Jan  ==  4 ) ||  ( weekDay1Jan  == -3 ) ||  ( ( weekDay1Jan ==  3 ) && leapYear ) || ( ( weekDay1Jan == -4 ) && leapYear ) )
    {
    	// Das ist korrekt und erlaubt
    }
    else
    {
    	week = 1; // Korrektur des Wertes
	}
      
  }

  return week;
}      
