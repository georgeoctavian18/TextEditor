#include "KeyActions.h"


void EnterKey(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines)
{
    string s;

    for (int i = 0; i < enterLines.size(); i++)
        if (enterLines[i] >= currLine)
            enterLines[i]++;
    enterLines.push_back(currLine);

    if (count(enterLines.begin(), enterLines.end(), currLine + 1))
    {

        s = lines[currLine].substr(currCol);
        lines[currLine].erase(currCol);

        currLine++;

        if (currLine + 1 > lines.size())
            lines.push_back(s);
        else lines.insert(lines.begin() + currLine, s);
    }
    else
    {
        if (currLine + 1 > lines.size())
        {
            CharToString(s, ' ');
            lines.push_back(s);
        }
        int line = currLine + 1, col = 0;
        for (int i = currCol; i < lines[currLine].size(); i++)
            InsertKey(line, col, charsPerLine, lines[currLine][i], lines, enterLines);

        lines[currLine].erase(currCol);
        currLine++;

    }

    currCol = 0;

}

void InsertKey(int& currLine, int& currCol, int charsPerLine, char ch, vector <string>& lines, vector <int>& enterLines)
{
    int spacePos;
    bool isNextLine = false;

    string s;
    CharToString(s, ch);

    InitLine(currLine, lines);

    lines[currLine].insert(currCol, s);

    if (lines[currLine].size() > charsPerLine)
    {
        int i = currLine;
        while (i < lines.size() && lines[i].size() > charsPerLine)
        {
            if (i == lines.size() - 1)
                InitLine(i + 1, lines);

            if (count(enterLines.begin(), enterLines.end(), i))
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

            if (i == currLine)
            {
                if (currCol > spacePos && spacePos != -1)
                {
                    currCol -= spacePos;
                    isNextLine = true;
                }
                else
                {
                    currCol++;
                    if (currCol > charsPerLine)
                        currCol = 1, isNextLine = true;
                }
            }
            i++;

        }
    }
    else currCol++;

    if (isNextLine)
        currLine++;
}

void BackspaceKey(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines)
{
    bool ok = false;
    if (!currLine && !currCol) return;

    if (currCol)
    {
        lines[currLine].erase(lines[currLine].begin() + currCol - 1, lines[currLine].begin() + currCol);

        if (!lines[currLine].size())
            currCol = lines[--currLine].size();
        else currCol--;
    }
    else
    {
        currCol = lines[currLine - 1].size();
        if (count(enterLines.begin(), enterLines.end(), currLine - 1))
        {
            auto pos = find(enterLines.begin(), enterLines.end(), currLine - 1);
            enterLines.erase(pos);
        }
        else
        {
            lines[currLine - 1].erase(lines[currLine - 1].begin() + currCol - 1, lines[currLine - 1].begin() + currCol);
            currCol--;
        }

        ok = true;
        currLine--;
    }

    int i = currLine;

    if (count(enterLines.begin(), enterLines.end(), i) && ok)
    {
        int emptySpace = charsPerLine - lines[i].size();

        while (emptySpace && lines[i + 1].size())
        {
            char ch = lines[i + 1].front();
            lines[i].push_back(ch);
            lines[i + 1].erase(lines[i + 1].begin(), lines[i + 1].begin() + 1);
            emptySpace--;
        }
        if (!lines[i + 1].size())
        {
            for (int j = i + 1; j < lines.size() - 1; j++)
                lines[j].replace(lines[j].begin(), lines[j].end(), lines[j + 1]);
            lines.pop_back();
        }
    }
    else
    {
        while (!count(enterLines.begin(), enterLines.end(), i) && i < lines.size() - 1)
        {
            int j = 0, emptySpace = charsPerLine - lines[i].size();

            while (emptySpace >= j && lines[i + 1].size())
            {

                while (lines[i + 1][j] != ' ' && j < lines[i + 1].size())
                    j++;

                if (lines[i + 1][j] == ' ' && !j)
                    j = 1;

                if (j <= emptySpace)
                {
                    string s = lines[i + 1].substr(0, j);
                    lines[i + 1].erase(lines[i + 1].begin(), lines[i + 1].begin() + j);
                    lines[i] += s;
                    j = 0;
                }
                else if (lines[i].back() != ' ' && lines[i + 1].front() != ' ' && emptySpace >= 0)
                {
                    string s = lines[i + 1].substr(0, emptySpace);
                    lines[i + 1].erase(lines[i + 1].begin(), lines[i + 1].begin() + emptySpace);
                    lines[i] += s;
                }
                emptySpace = charsPerLine - lines[i].size();
            }
            if (lines[i + 1].empty())
            {
                for (int j = i + 1; j < lines.size() - 1; j++)
                    lines[j].replace(lines[j].begin(), lines[j].end(), lines[j + 1]);

                for (int j = 0; j < enterLines.size(); j++)
                    if (enterLines[j] > i)
                        enterLines[j]--;
                lines.pop_back();
            }
            i++;
        }
    }
}

