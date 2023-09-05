#include <LuFile.h>
#include <LuString.h>
#include <dirent.h>
#include <errno.h>

string* io_OpenDirectoryRecursive(const string dirPath) {
	string* paths = io_OpenDirectory(dirPath);
	uint pathsCount = 1;

	for (uint i = 0; paths[i]; i++) {
		pathsCount++;
	}

	if (!paths) return NULL;

	for (uint i = 0; paths[i]; i++) {
		DIR* pDir;

		pDir = opendir(paths[i]);
		if (pDir == NULL) continue;

		struct dirent* pDirent;
		while ((pDirent = readdir(pDir)) != NULL) {
			if ((strcmp(pDirent->d_name, ".") == 0) || (strcmp(pDirent->d_name, "..") == 0)) {
				continue;
			}

			paths[pathsCount - 1] = calloc(strlen(paths[i]) + 1 + FILENAME_MAX, sizeof(char));
			memcpy(paths[pathsCount - 1], paths[i], strlen(paths[i]));
			paths[pathsCount - 1][strlen(paths[pathsCount - 1])] = '/';
			memcpy((paths[pathsCount - 1]) + strlen(paths[pathsCount - 1]), pDirent->d_name, FILENAME_MAX - 1);
			pathsCount++;
			paths = realloc(paths, pathsCount * sizeof(string));
			paths[pathsCount - 1] = NULL;
		}
	}

	return paths;
}

string* io_OpenDirectory(const string dirPath) {
	if (!dirPath) {
		fprintf(stderr, "File I/O Error: Directory Path Passed Is Nullptr\n");
		return NULL;
	}

	if (!strlen(dirPath)) {
		fprintf(stderr, "File I/O Error: Directory Path Passed Is Empty\n");
		return NULL;
	}

	DIR* pDir;

	pDir = opendir(dirPath);
	if (pDir == NULL) {
		fprintf(stderr, "File I/O Error: Could Not Open Directory%s\n\t\"%s\"\n",
				((errno == EACCES) ? " (Not Enough Perms)" : ""), dirPath);
		return NULL;
	}

	string* paths = calloc(1, sizeof(string));
	uint pathsCount = 1;

	struct dirent* pDirent;
	while ((pDirent = readdir(pDir)) != NULL) {
		if ((strcmp(pDirent->d_name, ".") == 0) || (strcmp(pDirent->d_name, "..") == 0)) {
			continue;
		}

		paths[pathsCount - 1] = calloc(strlen(dirPath) + 1 + FILENAME_MAX, sizeof(char));
		memcpy(paths[pathsCount - 1], dirPath, strlen(dirPath));
		paths[pathsCount - 1][strlen(paths[pathsCount - 1])] = '/';
		memcpy((paths[pathsCount - 1]) + strlen(paths[pathsCount - 1]), pDirent->d_name, FILENAME_MAX - 1);
		pathsCount++;
		paths = realloc(paths, pathsCount * sizeof(string));
		paths[pathsCount - 1] = NULL;
	}

	closedir(pDir);

	return paths;
}

string* io_LoadStrings(const string filePath) {
	if (!filePath) {
		fprintf(stderr, "File I/O Error: File Path Passed Is Nullptr\n");
		return NULL;
	}

	if (!strlen(filePath)) {
		fprintf(stderr, "File I/O Error: File Path Passed Is Empty\n");
		return NULL;
	}

	Array fileData = io_LoadBytes(filePath);

	if (!fileData.data) {
		return NULL;
	}

	arr_Push(&fileData, "\0", 1);
	string* temp = str_SplitTokens(fileData.data, "\x0a\x0d");
	arr_Clear(&fileData, false);
	return temp;
}

void io_SaveStrings(const string filePath, const string* lines) {
	if (!filePath) {
		fprintf(stderr, "File I/O Error: File Path Passed Is Nullptr\n");
		return;
	}

	if (!strlen(filePath)) {
		fprintf(stderr, "File I/O Error: File Path Passed Is Empty\n");
		return;
	}

	if (!lines) {
		fprintf(stderr, "File I/O Error: Lines Passed Is Nullptr\n\t\"%s\"\n", filePath);
		return;
	}

	FILE* file = fopen(filePath, "w");
	if (!file) {
		fprintf(stderr, "File I/O Error: Could Not Create File\n\t\"%s\"\n", filePath);
		return;
	}

	for (uint i = 0; lines[i]; i++) {
		fprintf(file, "%s\n", lines[i]);
	}

	fclose(file);
}

Array io_LoadBytes(const string filePath) {
	if (!filePath) {
		fprintf(stderr, "File I/O Error: File Path Passed Is Nullptr\n");
		return arr_Create(sizeof(char), 0, false, false);
	}

	if (!strlen(filePath)) {
		fprintf(stderr, "File I/O Error: File Path Passed Is Empty\n");
		return arr_Create(sizeof(char), 0, false, false);
	}

	FILE* file = fopen(filePath, "rb");
	if (!file) {
		fprintf(stderr, "File I/O Error: Could Not Read File\n\t\"%s\"\n", filePath);
		return arr_Create(sizeof(char), 0, false, false);
	}

	fseek(file, 0, SEEK_END);
	const uint fileSize = ftell(file);
	Array fileData = arr_Create(sizeof(char), fileSize, false, false);
	fileData.elementCount = fileSize;

	fseek(file, 0, SEEK_SET);
	fread(fileData.data, sizeof(char), fileSize, file);

	fclose(file);
	return fileData;
}

void io_SaveBytes(const string filePath, const Array* fileData) {
	if (!filePath) {
		fprintf(stderr, "File I/O Error: File Path Passed Is Nullptr\n");
		return;
	}

	if (!strlen(filePath)) {
		fprintf(stderr, "File I/O Error: File Path Passed Is Empty\n");
		return;
	}

	if (!fileData) {
		fprintf(stderr, "File I/O Error: File Data Passed Is Nullptr\n\t\"%s\"\n", filePath);
		return;
	}

	if (!fileData->dataSize) {
		fprintf(stderr, "File I/O Error: File Data Passed Is Invalid Size\n\t\"%s\"\n", filePath);
		return;
	}

	if (!fileData->data || !fileData->elementCount) {
		fprintf(stderr, "File I/O Error: File Data Passed Is Empty\n\t\"%s\"\n", filePath);
		return;
	}

	FILE* file = fopen(filePath, "wb");
	if (!file) {
		fprintf(stderr, "File I/O Error: Could Not Create File\n\t\"%s\"\n", filePath);
		return;
	}

	fwrite(fileData->data, fileData->dataSize, fileData->elementCount, file);

	fclose(file);
}
