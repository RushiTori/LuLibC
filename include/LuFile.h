#ifndef LU_FILE_H
#define LU_FILE_H

#include "LuArray.h"

string* io_OpenDirectoryRecursive(const string dirPath);
string* io_OpenDirectory(const string dirPath);
string* io_LoadStrings(const string filePath);
void io_SaveStrings(const string filePath, const string* lines);
Arraybyte io_LoadBytes(const string filePath);
void io_SaveBytes(const string filePath, const Arraybyte* fileData);

#endif	// LU_FILE_H
