#include "SettingsActions.h"



void DoWordWrap(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines)
{
    int spacePos, i = currLine;
    string s;

    currLine = currCol = 0;

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
        i++;
    }
}