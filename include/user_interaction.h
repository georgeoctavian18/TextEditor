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
bool functionMode = false;

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

void clickOnText(int& CurrLine1, int& CurrCol1, int& CurrLine2, int& CurrCol2, int LineBeginFrame, int LineEndFrame, int ColBeginFrame, int ColEndFrame, vector<string>& Lines, struct rectangle textScreen)
{
    int textBeginY = HEIGHT / 10;
    int textBeginX = CHAR_WIDTH;
    vector2 mouse = { mousex(), mousey() };
    /*
    int headerHeight = 30;
    struct rectangle textScreen = { 0, headerHeight, WIDTH, HEIGHT - headerHeight };
    */
    if (Lines.size()>0 && isInsideRect(mouse, textScreen))
    {
        if (isMouseClicked)
        {
            if (mouse.y < textBeginY)
                CurrLine1 = LineBeginFrame;
            else
                if (mouse.y > textBeginY + (Lines.size() - 1 - LineBeginFrame) * CHAR_HEIGHT)
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
        }
        else if (isMouseDoubleClicked)
        {
            /*selecteaza cuvant*/
        }

        if (isMouseDown && beginSelection)
        {
            if (mouse.y < textBeginY)
                CurrLine2 = LineBeginFrame;
            else
                if (mouse.y > textBeginY + (Lines.size() - 1 - LineBeginFrame) * CHAR_HEIGHT)
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
            lastCursorChanged = millis();
        }
        else
            beginSelection = false;
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


void clickDropdown(dropdown& header) {
    vector2 v = { mousex(), mousey() };
    bool res = isInsideRect(v, header.rect);
    if (isMouseClicked)
        header.isSelected = res;

    header.col = (res ? header.hovering_col : header.default_col);
    if (header.isSelected)
        header.col = header.selected_col;
}

void clickDropdown(dropdown header[], int nr_header)
{
    for (int i = 0; i < nr_header; i++)
        clickDropdown(header[i]);
}

void display(dropdown& header) {
    setfillstyle(SOLID_FILL, header.col);
    bar(header.rect.x, header.rect.y, header.rect.x + header.rect.width, header.rect.y + header.rect.height);
    //settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
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

//-------------------------------------------------------------------------------------------------------

struct slider{
    int state, maxState, minState, step;
    struct rectangle rect, mouseCatch;
    bool selected, exists, active;
    int offset;
    //button increase, decrease;
};

void createSlider(slider &s, float x, float y, float w, float h, int minState, int maxState, int defaultState, int step){
    s.rect = {x, y, w, h};
    s.minState = minState;
    s.maxState = maxState;
    s.step = step;
    s.state = defaultState;
    s.selected = false;
    s.exists = true;
    s.active = false;
}

void clickVerticalSlider(slider& s){
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
        int bgcol = COLOR(230, 230, 230);
        filledRect(s.rect.x, s.rect.y, s.rect.width, s.rect.height, bgcol, bgcol);
    }
    if (s.active)
    {
        int catchcol = COLOR(150, 150, 150);
        filledRect(s.mouseCatch.x, s.mouseCatch.y, s.mouseCatch.width, s.mouseCatch.height, catchcol, catchcol);
    }
}

#endif // USER_INTERACTION_H_INCLUDED
