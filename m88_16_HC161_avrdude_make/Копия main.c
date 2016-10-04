#define F_CPU			20000000

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

//*****ФАЙЛЫ УСТРОЙСТВ***********************************************
//#include "dev/usart_m88.c"
//#include "dev/iic.c"
//#include "dev/ht1613.c"
//#include "dev/74HC595.c"
//#include "dev/PCF8563.c"
//#include "dev/PCF8574.c"
//#include "dev/DS1621.c"
#include "dev/LCD4.c"


//*****ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ******************************************/
static FILE lcd_stdout = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);
//static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);volatile uint8_t Event = 0;

volatile uint8_t TCNT0Save;
volatile uint16_t Counter = 0;
volatile uint16_t CounterSave;
volatile uint16_t temp;
uint32_t freq;

//***** ПРЕРЫВАНИЯ **********************************************************
ISR(TIMER1_COMPA_vect)
{
	//PORTB ^= (1<<PB1);
	PORTB |= (1<<PB1);
	
	TCNT0Save = TCNT0;
	TCNT0 = 0;
	
	CounterSave = Counter;
	Counter = 0;	
}

ISR(TIMER1_COMPB_vect)
{
	//PORTB ^= (1<<PB2);
	PORTB &= ~(1<<PB1);

}



ISR(TIMER0_OVF_vect)
{
	Counter++;
	PORTB ^= (1<<PB0);
}

ISR(INT0_vect)
{
	LCD_XY(0,0);
	printf("INT0        ");

}

ISR(INT1_vect)
{
	LCD_XY(0,0);
	printf("INT1         ");

}


//*****ФУНКЦИИ*******************************************************

//*****MAIN**********************************************************
int main()
{
	//порты ввода-вывода
	DDRB = (1<<PB0)|(1<<PB1)|(1<<PB2);
	
	//подтяжка;	
	PORTD |= (1<<PD2)|(1<<PD3);
	
	_delay_ms(100);
	
	//инициализация HD77480
	LCD_Init();	
	stdout = &lcd_stdout;
	printf("Hello!\n");
	
	
	//*****TIMER1  ctc mode*****
	;TCCR1A = 0;				//
	TCCR1B = (1<<WGM12)|5;		//CTC mode - F_CPU/1024  
	OCR1A = 19531;	 			// 1sec 19531
	OCR1B = 5000;	
	
	//разрешение прерывания по совпадению А
	TIMSK1 = (1<<OCIE1A)|(1<<OCIE1B);
	
	//*****TIMER0 counter mode*****		
	TCCR0B = 7;					//по фронту внешнего сигнала		
	TIMSK0 |= (1<<TOIE0);		//разрешение прерывания по переполнению
	
	//*****INT0/INT1*****
	EIMSK |= (1<<INT0)|(1<<INT1);	//разрешение прерывания по INT0/INT1
		
	//глобальное разрешение прерывания 
	sei();
	
	while(1)
	{
		PORTB &= ~(1<<PB0);	
		freq = (CounterSave << 8) + TCNT0Save;
		
		LCD_XY(0,0);
		printf("TCN=%i",TCNT0Save);
		printf(" Co=%i       \n",CounterSave);
		printf("Freq=%luHz      ",freq);
		_delay_ms(1000);
		
	}//end while
}//end main





/*

	//ставим признак события совпадение
	Event = 1;
	PORTB ^= (1<<PB1);
	for (uint8_t i=0;i<=60; i++)
	{
		LCD_XY(0,1);
		printf("n=%d     ", i);
		_delay_ms(1000);	
	}
	
	
	

	printf("ttt!\n");
	
	//инициализация USART
	//USART_Init(MYUBRR);	
	//stdout = &uart_stdout;
	
	
	//printf("LCD4_DDR %d \n",LCD4_DDR);
	//printf("LCD4_DDR_E %d \n",LCD4_DDR_E);
	
		if (Event == 1){
			cli();//запрещаем прерывания
			PORTB ^= (1<<PB0);
			
			freq = (CountOvf<<8) + TCNT0;	

			
			LCD_XY(0,0);			
			printf ("F=%lu Hz          ", freq);			
			//printf ("CountOvf=%d", CountOvf);
			printf ("\nTCNT0= %d  ", TCNT0);			
			printf ("%d  ", count++);
			
			
			CountOvf = 0;
			TCNT1 = 0; // начинаем счет с начала
			TCNT0 = 0; // начинаем счет с начала			
			Event = 0; // признак ожидания события
			PORTB ^= (1<<PB0);
			sei();

*/



