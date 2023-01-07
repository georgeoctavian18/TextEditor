#ifndef USER_INTERACTION_H_INCLUDED
#define USER_INTERACTION_H_INCLUDED

//MOUSE FLAGS

bool isMouseDown = false;
bool isMouseClicked = false;
bool isMouseDoubleClicked = false;
bool isMouseUp = true;
bool isMouseClicked_helper = false;
bool isMouseDoubleClicked_helper = false;
bool windowShouldClose = false;
bool beginSelection = false;

void mousePressed(int x, int y)
{
    isMouseDown = true;
    isMouseUp = false;
    isMouseClicked_helper = true;
}

void mouseDoubleClicked(int x, int y)
{
    isMouseDoubleClicked_helper = true;
}

void mouseReleased(int x, int y)
{
    isMouseDown = false;
    isMouseUp = true;
}

void setupFlags()
{
    lastMouseClicked = lastCursorChanged = startTime = millis();
    registermousehandler(WM_LBUTTONDOWN, mousePressed);
    registermousehandler(WM_LBUTTONUP, mouseReleased);
    registermousehandler(WM_LBUTTONDBLCLK, mouseDoubleClicked);
}

void updateFlags()
{
    isMouseClicked = isMouseClicked_helper;
    isMouseDoubleClicked = isMouseDoubleClicked_helper;
    if (isMouseClicked)
        lastMouseClicked = millis();
}

void clearFlags()
{
    if (isMouseClicked)
        isMouseClicked_helper = false;
    if (isMouseDoubleClicked)
        isMouseDoubleClicked_helper = false;
}

int kindOfWord(char c)
{
    //if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || '0' <= c && c <= '9' || c == '_')
        //return 1;
    if (c == ' ' || c=='\t')
        return 2;
    return 1;
}

vector2 mouseBeginPos;

