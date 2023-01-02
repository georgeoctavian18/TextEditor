#include "FileExplorer.h"

bool getOpenPath(char path[])
{
    strcpy(path, "\0");
    char filter[256] = "Text Files (*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0";
    tagOFNA info = { sizeof(OPENFILENAME), NULL, 0, filter, NULL, 0, 0, path, 1024, NULL, 0, NULL, NULL, 0 };
    tagOFNA* p = &info;
    return GetOpenFileNameA(p);
}

bool getSavePath(char path[])
{
    strcpy(path, "\0");
    char filter[256] = "Text Files (*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0";
    tagOFNA info = { sizeof(OPENFILENAME), NULL, 0, filter, NULL, 0, 0, path, 1024, NULL, 0, NULL, NULL, 0 };
    tagOFNA* p = &info;
    return GetSaveFileNameA(p);
}

void getNameFromPath(char path[], char name[])
{
    if (strlen(path) > 0)
        strcpy(name, strrchr(path, '\\') + 1);
    else
        strcpy(name, "\0");
}

