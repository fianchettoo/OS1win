#include "disk_utils.h"
#include <iostream>
#include <windows.h>

// Get list of disks
void listDrives() {
    DWORD drives = GetLogicalDrives();
    if (drives == 0) {
        std::cerr << "Error in getting list of disks.\n";
        return;
    }

    DWORD dwSize = MAX_PATH;
    char szLogicalDrives[MAX_PATH] = {0};
    DWORD dwResult = GetLogicalDriveStringsA(dwSize, szLogicalDrives);
    if (dwResult == 0) {
        std::cout << "Error when getting the disk name.\n";
        return;
    }

    if (dwResult > 0 && dwResult <= MAX_PATH) {
        char* szSingleDrive = szLogicalDrives;
        while (*szSingleDrive) {
            printf("Drive: %s\n", szSingleDrive);
            szSingleDrive = szSingleDrive + strlen(szSingleDrive) + 1;
        }
    }
}

// Get info about disk
void printDriveInfo(LPCSTR drivePath) {
    UINT driveType = GetDriveTypeA(drivePath);
    switch (driveType) {
        case DRIVE_FIXED:
            std::cout << "Disk type: Hard drive (local disk)\n";
            break;
        case DRIVE_REMOVABLE:
            std::cout << "Disk type: Removable disk\n";
            break;
        case DRIVE_CDROM:
            std::cout << "Disk type: CD-ROM\n";
            break;
        case DRIVE_REMOTE:
            std::cout << "Disk type: Network drive\n";
            break;
        case DRIVE_RAMDISK:
            std::cout << "Disk type: RAM-disk\n";
            break;
        default:
            std::cout << "Disk type: Unknown\n";
    }

    char volumeName[MAX_PATH];
    char fileSystemName[MAX_PATH];
    DWORD serialNumber, maxComponentLen, fileSystemFlags;
    if (GetVolumeInformationA(drivePath, volumeName, sizeof(volumeName),
                              &serialNumber, &maxComponentLen, &fileSystemFlags,
                              fileSystemName, sizeof(fileSystemName)))
    {
        std::cout << "Volume Label: " << volumeName << "\n";
        std::cout << "File system: " << fileSystemName << "\n";
        std::cout << "Serial number: " << serialNumber << "\n\n";
    }

    DWORD sectorsPerCluster, bytesPerSector, numberOfFreeClusters, totalNumberOfClusters;
    if (GetDiskFreeSpaceA(drivePath, &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters)) {
        std::cout << "Free space: " 
                  << (numberOfFreeClusters * sectorsPerCluster * bytesPerSector) / (1024 * 1024) 
                  << " MB\n";
        std::cout << "Total space: " 
                  << (sectorsPerCluster * bytesPerSector * totalNumberOfClusters) / (1024 * 1024) 
                  << " MB\n";

    } else {
        std::cerr << "Error in getting information about free space.\n";
    }
}
