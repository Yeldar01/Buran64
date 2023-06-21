#include "uniteBoilerButtonMenu.h"

#include "boiler.h"
#include "menu.h"
#include "button_control.h"

#include "print.h"

#if (1)
#undef PRINT
#define PRINT(...)
#endif

extern menuPage_t main_page;
//#define NUM_MAIN_PAGE (2)

void incrementEntry(entry_t* e);
void decrementEntry(entry_t* e);

void boiler_toggle_wrap(entry_t* e){
    (void)e;
    boiler_toggle();
}


volatile entry_t main_page_list[] = {                                                                                                                                                                                            
  { .sel_callback = boiler_toggle_wrap, .left_callback = 0,               .right_callback = 0,              .left_right_callback = 0, .str_storage = boiler_state_str,.data.u8 = (uint8_t*)&boiler.state,       .prev_data.u8 = 255,    .min.u8 = 0,   .max.u8 = boiler_max - 1,   .text =  "\x83\xAB\xAC-\x83\xBC\xAB\xAC" /*"Work"*/,                    .type  = uint8_t_e, .displayData = 1, .useActiveIndex = 0, .useStrStorage = 1},
  { .sel_callback = 0,                  .left_callback = decrementEntry,  .right_callback = incrementEntry, .left_right_callback = 0, .str_storage = 0,               .data.i16 =  &boiler.water_goal_temp,     .prev_data.i16 = -1,    .min.i16 = 5, .max.i16 = 99,               .text =  "\x8E\xA1\xA4\xB1\xA6\xB3\xBD \xA5\xAF"/*"Heating Temp"*/,     .type  = int16_t_e, .displayData = 1, .useActiveIndex = 0, .useStrStorage = 0},
  { .sel_callback = 0,                  .left_callback = decrementEntry,  .right_callback = incrementEntry, .left_right_callback = 0, .str_storage = weather_dep_str, .data.u8 = (uint8_t*)&boiler.weather,     .prev_data.u8 = 255,    .min.u8 = 0,   .max.u8  = weather_max - 1, .text = "\x83\xB1\xA6\xAD\xC0 \xA4\xAF\xA5\xA1"/*"Weather Mode"*/,      .type  = uint8_t_e, .displayData = 1, .useActiveIndex = 0, .useStrStorage = 1},
  { .sel_callback = 0,                  .left_callback = decrementEntry,  .right_callback = incrementEntry, .left_right_callback = 0, .str_storage = 0,               .data.i16 = &boiler.gisteresis,           .prev_data.i16 = -1,    .min.i16 = 2,  .max.i16 = 10,              .text = "\x84\xA9\xB2\xB3\xA6\xB1\xA6\xA8\xA9\xB2"/*"Gisteresis"*/,     .type  = int16_t_e, .displayData = 1, .useActiveIndex = 0, .useStrStorage = 0},
  { .sel_callback = 0,                  .left_callback = decrementEntry,  .right_callback = incrementEntry, .left_right_callback = 0, .str_storage = 0,               .data.i16 = &boiler.water_pump_on_temp,   .prev_data.i16 = -1,    .min.i16 = 1, .max.i16 = 99,               .text =  "\x83\xAB\xAC \xAE\xA1\xB2\xAF\xB2 \xB0\xB1\xA9"/*"Pump ON"*/, .type  = int16_t_e, .displayData = 1, .useActiveIndex = 0, .useStrStorage = 0},  
  { .sel_callback = 0,                  .left_callback = decrementEntry,  .right_callback = incrementEntry, .left_right_callback = 0, .str_storage = 0,               .data.i16 = &boiler.water_pump_gist_temp, .prev_data.i16 = -1,    .min.i16 = 1,  .max.i16 = 99,              .text = "\x84\xA9\xB2\xB3. \xAE\xA1\xB2\xAF\xB2\xA1"/*"Pump Gist"*/,    .type  = int16_t_e, .displayData = 1, .useActiveIndex = 0, .useStrStorage = 0},                                                                                                                                                                                                                                                                                                               
  { .sel_callback = 0,                  .left_callback = decrementEntry,  .right_callback = incrementEntry, .left_right_callback = 0, .str_storage = boiler_role_str, .data.u8 = (uint8_t*)&boiler.role,        .prev_data.u8 = 255,    .min.u8  = 0,  .max.u8 = role_max - 1,     .text = "\x8D\xAF\xA5\xA6\xAC\xBD"/*"Model"*/,                          .type  = uint8_t_e, .displayData = 1, .useActiveIndex = 0, .useStrStorage = 1},
};
#define NUM_MAIN_PAGE (sizeof(main_page_list)/sizeof(entry_t))
menuPage_t main_page = {
  main_page_list,
  NUM_MAIN_PAGE,
  0,
  0
};

button_callbacks_t buttonCallbacks;

void cmdUp(void){
    menuCommand(up_e);
}

void cmdDown(void){
    menuCommand(down_e);
}

void cmdLeft(void){
    menuCommand(right_e);
}

void cmdRight(void){
    menuCommand(left_e);
}

void cmdSelect(void){
    menuCommand(enter_e);
}
        



void menu_buttons_init(void){
    PRINT(">>>menu_buttons_init()\r\n"); 
    
    PRINT("str = %s\r\n", main_page_list[2].str_storage[0]);
    PRINT("dir = %s\r\n", weather_dep_str[0]); 
    PRINT("data = %d\r\n", *(main_page_list[2].data.u8));
    
    //main_page_list[2].data.str =  weather_dep_str[boiler.weather];
    PRINT("weather_dep_str[boiler.weather] = %s\r\n", weather_dep_str[boiler.weather]);
    PRINT("main_page_list[2].data.str  = %s\r\n", main_page_list[2].data.str );
    buttonCallbacks.left = cmdLeft;
    buttonCallbacks.right = cmdRight;
    buttonCallbacks.up = cmdUp;
    buttonCallbacks.down = cmdDown;
    buttonCallbacks.select = cmdSelect;
    setActivePage(&main_page, void_e);
    renderMenu();
   
    PRINT("str = %s\r\n", main_page_list[2].str_storage[0]);
    PRINT("dir = %s\r\n", weather_dep_str[0]); 
    PRINT("data = %d\r\n", *(main_page_list[2].data.u8));
    PRINT("<<<menu_buttons_init()\r\n");
}

void menu_buttons_update(void){
    button_update(&buttonCallbacks);
}
