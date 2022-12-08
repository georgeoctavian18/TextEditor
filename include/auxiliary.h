#ifndef AUXILIARY_H_INCLUDED
#define AUXILIARY_H_INCLUDED

#include "Initialization.h"

#define WIDTH  (float)(getmaxx()+1)
#define HEIGHT (float)(getmaxy()+1)

struct vector2 {
    float x, y;
};

struct rectangle {
    float x, y, width, height;
};

bool isInsideRect(vector2 v, struct rectangle r) {
    return v.x >= r.x && v.x < r.x + r.width &&
        v.y >= r.y && v.y < r.y + r.height;
}

float map(float value, float istart, float istop, float ostart, float ostop) {
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

void background(int color)
{
    setfillstyle(SOLID_FILL, color);
    setcolor(color);
    bar(0, 0, WIDTH, HEIGHT);
}

void filledCircle(float x, float y, float r, int fillColor, int borderColor)
{
    setfillstyle(SOLID_FILL, fillColor);
    setcolor(borderColor);
    fillellipse(x, y, r, r);
}

void filledRect(float x, float y, float w, float h, int fillColor, int borderColor)
{
    setfillstyle(SOLID_FILL, fillColor);
    setcolor(borderColor);
    bar(x, y, x + w, y + h);
    rectangle(x, y, x + w, y + h);
}

void statusBar(int lin, int col)
{
    filledRect(0, HEIGHT - 50, 200, 50, COLOR(255, 255, 255), COLOR(0, 0, 0));
    bgiout << "Lin " << lin + 1 << ", Col " << col + 1;
    setbkcolor(WHITE);
    outstreamxy(10, HEIGHT - 40);
}

unsigned startTime, lastCursorChanged;

unsigned millis() {
    using namespace std::chrono;
    return static_cast<unsigned>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
}

#endif // AUXILIARY_H_INCLUDED
