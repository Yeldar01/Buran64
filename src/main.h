#ifndef _MAIN_H_
#define _MAIN_H_

#include <mega64a.h>
#include <delay.h>
#include <glcd.h>
#include <font5x7.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <eeprom.h>
#include <io.h>
#include <interrupt.h>
#include <stdint.h>
#include <math.h>

extern uint32_t time_zero;
extern uint8_t clapan_out_monitor;



//void glcd_outtextxy_edited(unsigned char x, unsigned char y, char *str);
//void setMenu(unsigned char m);
//void WorkStop(void);


/*===PYTHON====
#russian string to escape codes 
import pyperclip
al = {"�" : "\\x81","�" : "\\x82","�" : "\\x83","�" : "\\x84","�" : "\\x85","�" : "\\x86","�" : "\\x87","�" : "\\x88","�" : "\\x89","�" : "\\x8A","�" : "\\x8B","�" : "\\x8C","�" : "\\x8D","�" : "\\x8E","�" : "\\x8F","�" : "\\x90","�" : "\\x91","�" : "\\x92","�" : "\\x93","�" : "\\x94","�" : "\\x95","�" : "\\x96","�" : "\\x97","�" : "\\x98","�" : "\\x99","�" : "\\x9A","�" : "\\x9B","�" : "\\x9C","�" : "\\x9D","�" : "\\x9E","�" : "\\x9F","�" : "\\xA0","�" : "\\xA1","�" : "\\xA2","�" : "\\xA3","�" : "\\xA4","�" : "\\xA5","�" : "\\xA6","�" : "\\xA7","�" : "\\xA8","�" : "\\xA9","�" : "\\xAA","�" : "\\xAB","�" : "\\xAC","�" : "\\xAD","�" : "\\xAE","�" : "\\xAF","�" : "\\xB0","�" : "\\xB1","�" : "\\xB2","�" : "\\xB3","�" : "\\xB4","�" : "\\xB5","�" : "\\xB6","�" : "\\xB7","�" : "\\xB8","�" : "\\xB9","�" : "\\xBA","�" : "\\xBB","�" : "\\xBC","�" : "\\xBD","�" : "\\xBE","�" : "\\xBF","�" : "\\xC0", " ":" ",".":"."}


def r2c(str):
  s = [al[c] for c in str]
  print(''.join(s))
  pyperclip.copy(''.join(s))
  return s

*/


#endif