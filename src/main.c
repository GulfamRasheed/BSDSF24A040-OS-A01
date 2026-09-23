#include <stdio.h>
#include "../include/mystrfunctions.h"
#include "../include/myfilefunctions.h"

int main() {
    printf("--- Testing String Functions ---\n");

    char str1[50] = "Hello";
    char str2[50] = "World";
    char dest[100];

    int len = mystrlen(str1);
    printf("Length of '%s' = %d\n", str1, len);

    mystrcpy(dest, str1);
    printf("After mystrcpy, dest = %s\n", dest);

    mystrncpy(dest, str1, 3);
    printf("After mystrncpy (3 chars), dest = %s\n", dest);

    mystrcpy(dest, str1);
    mystrcat(dest, str2);
    printf("After mystrcat, dest = %s\n", dest);

    printf("\n--- Testing File Functions ---\n");

    FILE* fp = fopen("test.txt", "r");
    if (fp == NULL) {
        printf("Could not open test.txt\n");
        return -1;
    }

    int lines, words, chars;
    wordCount(fp, &lines, &words, &chars);
    printf("Lines = %d, Words = %d, Chars = %d\n", lines, words, chars);

    fclose(fp);
    fp = fopen("test.txt", "r");

    char** matches;
    int matchCount = mygrep(fp, "hello", &matches);
    printf("Found %d matching lines:\n", matchCount);

    for (int i = 0; i < matchCount; i++) {
        printf("%s", matches[i]);
    }

    fclose(fp);

    return 0;
}
