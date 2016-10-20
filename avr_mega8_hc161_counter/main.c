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

#define VERSION			3

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
volatile uint32_t Counter = 0;
uint32_t temp;
uint32_t freq;

//***** ПРЕРЫВАНИЯ **********************************************************
	
ISR(TIMER0_OVF_vect)
{
	Counter++;
}

ISR(INT0_vect)
{
	Event = EVENT_INT0;
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
	DDRB = (1<<PB0)|(1<<PB1)|(1<<PB2);	
	PORTB |= (1<<PB0)|(1<<PB1)|(1<<PB2);
	
	//подтяжка INT0 INT1;	
	PORTD |= (1<<PD2)|(1<<PD3);
	
	_delay_ms(1000);
	
	//инициализация HD77480
	LCD_Init();	
	stdout = &lcd_stdout;
	//LCD_pgm_text(hello_msg);
	
	//*****TIMER1  ctc mode output>OC1A*****
	
	/*
	TCCR1A = (1<<COM1A0);		//изменять значение выхода при совпадении
	TCCR1B = (1<<WGM12)|5;		//CTC mode - F_CPU/1024  
	OCR1A = 15624;	 			// 1sec (16MHz)
	*/
	
	//разрешение прерывания по совпадению А
	//TIMSK1 = (1<<OCIE1A);
	
	//*****TIMER0 counter mode*****		
	TCCR0 = 7;				//по фронту внешнего сигнала		
	TIMSK = (1<<TOIE0);		//разрешение прерывания по переполнению
	
	//*****INT0/INT1*****
	GICR |= (1<<INT0)|(1<<INT1);	//разрешение прерывания по INT0/INT1
	MCUCR |= (1<<ISC01);			//падение -\_ вызывает прерывание EICRA
		
	//глобальное разрешение прерывания 
	//sei();
	
	printf("\nDEBUG VERSION %i",VERSION);
	

		
	
	while(1)
	{		
		
		for (uint16_t i = 0; i < 6000; i++)
		{		
			
			_delay_ms(500);
			PORTB |= (1<<PB1);
			resetHC161();
			TCNT0 = 0;
			
			_delay_ms(50);	
			pulseHC161(i);	
			_delay_ms(50);
			
			PORTB &= ~(1<<PB1);
			pulseHC161(i);
			_delay_ms(50);
			
			
			
			LCD_XY(0,0);
			printf("i=%i ",i);
			printf("T=%i \n",TCNT0);
			printf("HC161=%i   ",readHC161());
		}		
	}//end while
	
}//end main




