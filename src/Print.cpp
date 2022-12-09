#include "Print.h"

void PrintCursor(int x1, int y1, int x2, int y2, int color)
{
    setcolor(color);
    line(x1, y1, x2, y2);
}

void PrintText(int posX, int posY, int lineBeginFrame, int lineEndFrame, int colBeginFrame, int colEndFrame, vector <string> lines)
{
    setcolor(BLACK);
    setbkcolor(WHITE);
    settextjustify(LEFT_TEXT, TOP_TEXT);

    char PrintArray[256];
    int yTemp = posY;
    string s;

    for (int i = lineBeginFrame; i < min(lineEndFrame + 1, lines.size()); i++)
    {
        for (int j = colBeginFrame; j < min(colEndFrame, lines[i].size()); j++)
            s.push_back(lines[i][j]);

        StringToArray(s, PrintArray);
        outtextxy(posX, yTemp, PrintArray);
        yTemp += LINE_WIDTH;
    }

}
