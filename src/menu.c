#include "menu.h"
#include "print.h"
#include <graphics.h>

#if (1)
#undef PRINT
#define PRINT(...)
#endif




menuPage_t* activePage = 0;

void setActivePage(void* data, type_tip none){
  (void)none;
  activePage = (menuPage_t*)data;
  activePage->selectedIndex = activePage->activeIndex;
}

int str_length(char str[]) {
  int count; 
  for (count = 0; str[count] != '\0'; ++count);
  return count*6; 
}

void dataAssign(entry_t *entry){
  void* data = entry->data.v;
  switch (entry->type){
  case void_e:
  break;
  case char_e:
    entry->prev_data.ch = *((char*)data);
  break;
  case int8_t_e:
      entry->prev_data.i8 = *((int8_t*)data);
      //glcd_printf(" %3d", *((int8_t*)data));
  break;
  case int16_t_e:
      entry->prev_data.i16 = *((int16_t*)data);
      //glcd_printf(" %3d", *((int16_t*)data));
  break;
  case int32_t_e:
      entry->prev_data.i32 = *((int32_t*)data);
      //glcd_printf(" %3d", *((int32_t*)data));
  break;
  case uint8_t_e:
    if(entry->useStrStorage){
      PRINT("storage = %s\r\n", entry->str_storage[*(entry->data.u8)]);
      PRINT("pointer = %d\r\n", *(entry->data.u8));
      //entry->str_storage = entry->str_storage[*entry->data.u8];
      //glcd_printf(" %s", entry->str_storage[*entry->data.u8]);
    }
    else{
      entry->prev_data.u8 = *entry->data.u8;
      //glcd_printf(" %3d", *entry->data.u8);
    }
  break;
  case uint16_t_e:
      entry->prev_data.u16 = *((uint16_t*)data);
      //glcd_printf(" %3d", *((uint16_t*)data));
  break;
  case uint32_t_e:
      entry->prev_data.u32 =*((uint32_t*)data);
      //glcd_printf(" %3d", *((uint32_t*)data));
  break;
  case float_e:
      entry->prev_data.f = *((float*)data);
  // glcd_printf(" %3.0f", *((float*)data));
  break;
  case str_e:
      entry->prev_data.str =entry->data.str;
      //glcd_printf(" %s", entry->data.str);
  break;                 
}
}

