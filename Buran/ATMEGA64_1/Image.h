


void fun (void)
{
	WriteXY(1,0, LCD_CS1 );
	 //_delay_ms(1);
	WriteData(0b00001000,LCD_CS1);
	WriteData(0b00101010,LCD_CS1);
	WriteData(0b00011100,LCD_CS1);
	WriteData(0b01111111,LCD_CS1);
	WriteData(0b00011100,LCD_CS1);	
	WriteData(0b00101010,LCD_CS1);
	WriteData(0b00001000,LCD_CS1);
	
	WriteXY(40,0,LCD_CS2);
	WriteData(0b01110001,LCD_CS2);
	WriteData(0b10001110,LCD_CS2);
	WriteData(0b00000000,LCD_CS2);
	WriteData(0b01110001,LCD_CS2);
	WriteData(0b10001110,LCD_CS2);
	WriteData(0b00000000,LCD_CS2);
	 
}	

	
	
	
