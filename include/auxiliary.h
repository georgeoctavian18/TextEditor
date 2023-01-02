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

void statusBar(int CurrLine, int CurrCol, vector<string>& lines, vector<int>& enterLines, char fileName[], struct rectangle box, bool isSaved, palette* theme)
{
    filledRect(box.x, box.y, box.width - 1, box.height - 1, theme->background, theme->contrast);
    int i, lin, col, maxi;
    lin = 0;
    maxi = -1;
    for (i = 0; i < (int)enterLines.size(); i++)
        if (enterLines[i] < CurrLine)
        {
            lin++;
            if (enterLines[i] > maxi)
                maxi = enterLines[i];
        }
    col = 0;
    for (i = maxi + 1; i < CurrLine; i++)
        col += lines[i].size();
    col += CurrCol;

    bgiout << "Lin " << lin + 1 << ", Col " << col + 1 << "   " << fileName << (!isSaved?'*':'\0');
    setbkcolor(theme->background);
    outstreamxy(10, box.y + 1);
}

void movePage(int CurrLine, int CurrCol, int &LineBeginFrame, int &ColBeginFrame, int &LineEndFrame, int &ColEndFrame, int RowsPerFrame, int CharsPerLine, vector <string>& Lines)
{
    if (!Lines.size()) return;

    if (CurrLine > LineEndFrame)
        LineBeginFrame = CurrLine - (RowsPerFrame - 1), LineEndFrame = CurrLine;
    else if (CurrLine < LineBeginFrame)
        LineBeginFrame = CurrLine, LineEndFrame = CurrLine + (RowsPerFrame - 1);

    if (CurrCol > ColEndFrame)
        ColBeginFrame = CurrCol - CharsPerLine, ColEndFrame = CurrCol;
    else if (CurrCol <= ColBeginFrame)
    {
        if (CurrCol <= Lines[CurrLine].size())
            ColBeginFrame = max(0, ColBeginFrame - CharsPerLine), ColEndFrame = max(CharsPerLine, ColEndFrame - CharsPerLine);
        else ColBeginFrame = CurrCol, ColEndFrame--;
    }

    if (LineEndFrame > Lines.size()-1 && LineBeginFrame>0)
        LineEndFrame = Lines.size()-1, LineBeginFrame =  LineEndFrame - (RowsPerFrame - 1);
}

unsigned startTime, lastCursorChanged, lastMouseClicked;

unsigned millis() {
    using namespace std::chrono;
    return static_cast<unsigned>(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
}

#endif // AUXILIARY_H_INCLUDED
