#ifndef MENU_H
#define MENU_H

#include <stdint.h>

#define MENU_LINES (3)
#define LINE_HEIGHT (8)

typedef enum{
    nothing = 0,
    up_e,
    down_e,
    right_e,
    left_e,
    left_right_e,
    enter_e,
}menuCommands_e;

typedef enum{
    void_e = 0,
    char_e,
    int8_t_e,
    uint8_t_e,
    int16_t_e,
    uint16_t_e,    
    int32_t_e,
    uint32_t_e,    
    float_e,
    str_e,
}type_tip;

typedef union{
    char ch;
    int8_t i8;
    uint8_t u8;
    int16_t i16;
    uint16_t u16;
    int32_t i32;
    uint32_t u32;
    float  f;
    char* str;
}val_t;


typedef union{
    void* v;
    char* ch;
    int8_t* i8;
    uint8_t* u8;
    int16_t* i16;
    uint16_t* u16;
    int32_t* i32;
    uint32_t* u32;
    float*  f;
    char* str;
}val_p;

typedef struct entry_s{
  void (*sel_callback)(struct entry_s*);
  void (*left_callback)(struct entry_s*);
  void (*right_callback)(struct entry_s*);
  void (*left_right_callback)(struct entry_s*);
  char** str_storage;
  val_p  data;
  val_t  prev_data;
  val_t  min;
  val_t  max;
  char*  text; 
  type_tip type;  
  unsigned displayData : 1; 
  unsigned useActiveIndex : 1;
  unsigned useStrStorage : 1;
}entry_t;

typedef struct{
  entry_t* entries;
  uint32_t size;
  uint32_t selectedIndex;
  uint32_t activeIndex;
}menuPage_t;

extern menuPage_t main_page;

void setActivePage(void* data, type_tip none);
void renderMenu(void);
void printEntry(entry_t* e);

void decrementEntry(entry_t* e);
void incrementEntry(entry_t* e);
void menuCommand(menuCommands_e command);


#endif
