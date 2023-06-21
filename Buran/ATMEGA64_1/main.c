  
 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <util/delay.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <avr/pgmspace.h>
 #include <avr/eeprom.h>
 #include "ks0108.h"
 #include "ADC.h"
 #include "Window1.h"
 #include "Window2.h"
 #include "Window3_Start.h"
 #include "Errors_LCD.h"
 #include "Image.h"
 #include "Uart.h"
 #include "Window_Lan.h"
 //#include <iostream>
 


 
 
 #define LeftButton	  PINE&0b00000100	   // ����� ����� ������ ����������
 #define RightButton  PINE&0b00001000	   // ����� ������ ������ ����������
 #define TopButton	  PINE&0b00010000	   // ����� ������� ������ ����������
 #define BottomButton PINE&0b00100000	   // ����� ������ ������ ����������
 #define SelButton	  PINE&0b01000000	   // ����� Ok ������ ����������
 
 #define Uroven_out	  PINF&0b00001000	   // 
 #define Ion_out  	  PINF&0b00010000	   // 
 #define t_max_out	  PINF&0b00100000	   // 
 #define clapan_out   PINF&0b01000000	   // 
 #define plamya_out	  PINF&0b10000000	   // 
 
 #define TransRozgiga_ON	PORTD |= (1<<(PORTD2));	   
 #define TransRozgiga_OFF	PORTD &=~(1<<(PORTD2));
 #define Ventilaytor_ON	    PORTD |= (1<<(PORTD3));
 #define Ventilaytor_OFF	PORTD &=~(1<<(PORTD3));
 #define Topl_Nasos_ON	    PORTD |= (1<<(PORTD4));
 #define Topl_Nasos_OFF	    PORTD &=~(1<<(PORTD4));
 #define Setevoi_nasos_ON	PORTD |= (1<<(PORTD5));
 #define Setevoi_nasos_OFF	PORTD &=~(1<<(PORTD5));
 #define Klapan1_ON	        PORTD |= (1<<(PORTD6));
 #define Klapan1_OFF	    PORTD &=~(1<<(PORTD6));
 #define Klapan2_ON	        PORTD |= (1<<(PORTD7));
 #define Klapan2_OFF	    PORTD &=~(1<<(PORTD7));
 
 
 
 unsigned int WaterTemp;
 int AirTemp;
 unsigned int WaterLoad ;
 unsigned int Weather_dependence ;
 unsigned int Weather_dependence_Mode ; 
 unsigned int Start_stop ;
 unsigned int Work = 0;
 unsigned int Time = 21;
 unsigned int Slave_time = 0;
 int UK_103 = 0;
 int UK_203 = 0;
 int UK_303 = 0;
 int window = 1; // � ����� ���� �����
 int cursor = 1; // ����� ������ ���� �������
 int cursor_lan = 1;
 int Lan = 0;
 
 int Flag_E1 = 0;
 int Flag_Fire = 0;
 int Flag_Fire_Test = 0;
 int Flag_Clapan = 0;
 int Flag_Clapan_Test = 0;
 int Flag_Temp = 0;
 int Master = 0;
 int Slave = 0;
 
 int Start_stop_compare;
 int Start_stop_compare_tx;
 int WaterLoad_compare_tx;
 
uint8_t WaterLoad_eepr EEMEM;
uint8_t Start_stop_eepr EEMEM;
uint8_t Weather_dependence_eepr EEMEM;
uint8_t Weather_dependence_Mode_eepr EEMEM;
uint8_t Master_eepr EEMEM;
uint8_t Slave_eepr EEMEM;
//uint8_t Slave_time_eepr EEMEM;

//========================================================================
  char bufkey [7]="";
  char bufvalue [2];
  char Water_Load_tx[2];
//======================================================================== 

 

 ISR (TIMER1_COMPA_vect)
{
  //-----------------------����� ���. ������ ����-�-----------------------
  SendAirTemp ();
  SendWaterTemp ();


  //-----------------------������ ��������� �������-----------------------
	Time --;
    if (Time == 0) { Time = 23;} 
    if (Start_stop && Flag_Temp && Work) {Reverse_time();}
    
    Slave_time ++;
    if (Slave_time >= 3600) { Slave_time = 3600;} 
}

