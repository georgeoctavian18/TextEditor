#pragma once
#include "Initialization.h"

void DoWordWrap(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines);

void UndoWordWrap(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines);