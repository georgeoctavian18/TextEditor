#pragma once
#include "Initialization.h"
#include "FileExplorer.h"
#include "KeyActions.h"

void DoWordWrap(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines);

void UndoWordWrap(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines);

void OpenFile(int& currLine, int& currCol, int charsPerLine, char path[], char fileName[], vector <string>& lines, vector <int>& enterLines, bool wordWrap);

void SaveAsFile(int& currLine, int& currCol, int charsPerLine, char path[], char fileName[], vector <string>& lines, vector <int>& enterLines, bool wordWrap, bool& isSaved);

void SaveFile(int& currLine, int& currCol, int charsPerLine, char path[], char fileName[], vector <string>& lines, vector <int>& enterLines, bool wordWrap, bool& isSaved);

void NewFile(int& currLine, int& currCol, int charsPerLine, char path[], char fileName[], vector <string>& lines, vector <int>& enterLines, bool wordWrap, bool& isSaved);