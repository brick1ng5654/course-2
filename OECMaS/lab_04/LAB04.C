#include <stdio.h>
#include <conio.h>
#include <dos.h>

int my_getch() { // function of reading the key
    union REGS regs;
    regs.h.ah = 0x00; // set the function 00h int16h interuption to read the key
    int86(0x16, &regs, &regs); // call int16h intreuption
    return regs.x.ax;  // return external code from register ax
}

int my_kbhit() { // function of pressing the key
    union REGS regs;
    regs.h.ah = 0x01; // set the function 01h int16 interuption to check is key pressed
    int86(0x16, &regs, &regs); // call int16h interuption
    return !(regs.x.flags & 0x40); // check zf flag (if 1 - key not pressed)
}

int main() {
    int x1 = 20, y1 = 5, x2 = 60, y2 = 15; // window coordinates
    int screen_x, screen_y, key = 0; // coordinates of * and value of entered key
    clrscr();
    textbackground(0);
    window(x1, y1, x2, y2); // set the window with coordinates (x1, y1) to (x2, y2)
    screen_x = (x2 - x1) / 2; // middle x coordinates
    screen_y = (y2 - y1) / 2; // middle y coordinates
    gotoxy(screen_x, screen_y); // cursor to middle coordinates
    cprintf("*");
    gotoxy(screen_x, screen_y); // go cursor under the symbol

    while (key != 0x011B) {  // 0x011B - key ESC
        key = my_getch();
        if (key == 0x3C00) {  // F2
            while (wherex() < (x2 - x1)) { // while right border not reached
                if (my_kbhit()) { // if we interupt the move
                    key = my_getch();
                    if (key == 0x3B00) {  // F1 - to move will be stopped
                        key = 0; 
                        break;
                    } else if (key == 0x011B) return 0;  // ESC
                }
                cprintf(" *");
                gotoxy(wherex() - 1, wherey()); // go cursor under the symbol
                delay(100);
            }
        }
        if (key == 0x3B00) {  // 0x3B00 - key F1
            while (wherex() > 1) { // while left border not reached
                if (my_kbhit()) { // if we interupt the move
                    key = my_getch();
                    if (key == 0x3C00) break;  // F2
                    else if (key == 0x011B) return 0;  // ESC
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
