#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cstring>
#include <algorithm>
#include "graphics.h"
#pragma comment(lib,"graphics.lib")
#include "winbgim.h"
#include <ctime>
#include <chrono>
#include <fstream>

using namespace std;

#define CHAR_DIST 14
#define LINE_WIDTH 20
#define CHAR_WIDTH 14
#define CHAR_HEIGHT 20
#define CR 13
#define ESC 27
#define TAB 9
#define BS 8
#define CTRLX 'X'-'A'+1
#define CTRLC 'C'-'A'+1
#define CTRLV 'V'-'A'+1
#define CTRLA 'A'-'A'+1
#define CTRLZ 'Z'-'A'+1
#define windSizeX 800
#define windSizeY 600


void setOptions();

void StringToArray(string& s, char arr[]);

void CharToString(string& s, char c);

void InitLine(int currLine, vector <string>& lines);

void Initialize(int& currLine, int& currCol, int& posX, int& posY, int& charsPerLine, int& rowsPerFrame, int& a, int& b, int& c, int& d);