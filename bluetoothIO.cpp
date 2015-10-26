#include "bluetoothIO.h"
#include <string.h>
#include "USART/usart.h"

uint8_t VARIABLES[4] = 
{
	0,
	240,
	255,
	40
} ;

const char ID[] = "ROB" ;
const char ID_OK[] = "ROB:OK" ;
const char ID_OK_READ[] = "ROB:OK:" ;
const char READ = '>' ;
const char WRITE = '<' ;
const char OPT = ':' ;

const uint8_t ID_LEN = sizeof(ID) / sizeof(char) - 1;
const uint8_t ID_OK_LEN = sizeof(ID_OK) / sizeof(char) - 1;
const uint8_t ID_OK_READ_LEN = sizeof(ID_OK_READ) / sizeof(char) - 1;

const char *OPT_ARGS[] = 
{
	"SEE",
	"RESET"
} ;

const void resetFun()
{
	Usart::sendLine("ROB:RESET") ;
}

const void seeFun()
{
	Usart::sendLine("ROB:SEE") ;
}

const void (*FUNCTION_PTRS[])() =
{
	seeFun,
	resetFun
} ;

const uint8_t FUNPTRS_LEN = sizeof(FUNCTION_PTRS) / sizeof(void *) ;
const uint8_t VARIABLES_LEN = sizeof(VARIABLES) / sizeof(uint8_t) ;

uint8_t myAtoi(uint8_t *u8Dest, const char *src)
{
	// zwraca ilość wczytanych znaków
	// wczytuje tylko liczbę 8bit, więc wczyta co najwyżej 3 cyfry

	uint8_t ret = 0 ;

	*u8Dest = 0 ;

	while(src[ret] >= '0' && src[ret] <= '9' && ret < 3)
		*u8Dest = ((*u8Dest) * 10) + (src[ret++] - '0') ;

	return ret ;
}

void sendVar(uint8_t whichVar)
{
	char message[ID_OK_READ_LEN+3+1] ; // + ":OK:" + 8bit + '\0'
	strcpy(message, ID_OK_READ) ;
	myItoa(VARIABLES[whichVar], message + ID_OK_READ_LEN) ;
	Usart::sendLine(message) ;
}

void myItoa(uint8_t var, char *dest)
{
	uint8_t i = 0 ;
	char tmp[3] ;

	if(var == 0)
	{
		dest[0] = '0' ;
		dest[1] = '\0' ;
		return ;
	}

	while(var)
	{
		tmp[i++] = (var % 10) + '0' ;
		var /= 10 ;
	}

	--i ;
	uint8_t j = i ;

	do
	{
		dest[j-i] = tmp[i] ;
	}while(i-- > 0) ;

	dest[j+1] = '\0' ;
}

void obslugaKomunikatu(const char *komunikat)
{
	if(strncmp(komunikat, ID, ID_LEN) == 0)
	{
		if(komunikat[ID_LEN] == '\0') // wprowadził tylko ID
			Usart::sendLine(ID) ;
		else
		{
			const char *ptr = komunikat + ID_LEN + 1 ;

			if(komunikat[ID_LEN] == OPT)	// komendy pozostałe
			{
				for(uint8_t i = 0 ; i < FUNPTRS_LEN ; i++)
					if(strcmp(OPT_ARGS[i], ptr) == 0)
						FUNCTION_PTRS[i]() ;
			}
			else if(komunikat[ID_LEN] == READ)	// komputer chce pobrać zmienne
			{
				if(strcmp(ptr, "A") == 0) // wczytujemy wszystko
				{
					for(uint8_t i = 0 ; i < VARIABLES_LEN ; i++)
						sendVar(i) ;

				}
				else	// podany zostanie konkretny numer zmiennej
				{
					uint8_t ileWczytanych;
					uint8_t varNum ;

					ileWczytanych = myAtoi(&varNum, ptr) ;

					if(ileWczytanych > 0 && varNum < VARIABLES_LEN)
						sendVar(varNum) ;
				}
			}
			else if(komunikat[ID_LEN] == WRITE)
			{
				uint8_t varNum ;
				uint8_t ileWczytanych = myAtoi(&varNum, ptr) ;

				if(ileWczytanych > 0 && varNum < VARIABLES_LEN && ptr[ileWczytanych] == ':')
					myAtoi(&VARIABLES[varNum], ptr + ileWczytanych + 1) ;

				Usart::sendLine(ID) ;
			}
		}
	}
}
	
