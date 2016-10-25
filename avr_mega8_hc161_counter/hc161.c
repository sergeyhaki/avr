void resetHC161(void)
{	
	PORTB &= ~(1<<PB0);
	//_delay_ms(100);
	PORTB |= (1<<PB0);
	//_delay_ms(100);
}

void pulseHC161(uint16_t cnt)
{
	for (uint16_t i = 0; i < cnt; i++)
	{		
		//_delay_us(1);
		PORTB |= (1<<PB2);
		//_delay_us(1);
		PORTB &= ~(1<<PB2);		
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
	if(PIND & (1<<PD4))	
		data = data + 8;
	return data;
}