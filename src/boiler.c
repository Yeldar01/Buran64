#include "boiler.h"
#include "print.h"
#include "input_output_signals.h"

#include <eeprom.h>
#include <delay.h>
#include "tick_time.h"

#if 0 
#undef PRINT
#define PRINT(...)
#endif

#define ERRORS_ACTIVE (1)

char* boiler_state_str[] = {
  "idle    ",
  "starting",
  "heating ",
  "pre-blow",
  "blow2   ",
  "cooling ",  
};

char* start_sequence_stage_str[] = {
    "none      ",      //"none      ",
    "blowingoff",      //"blowing   ",
    "blowPreChk",      //"fire test ",
    "blowing   ",      //"fire work ",
    "trans ON  ",      //"clapan tst",
    "clapTstChk",      //"clapTstChk"
    "clapanPreW",      //"clapanPreW"
    "clapan wrk",      //"clapan wrk",
    "transformr",      //"transformr"
    "ion test  ",      //"ion test  ",
    "ion work  ",      //"ion work  ",
};

char*  boiler_role_str[] = {
  " no model",
  "  UK 103  ",
  "  UK 203  ",
  "  UK 303  ",
  "LAN master",
  "LAN slave1",
  "LAN slave2",
};

char* weather_dep_str[] = { 
     "None  ",  //   "\x8E\xA1\xA4\xB1\xA6\xA3",  // 
     "Winter",  //   "  \x88\xA9\xAD\xA1",        // 
     "Summer"   //   "  \x8C\xA6\xB3\xAF"         // 
};

char* errors_str[] = {
   "None",                 // "\x8E\xA6\xB3",                                                                       // "No",   
   "Water level",          // "\x94\xB1\xAF\xA3\xA6\xAE\xBD \xA3\xAF\xA5\xBC",                                      // "WatL", 
   "Overheat",             // "\x90\xA6\xB1\xA6\xA4\xB1\xA6\xA3",                                                   // "OvH",  
   "Flame detector test",  // "\x8C\xAF\xA7\xAE\xAF\xA6 \xB0\xAC\xA1\xAD\xC0",                                      // "FlT",  
   "No flame",             // "\x8E\xA6\xB3 \xB0\xAC\xA1\xAD\xA6\xAE\xA9",                                          // "NoF",  
   "Ion detector test",    // "\x93\xA6\xB2\xB3 \xA9\xAF\xAE\xA9\xA8\xA1\xB7\xA9\xA9",                              // "IoT",  
   "No ionization",        // "\x8E\xA6\xB3 \xA9\xAF\xAE\xA9\xA8\xA1\xB7\xA9\xA9",                                  // "nIo",  
   "Air detector test",    // "\x93\xA6\xB2\xB3 \xA5\xA1\xB3\xB8\xA9\xAB\xA1 \xA3\xAF\xA8\xA5\xB4\xB6\xA1",         // "AiT",  
   "Air detector work",    // "\x91\xA1\xA2\xAF\xB3\xA1 \xA5\xA1\xB3\xB8\xA9\xAB\xA1 \xA3\xAF\xA8\xA5\xB4\xB6\xA1", // "AiW",  
   "Thermistor failure",   // "\x8F\xB9\xA9\xA2\xAB\xA1 \xB3\xA6\xB1\xAD\xA9\xB2\xB3\xAF\xB1\xA1",                  // "Trm",  
};                                           

typedef struct{
    int16_t        water_goal_temp;
    int16_t        gisteresis;
    int16_t        pump_on_temp;
    int16_t        pump_gist_temp;
    weather_dep_e  weather;
    boiler_role    role;
    boiler_state_e command;
    uint8_t        not_programmed;
}nvm_data_t;

eeprom  nvm_data_t eeinit_data;          

nvm_data_t eedefault = {
    .water_goal_temp = 50,
    .gisteresis      = 5,
    .pump_on_temp    = 15,
    .pump_gist_temp  = 3,
    .weather         = weather_none,
    .role            = role_UK_203,
    .command         = boiler_idle,
    .not_programmed  = 0,
};

nvm_data_t nvm_cache;

boiler_t boiler;

