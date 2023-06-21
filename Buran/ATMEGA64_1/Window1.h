

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


//----------------------------��������� ����-----------------------------------

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
            textx=6;texty=0;link ("  ����  ",8);
            textx=6;texty=1;link ("�������!",8);
        }
        
        if (Slave == 1)
		{
            textx=6;texty=0;link ("  ����  ",8);
            textx=6;texty=1;link ("�������1",8);
        }
        
        if (Slave == 2)
		{
            textx=6;texty=0;link ("  ����  ",8);
            textx=6;texty=1;link ("�������2",8);
        }
        
    /*    if (Slave == 3)
		{
            textx=6;texty=0;link ("  ����  ",8);
            textx=5;texty=2;link ("�������3",7);
        }   */
		
		textx=7;texty=3;link ("�����",5);
		
		textx=5;texty=5;link ("���������",9);
		
		if ( Weather_dependence )
		{
			if (Weather_dependence_Mode == 1 ) { textx=1;texty=7;link ("����� - ������.  ",17);}
			if (Weather_dependence_Mode == 2 ) { textx=1;texty=7;link ("����� - ��������.",17);}
			if (Weather_dependence_Mode == 3 ) { textx=1;texty=7;link ("����� - �����.   ",17);}

		}	
		
		if ( !Weather_dependence )
		{
			sprintf(Water_Load_Lcd, "%d", WaterLoad);
			textx=1;texty=7;link ("����� ������    �*",18);
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
            textx=6;texty=0;link ("  ����  ",8);
            textx=6;texty=1;link ("�������!",8);
        }
        
        if (Slave == 1)
		{
            textx=6;texty=0;link ("  ����  ",8);
            textx=6;texty=1;link ("�������1",8);
        }
        
        if (Slave == 2)
		{
            textx=6;texty=0;link ("  ����  ",8);
            textx=6;texty=1;link ("�������2",8);
        }
        
      
		textx=8;texty=3;link ("����",5);
        textx=1;texty=5;link ("���� ����� �������",18);
		
		if ( Weather_dependence )
		{
			if (Weather_dependence_Mode == 1 ) { textx=1;texty=7;link ("����� - ������.  ",17);}
			if (Weather_dependence_Mode == 2 ) { textx=1;texty=7;link ("����� - ��������.",17);}
			if (Weather_dependence_Mode == 3 ) { textx=1;texty=7;link ("����� - �����.   ",17);}

		}
        		
		if ( !Weather_dependence )
		{
			sprintf(Water_Load_Lcd, "%d", WaterLoad);
			textx=1;texty=7;link ("����� ������    �*",18);
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
            textx=6;texty=0;link ("  ����  ",8);
            textx=6;texty=1;link ("�������!",8);
        }
        
        if (Slave == 1)
		{
            textx=6;texty=0;link ("  ����  ",8);
            textx=6;texty=1;link ("�������1",7);
        }
        
        if (Slave == 2)
		{
            textx=6;texty=0;link ("  ����  ",8);
            textx=6;texty=1;link ("�������2",7);
        }
		
		textx=8;texty=3;link ("����",5);
        textx=1;texty=5;link ("���� ����� �������",18);
		
		if ( Weather_dependence )
		{
			if (Weather_dependence_Mode == 1 ) { textx=1;texty=7;link ("����� - ������.  ",17);}
			if (Weather_dependence_Mode == 2 ) { textx=1;texty=7;link ("����� - ��������.",17);}
			if (Weather_dependence_Mode == 3 ) { textx=1;texty=7;link ("����� - �����.   ",17);}

		}	
		
		if ( !Weather_dependence )
		{
			sprintf(Water_Load_Lcd, "%d", WaterLoad);
			textx=1;texty=7;link ("����� ������    �*",18);
			textx=14;texty=7;link (Water_Load_Lcd,2);
		}
		
		Cursor1 ();
		
		window = 4;	
	}
	
}






//#endif
