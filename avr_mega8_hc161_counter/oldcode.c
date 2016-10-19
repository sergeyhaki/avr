		if (Event == EVENT_INT0)
		{
			
			freq = (Counter * 256) + TCNT0;
			LCD_XY(0,0);
			printf("TCN=%i00,TCNT0);
			printf(" Co=%lu       \n",Counter);
			printf("Freq=%luHz      ",freq);
			
			resetHC161();
			
			TCNT0 = 0;
			Counter = 0;
			Event = 0;			
			
		}//end if	
	}//end while	
}//end main




/*
ISR(TIMER1_COMPA_vect)
{
	Event = EVENT_COMPA;
}


		switch(Event) 
		{
			case EVENT_INT0:
			//
			temp = TCNT0;
			
			freq = (Counter * 256) + (temp);
			//freq = Counter * 4096;			
			//freq = freq + (temp << 4);
			

			if(PIND & (1<<PD0))	
				freq = freq + 1;				
			if(PIND & (1<<PD1))	
				freq = freq + 2;				
			if(PIND & (1<<PD6))	
				freq = freq + 4;
			if(PIND & (1<<PD7))	
				freq = freq + 8;			
	
						
			LCD_XY(0,0);
			printf("TCN=%lu",temp);
			printf(" Co=%lu       \n",Counter);
			printf("Freq=%luHz      ",freq);
			//
						
			//reset hc161
			_delay_ms(10);
			PORTB &= ~(1<<PB0);
			_delay_ms(10);
			PORTB |= (1<<PB0);
			//
			freq = 0;
			TCNT0 = 0;
			Counter = 0;			
			Event = 0;			
			break;
		}//end switch		
	}//end while

	
	temp = 36;
	Counter = 273;
	freq = (Counter * 4096) + (temp * 36);
	//freq = 250000;
	LCD_XY(0,0);
	printf("TCN=%lu",temp);
	printf(" Co=%lu       \n",Counter);
	printf("Freq=%luHz      ",freq);
	_delay_ms(10000);



	//разрешение прерывания по совпадению А
	TIMSK1 = (1<<OCIE1A)|(1<<OCIE1B);


			freq = (Counter << 8) + TCNT0;			
			LCD_XY(0,0);
			printf("TCN=%i",TCNT0);
			printf(" Co=%i       \n",Counter);
			printf("Freq=%luHz      ",freq);			
			TCNT0 = 0;
			Counter = 0;
			Event = 0;


ISR(TIMER1_COMPA_vect)
{
	//PORTB ^= (1<<PB1);
	PORTB |= (1<<PB1);
	
	TCNT0Save = TCNT0;
	TCNT0 = 0;
	
	CounterSave = Counter;
	Counter = 0;	
}

		//проверяем флаг совпадения А таймера 1
		if (TIFR1 & (1<<OCF1A))
		{
			cli();
			PORTB &= ~(1<<PB0);
			
			freq = (Counter << 8) + TCNT0;			
			LCD_XY(0,0);
			printf("TCN=%i",TCNT0);
			printf(" Co=%i       \n",Counter);
			printf("Freq=%luHz      ",freq);			
			TCNT0 = 0;
			Counter = 0;
			//СБРОС ФЛАГА
			TIFR1 = (1<<OCF1A);	
			sei();		
		}

		//freq = (CounterSave << 8) + TCNT0Save;
		
		//LCD_XY(0,0);
		//printf("TCN=%i",TCNT0Save);
		//printf(" Co=%i       \n",CounterSave);
		//printf("Freq=%luHz      ",freq);
		//_delay_ms(1000);

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