void renderMenu(void){
  int i,x,y,py;
  static void* data;
  entry_t* entry;
  uint32_t  index_selected,index_1stline,index,data_index;
  static uint32_t prev_index = 1;
  static uint32_t prev_index_1stline = 0;
  static menuPage_t* prev_activePage = 0; 
  int fstline = 10;
  //clear   
  PRINT(">>>renderMenu\r\n");
  if(!activePage){
      PRINT("empty page!\r\n");
      PRINT("<<<renderMenu\r\n");
      return;
  }

  //1st line

  index_1stline = (activePage->selectedIndex / MENU_LINES) * MENU_LINES;
  
  if(index_1stline != prev_index_1stline || prev_activePage != activePage){
    glcd_setfillcolor(0);
    glcd_fillrectround(0,fstline,128,LINE_HEIGHT*MENU_LINES,0);
    for(i = 0; i < MENU_LINES; i++){
      y=i*LINE_HEIGHT + fstline;
      index = index_1stline + i;
      entry = &activePage->entries[index];   
      if(index_1stline != prev_index_1stline || prev_activePage != activePage){
        if(index >= activePage->size){
            break;
        }
        glcd_printfxy (7, y, "%s",  entry->text);
        //glcd_printfxy (90, y, "%d", str_length(entry->text));
      }
      if(activePage->entries[index].displayData){      
        //PRINT("displaying %s = %d (%s) type 0x%X disp 0x%X\r\n",  entry->text, *(int16_t*)data, entry->data.str, entry->type, entry->displayData);
        x=str_length(entry->text)+15;
        data = entry->data.v;
        switch (entry->type){
          case void_e:
          break;
          case char_e:
              //glcd_printfxy(x, y,"%3d", *((char*)data));
              glcd_printfxy(x,y," %3d", *((char*)data));
          break;
          case int8_t_e:
              glcd_printfxy(x, y,"%3d", *((int8_t*)data));
              //glcd_printf(" %3d", *((int8_t*)data));
          break;
          case int16_t_e:
              glcd_printfxy(x, y,"%3d", *((int16_t*)data));
              //glcd_printf(" %3d", *((int16_t*)data));
          break;
          case int32_t_e:
              glcd_printfxy(x, y,"%3d", *((int32_t*)data));
              //glcd_printf(" %3d", *((int32_t*)data));
          break;
          case uint8_t_e:
              if(entry->useStrStorage){
                PRINT("storage = %s\r\n", entry->str_storage[*(entry->data.u8)]);
                PRINT("pointer = %d\r\n", *(entry->data.u8));
                glcd_printfxy(x, y,"%s", entry->str_storage[*entry->data.u8]);
                //glcd_printf(" %s", entry->str_storage[*entry->data.u8]);
              }
              else{
                glcd_printfxy(x, y,"%3d", *entry->data.u8);
                //glcd_printf(" %3d", *entry->data.u8);
              }
          break;
          case uint16_t_e:
              glcd_printfxy(x, y,"%3d", *((uint16_t*)data));
              //glcd_printf(" %3d", *((uint16_t*)data));
          break;
          case uint32_t_e:
              glcd_printfxy(x, y,"%3d", *((uint32_t*)data));
              //glcd_printf(" %3d", *((uint32_t*)data));
          break;
          case float_e:
              glcd_printfxy(x, y,"%3.0f", *((float*)data));
          // glcd_printf(" %3.0f", *((float*)data));
          break;
          case str_e:
              glcd_printfxy(x, y,"%s", entry->data.str);
              //glcd_printf(" %s", entry->data.str);
          break;                 
        }
        dataAssign(entry);
      }
    }
    prev_index=1;
  }
  
  index_selected = activePage->selectedIndex%MENU_LINES;
  
  if(index_selected!=prev_index){
    y=index_selected*LINE_HEIGHT + fstline;
    py=prev_index*LINE_HEIGHT + fstline;
    glcd_printfxy (0, y, ">");
    glcd_printfxy (0, py, " ");
  }


  data_index = index_selected+index_1stline;
  if(activePage->entries[data_index].displayData){
    entry = &activePage->entries[data_index];
    x=str_length(entry->text)+15;
    y=index_selected*LINE_HEIGHT + fstline;
    data = entry->data.v;
    switch (entry->type){
      case void_e:
      break;
      case char_e:
          if(entry->prev_data.i16 != *((char*)data)){
            //glcd_printfxy(x, y,"%3d", *((char*)data));
            glcd_printfxy(x,y," %3d", *((char*)data));
          }
      break;
      case int8_t_e:
          if(entry->prev_data.i16 != *((int8_t*)data)){
            glcd_printfxy(x, y,"%3d", *((int8_t*)data));
            //glcd_printf(" %3d", *((int8_t*)data));
          } 
      break;
      case int16_t_e:
          if(entry->prev_data.i16 != *((int16_t*)data)){
            glcd_printfxy(x, y,"%3d", *((int16_t*)data));
            entry->prev_data.i16 = *((int16_t*)data);
          }
          //glcd_printf(" %3d", *((int16_t*)data));
      break;
      case int32_t_e:
          if(entry->prev_data.i32 != *((int32_t*)data)){
              glcd_printfxy(x, y,"%3d", *((int32_t*)data));
              //glcd_printf(" %3d", *((int32_t*)data));
          }
      break;
      case uint8_t_e:
          if(entry->prev_data.i8 != *((int8_t*)data)){
            if(entry->useStrStorage){
              PRINT("storage = %s\r\n", entry->str_storage[*(entry->data.u8)]);
              PRINT("pointer = %d\r\n", *(entry->data.u8));
              glcd_printfxy(x, y,"%s", entry->str_storage[*entry->data.u8]);
              //glcd_printf(" %s", entry->str_storage[*entry->data.u8]);
            }
            else{
              glcd_printfxy(x, y,"%3d", *entry->data.u8);
              //glcd_printf(" %3d", *entry->data.u8);
            }
          }
      break;
      case uint16_t_e:
          if(entry->prev_data.u16 != *((uint16_t*)data)){
            glcd_printfxy(x, y,"%3d", *((uint16_t*)data));
            //glcd_printf(" %3d", *((uint16_t*)data));
          }
      break;
      case uint32_t_e:
          if(entry->prev_data.u32 != *((uint32_t*)data)){
            glcd_printfxy(x, y,"%3d", *((uint32_t*)data));
            //glcd_printf(" %3d", *((uint32_t*)data));
          }
      break;
      case float_e:
        if(entry->prev_data.f != *((float*)data)){
          glcd_printfxy(x, y,"%3.0f", *((float*)data));
          // glcd_printf(" %3.0f", *((float*)data));
        }
      break;
      case str_e:
        if(entry->prev_data.str != entry->data.str){
          glcd_printfxy(x, y,"%s", entry->data.str);
          //glcd_printf(" %s", entry->data.str);
        }
      break;                 
    }
    dataAssign(entry);
  }

  prev_index_1stline = index_1stline;
  prev_activePage = activePage;
  prev_index=index_selected;
  PRINT("<<<renderMenu\r\n");
}

