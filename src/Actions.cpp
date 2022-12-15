#include "Actions.h"



void Deletion(int selectBeginLine, int selectBeginCol, int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, bool wordWrap)
{
    if (selectBeginLine > currLine || (selectBeginLine == currLine && selectBeginCol > currCol))
    {
        swap(selectBeginLine, currLine);
        swap(selectBeginCol, currCol);
    }
    else if (selectBeginLine == currLine && selectBeginCol == currCol)
    {
        BackspaceKey(currLine, currCol, charsPerLine, lines, enterLines, wordWrap);
        return;
    }

    while (currLine != selectBeginLine || currCol != selectBeginCol)
        BackspaceKey(currLine, currCol, charsPerLine, lines, enterLines, wordWrap);

}


void Insertion(int selectBeginLine, int selectBeginCol, int& currLine, int& currCol, int charsPerLine, char ch, vector <string>& lines, vector <int>& enterLines, bool wordWrap)
{
    if (selectBeginLine > currLine || (selectBeginLine == currLine && selectBeginCol > currCol))
    {
        swap(selectBeginLine, currLine);
        swap(selectBeginCol, currCol);
    }

    while (currLine != selectBeginLine || currCol != selectBeginCol)
        BackspaceKey(currLine, currCol, charsPerLine, lines, enterLines, wordWrap);

    InsertKey(currLine, currCol, charsPerLine, ch, lines, enterLines, wordWrap);
}