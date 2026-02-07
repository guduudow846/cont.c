#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


#include "safeinput.h"

/* One card */
typedef struct
{
    char number[32];
    bool hasAccess;
    char dateAdded[20];
} Card;

typedef struct
{
    Card* cards;
    int count;
} SystemState;

/* Get current date */
void getCurrentDate(char* buffer, int size)
{
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d", t);
}

/* Add new card */
void addCard(SystemState* state)
{
    Card newCard;
    int access = 0;

    if (GetInput("Enter card number: ", newCard.number, sizeof(newCard.number)) != INPUT_RESULT_OK)
    {
        printf("Invalid input.\n");
        return;
    }

    if (!GetInputInt("Has access (1=yes, 0=no): ", &access))
    {
        printf("Invalid input.\n");
        return;
    }

    newCard.hasAccess = access ? true : false;
    getCurrentDate(newCard.dateAdded, sizeof(newCard.dateAdded));

    Card* temp = realloc(state->cards, (state->count + 1) * sizeof(Card));
    if (temp == NULL)
    {
        printf("Memory error.\n");
        return;
    }

    state->cards = temp;
    state->cards[state->count] = newCard;
    state->count++;

    printf("Card added.\n");
}

/* Scan card */
void scanCard(SystemState* state)
{
    char input[32];
    bool found = false;

    if (GetInput("Scan card number: ", input, sizeof(input)) != INPUT_RESULT_OK)
    {
        printf("Invalid input.\n");
        return;
    }

    for (int i = 0; i < state->count; i++)
    {
        if (strcmp(state->cards[i].number, input) == 0)
        {
            found = true;
            if (state->cards[i].hasAccess)
            {
                printf("CURRENTLY LAMP IS: Green\n");
                printf("Door is open for 3 seconds...\n");
            }
            else
            {
                printf("CURRENTLY LAMP IS: Red\n");
            }
            return;
        }
    }

    if (!found)
    {
        printf("Card not found. CURRENTLY LAMP IS: Red\n");
    }
}

/* List all cards */
void listCards(SystemState* state)
{
    if (state->count == 0)
    {
        printf("No cards in system.\n");
        return;
    }

    for (int i = 0; i < state->count; i++)
    {
        printf("%d. %s - %s - %s\n",
               i + 1,
               state->cards[i].number,
               state->cards[i].hasAccess ? "ACCESS" : "NO ACCESS",
               state->cards[i].dateAdded);
    }
}

/* Main */
int main(void)
{
    SystemState state;
    state.cards = NULL;
    state.count = 0;

    int choice = -1;

    while (choice != 0)
    {
        printf("\n--- DOOR ADMIN ---\n");
        printf("1. Add card\n");
        printf("2. Scan card\n");
        printf("3. List all cards\n");
        printf("0. Exit\n");

        if (!GetInputInt("Choice: ", &choice))
        {
            printf("Invalid choice.\n");
            continue;
        }

        switch (choice)
        {
            case 1:
                addCard(&state);
                break;
            case 2:
                scanCard(&state);
                break;
            case 3:
                listCards(&state);
                break;
            case 0:
                printf("Exiting program.\n");
                break;
            default:
                printf("Wrong choice.\n");
        }
    }

    free(state.cards);
    return 0;
}
