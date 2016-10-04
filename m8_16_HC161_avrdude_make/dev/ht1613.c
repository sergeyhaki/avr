/*	t2313		m8
SK	PB6 		PB4
DI	PB7 		PB5		*/
#define	ht1613PORT		PORTB
#define	ht1613DDR		DDRB
#define	ht1613SK		PB6
#define	ht1613DI		PB7

#define TA				1	//us
#define TB				2	//us
#define TC				5	//us

//*******ФУНКЦИИ******************************************************

	//инициализация
	void ht1613_Init()
	{
		ht1613DDR |= (1<<ht1613DI)|(1<<ht1613SK);
	}

	//вывод символа на индикатор ht1613
	void ht1613t(char data, char flag)
	{
		if((data==0) && (flag!=0))
			data = 10;							//символ 0 в кодировке ht1613 
		_delay_us(TC);							//задержка между тетрадами
		
		unsigned char i = 4;
		while(i--)
		{
			_delay_us(TB);						
			if(data & 0x08)
				ht1613PORT	|= (1<<ht1613DI);
			else
				ht1613PORT	&= ~(1<<ht1613DI);
			ht1613PORT	|= (1<<ht1613SK);			
			_delay_us(TA);						//задержка 
			ht1613PORT	&= ~(1<<ht1613SK);
			data <<= 1;
		}
	}

	//вывод 10-ти значного числа на индикатор ht1613
	void ht1613Int(unsigned long data)
	{
		//очистка индикатора 
		for (char i=0;i<=10;i++)
			ht1613t(0,0);
		//проверка на 0
		if (data!=0)
		{
			char a;
			char flag=0;
			for (unsigned long i=1000000000;i>0;i/=10)
			{
				a = data/i;
				if ((flag==0) && (a != 0))
					flag = 1;
				ht1613t (a,flag);
				data = data-a*i;
			}
		}
		else
			ht1613t (10,1);		
	}


	//вывод 10-ти значного числа на индикатор ht1613 без очиски индикатора
	void ht1613_Int(unsigned long data)
	{
		//проверка на 0
		if (data!=0)
		{
			char a;
			char flag=0;
			for (unsigned long i=1000000000;i>0;i/=10)
			{
				a = data/i;
				if ((flag==0) && (a != 0))
					flag = 1;
				if (flag!=0)
					ht1613t (a,flag);
				data = data-a*i;
			}
		}
		else
			ht1613t (10,1);		
	}

	//очистка индикатора
	void  ht1613Clr()
	{
		for (char i=0;i<=10;i++)
			ht1613t(0,0);
	}



/* таблица знакогенератора
	0   ПУСТО
	1	1
	2	2
	3	3
	4	4
	5	5
	6	6
	7	7
	8	8
	9	9
	10	0
	11	F
	12	_|
	13 	|_
	14	P
	15	-
*/




