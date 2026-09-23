#include "../include/mystrfunctions.h"
int mystrlen(const char* s) {
    int count = 0;
    while (s[count] != '\0') {
        count++;
    }
    return count;
}


int mystrcpy(char* dest, const char* src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return i; 
}


int mystrncpy(char* dest, const char* src, int n) {
    int i = 0;
    while (i < n && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return i;
}


int mystrcat(char* dest, const char* src) {
    int destLen = mystrlen(dest); 
    int i = 0;
    while (src[i] != '\0') {
        dest[destLen + i] = src[i];
        i++;
    }
    dest[destLen + i] = '\0';
    return destLen + i; 
}

