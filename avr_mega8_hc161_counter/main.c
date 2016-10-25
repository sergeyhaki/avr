#define F_CPU			16000000

#include <math.h> 
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>

#define MYUBRR 			F_CPU/16/BAUD-1
#define BAUD 			9600

#define VERSION			8

#define EVENT_INT0			2
#define EVENT_INT1			3
#define EVENT_COMPA			4

//*****ФАЙЛЫ УСТРОЙСТВ**********************************************
#include "LCD4.c"
#include "hc161.c"

//******PROGMEM*****************************************************


//*****ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ*****************************************
static FILE lcd_stdout = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);
//static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

volatile uint8_t Event;
volatile uint16_t Counter = 0;
uint32_t temp;
volatile uint32_t Freq;

//***** ПРЕРЫВАНИЯ **********************************************************
	
ISR(TIMER0_OVF_vect)
{
	Counter++;
	PORTB ^= (1<<PB5);
}

ISR(INT0_vect)
{
	Event = EVENT_INT0;
	//Freq = (Counter<<12);
	//Freq = (Counter<<8) + TCNT0;
	Freq = ((uint32_t)Counter<<12)+((uint32_t)TCNT0<<4)+readHC161();
	LCD_XY(0,0);
	printf("F=%luHz       \n", Freq);	
	printf("T=%i ",TCNT0);
	printf("H=%i ",readHC161());
	printf("C=%i          ", Counter);
	
	resetHC161();
	TCNT0 = 0;
	Counter = 0;
}

ISR(INT1_vect)
{
	Event = EVENT_INT1;
}

//*****ФУНКЦИИ*******************************************************
			

//*****MAIN**********************************************************
int main()
{
	//порты ввода-вывода
	DDRB = (1<<PB0)|(1<<PB1)|(1<<PB3)|(1<<PB5);	
	PORTB |= (1<<PB0);
	
	//подтяжка INT0 INT1;	
	//PORTD |= (1<<PD2)|(1<<PD3);
	
	_delay_ms(1000);
	
	//инициализация HD77480
	LCD_Init();	
	stdout = &lcd_stdout;
	//LCD_pgm_text(hello_msg);
	
	//*****TIMER2  ctc mode output>OC2/PB3*****
	TCCR2  = (1<<WGM21)|(1<<COM20)|1;	//CTC mode - F_CPU/64
	OCR2 = 0;							//500Hz (16MHz)
	
	
	//*****TIMER1  ctc mode output>OC1A/PB2*****	
	TCCR1A = (1<<COM1A0);		//изменять значение выхода при совпадении
	TCCR1B = (1<<WGM12)|5;		//CTC mode - F_CPU/1024  
	OCR1A = 15624;	 			// 1sec (16MHz)
	
	
	//разрешение прерывания по совпадению А
	//TIMSK1 = (1<<OCIE1A);
	
	//*****TIMER0 counter mode*****		
	TCCR0 = 7;				//по фронту внешнего сигнала		
	TIMSK = (1<<TOIE0);		//разрешение прерывания по переполнению
	
	//*****INT0/INT1*****
	GICR |= (1<<INT0);	//разрешение прерывания по INT0/INT1
	MCUCR |= (1<<ISC01);			//падение -\_ вызывает прерывание EICRA
		
	//глобальное разрешение прерывания 
	sei();
	
	printf("\nDEBUG VERSION %i",VERSION);
	
	
	while(1)
	{	
	}//end while
	
}//end main