void menuCommand(menuCommands_e command){            
    entry_t* entry = &(activePage->entries[activePage->selectedIndex]);
    switch(command){
        case nothing:
        break;
        case up_e:
            if(activePage->selectedIndex == 0){
                activePage->selectedIndex = activePage->size - 1; 
            }
            else{
                activePage->selectedIndex--;
                
            }
            break;
      case down_e:
          if(activePage->selectedIndex < (activePage->size - 1)){
              activePage->selectedIndex++;
          }
          else{
              activePage->selectedIndex = 0;
          }
          break;
      case right_e:
          if(entry->right_callback)
            entry->right_callback(entry);
      break;   
      case left_e:
        if(entry->left_callback)
           entry->left_callback(entry);
      break;
      case left_right_e:
           if(entry->left_right_callback) 
             entry->left_right_callback(entry);
      break;
      case enter_e:
          if(entry->useActiveIndex){
              activePage->activeIndex = activePage->selectedIndex;
          }
          if(entry->sel_callback)
            entry->sel_callback(entry);
      break;
    }
    renderMenu();
}



void printMenuInfo(void){
    PRINT("MENU: ");
    PRINT("");
}


void printEntryData(entry_t* e){
    //void* data = e->data.v;
    switch (e->type){
        case void_e:
        break;
        case char_e:
            PRINT(" %3d",  *(e->data.ch));
        break;
        case int8_t_e:
            PRINT(" %3d", *(e->data.i8));
        break;
        case int16_t_e:
            PRINT(" %3d", *(e->data.i16));
        break;
        case int32_t_e:
            PRINT(" %3d", *(e->data.i32));
        break;
        case uint8_t_e:
            PRINT(" %3d", *(e->data.u8));
        break;
        case uint16_t_e:
            PRINT(" %3d", *(e->data.u16));
        break;
        case uint32_t_e:
            PRINT(" %3d", *(e->data.u32));
        break;
        case float_e:
            PRINT(" %3.0f", *(e->data.f));
        break;
        case str_e:
            PRINT(" %s", e->data.str);
        break;                 
    }     
}

void printEntry(entry_t* e){
    PRINT(">>>printEntry\r\n");
    PRINT("text = %s\r\n", e->text);
    PRINT("data = ");
    printEntryData(e); 
    PRINT("\r\n");
    PRINT("type = %d\r\n", e->type);
    PRINT("disp = %d\r\n", e->displayData);
    PRINT("useActId = %d\r\n", e->useActiveIndex);
    PRINT("<<<printEntry\r\n");    
}


void incrementEntry(entry_t* e){
    PRINT(">>>increment\r\n"); 
    switch (e->type)
    {   
        case void_e:
        break;
        case char_e:
            if(*(e->data.ch) < e->max.ch){
                (*(e->data.ch))++;
            }
        break;
        case int8_t_e:
            if(*(e->data.i8) < e->max.i8){
                (*(e->data.i8))++;
            }
        break;
        case int16_t_e: 
            if(*(e->data.i16) < e->max.i16){
                (*(e->data.i16))++;
            }
        break;
        case int32_t_e:
            if(*(e->data.i32) < e->max.i32){
                (*(e->data.i32))++;
            }
        break;
        case uint8_t_e:
            if(*(e->data.u8) < e->max.u8){
                (*(e->data.u8))++;
            }
        break;
        case uint16_t_e:
            if(*(e->data.u16) < e->max.u16){
                (*(e->data.u16))++;
            }
        break;
        case uint32_t_e:
            if(*(e->data.u32) < e->max.u32){
                (*(e->data.u32))++;
            }
        break;
        case float_e:
            if(*(e->data.f) < e->max.f){
                (*(e->data.f))++;
            }
        break;
        case str_e:
        break;    
    }
    PRINT("<<<icrement\r\n");
}


void decrementEntry(entry_t* e){
    PRINT(">>>decrement\r\n");
    switch (e->type)
    {
        case void_e:
        break;
        case char_e:
            if(*(e->data.ch) > e->min.ch){
                (*(e->data.ch))--;
            }
        break;
        case int8_t_e:
            if(*(e->data.i8) > e->min.i8){
                (*(e->data.i8))--;
            }
        break;
        case int16_t_e:
            if(*(e->data.i16) > e->min.i16){
                (*(e->data.i16))--;
            }
        break;
        case int32_t_e:
            if(*(e->data.i32) > e->min.i32){
                (*(e->data.i32))--;
            }
        break;
        case uint8_t_e: 
            PRINT("data = %d min %d max %d\r\n",*(e->data.u8),  e->min.u8,  e->max.u8 );
            if(*(e->data.u8) > e->min.u8){
                PRINT("--\r\n");
                (*(e->data.u8))--;  
                //(*(e->data.u8)) = (*(e->data.u8)) - 1; 
            }
            PRINT("data = %d min %d max %d\r\n",*(e->data.u8),  e->min.u8,  e->max.u8 );
        break;
        case uint16_t_e:
            if(*(e->data.u16) > e->min.u16){
                (*(e->data.u16))--;
            }
        break;
        case uint32_t_e:
            if(*(e->data.u32) > e->min.u32){
                (*(e->data.u32))--;
            }
        break;
        case float_e:
            if(*(e->data.f) > e->min.f){
                (*(e->data.f))--;
            }
        break;
        case str_e:
        break;    
    } 
    PRINT("<<<decrement\r\n");
}