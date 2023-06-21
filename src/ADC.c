//#include <util/delay.h>
#include <io.h>
#include <interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ADC.h"
#include <stdint.h>
#include <math.h>
#include "main.h"
#include "boiler.h"
#include "print.h"


#define R_GND_air (499000.0)
#define R_S_air ((499/2+47.0)*1000.0)

#define R_GND_water (20000.0*100000.0)/(20000.0+100000.0)
#define R_S_water (10000.0*47000.0)/(10000.0+47000.0)

/**======MATLAB=======*
T = [-10 25 45] + t0;
R = [62927 10000  4103];
L = log(R);
Y = 1./T;
y2 = (Y(2) - Y(1))/(L(2)-L(1));
y3 = (Y(3) - Y(1))/(L(3)-L(1));
C = ((y3-y2)/(L(3)-L(2)))/sum(L);
B = y2 - C * (L(1)^2 + L(1)*L(2)+ L(3)^2);
A = Y(1) - (B + L(1)^2*C)*L(1);
t = 1./(A + B.*log(Rt)+C.*((log(Rt).^3)))  -0.2;


************************/


#if 0//10K
#define A_coef (0.001261945688736)
#define B_coef (2.205492015409339e-04)
#define C_coef (7.498073353929740e-08)
#define T0K    (-273.15)
#define t_correction (-0.2)
#endif

#if 1 //  47K

#define A_coef (0.001097355758823)
#define B_coef (1.956560632531253e-04)
#define C_coef (1.225006226047844e-07)
#define T0K    (-273.15)
#define t_correction (-0.2)
#endif

/***approx****/
float air_k1 = 5.3201e-05;
float air_b1 = -0.0013211;
float air_k2 = 0.10318;
float air_b2 = 0.6107;
float air_c = -0.05;

float water_k1 = 4.4407e-05;
float water_b1 = -0.0012652;
float water_k2 = 0.062126;
float water_b2 = 0.74963;
float water_c = 0.0;


long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//------------------------------------------------------------------------------------------------------

void ADC_Init(void)
{
    ADCSRA |= (1<<ADEN)    
    |(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

//------------------------------------------------------------------------------------------------------

float DirectVolt;
uint16_t DirectADC;

void GetDirect(void){
    ADMUX = 1;       
    ADCSRA |= (1<<ADSC);         
    while((ADCSRA & (1<<ADSC)));  
    DirectADC = ADCW;
    DirectVolt = boiler.K_adc * DirectADC;
    // ADMUX &=~(1);
    ADCSRA &=~(1<<ADSC);
}


void GetRef(void){
    ADMUX = 0;  
    ADCSRA |= (1<<ADSC);         
    while((ADCSRA & (1<<ADSC))); 
    boiler.vref_adc = ADCW;
    boiler.K_adc = 2.050 / boiler.vref_adc;
    boiler.vdd = boiler.K_adc *1023.0; 
    ADCSRA &=~(1<<ADSC);
}


float adc2voltageApprox(float vdd, uint16_t adc, float k1, float k2, float b1, float b2, float c){
    return  vdd*(vdd*(k1*adc+k2)+b1*adc+b2) + c;
}

void SendAirTemp (void)
{        
    float logR;
    ADMUX = 1;       
    ADCSRA |= (1<<ADSC);          
    while((ADCSRA & (1<<ADSC)));  
    boiler.air_temp_adc = ADCW; 
    boiler.air_voltage =adc2voltageApprox(boiler.vdd, boiler.air_temp_adc, air_k1, air_k2, air_b1, air_b2, air_c);
    boiler.air_resistance = (R_S_air + R_GND_air)*( boiler.vdd / boiler.air_voltage - 1); 
    logR = log(boiler.air_resistance);    
    boiler.air_temp = 1/(A_coef + logR*(B_coef+C_coef*logR*logR))+t_correction + T0K;
    ADCSRA &=~(1<<ADSC);   
}

//------------------------------------------------------------------------------------------------------


void SendWaterTemp (void)
{   
    float logR;
    ADMUX = 2;                       
    ADCSRA |= (1<<ADSC);                          
    while((ADCSRA & (1<<ADSC)));                  
    boiler.water_temp_adc = ADCW;
    boiler.water_voltage =adc2voltageApprox(boiler.vdd, boiler.water_temp_adc, water_k1, water_k2, water_b1, water_b2, water_c);
    boiler.water_resistance = (R_S_water + R_GND_water)*( boiler.vdd / boiler.water_voltage - 1); 
    logR = log(boiler.water_resistance);    
    boiler.water_temp = 1/(A_coef + logR*(B_coef+C_coef*logR*logR))+t_correction + T0K;
    ADCSRA &=~(1<<ADSC);
}
