#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "Initialization.h"
#include "auxiliary.h"
#include "KeyActions.h"
#include "Print.h"
#include "user_interaction.h"
#include "loading.h"


int main()
{
    //cin>>windSizeX>>windSizeY;

    //SETUP
    setOptions();
    setupFlags();

    dropdown header[5];
    int nr_header;
    bool clickedOnHeader;

    createHeader(header, nr_header);


    vector <string> Lines;
    vector <int> EnterLines;
    char Ch;
    int CurrLine, CurrCol;
    int PosX, PosY, CharsPerLine, RowsPerFrame, Command;
    int a, b, c, d;

    Initialize(CurrLine, CurrCol, PosX, PosY, CharsPerLine, RowsPerFrame, a, b, c, d);

    int LineBeginFrame = 0, LineEndFrame = RowsPerFrame - 1;
    int ColBeginFrame = 0, ColEndFrame = CharsPerLine;

    bool WordWrap = false;

    //InitLine(CurrLine, Lines);

    while (!windowShouldClose)
    {

        //PREUPDATE
        updateFlags();

        //UPDATE
        //clickedOnHeader=false;
        clickDropdown(header, nr_header, clickedOnHeader);
        if (!clickedOnHeader)
            clickOnText(CurrLine, CurrCol, Lines);
        //keyboard();

        if (kbhit())
        {

            char Ch = getch();

            switch (Ch)
            {
            case ESC:
                windowShouldClose = true;
                break;
            case 0:
                Command = getch();
                SpecialKey(CurrLine, CurrCol, Command, CharsPerLine, Lines, EnterLines, WordWrap);
                break;
            case CR:
                EnterKey(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, WordWrap);
                break;
            case TAB:
                TabKey(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, WordWrap);
                break;
            case BS:
                BackspaceKey(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, WordWrap);
                break;
            default:
                InsertKey(CurrLine, CurrCol, CharsPerLine, Ch, Lines, EnterLines, WordWrap);
            }

            a = c = PosX + CurrCol * CHAR_DIST;
            b = PosY + CurrLine * LINE_WIDTH, d = PosY + (CurrLine + 1) * LINE_WIDTH;

            lastCursorChanged=millis();
        }

        //AFTERUPDATE
        clearFlags();

        //FRAME
        background(WHITE);

        if (CurrLine > LineEndFrame)
            LineBeginFrame++, LineEndFrame++;
        else if (CurrLine < LineBeginFrame)
            LineBeginFrame--, LineEndFrame--;

        if (CurrCol > ColEndFrame)
            ColBeginFrame++, ColEndFrame++;
        else if (CurrCol < ColBeginFrame)
        {
            if (CurrCol == Lines[CurrLine].size())
                ColBeginFrame = 0, ColEndFrame = CharsPerLine;
            else ColBeginFrame--, ColEndFrame--;
        }     


        PrintText(PosX, PosY, LineBeginFrame, LineEndFrame, ColBeginFrame, ColEndFrame, Lines);

        if ((millis() - lastCursorChanged) / 500 % 2 == 0)
            PrintCursor(PosX + (CurrCol - ColBeginFrame) * CHAR_DIST, PosY + (CurrLine - LineBeginFrame) * LINE_WIDTH,
                        PosX + (CurrCol - ColBeginFrame) * CHAR_DIST, PosY + (CurrLine - LineBeginFrame + 1) * LINE_WIDTH, BLACK);

        display(header, nr_header);

        statusBar(CurrLine, CurrCol);
        filledCircle(mousex(), mousey(), 5, (isMouseClicked ? COLOR(0, 255, 0) : COLOR(255, 0, 0)), COLOR(0, 255, 0));

        //AFTERFRAME
        swapbuffers();
    }

    closegraph();
    return 0;
}
