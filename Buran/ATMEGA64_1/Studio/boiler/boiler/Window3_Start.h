
extern int window;
extern int cursor;
extern unsigned int WaterLoad;
extern unsigned int Slave_time;
extern int AirTemp;
extern unsigned int Weather_dependence;
extern unsigned int Weather_dependence_Mode; 
extern unsigned int Start_stop;
extern unsigned int Work;
extern unsigned int Time;

void Start (void)
{
	clear();
	
	window = 3;
	Start_stop = 1;	
	Work = 1;  //?
//	Time = 21;
	
	textx=13;texty=0;link ("<",1);
	textx=6;texty=0;link (">",1);
	textx=7;texty=0;link (" СТОП ",6);		//Переменная "Старт - Стоп"
	
	if (UK_103 && !UK_203 && !UK_303)
	{
    Time = 21;
	textx=0;texty=3;link ("Продувка",8);
	textx=0;texty=5;link ("Пламя",5);
	}
	
	if (!UK_103 && UK_203 && !UK_303)
	{
    Time = 21;
	textx=0;texty=3;link ("Продувка",8);
	textx=0;texty=5;link ("Клапан",6);
	textx=0;texty=7;link ("Пламя",5);
	}
    
    if ((!UK_103 && !UK_203 && UK_303) || Master )
	{
    Time = 3;
	textx=0;texty=3;link ("Запуск",6);
	}
    
    if ((Slave == 1 || Slave == 2) /* && ((Slave == 1 && Slave_time <=10) || (Slave == 2 && Slave_time <=2400)) */)
	{
    Slave_time = 0;
	textx=0;texty=3;link ("Ожидание запуска",16);
	}
	
	
}

void Reverse_time (void)
{
    if (Slave == 0 && Slave == 0)
	{
        char i[3] = {};
        sprintf(i, "%d", Time);
        textx=18; texty=3; link (i,3);
        if (Time == 9) {textx=19; texty=3; link (" ",1);}
    }    
}

void Check_box1 (void)
{
	WriteXY(45,5,LCD_CS2);
	WriteData(0b00011000,LCD_CS2);
	WriteData(0b00110000,LCD_CS2);
	WriteData(0b01100000,LCD_CS2);
	WriteData(0b11000000,LCD_CS2);
	WriteData(0b00110000,LCD_CS2);
	WriteData(0b00001100,LCD_CS2);
	WriteData(0b00000110,LCD_CS2);
	WriteData(0b00000011,LCD_CS2);
}

void Check_box2 (void)
{
	WriteXY(45,7,LCD_CS2);
	WriteData(0b00011000,LCD_CS2);
	WriteData(0b00110000,LCD_CS2);
	WriteData(0b01100000,LCD_CS2);
	WriteData(0b11000000,LCD_CS2);
	WriteData(0b00110000,LCD_CS2);
	WriteData(0b00001100,LCD_CS2);
	WriteData(0b00000110,LCD_CS2);
	WriteData(0b00000011,LCD_CS2);
}


