void turnAllOff(void){
    TransRozgiga_OFF;
    Ventilaytor_OFF;
    Topl_Nasos_OFF;
    //Setevoi_nasos_OFF;
    Klapan1_OFF;
    Klapan2_OFF;
    boiler.time_of_last_stage = 0;
}
            
void boiler_init(void){
    PRINT(">>boiler_init()\r\n"); 
    eeprom_read_block(&nvm_cache, &eeinit_data, sizeof(nvm_data_t));
    if(nvm_cache.not_programmed){
        eeprom_write_block(&eedefault, &eeinit_data, sizeof(nvm_data_t)); 
        eeprom_read_block(&nvm_cache, &eeinit_data, sizeof(nvm_data_t));
    }
    
    boiler.command         = nvm_cache.command;
    boiler.water_goal_temp = nvm_cache.water_goal_temp; 
    boiler.water_pump_on_temp = nvm_cache.pump_on_temp;
    boiler.water_pump_gist_temp = nvm_cache.pump_gist_temp;    
    boiler.gisteresis      = nvm_cache.gisteresis;
    boiler.weather         = nvm_cache.weather;
    boiler.role            = nvm_cache.role;
    //TODO: CRC or checksum
        
    boiler.vdd = 5.0;
    boiler.vref = 2.050;
    boiler.K_adc =  boiler.vref / 1023;
    
    PRINT("<<boiler_init()\r\n");
}

char check_reset_error(errors_e err, uint32_t err_condition){
    if(boiler.error == err){
        if(!err_condition){
            boiler.error = error_None;
        }
    }
    if(err_condition){
        boiler.error = err;
        return 1;
    }
    return 0;
}


void check_errors(void){
    PRINT(">>>check_errors\r\n");
    if(check_reset_error(error_WaterLevel, !Uroven_out)){
        return;
    }
    if(check_reset_error(error_Overheat, t_max_out)){
        return;
    }
    if(check_reset_error(error_WaterTempError, (boiler.water_temp < WATER_TEMP_MIN) || (boiler.water_temp > WATER_TEMP_MAX) )){
        return;
    }
    
    PRINT("switch state = %s\r\n", boiler_state_str[boiler.state]);
    PRINT("switch start = %s\r\n", start_sequence_stage_str[boiler.start_stage]);
    PRINT("switch role = %s\r\n", boiler_role_str[boiler.role]);
    switch (boiler.state){
        case boiler_idle:
            PRINT("boiler idle...\r\n");
            break;
        case boiler_start_sequence:
            PRINT("boiler boiler_start_sequence...\r\n");
            switch(boiler.role){
                case role_not_selected: 
                    PRINT("role_not_selected...\r\n");
                    break;
                case role_UK_103:   
                    PRINT("role_UK_103...\r\n");
                    switch (boiler.start_stage){
                        case stage_none:
                        case stage_blowing:
                            PRINT("stage_none or stage_blowing...\r\n");
                            if(plamya_out){
                                boiler.error = error_FlameTest;
                            }
                            break;
                        case stage_transformer_on:
                            break;
                        default:

                            break;
                    }
                    break;
                case role_UK_203:   
                    PRINT("UK_203...\r\n");
                    switch (boiler.start_stage){
                        case stage_none:
                        case stage_blowing_test_off_check:
                            PRINT("stge none blowing test...\r\n");
                            if(clapan_out()){
                                PRINT("errror air test...\r\n");
                                boiler.error = error_AirDetectorTest;
                                return;
                            }
                            if(Ion_out){
                                PRINT("ion test...\r\n");
                                boiler.error = error_IonTest;
                            }
                            break;
                        case stage_blowing_test_on_pre_check:
                            PRINT("test on pre check...\r\n");
                            if(Ion_out){ 
                                PRINT("ion test...\r\n");
                                boiler.error = error_IonTest;
                            }
                            break;

                        case stage_blowing: 
                            PRINT("blowing...\r\n");
                            if(!clapan_out()){
                                boiler.error = error_AirDetectorWork;
                                return;
                            }
                            if(Ion_out){
                                boiler.error = error_IonTest;
                            }
                            break;
                        default:

                            break;

                    }
                    break;
                case role_UK_303:

                    break;
                case role_LAN_master:

                    break;
                case role_LAN_slave1:

                    break;
                case role_LAN_slave2:

                    break;

            }
            break;
        case boiler_work_heating:
            PRINT("boiler_work_heating...\r\n");
            switch(boiler.role){
                case role_not_selected:
                break;
                case role_UK_103:
                    if(!plamya_out){
                        PRINT("error_FlameWork...\r\n");
                        boiler.error = error_FlameWork;
                    }
                break;
                case role_UK_203:
                    if(!clapan_out()){
                        boiler.error = error_AirDetectorWork;
                        return;
                    }
                    if(!Ion_out){
                        boiler.error = error_IonWork;
                    }
                break;
                case role_UK_303:

                break;
                case role_LAN_master:

                break;
                case role_LAN_slave1:

                break;
                case role_LAN_slave2:

                break;

            }   
            break;
     
        case bolier_work_blowing_precheck:
        
            break;    
            
        case boiler_work_blowing_after_heating:   
            PRINT("boiler_work_blowing_after_heating...\r\n");
            switch(boiler.role){
                case role_not_selected:
                break;
                case role_UK_103:
                    PRINT("error_FlameWork...\r\n");
                    if(plamya_out){
                        boiler.error = error_FlameWork;
                    }
                break;
                case role_UK_203:
                    if(!clapan_out()){
                        boiler.error = error_AirDetectorWork;
                        return;
                    }
                    if(Ion_out){
                        boiler.error = error_IonWork;
                    }
                break;
                case role_UK_303:

                break;
                case role_LAN_master:

                break;
                case role_LAN_slave1:

                break;
                case role_LAN_slave2:

                break;
            }
            break;
        
        case boiler_work_cooling:
    
            break;
    }  
    PRINT("<<<check_errors\r\n");
}

