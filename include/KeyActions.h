#pragma once
#include "Initialization.h"
#include "Print.h"
#include "SettingsActions.h"
#include "Actions.h"



void EnterKey(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, bool wordWrap);

void SpecialKey(int& selectBeginLine, int& selectBeginCol, int& currLine, int& currCol, int command, int charsPerLine, vector <string> &lines, vector <int>& enterLines, vector <string>& copiedLines, vector <int>& enterLinesCopied, stack <vector<string>>& stackLines, stack <vector<int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool& wordWrap, bool& keepSelect, bool& isSaved, int& font);

void RightArrowKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines);

void LeftArrowKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines);

void UpArrowKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines);

void DownArrowKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines);

void HomeKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines);

void EndKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <int> enterLines);

void TabKey(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, bool wordWrap);

void DeleteKey(int selectBeginLine, int selectBeginCol, int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, stack <vector<string>>& stackLines, stack <vector<int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool wordWrap, bool& isSaved);

void InsertKey(int& currLine, int& currCol, int charsPerLine, char ch, vector <string>& lines, vector <int>& enterLines, bool wordWrap);

void BackspaceKey(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, bool wordWrap);

void DateTimeKey(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, bool wordWrap, bool& isSaved);
