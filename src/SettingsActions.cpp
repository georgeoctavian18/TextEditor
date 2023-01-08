#include "SettingsActions.h"



void DoWordWrap(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines)
{
    int spacePos, tabPos, position = -1, i = 0;
    string s;

    currLine = currCol = 0;

    while (i < lines.size())
    {
        if (i == lines.size() - 1 && lines[i].size() > charsPerLine)
            InitLine(i + 1, lines);

        if (count(enterLines.begin(), enterLines.end(), i) && lines[i].size() > charsPerLine)
        {
            lines.insert(lines.begin() + i + 1, "");
            for (int j = 0; j < enterLines.size(); j++)
                if (enterLines[j] >= i)
                    enterLines[j]++;
        }

        while (lines[i].size() > charsPerLine)
        {
            position = -1;
            spacePos = lines[i].find_last_of(' ');
            tabPos = lines[i].find_last_of('\t');

            int j = lines[i].size() - 1;

            if (lines[i][j] == '\t' || lines[i][j] == ' ')
            {
                int limit = 0;
                if (lines[i][j] == '\t') limit += 3;

                for (int it = 0; it <= limit; it++)
                {
                    CharToString(s, lines[i].back());
                    lines[i].erase(lines[i].end() - 1);
                    lines[i + 1].insert(0, s);
                    j--;
                }
                continue;
            }


            if ((spacePos == -1 || spacePos == charsPerLine) && (tabPos == charsPerLine || tabPos == -1))
            {
                CharToString(s, lines[i].back());
                lines[i].erase(lines[i].end() - 1);
                lines[i + 1].insert(0, s);
            }
            else
            {
                if (spacePos == -1 || spacePos == charsPerLine) position = tabPos;
                else if (tabPos == -1 || tabPos == charsPerLine) position = spacePos;
                else position = max(spacePos, tabPos);

                while (j > position)
                {
                    CharToString(s, lines[i].back());
                    lines[i].erase(lines[i].end() - 1);
                    lines[i + 1].insert(0, s);
                    j--;
                }

            }
        }
        i++;
    }

}

void UndoWordWrap(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines)
{
    int i = 1, pos = 0;

    currLine = currCol = 0;

    while (i < lines.size())
    {
        if (count(enterLines.begin(), enterLines.end(), i - 1))
        {
            pos++;
            lines[pos] = lines[i];
        }
        else
        {
            lines[pos] += lines[i];
            lines[i].clear();
        }
        i++;
    }
    while (pos + 1 < lines.size())
        lines.pop_back();

    enterLines.clear();
    for (int i = 0; i <= pos; i++)
        enterLines.push_back(i);
}

void OpenFile(int& currLine, int& currCol, int charsPerLine, char path[], char fileName[], vector <string>& lines, vector <int>& enterLines, stack <vector <string>>& stackLines, stack <vector <int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool wordWrap, bool& isSaved)
{
    char line[1000];

    if (!getOpenPath(path))
        return;

    isSaved = true;
    lines.clear();
    enterLines.clear();
    currLine = currCol = 0;

    getNameFromPath(path, fileName);

    ifstream fin(path);

    while (fin.getline(line, 1000))
    {
        for (int i = 0; i < strlen(line); i++)
        {
            if (line[i] == '\t')
                TabKey(currLine, currCol, charsPerLine, lines, enterLines, wordWrap);
            else InsertKey(currLine, currCol, charsPerLine, line[i], lines, enterLines, wordWrap);
        }
        EnterKey(currLine, currCol, charsPerLine, lines, enterLines, wordWrap);
    }
    fin.close();

    while (!stackLines.empty())
        stackLines.pop();
    while (!stackEnterLines.empty())
        stackEnterLines.pop();
    while (!stackLinCol.empty())
        stackLinCol.pop();
}

void SaveAsFile(int& currLine, int& currCol, int charsPerLine, char path[], char fileName[], vector <string>& lines, vector <int>& enterLines, bool wordWrap, bool& isSaved)
{
    if (!getSavePath(path))
        return;

    getNameFromPath(path, fileName);

    isSaved = true;

    ofstream fout(path);
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < lines[i].size(); j++)
        {
            if (lines[i][j] == '\t') j += 3;
            fout << lines[i][j];
        }
        if(count(enterLines.begin(), enterLines.end(), i))
            fout << '\n';
    }
    fout.close();
}

void NewFile(int& currLine, int& currCol, int charsPerLine, char path[], char fileName[], vector <string>& lines, vector <int>& enterLines, stack <vector <string>>& stackLines, stack <vector <int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool wordWrap, bool& isSaved)
{
    isSaved = true;
    strcpy(fileName, "Untitled");

    currLine = currCol = 0;
    lines.clear();
    enterLines.clear();
    while(!stackLines.empty())
        stackLines.pop();
    while(!stackEnterLines.empty())
        stackEnterLines.pop();
    while(!stackLinCol.empty())
        stackLinCol.pop();
}

