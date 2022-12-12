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

    slider lineSlider;

    createSlider(lineSlider, WIDTH-30, header[0].rect.y+header[0].rect.height, 30, HEIGHT - (header[0].rect.y+header[0].rect.height), 0, 10, 5, 1);


    vector <string> Lines;
    vector <int> EnterLines;
    char Ch;
    int CurrLine, CurrCol, SelectBeginLine, SelectBeginCol;
    int PosX, PosY, CharsPerLine, RowsPerFrame, Command;
    int a, b, c, d;

    Initialize(CurrLine, CurrCol, PosX, PosY, CharsPerLine, RowsPerFrame, a, b, c, d);

    SelectBeginLine = CurrLine;
    SelectBeginCol = CurrCol;

    int LineBeginFrame = 0, LineEndFrame = RowsPerFrame - 1;
    int ColBeginFrame = 0, ColEndFrame = CharsPerLine;

    bool WordWrap = false;

    //InitLine(CurrLine, Lines);

    while (!windowShouldClose)
    {

        //PREUPDATE
        updateFlags();

        //UPDATE

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
                SpecialKey(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Command, CharsPerLine, Lines, EnterLines, WordWrap);
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
            SelectBeginLine = CurrLine;
            SelectBeginCol = CurrCol;
        }

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

        if (header[2].isSelected && isClicked(header[2].options[0]))
        {
            WordWrap = !WordWrap;
            if (WordWrap) DoWordWrap(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines);
            else UndoWordWrap(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines);
            lastCursorChanged = millis();
            SelectBeginLine = CurrLine;
            SelectBeginCol = CurrCol;
            header[2].isSelected = false;
        }

        //clickedOnHeader=false;
        clickDropdown(header, nr_header, clickedOnHeader);
        //if (!clickedOnHeader)
            clickOnText(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, LineBeginFrame, LineEndFrame, ColBeginFrame, ColEndFrame, Lines);

        

        clickSlider(lineSlider);

        //AFTERUPDATE
        clearFlags();

        //FRAME
        background(WHITE);

        PrintText(PosX, PosY, LineBeginFrame, LineEndFrame, ColBeginFrame, ColEndFrame, SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Lines);

        if ((millis() - lastCursorChanged) / 500 % 2 == 0 || (millis() - lastCursorChanged) / 1000 > 5)
        {
            //PrintCursor(PosX + (SelectBeginCol - ColBeginFrame) * CHAR_DIST, PosY + (SelectBeginLine - LineBeginFrame) * LINE_WIDTH,
              //          PosX + (SelectBeginCol - ColBeginFrame) * CHAR_DIST, PosY + (SelectBeginLine - LineBeginFrame + 1) * LINE_WIDTH, COLOR(255, 100, 200));
            PrintCursor(PosX + (CurrCol - ColBeginFrame) * CHAR_DIST, PosY + (CurrLine - LineBeginFrame) * LINE_WIDTH,
                        PosX + (CurrCol - ColBeginFrame) * CHAR_DIST, PosY + (CurrLine - LineBeginFrame + 1) * LINE_WIDTH, BLACK);
        }

        display(header, nr_header);

        if (header[2].isSelected)
            if (WordWrap)
            {
                struct rectangle cb = header[2].options[0].rect;
                int col = header[2].options[0].hovering_col;
                int lat = cb.height*0.5;
                struct rectangle tick = {cb.x + cb.width + cb.height/2 - lat/2, cb.y + cb.height/2 - lat/2, lat, lat};
                filledRect(tick.x, tick.y, tick.width, tick.height, col, col);
                setcolor(COLOR(0, 0, 255));
                line(tick.x, tick.y, tick.x+tick.width, tick.y+tick.height);
                line(tick.x, tick.y+tick.height, tick.x+tick.width, tick.y);
            }

        display(lineSlider);
        statusBar(CurrLine, CurrCol, Lines, EnterLines);
        filledCircle(mousex(), mousey(), 5, (isMouseClicked ? COLOR(0, 255, 0) : COLOR(255, 0, 0)), COLOR(0, 255, 0));

        //AFTERFRAME
        swapbuffers();
    }

    closegraph();
    return 0;
}
