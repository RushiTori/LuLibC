#ifndef LU_STRING_H
#define LU_STRING_H

#include "LuUtils.h"

bool str_IsDigit(char c);
bool str_IsLetter(char c);
bool str_IsSpace(char c);

bool str_Equals(const string str, const string other);
bool str_InsensitiveEquals(const string str, const string other);
bool str_EqualsRange(const string str, const string other, uint len);
bool str_InsensitiveEqualsRange(const string str, const string other, uint len);

bool str_Contains(const string src, char c);
bool str_ContainsString(const string src, const string seq);
uint str_IndexOf(const string src, char c);
uint str_LastIndexOf(const string src, char c);
uint str_CountChar(const string src, char c);

ulong str_StrToUInt(const string str, uchar base, ulong maxValue);

string* str_SplitTokens(const string toSplit, const string tokens);

string str_RemoveChars(string str, const string charsToRemove);
string str_Replace(string src, const string seq, const string replace);
string str_Concat(string dest, const string src);

char str_ToLowerChar(char c);
string str_ToLower(string src);
string str_ToLowerRange(string src, uint range);
char str_ToUpperChar(char c);
string str_ToUpper(string src);
string str_ToUpperRange(string src, uint range);

#endif	// LU_STRING_H
