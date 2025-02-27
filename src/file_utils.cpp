#include "file_utils.h"
#include <iostream>
#include <windows.h>

// Create a file
void createFile(LPCSTR path) {
    HANDLE hFile = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        switch (error) {
            case ERROR_FILE_EXISTS:
                std::cerr << "Error: A file with this name already exists.\n";
                break;
            case ERROR_PATH_NOT_FOUND:
                std::cerr << "Error: The specified path does not exist. Check the directory.\n";
                break;
            case ERROR_ACCESS_DENIED:
                std::cerr << "Error: Access denied. Ensure you have the necessary permissions.\n";
                break;
            case ERROR_INVALID_NAME:
                std::cerr << "Error: The file name is invalid. Avoid using special characters.\n";
                break;
            case ERROR_DISK_FULL:
                std::cerr << "Error: The disk is full. Unable to create the file.\n";
                break;
            case ERROR_WRITE_PROTECT:
                std::cerr << "Error: The disk is write-protected.\n";
                break;
            default:
                std::cerr << "Error: Unable to create the file. Error code: " << error << "\n";
                break;
        }    } else {
        std::cout << "File created successfully.\n";
        CloseHandle(hFile);
    }
}

// Check if file exists
bool existsFile(LPCSTR path) {
    DWORD fileAttributes = GetFileAttributesA(path);
    return (fileAttributes != INVALID_FILE_ATTRIBUTES) && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

// Copy a file
void copyFile(LPCSTR srcPath, LPCSTR destPath) {
    if (!existsFile(srcPath)) {
        std::cerr << "Error: The source file does not exist.\n";
        return;
    }

    if (existsFile(destPath)) {
        char choice;
        std::cout << "A file with the same name already exists at the destination. Want to overwrite? (y/n): ";
        std::cin >> choice;
        if (choice != 'y' && choice != 'Y') {
            std::cout << "Copying was cancelled.\n";
            return;
        }
    }

    if (CopyFileA(srcPath, destPath, FALSE)) {
        std::cout << "File copied successfully.\n";
    } else {
        std::cerr << "Error copying file.\n";
    }
}

// Move a file
void moveFile(LPCSTR srcPath, LPCSTR destPath) {
    if (!existsFile(srcPath)) {
        std::cerr << "Error: The source file does not exist.\n";
        return;
    }

    if (existsFile(destPath)) {
        char choice;
        std::cout << "A file with the same name already exists at the destination. Want to overwrite? (y/n): ";
        std::cin >> choice;
        if (choice != 'y' && choice != 'Y') {
            std::cout << "Moving was cancelled.\n";
            return;
        }
    }

    if (MoveFileExA(srcPath, destPath, MOVEFILE_REPLACE_EXISTING)) {
        std::cout << "File moved successfully.\n";
    } else {
        std::cerr << "Error moving file.\n";
    }
}

// Change file attributes
void changeFileAttributes(LPCSTR path) {
    DWORD attributes = GetFileAttributesA(path);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        std::cerr << "Error in getting file attributes.\n";
        return;
    }

    std::cout << "Current attributes of file: " << attributes << std::endl;
    for (int i = 0; i < 7; i++) {
        char choice;
        switch (i) {
            case 0:
                std::cout << "Do you want to change the Archive attribute? (y/n) ";
                std::cin >> choice;
                if (choice == 'y') attributes ^= FILE_ATTRIBUTE_ARCHIVE;
                break;
            case 1:
                std::cout << "Do you want to change the Read-only attribute? (y/n) ";
                std::cin >> choice;
                if (choice == 'y') attributes ^= FILE_ATTRIBUTE_READONLY;
                break;
            case 2:
                std::cout << "Do you want to change the Offline attribute? (y/n) ";
                std::cin >> choice;
                if (choice == 'y') attributes ^= FILE_ATTRIBUTE_OFFLINE;
                break;
            case 3:
                std::cout << "Do you want to change the Not content indexed attribute? (y/n) ";
                std::cin >> choice;
                if (choice == 'y') attributes ^= FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
                break;
            case 4:
                std::cout << "Do you want to change the Hidden attribute? (y/n) ";
                std::cin >> choice;
                if (choice == 'y') attributes ^= FILE_ATTRIBUTE_HIDDEN;
                break;
            case 5:
                std::cout << "Do you want to change the System attribute? (y/n) ";
                std::cin >> choice;
                if (choice == 'y') attributes ^= FILE_ATTRIBUTE_SYSTEM;
                break;
            case 6:
                std::cout << "Do you want to change the Temporary attribute? (y/n) ";
                std::cin >> choice;
                if (choice == 'y') attributes ^= FILE_ATTRIBUTE_TEMPORARY;
                break;
        }
    }
    if (SetFileAttributesA(path, attributes)) {
        std::cout << "File attributes changed successfully.\n";
    } else {
        std::cerr << "Error changing file attributes.\n";
    }
}


