/*
 * timer0.h
 *
 *  Created on: 21 paź 2015
 *      Author: dicker
 */

#ifndef TIMERS_TIMER0_H_
#define TIMERS_TIMER0_H_

#include <avr/io.h>

#ifdef __AVR_ATmega_328P__

namespace Timer0
{
	
	struct Wgm
	{
		enum
		{
			NORMAL = 0x00,			// TOP = 0xFF, OCRx Update = Immediate,	OVF = MAX
			PWM_PHASE_CORRECT = 0x01,	// TOP = 0xFF, OCRx Update = TOP, 		OVF = BOTTOM
			CTC = 0x02,			// TOP = OCR0, OCRx Update = Immediate,	OVF = MAX
			FAST_PWM = 0x03 ,		// TOP = 0xFF, OCRx Update = BOTTOM, 	OVF = MAX
			PWM_PHASE_CORRECT2 = 0x05,	// TOP = OCRA, OCRx Update = TOP,	OVF = BOTTOM
			FAST_PWM2 = 0x07, 		// TOP = OCRA, OCRx Update = BOTTOM,	OVF = TOP
		} ;
		uint8_t setting ;
	};

	// BOTTOM 	= The counter reaches the BOTTOM when it becomes zero (0x00).
	// MAX		= The counter reaches its MAXimum when it becomes 0xFF (decimal 255)
	// TOP		= The counter reaches the TOP when it becomes equal to the highest value in the count sequence (OCRn)

	struct WgmOptions
	{
		enum
		{
			OC0_DISCONNECTED = 0 
		} ;
		
		uint8_t setting ;
	};
	
	struct NormalCTCMode : WgmOptions
	{
		enum 
		{
			TOGGLE_OC0 = 1,		// TOGGLE OCn on compare match
			CLEAR_OC0 = 2,		// CLEAR OCn on compare match
			SET_OC0 = 3 ,		// SET OCn on compare match
		} ;
	};

	struct FastPwmMode : WgmOptions
	{
		enum
		{
			OC0_DISCONNECTED_ = 1,	// WGM02 = 0: Normal Port Operation, OC0A Disconnected
			CLEAR_OCR_SET_BOTTOM = 2,	// CLEAR OCn on compare match, SET OCn at BOTTOM, (non-invert)
			SET_OCR_CLEAR_BOTTOM = 3,	// SET OCn on compare match, CLEAR OCn at BOTTOM, (invert)
		} ;
	} ;
	
	struct FastPwm2Mode : WgmOptions
	{
		enum
		{
			CLEAR_OCR_SET_BOTTOM = 2,	// CLEAR OCn on compare match, SET OCn at BOTTOM, (non-invert)
			SET_OCR_CLEAR_BOTTOM = 3,	// SET OCn on compare match, CLEAR OCn at BOTTOM, (invert)
		} ;
	} ;

	struct PwmPhaseCorrectMode : WgmOptions
	{
		enum
		{
			CLEAR_UPCOUNT_SET_DOWNCOUNT = 2,	// CLEAR OCn on compare match when up-counting. SET OCn on compare match when downcounting.
			SET_UPCOUNT_CLEAR_DOWNCOUNT = 3 	// SET OCn on compare match when up-counting. CLEAR OCn on compare match when downcounting
		} ;
	};

	struct ClockSelect
	{
		enum
		{
			NO_CLK = 0 , 			// Zegar wyłączony
			CLK_1 = 0x01, 			// CLK / 1 (bez skalowania)
			CLK_8 = 0x02, 			// CLK / 8
			CLK_64 = 0x03, 			// CLK / 64
			CLK_256 = 0x04, 		// CLK / 256
			CLK_1024 = 0x05, 		// CLK / 1024
			EXT_T0_FALLING_EDGE = 0x06, 	// External clock source on T0 pin. Clock on falling edge.
			EXT_T0_RISING_EDGE = 0x07, 	// External clock source on T0 pin. Clock on rising edge.
		}
		uint8_t setting ;
	};
	
	

	inline uint8_t 	getClock() { return TCNT0 ; }
	inline void	setClock(uint8_t data) { TCNT0 = data ; }
	
	inline void	interruptsSettings(bool isOverflowInt, bool isOutputCompareAInt, bool isOutputCompareBInt)
	{
		TIMSK0 &= ~((1 << TOIE0) | (1 << OCIE0A) | (1 << OCIE0B)) ;
		TIMSK0 |= 	(isOverflowInt << TOIE0) |
				(isOutputCompareAInt << OCIE0A) |
				(isOutputCompareBInt << OCIE0B) ;
	}

