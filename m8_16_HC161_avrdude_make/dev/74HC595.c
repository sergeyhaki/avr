#define	s74HC595PORT				PORTB
#define	s74HC595DDR					DDRB

#define	SH_CP						PB4			//�������
#define	ST_CP						PB3			//�������������
#define	DS							PB2			//������

#define T595						10			//us ��������

	//������������� s74HC595
	void s74HC595ini ()
	{		
		s74HC595DDR |= (1<<DS)|(1<<ST_CP)|(1<<SH_CP);
		s74HC595PORT |= (1<<ST_CP)|(1<<SH_CP);
	}

	//����� � ���� ������� 74HC595
	void s74HC595 (char ch)
	{
		//������� ���
		char counter = 8;
		//������ �����
		while (counter--)
		{
			if (ch & 0x80)				
				s74HC595PORT |= (1<<DS);
			else
				s74HC595PORT &= ~(1<<DS);				
			ch <<= 1;
			//��������
			//_delay_us(T595);
			//������� ���� �� �����
			s74HC595PORT &= ~(1<<ST_CP);
			//��������
			//_delay_us(T595);
			//��������� �������������
			s74HC595PORT |= (1<<ST_CP);															
		}
		//��������
		//_delay_us(T595);
		//����������� ������� �� �����
		s74HC595PORT &= ~(1<<SH_CP);
		//_delay_us(T595);
		s74HC595PORT |= (1<<ST_CP)|(1<<SH_CP);
	}

	//����� � ��� �������� 74HC595
	void two74HC595 (unsigned int ch)
	{
		//������� ���
		int counter = 16;						
		//������ �����
		while (counter--)
		{
			if (ch & 0x8000)				
				s74HC595PORT |= (1<<DS);
			else
				s74HC595PORT &= ~(1<<DS);				
			ch <<= 1;
			//��������
			//_delay_us(T595);
			//������� ���� �� �����
			s74HC595PORT &= ~(1<<ST_CP);
			//��������
			//_delay_us(T595);
			//��������� �������������
			s74HC595PORT |= (1<<ST_CP);															
		}
		//��������
		//_delay_us(T595);
		//����������� ������� �� �����
		s74HC595PORT &= ~(1<<SH_CP);
		//_delay_us(T595);
		s74HC595PORT |= (1<<ST_CP)|(1<<SH_CP);
	}

