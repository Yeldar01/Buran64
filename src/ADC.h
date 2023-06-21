#ifndef _ADC_H_
#define _ADC_H_

long map(long x, long in_min, long in_max, long out_min, long out_max);
void ADC_Init(void);
void SendAirTemp (void);
void SendWaterTemp (void);
void GetRef(void);
void GetDirect(void);

#endif //_ADC_H_
