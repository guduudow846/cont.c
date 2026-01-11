#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



/* Struct for one card */
typedef struct {
    char number[20];     // card number
    bool hasAccess;      // access yes/no
    char dateAdded[11];  // date added
} Card;

/* List of cards */
Card *cards = NULL;
int cardCount = 0;

/* Print menu */
void showMenu(void) {
    printf("\n--- DOOR ADMIN ---\n");
    printf("1. Add card\n");
    printf("2. Scan card\n");
    printf("3. List all cards\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

/* Add a new card */
void addCard(void) {
    Card newCard;
    int accessInput;

    printf("Enter card number: ");
    fgets(newCard.number, sizeof(newCard.number), stdin);
    newCard.number[strcspn(newCard.number, "\n")] = '\0';

    printf("Has access (1 = yes, 0 = no): ");
    scanf("%d", &accessInput);
    getchar(); 

    newCard.hasAccess = accessInput ? true : false;

    printf("Enter date added (YYYY-MM-DD): ");
    fgets(newCard.dateAdded, sizeof(newCard.dateAdded), stdin);
    newCard.dateAdded[strcspn(newCard.dateAdded, "\n")] = '\0';

    cards = realloc(cards, (cardCount + 1) * sizeof(Card));
    if (cards == NULL) {
        printf("Memory error\n");
        exit(1);
    }

    cards[cardCount] = newCard;
    cardCount++;

    printf("Card added\n");
}

/* Scan card and show lamp color */
void scanCard(void) {
    char input[20];
    bool found = false;

    printf("Scan card number: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    for (int i = 0; i < cardCount; i++) {
        if (strcmp(cards[i].number, input) == 0) {
            found = true;

            if (cards[i].hasAccess) {
                printf("CURRENTLY LAMP IS: Green\n");
                printf("Door is open for 3 seconds...\n");
            } else {
                printf("CURRENTLY LAMP IS: Red\n");
            }
        }
    }

    if (!found) {
        printf("Card not found. CURRENTLY LAMP IS: Red\n");
    }
}

/* Print all cards */
void listCards(void) {
    if (cardCount == 0) {
        printf("No cards in system\n");
        return;
    }

    for (int i = 0; i < cardCount; i++) {
        printf("%d. %s - %s - %s\n",
               i + 1,
               cards[i].number,
               cards[i].hasAccess ? "ACCESS" : "NO ACCESS",
               cards[i].dateAdded);
    }
}


int main(void) {
    int choice = -1;

    while (choice != 0) {
        showMenu();
        scanf("%d", &choice);
        getchar(); // clear input

        if (choice == 1) {
            addCard();
        } else if (choice == 2) {
            scanCard();
        } else if (choice == 3) {
            listCards();
        } else if (choice == 0) {
            printf("Exiting program\n");
        } else {
            printf("Wrong choice\n");
        }
    }

    free(cards);
    return 0;
}
