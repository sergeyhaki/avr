void resetHC161(void)
{	
	PORTB &= ~(1<<PB0);
	_delay_ms(10);
	PORTB |= (1<<PB0);
	_delay_ms(10);
}


void startDebug(void)
{	
	LCD_XY(0,0);
	printf("DEBUG VERSION %i\n",VERSION);
	_delay_ms(1000);
}

void pulseHC161(uint16_t cnt)
{
	for (uint16_t i = 0; i < cnt; i++)
	{
		PORTB &= ~(1<<PB2);
		_delay_ms(10);
		PORTB |= (1<<PB2);
		_delay_ms(10);	
	}
}

uint8_t readHC161(void)
{
	uint16_t data = 0;
	if(PIND & (1<<PD0))	
		data = data + 1;				
	if(PIND & (1<<PD1))	
		data = data + 2;				
	if(PIND & (1<<PD6))	
		data = data + 4;
	if(PIND & (1<<PD7))	
		data = data + 8;
	return data;
}