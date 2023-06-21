#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H
#include <mega64a.h>

#define RightButton     (!(PINE&(1<<5)))     
#define LeftButton        (!(PINE&(1<<6)))      
#define TopButton        (!(PINE&(1<<4)))       
#define BottomButton    (!(PINE&(1<<3)))     
#define SelButton        (!(PINE&(1<<2)))	  

#define BUTTON_REPEAT_TIMEOUT_SWITCH_1 (1000)
#define BUTTON_REPEAT_TIMEOUT_SWITCH_2 (2000)
#define BUTTON_REPEAT_TIMEOUT_SWITCH_3 (3000)
#define BUTTON_REPEAT_TIMEOUT_1 (350)
#define BUTTON_REPEAT_TIMEOUT_2 (200)
#define BUTTON_REPEAT_TIMEOUT_3 (100)

typedef struct{
    void (*left)(void);
    void (*right)(void);
    void (*up)(void);
    void (*down)(void);
    void (*select)(void); 
}button_callbacks_t;



void button_update(button_callbacks_t*);

#endif