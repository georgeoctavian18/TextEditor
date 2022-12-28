#include "Initialization.h"



void CharToString(string& s, char c)
{
    s.clear();
    s.push_back(c);
}

void StringToArray(string& s, char arr[])
{
    for (int i = 0; i < s.size(); i++)
        arr[i] = s[i];
    arr[s.size()] = '\0';
    s.clear();
}

void InitLine(int currLine, vector <string>& lines)
{
    if (currLine + 1 > lines.size())
        lines.push_back("");
}

void Initialize(int& currLine, int& currCol, int& posX, int& posY, int& charsPerLine, int& rowsPerFrame, int& a, int& b, int& c, int& d, int windSizeX, int windSizeY)
{
    currLine = 0, currCol = 0;
    posX = CHAR_WIDTH, posY = windSizeY / 10;
    rowsPerFrame = (windSizeY - 2 * posY) / CHAR_HEIGHT;
    charsPerLine = (windSizeX - 4 * posX) / CHAR_WIDTH;
    a = posX, b = posY, c = posX, d = posY + CHAR_HEIGHT;
}


void setOptions(int windSizeX, int windSizeY)
{
    initwindow(windSizeX, windSizeY, "Text Editor", (getmaxwidth() - windSizeX) / 2, (getmaxheight() - windSizeY) / 2);
    setbkcolor(15);
    bar(0, 0, windSizeX, windSizeY);
    setcolor(0);
    settextstyle(8, HORIZ_DIR, 2);
    setfillstyle(SOLID_FILL, 15);
}