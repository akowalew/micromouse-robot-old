#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "USART/usart.h"
#include "bluetoothIO.h"

#include <avr/pgmspace.h>
#include "USART/buforCykliczny.h"



int main()
{

	BuforCykliczny<uint8_t, 64> bufRX ;



	Usart::initUsart() ;
	BluetoothIO::inicjacjaObslugi() ;

	char str[60] ;


	while(1)
	{
		Usart::readLine(str, 59) ;

		BluetoothIO::obslugaKomunikatu(str) ;
	}
}

