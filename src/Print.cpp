#include "Print.h"

void PrintCursor(int x1, int y1, int x2, int y2, int color)
{
    setcolor(color);
    line(x1, y1, x2, y2);
}

void PrintText(int posX, int posY, int beginFrame, int endFrame, vector <string> lines)
{
    setcolor(BLACK);
    setbkcolor(WHITE);
    settextjustify(LEFT_TEXT, TOP_TEXT);

    char PrintArray[256];
    int yTemp = posY;

    for (int i = beginFrame; i <= min(endFrame, lines.size() - 1); i++)
    {
        StringToArray(lines[i], PrintArray);
        outtextxy(posX, yTemp, PrintArray);
        yTemp += LINE_WIDTH;
    }

}
