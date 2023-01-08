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
    int WindSizeX = 800, WindSizeY = 600;

    //SETUP
    setOptions(WindSizeX, WindSizeY);
    setupFlags();

    dropdown header[6];
    int nr_header, maxi=0;
    bool clickedOnHeader;
    palette theme;
    slider lineSlider, colSlider;
    struct rectangle headerBox, statusBarBox, textBox;
    int lang_selected=0;

    char projectDir[1024];
    GetCurrentDirectoryA(1024, projectDir);

    loadTheme(projectDir, "data\\light_theme.thm", &theme);
    loadLayout(projectDir, header, nr_header, headerBox, statusBarBox, lineSlider, colSlider, textBox, &theme, lang_selected);

    vector <string> Lines, CopiedLines;
    vector <int> EnterLines, EnterLinesCopied;
    stack <vector <string>> StackLines;
    stack <vector <int>>  StackEnterLines;
    stack <pair<int, int>> StackLinCol;
    char Ch = '\0';
    int CurrLine, CurrCol, SelectBeginLine, SelectBeginCol;
    int PosX, PosY, CharsPerLine, RowsPerFrame, Command, Font = COMPLEX_FONT;
    int a, b, c, d;

    Initialize(CurrLine, CurrCol, PosX, PosY, CharsPerLine, RowsPerFrame, a, b, c, d, WindSizeX, WindSizeY);
    InitLine(CurrLine, Lines);

    SelectBeginLine = CurrLine;
    SelectBeginCol = CurrCol;

    int LineBeginFrame = 0, LineEndFrame = RowsPerFrame - 1;
    int ColBeginFrame = 0, ColEndFrame = CharsPerLine;

    bool ShowStatusBar = true, Resize = false;
    bool WordWrap = false, isSaved = true;
    bool KeepSelect = false;

    char Path[1024] = "\0", FileName[256] = "Untitled";

    while (!windowShouldClose)
    {

        //PREUPDATE
        updateFlags();

        //UPDATE
        Ch=0;
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
                SpecialKey(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Command, CharsPerLine, Lines, EnterLines, CopiedLines, EnterLinesCopied, StackLines, StackEnterLines, StackLinCol, WordWrap, KeepSelect, isSaved, Font);
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
                updateMaxTextLength(maxi, Lines, CharsPerLine, false);
                break;
            case CTRLZ:
                Undo(CurrLine, CurrCol, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol, isSaved);
                updateMaxTextLength(maxi, Lines, CharsPerLine, false);
                break;
            case CTRLA:
                SelectAll(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Lines, KeepSelect);
                break;
            case CTRLS:
                SaveFile(CurrLine, CurrCol, CharsPerLine, Path, FileName, Lines, EnterLines, WordWrap, isSaved);
                break;
            case CTRLO:
                OpenFile(CurrLine, CurrCol, CharsPerLine, Path, FileName, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol, WordWrap, isSaved);
                break;
            case CTRLN:
                NewFile(CurrLine, CurrCol, CharsPerLine, Path, FileName, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol, WordWrap, isSaved);
                break;
           default:
               if (Ch > 26 || Ch == TAB)
               {
                   Insertion(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Ch, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol, WordWrap, isSaved);
                   updateMaxTextLength(maxi, Lines, CharsPerLine, false);
               }
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

            if ((Ch==BS || Ch==CTRLX)&& (StackLines.empty()?false:Lines.size()!= StackLines.top().size()))
                updateMaxTextLength(maxi, Lines, CharsPerLine, false);
        }

        clickedOnHeader=false;
        if (header[0].isSelected)
        {
            if (isClicked(header[0].options[0]))
                clickedOnHeader = true, NewFile(CurrLine, CurrCol, CharsPerLine, Path, FileName, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol, WordWrap, isSaved), SelectBeginLine = CurrLine, SelectBeginCol = CurrCol;
            if (isClicked(header[0].options[1]))
                clickedOnHeader = true, OpenFile(CurrLine, CurrCol, CharsPerLine, Path, FileName, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol, WordWrap, isSaved), SelectBeginLine = CurrLine, SelectBeginCol = CurrCol;
            if (isClicked(header[0].options[2]))
                clickedOnHeader = true, SaveFile(CurrLine, CurrCol, CharsPerLine, Path, FileName, Lines, EnterLines, WordWrap, isSaved), SelectBeginLine = CurrLine, SelectBeginCol = CurrCol;
            if (isClicked(header[0].options[3]))
                clickedOnHeader = true, SaveAsFile(CurrLine, CurrCol, CharsPerLine, Path, FileName, Lines, EnterLines, WordWrap, isSaved), SelectBeginLine = CurrLine, SelectBeginCol = CurrCol;
            if(isClicked(header[0].options[4]))
                windowShouldClose = true;
        }

        if ((header[0].isSelected && clickedOnHeader) || Ch==CTRLS || Ch==CTRLO || Ch==CTRLN)
        {
            updateMaxTextLength(maxi, Lines, CharsPerLine, true);
            movePage(CurrLine, CurrCol, LineBeginFrame, ColBeginFrame, LineEndFrame, ColEndFrame, RowsPerFrame, CharsPerLine, Lines);
            lastCursorChanged=millis();
        }

        if (header[1].isSelected)
        {
            if(isClicked(header[1].options[0]))
            {
                clickedOnHeader = true;
                Undo(CurrLine, CurrCol, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol, isSaved), SelectBeginLine = CurrLine, SelectBeginCol = CurrCol;
                updateMaxTextLength(maxi, Lines, CharsPerLine, false);
            }
            if(isClicked(header[1].options[1]))
            {
                clickedOnHeader = true;
                Cut(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, CopiedLines, EnterLinesCopied, StackLines, StackEnterLines, StackLinCol, WordWrap, KeepSelect, isSaved);
                SelectBeginLine = CurrLine;
                SelectBeginCol = CurrCol;
                if ((StackLines.empty()?false:Lines.size()!= StackLines.top().size()))
                    updateMaxTextLength(maxi, Lines, CharsPerLine, false);
            }
            if(isClicked(header[1].options[2]))
                clickedOnHeader = true, Copy(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Lines, EnterLines, CopiedLines, EnterLinesCopied, KeepSelect);
            if(isClicked(header[1].options[3]))
            {
                clickedOnHeader = true;
                Paste(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, CopiedLines, EnterLinesCopied, StackLines, StackEnterLines, StackLinCol, WordWrap, isSaved);
                SelectBeginLine = CurrLine;
                SelectBeginCol = CurrCol;
                updateMaxTextLength(maxi, Lines, CharsPerLine, false);
            }
            if(isClicked(header[1].options[4]))
            {
                clickedOnHeader = true;
                Deletion(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, StackLines, StackEnterLines, StackLinCol, WordWrap, isSaved), SelectBeginLine = CurrLine, SelectBeginCol = CurrCol;
                if ((StackLines.empty()?false:Lines.size()!= StackLines.top().size()))
                    updateMaxTextLength(maxi, Lines, CharsPerLine, false);
            }
            if(isClicked(header[1].options[5]))
                clickedOnHeader = true, SelectAll(SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Lines, KeepSelect);
            if(isClicked(header[1].options[6]))
            {
                clickedOnHeader = true;
                DateTimeKey(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines, WordWrap, isSaved);
                SelectBeginLine = CurrLine;
                SelectBeginCol = CurrCol;
                updateMaxTextLength(maxi, Lines, CharsPerLine, false);
            }
        }

        if (header[1].isSelected && clickedOnHeader)
        {
            movePage(CurrLine, CurrCol, LineBeginFrame, ColBeginFrame, LineEndFrame, ColEndFrame, RowsPerFrame, CharsPerLine, Lines);
            lastCursorChanged=millis();
        }

        if (header[2].isSelected)
        {
            if (isClicked(header[2].options[0]))
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
                    maxi = 0;
                }
                else
                    UndoWordWrap(CurrLine, CurrCol, CharsPerLine, Lines, EnterLines);
                updateMaxTextLength(maxi, Lines, CharsPerLine, true);


                colSlider.exists = !WordWrap;
                lineSlider.rect.height += (colSlider.exists?-1:1)*colSlider.rect.height;
                textBox.height += (colSlider.exists?-1:1)*colSlider.rect.height;


                lastCursorChanged = millis();
                movePage(CurrLine, CurrCol, LineBeginFrame, ColBeginFrame, LineEndFrame, ColEndFrame, RowsPerFrame, CharsPerLine, Lines);
                SelectBeginLine = CurrLine;
                SelectBeginCol = CurrCol;
                header[2].isSelected = false;
            }
            if (isClicked(header[2].options[1]))
            {
                clickedOnHeader = true;
                ShowStatusBar = !ShowStatusBar;
                colSlider.rect.y += (ShowStatusBar?-1:+1)*statusBarBox.height;
                lineSlider.rect.height += (ShowStatusBar?-1:+1)*statusBarBox.height;
                textBox.height += (ShowStatusBar?-1:+1)*statusBarBox.height;
            }
            if (isClicked(header[2].options[2]))
            {
                clickedOnHeader = true;
                SelectWindowSize(WindSizeX, WindSizeY, theme, lang_selected, projectDir);
                setOptions(WindSizeX, WindSizeY);
                setupFlags();
                ResizeWindow(CurrLine, CurrCol, WindSizeX, WindSizeY, PosX, PosY, RowsPerFrame, CharsPerLine, LineBeginFrame, LineEndFrame, ColBeginFrame, ColEndFrame, WordWrap, Lines, EnterLines);
                settextstyle(Font, HORIZ_DIR, 2);
                loadLayout(projectDir, header, nr_header, headerBox, statusBarBox, lineSlider, colSlider, textBox, &theme, lang_selected);
                Resize = true;
                SelectBeginLine = CurrLine, SelectBeginCol = CurrCol;
            }
        }

        if (header[3].isSelected)
        {
            if (isClicked(header[3].options[0]))
                clickedOnHeader = true, loadTheme(projectDir, "data\\light_theme.thm", &theme);
            if (isClicked(header[3].options[1]))
                clickedOnHeader = true, loadTheme(projectDir, "data\\dark_theme.thm", &theme);
        }

        if (header[4].isSelected)
        {
            if (isClicked(header[4].options[0]))
                clickedOnHeader = true, settextstyle(COMPLEX_FONT, HORIZ_DIR, 2), Font=COMPLEX_FONT;
            if (isClicked(header[4].options[1]))
                clickedOnHeader = true, settextstyle(BOLD_FONT, HORIZ_DIR, 2), Font=BOLD_FONT;
            if (isClicked(header[4].options[2]))
                clickedOnHeader = true, settextstyle(DEFAULT_FONT, HORIZ_DIR, 2), Font=DEFAULT_FONT;
        }

        if (header[4].isSelected && clickedOnHeader)
        {
            resizeHeader(header, nr_header);
        }

        if (header[5].isSelected)
        {
            if (isClicked(header[5].options[0]))
                clickedOnHeader = true, lang_selected = 0, loadLanguage(projectDir, "data\\english.lang", header, nr_header);
            if (isClicked(header[5].options[1]))
                clickedOnHeader = true, lang_selected = 1, loadLanguage(projectDir, "data\\romana.lang", header, nr_header);
        }

        if (header[5].isSelected && clickedOnHeader)
        {
            resizeHeader(header, nr_header);
        }

        clickDropdown(header, nr_header);
        if (!clickedOnHeader)
            clickOnText(PosX, PosY, SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, LineBeginFrame, LineEndFrame, ColBeginFrame, ColEndFrame, Lines, EnterLines, textBox);


        lineSlider.maxState = max(0, (int)Lines.size()-RowsPerFrame);
        lineSlider.state = LineBeginFrame;
        clickVerticalSlider(lineSlider);
        LineBeginFrame = lineSlider.state;
        LineEndFrame = LineBeginFrame + (RowsPerFrame - 1);


        colSlider.maxState = max(0, maxi-CharsPerLine);;
        colSlider.state = ColBeginFrame;
        clickHorizontalSlider(colSlider);

        ColBeginFrame = colSlider.state;
        ColEndFrame = ColBeginFrame + CharsPerLine;
        //AFTERUPDATE
        clearFlags();

        //FRAME
        background(theme.background);

        PrintText(PosX, PosY, LineBeginFrame, LineEndFrame, ColBeginFrame, ColEndFrame, SelectBeginLine, SelectBeginCol, CurrLine, CurrCol, Font, Lines, &theme);


        if (LineBeginFrame <= CurrLine && CurrLine <= LineEndFrame && ColBeginFrame <= CurrCol && CurrCol <= ColEndFrame)
            if ((millis() - lastCursorChanged) / 500 % 2 == 0 || (millis() - lastCursorChanged) / 1000 > 5)
                PrintCursor(PosX + (CurrCol - ColBeginFrame) * CHAR_WIDTH, PosY + (CurrLine - LineBeginFrame) * CHAR_HEIGHT,
                            PosX + (CurrCol - ColBeginFrame) * CHAR_WIDTH, PosY + (CurrLine - LineBeginFrame + 1) * CHAR_HEIGHT, theme.text);

        filledRect(headerBox.x, headerBox.y, headerBox.width, headerBox.height, theme.button_default, theme.button_default);
        display(header, nr_header);

        if (header[2].isSelected)
        {
            displayFlag(header[2].options[0], WordWrap);
            displayFlag(header[2].options[1], ShowStatusBar);
        }

        if (header[3].isSelected)
        {
            displayFlag(header[3].options[0], theme.background==WHITE);
            displayFlag(header[3].options[1], theme.background==BLACK);
        }

        if (header[4].isSelected)
        {
            displayFlag(header[4].options[0], Font==COMPLEX_FONT);
            displayFlag(header[4].options[1], Font==BOLD_FONT);
            displayFlag(header[4].options[2], Font==DEFAULT_FONT);
        }

        if (header[5].isSelected)
        {
            displayFlag(header[5].options[0], lang_selected==0);
            displayFlag(header[5].options[1], lang_selected==1);
        }

        display(lineSlider);
        display(colSlider);
        setcolor(theme.contrast);
        line(headerBox.x, headerBox.y+headerBox.height, headerBox.x+headerBox.width, headerBox.y+headerBox.height);

        if (!WordWrap)
            filledRect(lineSlider.rect.x, lineSlider.rect.y+lineSlider.rect.height, lineSlider.rect.width-1, statusBarBox.y, theme.scroll_background, theme.contrast);

        if (ShowStatusBar)
            statusBar(CurrLine, CurrCol, Lines, EnterLines, FileName, statusBarBox, isSaved, &theme);

        if (Resize)
        {
            Resize = false;
            header[0].isSelected = false;
        }

        //AFTERFRAME
        swapbuffers();
    }

    closegraph();
    return 0;
}
