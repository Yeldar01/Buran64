#include "button_control.h"
#include <stdint.h>
#include "print.h"
#include "tick_time.h"

/*
#define RightButton     (!(PINE&(1<<6)))     
#define LeftButton        (!(PINE&(1<<5)))      
#define TopButton        (!(PINE&(1<<4)))       
#define BottomButton    (!(PINE&(1<<3)))     
#define SelButton        (!(PINE&(1<<2)))	  
*/
#if 1
#undef PRINT
#define PRINT(...)
#endif

typedef enum{
    pressed_none = 0,
    pressed_RightButton  = 0x1<<0, 
    pressed_LeftButton   = 0x1<<1,  
    pressed_TopButton    = 0x1<<2,  
    pressed_BottomButton = 0x1<<3,
    pressed_SelButton    = 0x1<<4,  
}button_pressed_e;

void emit_command(button_callbacks_t* bc, button_pressed_e cmd){
    switch (cmd){
        case pressed_none:            
        break;        
        case pressed_RightButton: 
            bc->right();
        break;
        case pressed_LeftButton:
            bc->left();
        break;
        case pressed_TopButton:
            bc->up();
        break;
        case pressed_BottomButton:
            bc->down();
        break;
        case pressed_SelButton:
            bc->select();
        break;
    }
}


void button_update(button_callbacks_t* bc){

    static button_pressed_e pressed = pressed_none;  
    button_pressed_e currently_pressed = pressed_none;
    uint8_t buttons_pressed = 0; 
    static uint32_t time_pressed = 0;//for repeatedly emitting commands 
                                            //without button being released 
                                            //select button needs to be handled differently 
    static uint32_t time_emitted = 0;
    uint16_t timeout_repeat = BUTTON_REPEAT_TIMEOUT_1;                                        
    int i = 0;
    PRINT(">>>button_update()\r\n");
    //jump to what was pressed last time, if user press
    // another button while keeping 1st button, we ignore that 
    currently_pressed |= RightButton ? pressed_RightButton : pressed_none;
    currently_pressed |= LeftButton ? pressed_LeftButton : pressed_none;
    currently_pressed |= TopButton ? pressed_TopButton : pressed_none;
    currently_pressed |= BottomButton ? pressed_BottomButton : pressed_none;
    currently_pressed |= SelButton ? pressed_SelButton : pressed_none;
    
    for (i = 0; i<8; i++){
        buttons_pressed += (currently_pressed >> i) & 0x01;
    }
    //PRINT("buttons %d\r\n", buttons_pressed);
    if(buttons_pressed == 0 || buttons_pressed > 1 || 
        (!(currently_pressed & pressed) && pressed != pressed_none)){
        PRINT("=====RESET buttons_pressed = %d\r\n", buttons_pressed);
        time_pressed = 0;
        time_emitted = 0;
        pressed = pressed_none;
        PRINT("<<<button_update()\r\n");
        return;    
    }
 
    //now we have only one unique button pressed
    if(pressed == pressed_none){//1st press 
        PRINT("====BUTTON 0x%d prev = 0x%d==\r\n", currently_pressed, pressed);
        emit_command(bc, currently_pressed);    
        pressed = currently_pressed;
        time_pressed = GetTick_ms();
        time_emitted = GetTick_ms();   
    }
    //PRINT("tick_ms %d time_emitted %d time_pressed %d\r\n", tick_ms, time_emitted, time_pressed); 
    PRINT("PRESSED %d\r\n", tick_ms - time_pressed);
    if(GetTick_ms() - time_pressed > BUTTON_REPEAT_TIMEOUT_SWITCH_2){
        timeout_repeat =  BUTTON_REPEAT_TIMEOUT_2;
    }
    if(GetTick_ms() - time_pressed > BUTTON_REPEAT_TIMEOUT_SWITCH_3){
        timeout_repeat =  BUTTON_REPEAT_TIMEOUT_3;
    }
    if(GetTick_ms() - time_emitted > timeout_repeat){
        //PRINT("timeout_repeat\r\n");
        if(currently_pressed != pressed_SelButton){
            emit_command(bc, currently_pressed);
            PRINT("====continous=============\r\n");
        }
        time_emitted = GetTick_ms();
    }
    
    
    PRINT("<<<button_update()\r\n");
}
    