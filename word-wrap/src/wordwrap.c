/*
 ============================================================================
 Name        : wordwrap.c
 Author      : 161953323+GI-gabriel@users.noreply.github.com
 Version     : 1.0
 Copyright   : GPLv3
 Description : Take a string and remove line breaks, if any. Then add line
 breaks in correct position in order to get maximum line width. Doesn't split
 words. Finally, indent every line with a space.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 4000
#define MAX_WIDTH  77

const char *inputFileName = "input.txt";
const char *intermFileName = "intermediate.txt";
const char *outputFileName = "output.txt";
const char *title = "";
const char *indentSym = " ";

void readFile(const char *filename, char *string, int start) {
    FILE *file_ = fopen(filename, "r");
    if (file_ == NULL) {
        printf("Cannot find file\n");
        exit(1);
    }

    int i = start;
    char c = fgetc(file_);
    while (c != EOF && i < MAX_BUFFER - 1) {
        string[i++] = (char) c;
        c = fgetc(file_);
    }
    string[i] = '\0';

    if (fclose(file_) == EOF) {
        printf("Cannot close file\n");
        exit(1);
    }
    file_ = NULL;
}

void charReplace(char *string, char from, char to) {
    for (int i = 0; string[i] != '\0'; i++) {
        if (string[i] == from)
            string[i] = to;
    }
}

void writeFile(const char *filename, char *string) {
    FILE *file_ = fopen(filename, "w");
    if (file_ == NULL) {
        printf("Cannot find file\n");
        exit(1);
    }

    for (int i = 0; i < strlen(string); i++)
        if (fputc(string[i], file_) == EOF)
            break;

    if (fclose(file_) == EOF) {
        printf("Cannot close file\n");
        exit(1);
    }
    file_ = NULL;
}

void wrap(char *string, int max) {
    int i, l, w;

    i = l = w = 0;
    while (string[i] != '\0') {
        if (i > 0 && string[i] == ' ' && string[i - 1] != ' ')
            w = i;

        if (l >= max) {
            string[w] = '\n';
            i = w + 1;
            l = 0;
        } else {
            i++;
            l++;
        }
    }
}

void indent(char *to, const char *from, const char *indent) {
    int i, j, k;

    i = j = 0;
    while (from[j] != '\0' && i < MAX_BUFFER) {

        k = 0;
        while (indent[k] != '\0' && i < MAX_BUFFER)
            to[i++] = indent[k++];

        while (from[j] != '\n' && i < MAX_BUFFER)
            to[i++] = from[j++];

        to[i++] = from[j++];
    }
}

void usage() {
    printf(
            "Word wrap tool to limit line width to predefined value.\n\n"
                    "Usage: wordwrap [OPTION]\n\n"
                    "OPTION\n"
                    "    -1: Create intermediate text file ready to add break lines (you must\n"
                    "        do it manually).\n"
                    "    -2: Starting from properly edited intermediate text file, with  break\n"
                    "        at desired locations, this command will limit line width by adding\n"
                    "        word wrap.\n\n"
                    "FILES\n"
                    "    You must provide input.txt in same folder.\n"
                    "    Option -1 reads input.txt and creates intermediate.txt for you to edit.\n"
                    "    Option -2 reads intermediate.txt and produces output.txt.\n");
    exit(2);
}

int main(int argc, char *argv[]) {
    char stringStep1[MAX_BUFFER] = { '\0' };
    char stringStep2[MAX_BUFFER] = { '\0' };

    if (argc == 2) {
        if (!strcmp(argv[1], "-1")) {
            strcpy(stringStep1, title);
            readFile(inputFileName, stringStep1, strlen(title));
            charReplace(stringStep1, '\n', ' ');
            writeFile(intermFileName, stringStep1);
        } else if (!strcmp(argv[1], "-2")) {
            readFile(intermFileName, stringStep1, 0);
            wrap(stringStep1, MAX_WIDTH - strlen(indentSym));
            indent(stringStep2, stringStep1, indentSym);
            writeFile(outputFileName, stringStep2);
        } else
            usage();
    } else
        usage();
}