void SaveFile(int& currLine, int& currCol, int charsPerLine, char path[], char fileName[], vector <string>& lines, vector <int>& enterLines, bool wordWrap, bool& isSaved)
{
    if (!strcmp(fileName, "Untitled"))
    {
        SaveAsFile(currLine, currCol, charsPerLine, path, fileName, lines, enterLines, wordWrap, isSaved);
        return;
    }
    isSaved = true;
    ofstream fout(path);
    for (int i = 0; i < lines.size(); i++)
    {
        fout << lines[i];
        if (count(enterLines.begin(), enterLines.end(), i))
            fout << '\n';
    }
    fout.close();
}

void SelectWindowSize(int& windSizeX, int& windSizeY, palette theme, int lang, const char projectDir[])
{
    bool close = false, box = false;
    int x = 125, y = 45, line = 0;
    char ch = '\0', p[3] = {'X', ':', '\0'}, text[50], temp[1024];
    initwindow(300, 150, "Text Editor", (getmaxwidth() - 300) / 2, (getmaxheight() - 150) / 2);
    setfillstyle(SOLID_FILL, theme.background);
    setcolor(theme.text);
    setbkcolor(theme.background);
    bar(0, 0, 300, 150);

    outtextxy(80, 45, p);
    p[0] = 'Y';
    outtextxy(80, 95, p);


    setfillstyle(SOLID_FILL, theme.scroll_background);
    bar(115, 40, 185, 70);
    bar(115, 90, 185, 120);

    settextstyle(8, HORIZ_DIR, 2);
    setfillstyle(SOLID_FILL, theme.text);

    GetCurrentDirectoryA(1024, temp);
    SetCurrentDirectoryA(projectDir);
    ifstream fin("data\\window_resize.txt");
    while (fin.getline(text, 50) && line < lang)
        line++;
    fin.close();
    SetCurrentDirectoryA(temp);

    char printArray[50];
    string stText, ndText;

    outtextxy(10, 10, text);

    setbkcolor(theme.scroll_background);

    while (!close)
    {
        if (kbhit())
        {
            ch = getch();
            if (ch == CR)
            {
                if (!box)
                    box = true;
                else close = true;
            }
            else if (ch == ESC) close = true;
            else if (ch == 0)
            {
                ch = getch();
                if (ch == KEY_DOWN && !box)
                    box = true;
                else if (ch == KEY_UP && box)
                    box = false;
            }
            else
            {
                if (!box)
                {
                    if (ch >= '0' && ch <= '9' && stText.size() < 4)
                        stText += ch;
                    else if (ch == BS && !stText.empty())
                    {
                        stText.pop_back();
                        setfillstyle(SOLID_FILL, theme.scroll_background);
                        bar(115, 40, 185, 70);
                        bar(115, 90, 185, 120);
                        setfillstyle(SOLID_FILL, theme.text);
                    }
                }
                else
                {
                    if (ch >= '0' && ch <= '9' && ndText.size() < 4)
                        ndText += ch;
                    else if (ch == BS && !ndText.empty())
                    {
                        ndText.pop_back();
                        setfillstyle(SOLID_FILL, theme.scroll_background);
                        bar(115, 40, 185, 70);
                        bar(115, 90, 185, 120);
                        setfillstyle(SOLID_FILL, theme.text);
                    }
                }
            }
        }

        string copySt = stText;
        StringToArray(copySt, printArray);
        outtextxy(x, y, printArray);

        string copyNd = ndText;
        StringToArray(copyNd, printArray);
        outtextxy(x, y + 50, printArray);
    }

    if (!stText.empty() && !ndText.empty())
    {
        int newWindX, newWindY;
        std::string::size_type sz;
        newWindX = stoi(stText, &sz);
        newWindY = stoi(ndText, &sz);

        if (newWindX >= 550 && newWindX <= getmaxwidth() && newWindY >= 300 && newWindY <= getmaxheight())
            windSizeX = newWindX, windSizeY = newWindY;
    }

    closegraph();
}

void ResizeWindow(int& currLine, int& currCol, int windSizeX, int windSizeY, int posX, int& posY, int& rowsPerFrame, int& charsPerLine, int& lineBeginFrame, int& lineEndFrame, int& colBeginFrame, int& colEndFrame, bool wordWrap, vector <string>& lines, vector <int>& enterLines)
{
    currLine = 0, currCol = 0;
    posY = windSizeY / 10;
    rowsPerFrame = (windSizeY - 2 * posY) / CHAR_HEIGHT;
    charsPerLine = (windSizeX - 4 * posX) / CHAR_WIDTH;
    lineBeginFrame = 0, lineEndFrame = rowsPerFrame - 1;
    colBeginFrame = 0, colEndFrame = charsPerLine;

    if (wordWrap)
        DoWordWrap(currLine, currCol, charsPerLine, lines, enterLines);
}
