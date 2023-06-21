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
al = {"À" : "\\x81","Á" : "\\x82","Â" : "\\x83","Ã" : "\\x84","Ä" : "\\x85","Å" : "\\x86","Æ" : "\\x87","Ç" : "\\x88","È" : "\\x89","É" : "\\x8A","Ê" : "\\x8B","Ë" : "\\x8C","Ì" : "\\x8D","Í" : "\\x8E","Î" : "\\x8F","Ï" : "\\x90","Ð" : "\\x91","Ñ" : "\\x92","Ò" : "\\x93","Ó" : "\\x94","Ô" : "\\x95","Õ" : "\\x96","Ö" : "\\x97","×" : "\\x98","Ø" : "\\x99","Ù" : "\\x9A","Ú" : "\\x9B","Û" : "\\x9C","Ý" : "\\x9D","Ü" : "\\x9E","Þ" : "\\x9F","ß" : "\\xA0","à" : "\\xA1","á" : "\\xA2","â" : "\\xA3","ã" : "\\xA4","ä" : "\\xA5","å" : "\\xA6","æ" : "\\xA7","ç" : "\\xA8","è" : "\\xA9","é" : "\\xAA","ê" : "\\xAB","ë" : "\\xAC","ì" : "\\xAD","í" : "\\xAE","î" : "\\xAF","ï" : "\\xB0","ð" : "\\xB1","ñ" : "\\xB2","ò" : "\\xB3","ó" : "\\xB4","ô" : "\\xB5","õ" : "\\xB6","ö" : "\\xB7","÷" : "\\xB8","ø" : "\\xB9","ù" : "\\xBA","ú" : "\\xBB","û" : "\\xBC","ü" : "\\xBD","ý" : "\\xBE","þ" : "\\xBF","ÿ" : "\\xC0", " ":" ",".":"."}


def r2c(str):
  s = [al[c] for c in str]
  print(''.join(s))
  pyperclip.copy(''.join(s))
  return s

*/


#endif