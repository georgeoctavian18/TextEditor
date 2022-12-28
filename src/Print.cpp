#include "Print.h"

void PrintCursor(int x1, int y1, int x2, int y2, int color)
{
    setcolor(color);
    line(x1, y1, x2, y2);
}

void PrintText(int posX, int posY, int lineBeginFrame, int lineEndFrame, int colBeginFrame, int colEndFrame, int selectBeginLine, int selectBeginCol, int selectEndLine, int selectEndCol, int font, vector <string> lines)
{
    setcolor(BLACK);
    setbkcolor(WHITE);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    

    int yTemp = posY;

    if (selectBeginLine > selectEndLine || (selectBeginLine == selectEndLine && selectBeginCol > selectEndCol))
    {
        swap(selectBeginLine, selectEndLine);
        swap(selectBeginCol, selectEndCol);
    }

    for (int i = lineBeginFrame; i < min(lineEndFrame + 1, (int)lines.size()); i++)
    {
        int xTemp = posX;

        for (int j = colBeginFrame; j < min(colEndFrame, (int)lines[i].size()); j++)
        {
            char PrintArray[2] = { lines[i][j], '\0' };
            if (lines[i][j] == '\t')
                PrintArray[0] = ' ';

            if (selectBeginLine == selectEndLine && selectBeginCol == selectEndCol)
                setbkcolor(WHITE);
            else if (selectBeginLine == selectEndLine && i == selectEndLine && j >= selectBeginCol && j < selectEndCol)
                setbkcolor(COLOR(200, 200, 255));
            else if(i == selectBeginLine && j >= selectBeginCol && i != selectEndLine)
                setbkcolor(COLOR(200, 200, 255));
            else if(i != selectBeginLine && i == selectEndLine && j < selectEndCol)
                setbkcolor(COLOR(200, 200, 255));
            else if(i > selectBeginLine && i < selectEndLine && i != selectEndLine)
                setbkcolor(COLOR(200, 200, 255));
            else setbkcolor(WHITE);

            outtextxy(xTemp, yTemp, PrintArray);

            xTemp += CHAR_WIDTH;
        }

        yTemp += CHAR_HEIGHT;
    }

}