void boiler_update(){
    PRINT(">>>boiler_update()\r\n"); 
    PRINT("hadling %s\r\n", boiler_role_str[boiler.role]); 
#if ERRORS_ACTIVE
    check_errors();
    PRINT("error = %s\r\n", errors_str[boiler.error]);
#endif

    
    static uint32_t oneminute_counter = 0;
    
    if(boiler.error == error_None){
    }
    else{
        if( (boiler.error == error_WaterLevel) ||
            (boiler.error == error_Overheat)  ||
            (boiler.error == error_WaterTempError)){   
        }
        else{//more serious errors
            boiler.command = boiler_idle; 
        }
        boiler.state = boiler_idle;
        boiler.start_stage = stage_none;
    }
    
    switch (boiler.state){
        case boiler_idle:
            //turn all off
            PRINT("boiler_idle...\r\n");  
            turnAllOff();
            if(boiler.command == boiler_work_heating){
                if(boiler.water_temp < boiler.water_goal_temp - boiler.gisteresis){
                    boiler.state = boiler_start_sequence;
                    boiler.start_stage = stage_none; 
                }
                else{
                    boiler.state = boiler_work_cooling;
                }
            }
            else{
                //keep evething off
            }
            break;
    
        case boiler_start_sequence:
            PRINT("boiler_start_sequence...\r\n");  
            if(boiler.command == boiler_work_heating){
                if(boiler.water_temp < boiler.water_goal_temp){
                    PRINT("water temp < goal...\r\n");  
                    //ignition sequence 
                    switch(boiler.role){
                        case role_not_selected: 
                            PRINT("role_not_selected...\r\n");  
                            boiler.state = boiler_work_heating;
                            break;           
                        case role_UK_103: 
                            PRINT("role_UK_103...\r\n");
                            switch(boiler.start_stage){
                                case stage_none:
                                    PRINT("stage_none...\r\n"); 
                                    boiler.time_of_last_stage = GetTick_ms();
                                    boiler.start_stage = stage_blowing;
                                    Ventilaytor_ON;
                                    break;
                                case stage_blowing: 
                                    PRINT("stage_blowing...\r\n");
                                    if(GetTick_ms() - boiler.time_of_last_stage > 20000){ 
                                        TransRozgiga_ON;
                                        Topl_Nasos_ON;
                                        boiler.start_stage = stage_transformer_on;                                         
                                        boiler.time_of_last_stage = GetTick_ms();  
                                    }
                                    break;
                                case stage_transformer_on:
                                    PRINT("stage_transformer_on...\r\n");
                                    if(GetTick_ms() - boiler.time_of_last_stage < 3000){
                                    #if ERRORS_ACTIVE
                                        if(plamya_out){
                                    #endif
                                            TransRozgiga_OFF;
                                            boiler.start_stage = stage_none;
                                            boiler.state = boiler_work_heating;
                                            boiler.time_of_last_stage = GetTick_ms();
                                    #if ERRORS_ACTIVE                                    
                                        } 
                                    #endif      
                                    }
                                    else{
                                        PRINT("error_FlameWork...\r\n");
                                        boiler.error = error_FlameWork;
                                    }
                                    break;
                            }
                            break; 
                        
                        case role_UK_203:
                            switch(boiler.start_stage){
                                case stage_none:
                                    boiler.time_of_last_stage = GetTick_ms();
                                    boiler.start_stage = stage_blowing_test_off_check;
                                    break;
                                
                                case stage_blowing_test_off_check:
                                    if(GetTick_ms() - boiler.time_of_last_stage > 300){
                                        boiler.start_stage = stage_blowing_test_on_pre_check;
                                        Ventilaytor_ON;
                                        boiler.time_of_last_stage = GetTick_ms();  
                                    }
                                    break;
                                case stage_blowing_test_on_pre_check:
                                    if(GetTick_ms() - boiler.time_of_last_stage > 5000){
                                        boiler.start_stage = stage_blowing;
                                        boiler.time_of_last_stage = GetTick_ms();  
                                    }
                                    break;
                                case stage_blowing:
                                    if(GetTick_ms() - boiler.time_of_last_stage > 20000){ 
                                        TransRozgiga_ON;
                                        Klapan1_ON;
                                        boiler.start_stage = stage_transformer_on;                                          
                                        boiler.time_of_last_stage = GetTick_ms();  
                                    }
                                    break;
                                case stage_transformer_on:
                                    if(GetTick_ms() - boiler.time_of_last_stage < 3000){
                                    #if ERRORS_ACTIVE
                                        if(!clapan_out()){
                                            boiler.error = error_AirDetectorWork;
                                        }
                                        if(Ion_out){
                                    #endif
                                            TransRozgiga_OFF;
                                            boiler.start_stage = stage_none;
                                            boiler.state = boiler_work_heating;
                                            boiler.time_of_last_stage = GetTick_ms();
                                    #if ERRORS_ACTIVE  
                                        }            
                                    #endif
                                    }
                                    else{
                                        boiler.error = error_IonWork;
                                    }
                                    break;
                            }
                        break;   
                        
                        case role_UK_303:
                            Klapan2_ON;
                            boiler.time_of_last_stage = GetTick_ms();
                            boiler.state = boiler_work_heating;
                        break;  
                        
                        case role_LAN_master:
                
                        break;   
                        
                        case role_LAN_slave1:
                
                        break;  
                        
                        case role_LAN_slave2:
                
                        break;
                    }
                
                
                }
                else{
                    boiler.state = boiler_work_cooling;
                }
                
            }
            else{
                //all off
                boiler.state = boiler_idle;
            }
        break; 
        
        case boiler_work_heating:
            PRINT("boiler_work_heating...\r\n");
            switch (boiler.role){
                case role_not_selected:
                case role_UK_103:
                case role_UK_203:
                case role_LAN_master:
                case role_LAN_slave1:
                case role_LAN_slave2:
                    if(boiler.command == boiler_work_heating){
                        if(boiler.water_temp < boiler.water_goal_temp){                        
                        }
                        else{
                            boiler.state = bolier_work_blowing_precheck;
                            boiler.time_of_last_stage = GetTick_ms();
                            oneminute_counter = 0;
                        }
                    }
                    else{
                    //all off
                        boiler.state = bolier_work_blowing_precheck;
                        boiler.time_of_last_stage = GetTick_ms();
                        oneminute_counter = 0;                        
                    }
                    
                    break;
                case role_UK_303:
                    if(boiler.command == boiler_work_heating){
                        if(boiler.water_temp < boiler.water_goal_temp){                        
                        }
                        else{
                            boiler.state = boiler_work_cooling;
                        }
                    }
                    else{
                        //all off
                        boiler.state = boiler_idle;                        
                    }
                    break;
            } 
            break; 
        
        case bolier_work_blowing_precheck:
            switch (boiler.role){
                case role_not_selected:
                case role_UK_103:
                case role_UK_203:
                case role_LAN_master:
                case role_LAN_slave1:
                case role_LAN_slave2:
                    TransRozgiga_OFF;
                    Topl_Nasos_OFF;
                    Klapan1_OFF;
                    Klapan2_OFF;
                    if(GetTick_ms() - boiler.time_of_last_stage > 3000){
                        boiler.state = boiler_work_blowing_after_heating;
                        boiler.time_of_last_stage = GetTick_ms();
                        oneminute_counter = 0;      
                    }                    
                    break;
                case role_UK_303:
                    break;
            } 
            break;
        break;
        
        case boiler_work_blowing_after_heating:
        
            //this stage does not react to command, leaving the rest to boiler_work_cooling stage
            TransRozgiga_OFF;
            Topl_Nasos_OFF;
            Klapan1_OFF;
            Klapan2_OFF;
            if(GetTick_ms() - boiler.time_of_last_stage > 60000){//limitations of 16bit max
                boiler.time_of_last_stage = GetTick_ms();
                //oneminute_counter++;
                //if(oneminute_counter == 3){//3*20000ms=60 sec
                    boiler.state = boiler_work_cooling;//will turn off completely on the next round
                //    oneminute_counter = 0;
                //}
            }
            break;
        
        case boiler_work_cooling:
            turnAllOff();
            if(boiler.command == boiler_work_heating){
                if(boiler.water_temp < boiler.water_goal_temp - boiler.gisteresis){  
                      boiler.state = boiler_start_sequence;                 
                }
                else{
                    //all off
                }       
            }
            else{
                //all off
                boiler.state = boiler_idle;                        
            }
        break;
    }
    PRINT("error = %s\r\n", errors_str[boiler.error]);
    PRINT("<<<boiler_update()\r\n");
}

