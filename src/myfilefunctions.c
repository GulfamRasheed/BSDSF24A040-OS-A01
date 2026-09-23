#include "../include/myfilefunctions.h"
#include <string.h>
#include <stdlib.h>

int wordCount(FILE* file, int* lines, int* words, int* chars) {
    if (file == NULL) {
        return -1;
    }

    *lines = 0;
    *words = 0;
    *chars = 0;

    char line[1000];

    while (fgets(line, sizeof(line), file) != NULL) {
        (*lines)++;
        *chars = *chars + strlen(line);

        int i = 0;
        int insideWord = 0;

        while (line[i] != '\0') {
            if (line[i] == ' ' || line[i] == '\n' || line[i] == '\t') {
                insideWord = 0;
            } else {
                if (insideWord == 0) {
                    (*words)++;
                    insideWord = 1;
                }
            }
            i++;
        }
    }

    return 0;
}

int mygrep(FILE* fp, const char* search_str, char*** matches) {
    if (fp == NULL || search_str == NULL) {
        return -1;
    }

    char line[1000];
    int count = 0;

    char** result = malloc(100 * sizeof(char*));

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, search_str) != NULL) {
            result[count] = malloc(strlen(line) + 1);
            strcpy(result[count], line);
            count++;
        }
    }

    *matches = result;
    return count;
}
