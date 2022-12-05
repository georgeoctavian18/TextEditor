#ifndef USER_INTERACTION_H_INCLUDED
#define USER_INTERACTION_H_INCLUDED

//MOUSE FLAGS

bool isMouseDown = false;
bool isMouseClicked = false;
bool isMouseUp = true;
bool isMouseClicked_helper = false;
bool windowShouldClose = false;

void mousePressed(int x, int y)
{
    isMouseDown = true;
    isMouseUp = false;
    isMouseClicked_helper = true;
}


void mouseReleased(int x, int y)
{
    isMouseDown = false;
    isMouseUp = true;
}

void setupFlags()
{
    lastCursorChanged = startTime = millis();
    registermousehandler(WM_LBUTTONDOWN, mousePressed);
    registermousehandler(WM_LBUTTONUP, mouseReleased);
}

void updateFlags()
{
    isMouseClicked = isMouseClicked_helper;
}

void clearFlags()
{
    if (isMouseClicked)
        isMouseClicked_helper = false;
}

void clickOnText(int& CurrLine, int& CurrCol, vector<string>& Lines)
{
    int textBeginY = HEIGHT / 10;
    int textBeginX = CHAR_WIDTH;
    int headerHeight = 30;
    vector2 mouse = { mousex(), mousey() };
    struct rectangle textScreen = { 0, headerHeight, WIDTH, HEIGHT - headerHeight };
    if (isInsideRect(mouse, textScreen) && isMouseClicked)
    {
        if (mouse.y < textBeginY)
            CurrLine = 0;
        else
            if (mouse.y > textBeginY + (Lines.size()) * CHAR_HEIGHT)
                CurrLine = Lines.size();
            else
                CurrLine = (mouse.y - textBeginY) / CHAR_HEIGHT;

        if (mouse.x < textBeginX)
            CurrCol = 0;
        else
            if (mouse.x > textBeginX + (Lines[CurrLine].size()) * CHAR_WIDTH)
                CurrCol = Lines[CurrLine].size();
            else
                CurrCol = (mouse.x - textBeginX) / CHAR_WIDTH;

        //cout<<CurrLine<<' '<<CurrCol<<endl;
    }
}

//-------------------------------------------------------------------------------------------------------

struct button {
    struct rectangle rect;
    char text[100];
    int col, default_col, hovering_col;
    int padding;
};

void createButton(button& b, float x, float y, float w, float h, int default_col, int hovering_col, int padding) {
    b.rect = { x, y, w, h };
    b.col = b.default_col = default_col;
    b.hovering_col = hovering_col;
    strcpy(b.text, "");
    b.padding = padding;
}


bool isClicked(button& b) {
    vector2 v = { mousex(), mousey() };
    bool res = isInsideRect(v, b.rect);
    b.col = (res ? b.hovering_col : b.default_col);
    return (isMouseClicked && res);
}

void display(button& b) {
    setfillstyle(SOLID_FILL, b.col);
    bar(b.rect.x, b.rect.y, b.rect.x + b.rect.width, b.rect.y + b.rect.height);

    setcolor(BLACK);
    setbkcolor(b.col);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    outtextxy(b.rect.x + b.padding, b.rect.y + b.rect.height / 2 - textheight(b.text) / 2, b.text);
}

//-------------------------------------------------------------------------------------------------------

struct dropdown {
    struct rectangle rect;
    char text[100];
    int col, selected_col, default_col, hovering_col;
    bool isSelected;
    button options[20];
    int nr_options, padding;
};

void createDropdown(dropdown& header, float x, float y, float w, float h, int default_col, int hovering_col, int selected_col, int padding, int nr_options) {
    header.rect = { x, y, w, h };
    header.col = header.default_col = default_col;
    header.hovering_col = hovering_col;
    header.selected_col = selected_col;
    strcpy(header.text, "");
    header.isSelected = false;

    header.nr_options = nr_options;
    header.padding = padding;
    //for (int i=0; i<header.nr_options; i++)
        //createButton(header.options[i], x, y+h+i*h, w, h, default_col, hovering_col);
}


void clickDropdown(dropdown& header, bool& optionSelected) {
    vector2 v = { mousex(), mousey() };
    bool res = isInsideRect(v, header.rect);
    if (isMouseClicked)
        header.isSelected = res;

    header.col = (res ? header.hovering_col : header.default_col);
    if (header.isSelected)
        header.col = header.selected_col;
    optionSelected = false;
    if (header.isSelected)
        for (int i = 0; i < header.nr_options; i++)
            optionSelected = optionSelected || isClicked(header.options[i]);

}

void clickDropdown(dropdown header[], int nr_header, bool& optionSelected)
{
    optionSelected = false;
    bool aux;
    for (int i = 0; i < nr_header; i++)
    {
        clickDropdown(header[i], aux);
        optionSelected = optionSelected || aux;
    }
}

void display(dropdown& header) {
    setfillstyle(SOLID_FILL, header.col);
    bar(header.rect.x, header.rect.y, header.rect.x + header.rect.width, header.rect.y + header.rect.height);

    setcolor(BLACK);
    setbkcolor(header.col);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    outtextxy(header.rect.x + header.padding, header.rect.y + header.rect.height / 2 - textheight(header.text) / 2, header.text);

    if (header.isSelected)
        for (int i = 0; i < header.nr_options; i++)
            display(header.options[i]);
}

void display(dropdown header[], int nr_header)
{
    for (int i = 0; i < nr_header; i++)
        display(header[i]);
}
/*
void keyboard()
{
    if (kbhit())
        {

            char Ch = getch();

            switch (Ch)
            {
            case ESC:
                windowShouldClose = true;
                break;
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
            //delay(15);
        }
}
*/

#endif // USER_INTERACTION_H_INCLUDED
