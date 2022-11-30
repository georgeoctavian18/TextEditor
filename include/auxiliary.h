#ifndef AUXILIARY_H_INCLUDED
#define AUXILIARY_H_INCLUDED

#define WIDTH  (float)(getmaxx()+1)
#define HEIGHT (float)(getmaxy()+1)

struct vector2{
    float x, y;
};

struct rectangle{
    float x, y, width, height;
};

bool isInsideRect(vector2 v, struct rectangle r){
    return v.x>=r.x && v.x<r.x+r.width &&
           v.y>=r.y && v.y<r.y+r.height;
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

void disk(float x, float y, float r, int fillColor, int borderColor)
{
    setfillstyle(SOLID_FILL, fillColor);
    setcolor(borderColor);
    fillellipse(x, y, r, r);
}

bool isMouseDown = false;
bool isMouseClicked = false;
bool isMouseUp = true;
bool isMouseClicked_helper = false;

void mousePressed(int x, int y)
{
    isMouseDown = true;
    isMouseUp = false;
    isMouseClicked_helper = true;
}


void mouseReleased(int x, int y)
{
    isMouseDown = false;
    isMouseUp = true;
}

#endif // AUXILIARY_H_INCLUDED