void boiler_start(void){
    if(boiler.state == boiler_idle){
        boiler.error =  error_None;
    }
    boiler.command = boiler_work_heating;
}

void boiler_stop(void){
    boiler.command = boiler_idle;
}

void boiler_toggle(void){
    if(boiler.command == boiler_idle){
        boiler_start();
    }
    else{
        boiler_stop();
    }
}

void boiler_update_nvm(void){
    //PRINT("===>>>boiler_update_nvm\r\n");
    if( (boiler.command         != nvm_cache.command         )||
        (boiler.water_goal_temp != nvm_cache.water_goal_temp )||
        (boiler.gisteresis      != nvm_cache.gisteresis      )||
        (boiler.weather         != nvm_cache.weather         )||
        (boiler.role            != nvm_cache.role            )){
        //PRINT("=======WRITING EEPROM!!!=========\r\n");
        nvm_cache.command         = boiler.command         ;
        nvm_cache.water_goal_temp = boiler.water_goal_temp ;
        nvm_cache.gisteresis      = boiler.gisteresis      ;
        nvm_cache.weather         = boiler.weather         ;
        nvm_cache.role            = boiler.role            ;
        eeprom_write_block(&nvm_cache, &eeinit_data, sizeof(nvm_data_t));
        //delay_ms(1000); 
    }
    //PRINT("===<<<boiler_update_nvm\r\n");
}

void boiler_update_pump(void){//circulyacionny nasos
    switch (boiler.weather){
        case weather_none:
        case weather_summer:
            if(boiler.error && (boiler.error != error_WaterLevel)){
                Setevoi_nasos_ON; 
            }
            else{
                Setevoi_nasos_OFF;    
            }       
            break;
        case weather_winter:
            if(boiler.error){
               Setevoi_nasos_ON; 
            }
            else{//handle temperature, gisteresis 
                int16_t pump_on_diff = (boiler.gisteresis > boiler.water_pump_on_temp) ? \
                                            boiler.gisteresis : boiler.water_pump_on_temp;                
                if(boiler.water_temp > (boiler.water_goal_temp - \
                                        pump_on_diff)){
                    Setevoi_nasos_ON;                                                                    
                }
                if(boiler.water_temp < (boiler.water_goal_temp - \
                                        pump_on_diff - \
                                        boiler.water_pump_gist_temp)){
                    Setevoi_nasos_OFF;
                }
            }
            break;                                                 
    }
}