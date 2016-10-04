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
	//1 nibble старший полубайт
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
	//2 nibble младший полубайт
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
		LCD4_DDR |= 0x0f; //занимаем 0..3
		LCD4_DDR_E |= (1<<LCD4_E)|(1<<LCD4_RS); //занимаем E RS
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

	//вывод текста: аргумент - указатель на текст в 
	//програмной памяти
	void LCD_Text(char *pointer)
	{
		uint8_t i=0;
		while(pgm_read_byte (&pointer[i]))
			LCD_SendData(pgm_read_byte (&pointer[i++]));		
	}
	
	//вывод целого числа: аргумент - длинное целое со знаком
	void LCD_Int(long int data)
	{
		static char cBuffer[8 * sizeof (long int) + 1];
		ltoa (data, cBuffer, 10); 
		uint8_t i=0;
		while (cBuffer[i])
			LCD_SendData(cBuffer[i++]);
	}
	
	//установка курсора в позиции (x,y)
	void LCD_XY(uint8_t x,uint8_t y)
	{
		if (y==0)
			LCD4_Send((0x80|x),LCD4_CMD);
		if (y==1)
			LCD4_Send((0x80|0x40|x),LCD4_CMD);	
	}
	
	//создание нового символа
	void LCD_New()
	{
		LCD_SendCmd(0x40);				// символ 0
		LCD_SendData(0b11111111);		 
		LCD_SendData(0b10000011);		
		LCD_SendData(0b10000111);		
		LCD_SendData(0b10001111);		
		LCD_SendData(0b10000001);		
		LCD_SendData(0b10000001);		
		LCD_SendData(0b11111111);

		LCD_SendCmd(0x48);				// символ 1
		LCD_SendData(0b01111100);		 
		LCD_SendData(0b10000001);		
		LCD_SendData(0b10011001);		
		LCD_SendData(0b11111000);		
		LCD_SendData(0b10011001);		
		LCD_SendData(0b10000001);		
		LCD_SendData(0b01111100);

		LCD_SendCmd(0x50);				// символ 2
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
	
	
	/* Несколько слов о процессе инициализации ЖКИ-модуля. 
	Производитель контроллера рекомендует выполнять следующую 
	последовательность действий для инициализации. Выдержать 
	паузу не менее 15 мс между установлением рабочего напряжения 
	питания (> 4,5 В) и выполнением каких-либо операций с 
	контроллером. Первой операцией выполнить команду, выбирающую 
	разрядность шины (это должна быть команда $30 независимо 
	от того, какой разрядности интерфейс вы собираетесь 
	использовать в дальнейшем), причем перед выполнением этой 
	операции не проверять значение флага BF. Далее опять 
	выдержать паузу не менее 4,1 мс и повторить команду выбора 
	разрядности шины, причем перед подачей команды вновь не 
	производить проверку флага BF. Следующим шагом необходимо 
	вновь выдержать паузу, на этот раз 100 мкс, и в третий раз 
	повторить команду установления разрядности шины, вновь без 
	проверки BF. Эти три операции являются инициализирующими и 
	призваны вывести контроллер в исходный режим работы (то есть 
	перевести в режим работы с 8-ми разрядной шиной) из любого 
	состояния. Следом за ними нормальным порядком (без 
	выдерживания пауз, но с проверкой флага BF) выполняется 
	инициализация режимов работы с выдачей инициализирующей 
	последовательности, аналогичной указанной в таблице 7 
	(содержащей в том числе команду выбора необходимой 
	разрядности шины).	Необходимо помнить, что когда Вы 
	объявляете режим работы с 4-х разрядной шиной, то есть 
	выдаете команду $20, то делаете это обычно из 8-ми 
	разрядного режима, который устанавливается автоматически 
	после подачи напряжения питания, а значит вы не сможете 
	адекватно объявить необходимое значение флагов N и F, 
	располагающихся в младшей тетраде команды установки 
	разрядности шины. Поэтому команду необходимо повторить 
	в уже установившемся 4-х разрядном режиме путем 
	последовательной передачи двух тетрад, то есть для 
	4-х разрядного режима образом.
	
	D7 	D6 	D5 	D4 	D3 	D2 	D1 	D0 	Назначение							 
	0 	0 	0 	0 	0 	0 	0 	1 	Очистка экрана, АС = 0, адресация АС на DDRAM 
	0 	0 	0 	0 	0 	0 	1 	- 	АС = 0, адресация на DDRAM, сброшены сдвиги, начало строки адресуется в начале DDRAM 
	0 	0 	0 	0 	0 	1 	I/D S 	Выбирается направление сдвига курсора или экрана 
	0 	0 	0 	0 	1 	D 	C 	B 	Выбирается режим отображения 
	0 	0 	0 	1 	S/C R/L - 	- 	Команда сдвига курсора/экрана 
	0 	0 	1 	DL 	N 	F 	- 	- 	Определение параметров развертки и ширины шины данных 
	0 	1 	AG 	AG 	AG 	AG 	AG 	AG  Присвоение счетчику АС адреса в области CGRAM 
	1 	AD 	AD 	AD 	AD 	AD 	AD 	AD 	Присвоение счетчику АС адреса в области DDRAM 
	
	
	
	INIT_LCD				; Инициализируем 
	WR_CGADR 0				; Указатель на начало знакогенератора. 
	WR_DATA 0b00000001		; Запись данных нового знака
	WR_DATA 0b00000010
	WR_DATA 0b00000100
	WR_DATA 0b00011111
	WR_DATA 0b00000010
	WR_DATA 0b00000100
	WR_DATA 0b00001000 
	WR_DDADR 0				; Указатель на начало видео памяти (ячейка с координатми 0,0) 
	WR_DATA 0				; У новоиспеченного символа код 0 напечатем его


	
	
	
	
	
	
	
	
	
	
	
	
	*/



	































