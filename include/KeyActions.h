#pragma once
#include "Initialization.h"
#include "Print.h"



void EnterKey(int& currLine, int& currCol, vector <string>& lines, vector <int>& enterLines);

void ArrowKey(int& currLine, int& currCol, int charsPerLine, vector <string> lines, vector <pair<int, int>> tabsLocation);

void TabKey(int& currLine, int& currCol, vector <string>& lines, vector <int>& enterLines, vector <pair<int, int>>& tabsLocation, int charsPerLine);

void InsertKey(int& currLine, int& currCol, vector <string>& lines, vector <int>& enterLines, vector <pair<int, int>>& tabsLocation, int charsPerLine, char ch);

void BackspaceKey(int& currLine, int& currCol, vector <string>& lines, int charsPerLine);