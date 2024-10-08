#include <conio.h>
#include <dos.h>

#define MAX_WIDTH 41 // width of window
#define MAX_HEIGHT 11 // height of window
#define BUFFER_SIZE ((MAX_WIDTH + 1) * (MAX_HEIGHT + 1) * 2 + 1) // MW and MH + 1 just for reserve memory to border, *2 cause every texel contain 2 bytes, and +1 for end string symbol

void scrollWindow(int lines, int x1, int y1, int x2, int y2) {
    union REGS regs;

    regs.h.ah = 0x06;            // Scroll function (upwards)
    regs.h.al = lines;           // Number of lines to scroll
    regs.h.bh = 0x00;            // Attribute of blank lines
    regs.h.ch = y1 - 1;          // Top-left row (include zero index)
    regs.h.cl = x1 - 1;          // Top-left column
    regs.h.dh = y2 - 1;          // Bottom-right row
    regs.h.dl = x2 - 1;          // Bottom-right column

    int86(0x10, &regs, &regs);   // BIOS interrupt 0x10 to perform the scroll
}

int main() {
    int text_color, background_color, string, x1 = 20, y1 = 5, x2 = 60, y2 = 15, count = 1;
    clrscr(); // Clear the entire screen
    window(x1, y1, x2, y2); // Set the window with coordinates (x1, y1) to (x2, y2)

    for (background_color = 0; background_color < 16; background_color++) {  
        for (text_color = 0; text_color < 16; text_color++) {
            textcolor(text_color);            // Set color of symbols
            textbackground(background_color); // Set background color of symbols
            if(count == 10){
                scrollWindow(3, x1, y1, x2, y2);
                count = 7;
                gotoxy(1,count);
                delay(200);
            }

            cprintf("Text color: %d, Background color: %d", text_color, background_color); // Print the line with the current color combination
            count++;
            gotoxy(1, count);
            delay(200);
        }
    }
    getch(); // Wait for user before exiting
    return 0;
}