void clickOnText(int textBeginX, int textBeginY, int& CurrLine1, int& CurrCol1, int& CurrLine2, int& CurrCol2, int &LineBeginFrame, int &LineEndFrame, int &ColBeginFrame, int &ColEndFrame, vector<string>& Lines, vector<int> EnterLines, struct rectangle textScreen)
{
    vector2 mouse = { mousex(), mousey() };

    int i, j, si, sj, mini, maxi;
    bool g;
    mini = -1;
    maxi = Lines.size();
    for (i=0; i<EnterLines.size(); i++)
    {
        if (CurrLine1<=EnterLines[i] && EnterLines[i]<maxi)
            maxi = EnterLines[i];
        if (mini<EnterLines[i] && EnterLines[i]<CurrLine1)
            mini = EnterLines[i];
    }

    if (Lines.size()>0 && isInsideRect(mouse, textScreen))
    {
        if (isMouseClicked)
        {
            mouseBeginPos = mouse;
            if (mouse.y < textBeginY)
                CurrLine1 = LineBeginFrame;
            else
                if (mouse.y > textBeginY + min((int)Lines.size() - 1 - LineBeginFrame, LineEndFrame-LineBeginFrame) * CHAR_HEIGHT)
                    CurrLine1 = min((int)Lines.size() - 1, LineEndFrame);
                else
                    CurrLine1 = LineBeginFrame + (mouse.y - textBeginY) / CHAR_HEIGHT;

            if (mouse.x < textBeginX)
                CurrCol1 = ColBeginFrame;
            else
                if (mouse.x > textBeginX + min(((int)Lines[CurrLine1].size() - ColBeginFrame), ColEndFrame-ColBeginFrame)* CHAR_WIDTH)
                    CurrCol1 = min((int)Lines[CurrLine1].size(), ColEndFrame);
                else
                    CurrCol1 = ColBeginFrame + (mouse.x - textBeginX + CHAR_WIDTH/2)/CHAR_WIDTH;
            beginSelection = true;

            si = CurrLine1;
            sj = max(0, (CurrCol1==Lines[CurrLine1].size()?CurrCol1-1:CurrCol1));
            if (Lines[si][sj]=='\t')
            {
                g = false;
                for (j=sj; j>=0 && !g; j--)
                    if (Lines[si][j]!='\t')
                        g = true;
                for (i=si-1; i>=0 && i>mini && !g; i--)
                    for (j=Lines[i].size()-1; j>=0 && !g; j--)
                        if (Lines[i][j]!='\t')
                            g = true;
                CurrLine1 = i+1;
                CurrCol1 = max(0, (g?j+2:j));

                i = CurrLine1;
                j = CurrCol1;
                while (i<si || j<sj)
                {
                    j+=4;
                    if (j>Lines[i].size())
                    {
                        i++;
                        j=4;
                    }
                }
                CurrLine1 = i;
                CurrCol1 = (sj-(j-4)<=2?j-4:j);

            }
        }
        else if (isMouseDoubleClicked)
        {
            si = CurrLine1;
            sj = (CurrCol1==Lines[CurrLine1].size()?CurrCol1-1:CurrCol1);
            if (sj<0)
                sj=0;
            int caz = kindOfWord(Lines[si][sj]);
            g = false;
            for (j=sj; j<Lines[si].size() && !g; j++)
                if (caz!=kindOfWord(Lines[si][j]))
                    g = true;
            for (i=si+1; i<Lines.size() && i<=maxi && !g; i++)
                for (j=0; j<Lines[i].size() && !g; j++)
                    if (caz!=kindOfWord(Lines[i][j]))
                        g = true;
            CurrLine2 = i-1;
            CurrCol2 = (g?j-1:j);

            g = false;
            for (j=sj; j>=0 && !g; j--)
                if (caz!=kindOfWord(Lines[si][j]))
                    g = true;
            for (i=si-1; i>=0 && i>mini && !g; i--)
                for (j=Lines[i].size()-1; j>=0 && !g; j--)
                    if (caz!=kindOfWord(Lines[i][j]))
                        g = true;
            CurrLine1 = i+1;
            CurrCol1 = (g?j+2:j);
        }

    }

    if (isMouseDown && beginSelection)
    {
        if (mouse.y < textBeginY)
            CurrLine2 = LineBeginFrame;
        else
            if (mouse.y > textBeginY + min((int)Lines.size() - 1 - LineBeginFrame, LineEndFrame-LineBeginFrame) * CHAR_HEIGHT)
                CurrLine2 = min((int)Lines.size() - 1, LineEndFrame);
            else
                CurrLine2 = LineBeginFrame + (mouse.y - textBeginY) / CHAR_HEIGHT;

        if (mouse.x < textBeginX)
            CurrCol2 = ColBeginFrame;
        else
            if (mouse.x > textBeginX + min(((int)Lines[CurrLine2].size() - ColBeginFrame), ColEndFrame-ColBeginFrame)* CHAR_WIDTH)
                CurrCol2 = min((int)Lines[CurrLine2].size(), ColEndFrame);
            else
                CurrCol2 = ColBeginFrame + (mouse.x - textBeginX + CHAR_WIDTH/2)/CHAR_WIDTH;

        si = CurrLine2;
        sj = (CurrCol2==Lines[CurrLine2].size()?CurrCol2-1:CurrCol2);
        if (sj<0)
            sj=0;
        if (Lines[si][sj]=='\t')
        {
            g = false;
            for (j=sj; j>=0 && !g; j--)
                if (Lines[si][j]!='\t')
                    g = true;
            for (i=si-1; i>=0 && i>mini && !g; i--)
                for (j=Lines[i].size()-1; j>=0 && !g; j--)
                    if (Lines[i][j]!='\t')
                        g = true;
            CurrLine2 = i+1;
            CurrCol2 = max(0, (g?j+2:j));

            i = CurrLine2;
            j = CurrCol2;
            while (i<si || j<sj)
            {
                j+=4;
                if (j>Lines[i].size())
                {
                    i++;
                    j=4;
                }
            }
            CurrLine2 = i;
            CurrCol2 = (sj-(j-4)<=2?j-4:j);
        }
        int frequency=30;
        if (lastCursorChanged/frequency<millis()/frequency)
        {
            if (mouse.x > mouseBeginPos.x && mouse.x > textBeginX + (ColEndFrame-ColBeginFrame)* CHAR_WIDTH && Lines[CurrLine2].size()>ColEndFrame)
                RightArrowKey(CurrLine2, CurrCol2, ColEndFrame-ColBeginFrame, Lines, EnterLines);
            else if (mouse.x < mouseBeginPos.x && mouse.x < textBeginX && CurrCol2>0)
                LeftArrowKey(CurrLine2, CurrCol2, ColEndFrame-ColBeginFrame, Lines, EnterLines);
            if (mouse.y < mouseBeginPos.y && mouse.y < textBeginY && CurrLine2>0)
                UpArrowKey(CurrLine2, CurrCol2, ColEndFrame-ColBeginFrame, Lines, EnterLines);
            else if (mouse.y > mouseBeginPos.y && mouse.y > textBeginY + (LineEndFrame-LineBeginFrame+1)* CHAR_HEIGHT && Lines.size()>LineEndFrame+1)
                DownArrowKey(CurrLine2, CurrCol2, ColEndFrame-ColBeginFrame, Lines, EnterLines);
        }

        lastCursorChanged = millis();
        movePage(CurrLine2, CurrCol2, LineBeginFrame, ColBeginFrame, LineEndFrame, ColEndFrame, LineEndFrame-LineBeginFrame+1, ColEndFrame-ColBeginFrame, Lines);
    }
    else
        beginSelection = false;
    if (CurrCol1<0)
        CurrCol1=0;
    if (CurrCol2<0)
        CurrCol2=0;
}

