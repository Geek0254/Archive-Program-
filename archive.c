#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 256

typedef struct {
    char filename[MAX_FILENAME_LENGTH];
    long size;
} FileEntry;

void createArchive(const char* archiveFilename, int numFiles, char** filenames);
void listArchiveContents(const char* archiveFilename);
void unpackArchive(const char* archiveFilename);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <operation> <archive_file> [file1 file2 ...]\n", argv[0]);
        printf("Operations:\n");
        printf("  -c: Create archive\n");
        printf("  -l: List archive contents\n");
        printf("  -u: Unpack archive\n");
        return 1;
    }

    const char* operation = argv[1];
    const char* archiveFilename = argv[2];

    if (strcmp(operation, "-c") == 0) {
        if (argc < 4) {
            printf("At least one file must be specified for archiving.\n");
            return 1;
        }
        int numFiles = argc - 3;
        char** filenames = &argv[3];
        createArchive(archiveFilename, numFiles, filenames);
    } else if (strcmp(operation, "-l") == 0) {
        listArchiveContents(archiveFilename);
    } else if (strcmp(operation, "-u") == 0) {
        unpackArchive(archiveFilename);
    } else {
        printf("Invalid operation: %s\n", operation);
        return 1;
    }

    return 0;
}

void createArchive(const char* archiveFilename, int numFiles, char** filenames) {
    FILE* archive = fopen(archiveFilename, "wb");
    if (archive == NULL) {
        printf("Failed to create archive: %s\n", archiveFilename);
        return;
    }

    fwrite(&numFiles, sizeof(int), 1, archive);

    FileEntry* fileEntries = (FileEntry*)malloc(numFiles * sizeof(FileEntry));
    if (fileEntries == NULL) {
        printf("Memory allocation error.\n");
        fclose(archive);
        return;
    }

    for (int i = 0; i < numFiles; i++) {
        FILE* file = fopen(filenames[i], "rb");
        if (file == NULL) {
            printf("Failed to open file: %s\n", filenames[i]);
            free(fileEntries);
            fclose(archive);
            return;
        }

        fseek(file, 0L, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);

        fileEntries[i].size = fileSize;
        strncpy(fileEntries[i].filename, filenames[i], MAX_FILENAME_LENGTH);

        fwrite(&fileEntries[i], sizeof(FileEntry), 1, archive);

        char* buffer = (char*)malloc(fileSize);
        if (buffer == NULL) {
            printf("Memory allocation error.\n");
            fclose(file);
            free(fileEntries);
            fclose(archive);
            return;
        }

        fread(buffer, sizeof(char), fileSize, file);
        fwrite(buffer, sizeof(char), fileSize, archive);

        fclose(file);
        free(buffer);
    }

    fseek(archive, sizeof(int), SEEK_SET);
    fwrite(fileEntries, sizeof(FileEntry), numFiles, archive);

    free(fileEntries);
    fclose(archive);

    printf("Archive created: %s\n", archiveFilename);
}

void listArchiveContents(const char* archiveFilename) {
    FILE* archive = fopen(archiveFilename, "rb");
    if (archive == NULL) {
        printf("Failed to open archive: %s\n", archiveFilename);
        return;
    }

    int numFiles;
    fread(&numFiles, sizeof(int), 1, archive);

    FileEntry* fileEntries = (FileEntry*)malloc(numFiles * sizeof(FileEntry));
    if (fileEntries == NULL) {
        printf("Memory allocation error.\n");
        fclose(archive);
        return;
    }

    fread(fileEntries, sizeof(FileEntry), numFiles, archive);

    printf("Archive contents:\n");
    for (int i = 0; i < numFiles; i++) {
        printf("%s (%ld bytes)\n", fileEntries[i].filename, fileEntries[i].size);
    }

    free(fileEntries);
    fclose(archive);
}

void unpackArchive(const char* archiveFilename) {
    FILE* archive = fopen(archiveFilename, "rb");
    if (archive == NULL) {
        printf("Failed to open archive: %s\n", archiveFilename);
        return;
    }

    int numFiles;
    fread(&numFiles, sizeof(int), 1, archive);

    FileEntry* fileEntries = (FileEntry*)malloc(numFiles * sizeof(FileEntry));
    if (fileEntries == NULL) {
        printf("Memory allocation error.\n");
        fclose(archive);
        return;
    }

    fread(fileEntries, sizeof(FileEntry), numFiles, archive);

    for (int i = 0; i < numFiles; i++) {
        FILE* file = fopen(fileEntries[i].filename, "wb");
        if (file == NULL) {
            printf("Failed to create file: %s\n", fileEntries[i].filename);
            free(fileEntries);
            fclose(archive);
            return;
        }

        char* buffer = (char*)malloc(fileEntries[i].size);
        if (buffer == NULL) {
            printf("Memory allocation error.\n");
            fclose(file);
            free(fileEntries);
            fclose(archive);
            return;
        }

        fread(buffer, sizeof(char), fileEntries[i].size, archive);
        fwrite(buffer, sizeof(char), fileEntries[i].size, file);

        fclose(file);
        free(buffer);
    }

    free(fileEntries);
    fclose(archive);

    printf("Archive unpacked: %s\n", archiveFilename);
}
