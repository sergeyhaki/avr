	void pcf8574(unsigned char data) 
	{
		IIC_Start();
		IIC_Send(0b01000000);
		IIC_Send(data);
		IIC_Send(data);
		IIC_Stop();
	}

	int pcf8574Read()
	{
		int temp = 0;

		IIC_Start();
		IIC_Send(0b01000001);
		IIC_Read(ACK);
		temp = IIC_Read(NACK);
		IIC_Stop();

		return(temp);
	}
