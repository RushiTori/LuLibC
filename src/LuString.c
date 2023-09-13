
#include <LuArray.h>
#include <LuString.h>

bool str_IsDigit(char c) { return ((c >= '0') && (c <= '9')); }

bool str_IsLetter(char c) { return (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'))); }

bool str_IsSpace(char c) {
	return ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\v') || (c == '\f') || (c == '\r'));
}

bool str_Equals(const string str, const string other) {
	if (!str || !other) return false;
	if (str == other) return true;
	uint strLen = strlen(str);
	uint otherLen = strlen(other);
	if (strLen != otherLen) return false;

	for (uint i = 0; i < strLen; i++) {
		if (str[i] != other[i]) return false;
	}

	return true;
}

bool str_InsensitiveEquals(const string str, const string other) {
	if (!str || !other) return false;
	if (str == other) return true;
	uint strLen = strlen(str);
	uint otherLen = strlen(other);
	if (strLen != otherLen) return false;

	for (uint i = 0; i < strLen; i++) {
		if (str_ToLowerChar(str[i]) != str_ToLowerChar(other[i])) return false;
	}

	return true;
}

bool str_EqualsRange(const string str, const string other, uint len) {
	if (!str || !other) return false;
	if (str == other) return true;
	if (strlen(str) < len) return false;
	if (strlen(other) < len) return false;

	for (uint i = 0; i < len; i++) {
		if (str[i] != other[i]) return false;
	}

	return true;
}

bool str_InsensitiveEqualsRange(const string str, const string other, uint len) {
	if (!str || !other) return false;
	if (str == other) return true;
	if (strlen(str) < len) return false;
	if (strlen(other) < len) return false;

	for (uint i = 0; i < len; i++) {
		if (str_ToLowerChar(str[i]) != str_ToLowerChar(other[i])) return false;
	}

	return true;
}

bool str_Contains(const string src, char c) {
	if (!src) return false;
	uint srcLen = strlen(src);
	if (!srcLen) return false;

	for (uint i = 0; i < srcLen; i++) {
		if (src[i] == c) return true;
	}

	return false;
}

bool str_ContainsString(const string src, const string seq) {
	if (!src) return false;
	uint srcLen = strlen(src);
	if (!srcLen) return false;

	if (!seq) return false;
	uint seqLen = strlen(seq);
	if (!seqLen) return true;

	if (seqLen > srcLen) return false;

	for (uint i = 0; i < srcLen - seqLen; i++) {
		bool foundSeq = true;
		for (uint j = 0; j < seqLen; j++) {
			if (src[i + j] != seq[j]) {
				foundSeq = false;
				break;
			}
		}
		if (foundSeq) return true;
	}

	return false;
}

uint str_IndexOf(const string src, char c) {
	if (!str_Contains(src, c)) return -1;
	uint srcLen = strlen(src);

	for (uint i = 0; i < srcLen; i++) {
		if (src[i] == c) return i;
	}

	return -1;
}

uint str_LastIndexOf(const string src, char c) {
	if (!str_Contains(src, c)) return -1;
	uint srcLen = strlen(src);

	for (int i = srcLen; i >= 0; i--) {
		if (src[i] == c) return i;
	}

	return -1;
}

ulong str_StrToUInt(const string str, uchar base, ulong maxValue) {
	if (!str) return 0;
	uint len = strlen(str);
	if (!len) return 0;
	if ((base == 0) || (base > 36)) return 0;
	if (!maxValue) maxValue = UINT64_MAX;

	ulong result = 0;
	for (uint i = 0; i < len; i++) {
		if (str[i] == '\'') continue;

		uchar charValue = 0;
		if (str_IsDigit(str[i])) {
			charValue = str[i] - '0';
		} else if (str_IsLetter(str[i])) {
			charValue = str_ToLowerChar(str[i]) - 'a' + 10;
		} else {
			printf("StrToUInt : \'%c\' is not recognised a valid digit. returning : %lu\n", str[i], result);
			break;
		}

		if (charValue >= base) {
			printf("StrToUInt : \'%c\' is not a valid digit for base %d. returning : %lu\n", str[i], base, result);
			break;
		}

		ulong temp = maxValue - charValue;
		temp /= base;
		if (result > temp) {
			printf("StrToUInt : counting \'%c\' would go past %lu. returning : %lu\n", str[i], maxValue, result);
			break;
		}
		result *= base;
		result += charValue;
	}

	return result;
}

string* str_SplitTokens(const string src, const string tokens) {
	if (!src || !tokens) return NULL;

	uint srcLen = strlen(src);
	uint tokensLen = strlen(tokens);

	if (!srcLen || !tokensLen) return NULL;
	
	int splitCount = 1;
	string* subStrings = calloc(1, sizeof(string));

	for (uint i = 0; i < srcLen; i++) {
		if (str_Contains(tokens, src[i])) continue;

		uint j = i;
		while (j < srcLen) {
			if (str_Contains(tokens, src[j])) break;
			j++;
		}

		uint subLen = j - i;
		string sub = calloc(subLen + 1, sizeof(char));
		memcpy(sub, src + i, subLen);
		subStrings[splitCount - 1] = sub;

		splitCount++;
		subStrings = realloc(subStrings, splitCount * sizeof(string));
		subStrings[splitCount - 1] = NULL;

		i = j - 1;
	}

	return subStrings;
}

string str_Replace(string src, const string seq, const string replace) {
	if (!src) return NULL;
	const uint srcLen = strlen(src);
	if (!srcLen) return NULL;

	if (!seq) return NULL;
	const uint seqLen = strlen(seq);
	if (!seqLen) return NULL;

	if (!replace) return NULL;
	const uint replaceLen = strlen(replace);
	if (!replaceLen) return NULL;

	if (!str_ContainsString(src, seq)) return NULL;

	Array result = arr_Create(sizeof(char), 0, false, false);

	arr_Reserve(&result, srcLen + 1);
	for (uint i = 0; i < srcLen; i++) {
		bool isSeq = true;
		for (uint j = 0; j < seqLen; j++) {
			if (i + j >= srcLen) {
				isSeq = false;
				break;
			}
			if (src[i + j] != seq[j]) {
				isSeq = false;
				break;
			}
		}
		if (isSeq) {
			arr_Push(&result, replace, replaceLen);
			i += seqLen;
			i--;
		} else {
			arr_Push(&result, src + i, 1);
		}
	}

	arr_Push(&result, "\0", 1);
	arr_ShrinkToFit(&result);
	return result.data;
}

string str_RemoveChars(string str, const string charsToRemove) {
	if (!str) return str;
	if (!charsToRemove) return str;
	if (!strlen(charsToRemove)) return str;

	uint idx = 0;
	const uint len = strlen(str);
	for (uint i = 0; i < len; i++) {
		char temp = str[i];
		str[i] = '\0';
		if (str_Contains(charsToRemove, temp)) continue;
		str[idx] = temp;
		idx++;
	}

	return str;
}

string str_Concat(string dest, const string src) {
	if (!src) return dest;
	if (!strlen(src)) return dest;
	if (!dest) return dest;

	memmove(dest + strlen(dest), src, strlen(src));

	return dest;
}

char str_ToLowerChar(char c) { return ((c >= 'A') && (c <= 'Z')) ? c + ('a' - 'A') : c; }

string str_ToLower(string src) {
	if (!src) return src;
	uint len = strlen(src);

	for (uint i = 0; i < len; i++) {
		src[i] = str_ToLowerChar(src[i]);
	}

	return src;
}

string str_ToLowerRange(string src, uint range) {
	if (!src) return src;

	uint len = strlen(src);
	if (range) len = min(len, range);

	for (uint i = 0; i < len; i++) {
		src[i] = str_ToLowerChar(src[i]);
	}

	return src;
}

char str_ToUpperChar(char c) { return ((c >= 'a') && (c <= 'z')) ? c + ('A' - 'a') : c; }

string str_ToUpper(string src) {
	if (!src) return src;
	uint len = strlen(src);

	for (uint i = 0; i < len; i++) {
		src[i] = str_ToUpperChar(src[i]);
	}

	return src;
}

string str_ToUpperRange(string src, uint range) {
	if (!src) return src;

	uint len = strlen(src);
	if (range) len = min(len, range);

	for (uint i = 0; i < len; i++) {
		src[i] = str_ToUpperChar(src[i]);
	}

	return src;
}
