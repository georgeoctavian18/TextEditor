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

void OpenFile(int& currLine, int& currCol, int charsPerLine, char path[], char fileName[], vector <string>& lines, vector <int>& enterLines, bool wordWrap)
{
    char line[1000];

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
    currLine = currCol = 0;
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