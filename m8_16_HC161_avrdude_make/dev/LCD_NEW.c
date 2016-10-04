#define LCD4_E_D1			500 //uS 
#define LCD4_E_D2			1 //mS
#define LCD4_E_D3			1 //mS

#define LCD4_DATA			1
#define LCD4_CMD			0

#define LCD4_DDR			DDRC
#define LCD4_DDR_E			DDRB	
#define LCD4_PORT			PORTC
#define LCD4_PORT_E			PORTB

#define LCD4_E				PB0
#define LCD4_RS				PB1

#define LCD_SendData(x) 	LCD4_Send(x,LCD4_DATA)
#define LCD_SendCmd(x) 		LCD4_Send(x,LCD4_CMD)
#define LCD_Clear()			LCD4_Send(1,LCD4_CMD)
#define LCD_NewLine()		LCD4_Send(0xc0,LCD4_CMD)


void LCD4_Send(uint8_t data, uint8_t flag)
{
	uint8_t temp;		
	//1 nibble ������� ��������
	_delay_ms(LCD4_E_D3);		
	temp = (data>>4);		
	LCD4_PORT_E |= (1<<LCD4_E);		
	if (flag)
		LCD4_PORT_E |= (1<<LCD4_RS);
	else
		LCD4_PORT_E &= ~(1<<LCD4_RS);		
	LCD4_PORT = temp;		
	_delay_us(LCD4_E_D1);		
	LCD4_PORT_E &= ~(1<<LCD4_E);		
	//2 nibble ������� ��������
	_delay_ms(LCD4_E_D2);
	temp = (data & 0x00ff);
	LCD4_PORT_E |= (1<<LCD4_E);
	if (flag)
		LCD4_PORT_E |= (1<<LCD4_RS);
	else
		LCD4_PORT_E &= ~(1<<LCD4_RS);			
	LCD4_PORT = temp;		
	_delay_us(LCD4_E_D1);		
	LCD4_PORT_E &= ~(1<<LCD4_E);		
}//end LCD4_Semd


	inline static void LCD_Init()
	{
		LCD4_DDR |= 0x0f; //�������� 0..3
		LCD4_DDR_E |= (1<<LCD4_E)|(1<<LCD4_RS); //�������� E RS
		//initialisation
		_delay_ms(100);
		LCD4_PORT |= 0b00000010;
		LCD4_PORT_E |= (1<<LCD4_E);
		_delay_us(100);
		LCD4_PORT_E &= ~(1<<LCD4_E);
		_delay_ms(100);
		
		LCD4_Send(0b00101000, LCD4_CMD);	
		LCD4_Send(0b00000001, LCD4_CMD);
		LCD4_Send(0b00000110, LCD4_CMD);	
		LCD4_Send(0b00001100, LCD4_CMD);		
		LCD4_Send(0b00000001, LCD4_CMD);	
		LCD4_Send(0b00000001, LCD4_CMD);
	
	}//end LCD4_Init

	//����� ������: �������� - ��������� �� ����� � 
	//���������� ������
	void LCD_Text(char *pointer)
	{
		uint8_t i=0;
		while(pgm_read_byte (&pointer[i]))
			LCD_SendData(pgm_read_byte (&pointer[i++]));		
	}
	
	//����� ������ �����: �������� - ������� ����� �� ������
	void LCD_Int(long int data)
	{
		static char cBuffer[8 * sizeof (long int) + 1];
		ltoa (data, cBuffer, 10); 
		uint8_t i=0;
		while (cBuffer[i])
			LCD_SendData(cBuffer[i++]);
	}
	
	//��������� ������� � ������� (x,y)
	void LCD_XY(uint8_t x,uint8_t y)
	{
		if (y==0)
			LCD4_Send((0x80|x),LCD4_CMD);
		if (y==1)
			LCD4_Send((0x80|0x40|x),LCD4_CMD);	
	}
	
	//�������� ������ �������
	void LCD_New()
	{
		LCD_SendCmd(0x40);				// ������ 0
		LCD_SendData(0b11111111);		 
		LCD_SendData(0b10000011);		
		LCD_SendData(0b10000111);		
		LCD_SendData(0b10001111);		
		LCD_SendData(0b10000001);		
		LCD_SendData(0b10000001);		
		LCD_SendData(0b11111111);

		LCD_SendCmd(0x48);				// ������ 1
		LCD_SendData(0b01111100);		 
		LCD_SendData(0b10000001);		
		LCD_SendData(0b10011001);		
		LCD_SendData(0b11111000);		
		LCD_SendData(0b10011001);		
		LCD_SendData(0b10000001);		
		LCD_SendData(0b01111100);

		LCD_SendCmd(0x50);				// ������ 2
		LCD_SendData(0b00000001);		 
		LCD_SendData(0b00000011);		
		LCD_SendData(0b00000111);		
		LCD_SendData(0b00001111);		
		LCD_SendData(0b00111111);		
		LCD_SendData(0b00001111);		
		LCD_SendData(0b00000111);		
	}	
	
