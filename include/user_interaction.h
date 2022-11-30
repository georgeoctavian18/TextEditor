#ifndef USER_INTERACTION_H_INCLUDED
#define USER_INTERACTION_H_INCLUDED


struct button{
    struct rectangle rect;
    char text[100];
    int col, default_col, hovering_col;
    float fontSize;
};

void createButton(button &b, float x, float y, float w, float h, int default_col, int hovering_col, const char message[], float fontSize){
    b.rect={x, y, w, h};
    b.col = b.default_col = default_col;
    b.hovering_col = hovering_col;
    strcpy(b.text, message);
    b.fontSize = fontSize;
}


bool isClicked(button &b) {
    vector2 v = {mousex(), mousey()};
    bool res = isInsideRect(v, b.rect);
    b.col = (res ? b.hovering_col : b.default_col);
    return (isMouseClicked && res);
}



void display(button &b){
    setfillstyle(SOLID_FILL, b.col);
    bar(b.rect.x, b.rect.y, b.rect.x+b.rect.width, b.rect.y+b.rect.height);

    setcolor(BLACK);
    setbkcolor(b.col);
    settextjustify(CENTER_TEXT, TOP_TEXT);
    //setusercharsize(b.fontSize*100, 100, b.fontSize*100, 100);
    outtextxy(b.rect.x+b.rect.width/2, b.rect.y+b.rect.height/2 - textheight(b.text)/2, b.text);
}

//-------------------------------------------------------------------------------------------------------

struct dropdown{
    struct rectangle rect;
    char text[100];
    int col, selected_col, default_col, hovering_col;
    bool isSelected;
    float fontSize;
    button options[20];
    int nr_options;
};

void createDropdown(dropdown &header, float x, float y, float w, float h, int default_col, int hovering_col, int selected_col, const char message[], float fontSize, char optiontext[][21], int nr_options){
    header.rect={x, y, w, h};
    header.col = header.default_col = default_col;
    header.hovering_col = hovering_col;
    header.selected_col = selected_col;
    strcpy(header.text, message);
    header.isSelected = false;
    header.fontSize = fontSize;

    header.nr_options = nr_options;
    for (int i=0; i<header.nr_options; i++)
        createButton(header.options[i], x, y+h+i*h, w, h, default_col, hovering_col, optiontext[i], fontSize);
}


void clickDropdown(dropdown &header) {
    vector2 v = {mousex(), mousey()};
    bool res = isInsideRect(v, header.rect);
    if (isMouseClicked)
        header.isSelected = res;

    header.col = (res ? header.hovering_col : header.default_col);
    if (header.isSelected)
        header.col = header.selected_col;

    if (header.isSelected)
    {
        bool optionSelected=false;
        for (int i=0; i<header.nr_options; i++)
            optionSelected = optionSelected || isClicked(header.options[i]);
    }
}



void display(dropdown &header){
    setfillstyle(SOLID_FILL, header.col);
    bar(header.rect.x, header.rect.y, header.rect.x+header.rect.width, header.rect.y+header.rect.height);

    setcolor(BLACK);
    setbkcolor(header.col);
    settextjustify(CENTER_TEXT, TOP_TEXT);
   // setusercharsize(header.fontSize*100, 100, header.fontSize*100, 100);
    outtextxy(header.rect.x+header.rect.width/2, header.rect.y+header.rect.height/2 - textheight(header.text)/2, header.text);

    if (header.isSelected)
        for (int i=0; i<header.nr_options; i++)
            display(header.options[i]);
}



#endif // USER_INTERACTION_H_INCLUDED
