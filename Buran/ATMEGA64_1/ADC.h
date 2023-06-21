

#ifndef ks0108_H_
#define ks0108_H_


   


 extern unsigned int WaterTemp;
 unsigned int adc2;
 char Water_temp[4] = {};
 
 
 extern int AirTemp;
 char Air_temp[4] = {}; 
 
 long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
 
 //------------------------------------------------------------------------------------------------------ 

 void ADC_Init(void)
 {
	 ADCSRA |= (1<<ADEN) | (1<<ADATE)    // Разрешение использования АЦП
	 |(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Делитель 128 = 64 кГц  
 }
 
 //------------------------------------------------------------------------------------------------------

 void SendAirTemp (void)
 {
    ADMUX |= (1<<MUX0);           // Включаем канал ADC1
	ADCSRA |= (1<<ADSC);          //Начинаем преобразование
	while((ADCSRA & (1<<ADSC)));  //проверим закончилось ли аналого-цифровое преобразование  
	AirTemp = ADC;
	AirTemp = AirTemp /10 - 3;
	textx=1; texty=0; link ("   ",3);
    sprintf(Air_temp, "%d", AirTemp);
	textx=1; texty=0; link (Air_temp,3);
	ADMUX &=~(1<<MUX0);
	ADCSRA &=~(1<<ADSC);
 }
  
 //------------------------------------------------------------------------------------------------------ 


 void SendWaterTemp (void)
 {
    ADMUX |= (1<<MUX1);           				// Включаем канал ADC2
	ADCSRA |= (1<<ADSC);          				//Начинаем преобразование
	while((ADCSRA & (1<<ADSC)));  				//проверим закончилось ли аналого-цифровое преобразование
	adc2 = ADC;
	
	if(adc2 <= 788 && adc2 >= 653) {WaterTemp = map(adc2, 788, 653, 0, 10);}
	if(adc2 <= 652 && adc2 >= 516) {WaterTemp = map(adc2, 652, 516, 11, 20);}
	if(adc2 <= 515 && adc2 >= 446) {WaterTemp = map(adc2, 515, 446, 21, 25);}
	if(adc2 <= 445 && adc2 >= 387) {WaterTemp = map(adc2, 445, 387, 26, 30);}
	if(adc2 <= 386 && adc2 >= 284) {WaterTemp = map(adc2, 386, 284, 31, 40);}
	if(adc2 <= 283 && adc2 >= 204) {WaterTemp = map(adc2, 283, 204, 41, 50);}
	if(adc2 <= 203 && adc2 >= 149) {WaterTemp = map(adc2, 203, 149, 51, 60);}
	if(adc2 <= 148 && adc2 >= 112) {WaterTemp = map(adc2, 148, 112, 61, 70);}
	if(adc2 <= 111 && adc2 >= 83) {WaterTemp = map(adc2, 111, 83, 71, 80);}
	if(adc2 <= 82 && adc2 >= 65) {WaterTemp = map(adc2, 82, 65, 81, 90);}
	if(adc2 <= 64 && adc2 >= 53) {WaterTemp = map(adc2, 64, 53, 91, 99);}

	textx=18; texty=0; link ("  ",2);
    sprintf(Water_temp, "%d", WaterTemp);
	textx=18; texty=0; link (Water_temp,4);
 	ADMUX &=~(1<<MUX1);
	ADCSRA &=~(1<<ADSC);
 }
 
 //------------------------------------------------------------------------------------------------------
#endif













