

//#ifndef MAIN_H
//#define MAIN_H


extern int window;
extern int cursor;
extern int UK_103;
extern int UK_203;
extern int UK_303;
extern int Master;
extern int Slave;
extern unsigned int WaterLoad;
extern unsigned int Weather_dependence;
extern unsigned int Weather_dependence_Mode; 
extern unsigned int Start_stop;
extern int Flag_Temp;
//extern int Lan;

char Water_Load_Lcd[2] = {}; 


void Cursor1 (void)
{
	textx=19;texty=5;link (" ",1);
	textx=0;texty=5;link (" ",1);
	textx=19;texty=3;link ("<",1);
	textx=0;texty=3;link (">",1);
	cursor = 1;
}

void Cursor2 (void)
{
	textx=19;texty=3;link (" ",1);
	textx=0;texty=3;link (" ",1);
	textx=19;texty=5;link ("<",1);
	textx=0;texty=5;link (">",1);
	cursor = 2;
}


//----------------------------Начальное окно-----------------------------------

void Window1 (void)
{
	clear();
	
	//---------------------------------------------------------------------------------------------------------------
	if (Start_stop == 0 )
	{
	
		if (UK_103 && !UK_203 && !UK_303) 
		{textx=6;texty=0;link (" UK-103 ",8);}
		if (!UK_103 && UK_203 && !UK_303)
		{textx=6;texty=0;link (" UK-203 ",8);}
		if (!UK_103 && !UK_203 && UK_303)
		{textx=6;texty=0;link (" UK-303 ",8);}
        
        if (Master)
		{
            textx=6;texty=0;link ("  Сеть  ",8);
            textx=6;texty=1;link ("Ведущий!",8);
        }
        
        if (Slave == 1)
		{
            textx=6;texty=0;link ("  Сеть  ",8);
            textx=6;texty=1;link ("Ведомый1",8);
        }
        
        if (Slave == 2)
		{
            textx=6;texty=0;link ("  Сеть  ",8);
            textx=6;texty=1;link ("Ведомый2",8);
        }
        
    /*    if (Slave == 3)
		{
            textx=6;texty=0;link ("  Сеть  ",8);
            textx=5;texty=2;link ("Ведомый3",7);
        }   */
		
		textx=7;texty=3;link ("СТАРТ",5);
		
		textx=5;texty=5;link ("Установки",9);
		
		if ( Weather_dependence )
		{
			if (Weather_dependence_Mode == 1 ) { textx=1;texty=7;link ("Режим - эконом.  ",17);}
			if (Weather_dependence_Mode == 2 ) { textx=1;texty=7;link ("Режим - стандарт.",17);}
			if (Weather_dependence_Mode == 3 ) { textx=1;texty=7;link ("Режим - жарко.   ",17);}

		}	
		
		if ( !Weather_dependence )
		{
			sprintf(Water_Load_Lcd, "%d", WaterLoad);
			textx=1;texty=7;link ("Задан нагрев    с*",18);
			textx=14;texty=7;link (Water_Load_Lcd,2);
		}
		
		Cursor1 ();		
		
		window = 1;
	}
	
	//---------------------------------------------------------------------------------------------------------------
	if (Start_stop == 1 && Flag_Temp)
	{
		if (UK_103 && !UK_203 && !UK_303) 
		{textx=6;texty=0;link (" UK-103 ",8);}
		if (!UK_103 && UK_203 && !UK_303)
		{textx=6;texty=0;link (" UK-203 ",8);}
		if (!UK_103 && !UK_203 && UK_303)
		{textx=6;texty=0;link (" UK-303 ",8);}
        
        if (Master)
		{
            textx=6;texty=0;link ("  Сеть  ",8);
            textx=6;texty=1;link ("Ведущий!",8);
        }
        
        if (Slave == 1)
		{
            textx=6;texty=0;link ("  Сеть  ",8);
            textx=6;texty=1;link ("Ведомый1",8);
        }
        
        if (Slave == 2)
		{
            textx=6;texty=0;link ("  Сеть  ",8);
            textx=6;texty=1;link ("Ведомый2",8);
        }
        
      
		textx=8;texty=3;link ("СТОП",5);
        textx=1;texty=5;link ("Идет набор теплоты",18);
		
		if ( Weather_dependence )
		{
			if (Weather_dependence_Mode == 1 ) { textx=1;texty=7;link ("Режим - эконом.  ",17);}
			if (Weather_dependence_Mode == 2 ) { textx=1;texty=7;link ("Режим - стандарт.",17);}
			if (Weather_dependence_Mode == 3 ) { textx=1;texty=7;link ("Режим - жарко.   ",17);}

		}
        		
		if ( !Weather_dependence )
		{
			sprintf(Water_Load_Lcd, "%d", WaterLoad);
			textx=1;texty=7;link ("Задан нагрев    с*",18);
			textx=14;texty=7;link (Water_Load_Lcd,2);
		}
        Cursor1 ();
		
		window = 4;	
	} 
    
    
    
        
        if (Start_stop == 1 && !Flag_Temp)
	{
		if (UK_103 && !UK_203 && !UK_303)
		{textx=6;texty=0;link (" UK-103 ",8);}
		if (!UK_103 && UK_203 && !UK_303)
		{textx=6;texty=0;link (" UK-203 ",8);}
		if (!UK_103 && !UK_203 && UK_303)
		{textx=6;texty=0;link (" UK-303 ",8);}
        
        if (Master)
		{
            textx=6;texty=0;link ("  Сеть  ",8);
            textx=6;texty=1;link ("Ведущий!",8);
        }
        
        if (Slave == 1)
		{
            textx=6;texty=0;link ("  Сеть  ",8);
            textx=6;texty=1;link ("Ведомый1",7);
        }
        
        if (Slave == 2)
		{
            textx=6;texty=0;link ("  Сеть  ",8);
            textx=6;texty=1;link ("Ведомый2",7);
        }
		
		textx=8;texty=3;link ("СТОП",5);
        textx=1;texty=5;link ("Идет сброс теплоты",18);
		
		if ( Weather_dependence )
		{
			if (Weather_dependence_Mode == 1 ) { textx=1;texty=7;link ("Режим - эконом.  ",17);}
			if (Weather_dependence_Mode == 2 ) { textx=1;texty=7;link ("Режим - стандарт.",17);}
			if (Weather_dependence_Mode == 3 ) { textx=1;texty=7;link ("Режим - жарко.   ",17);}

		}	
		
		if ( !Weather_dependence )
		{
			sprintf(Water_Load_Lcd, "%d", WaterLoad);
			textx=1;texty=7;link ("Задан нагрев    с*",18);
			textx=14;texty=7;link (Water_Load_Lcd,2);
		}
		
		Cursor1 ();
		
		window = 4;	
	}
	
}






//#endif
