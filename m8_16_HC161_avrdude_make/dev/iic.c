//���� iic 

#define	IIC_PORT		PORTB	//��������! B IIC_PORT ������ ����
#define	IIC_DDR			DDRB
#define	IIC_PIN			PINB
#define	SDA				PB0		//������
#define	SCL				PB1		//�������������
#define IIC_DELAY_A		100		//us ��������
#define ACK				1		//������� ������������
#define NACK			0		//������� ��������������

	inline void IIC_Init()
	{	
		IIC_PORT &= ~(1<<SCL)|(1<<SDA);
		IIC_DDR &= ~(1<<SCL)|(1<<SDA);
	}//end IIC_Init

	void IIC_Start(void)
	{//������� Srart		 		
		//��� ��������
		//������������� SDA
		IIC_DDR &= ~(1<<SDA);
		_delay_us(IIC_DELAY_A);
		//������������� SCL				
		IIC_DDR &= ~(1<<SCL);		
		_delay_us(IIC_DELAY_A);
		//������� SDA
		IIC_DDR |= (1<<SDA);
		//����� �
		_delay_us(IIC_DELAY_A);
		//������� SCL
		IIC_DDR |= (1<<SCL);
	}//end IIC_Start

	int IIC_Send(unsigned char data)
	{
		unsigned char counter = 8, ack = 0;
		//���� �������� ��������
		while(counter--)
		{
			_delay_us(IIC_DELAY_A);
			if(data & 0x80)
				//������������� SDA
				IIC_DDR &= ~(1<<SDA);
			else
				//������� SDA			
				IIC_DDR |= (1<<SDA);
			_delay_us(IIC_DELAY_A);
			//������������� SCL				
			IIC_DDR &= ~(1<<SCL);
			while (!(IIC_PIN & (1<<SCL)));			
			/*	��� ���������� SCL ���� ������� �� ���, 
				��� ����� ���������. ���� �� ���������, 
				�� ���� ������������ � ����� �� ��� ���, 
				���� Slave �� �� ��������. ����� ���������� 
				� ���� �� �����.	*/
			_delay_us(IIC_DELAY_A);
			//������� SCL
			IIC_DDR |= (1<<SCL);
			_delay_us(IIC_DELAY_A);
			//������� SDA
			IIC_DDR |= (1<<SDA);
			//����� �����
			data <<= 1;							
		}//end while
		_delay_us(IIC_DELAY_A);
		//������������� SDA
		IIC_DDR &= ~(1<<SDA);
		_delay_us(IIC_DELAY_A);
		//������������� SCL				
		IIC_DDR &= ~(1<<SCL);
		_delay_us(IIC_DELAY_A);
		while (!(IIC_PIN & (1<<SCL)));
		//��������� ������ ������������� ACK
		if (IIC_PIN & (1<<SDA))
			ack = NACK;
		else 
		{
			ack = ACK;
			//������� SDA
			IIC_DDR |= (1<<SDA);
		}//end else
		//������� SCL
		IIC_DDR |= (1<<SCL);
		_delay_us(IIC_DELAY_A);
		return(ack);		
	}//end IIC_Send

	int IIC_Read(unsigned char IsAck)	
	{
		unsigned char data = 0,counter = 8;
		//������������� SDA
		IIC_DDR &= ~(1<<SDA);
		//���� �������� ��������
		while(counter--)		
		{			
			//������� SCL
			IIC_DDR |= (1<<SCL);					
			_delay_us(IIC_DELAY_A);
			//������������� SCL				
			IIC_DDR &= ~(1<<SCL);			
			while (!(IIC_PIN & (1<<SCL)));
			/*	��� ���������� SCL ���� ������� �� ���, 
				��� ����� ���������. ���� �� ���������, 
				�� ���� ������������ � ����� �� ��� ���, 
				���� Slave �� �� ��������. ����� ���������� 
				� ���� �� �����.	*/
			//����� ������
			data <<= 1;
			//������ SDA
			if(IIC_PIN & (1<<SDA))
				//������������� ���
				data |= (1<<0);
			else
				//���������� ���
				data &= ~(1<<0);
			_delay_us(IIC_DELAY_A);			
			//������� SCL
			IIC_DDR |= (1<<SCL);
			_delay_us(IIC_DELAY_A);							
		}//end while
		//��������� ������ ������������� ACK
		_delay_us(IIC_DELAY_A);
		_delay_us(IIC_DELAY_A);
		if (IsAck == 1)
			{//�������� ACK				
				//������� SDA
				IIC_DDR |= (1<<SDA);
				_delay_us(IIC_DELAY_A);
				//������������� SCL				
				IIC_DDR &= ~(1<<SCL);	
				while (!(IIC_PIN & (1<<SCL)));
				_delay_us(IIC_DELAY_A);
				//������� SCL
				IIC_DDR |= (1<<SCL);
				_delay_us(IIC_DELAY_A);
				//������������� SDA
				IIC_DDR &= ~(1<<SDA);
			}//end if
		else
			{//�������� NACK
				_delay_us(IIC_DELAY_A);
				//������������� SCL				
				IIC_DDR &= ~(1<<SCL);	
				while (!(IIC_PIN & (1<<SCL)));
				_delay_us(IIC_DELAY_A);
				//������� SCL
				IIC_DDR |= (1<<SCL);
				_delay_us(IIC_DELAY_A);
			}//end else
		return(data);
	}//end IIC_Read

	void IIC_Stop(void)
	{
		_delay_us(IIC_DELAY_A);
		//������� SDA
		IIC_DDR |= (1<<SDA);
		//������� SCL
		IIC_DDR |= (1<<SCL);
		//����� �
		_delay_us(IIC_DELAY_A);
		//������������� SCL				
		IIC_DDR &= ~(1<<SCL);
		while (!(IIC_PIN & (1<<SCL)));
		//����� �
		_delay_us(IIC_DELAY_A);
		//������������� SDA
		IIC_DDR &= ~(1<<SDA);
	}//end IIC_Stop









