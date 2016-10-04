	void USART_Init( unsigned int baud )
	{
		/* Set baud rate */
		UBRRH = (unsigned char)(baud>>8);
		UBRRL = (unsigned char)baud;
		/* Enable receiver and transmitter */
		UCSRB = (1<<RXEN)|(1<<TXEN);
		/* Set frame format: 8data, 2stop bit */
		//UCSRC = (1<<USBS)|(3<<UCSZ0);
	}


	void USART_Transmit( unsigned char data )
	{
		/* Wait for empty transmit buffer */
		while ( !( UCSRA & (1<<UDRE)) )
		;
		/* Put data into buffer, sends the data */
		UDR = data;
	}


	unsigned char USART_Receive( void )
	{
		/* Wait for data to be received */
		while ( !(UCSRA & (1<<RXC)) )
		;
		/* Get and return received data from buffer */
		return UDR;
	}



	//����� 10-�� �������� ����� ����� USART
	void USART_Int(unsigned long data)
	{
		//�������� �� 0
		if (data!=0)
		{
			char a;
			char flag=0;
			for (unsigned long i=1000000000;i>0;i/=10)
			{
				a = data/i;
				if ((flag==0) && (a != 0))
					flag = 1;
				if(flag!=0)
					USART_Transmit('0'+a);
				data = data-a*i;
			}
		}
		else
			USART_Transmit('0');		
	}



/*
			ASCII code

			//������ 32			
			USART_Transmit(' ');
			USART_Transmit(32);

			//�����	8		
			USART_Transmit('\b');	
			USART_Transmit(8);						

			//������� ������ 13			
			USART_Transmit('\r');	
			USART_Transmit(13);

*/

	//����� �������-����������� ����� 
	void USART_Bcd(unsigned char data)
	{
		unsigned char temp;
		//����� �������
		temp = (data & 0xf0)/0xf;
		USART_Transmit('0'+temp);
		//������ �������
		temp = (data & 0xf);
		USART_Transmit('0'+temp);
	}











