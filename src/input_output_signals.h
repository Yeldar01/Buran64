#ifndef INPUT_OUTPUT_SIGNALS_H
#define INPUT_OUTPUT_SIGNALS_H

#include <mega64a.h>

//#define USE_220 //for air relay on uk303

#define CLAPAN_OUT_Pin (1<<6)
#define CLAPAN_OUT_PIN (PINF)
#define Uroven_out (!(PINF & 0b00001000))   
#define Ion_out    (!(PINF & 0b00010000))    
#define t_max_out  (!(PINF & 0b00100000))     
#define plamya_out (!!(PINF & 0b10000000)) 
  
#ifdef USE_220
extern uint8_t clapan_out_monitor;
#define clapan_out()  clapan_out_monitor
#else
#define clapan_out() (CLAPAN_OUT_PIN & CLAPAN_OUT_Pin)     //
#endif
#define clapan_timeout  (30)

#define isTransRozgiga_ON   (!!(PORTD & (1 << (PORTD2))))
#define isVentilaytor_ON    (!!(PORTD & (1 << (PORTD3))))
#define isTopl_Nasos_ON     (!!(PORTD & (1 << (PORTD4))))
#define isSetevoi_nasos_ON  (!!(PORTD & (1 << (PORTD5))))
#define isKlapan1_ON        (!!(PORTD & (1 << (PORTD6))))
#define isKlapan2_ON        (!!(PORTD & (1 << (PORTD7))))

#define TransRozgiga_ON     PORTD |= (1 << (PORTD2))
#define TransRozgiga_OFF    PORTD &= ~(1 << (PORTD2))
#define Ventilaytor_ON      PORTD |= (1 << (PORTD3))
#define Ventilaytor_OFF     PORTD &= ~(1 << (PORTD3))
#define Topl_Nasos_ON       PORTD |= (1 << (PORTD4))
#define Topl_Nasos_OFF      PORTD &= ~(1 << (PORTD4))
#define Setevoi_nasos_ON    PORTD |= (1 << (PORTD5))
#define Setevoi_nasos_OFF   PORTD &= ~(1 << (PORTD5))
#define Klapan1_ON          PORTD |= (1 << (PORTD6))
#define Klapan1_OFF         PORTD &= ~(1 << (PORTD6))
#define Klapan2_ON          PORTD |= (1 << (PORTD7))
#define Klapan2_OFF         PORTD &= ~(1 << (PORTD7))


#endif