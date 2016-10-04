	/*The PCF8563 is a CMOS real-time clock/calendar

		; PCF8563 pins
		; 1 - osc
		; 2 - osc
		; 3 - int
		; 4 - GRN
		; 5 - SDA
		; 6 - SCL
		; 7 - ckout
		; 8 - +5V

		; ВНИМАНИЕ!!! Програмная задержка не менее 100мс

		PCF8563 slave address: read A3H; write A2H

		Set:
		S SLAVE_ADDR_W SUB_ADDR DATA...DATA P

		[Read]:
		S SLAVE_ADDR_W SUB_ADDR S SLAVE_ADDR_R [DATA(ACK)...DATA(NACK)] P

		registr				SUB_ADDR		bits
		CONTROL/STATUS_1	0
		CONTROL/STATUS_2	1
		SECONDS/VL 			2			6 to 0 <seconds> Example: 101 1001, represents the value 59 s		
		MINUTES 			3			6 to 0 <minutes>
		HOURS 				4			5 to 0 <hours>
		DAYS				5			5 to 0 <days>
		WEEKDAYS 			6			2 to 0 <weekdays>
		MONTHS/CENTURY 		7			4 to 0 <months>
		YEARS 				8			7 to 0 <years>

	*/

	void pcf8563Write(void)
	{
		unsigned char i = 0;
		//
		IIC_Start();
		IIC_Send(0xa2);						//SLAVE_ADDR_W
		IIC_Send(0x02);						//SUB_ADDR
		IIC_Send(CommonArray [i++]);		//6 to 0 <seconds>
		IIC_Send(CommonArray [i++]);		//6 to 0 <minutes>
		IIC_Send(CommonArray [i++]);		//5 to 0 <hours>
		IIC_Send(CommonArray [i++]);		//5 to 0 <days>
		IIC_Send(CommonArray [i++]);		//2 to 0 <weekdays>
		IIC_Send(CommonArray [i++]);		//4 to 0 <months>
		IIC_Send(CommonArray [i++]);		//7 to 0 <years>
		IIC_Stop();
	}

	void pcf8563Read(void)
	{
		unsigned char i = 0;
		//
		IIC_Start();
		IIC_Send(0xa2);						//SLAVE_ADDR_W
		IIC_Send(0x02);						//SUB_ADDR
		IIC_Start();
		IIC_Send(0xa3);						//SLAVE_ADDR_R
		CommonArray [i++] = IIC_Read(ACK);	//6 to 0 <seconds>
		CommonArray [i++] = IIC_Read(ACK);	//6 to 0 <minutes>
		CommonArray [i++] = IIC_Read(ACK);	//5 to 0 <hours>
		CommonArray [i++] = IIC_Read(ACK);	//5 to 0 <days>
		CommonArray [i++] = IIC_Read(ACK);	//2 to 0 <weekdays>
		CommonArray [i++] = IIC_Read(ACK);	//4 to 0 <months>
		CommonArray [i++] = IIC_Read(NACK);	//7 to 0 <years>
		IIC_Stop();
	}
