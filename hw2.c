#include "hw2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
	init_stack_size();
	return hw2_main(argc, argv);
}

int hw2_main(int argc, char *argv[])
{
	char wordsRead[MAX_WORDS + 1][MAX_WORD_LEN] = {{'\0'}};
	char *wordsIndex[MAX_WORDS + 1][2] = {{NULL}};
	char synonyms[MAX_SYNONYMS][MAX_WORD_LEN << 1] = {{'\0'}};

	int histogram[MAX_WORDS + 1] = {0};
	int commands[3] = {0};

	checkInput(argc, argv, commands);

	readWords(wordsRead, wordsIndex);

	if (commands[0] == 1)
	{
		makeHistogram(wordsIndex, histogram);
		printHistogram(wordsIndex, histogram);
	}

	if (commands[1] == 1)
	{
		read_synonyms("in/s_in4", synonyms);
		linkWordToSynonym(wordsIndex, synonyms);
	}

	if (commands[2] == 1)
	{
		printText(wordsIndex, MAX_LINE_LEN);
	}
	return 0;
}

void linkWordToSynonym(char *wordIndex[][2], char synonyms[MAX_SYNONYMS][MAX_WORD_LEN << 1])
{
	char currentWord[MAX_WORD_LEN] = {'\0'};
	int cmp = -1;

	for (int i = 0; i <= MAX_WORDS; i++)
	{
		if (wordIndex[i][0] != NULL && strlen(wordIndex[i][0]) != 0)
		{
			strcpy(currentWord, wordIndex[i][0]);
			for (int j = 0; j < MAX_SYNONYMS; j++)
			{
				cmp = strcmp(currentWord, &synonyms[j][0]);
				if (cmp == 0)
				{
					wordIndex[i][1] = &synonyms[j][strlen(currentWord) + 1];
				}
			}
		}
	}
}

void readWords(char wordsRead[][MAX_WORD_LEN], char *wordsIndex[][2])
{
	int existsInIndex = 0;

	char c = 'a';
	char word[MAX_WORD_LEN] = {'\0'};

	int wordCounter = 0;
	int wordLength = 0;

	int nextFreeSpace = 0;
	int indexIterator = 0;

	while (scanf("%c", &c) == 1 && nextFreeSpace <= MAX_WORDS)
	{

		if (c != ' ' && c != '\n')
		{
			word[wordLength++] = c;
		}
		else
		{
			if (strlen(word) == 0)
			{
				continue;
			}
			word[wordLength] = '\0';

			stringToLowercase(word);

			for (indexIterator = 0; indexIterator <= MAX_WORDS; indexIterator++)
			{
				if (wordsIndex[indexIterator][0] != NULL)
				{
					if (strcmp(wordsIndex[indexIterator][0], word) == 0)
					{
						existsInIndex = 1;
						break;
					}
				}
			}

			if (existsInIndex == 0)
			{
				strcpy(wordsRead[wordCounter], word);
				wordsIndex[nextFreeSpace][0] = wordsRead[wordCounter];

				wordCounter++;
			}
			else
			{
				wordsIndex[nextFreeSpace][0] = wordsIndex[indexIterator][0];
			}

			wordLength = 0;
			existsInIndex = 0;

			nextFreeSpace++;
		}
	}
}
void makeHistogram(char *wordIndex[][2], int histogram[MAX_WORDS])
{
	char word[MAX_WORD_LEN] = {'\0'};
	int isCalculated[MAX_WORDS + 1] = {0};

	for (int i = 0; i <= MAX_WORDS; i++)
	{
		if (wordIndex[i][0] == NULL)
		{
			continue;
		}

		if (strlen(wordIndex[i][0]) == 0)
		{
			continue;
		}
		if (isCalculated[i] == 1)
		{
			continue;
		}
		strcpy(word, wordIndex[i][0]);

		histogram[i]++;
		isCalculated[i] = 1;

		for (int j = 0; j <= MAX_WORDS; j++)
		{
			if (j != i)
			{
				if (wordIndex[i][0] == wordIndex[j][0])
				{
					histogram[i]++;
					isCalculated[j] = 1;
				}
			}
		}
	}
}

