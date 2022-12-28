#define _CRT_SECURE_NO_WARNINGS
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
#include <windows.h>

using namespace std;


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
#define CTRLS 'S'-'A'+1
#define CTRLN 'N'-'A'+1




void setOptions(int windSizeX, int windSizeY);

void StringToArray(string& s, char arr[]);

void CharToString(string& s, char c);

void InitLine(int currLine, vector <string>& lines);

void Initialize(int& currLine, int& currCol, int& posX, int& posY, int& charsPerLine, int& rowsPerFrame, int& a, int& b, int& c, int& d, int windSizeX, int windSizeY);
