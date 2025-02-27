#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <windows.h>

void createFile(LPCSTR path);
bool existsFile(LPCSTR path);
void copyFile(LPCSTR srcPath, LPCSTR destPath);
void moveFile(LPCSTR srcPath, LPCSTR destPath);
void changeFileAttributes(LPCSTR path);
std::string fileTimeToString(const FILETIME& ft);
void fileInfo(LPCSTR path);
void setTimeFile(LPCSTR path);
#endif