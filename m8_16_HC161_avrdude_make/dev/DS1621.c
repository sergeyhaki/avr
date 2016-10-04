
	int ds1621()
	{
		int temp = 0;

		_delay_ms(10);

		IIC_Start();
		IIC_Send(0x90);
		IIC_Send(0xac);
		IIC_Send(0x00);
		IIC_Stop();

		IIC_Start();
		IIC_Send(0x90);
		IIC_Send(0xee);
		IIC_Stop();

		_delay_ms(750);

		IIC_Start();
		IIC_Send(0x90);
		IIC_Send(0x22);
		IIC_Stop();

		IIC_Start();
		IIC_Send(0x90);
		IIC_Send(0xaa);
		IIC_Start();
		IIC_Send(0x91);
		temp = IIC_Read(ACK);
		IIC_Read(NACK);
		IIC_Stop();


		return(temp);
	}
