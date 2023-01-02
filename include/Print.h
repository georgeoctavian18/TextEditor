#pragma once
#include "Initialization.h"

void PrintCursor(int x1, int y1, int x2, int y2, int color);

void PrintText(int posX, int posY, int beginFrame, int endFrame, int colBeginFrame, int colEndFrame, int selectBeginLine, int selectBeginCol, int selectEndLine, int selectEndCol, int font, vector <string> lines, palette* theme);
