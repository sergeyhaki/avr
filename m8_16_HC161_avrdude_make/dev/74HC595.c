#define	s74HC595PORT				PORTB
#define	s74HC595DDR					DDRB

#define	SH_CP						PB4			//защелка
#define	ST_CP						PB3			//синхранизация
#define	DS							PB2			//данные

#define T595						10			//us задержка

	//инициализация s74HC595
	void s74HC595ini ()
	{		
		s74HC595DDR |= (1<<DS)|(1<<ST_CP)|(1<<SH_CP);
		s74HC595PORT |= (1<<ST_CP)|(1<<SH_CP);
	}

	//вывод в один регистр 74HC595
	void s74HC595 (char ch)
	{
		//счетчик бит
		char counter = 8;
		//начало цикла
		while (counter--)
		{
			if (ch & 0x80)				
				s74HC595PORT |= (1<<DS);
			else
				s74HC595PORT &= ~(1<<DS);				
			ch <<= 1;
			//задержка
			//_delay_us(T595);
			//записль бита по спаду
			s74HC595PORT &= ~(1<<ST_CP);
			//задержка
			//_delay_us(T595);
			//поднимаем синхранизацию
			s74HC595PORT |= (1<<ST_CP);															
		}
		//задержка
		//_delay_us(T595);
		//защелкиваем регистр по спаду
		s74HC595PORT &= ~(1<<SH_CP);
		//_delay_us(T595);
		s74HC595PORT |= (1<<ST_CP)|(1<<SH_CP);
	}

	//вывод в два регистра 74HC595
	void two74HC595 (unsigned int ch)
	{
		//счетчик бит
		int counter = 16;						
		//начало цикла
		while (counter--)
		{
			if (ch & 0x8000)				
				s74HC595PORT |= (1<<DS);
			else
				s74HC595PORT &= ~(1<<DS);				
			ch <<= 1;
			//задержка
			//_delay_us(T595);
			//записль бита по спаду
			s74HC595PORT &= ~(1<<ST_CP);
			//задержка
			//_delay_us(T595);
			//поднимаем синхранизацию
			s74HC595PORT |= (1<<ST_CP);															
		}
		//задержка
		//_delay_us(T595);
		//защелкиваем регистр по спаду
		s74HC595PORT &= ~(1<<SH_CP);
		//_delay_us(T595);
		s74HC595PORT |= (1<<ST_CP)|(1<<SH_CP);
	}

