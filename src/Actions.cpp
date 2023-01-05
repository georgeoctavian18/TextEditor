#include "Actions.h"



void Deletion(int selectBeginLine, int selectBeginCol, int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, stack <vector<string>>& stackLines, stack <vector<int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool wordWrap, bool& isSaved)
{
    if (!currLine && !currCol && !selectBeginLine && !selectBeginCol) return;

    isSaved = false;
    stackLines.push(lines);
    stackEnterLines.push(enterLines);
    stackLinCol.push({ currLine, currCol });

    if (selectBeginLine > currLine || (selectBeginLine == currLine && selectBeginCol > currCol))
    {
        swap(selectBeginLine, currLine);
        swap(selectBeginCol, currCol);
    }
    else if (selectBeginLine == currLine && selectBeginCol == currCol)
    {
        BackspaceKey(currLine, currCol, charsPerLine, lines, enterLines, wordWrap);
        return;
    }

    while (currLine != selectBeginLine || currCol != selectBeginCol && currCol > 0)
        BackspaceKey(currLine, currCol, charsPerLine, lines, enterLines, wordWrap);
}

void Insertion(int selectBeginLine, int selectBeginCol, int& currLine, int& currCol, int charsPerLine, char ch, vector <string>& lines, vector <int>& enterLines, stack <vector<string>>& stackLines, stack <vector<int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool wordWrap, bool& isSaved)
{
    isSaved = false;
    stackLines.push(lines);
    stackEnterLines.push(enterLines);
    stackLinCol.push({ currLine, currCol });

    if (selectBeginLine > currLine || (selectBeginLine == currLine && selectBeginCol > currCol))
    {
        swap(selectBeginLine, currLine);
        swap(selectBeginCol, currCol);
    }

    while (currLine != selectBeginLine || currCol != selectBeginCol)
        BackspaceKey(currLine, currCol, charsPerLine, lines, enterLines, wordWrap);

    if (ch == '\t')
        TabKey(currLine, currCol, charsPerLine, lines, enterLines, wordWrap);
    else InsertKey(currLine, currCol, charsPerLine, ch, lines, enterLines, wordWrap);
}

void Copy(int selectBeginLine, int selectBeginCol, int currLine, int currCol, vector <string>& lines, vector <int>& enterLines, vector <string>& copiedLines, vector <int>& enterLinesCopied, bool& keepSelect)
{
    if (currLine == selectBeginLine && currCol == selectBeginCol) return;

    copiedLines.clear();
    enterLinesCopied.clear();

    keepSelect = true;

    if (selectBeginLine > currLine || (selectBeginLine == currLine && selectBeginCol > currCol))
    {
        swap(selectBeginLine, currLine);
        swap(selectBeginCol, currCol);
    }

    if (selectBeginLine == currLine)
        copiedLines.push_back(lines[currLine].substr(selectBeginCol, currCol - selectBeginCol));
    else
    {
        copiedLines.push_back(lines[selectBeginLine].substr(selectBeginCol));

        if (count(enterLines.begin(), enterLines.end(), selectBeginLine))
            enterLinesCopied.push_back(copiedLines.size() - 1);

        int i = selectBeginLine + 1;

        while (i != currLine)
        {
            copiedLines.push_back(lines[i]);

            if (count(enterLines.begin(), enterLines.end(), i))
                enterLinesCopied.push_back(copiedLines.size() - 1);

            i++;
        }

        string s;

        for (int j = 0; j < currCol; j++)
            s.push_back(lines[i][j]);

        copiedLines.push_back(s);
    }
}

void Paste(int selectBeginLine, int selectBeginCol, int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, vector <string>& copiedLines, vector <int>& enterLinesCopied, stack <vector<string>>& stackLines, stack <vector<int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool wordWrap, bool& isSaved)
{
    isSaved = false;
    stackLines.push(lines);
    stackEnterLines.push(enterLines);
    stackLinCol.push({ currLine, currCol });

    if (selectBeginLine > currLine || (selectBeginLine == currLine && selectBeginCol > currCol))
    {
        swap(selectBeginLine, currLine);
        swap(selectBeginCol, currCol);
    }

    if (selectBeginLine != currLine || selectBeginCol != currCol)
    {
        Deletion(selectBeginLine, selectBeginCol, currLine, currCol, charsPerLine, lines, enterLines, stackLines, stackEnterLines, stackLinCol, wordWrap, isSaved);
        stackLines.pop(); stackEnterLines.pop(); stackLinCol.pop();
    }

    for (int i = 0; i < copiedLines.size(); i++)
    {
        for(int j = 0; j < copiedLines[i].size(); j++)
            InsertKey(currLine, currCol, charsPerLine, copiedLines[i][j], lines, enterLines, wordWrap);

        if(count(enterLinesCopied.begin(), enterLinesCopied.end(), i))
            EnterKey(currLine, currCol, charsPerLine, lines, enterLines, wordWrap);
    }
}

void Cut(int selectBeginLine, int selectBeginCol, int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, vector <string>& copiedLines, vector <int>& enterLinesCopied, stack <vector<string>>& stackLines, stack <vector<int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool wordWrap, bool keepSelect, bool& isSaved)
{
    if (selectBeginLine != currLine || selectBeginCol != currCol)
    {
        Copy(selectBeginLine, selectBeginCol, currLine, currCol, lines, enterLines, copiedLines, enterLinesCopied, keepSelect);
        Deletion(selectBeginLine, selectBeginCol, currLine, currCol, charsPerLine, lines, enterLines, stackLines, stackEnterLines, stackLinCol, wordWrap, isSaved);
    }
}

void SelectAll(int& selectBeginLine, int& selectBeginCol, int& currLine, int& currCol, vector <string> lines, bool& keepSelect)
{
    if (!lines.empty())
    {
        keepSelect = true;
        selectBeginLine = selectBeginCol = 0;
        currLine = lines.size() - 1;
        currCol = lines[currLine].size();
    }
}

void Undo(int& currLine, int& currCol, vector <string>& lines, vector <int>& enterLines, stack <vector<string>>& stackLines, stack <vector<int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool& isSaved)
{
    if (!stackLines.empty())
    {
        isSaved = false;
        lines = stackLines.top();
        stackLines.pop();
        enterLines = stackEnterLines.top();
        stackEnterLines.pop();
        currLine = stackLinCol.top().first;
        currCol = stackLinCol.top().second;
        stackLinCol.pop();
    }
}
