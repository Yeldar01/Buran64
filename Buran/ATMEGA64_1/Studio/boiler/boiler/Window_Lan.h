

extern int cursor_lan;

void CursorLan1 (void)
{
	textx=3;texty=2;link (">",1);
	textx=17;texty=2;link ("<",1);
	textx=3;texty=4;link (" ",1);
	textx=17;texty=4;link (" ",1);
	cursor_lan = 1;
}

void CursorLan2 (void)
{
	textx=3;texty=4;link (">",1);
	textx=17;texty=4;link ("<",1);
	textx=3;texty=2;link (" ",1);
	textx=17;texty=2;link (" ",1);
    textx=3;texty=5;link (" ",1);
	textx=17;texty=5;link (" ",1);
	cursor_lan = 2;
}

void CursorLan3 (void)
{
	textx=3;texty=5;link (">",1);
	textx=17;texty=5;link ("<",1);
	textx=3;texty=4;link (" ",1);
	textx=17;texty=4;link (" ",1);
    textx=3;texty=6;link (" ",1);
	textx=17;texty=6;link (" ",1);
	cursor_lan = 3;
}

void CursorLan4 (void)
{
	textx=3;texty=6;link (">",1);
	textx=17;texty=6;link ("<",1);
	textx=3;texty=5;link (" ",1);
	textx=17;texty=5;link (" ",1);
	cursor_lan = 4;
}



void WindowLan(void)
{

    window = 5;
    
    textx=8;texty=0;link ("����",4);
    textx=5;texty=2;link ("  �������  ",11);
    textx=4;texty=4;link ("  ������� 1  ",13);
    textx=4;texty=5;link ("  ������� 2  ",13);
  //  textx=4;texty=6;link ("  ������� 3  ",13);


}