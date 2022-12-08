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
    int PosX, PosY, CharsPerLine, Command;
    int a, b, c, d;

    Initialize(CurrLine, CurrCol, PosX, PosY, CharsPerLine, a, b, c, d);

    InitLine(CurrLine, Lines);

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
                //delay(15);
                Command = getch();
                SpecialKey(CurrLine, CurrCol, Command, CharsPerLine, Lines, EnterLines);
                break;
            case CR:
                EnterKey(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines);
                break;
            case TAB:
                TabKey(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines);
                break;
            case BS:
                BackspaceKey(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines);
                break;
            default:
                InsertKey(CurrLine, CurrCol, CharsPerLine, Ch, Lines, EnterLines);
            }

            a = c = PosX + CurrCol * CHAR_DIST;
            b = PosY + CurrLine * LINE_WIDTH, d = PosY + (CurrLine + 1) * LINE_WIDTH;


            lastCursorChanged=millis();
            //cout << CurrLine << ' ' << CurrCol << '\n';
            //delay(15);
        }

        //AFTERUPDATE
        clearFlags();

        //FRAME
        background(WHITE);

        PrintText(Lines, PosX, PosY);
        if ((millis()-lastCursorChanged)/500%2==0)
            PrintCursor(PosX + CurrCol * CHAR_DIST, PosY + CurrLine * LINE_WIDTH,
                PosX + CurrCol * CHAR_DIST, PosY + (CurrLine + 1) * LINE_WIDTH, BLACK);

        display(header, nr_header);

        statusBar(CurrLine, CurrCol);
        filledCircle(mousex(), mousey(), 5, (isMouseClicked ? COLOR(0, 255, 0) : COLOR(255, 0, 0)), COLOR(0, 255, 0));

        //AFTERFRAME
        swapbuffers();
    }

    closegraph();
    return 0;
}
