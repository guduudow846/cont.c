#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "safeinput.h"

/* ---------- Konstanter ---------- */
#define DOOR_OPEN_SECONDS 5
#define LAMP_OFF   0
#define LAMP_GREEN 1
#define LAMP_RED   2

/* ---------- Datastrukturer ---------- */

typedef struct
{
    char number[32];
    bool hasAccess;
    char dateAdded[20];
} Card;

typedef struct
{
    Card* cards;          /* dynamisk array */
    int count;            /* antal kort */
    int lampState;        /* LAMP_OFF / LAMP_GREEN / LAMP_RED */
    time_t lampChangedAt; /* när lampan senast ändrades */
} SystemState;

/* ---------- Datum ---------- */

static void getCurrentDate(char* buffer, int size)
{
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d", t);
}

/* ---------- Lampa ---------- */

static const char* lampName(int state)
{
    switch (state) {
        case LAMP_GREEN: return "Green";
        case LAMP_RED:   return "Red";
        default:         return "Off";
    }
}

static void setLamp(SystemState* state, int lamp)
{
    state->lampState = lamp;
    state->lampChangedAt = time(NULL);
    printf("CURRENTLY LAMP IS:%s\n", lampName(lamp));
}

static void printLamp(const SystemState* state)
{
    printf("CURRENTLY LAMP IS:%s\n", lampName(state->lampState));
}

/* Om dörren varit öppen i 5 sek → släck lampan */
static void updateLampTimeout(SystemState* state)
{
    if (state->lampState != LAMP_OFF &&
        difftime(time(NULL), state->lampChangedAt) >= DOOR_OPEN_SECONDS)
    {
        setLamp(state, LAMP_OFF);
    }
}

/* ---------- Kortlistan ---------- */

static Card* findCard(SystemState* state, const char* number)
{
    for (int i = 0; i < state->count; i++) {
        if (strcmp(state->cards[i].number, number) == 0)
            return &state->cards[i];
    }
    return NULL;
}

static Card* addOrGetCard(SystemState* state, const char* number)
{
    Card* existing = findCard(state, number);
    if (existing) return existing;

    Card* temp = realloc(state->cards, (state->count + 1) * sizeof(Card));
    if (!temp) {
        printf("Memory error.\n");
        return NULL;
    }
    state->cards = temp;

    Card* c = &state->cards[state->count++];
    strncpy(c->number, number, sizeof(c->number) - 1);
    c->number[sizeof(c->number) - 1] = '\0';
    c->hasAccess = false;
    getCurrentDate(c->dateAdded, sizeof(c->dateAdded));
    return c;
}

static void freeSystem(SystemState* state)
{
    free(state->cards);
    state->cards = NULL;
    state->count = 0;
}

/* ---------- Menyfunktioner ---------- */

static void remoteOpenDoor(SystemState* state)
{
    setLamp(state, LAMP_GREEN);
    printf("Door opened remotely for %d seconds.\n", DOOR_OPEN_SECONDS);
}

static void listAllCards(SystemState* state)
{
    printf("All cards in system\n");
    if (state->count == 0) {
        printf("(No cards in system)\n");
    } else {
        for (int i = 0; i < state->count; i++) {
            printf("%s %s Added to system: %s\n",
                   state->cards[i].number,
                   state->cards[i].hasAccess ? "Access" : "No access",
                   state->cards[i].dateAdded);
        }
    }
    printf("Press key to continue\n");
    getchar();
}

static void addRemoveAccess(SystemState* state)
{
    char number[32];

    if (GetInput("Enter cardnumber>", number, sizeof(number)) != INPUT_RESULT_OK) {
        printf("Invalid input.\n");
        return;
    }

    Card* c = addOrGetCard(state, number);
    if (!c) return;

    printf("This card %s\n", c->hasAccess ? "has access" : "has no access");
    printf("Enter 1 for access, 2 for no access\n");

    int val;
    if (!GetInputInt("", &val)) {
        printf("Invalid input, nothing changed.\n");
        return;
    }

    if (val == 1) {
        c->hasAccess = true;
        printf("Card %s now has access.\n", c->number);
    } else if (val == 2) {
        c->hasAccess = false;
        printf("Card %s no longer has access.\n", c->number);
    } else {
        printf("Invalid choice, nothing changed.\n");
    }
}

static void fakeScanCard(SystemState* state)
{
    char input[32];

    printf("Please scan card to enter or X to go back to admin menu\n");
    printLamp(state);

    if (GetInput("", input, sizeof(input)) != INPUT_RESULT_OK) {
        printf("Invalid input.\n");
        return;
    }

    /* X = tillbaka till admin-menyn */
    if (strcmp(input, "X") == 0 || strcmp(input, "x") == 0) {
        return;
    }

    Card* c = findCard(state, input);
    if (c && c->hasAccess) {
        setLamp(state, LAMP_GREEN);
    } else {
        setLamp(state, LAMP_RED);
    }
}

/* ---------- Admin-meny ---------- */

static void printAdminMenu(void)
{
    printf("Admin menu\n");
    printf("1. Remote open door\n");
    printf("2. List all cards in system\n");
    printf("3. Add/remove access\n");
    printf("4. Exit\n");
    printf("9. FAKE TEST SCAN CARD\n");
}

/* ---------- Main ---------- */

int main(void)
{
    SystemState state;
    state.cards = NULL;
    state.count = 0;
    state.lampState = LAMP_OFF;
    state.lampChangedAt = time(NULL);

    while (1)
    {
        updateLampTimeout(&state);

        printAdminMenu();
        printLamp(&state);

        int choice;
        if (!GetInputInt("", &choice)) {
            printf("Invalid choice, try again.\n");
            continue;
        }

        switch (choice)
        {
            case 1:
                remoteOpenDoor(&state);
                break;
            case 2:
                listAllCards(&state);
                break;
            case 3:
                addRemoveAccess(&state);
                break;
            case 4:
                printf("Exiting program.\n");
                freeSystem(&state);
                return 0;
            case 9:
                fakeScanCard(&state);
                break;
            default:
                printf("Wrong choice, try again.\n");
                break;
        }
    }

    freeSystem(&state);
    return 0;
}