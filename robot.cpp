#define F_CPU 16000000UL

#include <avr/io.h>
#include "Bluetooth/bluetoothIO.h"
#include "mainProgramFunctions.h"

int main()
{
	BluetoothIO::initBt() ;
	char str[64] ;	// bufor do odczytu z RX

	while(1)
	{
		if(isSetFlag(READ_MSG_FLAG))
		{
			clearFlag(READ_MSG_FLAG) ;

			BluetoothIO::getReadMessage((uint8_t *) str) ;	// kopiujemy odczytany string do tablicy
			BluetoothIO::checkMessage(str) ;	// sprawdzamy, czy możemy z tym stringiem coś uczynić.
		}
	}
}