void TabKey(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines)
{
    if (currCol + 3 > charsPerLine)
        currLine++, currCol = 0;

    InsertKey(currLine, currCol, charsPerLine, ' ', lines, enterLines);
    InsertKey(currLine, currCol, charsPerLine, ' ', lines, enterLines);
    InsertKey(currLine, currCol, charsPerLine, ' ', lines, enterLines);
    InsertKey(currLine, currCol, charsPerLine, ' ', lines, enterLines);

}

void SpecialKey(int& currLine, int& currCol, int command, int charsPerLine, vector <string> &lines, vector <int> enterLines, bool& wordWrap)
{
    
    switch (command)
    {
    case KEY_RIGHT:
        RightArrowKey(currLine, currCol, charsPerLine, lines, enterLines);
        break;
    case KEY_LEFT:
        LeftArrowKey(currLine, currCol, charsPerLine, lines, enterLines);
        break;
    case KEY_UP:
        UpArrowKey(currLine, currCol, charsPerLine, lines, enterLines);
        break;
    case KEY_DOWN:
        DownArrowKey(currLine, currCol, charsPerLine, lines, enterLines);
        break;
    case KEY_HOME:
        HomeKey(currLine, currCol, charsPerLine, lines, enterLines);
        break;
    case KEY_END:
        EndKey(currLine, currCol, charsPerLine, lines, enterLines);
        break;
    case KEY_DELETE:
        DeleteKey(currLine, currCol, charsPerLine, lines, enterLines);
        break;
    case KEY_INSERT:
        wordWrap = wordWrap ? false : true;
        if (wordWrap) DoWordWrap(currLine, currCol, charsPerLine, lines, enterLines);
    }  
    
}

void RightArrowKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines)
{
    if (currCol == lines[currLine].size() || (currCol == lines[currLine].size() - 1 && !count(enterLines.begin(), enterLines.end(), currLine)))
    {
        if (lines.size() != currLine + 1)
        {
            currCol = 0;
            currLine++;
        }
        else if (currCol != lines[currLine].size()) currCol++;
    }
    else if (currCol != lines[currLine].size()) currCol++;
}

void LeftArrowKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines)
{
    if (currCol == 0)
    {
        if (currLine > 0)
        {
            currLine--;
            currCol = lines[currLine].size();

            if (!count(enterLines.begin(), enterLines.end(), currLine) && lines[currLine][currCol - 1] != ' ')
                currCol--;
        }
    }
    else currCol--;
}

void UpArrowKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines)
{
    if (currLine > 0)
    {
        currLine--;
        currCol = currCol < lines[currLine].size() ? currCol : lines[currLine].size();
    }
}

void DownArrowKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines)
{
    if (lines.size() != currLine + 1)
    {
        currLine++;
        currCol = currCol < lines[currLine].size() ? currCol : lines[currLine].size();
    }
}

void HomeKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines)
{
    if (currCol)
        currCol = 0;
    else if (!count(enterLines.begin(), enterLines.end(), currLine - 1) && currLine > 0)
    {
        currLine--;
        currCol = 0;
    }
}

void EndKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines)
{
    if (currCol != lines[currLine].size())
        currCol = lines[currLine].size();
    else if (!count(enterLines.begin(), enterLines.end(), currLine) && currLine < lines.size() - 1)
    {
        currLine++;
        currCol = lines[currLine].size();
    }
}

void DeleteKey(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines)
{
    int last = currCol;
    RightArrowKey(currLine, currCol, charsPerLine, lines, enterLines);

    if(last != currCol)
        BackspaceKey(currLine, currCol, charsPerLine, lines, enterLines);
}
