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