void printHistogram(char *wordIndex[][2], int histogram[MAX_WORDS])
{
	printf("\n");
	int hundreds, dozens, units, thousands, times;

	for (int i = 0; i <= MAX_WORDS; i++)
	{
		if (histogram[i] != 0 && wordIndex[i][0] != NULL)
		{
			printf("%24s: ", wordIndex[i][0]);
			times = histogram[i];

			thousands = times / 1000;
			times = times % 1000;

			hundreds = times / 100;
			times = times % 100;

			dozens = times / 10;
			times = times % 10;

			units = times;
			for (int j = 0; j < thousands; j++)
			{
				printf("$");
			}
			for (int j = 0; j < hundreds; j++)
			{
				printf("#");
			}
			for (int j = 0; j < dozens; j++)
			{
				printf("@");
			}
			for (int j = 0; j < units; j++)
			{
				printf("*");
			}
			printf("\n");
		}
	}
	printf("\n");
}

void printText(char *wordIndex[][2], int linelen)
{
	printf("\n");

	char previousWord[MAX_WORD_LEN] = {'\0'};
	char currentWord[MAX_WORD_LEN] = {'\0'};
	char synonym[MAX_WORD_LEN] = {'\0'};
	int currentLength = 0;

	strcpy(previousWord, wordIndex[0][0]);

	printf("%c%s", toupper(previousWord[0]), &previousWord[1]);

	if (wordIndex[0][1] != NULL)
	{
		strcpy(synonym, wordIndex[0][1]);

		printf(" ");
		printf("(%c%s)", toupper(synonym[0]), &synonym[1]);

		currentLength++;
		currentLength += 2;
		currentLength += strlen(synonym);
	}
	currentLength += strlen(previousWord);

	for (int i = 1; i <= MAX_WORDS; i++)
	{
		if (wordIndex[i][0] == NULL)
		{
			continue;
		}
		if (strlen(wordIndex[i][0]) == 0)
		{
			continue;
		}

		strcpy(currentWord, wordIndex[i][0]);

		if (wordIndex[i][1] != NULL)
		{
			strcpy(synonym, wordIndex[i][1]);
		}
		else
		{
			for (int j = 0; j < MAX_WORD_LEN; j++)
			{
				synonym[j] = '\0';
			}
		}

		if (currentLength + strlen(currentWord) >= linelen)
		{
			printf("\n");

			currentLength = 0;
		}
		else
		{
			printf(" ");
			currentLength++;
		}

		if (strcmp(previousWord, ".") == 0 ||
			strcmp(previousWord, "!") == 0 ||
			strcmp(previousWord, "?") == 0)
		{

			printf("%c%s", toupper(currentWord[0]), &currentWord[1]);
			currentLength += strlen(currentWord);
			if (strlen(synonym) != 0)
			{
				if (currentLength + strlen(synonym) + 2 >= linelen)
				{
					printf("\n");
					currentLength = 0;
				}
				else
				{
					printf(" ");
					currentLength++;
				}
				printf("(%c%s)", toupper(synonym[0]), &synonym[1]);
				currentLength += strlen(synonym);
				currentLength += 2;
			}
		}
		else
		{
			printf("%s", currentWord);
			currentLength += strlen(currentWord);
			if (strlen(synonym) != 0)
			{
				if (currentLength + strlen(synonym) + 2 >= linelen)
				{
					printf("\n");
					currentLength = 0;
				}
				else
				{
					printf(" ");
					currentLength++;
				}
				printf("(%s)", synonym);
				currentLength += strlen(synonym);
				currentLength += 2;
			}
		}

		strcpy(previousWord, currentWord);
	}
	printf("\n");
}

void stringToLowercase(char string[MAX_WORD_LEN])
{
	if (string == NULL)
	{
		return;
	}
	for (int i = 0; i < MAX_WORD_LEN; i++)
	{
		string[i] = tolower(string[i]);
	}
}

void checkInput(int argc, char *argv[], int commands[])
{

	if (argc > 5 || argc == 1)
	{
		printf("Incorrect command-line arguments!\n");
		exit(1);
	}
	int i = 1;

	while (i < argc)
	{
		if (strcmp(argv[i], "-h") == 0)
		{
			commands[0] = 1;
			i++;
		}
		else if (strcmp(argv[i], "-r") == 0)
		{
			if (i + 1 >= argc)
			{
				printf("Incorrect command-line arguments!\n");
				exit(2);
			}

			if (argv[i + 1][0] == '-')
			{
				printf("Incorrect command-line arguments!\n");
				exit(2);
			}
			commands[1] = 1;

			i++;
		}
		else if (strcmp(argv[i], "-p") == 0)
		{
			commands[2] = 1;
			i++;
		}
		else
		{
			if (strcmp(argv[i - 1], "-r") == 0)
			{
				i++;
				continue;
			}
			printf("Incorrect command-line arguments!\n");
			exit(3);
		}
	}
}
