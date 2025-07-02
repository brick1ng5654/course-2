#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <cstdlib>

// Screen dimensions
const int XMAX = 120;
const int YMAX = 50;

// Point class for screen coordinates
class point {
public:
    int x, y;
    point(int a = 0, int b = 0) : x(a), y(b) {}
};

// Screen utility functions
void put_point(int a, int b);    // Draw a point (2 variants)
void put_point(point p) { put_point(p.x, p.y); }

void put_line(int, int, int, int); // Draw a line (2 variants)
void put_line(point a, point b) { put_line(a.x, a.y, b.x, b.y); }

void screen_init();      // Initialize screen
void screen_destroy();   // Clean up screen
void screen_refresh();   // Refresh screen display
void screen_clear();     // Clear screen

#endif // SCREEN_H
