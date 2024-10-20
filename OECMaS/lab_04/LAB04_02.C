#include <stdio.h>
#include <conio.h>
#include <dos.h>

int main(){
	int x1=20, y1=5, x2=60, y2=15; // window coordinates
	int screen_x, screen_y, key = 0; // coordinates of * and value of entered key
	clrscr();
	textbackground(0);
	window(x1, y1, x2, y2); // Set the window with coordinates (x1, y1) to (x2, y2)
	screen_x = (x2 - x1) / 2; // middle x coordinates
    screen_y = (y2 - y1) / 2; // middle y coordinates
    gotoxy(screen_x, screen_y); // cursor to middle coordinates
	cprintf("*");
    gotoxy(screen_x, screen_y); // go cursor under the symbol

	while(key != 27){ // 27 - key ESC
		key = getch();
		if(key == 60){ // F2
			while(wherex()<(x2-x1)){ // while right border not reached
				if(kbhit()){ // if we interupt the move
					key = getch();
					if(key == 59){ // F1 - to move will be stopped
						key = 0;
						break;
					}
					else if (key == 27) return 0; // ESC
				}
				cprintf(" *");
				gotoxy(wherex() - 1, wherey()); // go cursor under the symbol
				delay(100);
			}
		}
		if(key == 59){ // F1
			while(wherex()>1){ // while left border npt reached
				if(kbhit()){ // if we interupt the move
					key = getch();
					if(key == 60) break; // F2 - to move will be stopped
					else if (key == 27) return 0; // ESC
				}
				cprintf(" "); // change current symbol on empty
                gotoxy(wherex() - 2, wherey()); // get back on 2 symbols (cause cprintf(" ") change position of cursor on +1)
                cprintf("*");
                gotoxy(wherex() - 1, wherey()); // go cursor under the symbol
				delay(100);
			}
		}
	}
	return 0;
}