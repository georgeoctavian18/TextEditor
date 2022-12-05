#include "KeyActions.h"


void EnterKey(int& currLine, int& currCol, vector <string>& lines, vector <int>& enterLines, vector <pair<int, int>>& tabsLocation, int charsPerLine)
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
            InsertKey(line, col, lines, enterLines, tabsLocation, charsPerLine, lines[currLine][i]);

        lines[currLine].erase(currCol);
        currLine++;

    }

    currCol = 0;


}

void InsertKey(int& currLine, int& currCol, vector <string>& lines, vector <int>& enterLines, vector <pair<int, int>>& tabsLocation, int charsPerLine, char ch)
{
    int spacePos;
    bool isNextLine = false;

    string s;
    CharToString(s, ch);

    InitLine(currLine, lines);

    lines[currLine].insert(currCol, s);

    /*for (int i = 0; i < tabsLocation.size(); i++)
        if (tabsLocation[i].first == currLine && tabsLocation[i].second >= currCol)
        {
            if (tabsLocation[i].second + 4 == charsPerLine)
            {
                lines[currLine].erase(lines[currLine].begin() + tabsLocation[i].second + 1, lines[currLine].end());
                tabsLocation.erase(tabsLocation.begin() + i);
            }
            else tabsLocation[i].second++;
        }*/


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
                    /*if (lines[i + 1].size() == 1 && (lines[i + 1].front() >= ' ' && lines[i + 1].front() <= '~'))
                        lines[i + 1].insert(0, " ");
                    else if (lines[i + 1].size() > 1)
                        lines[i + 1].insert(0, " ");*/

                    while (j > spacePos)
                    {
                        CharToString(s, lines[i].back());
                        lines[i].erase(lines[i].end() - 1);
                        lines[i + 1].insert(0, s);
                        j--;
                    }

                   //lines[i].erase(lines[i].end() - 1);
                    //if (i == currLine) currCol--;
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

            /*for (int j = 0; j < tabsLocation.size(); j++)
                if (tabsLocation[j].first == i)
                {
                    if (tabsLocation[j].second + 4 == charsPerLine)
                    {
                        lines[i].erase(lines[i].begin() + tabsLocation[j].second + 1, lines[i].end());
                        tabsLocation.erase(tabsLocation.begin() + j);
                    }
                    else tabsLocation[j].second++;
                }*/
        }
    }
    else currCol++;

    if (isNextLine)
        currLine++;
}

void TabKey(int& currLine, int& currCol, vector <string>& lines, vector <int>& enterLines, vector <pair<int, int>>& tabsLocation, int charsPerLine)
{
    if (currCol + 3 > charsPerLine)
        currLine++, currCol = 0;

    //int cLine = currLine, cCol = currCol;

    InsertKey(currLine, currCol, lines, enterLines, tabsLocation, charsPerLine, ' ');
    InsertKey(currLine, currCol, lines, enterLines, tabsLocation, charsPerLine, ' ');
    InsertKey(currLine, currCol, lines, enterLines, tabsLocation, charsPerLine, ' ');
    InsertKey(currLine, currCol, lines, enterLines, tabsLocation, charsPerLine, ' ');

    //tabsLocation.push_back({ cLine, cCol });

}

void SpecialKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines, vector <pair<int, int>> tabsLocation)
{
    delay(15);
    int command = getch();
    if (command == KEY_RIGHT)
    {
       /*for (auto it : tabsLocation)
            if (it.first == currLine && it.second == currCol)
            {
                currCol += 4;
                if (currCol > charsPerLine)
                {
                    currCol -= charsPerLine;
                    currLine++;
                }
                return;
            }*/

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
        /*for (auto it : tabsLocation)
            if ((it.first == currLine && it.second + 4 == currCol))
            {
                currCol -= 4;
                if (currCol < 0)
                {
                    currCol += charsPerLine;
                    currLine--;
                }
                return;
            }
            */

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

        /*for (auto it : tabsLocation)
            if (it.first == currLine && (currCol - it.second > 0 && currCol - it.second < 4))
            {
                if (currCol - it.second == 1)
                    currCol = it.second;
                else currCol = it.second + 4;
            }*/

        currCol = (currCol<lines[currLine].size()?currCol:lines[currLine].size()); //min(currCol, lines[currLine].size());
    }
    else if (command == KEY_UP && currLine > 0)
    {
        currLine--;

       /*for (auto it : tabsLocation)
            if (it.first == currLine && (currCol - it.second > 0 && currCol - it.second < 4))
            {
                if (currCol - it.second == 1)
                    currCol = it.second;
                else currCol = it.second + 4;
            }*/

        currCol = (currCol<lines[currLine].size()?currCol:lines[currLine].size()); //min(currCol, lines[currLine].size());
    }
    else if (command == KEY_END)
    {
        if(currCol != lines[currLine].size())
             currCol = lines[currLine].size();
        else if (!count(enterLines.begin(), enterLines.end(), currLine) && currLine < lines.size() - 1)
        {
            currLine++;
            currCol = lines[currLine].size();
        }
    }
    else if (command == KEY_HOME)
    {
        if (currCol)
            currCol = 0;
        else if (!count(enterLines.begin(), enterLines.end(), currLine - 1) && currLine > 0)
        {
            currLine--;
            currCol = 0;
        }
    }
}

void BackspaceKey(int& currLine, int& currCol, vector <string>& lines, vector <int>& enterLines, int charsPerLine)
{
    bool ok = false;
    if (!currLine && !currCol) return;

    if (currCol)
    {
        lines[currLine].erase(lines[currLine].begin() + currCol - 1, lines[currLine].begin() + currCol);
        currCol--;
    }
    else
    {

        if (count(enterLines.begin(), enterLines.end(), currLine - 1))
        {
            auto pos = find(enterLines.begin(), enterLines.end(), currLine - 1);
            enterLines.erase(pos);
        }
        ok = true;
        currLine--;
        currCol = lines[currLine].size();
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
