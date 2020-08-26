#ifndef _HW2_H_
#define _HW2_H_

#define MAX_WORDS 50000
#define MAX_WORD_LEN 24
#define MAX_SYNONYMS 100
#define MAX_LINE_LEN 70
#define MAX_FILENAME_LEN 20

int hw2_main(int argc, char *argv[]);

void init_stack_size();

int read_synonyms(char *filename, char synonyms[MAX_SYNONYMS][MAX_WORD_LEN << 1]);

void checkInput(int argc, char *argv[], int commands[]);

void readWords(char wordsRead[][MAX_WORD_LEN], char *wordsIndex[][2]);

void stringToLowercase(char string[MAX_WORD_LEN]);

void printText(char *wordIndex[][2], int linelen);

void makeHistogram(char *wordIndex[][2], int histogram[MAX_WORDS]);

void printHistogram(char *wordIndex[][2], int histogram[MAX_WORDS]);

void linkWordToSynonym(char *wordIndex[][2], char synonyms[MAX_SYNONYMS][MAX_WORD_LEN << 1]);

#endif
