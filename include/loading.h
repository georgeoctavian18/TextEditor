#ifndef LOADING_H_INCLUDED
#define LOADING_H_INCLUDED

void loadLanguage(dropdown header[], int nr_header)
{
    ifstream f("data\\english.txt");
    int i, j;
    for (i=0; i<nr_header; i++)
    {
        f.getline(header[i].text, 101);
        for (j=0; j<header[i].nr_options; j++)
            f.getline(header[i].options[j].text, 101);
    }

    f.close();
}

void createHeader(dropdown header[], int &nr_header)
{
    ifstream f("data\\header_settings.txt");
    int w, h, col1, col2, col3, r, g, b;
    f>>w>>h;
    f>>r>>g>>b;
    col1=COLOR(r, g, b);
    f>>r>>g>>b;
    col2=COLOR(r, g, b);
    f>>r>>g>>b;
    col3=COLOR(r, g, b);
    int i, j, nr_options;
    f>>nr_header;
    for (i=0; i<nr_header; i++)
    {
        f>>nr_options;
        createDropdown(header[i], i*w, 0, w, h, col1, col2, col3, w/10, nr_options);
        for (j=0; j<header[i].nr_options; j++)
            createButton(header[i].options[j], header[i].rect.x, header[i].rect.y+h+j*h, w, h, col1, col2, header[i].padding);
    }
    f.close();

    loadLanguage(header, nr_header);

    int offset=0;
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

#endif // LOADING_H_INCLUDED
