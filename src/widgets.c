#include "widgets.h"
#include "print.h"
#include "boiler.h"
#include "tick_time.h"
#include <graphics.h>
#include "input_output_signals.h"
#include "icons.h"

#define DRAW_DEFAULT_PERIOD    (1000)
#define DRAW_AIRTEMP_PERIOD    (1000)
#define DRAW_WATERTEMP_PERIOD  (1000)
#define DRAW_MODELNAME_PERIOD  (1000)
#define DRAW_PUMP_PERIOD       (1000)
#define DRAW_BURNER_PERIOD     (1000)
#define DRAW_FIRE_PERIOD       (1000)
#define BURNER_X               (0)
#define FIRE_X                 BURNER_X+(22)
#define CIRCULATION_X          FIRE_X+(24)
#define ANTIFREEZE_X           CIRCULATION_X+(35)
#define DEFAULT_Y              (44)

void clear(uint32_t x,uint32_t y,uint32_t w, uint32_t h){
    glcd_setfillcolor(0);
    glcd_fillrectround(x,y,w,h,0);
}


void drawAirTemp(void){
    static uint32_t time = 0;
    uint16_t x = 0;
    uint16_t y = 0;
    if(GetTick_ms() - time > DRAW_DEFAULT_PERIOD){
        glcd_printfxy(x,y, "%3.0f", boiler.air_temp);
        time = GetTick_ms();
    }
}

void drawWaterTemp(void){
    static uint32_t time = 0;
    uint16_t x = 127-7*6;
    uint16_t y = 0;
    if(GetTick_ms() - time > DRAW_DEFAULT_PERIOD){
        glcd_printfxy(x,y, "%3.0f->%d", boiler.water_temp, boiler.water_goal_temp);
        time = GetTick_ms();
    }

}

void drawModelName(void){
    static uint32_t time = 0;
    static char* br_str="";
    uint16_t x = 127/4;
    uint16_t y = 0;
    //glcd_printfxy(x,y, "%s",br_str);
    if(br_str != boiler_role_str[boiler.role]){
        if(GetTick_ms() - time > DRAW_DEFAULT_PERIOD){
            glcd_printfxy(x,y, "%s", boiler_role_str[boiler.role]);
            br_str=boiler_role_str[boiler.role];
            time = GetTick_ms();
        }
        
    }

}

void drawPump(void){
    static uint32_t time = 0; 
    static char drawn = 0;
    uint16_t x = 0;
    uint16_t y = 56;
    if(GetTick_ms() - time > DRAW_DEFAULT_PERIOD){
        if(isSetevoi_nasos_ON){
            //glcd_printfxy(x,y, "TOPL_ON ");
            if(!drawn){
                glcd_putimagef(CIRCULATION_X,DEFAULT_Y ,circulation,0);
                drawn = 1;   
            }    
        }
        else{
            //glcd_printfxy(x,y, "TOPL_OFF");
            if(drawn){
                clear(CIRCULATION_X-1,DEFAULT_Y-1,circulation[0]+2,circulation[2]+2);
                drawn = 0;                      
            }            
        }
        time = GetTick_ms();
    }
}

void drawBurner(void){
    static uint32_t time = 0;
    static uint32_t j=0;
    uint16_t x = 0;
    uint16_t y = 48;
    if(GetTick_ms() - time > DRAW_DEFAULT_PERIOD){
        //glcd_printfxy(x,y, "BURNER");
        glcd_putimagef(BURNER_X,DEFAULT_Y,burner[j%3],0);
        if(isVentilaytor_ON){
            j++;// if(isVentilaytor_ON)
        }        
        time = GetTick_ms();
    }
}

void drawFire(void){
    static uint32_t time = 0;
    static char drawn = 0;
    uint16_t x = 0;
    uint16_t y = 40;
    if(GetTick_ms() - time > DRAW_DEFAULT_PERIOD){
        if(plamya_out ||\
             Ion_out ||\
             ((boiler.role == role_UK_303) && clapan_out())){
            //glcd_printfxy(x,y, "FIRE_ON ");     
            if(!drawn){
                glcd_putimagef(FIRE_X,DEFAULT_Y-1,flame,0);
                drawn = 1;
            }
        }
        else{
            //glcd_printfxy(x,y, "FIRE_OFF");
            if(drawn){
                clear(FIRE_X-1,DEFAULT_Y-1,flame[0]+2,flame[2]+2);
                drawn = 0;    
            }
        }
        time = GetTick_ms();
    }
}

