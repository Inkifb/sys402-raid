#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void list_stores() {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("./*.store", &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Failed to open directory\n");
        return;
    } 

    printf("Store Identifiers:\n");
    do {
        printf("%s\n", findFileData.cFileName);
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

int main() {
    list_stores();
    return EXIT_SUCCESS;
}