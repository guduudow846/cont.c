#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <stdbool.h>
#include "safeinput.h"

INPUT_RESULT GetInput(char* prompt, char* buff, int maxSize)
{
    if (prompt != NULL)
        printf("%s", prompt);

    if (fgets(buff, maxSize, stdin) == NULL)
        return INPUT_RESULT_NO_INPUT;

    if (buff[strlen(buff) - 1] != '\n') {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        return INPUT_RESULT_TOO_LONG;
    }

    buff[strcspn(buff, "\n")] = '\0';

    if (strlen(buff) == 0)
        return INPUT_RESULT_NO_INPUT;

    return INPUT_RESULT_OK;
}

bool GetInputInt(char* prompt, int* value)
{
    char buff[100];
    char* endptr;
    long temp;

    if (GetInput(prompt, buff, sizeof(buff)) != INPUT_RESULT_OK)
        return false;

    errno = 0;
    temp = strtol(buff, &endptr, 10);

    if (*endptr != '\0' || errno != 0)
        return false;

    *value = (int)temp;
    return true;
}

bool GetInputFloat(char* prompt, float* value)
{
    char buff[100];
    char* endptr;

    if (GetInput(prompt, buff, sizeof(buff)) != INPUT_RESULT_OK)
        return false;

    *value = strtof(buff, &endptr);

    if (*endptr != '\0')
        return false;

    return true;
}

bool GetInputChar(char* prompt, char* value)
{
    char buff[10];

    if (GetInput(prompt, buff, sizeof(buff)) != INPUT_RESULT_OK)
        return false;

    *value = buff[0];
    return true;
}