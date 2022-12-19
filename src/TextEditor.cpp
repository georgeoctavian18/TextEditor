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
    struct rectangle headerBox = header[0].rect;
    headerBox.width = WIDTH;
    struct rectangle statusBarBox = {0, HEIGHT - 30, WIDTH, 30};
    createSlider(lineSlider, WIDTH-30, headerBox.y + headerBox.height, 30, HEIGHT - (headerBox.y + headerBox.height + statusBarBox.height), 0, 0, 0, 1);
    struct rectangle textBox = { 0, (headerBox.y + headerBox.height), WIDTH - lineSlider.rect.width, HEIGHT - (headerBox.y + headerBox.height + statusBarBox.height) };


    vector <string> Lines, CopiedLines;
    vector <int> EnterLines, EnterLinesCopied;
    stack <vector <string>> StackLines;
    stack <vector <int>>  StackEnterLines;
    stack <pair<int, int>> StackLinCol;
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
    bool KeepSelect;


    //InitLine(CurrLine, Lines);

    while (!windowShouldClose)
    {

        //PREUPDATE
        updateFlags();

        //UPDATE

        //keyboard();

        if (kbhit())
        {
            KeepSelect = false;
            Ch = getch();
            cout<<(int)Ch<<endl;
            switch (Ch)
            {
            case ESC:
                windowShouldClose = true;
                break;
            case 0:
                Command = getch();
                SpecialKey(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Command, CharsPerLine, Lines, EnterLines, CopiedLines, EnterLinesCopied, StackLines, StackEnterLines, StackLinCol, WordWrap, KeepSelect);
                break;
            case CR:
                EnterKey(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, WordWrap);
                break;
            case TAB:
               TabKey(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, WordWrap);
                break;
            case BS:
                Deletion(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol, WordWrap);
                break;
            case CTRLX:
                Cut(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, CopiedLines, EnterLinesCopied, StackLines, StackEnterLines, StackLinCol, WordWrap, KeepSelect);
                break;
            case CTRLC:
                Copy(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Lines, EnterLines, CopiedLines, EnterLinesCopied, KeepSelect);
                break;
            case CTRLV:
                Paste(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, CopiedLines, EnterLinesCopied, StackLines, StackEnterLines, StackLinCol, WordWrap);
                break;
            case CTRLZ:
                Undo(CurrLine, CurrCol, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol);
                break;
            case CTRLA:
                SelectAll(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Lines, KeepSelect);
                break;
           default:
               Insertion(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Ch, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol, WordWrap);
            }

            a = c = PosX + CurrCol * CHAR_DIST;
            b = PosY + CurrLine * LINE_WIDTH, d = PosY + (CurrLine + 1) * LINE_WIDTH;

            lastCursorChanged=millis();
            movePage(CurrLine, CurrCol, LineBeginFrame, ColBeginFrame, LineEndFrame, ColEndFrame, RowsPerFrame, CharsPerLine, Lines);
            if (!KeepSelect)
            {
                SelectBeginLine = CurrLine;
                SelectBeginCol = CurrCol;
            }
        }
        /*
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
        }*/
        

        clickedOnHeader=false;
        if (header[0].isSelected)
        {
            if(isClicked(header[0].options[4]))
                windowShouldClose = true;
        }
        if (header[1].isSelected)
        {
            if(isClicked(header[1].options[0]))
                clickedOnHeader=true, Undo(CurrLine, CurrCol, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol);
            if(isClicked(header[1].options[1]))
                clickedOnHeader=true, Cut(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, CopiedLines, EnterLinesCopied, StackLines, StackEnterLines, StackLinCol, WordWrap, KeepSelect);
            if(isClicked(header[1].options[2]))
                clickedOnHeader=true, Copy(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Lines, EnterLines, CopiedLines, EnterLinesCopied, KeepSelect);
            if(isClicked(header[1].options[3]))
                clickedOnHeader=true, Paste(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, CopiedLines, EnterLinesCopied, StackLines, StackEnterLines, StackLinCol, WordWrap);
            if(isClicked(header[1].options[4]))
                clickedOnHeader=true, Deletion(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol, WordWrap);
            if(isClicked(header[1].options[10]))
                clickedOnHeader=true, SelectAll(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Lines, KeepSelect);
        }

        if (header[2].isSelected && isClicked(header[2].options[0]))
        {
            clickedOnHeader=true;
            WordWrap = !WordWrap;
            while (!StackLines.empty())
            {
                StackLines.pop();
                StackEnterLines.pop();
                StackLinCol.pop();
            }

            if (WordWrap)
            {
                DoWordWrap(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines);
                LineBeginFrame = 0, LineEndFrame = RowsPerFrame - 1;
                ColBeginFrame = 0, ColEndFrame = CharsPerLine;
            }
            else
                UndoWordWrap(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines);
            lastCursorChanged = millis();
            movePage(CurrLine, CurrCol, LineBeginFrame, ColBeginFrame, LineEndFrame, ColEndFrame, RowsPerFrame, CharsPerLine, Lines);
            SelectBeginLine = CurrLine;
            SelectBeginCol = CurrCol;
            header[2].isSelected = false;
        }

        clickDropdown(header, nr_header);
        if (!clickedOnHeader)
            clickOnText(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, LineBeginFrame, LineEndFrame, ColBeginFrame, ColEndFrame, Lines, textBox);

        lineSlider.maxState = max(0, (int)Lines.size()-RowsPerFrame);
        lineSlider.state = LineBeginFrame;
        clickSlider(lineSlider);
        LineBeginFrame = lineSlider.state;
        LineEndFrame = LineBeginFrame + (RowsPerFrame - 1);

        //AFTERUPDATE
        clearFlags();

        //FRAME
        background(WHITE);

        PrintText(PosX, PosY, LineBeginFrame, LineEndFrame, ColBeginFrame, ColEndFrame, SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Lines);

        if (LineBeginFrame <= CurrLine && CurrLine <= LineEndFrame && ColBeginFrame <= CurrCol && CurrCol <= ColEndFrame)
            if ((millis() - lastCursorChanged) / 500 % 2 == 0 || (millis() - lastCursorChanged) / 1000 > 5)
                PrintCursor(PosX + (CurrCol - ColBeginFrame) * CHAR_DIST, PosY + (CurrLine - LineBeginFrame) * LINE_WIDTH,
                            PosX + (CurrCol - ColBeginFrame) * CHAR_DIST, PosY + (CurrLine - LineBeginFrame + 1) * LINE_WIDTH, BLACK);

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
        statusBar(CurrLine, CurrCol, Lines, EnterLines, statusBarBox);
        //filledCircle(mousex(), mousey(), 5, (isMouseClicked ? COLOR(0, 255, 0) : COLOR(255, 0, 0)), COLOR(0, 255, 0));

        //AFTERFRAME
        swapbuffers();
    }

    closegraph();
    return 0;
}
