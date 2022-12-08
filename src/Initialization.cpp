#include "Initialization.h"



void CharToString(string& s, char c)
{
    s.clear();
    s.push_back(c);
}

void StringToArray(string s, char arr[])
{
    for (int i = 0; i < s.size(); i++)
        arr[i] = s[i];
    arr[s.size()] = '\0';
}

void InitLine(int currLine, vector <string> &lines)
{
    if (currLine + 1 > lines.size())
        lines.push_back("");
}

void Initialize(int& currLine, int& currCol, int& posX, int& posY, int& charsPerLine, int& rowsPerFrame, int& a, int& b, int& c, int& d)
{
    currLine = 0, currCol = 0;
    posX = CHAR_DIST, posY = windSizeY / 10;
    rowsPerFrame = (windSizeY - 2 * posY) / CHAR_HEIGHT;
    charsPerLine = (windSizeX - 4 * posX) / CHAR_DIST;
    a = posX, b = posY, c = posX, d = posY + CHAR_HEIGHT;
}


void setOptions()
{
    initwindow(windSizeX, windSizeY, "Editor de Texte");
    setbkcolor(15);
    bar(0, 0, windSizeX, windSizeY);
    setcolor(0);
    settextstyle(8, HORIZ_DIR, 2);
    setfillstyle(SOLID_FILL, 15);
}
