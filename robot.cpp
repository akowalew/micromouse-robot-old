#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "USART/usart.h"
#include <string.h>
#include "bluetoothIO.h"

#define	LED PB5
volatile uint8_t zmienneFunkcyjne[4] ;
	
int main()
{
	
	Usart::initUsart() ;
	DDRB |= (1 << LED) ;
	
	char str[60] ;
	bool stan ;
	
	while(1)
	{
		PORTB &= ~(1 << LED) ;
		stan = Usart::readLine(str, 10) ;
		if(stan)
			PORTB |= (1 << LED) ;
			
		Usart::sendLine(str) ;	
	}
	
}

