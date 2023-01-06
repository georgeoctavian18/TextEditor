#ifndef LOADING_H_INCLUDED
#define LOADING_H_INCLUDED

void loadLanguage(const char projectDir[], const char file[], dropdown header[], int nr_header)
{
    char temp[1024];
    GetCurrentDirectoryA(1024, temp);
    SetCurrentDirectoryA(projectDir);
    ifstream f(file);
    int i, j;

    for (i=0; i < nr_header; i++)
    {
        f.getline(header[i].text, 101);
        for (j=0; j < header[i].nr_options; j++)
            f.getline(header[i].options[j].text, 101);
    }
    f.close();
    SetCurrentDirectoryA(temp);

}

void loadTheme(const char projectDir[], const char file[], palette* theme)
{
    int i, r, g, b, *v;
    v=(int*)theme;
    char temp[1024];
    GetCurrentDirectoryA(1024, temp);
    SetCurrentDirectoryA(projectDir);
    ifstream f(file);
    for (i=0; i<sizeof(palette)/sizeof(int); i++)
    {
        f>>r>>g>>b;
        v[i]=COLOR(r, g, b);
    }
    f.close();
    SetCurrentDirectoryA(temp);
}

void resizeHeader(dropdown header[], int &nr_header)
{
    int offset=0, i, j;
    for (i=0; i<nr_header; i++)
    {
        header[i].rect.x = offset;
        header[i].rect.width = textwidth(header[i].text)+2*header[i].padding;
        offset += header[i].rect.width;
        for (j=0; j<header[i].nr_options; j++)
            header[i].options[j].rect.x = header[i].rect.x;
    }

    int maxi, temp;
    for (i=0; i<nr_header; i++)
    {
        maxi=textwidth(header[i].options[0].text)+header[i].options[0].padding*2;
        for (j=0; j<header[i].nr_options; j++)
        {
            temp = textwidth(header[i].options[j].text)+header[i].options[j].padding*2;
            if (temp>maxi)
                maxi = temp;
        }

        maxi = (maxi>header[i].rect.width?maxi:header[i].rect.width);
        for (j=0; j<header[i].nr_options; j++)
            header[i].options[j].rect.width = maxi;
    }
}

void loadLayout(const char projectDir[], dropdown header[], int &nr_header, struct rectangle &headerBox, struct rectangle& statusBarBox, slider& lineSlider, slider& colSlider, struct rectangle& textBox, palette* theme, int lang_selected)
{
    char temp[1024];
    GetCurrentDirectoryA(1024, temp);
    SetCurrentDirectoryA(projectDir);
    ifstream f("data\\window_layout.txt");
    int i, j;
    float h;

    f>>h>>nr_header;
    for (i=0; i<nr_header; i++)
    {
        header[i].rect.y = 0;
        header[i].rect.height = h;
        header[i].theme = theme;
        header[i].col = theme->button_default;
        header[i].padding = h/2;
        header[i].isSelected = false;
        f>>header[i].nr_options;

        for (j=0; j<header[i].nr_options; j++)
        {
            header[i].options[j].rect.y = header[i].rect.y+h+j*h;
            header[i].options[j].rect.height = h;
            header[i].options[j].theme = theme;
            header[i].options[j].col = theme->button_default;
            header[i].options[j].padding = header[i].padding;
        }
    }

    loadLanguage(projectDir, (lang_selected?"data\\romana.lang":"data\\english.lang"), header, nr_header);
    resizeHeader(header, nr_header);

    headerBox = header[0].rect;
    headerBox.width = WIDTH;
    f>>h;
    statusBarBox = {0, HEIGHT - h, WIDTH, h};
    f>>h;
    createSlider(lineSlider, WIDTH-h, headerBox.y + headerBox.height, h, HEIGHT - (headerBox.y + headerBox.height + statusBarBox.height), 0, 0, 0, 1, theme);
    createSlider(colSlider, 0, HEIGHT - statusBarBox.height - lineSlider.rect.width, WIDTH - lineSlider.rect.width, lineSlider.rect.width, 0, 0, 0, 1, theme);
    lineSlider.rect.height -= colSlider.rect.height;
    textBox = { 0, (headerBox.y + headerBox.height), WIDTH - lineSlider.rect.width, HEIGHT - (headerBox.y + headerBox.height + statusBarBox.height + colSlider.rect.height) };

    f.close();
    SetCurrentDirectoryA(temp);
}

#endif // LOADING_H_INCLUDED
