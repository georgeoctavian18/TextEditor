#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include "graphics.h"
#pragma comment(lib,"graphics.lib")
#include "winbgim.h"
#include <ctime>
#include <chrono>
#include <fstream>

using namespace std;

#define CHAR_DIST 12
#define LINE_WIDTH 20
#define CHAR_WIDTH 12
#define CHAR_HEIGHT 20
#define CR 13
#define ESC 27
#define TAB 9
#define BS 8
#define windSizeX 800
#define windSizeY 600


void setOptions();

void StringToArray(string s, char arr[]);

void CharToString(string& s, char c);

void InitLine(int currLine, vector <string> &lines);

void Initialize(int& currLine, int& currCol, int& posX, int& posY, int& charsPerLine, int& rowsPerFrame, int& a, int& b, int& c, int& d);
