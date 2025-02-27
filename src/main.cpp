#include <iostream>
#include <string>
#include "disk_utils.h"
#include "directory_utils.h"
#include "file_utils.h"

void printMenu() {
    std::cout << "1.  List of disks\n";
    std::cout << "2.  Information about disk\n";
    std::cout << "3.  Create a directory\n";
    std::cout << "4.  Remove a directory\n";
    std::cout << "5.  Create a file\n";
    std::cout << "6.  Copy a file\n";
    std::cout << "7.  Move a file\n";
    std::cout << "8.  Change attributes of file\n";
    std::cout << "9.  File info\n";
    std::cout << "10. Set file time\n";
    std::cout << "0.  Exit\n";
}

int main() {
    int choice;
    std::string path;
    std::string destPath;
    char check;

    while (true) {
        printMenu();
        std::cout << "Choose option: ";
        std::cin >> choice;
        std::cin.ignore(); // Убираем остаток символов в буфере после ввода

        switch (choice) {
            case 1:
                listDrives();
                break;
            case 2:
                std::cout << "Enter the disk path (e.g., C:\\): ";
                std::getline(std::cin, path);
                printDriveInfo(path.c_str());
                break;
            case 3:
                std::cout << "Enter the path to create directory: ";
                std::getline(std::cin, path);
                createDirectory(path.c_str());
                break;
            case 4:
                std::cout << "Enter the path to remove directory: ";
                std::getline(std::cin, path);
                std::cout << "Are you sure you want to delete this directory? \n";
                std::cout << "Type 'y' for yes or any other key for no.\n> ";
                std::cin >> check;
                if (check == 'y' || check == 'Y'){
                    removeDirectory(path.c_str());
                }
                break;
            case 5:
                std::cout << "Enter the path to create file: ";
                std::getline(std::cin, path);
                createFile(path.c_str());
                break;
            case 6:
                std::cout << "Enter the source path of the file: ";
                std::getline(std::cin, path);
                std::cout << "Enter the target file path: ";
                std::getline(std::cin, destPath);
                copyFile(path.c_str(), destPath.c_str());
                break;
            case 7:
                std::cout << "Enter the source path of the file: ";
                std::getline(std::cin, path);
                std::cout << "Enter the target file path: ";
                std::getline(std::cin, destPath);
                moveFile(path.c_str(), destPath.c_str());
                break;
            case 8:
                std::cout << "Enter the file path: ";
                std::getline(std::cin, path);
                changeFileAttributes(path.c_str());
                break;
            case 9:
                std::cout << "Enter the file path: ";
                std::getline(std::cin, path);
                fileInfo(path.c_str());
                break;
            case 10:
                std::cout << "Enter the file path: ";
                std::getline(std::cin, path);
                setTimeFile(path.c_str());
                break;
            case 0:
                return 0;
            default:
                std::cout << "Incorrect option. Try again.\n";
        }
    }

    return 0;
}