std::string fileTimeToString(const FILETIME& fileTime) {
    SYSTEMTIME stUTC, stLocal;
    FileTimeToSystemTime(&fileTime, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

    char buffer[32];
    sprintf(buffer, "%02d/%02d/%d %02d:%02d:%02d",
            stLocal.wDay, stLocal.wMonth, stLocal.wYear,
            stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
    return std::string(buffer);
}

// Get info about file
void fileInfo(LPCSTR path) {
    HANDLE hFile = CreateFileA(
        path, GENERIC_READ, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening the file. Error code: " << GetLastError() << "\n";
        return;
    }

    FILETIME creationTime, lastAccessTime, lastWriteTime;
    if (GetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime)) {
        std::cout << "File time information:\n";
        std::cout << "Creation time: " << fileTimeToString(creationTime) << "\n";
        std::cout << "Last access time: " << fileTimeToString(lastAccessTime) << "\n";
        std::cout << "Last write time: " << fileTimeToString(lastWriteTime) << "\n";
    } else {
        std::cerr << "Error getting file time information. Error code: " << GetLastError() << "\n";
    }

    BY_HANDLE_FILE_INFORMATION fileInfo;
    if (GetFileInformationByHandle(hFile, &fileInfo)) {
        std::cout << "File attributes:\n";
        if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_READONLY) std::cout << " - Read-only\n";
        if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) std::cout << " - Hidden\n";
        if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) std::cout << " - System\n";
        if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) std::cout << " - Archive\n";
        if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) std::cout << " - Temporary\n";

        std::cout << "Number of links: " << fileInfo.nNumberOfLinks << "\n";
        std::cout << "Volume serial number: " << fileInfo.dwVolumeSerialNumber << "\n";
        std::cout << "File size: " 
             << ((static_cast<ULONGLONG>(fileInfo.nFileSizeHigh) << 32) | fileInfo.nFileSizeLow) 
             << " bytes\n";
    } else {
        std::cerr << "Error getting file information. Error code: " << GetLastError() << "\n";
    }
    CloseHandle(hFile);
}

// Set file time
void setTimeFile(LPCSTR path) {
    HANDLE hFile = CreateFileA(
        path, GENERIC_WRITE, FILE_SHARE_READ, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening the file. Error code: " << GetLastError() << "\n";
        return;
    }

    FILETIME fileTime;
    SYSTEMTIME systemTime;
    GetSystemTime(&systemTime);
    SystemTimeToFileTime(&systemTime, &fileTime);

    int timeChoice;
    std::cout << "Which time do you want to reset?\n";
    std::cout << "1) Creation time\n";
    std::cout << "2) Last access time\n";
    std::cout << "3) Last write time\n";
    std::cout << "> ";
    std::cin >> timeChoice;

    BOOL res;
    switch (timeChoice) {
        case 1:
            res = SetFileTime(hFile, &fileTime, NULL, NULL);
            break;
        case 2:
            res = SetFileTime(hFile, NULL, &fileTime, NULL);
            break;
        case 3:
            res = SetFileTime(hFile, NULL, NULL, &fileTime);
            break;
        default:
            std::cout << "Invalid choice. Nothing changed.\n";
            CloseHandle(hFile);
            return;
    }

    if (res) {
        std::cout << "File time successfully changed.\n";
    } else {
        std::cerr << "Error setting file time. Error code: " << GetLastError() << "\n";
    }

    CloseHandle(hFile);
}