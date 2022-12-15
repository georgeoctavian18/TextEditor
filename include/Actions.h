#pragma once
#include "KeyActions.h"
#include "Initialization.h"


void Deletion(int selectBeginLine, int selectBeginCol, int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines, bool wordWrap);

void Insertion(int selectBeginLine, int selectBeginCol, int& currLine, int& currCol, int charsPerLine, char ch, vector <string>& lines, vector <int>& enterLines, bool wordWrap);
