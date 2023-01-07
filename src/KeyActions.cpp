#include "KeyActions.h"


void EnterKey(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, bool wordWrap)
{
    string s;

    InitLine(currLine, lines);

    for (int i = 0; i < enterLines.size(); i++)
        if (enterLines[i] >= currLine)
            enterLines[i]++;
    enterLines.push_back(currLine);

    if (count(enterLines.begin(), enterLines.end(), currLine + 1) || !wordWrap)
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
            InsertKey(line, col, charsPerLine, lines[currLine][i], lines, enterLines, wordWrap);

        if(currCol < lines[currLine].size())
            lines[currLine].erase(currCol);

        currLine++;
    }
    InitLine(currLine, lines);
    currCol = 0;

}

void InsertKey(int& currLine, int& currCol, int charsPerLine, char ch, vector <string>& lines, vector <int>& enterLines, bool wordWrap)
{
    int spacePos, tabPos, position = -1;
    bool isNextLine = false;

    string s;
    CharToString(s, ch);

    InitLine(currLine, lines);

    lines[currLine].insert(currCol, s);

    if(!wordWrap)
    {
        currCol++;
        return;
    }

    if (lines[currLine].size() > charsPerLine)
    {
        int i = currLine;
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

            if (i == currLine)
            {
                if (currCol > position && position != -1)
                {
                    currCol -= position;
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

void BackspaceKey(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, bool wordWrap)
{
    bool ok = false;
    if (!currLine && !currCol) return;

    if (wordWrap)
    {
        bool tab = false;
        if (currCol)
        {
            if(lines[currLine][currCol - 1] == '\t')
                lines[currLine].erase(lines[currLine].begin() + currCol - 4, lines[currLine].begin() + currCol),  tab = true;
            else lines[currLine].erase(lines[currLine].begin() + currCol - 1, lines[currLine].begin() + currCol);

            if (!lines[currLine].size() && currLine)
                currCol = lines[--currLine].size();
            else
            {
                if (tab) currCol -= 4;
                else currCol--;
            }
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
                if (lines[currLine - 1][currCol - 1] == '\t')
                    lines[currLine - 1].erase(lines[currLine - 1].begin() + currCol - 4, lines[currLine - 1].begin() + currCol), tab = true;
                else lines[currLine - 1].erase(lines[currLine - 1].begin() + currCol - 1, lines[currLine - 1].begin() + currCol);

                if (tab) currCol -= 4;
                else currCol--;
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

                for (int j = 0; j < enterLines.size(); j++)
                    if (enterLines[j] > i)
                        enterLines[j]--;

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
                    if (lines[i + 1][j] == ' ' && !j)
                        j = 1;
                    else if (lines[i + 1][j] == '\t' && !j)
                        j = 4;
                    else while (lines[i + 1][j] != ' ' && j < lines[i + 1].size())
                        j++;

                    if (j <= emptySpace)
                    {
                        string s = lines[i + 1].substr(0, j);
                        lines[i + 1].erase(lines[i + 1].begin(), lines[i + 1].begin() + j);
                        lines[i] += s;
                        j = 0;
                    }
                    else if (lines[i].back() != ' ' && lines[i].back() != '\t' && lines[i + 1].front() != ' ' && lines[i + 1].front() != '\t' )
                    {
                        int wordStart = lines[i].size() - 1;

                        while (wordStart >= 0 && lines[i][wordStart] != ' ' && lines[i][wordStart] != '\t')
                            wordStart--;

                        if (wordStart < 0 && emptySpace >= 0)
                        {
                            string s = lines[i + 1].substr(0, emptySpace);
                            lines[i + 1].erase(lines[i + 1].begin(), lines[i + 1].begin() + emptySpace);
                            lines[i] += s;
                        }
                        else if (wordStart >= 0)
                        {
                            int line = i + 1, col = 0;

                            for(int p = wordStart + 1; p < lines[i].size(); p++)
                                InsertKey(line, col, charsPerLine, lines[i][p], lines, enterLines, wordWrap);

                            currLine = i + 1;
                            currCol = lines[i].size() - wordStart - 1;

                            lines[i].erase(lines[i].begin() + wordStart + 1, lines[i].end());

                        }

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
    else
    {
        bool tab = false;
        if (currCol)
        {
            if (lines[currLine][currCol - 1] == '\t')
                lines[currLine].erase(lines[currLine].begin() + currCol - 4, lines[currLine].begin() + currCol), tab = true;
            else lines[currLine].erase(lines[currLine].begin() + currCol - 1, lines[currLine].begin() + currCol);

            if (tab) currCol -= 4;
            else currCol--;
        }
        else
        {
            if (count(enterLines.begin(), enterLines.end(), currLine - 1))
            {
                auto pos = find(enterLines.begin(), enterLines.end(), currLine - 1);
                enterLines.erase(pos);
            }
            else
            {
                if (lines[currLine - 1][currCol - 1] == '\t')
                    lines[currLine - 1].erase(lines[currLine - 1].begin() + currCol - 4, lines[currLine - 1].begin() + currCol), tab = true;
                else lines[currLine - 1].erase(lines[currLine - 1].begin() + currCol - 1, lines[currLine - 1].begin() + currCol);

                if (tab) currCol -= 4;
                else currCol--;
            }

            ok = true;
            currLine--;
            currCol = lines[currLine].size();
        }

        int i = currLine;

        if (ok)
        {

            while (lines[i + 1].size())
            {
                char ch = lines[i + 1].front();
                lines[i].push_back(ch);
                lines[i + 1].erase(lines[i + 1].begin(), lines[i + 1].begin() + 1);
            }
            if (!lines[i + 1].size())
            {
                for (int j = i + 1; j < lines.size() - 1; j++)
                    lines[j].replace(lines[j].begin(), lines[j].end(), lines[j + 1]);

                for (int j = 0; j < enterLines.size(); j++)
                    if (enterLines[j] > i)
                        enterLines[j]--;

                lines.pop_back();
            }
        }

    }

}

void TabKey(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, bool wordWrap)
{
    if (currCol + 4 > charsPerLine && wordWrap)
        currLine++, currCol = 0;

    InsertKey(currLine, currCol, charsPerLine, '\t', lines, enterLines, wordWrap);
    InsertKey(currLine, currCol, charsPerLine, '\t', lines, enterLines, wordWrap);
    InsertKey(currLine, currCol, charsPerLine, '\t', lines, enterLines, wordWrap);
    InsertKey(currLine, currCol, charsPerLine, '\t', lines, enterLines, wordWrap);
}

void SpecialKey(int& selectBeginLine, int& selectBeginCol, int& currLine, int& currCol, int command, int charsPerLine, vector <string> &lines, vector <int>& enterLines, vector <string>& copiedLines, vector <int>& enterLinesCopied, stack <vector<string>>& stackLines, stack <vector<int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool& wordWrap, bool& keepSelect, bool& isSaved, int& font)
{
    int selectEndLine = currLine, selectEndCol = currCol;

    if (command == KEY_RIGHT || command == KEY_LEFT)
        if (selectBeginLine > selectEndLine || (selectBeginLine == selectEndLine && selectBeginCol > selectEndCol))
        {
            swap(selectBeginLine, selectEndLine);
            swap(selectBeginCol, selectEndCol);
        }

    switch (command)
    {
    case KEY_RIGHT:
        if (selectBeginCol != selectEndCol || selectBeginLine != selectEndLine)
            currCol = selectEndCol, currLine = selectEndLine;
        else RightArrowKey(currLine, currCol, charsPerLine, lines, enterLines);
        break;
    case KEY_LEFT:
        if (selectBeginCol != selectEndCol || selectBeginLine != selectEndLine)
            currCol = selectBeginCol, currLine = selectBeginLine;
        else LeftArrowKey(currLine, currCol, charsPerLine, lines, enterLines);
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
        DeleteKey(selectBeginLine, selectBeginCol, currLine, currCol, charsPerLine, lines, enterLines, stackLines, stackEnterLines, stackLinCol, wordWrap, isSaved);
        break;
    case KEY_F5:
        DateTimeKey(currLine, currCol, charsPerLine, lines, enterLines, wordWrap, isSaved);
        break;
    case KEY_F1:
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        font = DEFAULT_FONT;
        break;
    case KEY_F2:
        settextstyle(BOLD_FONT, HORIZ_DIR, 2);
        font = BOLD_FONT;
        break;
    case KEY_F3:
        settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
        font = COMPLEX_FONT;
        break;
    }
}

void RightArrowKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines)
{
    if (currCol == lines[currLine].size() && lines.size() != currLine + 1)
    {
        currCol = 0;
        currLine++;
    }
    else if (currCol != lines[currLine].size())
    {
        if (lines[currLine][currCol] == '\t')
            currCol += 4;
        else currCol++;
    }
}

void LeftArrowKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines)
{
    if (currCol == 0)
    {
        if (currLine > 0)
        {
            currLine--;
            currCol = lines[currLine].size();

            //if (!count(enterLines.begin(), enterLines.end(), currLine) && lines[currLine][currCol - 1] != ' ' )
              //  currCol--;
        }
    }
    else
    {
        if (lines[currLine][currCol - 1] == '\t')
            currCol -= 4;
        else currCol--;
    }
}

void UpArrowKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines)
{
    if (currLine > 0)
    {
        currLine--;
        currCol = currCol < lines[currLine].size() ? currCol : lines[currLine].size();

        if (lines[currLine][currCol] == '\t')
        {
            int j = currCol, nrRgt = 0, nrLft = 0;
            while (j < lines[currLine].size() && lines[currLine][j] == '\t')
                j++, nrRgt++;


            j = currCol - 1;
            while (j >= 0 && lines[currLine][j] == '\t')
                j--, nrLft++;

            nrRgt = nrRgt - 4 * (nrRgt / 4);
            nrLft = nrLft - 4 * (nrLft / 4);

            if (nrRgt < nrLft)
                currCol += nrRgt;
            else currCol -= nrLft;
        }
    }
}

void DownArrowKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines)
{
    if (lines.size() != currLine + 1)
    {
        currLine++;
        currCol = currCol < lines[currLine].size() ? currCol : lines[currLine].size();

        if (lines[currLine][currCol] == '\t')
        {
            int j = currCol, nrRgt = 0, nrLft = 0;
            while (j < lines[currLine].size() && lines[currLine][j] == '\t')
                j++, nrRgt++;


            j = currCol - 1;
            while (j >= 0 && lines[currLine][j] == '\t')
                j--, nrLft++;

            nrRgt = nrRgt - 4 * (nrRgt / 4);
            nrLft = nrLft - 4 * (nrLft / 4);

            if (nrRgt < nrLft)
                currCol += nrRgt;
            else currCol -= nrLft;
        }
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

void DeleteKey(int selectBeginLine, int selectBeginCol, int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, stack <vector<string>>& stackLines, stack <vector<int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool wordWrap, bool& isSaved)
{
    if (selectBeginLine == currLine && selectBeginCol == currCol)
    {
        int last = currCol;

        stackLines.push(lines);
        stackEnterLines.push(enterLines);
        stackLinCol.push({ currLine, currCol });

        RightArrowKey(currLine, currCol, charsPerLine, lines, enterLines);

        if (last != currCol)
            Deletion(selectBeginLine, selectBeginCol, currLine, currCol, charsPerLine, lines, enterLines, stackLines, stackEnterLines, stackLinCol, wordWrap, isSaved);
    }
    else Deletion(selectBeginLine, selectBeginCol, currLine, currCol, charsPerLine, lines, enterLines, stackLines, stackEnterLines, stackLinCol, wordWrap, isSaved);
}

void DateTimeKey(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, bool wordWrap, bool& isSaved)
{
    char buffer[32];
    time_t now = time(NULL);
    tm* ptm = localtime(&now);
    isSaved = true;
    // Format: Mo, 20:20 15/06/2009
    strftime(buffer, 32, "%a, %H:%M %d/%m/%Y", ptm);

    for (int i = 0; i < strlen(buffer); i++)
        InsertKey(currLine, currCol, charsPerLine, buffer[i], lines, enterLines, wordWrap);
}
