#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "Initialization.h"
#include "KeyActions.h"
#include "Print.h"
#include "auxiliary.h"
#include "user_interaction.h"


int main()
{
    //cin>>windSizeX>>windSizeY;

    setOptions();

    registermousehandler(WM_LBUTTONDOWN, mousePressed);
    registermousehandler(WM_LBUTTONUP, mouseReleased);
    setactivepage(1);
    setvisualpage(2);

    dropdown header[5];
    char optiontext1[][21] = { "New", "Open", "Save", "Save as", "Exit" };
    char optiontext2[][21] = { "Undo", "Cut", "Copy", "Paste", "Delete", "Find", "Find next", "Find previous", "Replace", "Go to", "Select all", "Time/Date" };
    char optiontext3[][21] = { "Word wrap", "Font" };
    char optiontext4[][21] = { "Zoom in", "Zoom out", "Restore default zoom", "Status bar" };
    char optiontext5[][21] = { "Help" };
    createDropdown(header[0], 0, 0, 150, 50, COLOR(255, 255, 255), COLOR(200, 200, 255), COLOR(0, 0, 255), "File", 1, optiontext1, 5);
    createDropdown(header[1], 150, 0, 150, 50, COLOR(255, 255, 255), COLOR(200, 200, 255), COLOR(0, 0, 255), "Edit", 1, optiontext2, 12);
    createDropdown(header[2], 300, 0, 150, 50, COLOR(255, 255, 255), COLOR(200, 200, 255), COLOR(0, 0, 255), "Format", 1, optiontext3, 2);
    createDropdown(header[3], 450, 0, 150, 50, COLOR(255, 255, 255), COLOR(200, 200, 255), COLOR(0, 0, 255), "View", 1, optiontext4, 4);
    createDropdown(header[4], 600, 0, 150, 50, COLOR(255, 255, 255), COLOR(200, 200, 255), COLOR(0, 0, 255), "Help", 1, optiontext5, 1);
    unsigned prevtime, time_;
    prevtime = time_ = time(NULL);
    int frameCount = 0;



    vector <string> Lines;
    vector <int> EnterLines;
    vector <pair<int, int>> TabsLocation;
    char Ch;
    int CurrLine, CurrCol;
    int PosX, PosY, CharsPerLine;
    int a, b, c, d;


    Initialize(CurrLine, CurrCol, PosX, PosY, CharsPerLine, a, b, c, d);

    InitLine(CurrLine, Lines);

    line(a, b, c, d);

    while (true)
    {

        time_ = time(NULL);
        if (time_ - prevtime == 1)
        {
            cout << frameCount << endl;
            frameCount = 0;
        }
        prevtime = time_;

        //preframe
        isMouseClicked = isMouseClicked_helper;

        //frame
        background(WHITE);

        for (int i = 0; i < 5; i++)
            clickDropdown(header[i]);

        disk(mousex(), mousey(), 10, (isMouseClicked ? COLOR(0, 255, 0) : COLOR(255, 0, 0)), COLOR(0, 255, 0));

        for (int i = 0; i < 5; i++)
            display(header[i]);

        PrintText(Lines, PosX, PosY);
        PrintCursor(PosX + CurrCol * CHAR_DIST, PosY + CurrLine * LINE_WIDTH,
            PosX + CurrCol * CHAR_DIST, PosY + (CurrLine + 1) * LINE_WIDTH, BLACK);

        swapbuffers();

        //afterframe
        if (isMouseClicked)
            isMouseClicked_helper = false;
        frameCount++;

        if (kbhit())
        {

            Ch = getch();

            if (Ch == ESC) break;

            switch (Ch)
            {
            case 0:
                SpecialKey(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, TabsLocation);
                break;
            case CR:
                EnterKey(CurrLine, CurrCol, Lines, EnterLines, TabsLocation, CharsPerLine);
                break;
            case TAB:
                TabKey(CurrLine, CurrCol, Lines, EnterLines, TabsLocation, CharsPerLine);
                break;
            case BS:
                BackspaceKey(CurrLine, CurrCol, Lines, EnterLines, CharsPerLine);
                break;
            default:
                InsertKey(CurrLine, CurrCol, Lines, EnterLines, TabsLocation, CharsPerLine, Ch);
            }

            a = c = PosX + CurrCol * CHAR_DIST;
            b = PosY + CurrLine * LINE_WIDTH, d = PosY + (CurrLine + 1) * LINE_WIDTH;

            //cout << CurrLine << ' ' << CurrCol << '\n';
            delay(15);
        }
    }

    closegraph();
    return 0;
}