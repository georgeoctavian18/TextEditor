#pragma once
#include "Initialization.h"
#include "FileExplorer.h"
#include "KeyActions.h"
//#include "user_interaction.h"

void DoWordWrap(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines);

void UndoWordWrap(int& currLine, int& currCol, int charsPerLine, vector <string>& lines, vector <int>& enterLines);

void OpenFile(int& currLine, int& currCol, int charsPerLine, char path[], char fileName[], vector <string>& lines, vector <int>& enterLines, stack <vector <string>>& stackLines, stack <vector <int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool wordWrap, bool& isSaved);

void SaveAsFile(int& currLine, int& currCol, int charsPerLine, char path[], char fileName[], vector <string>& lines, vector <int>& enterLines, bool wordWrap, bool& isSaved);

void SaveFile(int& currLine, int& currCol, int charsPerLine, char path[], char fileName[], vector <string>& lines, vector <int>& enterLines, bool wordWrap, bool& isSaved);

void NewFile(int& currLine, int& currCol, int charsPerLine, char path[], char fileName[], vector <string>& lines, vector <int>& enterLines, stack <vector <string>>& stackLines, stack <vector <int>>& stackEnterLines, stack <pair<int, int>>& stackLinCol, bool wordWrap, bool& isSaved);

void SelectWindowSize(int& windSizeX, int& windSizeY, palette theme, int lang, const char projectDir[]);

void ResizeWindow(int& currLine, int& currCol, int windSizeX, int windSizeY, int posX, int& posY, int& rowsPerFrame, int& charsPerLine, int& lineBeginFrame, int& lineEndFrame, int& colBeginFrame, int& colEndFrame, bool wordWrap, vector <string>& lines, vector <int>& enterLines);
