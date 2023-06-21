/*******************************************************
This program was created by the CodeWizardAVR V3.40 
Automatic Program Generator
� Copyright 1998-2020 Pavel Haiduc, HP InfoTech S.R.L.
http://www.hpinfotech.ro

Project : buran_atmega64a
Version : 0.01
Date    : 09-May-2023
Author  : 
Company : 
Comments: 


Chip type               : ATmega64A
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 1024
*******************************************************/

#include "main.h"
#include "ADC.h"
#include "boiler.h"
#include "print.h"
#include "menu.h"
#include "uniteBoilerButtonMenu.h"
#include "widgets.h"
#include "input_output_signals.h"
#include "tick_time.h"

volatile uint32_t tick_ms = 0;  
uint32_t time_zero = 0;
uint8_t clapan_out_monitor;
#define CLAPAN_MONITOR_TIMEOUT (100) 


#define USART_PORT PORTB
#define USART_DDR DDRB
#define USART_TX_PIN (3)

// Define the baud rate and bit time in microseconds
#define BAUD_RATE 9600
#define BIT_TIME_US (8000000UL / BAUD_RATE)

void displayInit(GLCDINIT_t glcd_init_data){
  glcd_init_data.font=font5x7;
  glcd_init_data.readxmem=NULL;
  glcd_init_data.writexmem=NULL;
  glcd_init(&glcd_init_data);
}

void PORT_INIT(){
  DDRA = 0xff;
  PORTA = 0x00;
  DDRC = 0xdf;
  PORTC |= 0x11;
  
  DDRE = 0x00;
  PORTE = 0;
  
  DDRD = 0xff;
  PORTD = 0x00;
  
  DDRF = 0x00;
  PORTF = 0;//0b11011001;
  
  DDRB = 0b00000000;
  PORTB = 0b10000000;
  
  DDRG = 0b000000;
  PORTG = 0b00011000;
}

void timerInit(){
//adc  trig
  TCCR1B |= (1<<WGM21); 
  TIMSK |= (1<<OCIE1A); 
  OCR1A = 32768;      
  TCCR1B |= (1<<CS12);  
  
//systick 1ms
  ASSR=0<<AS0;
  TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (1<<CS01) | (1<<CS00);
  TCNT0=0x06;
  OCR0=0x00;
  TIMSK |= (1<<TOIE0);
}


void uartInit(){
    // USART0 initialization
  // Communication Parameters: 8 Data, 1 Stop, No Parity
  // USART0 Receiver: On
  // USART0 Transmitter: On
  // USART0 Mode: Asynchronous
  // USART0 Baud Rate: 9600
  UCSR0A=(0<<RXC0) | (0<<TXC0) | (0<<UDRE0) | (0<<FE0) | (0<<DOR0) | (0<<UPE0) | (0<<U2X0) | (0<<MPCM0);
  UCSR0B=(0<<RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
  UCSR0C=(0<<UMSEL0) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);
  UBRR0H=0x00;
  
  //9600
  UBRR0L=0x33;
  //56000
  UBRR0L=0x08; 
}

uint32_t GetTick_ms(void){
    return tick_ms;
}

interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{  
    // Reinitialize Timer 0 value
    TCNT0=0x06;                   
    tick_ms++;

    //PRINT("\r\nTIMER 1 ms\r\n");
    // Place your code here
    if(CLAPAN_OUT_PIN & CLAPAN_OUT_Pin){
      time_zero = tick_ms;
      clapan_out_monitor = 1;     
    }
    else{
        if(clapan_out_monitor){
            if(tick_ms - time_zero > CLAPAN_MONITOR_TIMEOUT){
              clapan_out_monitor = 0;      
            }  
        }
    }
}

interrupt[TIM1_COMPA] void timer1_compa_isr(void)
{  
  GetRef();
  SendAirTemp();
  SendWaterTemp();
  //GetDirect();   
}

