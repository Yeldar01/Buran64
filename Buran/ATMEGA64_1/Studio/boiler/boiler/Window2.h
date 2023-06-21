


//#ifndef MAIN_H
//#define MAIN_H


extern int window;
extern int cursor;
extern unsigned int WaterLoad;
extern int AirTemp;
extern unsigned int Weather_dependence;
extern unsigned int Weather_dependence_Mode; 

char Water_Load[2] = {}; 


void cursor2_1 (void)
{
	textx=16;texty=4;link ("<",1);
	textx=3;texty=4;link (">",1);
	textx=3;texty=7;link (" ",1);
	textx=16;texty=7;link (" ",1);
	cursor = 1;
}


void cursor2_2 (void)
{
	textx=16;texty=4;link (" ",1);
	textx=3;texty=4;link (" ",1);
	textx=3;texty=7;link (">",1);
	textx=16;texty=7;link ("<",1);
	cursor = 2;
}


//----------------------------Окно установок-----------------------------------

void Window2_1 (void)
{
	clear();
	
	if ( !Weather_dependence )
	{
	    //clear();
		textx=0;texty=2;link ("Температура нагрева:",20);
		sprintf(Water_Load, "%d", WaterLoad);
		textx=9;texty=4;link (Water_Load,2);
		
        cursor2_1 ();
	}
	
	if ( Weather_dependence )
	{
	    //clear();
		textx=0;texty=2;link ("Температура нагрева:",20);
		if (Weather_dependence_Mode == 1 ) { textx=6;texty=4;link ("Эконом  ",8);}
		if (Weather_dependence_Mode == 2 ) { textx=6;texty=4;link ("Стандарт",8);}
		if (Weather_dependence_Mode == 3 ) { textx=6;texty=4;link ("Жарко   ",8);}
		
        cursor2_1 ();
	}	
	
	if (AirTemp < 80)
	{
		textx=0;texty=6;link ("Погодозависимость:",18);
		if ( !Weather_dependence ) { textx=6;texty=7;link ("Включить ",9);}
		if ( Weather_dependence ) { textx=6;texty=7;link ("Выключить",9);}
		
		//cursor2_2  ();
	}
	
	//cursor = 1;
	window = 2;
}


void Window2_1_WaterLoad (void)
{
	sprintf(Water_Load, "%d", WaterLoad);
	textx=9;texty=4;link (Water_Load,2);
}	






//#endif

