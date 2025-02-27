#include "directory_utils.h"
#include <iostream>
#include <windows.h>

// Create a directory
void createDirectory(LPCSTR path) {
    if (CreateDirectoryA(path, NULL)) {
        std::cout << "Directory created successfully.\n";
    } else {
        DWORD error = GetLastError();
        switch (error) {
            case ERROR_ALREADY_EXISTS:
                std::cerr << "Error: A directory with this name already exists.\n";
                break;
            case ERROR_PATH_NOT_FOUND:
                std::cerr << "Error: The specified path was not found. Please check the directory structure.\n";
                break;
            case ERROR_ACCESS_DENIED:
                std::cerr << "Error: Access denied. You do not have the required permissions.\n";
                break;
            case ERROR_FILENAME_EXCED_RANGE:
                std::cerr << "Error: Directory name is too long.\n";
                break;
            case ERROR_INVALID_NAME:
                std::cerr << "Error: Invalid directory name. Ensure the path doesn't contain illegal symbols.\n";
                break;
            case ERROR_DISK_FULL:
                std::cerr << "Error: Disk is full. Cannot create directory.\n";
                break;
            default:
                std::cerr << "Error while creating directory. Error code: " << error << "\n";
                break;
        }
    }
}

// Remove a directory
void removeDirectory(LPCSTR path) {
    if (RemoveDirectoryA(path)) {
        std::cout << "Directory removed successfully.\n";
    } else {
        DWORD error = GetLastError();
        switch (error) {
            case ERROR_FILE_NOT_FOUND:
                std::cerr << "Error: The specified directory does not exist.\n";
                break;
            case ERROR_PATH_NOT_FOUND:
                std::cerr << "Error: The specified path is invalid.\n";
                break;
            case ERROR_ACCESS_DENIED:
                std::cerr << "Error: Access denied. Ensure the directory is not read-only or in use.\n";
                break;
            case ERROR_DIR_NOT_EMPTY:
                std::cerr << "Error: The directory is not empty. Please delete all contents first.\n";
                break;
            case ERROR_SHARING_VIOLATION:
                std::cerr << "Error: The directory is being used by another process.\n";
                break;
            case ERROR_INVALID_NAME:
                std::cerr << "Error: The specified directory name is invalid.\n";
                break;
            case ERROR_FILENAME_EXCED_RANGE:
                std::cerr << "Error: The directory path is too long.\n";
                break;
            case ERROR_NOT_SUPPORTED:
                std::cerr << "Error: The operation is not supported in this file system.\n";
                break;
            default:
                std::cerr << "Error: Unable to remove the directory. Error code: " << error << "\n";
                break;
        }    }
}
