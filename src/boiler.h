#ifndef  BOILER_H
#define  BOILER_H

#include <stdint.h>

#define selUK103  (!(PINB & 0b10000000))
#define selUK203  (!(PING & 0b00001000))
#define selUK303  (!(PING & 0b00010000))

#define WATER_TEMP_MIN (17)
#define WATER_TEMP_MAX (95)

typedef enum{
  boiler_idle = 0,
  boiler_start_sequence,
  boiler_work_heating,               
  bolier_work_blowing_precheck,
  boiler_work_blowing_after_heating,
  boiler_work_cooling,
  boiler_max,
}boiler_state_e;

extern char* boiler_state_str[];      

typedef enum{
    stage_none= 0,
    stage_blowing_test_off_check,
    stage_blowing_test_on_pre_check,        
    stage_blowing, 
    stage_transformer_on,    
}start_sequence_stage_e;

extern char* start_sequence_stage_str[];


typedef enum{
  role_not_selected = 0,
  role_UK_103,
  role_UK_203,
  role_UK_303,
  role_LAN_master,
  role_LAN_slave1,
  role_LAN_slave2,
  role_max,
}boiler_role;

extern char*  boiler_role_str[];

typedef enum{
    weather_none = 0,
    weather_winter,
    weather_summer,
    weather_max,
}weather_dep_e;

extern char* weather_dep_str[];


typedef enum{
    error_None = 0,
    error_WaterLevel,
    error_Overheat ,
    error_FlameTest,
    error_FlameWork,
    error_IonTest,
    error_IonWork,
    error_AirDetectorTest,
    error_AirDetectorWork, 
    error_WaterTempError,
    error_max,
}errors_e;

extern char* errors_str[];

typedef struct
{
  uint16_t vref_adc;
  uint16_t air_temp_adc;
  uint16_t water_temp_adc;  
  
  float vref;
  float vdd;
  float air_temp;
  float water_temp; 
  
  float air_resistance;
  float water_resistance;
  
  float air_voltage;
  float water_voltage;
  
  float K_adc;
  
  int16_t gisteresis;
  int16_t water_goal_temp;
  int16_t water_pump_on_temp;//difference between water_goal and when pump must be ON.
  int16_t water_pump_gist_temp;
  uint32_t time_of_last_stage; 
  weather_dep_e weather;
  boiler_state_e state;
  boiler_state_e command;
  boiler_role role; 
  start_sequence_stage_e start_stage;
  errors_e error;
}boiler_t;

extern boiler_t boiler;

void boiler_init(void); 

void boiler_toggle(void);
void boiler_start(void);
void boiler_stop(void);
void boiler_update(void);
void boiler_update_nvm(void);  
void boiler_update_pump(void); 
  

#endif