	inline void	setOCRA(uint8_t data) { OCR0A = data ; }
	inline void	setOCRB(uint8_t data) { OCR0B = data ; }
	
	inline void 	timerInit(	WAVEFORM_GENERATE_MODE wgmMode = FAST_PWM,
					uint8_t modeFunctionality = CLEAR_OCR_SET_BOTTOM,
					CLOCK_SELECT cs = CLK_1)
	{
		TCCR0 = (modeFunctionality << COM00) | (cs << CS00) | (wgmMode << WGM00) ;
	}
}


#else // for example ATmega32

namespace Timer0
{
	enum WAVEFORM_GENERATE_MODE
	{
		NORMAL = 0,					// TOP = 0xFF, OCR0 Update = Immediate,	OVF = MAX
		PWM_PHASE_CORRECT = 0x01,	// TOP = 0xFF, OCR0 Update = TOP, 		OVF = BOTTOM
		CTC = 0x08,					// TOP = OCR0, OCR0 Update = Immediate,	OVF = MAX
		FAST_PWM = 0x09 ,			// TOP = 0xFF, OCR0 Update = BOTTOM, 	OVF = MAX
	};

	// BOTTOM 	= The counter reaches the BOTTOM when it becomes zero (0x00).
	// MAX		= The counter reaches its MAXimum when it becomes 0xFF (decimal 255)
	// TOP		= The counter reaches the TOP when it becomes equal to the highest value in the count sequence (OCRn)

	enum NORMAL_CTC_MODE
	{
		OC0_DISCONNECTED = 0 ,	// NORMAL port operation, OCn disconnected.
		TOGGLE_OC0 = 1,			// TOGGLE OCn on compare match
		CLEAR_OC0 = 2,			// CLEAR OCn on compare match
		SET_OC0 = 3 ,			// SET OCn on compare match
	};

	enum FAST_PWM_MODE
	{
		FAST_PWM_DISCONNECTED = 0,	// NORMAL port operation, OCn disconnected.
		CLEAR_OCR_SET_BOTTOM = 2,	// CLEAR OCn on compare match, SET OCn at BOTTOM, (non-invert)
		SET_OCR_CLEAR_BOTTOM = 3,	// SET OCn on compare match, CLEAR OCn at BOTTOM, (invert)
	} ;

	enum PWM_PHASE_CORRECT_MODE
	{
		PWM_PHASE_CORRECT_DISCONNECTED = 0,	// NORMAL port operation, OCn disconnected.
		CLEAR_UPCOUNT_SET_DOWNCOUNT = 2,	// CLEAR OCn on compare match when up-counting. SET OCn on compare match when downcounting.
		SET_UPCOUNT_CLEAR_DOWNCOUNT = 3 	// SET OCn on compare match when up-counting. CLEAR OCn on compare match when downcounting
	};

	enum CLOCK_SELECT
	{
		NO_CLK = 0 , 			// Zegar wyłączony
		CLK_1 = 0x01, 			// CLK / 1 (bez skalowania)
		CLK_8 = 0x02, 			// CLK / 8
		CLK_64 = 0x03, 			// CLK / 64
		CLK_256 = 0x04, 		// CLK / 256
		CLK_1024 = 0x05, 		// CLK / 1024
		EXT_T0_FALLING_EDGE = 0x06, 	// External clock source on T0 pin. Clock on falling edge.
		EXT_T0_RISING_EDGE = 0x07, 	// External clock source on T0 pin. Clock on rising edge.
	};
	// RIGHT FOR BOTH
	
	inline uint8_t 	getClock() { return TCNT0 ; }
	inline void	setClock(uint8_t data) { TCNT0 = data ; }
	
	


	inline void	interruptsSettings(bool isOverflowInt, bool isOutputCompareInt)
	{
		TIMSK &= ~((1 << TOIE0) | (1 << OCIE0)) ;
		TIMSK |= (isOverflowInt << TOIE0) | (isOutputCompareInt << OCIE0) ;
	}

	inline void	setOCR(uint8_t data) { OCR0 = data ; }
	
	inline void 	timerInit(	WAVEFORM_GENERATE_MODE wgmMode = FAST_PWM,
								uint8_t modeFunctionality = CLEAR_OCR_SET_BOTTOM,
								CLOCK_SELECT cs = CLK_1)
	{
		TCCR0 = (modeFunctionality << COM00) | (cs << CS00) | (wgmMode << WGM00) ;
	}
}
#endif




#endif /* TIMERS_TIMER0_H_ */