static int lcd_putchar(char c, FILE *stream)
{	
	if (c == '\n')
		LCD_NewLine();
	else
		LCD_SendData(c);
	return 0;
}//end lcd_putchar
	
	
	/* ��������� ���� � �������� ������������� ���-������. 
	������������� ����������� ����������� ��������� ��������� 
	������������������ �������� ��� �������������. ��������� 
	����� �� ����� 15 �� ����� ������������� �������� ���������� 
	������� (> 4,5 �) � ����������� �����-���� �������� � 
	������������. ������ ��������� ��������� �������, ���������� 
	����������� ���� (��� ������ ���� ������� $30 ���������� 
	�� ����, ����� ����������� ��������� �� ����������� 
	������������ � ����������), ������ ����� ����������� ���� 
	�������� �� ��������� �������� ����� BF. ����� ����� 
	��������� ����� �� ����� 4,1 �� � ��������� ������� ������ 
	����������� ����, ������ ����� ������� ������� ����� �� 
	����������� �������� ����� BF. ��������� ����� ���������� 
	����� ��������� �����, �� ���� ��� 100 ���, � � ������ ��� 
	��������� ������� ������������ ����������� ����, ����� ��� 
	�������� BF. ��� ��� �������� �������� ����������������� � 
	�������� ������� ���������� � �������� ����� ������ (�� ���� 
	��������� � ����� ������ � 8-�� ��������� �����) �� ������ 
	���������. ������ �� ���� ���������� �������� (��� 
	������������ ����, �� � ��������� ����� BF) ����������� 
	������������� ������� ������ � ������� ���������������� 
	������������������, ����������� ��������� � ������� 7 
	(���������� � ��� ����� ������� ������ ����������� 
	����������� ����).	���������� �������, ��� ����� �� 
	���������� ����� ������ � 4-� ��������� �����, �� ���� 
	������� ������� $20, �� ������� ��� ������ �� 8-�� 
	���������� ������, ������� ��������������� ������������� 
	����� ������ ���������� �������, � ������ �� �� ������� 
	��������� �������� ����������� �������� ������ N � F, 
	��������������� � ������� ������� ������� ��������� 
	����������� ����. ������� ������� ���������� ��������� 
	� ��� �������������� 4-� ��������� ������ ����� 
	���������������� �������� ���� ������, �� ���� ��� 
	4-� ���������� ������ �������.
	
	D7 	D6 	D5 	D4 	D3 	D2 	D1 	D0 	����������							 
	0 	0 	0 	0 	0 	0 	0 	1 	������� ������, �� = 0, ��������� �� �� DDRAM 
	0 	0 	0 	0 	0 	0 	1 	- 	�� = 0, ��������� �� DDRAM, �������� ������, ������ ������ ���������� � ������ DDRAM 
	0 	0 	0 	0 	0 	1 	I/D S 	���������� ����������� ������ ������� ��� ������ 
	0 	0 	0 	0 	1 	D 	C 	B 	���������� ����� ����������� 
	0 	0 	0 	1 	S/C R/L - 	- 	������� ������ �������/������ 
	0 	0 	1 	DL 	N 	F 	- 	- 	����������� ���������� ��������� � ������ ���� ������ 
	0 	1 	AG 	AG 	AG 	AG 	AG 	AG  ���������� �������� �� ������ � ������� CGRAM 
	1 	AD 	AD 	AD 	AD 	AD 	AD 	AD 	���������� �������� �� ������ � ������� DDRAM 
	
	
	
	INIT_LCD				; �������������� 
	WR_CGADR 0				; ��������� �� ������ ���������������. 
	WR_DATA 0b00000001		; ������ ������ ������ �����
	WR_DATA 0b00000010
	WR_DATA 0b00000100
	WR_DATA 0b00011111
	WR_DATA 0b00000010
	WR_DATA 0b00000100
	WR_DATA 0b00001000 
	WR_DDADR 0				; ��������� �� ������ ����� ������ (������ � ����������� 0,0) 
	WR_DATA 0				; � ��������������� ������� ��� 0 ��������� ���


	
	
	
	
	
	
	
	
	
	
	
	
	*/



	































