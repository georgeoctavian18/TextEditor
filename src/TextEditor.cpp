#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "Initialization.h"
#include "auxiliary.h"
#include "KeyActions.h"
#include "Print.h"
#include "user_interaction.h"
#include "loading.h"
#include "FileExplorer.h"


int main()
{
    int WindSizeX, WindSizeY;

    SelectWindowSize(WindSizeX, WindSizeY);

    //SETUP
    setOptions(WindSizeX, WindSizeY);
    setupFlags();

    dropdown header[5];
    int nr_header;
    bool clickedOnHeader;

    createHeader(header, nr_header);

    slider lineSlider, colSlider;
    struct rectangle headerBox = header[0].rect;
    headerBox.width = WIDTH;
    struct rectangle statusBarBox = {0, HEIGHT - 30, WIDTH, 30};
    createSlider(lineSlider, WIDTH-25, headerBox.y + headerBox.height, 25, HEIGHT - (headerBox.y + headerBox.height + statusBarBox.height), 0, 0, 0, 1);
    createSlider(colSlider, 0, HEIGHT - statusBarBox.height - lineSlider.rect.width, WIDTH - lineSlider.rect.width, lineSlider.rect.width, 0, 0, 0, 1);
    lineSlider.rect.height -= colSlider.rect.height;
    struct rectangle textBox = { 0, (headerBox.y + headerBox.height), WIDTH - lineSlider.rect.width, HEIGHT - (headerBox.y + headerBox.height + statusBarBox.height + colSlider.rect.height) };

    vector <string> Lines, CopiedLines;
    vector <int> EnterLines, EnterLinesCopied;
    stack <vector <string>> StackLines;
    stack <vector <int>>  StackEnterLines;
    stack <pair<int, int>> StackLinCol;
    char Ch;
    int CurrLine, CurrCol, SelectBeginLine, SelectBeginCol;
    int PosX, PosY, CharsPerLine, RowsPerFrame, Command, Font = COMPLEX_FONT;
    int a, b, c, d;

    Initialize(CurrLine, CurrCol, PosX, PosY, CharsPerLine, RowsPerFrame, a, b, c, d, WindSizeX, WindSizeY);

    SelectBeginLine = CurrLine;
    SelectBeginCol = CurrCol;

    int LineBeginFrame = 0, LineEndFrame = RowsPerFrame - 1;
    int ColBeginFrame = 0, ColEndFrame = CharsPerLine;

    bool WordWrap = false, isSaved = true;
    bool KeepSelect;

    char Path[1024] = "\0", FileName[256]="Untitled";


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
            //cout<<(int)Ch<<endl;
            switch (Ch)
            {
            case ESC:
                windowShouldClose = true;
                break;
            case 0:
                Command = getch();
                if (Command == KEY_F4)
                {
                    SelectWindowSize(WindSizeX, WindSizeY);
                    setOptions(WindSizeX, WindSizeY);
                    setupFlags();
                    CurrLine = 0, CurrCol = 0;
                    PosY = WindSizeY / 10;
                    RowsPerFrame = (WindSizeY - 2 * PosY) / CHAR_HEIGHT;
                    CharsPerLine = (WindSizeX - 4 * PosX) / CHAR_WIDTH;
                    LineBeginFrame = 0, LineEndFrame = RowsPerFrame - 1;
                    ColBeginFrame = 0, ColEndFrame = CharsPerLine;

                    createHeader(header, nr_header);

                    headerBox = header[0].rect;
                    headerBox.width = WIDTH;
                    statusBarBox = { 0, HEIGHT - 30, WIDTH, 30 };
                    createSlider(lineSlider, WIDTH - 25, headerBox.y + headerBox.height, 25, HEIGHT - (headerBox.y + headerBox.height + statusBarBox.height), 0, 0, 0, 1);
                    createSlider(colSlider, 0, HEIGHT - statusBarBox.height - lineSlider.rect.width, WIDTH - lineSlider.rect.width, lineSlider.rect.width, 0, 0, 0, 1);
                    lineSlider.rect.height -= colSlider.rect.height;
                    textBox = { 0, (headerBox.y + headerBox.height), WIDTH - lineSlider.rect.width, HEIGHT - (headerBox.y + headerBox.height + statusBarBox.height + colSlider.rect.height) };
                    colSlider.exists = !WordWrap;
                    lineSlider.rect.height += (colSlider.exists ? -1 : 1) * colSlider.rect.height;
                    textBox.height += (colSlider.exists ? -1 : 1) * colSlider.rect.height;
                }
                else SpecialKey(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Command, CharsPerLine, Lines, EnterLines, CopiedLines, EnterLinesCopied, StackLines, StackEnterLines, StackLinCol, WordWrap, KeepSelect, isSaved, Font);
                break;
            case CR:
                EnterKey(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, WordWrap);
                break;
            case BS:
                Deletion(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol, WordWrap, isSaved);
                break;
            case CTRLX:
                Cut(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, CopiedLines, EnterLinesCopied, StackLines, StackEnterLines, StackLinCol, WordWrap, KeepSelect, isSaved);
                break;
            case CTRLC:
                Copy(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Lines, EnterLines, CopiedLines, EnterLinesCopied, KeepSelect);
                break;
            case CTRLV:
                Paste(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, CopiedLines, EnterLinesCopied, StackLines, StackEnterLines, StackLinCol, WordWrap, isSaved);
                break;
            case CTRLZ:
                Undo(CurrLine, CurrCol, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol, isSaved);
                break;
            case CTRLA:
                SelectAll(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Lines, KeepSelect);
                break;
            case CTRLS:
                SaveFile(CurrLine, CurrCol, CharsPerLine, Path, FileName, Lines, EnterLines, WordWrap, isSaved);
                break;
            case CTRLN:
                NewFile(CurrLine, CurrCol, CharsPerLine, Path, FileName, Lines, EnterLines, WordWrap, isSaved);
                break;
           default:
               Insertion(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Ch, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol, WordWrap, isSaved);
            }

            a = c = PosX + CurrCol * CHAR_WIDTH;
            b = PosY + CurrLine * CHAR_HEIGHT, d = PosY + (CurrLine + 1) * CHAR_HEIGHT;

            lastCursorChanged=millis();
            movePage(CurrLine, CurrCol, LineBeginFrame, ColBeginFrame, LineEndFrame, ColEndFrame, RowsPerFrame, CharsPerLine, Lines);
            if (!KeepSelect)
            {
                SelectBeginLine = CurrLine;
                SelectBeginCol = CurrCol;
            }
        }

        clickedOnHeader=false;
        if (header[0].isSelected)
        {
            if (isClicked(header[0].options[0]))
                clickedOnHeader = true, NewFile(CurrLine, CurrCol, CharsPerLine, Path, FileName, Lines, EnterLines, WordWrap, isSaved), SelectBeginLine = CurrLine, SelectBeginCol = CurrCol;
            if (isClicked(header[0].options[1]))
                clickedOnHeader = true, OpenFile(CurrLine, CurrCol, CharsPerLine, Path, FileName, Lines, EnterLines, WordWrap, isSaved), SelectBeginLine = CurrLine, SelectBeginCol = CurrCol;
            if (isClicked(header[0].options[2]))
                clickedOnHeader = true, SaveFile(CurrLine, CurrCol, CharsPerLine, Path, FileName, Lines, EnterLines, WordWrap, isSaved), SelectBeginLine = CurrLine, SelectBeginCol = CurrCol;
            if (isClicked(header[0].options[3]))
                clickedOnHeader = true, SaveAsFile(CurrLine, CurrCol, CharsPerLine, Path, FileName, Lines, EnterLines, WordWrap, isSaved), SelectBeginLine = CurrLine, SelectBeginCol = CurrCol;
            if(isClicked(header[0].options[4]))
                windowShouldClose = true;
        }
        if (header[1].isSelected)
        {
            if(isClicked(header[1].options[0]))
                clickedOnHeader=true, Undo(CurrLine, CurrCol, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol, isSaved), SelectBeginLine = CurrLine, SelectBeginCol = CurrCol;
            if(isClicked(header[1].options[1]))
                clickedOnHeader=true, Cut(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, CopiedLines, EnterLinesCopied, StackLines, StackEnterLines, StackLinCol, WordWrap, KeepSelect, isSaved), SelectBeginLine = CurrLine, SelectBeginCol = CurrCol;
            if(isClicked(header[1].options[2]))
                clickedOnHeader=true, Copy(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Lines, EnterLines, CopiedLines, EnterLinesCopied, KeepSelect);
            if(isClicked(header[1].options[3]))
                clickedOnHeader=true, Paste(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, CopiedLines, EnterLinesCopied, StackLines, StackEnterLines, StackLinCol, WordWrap, isSaved), SelectBeginLine = CurrLine, SelectBeginCol = CurrCol;
            if(isClicked(header[1].options[4]))
                clickedOnHeader=true, Deletion(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol, WordWrap, isSaved), SelectBeginLine = CurrLine, SelectBeginCol = CurrCol;
            if(isClicked(header[1].options[10]))
                clickedOnHeader=true, SelectAll(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Lines, KeepSelect);
            if (isClicked(header[1].options[11]))
                clickedOnHeader = true, DateTimeKey(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, WordWrap, isSaved), SelectBeginLine = CurrLine, SelectBeginCol = CurrCol;
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

            colSlider.exists = !WordWrap;
            lineSlider.rect.height += (colSlider.exists?-1:1)*colSlider.rect.height;
            textBox.height += (colSlider.exists?-1:1)*colSlider.rect.height;


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
        clickVerticalSlider(lineSlider);
        LineBeginFrame = lineSlider.state;
        LineEndFrame = LineBeginFrame + (RowsPerFrame - 1);

        int maxi = 0;
        for (int i=0; i<Lines.size(); i++)
            if (Lines[i].size()>maxi)
                maxi = Lines[i].size();

        colSlider.maxState = max(0, maxi-CharsPerLine);
        colSlider.state = ColBeginFrame;
        clickHorizontalSlider(colSlider);
        ColBeginFrame = colSlider.state;
        ColEndFrame = ColBeginFrame + CharsPerLine;

        //AFTERUPDATE
        clearFlags();

        //FRAME
        background(WHITE);

        PrintText(PosX, PosY, LineBeginFrame, LineEndFrame, ColBeginFrame, ColEndFrame, SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Font, Lines);
        

        if (LineBeginFrame <= CurrLine && CurrLine <= LineEndFrame && ColBeginFrame <= CurrCol && CurrCol <= ColEndFrame)
            if ((millis() - lastCursorChanged) / 500 % 2 == 0 || (millis() - lastCursorChanged) / 1000 > 5)
                PrintCursor(PosX + (CurrCol - ColBeginFrame) * CHAR_WIDTH, PosY + (CurrLine - LineBeginFrame) * CHAR_HEIGHT,
                            PosX + (CurrCol - ColBeginFrame) * CHAR_WIDTH, PosY + (CurrLine - LineBeginFrame + 1) * CHAR_HEIGHT, BLACK);

        display(header, nr_header);

        if (header[2].isSelected)
            if (WordWrap)
            {
                struct rectangle cb = header[2].options[0].rect;
                int col = header[2].options[0].default_col;
                int lat = cb.height*0.5;
                struct rectangle tick = {cb.x + cb.width + cb.height/2 - lat/2, cb.y + cb.height/2 - lat/2, lat, lat};
                filledRect(tick.x, tick.y, tick.width, tick.height, col, col);
                setcolor(COLOR(0, 0, 255));
                line(tick.x, tick.y, tick.x+tick.width, tick.y+tick.height);
                line(tick.x, tick.y+tick.height, tick.x+tick.width, tick.y);
            }


        display(lineSlider);
        display(colSlider);
        if (!WordWrap)
        {
            int bgcol = COLOR(230, 230, 230);
            filledRect(lineSlider.rect.x, lineSlider.rect.y+lineSlider.rect.height, lineSlider.rect.width-1, statusBarBox.y, bgcol, BLACK);
        }

        statusBar(CurrLine, CurrCol, Lines, EnterLines, FileName, statusBarBox, isSaved);
        //filledCircle(mousex(), mousey(), 5, (isMouseClicked ? COLOR(0, 255, 0) : COLOR(255, 0, 0)), COLOR(0, 255, 0));

        //AFTERFRAME
        swapbuffers();
    }

    closegraph();
    return 0;
}