//------------------------------------------------------------------------ 

void WorkStop (void)
{
	TransRozgiga_OFF;
	Ventilaytor_OFF;
	Topl_Nasos_OFF;
	Setevoi_nasos_OFF;
	Klapan1_OFF;
	Klapan2_OFF;
	Flag_Fire = 0;
    Flag_Fire_Test = 0;
    Flag_Clapan = 0;
    Flag_Clapan_Test = 0;
    Flag_Temp = 0;
    Slave_time = 0;
}	

//-------------------------------------------------------------------------

                                                                                 


int main(void)
{

//----------------------������������� ������-------------------------------
	DDRE = 0b10000011;
	PORTE = 0b10000011;
	
	DDRD = 0xff;
	PORTD = 0x00;
	
	DDRF = 0x00;
	PORTF = 0b11111001;
//----------------------������������� LCD----------------------------------
	DDRC=0xFF;
	DDRA=0xFF;
	
	init_lcd();
	_delay_ms(10);
	clearInit();
	_delay_ms(50);
//---------------------������������� UK-103,203,303------------------------	
	
	DDRB = 0b00000000;				
	PORTB = 0b10000000;         	
	
	DDRG = 0b000000;				
	PORTG = 0b00011000;

//--------------------����� ��������� �������������� UK-103,203,303-------- 

	void ErrorInit (void)
    {
      textx=0;texty=0;link ("������!",7);
	  textx=0;texty=2;link ("������� ������",14);
	  textx=0;texty=3;link ("����� ������.",13);
	  while(1);
    }
    
	if (!(PINB&0b10000000) )   
	{
	    UK_103 = 1;	
	}
	if (!(PING&0b00001000) )  
	{
		UK_203 = 1;
	}
	if (!(PING&0b00010000) )  
	{
		UK_303 = 1; 
	}
	if (UK_103 && UK_203 && UK_303 == 0) {ErrorInit();} 
	if (UK_103 && UK_303 && UK_203 == 0) {ErrorInit();}
    if (UK_203 && UK_303 && UK_103 == 0) {ErrorInit();}



//------------------------------������ ������ �����������--------------------------------------------------------- 
 
	TCCR1B |= (1<<WGM21); // ������������� ����� ��� (����� �� ����������)
	TIMSK |= (1<<OCIE1A); //������������� ��� ���������� ���������� 1��� �������� �� ���������� � OCR1A(H � L)
	OCR1A = 32768;        //���������� � ������� ����� ��� ���������
	TCCR1B |= (1<<CS12);  //��������� ��������.
//----------------------------------------------------------------------------------------------------------------



//-------------------------------������������� ���������� �� EEPROM-----------------------------------------------

    
    WaterLoad = eeprom_read_byte (&WaterLoad_eepr);         //������
    if ( WaterLoad == 255 ) {WaterLoad = 50 ;}              // ��� ������� ���������
    
    Start_stop = eeprom_read_byte (&Start_stop_eepr);
    if ( Start_stop == 255 ) {Start_stop = 0 ;}             
    
    Weather_dependence = eeprom_read_byte (&Weather_dependence_eepr);
    if ( Weather_dependence == 255 ) {Weather_dependence = 0 ;}             
    
    Weather_dependence_Mode = eeprom_read_byte (&Weather_dependence_Mode_eepr);
    if ( Weather_dependence_Mode == 255 ) {Weather_dependence_Mode = 1 ;}   

    Master = eeprom_read_byte (&Master_eepr);
    if ( Master == 255 ) {Master = 0 ;}   
 
    Slave = eeprom_read_byte (&Slave_eepr);
    if ( Slave == 255 ) {Slave = 0 ;}
    
 //   Slave_time = eeprom_read_byte (&Slave_time_eepr);
 //   if ( Slave_time == 255 ) {Slave_time = 0 ;} 
    

//----------------------------------------------------------------------------------------------------------------
	ADC_Init();
    SUART_Init();
	sei(); 
    _delay_ms(1500);    // ��� ������� ���������� ����������� (������ 1�)
//	Window1 ();
//	Cursor1 ();
    if ((UK_103 && !UK_203 && !UK_303) || (!UK_103 && UK_203 && !UK_303) || (!UK_103 && !UK_203 && UK_303)) {Lan = 0; Window1(); Cursor1();}
    if (/*(UK_103 && UK_303 && UK_203) && */ Master == 1 || Slave == 1 || Slave == 2 ) {Lan = 1; Window1(); Cursor1();}
    if ((UK_103 && UK_303 && UK_203) && (Master == 0 && Slave == 0 )) {Lan = 1; WindowLan(); CursorLan1();}
    fun ();			// <---
//----------------------------------------------------------------------------------------------------------------

 
    
//cli();  //��������� ����������
	while(1)
 {
 
 //if (!Lan){
	
	//---------------------��������� ������ ��� ���������� ����-------------------------------
		
		if ( window == 1 && cursor == 2 && SelButton ) { Window2_1 (); while(SelButton){};}  // ������� � ����� ���������
		
		if (BottomButton && window == 1 ) { Cursor2 ();}
		
		if (TopButton && window == 1 ) { Cursor1 ();}
		
		
	//--------------------��������� ������ ��� ���� ���������--------------------------------	
		
		if ( window == 2 && SelButton && cursor == 1 ) { Window1 (); while(SelButton){};}
		
		//-----��������� ����������� �������------------------------------------
		
		if ( window == 2 && RightButton && cursor == 1 && Weather_dependence == 0)
		{
		  if (WaterLoad < 85) {WaterLoad ++; eeprom_write_byte(&WaterLoad_eepr, WaterLoad); Window2_1_WaterLoad (); _delay_ms(300);}
		} 
			
		if ( window == 2 && LeftButton &&cursor == 1 && Weather_dependence == 0)
		{
		  if (WaterLoad > 25) {WaterLoad --; eeprom_write_byte(&WaterLoad_eepr, WaterLoad); Window2_1_WaterLoad (); _delay_ms(300);}
		} 
		
		//-----������ � ����, � ���--------------------------------------------
		
		if (AirTemp < 80 && window == 2 && cursor == 1 && BottomButton)
	    {
			cursor2_2 ();
		}
		if (AirTemp < 80 && window == 2 && cursor == 2 && TopButton)
	    {
			cursor2_1 ();
		}
		
		//-----��� . ���� �����������������-------------------------------------
		if (AirTemp < 80 && window == 2 && cursor == 2 && Weather_dependence == 0 && SelButton) { Weather_dependence = 1; eeprom_write_byte(&Weather_dependence_eepr, 1); textx=6;texty=7;link ("���������",9); cursor2_1 (); Window2_1 (); while(SelButton){};}
		if (AirTemp < 80 && window == 2 && cursor == 2 && Weather_dependence == 1 && SelButton) { Weather_dependence = 0; eeprom_write_byte(&Weather_dependence_eepr, 0); textx=6;texty=7;link ("�������� ",9); cursor2_1 (); Window2_1 (); while(SelButton){};}
		
		//-----����� ������ �����������������-----------------------------------
		
		if ( window == 2 && cursor == 1 && Weather_dependence == 1 && Weather_dependence_Mode == 1 && RightButton ) { Weather_dependence_Mode = 2; eeprom_write_byte(&Weather_dependence_Mode_eepr, 2); Window2_1(); while(RightButton){};}
		if ( window == 2 && cursor == 1 && Weather_dependence == 1 && Weather_dependence_Mode == 2 && RightButton ) { Weather_dependence_Mode = 3; eeprom_write_byte(&Weather_dependence_Mode_eepr, 3); Window2_1(); while(RightButton){};}
		if ( window == 2 && cursor == 1 && Weather_dependence == 1 && Weather_dependence_Mode == 3 && LeftButton ) { Weather_dependence_Mode = 2; eeprom_write_byte(&Weather_dependence_Mode_eepr, 2); Window2_1(); while(LeftButton){};}
		if ( window == 2 && cursor == 1 && Weather_dependence == 1 && Weather_dependence_Mode == 2 && LeftButton ) { Weather_dependence_Mode = 1; eeprom_write_byte(&Weather_dependence_Mode_eepr, 1); Window2_1(); while(LeftButton){};}
		
	//--------------------��������� ������� ������ SelButton-------------------------------
		
		if ( window == 1 && cursor == 1 && Start_stop == 0 && Flag_Temp == 1 && SelButton ) { Start (); while(SelButton){};}								 //������ �� ��������� ��������
        if ( window == 1 && cursor == 1 && Start_stop == 0 && Flag_Temp == 0 && SelButton ) {Start_stop = 1; Window1(); while(SelButton){};}			     //������ �� ��������� �������� � ������ ����
		if ( window == 1 && cursor == 1 && Start_stop == 1 && SelButton ) { Start_stop = 0; /*WorkStop ();*/ Window1 (); WorkStop(); while(SelButton){};}        //��������� �� ��������� ��������
		if ( (window == 3 || window == 4) && Start_stop == 1 && SelButton ) { Start_stop = 0; Window1 (); WorkStop(); while(SelButton){};}				     //��������� � �������� ��������
        
        
   
		
	//----------------��������� Start_stop ��������� � ������ ��� ��������� � EEPROM--------
        
        if ( Start_stop != Start_stop_compare ) { Start_stop_compare = Start_stop; eeprom_write_byte(&Start_stop_eepr, Start_stop);}


	
//--------------------�������� ����---------------------------�������� ����-----------------------------�������� ����--------------------------------------------------------------

	//--��������� ����������������� ��� ��������� ������� ����������� (����� , ��)----------
	
	  if ( Weather_dependence && AirTemp > 80 && Start_stop) { Weather_dependence = 0; eeprom_write_byte(&Weather_dependence_eepr, 0);  Window1 (); }
	
	//----------------------------------��������� ������ � ��������� � �������� ����--------
	  if (!(Uroven_out) && Start_stop )
	  {
		WorkStop ();
		E1();
		Flag_E1 = 1;
		Work = 0;
		do
		{if ( window == 3 && cursor == 1 && Start_stop == 1 && SelButton ) { Start_stop = 0; Work = 0; WorkStop (); Window1(); while(SelButton){};}} // ����� �� ���.����� ������� �� �������� �1
		while(!(Uroven_out) && Start_stop);
	  } 
	  else if ( Flag_E1 && Uroven_out && Start_stop )
	  { 
		Flag_E1 = 0;
		Work = 1;    //?
		Window1 ();
	  }	
	  
	//----------------------------------��������� ��������� ����--------------------------
	
	  if (!(t_max_out) && Start_stop  )
	  {
		WorkStop ();
		E2();
		Work = 0;
	    while(1){};
	  }
	  
	//----------------------------------��������� ������� �������--------------------------
	
	  if ((UK_103 && !UK_203 && !UK_303) && Start_stop  && plamya_out && Flag_Fire_Test)
	  {
		WorkStop ();
		E3();
		Work = 0;
        eeprom_write_byte(&Start_stop_eepr, 0);
	    while(1){};
	  }
	  if ((UK_103 && !UK_203 && !UK_303) && Start_stop  && (!(plamya_out)) && Flag_Fire)
	  {
		WorkStop ();
		E4();
		Work = 0;
        eeprom_write_byte(&Start_stop_eepr, 0);
	    while(1){};
	  }	 
	 
	//----------------------------------��������� ������� ���������------------------------
	
	  if ((!UK_103 && UK_203 && !UK_303) && Start_stop  && Ion_out && Flag_Fire_Test)
	  {
		WorkStop ();
		E5();
		Work = 0;
        eeprom_write_byte(&Start_stop_eepr, 0);
	    while(1){};
	  }
	  if ((!UK_103 && UK_203 && !UK_303) && Start_stop  && (!(Ion_out)) && Flag_Fire)
	  {
		WorkStop ();
		E6();
		Work = 0;
        eeprom_write_byte(&Start_stop_eepr, 0);
	    while(1){};
	  }	  
	  
	//----------------------------------��������� ������� ��������-------------------------
	
	  if ((!UK_103 && UK_203 && !UK_303) && Start_stop  && clapan_out && Flag_Clapan_Test)
	  {
		WorkStop ();
		E7();
		Work = 0;
        eeprom_write_byte(&Start_stop_eepr, 0);
	    while(1){};
	  }
	  if ((!UK_103 && UK_203 && !UK_303) && Start_stop  && (!(clapan_out)) && Flag_Clapan)
	  {
		WorkStop ();
		E8();
		Work = 0;
        eeprom_write_byte(&Start_stop_eepr, 0);
	    while(1){};
	  }	 
      
      //----------------------------------UK-303 ������ �������---------------------------
	
	  if (((!UK_103 && !UK_203 && UK_303) || Lan) && Start_stop  && clapan_out && Flag_Clapan_Test)
	  {
		WorkStop ();
		E10();
		Work = 0;
        eeprom_write_byte(&Start_stop_eepr, 0); //---------------------------------------------------------------------------------------------------------------------
	    while(1){};
	  }
	  if (((!UK_103 && !UK_203 && UK_303) || Lan) && Start_stop  && (!(clapan_out)) && Flag_Clapan)
	  {
		WorkStop ();
		E11();
		Work = 0;
        eeprom_write_byte(&Start_stop_eepr, 0); //---------------------------------------------------------------------------------------------------------------------
	    while(1){};
	  }
	  
	//----------------------------------��������� ����������� ����-------------------------
    //--------(����� , ��)----------
      if ((WaterTemp < 0 || WaterTemp > 99) && Start_stop) {WorkStop(); E9(); eeprom_write_byte(&Start_stop_eepr, 0); Work = 0; while(1){};}
      
    //------------------------------  
	  if (!Weather_dependence)
      {
	    if ( WaterTemp <= (WaterLoad - 10) && (!(Flag_Temp)) ) { Flag_Temp = 1; Work = 1; Slave_time = 0;}
	    if ( WaterTemp >= WaterLoad && Flag_Temp) { Flag_Temp = 0; WorkStop(); Window1();}
      }
        //-----------------------������ �������� ������-------------
	    if ( WaterTemp >= (WaterLoad / 1.1) ) { Setevoi_nasos_ON; }
	    if ( WaterTemp <= (WaterLoad / 1.2) ) { Setevoi_nasos_OFF;}
    
      
    //---------------------------�����������������-----------------------------------------
      if (Weather_dependence && Weather_dependence_Mode == 1) // ������
      {
        int On = ((AirTemp-15)*-2);
        int Off = ((AirTemp-15)*-2+10);
        
        if (Off > 85) {Off = 85;}
        if (Off < 14) {Off = 15;}
        if (On > 74) {On = 75;}
        if (On < 25) {On = 25;}
        
        if ( WaterTemp <= On && (!(Flag_Temp)) ) { Flag_Temp = 1; Work = 1;}
        if ( WaterTemp >= Off && Flag_Temp) { Flag_Temp = 0; WorkStop(); Window1();}
        
        if ( WaterTemp >= (Off / 1.1) ) { Setevoi_nasos_ON; }
	    if ( WaterTemp <= (Off / 1.2) ) { Setevoi_nasos_OFF;}
      }
      
      if (Weather_dependence && Weather_dependence_Mode == 2) // ��������
      {
        int On = ((AirTemp-15)*-2+15);
        int Off = ((AirTemp-15)*-2+25);
        
        if (Off > 85) {Off = 85;}
        if (Off < 14) {Off = 15;}
        if (On > 74) {On = 75;}
        if (On < 25) {On = 25;}
        
        if ( WaterTemp <= On && (!(Flag_Temp)) ) { Flag_Temp = 1; Work = 1;}
        if ( WaterTemp >= Off && Flag_Temp) { Flag_Temp = 0; WorkStop(); Window1();}
        
        if ( WaterTemp >= (Off / 1.1) ) { Setevoi_nasos_ON; }
	    if ( WaterTemp <= (Off / 1.2) ) { Setevoi_nasos_OFF;}
      }
      
      if (Weather_dependence && Weather_dependence_Mode == 3)
      {
        int On = ((AirTemp-15)*-2+20);
        int Off = ((AirTemp-15)*-2+30);
        
        if (Off > 85) {Off = 85;}
        if (Off < 14) {Off = 15;}
        if (On > 74) {On = 75;}
        if (On < 25) {On = 25;}
        
        if ( WaterTemp <= On && (!(Flag_Temp)) ) { Flag_Temp = 1; Work = 1;}
        if ( WaterTemp >= Off && Flag_Temp) { Flag_Temp = 0; WorkStop(); Window1();}
        
        if ( WaterTemp >= (Off / 1.1) ) { Setevoi_nasos_ON; }
	    if ( WaterTemp <= (Off / 1.2) ) { Setevoi_nasos_OFF;}
      }
	  
  
//--------------------����� ������� ������������----------------------------------------------------------------------------------------	  

	   if ( (UK_103 && !UK_203 && !UK_303) && Start_stop && Flag_Temp && Work )
	   {
	     int a = 1;
		 
		 Ventilaytor_ON;
		 
		 if (window != 3) {Start();}
		 if (Time == 10) {Flag_Fire_Test = 1;}
		 if (Time == 8 && a) {Flag_Fire_Test = 0; a = 0; _delay_ms(50); Check_box1();}
		 if (Time == 4) {Topl_Nasos_ON; TransRozgiga_ON; }
		 if (Time == 1) {TransRozgiga_OFF; Flag_Fire = 1; Work = 0; Window1();}
		 
	   }
       
       if ( (!UK_103 && UK_203 && !UK_303) && Start_stop && Flag_Temp && Work )
	   {
	     int a = 1;
	 
		 if (window != 3) {Start();}
         if (Time == 20) {Flag_Clapan_Test = 1;}
         if (Time == 19) {Flag_Clapan_Test = 0; Ventilaytor_ON; _delay_ms(50); Check_box1();}
		 if (Time == 10) {Flag_Fire_Test = 1;}
		 if (Time == 8 && a) {Flag_Fire_Test = 0; a = 0; _delay_ms(50); Check_box2();}
		 if (Time == 4) {Klapan1_ON; TransRozgiga_ON; }
		 if (Time == 1) {TransRozgiga_OFF; Klapan2_ON; Flag_Fire = 1; Work = 0; Window1();}
		 
	   }
       
       if ( ((!UK_103 && !UK_203 && UK_303) || Master) && Start_stop && Flag_Temp && Work )
	   {
         if (window != 3) {Start();}
         if (Time == 3) {Flag_Clapan_Test = 1;}
         if (Time == 2) {Flag_Clapan_Test = 0;Klapan1_ON;}
         if (Time == 1) {Flag_Clapan = 1; Work = 0; Window1();}
       }
       
       if ((Slave == 1 && Slave_time >= 900) && Start_stop && Flag_Temp && Work )
	   {
         if (window != 3) {Start();}
         if (Time == 3) {Flag_Clapan_Test = 1;}
         if (Time == 2) {Flag_Clapan_Test = 0;Klapan1_ON;}
         if (Time == 1) {Flag_Clapan = 1; Work = 0; Window1();}
       }
       
       if ((Slave == 2 && Slave_time >= 2400) && Start_stop && Flag_Temp && Work )
	   {
         if (window != 3) {Start();}
         if (Time == 3) {Flag_Clapan_Test = 1;}
         if (Time == 2) {Flag_Clapan_Test = 0;Klapan1_ON;}
         if (Time == 1) {Flag_Clapan = 1; Work = 0; Window1();}
       }



       
//=============================================================Uart RX=====================================================================    
if (!Lan)
  {	
    if (qin==7)
	{
      uint8_t i;
      int Num;
        
      for(i = 0; i < 7 ; i++) {bufkey [i] = inbuf[i];}   
      
      //----------------------------------Start_stop-----------------------------------------
      
        if ( window == 1 && cursor == 1 && Start_stop == 0 && Flag_Temp == 1 && (strncmp  (bufkey, "On_Off1", 7)) == 0 ) { Start ();}						         //������ �� ��������� ��������
        if ( window == 1 && cursor == 1 && Start_stop == 0 && Flag_Temp == 0 && (strncmp  (bufkey, "On_Off1", 7)) == 0 ) {Start_stop = 1; Window1();}		         //������ �� ��������� �������� � ������ ����
        if ( window == 1 && cursor == 1 && Start_stop == 1 && (strncmp  (bufkey, "On_Off0", 7)) == 0 ) { Start_stop = 0; /*WorkStop();*/ Window1 (); WorkStop();}       //��������� �� ��������� ��������
		if ( (window == 3 || window == 4) && Start_stop == 1 && (strncmp  (bufkey, "On_Off0", 7)) == 0 ) { Start_stop = 0; Window1 (); WorkStop();}				     //��������� � �������� ��������
        
      //----------------------------------WaterLoad------------------------------------------
                     
        if ((strncmp  (bufkey, "WatLd", 5)) == 0) 
        { 
          for(i = 5; i < 7 ; i++) {bufvalue [i-5] = inbuf[i];}
          sscanf(bufvalue, "%d", &Num);
          WaterLoad = Num;
          eeprom_write_byte(&WaterLoad_eepr, WaterLoad); Window1();
        }

        SUART_FlushInBuf();
     }
     
//=============================================================Uart RX=====================================================================  

     if ( Start_stop != Start_stop_compare_tx ) 
     { 
        Start_stop_compare_tx = Start_stop; 
        if ( Start_stop) {SUART_PutStr("On_Off1");}
        if ( Start_stop == 0) {SUART_PutStr("On_Off0");}
     }
     if ( WaterLoad != WaterLoad_compare_tx ) 
     { 
        WaterLoad_compare_tx = WaterLoad;
        sprintf(Water_Load_tx, "%d", WaterLoad);
        char dst[8]="WatLd";
        strcat (dst, Water_Load_tx);
        SUART_PutStr (dst);
     }
  }

//=============================================================Lan ========================================================================   

    if (BottomButton && window == 5 && cursor_lan == 1 ) { CursorLan2(); while(BottomButton);}
    if (BottomButton && window == 5 && cursor_lan == 2 ) { CursorLan3(); while(BottomButton);}
 //   if (BottomButton && window == 5 && cursor_lan == 3 ) { CursorLan4(); while(BottomButton);}
 //   if (TopButton && window == 5 && cursor_lan == 4 ) { CursorLan3(); while(TopButton);}
    if (TopButton && window == 5 && cursor_lan == 3 ) { CursorLan2(); while(TopButton);}
    if (TopButton && window == 5 && cursor_lan == 2 ) { CursorLan1(); while(TopButton);}
 
    if (SelButton && window == 5 && cursor_lan == 1 && Master == 0) {eeprom_write_byte(&Master_eepr, 1); Master = 1; Start_stop = 1; Work = 0; Window1(); Cursor1();}
    
    if (SelButton && window == 5 && cursor_lan == 2 ) { eeprom_write_byte(&Slave_eepr, 1); Slave = 1; Start_stop = 1; Work = 0; Window1(); Cursor1();}
    if (SelButton && window == 5 && cursor_lan == 3 ) { eeprom_write_byte(&Slave_eepr, 2); Slave = 2; Start_stop = 1; Work = 0; Window1(); Cursor1();}
  //  if (SelButton && window == 5 && cursor_lan == 4 ) { Slave = 3; eeprom_write_byte(&Slave_eepr, 3); Start_stop = 1; Work = 0; Window1(); Cursor1(); while(SelButton);}
    
  
 
 }
 

 
 
}


