void drawFan(void){
    static uint32_t time = 0;
    uint16_t x = 0;
    uint16_t y =  32;    
    if(GetTick_ms() - time > DRAW_DEFAULT_PERIOD){
        if(isVentilaytor_ON){
            glcd_printfxy(x,y, "FAN_ON ");
        }
        else{
            glcd_printfxy(x,y, "FAN_OFF");
        }
        
        time = GetTick_ms();
    }
}

void drawClapan1(void){
    static uint32_t time = 0;
    uint16_t x = 50;
    uint16_t y = 32;    
    if(GetTick_ms() - time > DRAW_DEFAULT_PERIOD){
        if(isKlapan1_ON){
            glcd_printfxy(x,y, "CL1_ON ");
        }
        else{
            glcd_printfxy(x,y, "CL1_OFF");
        }
        
        time = GetTick_ms();
    }
}

void drawClapan2(void){
    static uint32_t time = 0;
    uint16_t x = 50;
    uint16_t y = 40;    
    if(GetTick_ms() - time > DRAW_DEFAULT_PERIOD){
        if(isKlapan2_ON){
            glcd_printfxy(x,y, "CL2_ON ");
        }
        else{
            glcd_printfxy(x,y, "CL2_OFF");
        }
        
        time = GetTick_ms();
    }
}

void drawState(void){
    static uint32_t time = 0;
    uint16_t x = 50;
    uint16_t y = 48;    
    if(GetTick_ms() - time > DRAW_DEFAULT_PERIOD){
        glcd_printfxy(x,y, "%s", boiler_state_str[boiler.state]);
        //PRINT("%s\r\n", boiler_state_str[boiler.state]);
        time = GetTick_ms();
    }
}

void drawStartState(void){
    static uint32_t time = 0;
    uint16_t x = 50;
    uint16_t y = 56;    
    if(GetTick_ms() - time > DRAW_DEFAULT_PERIOD){
        glcd_printfxy(x,y, "%s", start_sequence_stage_str[boiler.start_stage]);
        //PRINT("%s\r\n", start_sequence_stage_str[boiler.start_stage]);
        time = GetTick_ms();
    }
}

void drawCmdState(void){
    static uint32_t time = 0;
    uint16_t x = 0;
    uint16_t y = 48;    
    if(GetTick_ms() - time > DRAW_DEFAULT_PERIOD){
        glcd_printfxy(x,y, "%s", boiler_state_str[boiler.command]);
        //PRINT("%s\r\n", start_sequence_stage_str[boiler.start_stage]);
        time = GetTick_ms();
    }
}

void drawError(void){
    static uint32_t time = 0;
    uint16_t x = 30;
    uint16_t y = 40;    
    if(GetTick_ms() - time > DRAW_DEFAULT_PERIOD){
        //glcd_printfxy(x,y, "Error %d", boiler.error);
        glcd_printfxy(x,y, "\x8F\xB9\xA9\xA2\xAB\xA1 %d", boiler.error);
        glcd_printfxy(x,y+12, "%s", errors_str[boiler.error]);
        glcd_putimagef(BURNER_X,DEFAULT_Y,error_icon,0);
        //PRINT("%s\r\n", start_sequence_stage_str[boiler.start_stage]);
        time = GetTick_ms();
    }
}

void drawAntiFreeze(void){
    static uint32_t time = 0;
    if(GetTick_ms() - time > DRAW_DEFAULT_PERIOD){
        glcd_putimagef(ANTIFREEZE_X,DEFAULT_Y,snow,0);
        //PRINT("%s\r\n", start_sequence_stage_str[boiler.start_stage]);
        time = GetTick_ms();
    }
}


typedef enum
{
    drawn_none = 0,
    drawn_pictures,
    drawn_error,
}drawn_e;

void widgetsUpdate(void){
    static drawn_e state = drawn_none;
    //PRINT(">>>widgetsUpdate\r\n");
    drawAirTemp();
    //drawWaterTemp();
    drawModelName();
    // drawPump();
    // //drawCmdState();
    // drawFire();
    // drawBurner();
    // drawAntiFreeze();        
    if(boiler.error == error_None){
        if(state != drawn_pictures){
            clear(0, 33, 127, 63);
            state = drawn_pictures;        
        }
        drawPump();
        //drawCmdState();
        drawFire();
        drawBurner();
        drawAntiFreeze();
        //drawFan();
        //drawClapan1();
        //drawClapan2();  
        //drawState();
        //drawStartState();
    }
    else{
        if(state != drawn_error){
            clear(0, 33, 127, 63);
            drawError();
            state = drawn_error;      
        }        
    }
    //PRINT("<<<widgetsUpdate\r\n");
}