//-------------------------------------------------------------------------------------------------------

struct button {
    struct rectangle rect;
    char text[100];
    int col;
    palette* theme;
    int padding;
};

bool isClicked(button& b) {
    vector2 v = { mousex(), mousey() };
    bool res = isInsideRect(v, b.rect);
    b.col = (res ? b.theme->button_hovering: b.theme->button_default);
    return (isMouseClicked && res);
}

void display(button& b) {
    setfillstyle(SOLID_FILL, b.col);
    bar(b.rect.x, b.rect.y, b.rect.x + b.rect.width, b.rect.y + b.rect.height);

    setcolor(b.theme->text);
    setbkcolor(b.col);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    outtextxy(b.rect.x + b.padding, b.rect.y + b.rect.height / 2 - textheight(b.text) / 2, b.text);
}

void displayFlag(button& b, bool flag)
{
    struct rectangle box = b.rect;
    int x = box.x + box.width + box.height/2;
    int y = box.y + box.height/2;
    int lat = box.height*0.5*0.7;

    filledCircle(x, y, lat, b.theme->button_default, b.theme->contrast);
    if (flag)
        filledCircle(x, y, lat*0.6, b.theme->contrast, b.theme->contrast);
}

//-------------------------------------------------------------------------------------------------------

struct dropdown {
    struct rectangle rect;
    char text[100];
    int col;
    palette* theme;
    bool isSelected;
    button options[20];
    int nr_options, padding;
};


void clickDropdown(dropdown& header) {
    vector2 v = { mousex(), mousey() };
    bool res = isInsideRect(v, header.rect);
    if (isMouseClicked)
        header.isSelected = res;

    header.col = (res ? header.theme->button_hovering : header.theme->button_default);
    if (header.isSelected)
        header.col = header.theme->button_selected;
}

void clickDropdown(dropdown header[], int nr_header)
{
    for (int i = 0; i < nr_header; i++)
        clickDropdown(header[i]);
}

