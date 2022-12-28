#include "SettingsActions.h"



void DoWordWrap(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines)
{
    int spacePos, i = 0;
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
            spacePos = lines[i].find_last_of(' ');
            int j = lines[i].size() - 1;

            if (lines[i][j] == '\t')
            {
                for (int it = 0; it <= 3; it++)
                {
                    CharToString(s, lines[i].back());
                    lines[i].erase(lines[i].end() - 1);
                    lines[i + 1].insert(0, s);
                    j--;
                }
                continue;
            }

            if (spacePos + 1 >= charsPerLine || spacePos == -1)
            {
                CharToString(s, lines[i].back());
                lines[i].erase(lines[i].end() - 1);
                lines[i + 1].insert(0, s);
            }
            else
            {
                while (j > spacePos)
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

void OpenFile(int& currLine, int& currCol, int charsPerLine, char path[], char fileName[], vector <string>& lines, vector <int>& enterLines, bool wordWrap, bool& isSaved)
{
    char line[1000];

    isSaved = true;
    lines.clear();
    enterLines.clear();
    currLine = currCol = 0;

    if (!getOpenPath(path))
        strcpy(path, "\0");

    getNameFromPath(path, fileName);

    ifstream fin(path);

    while (fin.getline(line, 1000))
    {
        for (int i = 0; i < strlen(line); i++)
            InsertKey(currLine, currCol, charsPerLine, line[i], lines, enterLines, wordWrap);
        EnterKey(currLine, currCol, charsPerLine, lines, enterLines, wordWrap);
    }
    fin.close();
    
}

void SaveAsFile(int& currLine, int& currCol, int charsPerLine, char path[], char fileName[], vector <string>& lines, vector <int>& enterLines, bool wordWrap, bool& isSaved)
{
    if (!getSavePath(path))
        strcpy(path, "\0");

    getNameFromPath(path, fileName);
    isSaved = true;

    ofstream fout(path);
    for (int i = 0; i < lines.size(); i++)
    {
        fout << lines[i];
        if(count(enterLines.begin(), enterLines.end(), i))
            fout << '\n';
    }
    fout.close();
}

void NewFile(int& currLine, int& currCol, int charsPerLine, char path[], char fileName[], vector <string>& lines, vector <int>& enterLines, bool wordWrap, bool& isSaved)
{
    if (!getSavePath(path))
        strcpy(path, "\0");

    getNameFromPath(path, fileName);
    isSaved = true;

    ofstream fout(path);
    fout << "";
    fout.close();

    currLine = currCol = 0;
    lines.clear();
    enterLines.clear();
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

void SelectWindowSize(int& windSizeX, int& windSizeY)
{
    bool close = false, box = false;
    int x = 125, y = 45;
    char ch = '\0', p[3] = {'X', ':', '\0'};
    initwindow(300, 150, "Text Editor", (getmaxwidth() - 300) / 2, (getmaxheight() - 150) / 2);
    setcolor(0);
    setbkcolor(15);
    bar(0, 0, 300, 150);
    
    outtextxy(80, 45, p);
    p[0] = 'Y';
    outtextxy(80, 95, p);

    
    setfillstyle(SOLID_FILL, 7);
    bar(115, 40, 185, 70);
    bar(115, 90, 185, 120);

    settextstyle(8, HORIZ_DIR, 2);
    setfillstyle(SOLID_FILL, 15);

    char text[50] = "Select the dimensions:", printArray[50];
    string stText, ndText;

    outtextxy(10, 10, text);

    setbkcolor(7);
    
    while (!close)
    {
        if (kbhit())
        {
            ch = getch();
            if (ch == CR)
                close = true;
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
                        setfillstyle(SOLID_FILL, 7);
                        bar(115, 40, 185, 70);
                        bar(115, 90, 185, 120);
                        setfillstyle(SOLID_FILL, 15);
                    }
                }
                else
                {
                    if (ch >= '0' && ch <= '9' && ndText.size() < 4)
                        ndText += ch;
                    else if (ch == BS && !ndText.empty())
                    {
                        ndText.pop_back();
                        setfillstyle(SOLID_FILL, 7);
                        bar(115, 40, 185, 70);
                        bar(115, 90, 185, 120);
                        setfillstyle(SOLID_FILL, 15);
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
        
        //swapbuffers();
    }

    std::string::size_type sz;
    windSizeX = stoi(stText, &sz);
    windSizeY = stoi(ndText, &sz);
    
    closegraph();
}