#ifndef L_FILE_H
#define L_FILE_H

#include "LuArray.h"

string* io_OpenDirectoryRecursive(const string dirPath);
string* io_OpenDirectory(const string dirPath);
string* io_LoadStrings(const string filePath);
void io_SaveStrings(const string filePath, const string* lines);
Array io_LoadBytes(const string filePath);
void io_SaveBytes(const string filePath, const Array* fileData);

#endif	// L_FILE_H
