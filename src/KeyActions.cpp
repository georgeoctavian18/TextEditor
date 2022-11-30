#include "KeyActions.h"


void EnterKey(int& currLine, int& currCol, vector <string> &lines, vector <int>& enterLines)
{
    for(int i = 0; i < enterLines.size(); i++)
        if (enterLines[i] >= currLine)
            enterLines[i]++;

    enterLines.push_back(currLine);

    string s = lines[currLine].substr(currCol);
    lines[currLine].erase(currCol);

    currLine++;
    currCol = 0;

    if (currLine + 1 > lines.size())
        lines.push_back(s);
    else lines.insert(lines.begin() + currLine, s);

}


void InsertKey(int& currLine, int& currCol, vector <string>& lines, vector <int>& enterLines, vector <pair<int, int>>& tabsLocation, int charsPerLine, char ch)
{
    string s;
    CharToString(s, ch);

    InitLine(currLine, lines);

    lines[currLine].insert(currCol, s);

    for (int i = 0; i < tabsLocation.size(); i++)
        if (tabsLocation[i].first == currLine && tabsLocation[i].second >= currCol)
        {
            if (tabsLocation[i].second + 4 == charsPerLine)
            {
                lines[currLine].erase(lines[currLine].begin() + tabsLocation[i].second + 1, lines[currLine].end());
                tabsLocation.erase(tabsLocation.begin() + i);
            }
            else tabsLocation[i].second++;
        }
    currCol++;

    if (lines[currLine].size() > charsPerLine)
    {
        int i = currLine;
        while (i < lines.size() && lines[i].size() > charsPerLine)
        {
             if (i == lines.size() - 1) 
                InitLine(i + 1, lines);

             if (find(enterLines.begin(), enterLines.end(), i) != enterLines.end())
             {
                 lines.insert(lines.begin() + i + 1, "");
                 for (int j = 0; j < enterLines.size(); j++)
                     if (enterLines[j] >= i)
                         enterLines[j]++;
             }

             CharToString(s, lines[i].back());
             lines[i].erase(lines[i].end() - 1);
             lines[i + 1].insert(0, s);
             i++;

            for (int j = 0; j < tabsLocation.size(); j++)
                if (tabsLocation[j].first == i)
                {
                    if (tabsLocation[j].second + 4 == charsPerLine)
                    {
                        lines[i].erase(lines[i].begin() + tabsLocation[j].second + 1, lines[i].end());
                        tabsLocation.erase(tabsLocation.begin() + j);
                    }
                    else tabsLocation[j].second++;
                }
        }
    }
    if (currCol > charsPerLine) currCol = 1, currLine++;
  
}

void TabKey(int& currLine, int& currCol, vector <string>& lines, vector <int>& enterLines, vector <pair<int, int>>& tabsLocation, int charsPerLine)
{
    if (currCol + 4 > charsPerLine)
    {
        currLine++, currCol = 0;
    }

    int cLine = currLine, cCol = currCol;

    InsertKey(currLine, currCol, lines, enterLines, tabsLocation, charsPerLine, ' ');
    InsertKey(currLine, currCol, lines, enterLines, tabsLocation, charsPerLine, ' ');
    InsertKey(currLine, currCol, lines, enterLines, tabsLocation, charsPerLine, ' ');
    InsertKey(currLine, currCol, lines, enterLines, tabsLocation, charsPerLine, ' ');

    tabsLocation.push_back({ cLine, cCol });

}


void ArrowKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <pair<int, int>> tabsLocation)
{
    delay(15);
    int command = getch();
    if (command == KEY_RIGHT)
    {
        for (auto it : tabsLocation)
            if (it.first == currLine && it.second == currCol)
            {
                currCol += 4;
                if (currCol > charsPerLine)
                {
                    currCol -= charsPerLine;
                    currLine++;
                }
                return;
            }

        if (currCol == lines[currLine].size())
        {
            if (lines.size() != currLine + 1)
            {
                currCol = 0;
                currLine++;
            }
        }
        else currCol++;
    }
    else if (command == KEY_LEFT)
    {
        for (auto it : tabsLocation)
            if ((it.first == currLine && it.second + 4 == currCol)  )
            {
                currCol -= 4;
                if (currCol < 0)
                {
                    currCol += charsPerLine;
                    currLine--;
                }
                return;
            }

        if (currCol == 0)
        {
            if (currLine > 0)
            {
                currLine--;
                currCol = lines[currLine].size();
            }
        }
        else currCol--;
    }
    else if (command == KEY_DOWN && lines.size() != currLine + 1)
    {
        currLine++;

        for (auto it : tabsLocation)
            if (it.first == currLine && (currCol - it.second > 0 && currCol - it.second < 4))
            {
                if (currCol - it.second == 1)
                    currCol = it.second;
                else currCol = it.second + 4;
            }

        currCol = min(currCol, lines[currLine].size());
    }
    else if (command == KEY_UP && currLine > 0)
    {
        currLine--;

        for (auto it : tabsLocation)
            if (it.first == currLine && (currCol - it.second > 0 && currCol - it.second < 4))
            {
                if (currCol - it.second == 1)
                    currCol = it.second;
                else currCol = it.second + 4;
            }

        currCol = min(currCol, lines[currLine].size());
    }
}

void BackspaceKey(int& currLine, int& currCol, vector <string>& lines, int charsPerLine)
{
    if (currCol)
    {
        lines[currLine].erase(currCol - 1, currCol);

        int i = currLine;
        while (lines[i].size() + 1 == charsPerLine && i < lines.size() - 1)
        {
            lines[i].push_back(lines[i + 1].front());
            lines[i + 1].erase(lines[i].begin(), lines[i].begin() + 1);
            i++;
        }
        currCol--;
       // if(lines[i].empty())

    }
}