//шина iic 

#define	IIC_PORT		PORTB	//ВНИМАНИЕ! B IIC_PORT ВСЕГДА НОЛЬ
#define	IIC_DDR			DDRB
#define	IIC_PIN			PINB
#define	SDA				PB0		//данные
#define	SCL				PB1		//синхранизация
#define IIC_DELAY_A		100		//us задержка
#define ACK				1		//признак подверждения
#define NACK			0		//признак НЕподверждения

	inline void IIC_Init()
	{	
		IIC_PORT &= ~(1<<SCL)|(1<<SDA);
		IIC_DDR &= ~(1<<SCL)|(1<<SDA);
	}//end IIC_Init

	void IIC_Start(void)
	{//событие Srart		 		
		//для ПовСтарт
		//востановление SDA
		IIC_DDR &= ~(1<<SDA);
		_delay_us(IIC_DELAY_A);
		//востановление SCL				
		IIC_DDR &= ~(1<<SCL);		
		_delay_us(IIC_DELAY_A);
		//падение SDA
		IIC_DDR |= (1<<SDA);
		//пауза А
		_delay_us(IIC_DELAY_A);
		//падение SCL
		IIC_DDR |= (1<<SCL);
	}//end IIC_Start

	int IIC_Send(unsigned char data)
	{
		unsigned char counter = 8, ack = 0;
		//цикл тактовых сигналов
		while(counter--)
		{
			_delay_us(IIC_DELAY_A);
			if(data & 0x80)
				//востановление SDA
				IIC_DDR &= ~(1<<SDA);
			else
				//падение SDA			
				IIC_DDR |= (1<<SDA);
			_delay_us(IIC_DELAY_A);
			//востановление SCL				
			IIC_DDR &= ~(1<<SCL);
			while (!(IIC_PIN & (1<<SCL)));			
			/*	при отпускании SCL надо следить за тем, 
				что линия поднялась. Если не поднялась, 
				то надо остановиться и ждать до тех пор, 
				пока Slave ее не отпустит. Потом продолжить 
				с того же места.	*/
			_delay_us(IIC_DELAY_A);
			//падение SCL
			IIC_DDR |= (1<<SCL);
			_delay_us(IIC_DELAY_A);
			//падение SDA
			IIC_DDR |= (1<<SDA);
			//сдвиг слево
			data <<= 1;							
		}//end while
		_delay_us(IIC_DELAY_A);
		//востановление SDA
		IIC_DDR &= ~(1<<SDA);
		_delay_us(IIC_DELAY_A);
		//востановление SCL				
		IIC_DDR &= ~(1<<SCL);
		_delay_us(IIC_DELAY_A);
		while (!(IIC_PIN & (1<<SCL)));
		//проверяем сигнал подтверждения ACK
		if (IIC_PIN & (1<<SDA))
			ack = NACK;
		else 
		{
			ack = ACK;
			//падение SDA
			IIC_DDR |= (1<<SDA);
		}//end else
		//падение SCL
		IIC_DDR |= (1<<SCL);
		_delay_us(IIC_DELAY_A);
		return(ack);		
	}//end IIC_Send

	int IIC_Read(unsigned char IsAck)	
	{
		unsigned char data = 0,counter = 8;
		//востановление SDA
		IIC_DDR &= ~(1<<SDA);
		//цикл тактовых сигналов
		while(counter--)		
		{			
			//падение SCL
			IIC_DDR |= (1<<SCL);					
			_delay_us(IIC_DELAY_A);
			//востановление SCL				
			IIC_DDR &= ~(1<<SCL);			
			while (!(IIC_PIN & (1<<SCL)));
			/*	при отпускании SCL надо следить за тем, 
				что линия поднялась. Если не поднялась, 
				то надо остановиться и ждать до тех пор, 
				пока Slave ее не отпустит. Потом продолжить 
				с того же места.	*/
			//сдвиг вправо
			data <<= 1;
			//чтение SDA
			if(IIC_PIN & (1<<SDA))
				//устанавливаем бит
				data |= (1<<0);
			else
				//сбрасываем бит
				data &= ~(1<<0);
			_delay_us(IIC_DELAY_A);			
			//падение SCL
			IIC_DDR |= (1<<SCL);
			_delay_us(IIC_DELAY_A);							
		}//end while
		//формируем сигнал подтверждения ACK
		_delay_us(IIC_DELAY_A);
		_delay_us(IIC_DELAY_A);
		if (IsAck == 1)
			{//посылаем ACK				
				//падение SDA
				IIC_DDR |= (1<<SDA);
				_delay_us(IIC_DELAY_A);
				//востановление SCL				
				IIC_DDR &= ~(1<<SCL);	
				while (!(IIC_PIN & (1<<SCL)));
				_delay_us(IIC_DELAY_A);
				//падение SCL
				IIC_DDR |= (1<<SCL);
				_delay_us(IIC_DELAY_A);
				//востановление SDA
				IIC_DDR &= ~(1<<SDA);
			}//end if
		else
			{//посылаем NACK
				_delay_us(IIC_DELAY_A);
				//востановление SCL				
				IIC_DDR &= ~(1<<SCL);	
				while (!(IIC_PIN & (1<<SCL)));
				_delay_us(IIC_DELAY_A);
				//падение SCL
				IIC_DDR |= (1<<SCL);
				_delay_us(IIC_DELAY_A);
			}//end else
		return(data);
	}//end IIC_Read

	void IIC_Stop(void)
	{
		_delay_us(IIC_DELAY_A);
		//падение SDA
		IIC_DDR |= (1<<SDA);
		//падение SCL
		IIC_DDR |= (1<<SCL);
		//пауза А
		_delay_us(IIC_DELAY_A);
		//востановление SCL				
		IIC_DDR &= ~(1<<SCL);
		while (!(IIC_PIN & (1<<SCL)));
		//пауза А
		_delay_us(IIC_DELAY_A);
		//востановление SDA
		IIC_DDR &= ~(1<<SDA);
	}//end IIC_Stop









