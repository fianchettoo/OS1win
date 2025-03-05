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
        ULONGLONG freeBytes = static_cast<ULONGLONG>(sectorsPerCluster) * bytesPerSector * numberOfFreeClusters;
        ULONGLONG totalBytes = static_cast<ULONGLONG>(sectorsPerCluster) * bytesPerSector * totalNumberOfClusters;

        std::cout << "Free space: " << static_cast<double>(freeBytes) / (1024 * 1024 * 1024) << " GB\n";
        std::cout << "Total space: " << static_cast<double>(totalBytes) / (1024 * 1024 * 1024) << " GB\n";
    } else {
        std::cerr << "Error in getting disk space information.\n";
    }

    char check;

    std::cout << "Would you like to get more information about the disk?\n";
    std::cout << "Type 'y' for yes or any other key for no.\n> ";
    std::cin >> check;
    if (check == 'y' || check == 'Y'){
        if (fileSystemFlags & FILE_READ_ONLY_VOLUME) std::cout << "Read-only volume.\n";
        if (fileSystemFlags & FILE_SUPPORTS_INTEGRITY_STREAMS) std::cout << "Support of integrity streams.\n";
        if (fileSystemFlags & FILE_SUPPORTS_TRANSACTIONS) std::cout << "Support of transactions.\n";
        if (fileSystemFlags & FILE_SUPPORTS_OPEN_BY_FILE_ID) std::cout << "Support of opening by FileID.\n";
        if (fileSystemFlags & FILE_UNICODE_ON_DISK) std::cout << "Support of Unicode in file names.\n";
        if (fileSystemFlags & FILE_FILE_COMPRESSION) std::cout << "Support of file-based compression.\n";
        if (fileSystemFlags & FILE_VOLUME_QUOTAS) std::cout << "Support of disk quotas.\n";
        if (fileSystemFlags & FILE_SUPPORTS_GHOSTING) std::cout << "Support of ghosting.\n";
        if (fileSystemFlags & FILE_SUPPORTS_SPARSE_FILES) std::cout << "Support of sparse files.\n";
        if (fileSystemFlags & FILE_SUPPORTS_REPARSE_POINTS) std::cout << "Support of reparse points.\n";
        if (fileSystemFlags & FILE_SUPPORTS_REMOTE_STORAGE) std::cout << "Support of remote storage.\n";
        if (fileSystemFlags & FILE_SUPPORTS_POSIX_UNLINK_RENAME) std::cout << "Support of POSIX-style delete/rename.\n";
        if (fileSystemFlags & FILE_SUPPORTS_ENCRYPTION) std::cout << "Support of EFS (encrypted file system).\n";
        if (fileSystemFlags & FILE_NAMED_STREAMS) std::cout << "Support of named streams.\n";
        if (fileSystemFlags & FILE_CASE_SENSITIVE_SEARCH) std::cout << "Support of case-sensitive file names.\n";
        if (fileSystemFlags & FILE_SEQUENTIAL_WRITE_ONCE) std::cout << "Support of sequential write once.\n";
        if (fileSystemFlags & FILE_SUPPORTS_OBJECT_IDS) std::cout << "Support of object identifiers.\n";
        if (fileSystemFlags & FILE_SUPPORTS_HARD_LINKS) std::cout << "Support of hard links.\n";
        if (fileSystemFlags & FILE_SUPPORTS_EXTENDED_ATTRIBUTES) std::cout << "Supports extended attributes.\n";
        if (fileSystemFlags & FILE_SUPPORTS_USN_JOURNAL) std::cout << "Support of USN journal.\n";
        if (fileSystemFlags & FILE_SUPPORTS_BLOCK_REFCOUNTING) std::cout << "Support of block refcounting.\n";
        if (fileSystemFlags & FILE_SUPPORTS_SPARSE_VDL) std::cout << "Tracks sparse valid data length (VDL).\n";
        if (fileSystemFlags & FILE_PERSISTENT_ACLS) std::cout << "Preserves and enforces ACLs.\n";
        if (fileSystemFlags & FILE_CASE_PRESERVED_NAMES) std::cout << "Preserves case of file names.\n";
        if (fileSystemFlags & FILE_RETURNS_CLEANUP_RESULT_INFO) std::cout << "Returns cleanup result info.\n";
        if (fileSystemFlags & FILE_VOLUME_IS_COMPRESSED) std::cout << "Compressed volume.\n";
        if (fileSystemFlags & FILE_DAX_VOLUME) std::cout << "Direct access (DAX) volume.\n";
    }

}