void printBoilerInfo(void){
    //PRINT("===========================\r\n");
    //PRINT("INFO: ");
    /*
    PRINT("role=%s ", boiler_role_str[boiler.role]);            
    PRINT("vdd=%.1f ", boiler.vdd);
    PRINT("air_temp=%.2f ", boiler.air_temp);
    PRINT("air_temp_adc=%d ", boiler.air_temp_adc);
    PRINT("water_temp=%.2f ", boiler.water_temp); 
    PRINT("water_adc=%d ", boiler.water_temp_adc);   
    PRINT("gisteresis=%d ", boiler.gisteresis);
    PRINT("\r\n");
    PRINT("water_goal_temp=%d ", boiler.water_goal_temp);
    */
    PRINT("timeLastStage=%d ", boiler.time_of_last_stage);
    PRINT("timSinceLastStage=%d ", tick_ms - boiler.time_of_last_stage);
    //PRINT("weather=%s ", weather_dep_str[boiler.weather]);
    PRINT("state=%s ", boiler_state_str[boiler.state]);
    PRINT("start=%s ", start_sequence_stage_str[boiler.start_stage]);
    PRINT("cmd=%s ", boiler_state_str[boiler.command]); 
    PRINT("err=%s ", errors_str[boiler.error]);            
    PRINT("weather=%s ", weather_dep_str[boiler.weather]);
    //PRINT("\r\n");
    PRINT("URVN=%d ", Uroven_out);
    PRINT("Ion=%d ", Ion_out);
    PRINT("t_max=%d ", t_max_out);
    PRINT("plamya=%d ", plamya_out);
    PRINT("clapan=%d ", clapan_out());
    PRINT("TransRozg=%d ", isTransRozgiga_ON);
    PRINT("Ventilaytor=%d ", isVentilaytor_ON);
    PRINT("ToplNasos=%d ", isTopl_Nasos_ON);
    PRINT("SetNasos=%d ", isSetevoi_nasos_ON);
    PRINT("K-n1=%d ", isKlapan1_ON);
    PRINT("K-n2=%d ", isKlapan2_ON);    
    PRINT("TClap=%d ", tick_ms - time_zero);
    
    PRINT("\r\n"); 
}

extern float DirectVolt;
extern uint16_t DirectADC;
  
void main(void){
  GLCDINIT_t glcd_init_data; 

  PORT_INIT();
#if 0
    /* 
    TransRozgiga_ON;
     Ventilaytor_ON;
     Topl_Nasos_ON;
     Setevoi_nasos_ON;
      Klapan1_ON;
      */  
      while(1){
      Klapan1_ON;
      Klapan2_ON; 
      delay_ms(1000);
      Klapan1_OFF;
      Klapan2_OFF;
      delay_ms(1000);}
      while(1);
#endif  
  
  uartInit();
  //PRINT("uart ok\r\n");
  #asm("sei") 
  //PRINT("sei\r\n");
  ADC_Init();    
  //PRINT("adc init()\r\n") ;
  timerInit();
  
  delay_ms(200);
  displayInit(glcd_init_data); 
  delay_ms(200);
  glcd_clear();
  
  //uartInit();
        
  boiler_init();           
  menu_buttons_init();

  
  
#if (0)

  ADC_Init();
  printf("\r\n\r\n\r\n");
  sei();
  printf("vdd\t");
  printf("water_volt\t");
  printf("water_adc\t");
  printf("waterTemp\t");
  printf("water_R\t");
  printf("DirectADC\t");
  printf("DirectVolt\t");
  printf("\r\n");
  while (1){          
      printf("%.2f\t", boiler.vdd);
      printf("%.2f\t", boiler.water_voltage);
      printf("%d\t",   boiler.water_temp_adc);
      printf("%.2f\t", boiler.water_temp);
      printf("%.2f\t", boiler.water_resistance);
      printf("%d\t",   DirectADC);
      printf("%.2f\t", DirectVolt);
      printf("\r\n");
  	  delay_ms(20);  	
  }                                    
 
#endif
  
  
  while (1){        
    //PRINT(">>>ITERATION BEGIN\r\n"); 
    printBoilerInfo(); 
    menu_buttons_update(); 
    boiler_update();
    boiler_update_pump();    
    boiler_update_nvm();
    widgetsUpdate();
    
    //printBoilerInfo();
    //delay_ms(100);
   // PRINT("<<<ITERATION END\r\n\r\n\r\n");   */
  }
}
