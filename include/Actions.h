#pragma once
#include "KeyActions.h"
#include "Initialization.h"


void Deletion(int selectBeginLine, int selectBeginCol, int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, stack <vector<string>>& stackLines, stack <vector<int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool wordWrap, bool& isSaved);

void Insertion(int selectBeginLine, int selectBeginCol, int& currLine, int& currCol, int charsPerLine, char ch, vector <string>& lines, vector <int>& enterLines, stack <vector<string>>& stackLines, stack <vector<int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool wordWrap, bool& isSaved);

void Copy(int selectBeginLine, int selectBeginCol, int currLine, int currCol, vector <string>& lines, vector <int>& enterLines, vector <string>& copiedLines, vector <int>& enterLinesCopied, bool& keepSelect);

void Paste(int selectBeginLine, int selectBeginCol, int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, vector <string>& copiedLines, vector <int>& enterLinesCopied, stack <vector<string>>& stackLines, stack <vector<int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool wordWrap, bool& isSaved);

void Cut(int selectBeginLine, int selectBeginCol, int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, vector <string>& copiedLines, vector <int>& enterLinesCopied, stack <vector<string>>& stackLines, stack <vector<int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool wordWrap, bool keepSelect, bool& isSaved);

void SelectAll(int& selectBeginLine, int& selectBeginCol, int& currLine, int& currCol, vector <string> lines, bool& keepSelect);

void Undo(int& currLine, int& currCol, vector <string>& lines, vector <int>& enterLines, stack <vector<string>>& stackLines, stack <vector<int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool& isSaved);