void display(dropdown& header) {
    setfillstyle(SOLID_FILL, header.col);
    bar(header.rect.x, header.rect.y, header.rect.x + header.rect.width, header.rect.y + header.rect.height);
    setcolor(header.theme->text);
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

//-------------------------------------------------------------------------------------------------------

struct slider{
    int state, maxState, minState, step;
    struct rectangle rect, mouseCatch;
    bool selected, exists, active;
    int offset;
    palette* theme;
    //button increase, decrease;
};

void createSlider(slider &s, float x, float y, float w, float h, int minState, int maxState, int defaultState, int step, palette* theme){
    s.rect = {x, y, w, h};
    s.minState = minState;
    s.maxState = maxState;
    s.step = step;
    s.state = defaultState;
    s.theme = theme;
    s.selected = false;
    s.exists = true;
    s.active = false;
}

void clickVerticalSlider(slider& s){
    if (s.state<s.minState)
            s.state = s.minState;
    if (s.state>s.maxState)
        s.state = s.maxState;
    s.active = (s.minState!=s.maxState);

    if (s.exists && s.active)
    {
        struct vector2 v = {mousex(), mousey()};
        int total_states = (s.maxState-s.minState)/s.step;
        int total_empty_space = s.rect.width * total_states;

        float height = max(s.rect.height - total_empty_space, s.rect.width*2);
        float y = map(s.state, s.minState, s.maxState, s.rect.y, s.rect.y+s.rect.height - height);

        s.mouseCatch = {s.rect.x, y, s.rect.width, height};

        if (isMouseClicked)
            if (isInsideRect(v, s.mouseCatch))
            {
                s.selected = true;
                s.offset = v.y - s.mouseCatch.y;
            }
        if (isMouseUp)
            s.selected = false;

        if (s.selected){
            if (v.y>=s.rect.y + s.rect.height - s.mouseCatch.height + s.offset)
                s.state = s.maxState;
            else if (v.y<=s.rect.y + s.offset)
                s.state = s.minState;
            else{
                s.state = map(v.y-s.offset, s.rect.y, s.rect.y + s.rect.height - s.mouseCatch.height, s.minState, s.maxState);
                s.state = s.state-s.state%s.step;
            }
        }
    }

}

void clickHorizontalSlider(slider& s){
    if (s.state<s.minState)
        s.state = s.minState;
    if (s.state>s.maxState)
        s.state = s.maxState;
    s.active = (s.minState!=s.maxState);

    if (s.exists && s.active)
    {
        struct vector2 v = {mousex(), mousey()};
        int total_states = (s.maxState-s.minState)/s.step;
        int total_empty_space = s.rect.height * total_states;

        float width = max(s.rect.width - total_empty_space, s.rect.height*2);
        float x = map(s.state, s.minState, s.maxState, s.rect.x, s.rect.x+s.rect.width - width);

        s.mouseCatch = {x, s.rect.y, width, s.rect.height};

        if (isMouseClicked)
            if (isInsideRect(v, s.mouseCatch))
            {
                s.selected = true;
                s.offset = v.x - s.mouseCatch.x;
            }
        if (isMouseUp)
            s.selected = false;

        if (s.selected){
            if (v.x>=s.rect.x + s.rect.width - s.mouseCatch.width + s.offset)
                s.state = s.maxState;
            else if (v.x<=s.rect.x + s.offset)
                s.state = s.minState;
            else{
                s.state = map(v.x-s.offset, s.rect.x, s.rect.x + s.rect.width - s.mouseCatch.width, s.minState, s.maxState);
                s.state = s.state-s.state%s.step;
            }
        }
    }

}

void display(slider &s){

    if (s.exists)
    {
        filledRect(s.rect.x, s.rect.y, s.rect.width, s.rect.height, s.theme->scroll_background, s.theme->scroll_background);
        if (s.active)
            filledRect(s.mouseCatch.x, s.mouseCatch.y, s.mouseCatch.width, s.mouseCatch.height, (s.selected?s.theme->scroll_selected:s.theme->scroll_button), s.theme->scroll_button);
    }
}

void updateMaxTextLength(int& maxi, vector <string>& Lines, int CharsPerLine, bool from_0)
{
    if (from_0)
        maxi = 0;
    for (int i=0; i<Lines.size(); i++)
        if (Lines[i].size()>maxi)
            maxi = Lines[i].size();
}

#endif // USER_INTERACTION_H_INCLUDED
