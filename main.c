/*******************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3
 * Group 3
*******************************************************************************/


/*****************************************************************************************************/
/*****************************************************************************************************/
/*****************************************************************************************************
 *********** PROGRAM DECLARATIONS
 *****************************************************************************************************/
/*****************************************************************************************************/
/*****************************************************************************************************/



/*******************************************************************************
 * List header files - do NOT use any other header files. Note that stdlib.h is
 * included in case you want to use any of the functions in there. However the
 * task can be achieved with stdio.h and string.h only.
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "card.h"
#include "card_stack.h"
#include "card_queue.h"
#include "encryption.c"
#include "compression.c"

#define MAX_DECK 52

#define KEY 5
#define KEYS "givemehighmarks"

#define FileLetterTab "lettertab.txt"
#define FileBitTab "NbrBit.txt"

#define DECK_VALUE "A23456789TJQK"
#define DECK_SUITE "DCHS"
#define MAIN_MENU_CHOICES "VARMSEQ"
#define ACTION_CHOICES "VBRA"
#define INTRO_CHOICES "NLS"
#define REMOVE_TYPE "SVC"
#define PRESET_CHOICE "SHCA"
#define SORT_METHOD "BI"
#define MIX_TYPE "SNW"

#define DECK_VALUE_LEN 12
#define DECK_SUITE_LEN 4

#define INVALID_VALUE_ERROR "Please enter a valid card value\n\n"
#define INVALID_SUITE_ERROR "Please enter a valid card suite [D/C/H/S]\n\n"
#define INVALID_ACTION_ERROR "Please enter a valid action code\n"
#define INVALID_REMOVE_TYPE_ERROR "Please enter a valid remove type [S/V/C]\n"
#define INVALID_SORT_TYPE_ERROR "Please enter a valid sort type [S/V]\n"
#define INVALID_SORT_METHOD_ERROR "Please enter a valid sort method [B/I]\n"
#define INVALID_MIX_TYPE_ERROR "Please enter a valid mix type [S/N/T]\n"



/*******************************************************************************
 * Function prototypes:
 *
*******************************************************************************/
/* Base */

int main(int argc, char * argv[]);
void header(void);

/* Functionality */

void intro (void);
void load_deck (void);
void select_preset (void);
void main_menu (void);
void view_deck (int viewMode);

int add_card (card_t cards[], int cardsCount);
void remove_card (card_t deck[]);
void remove_card_specific (card_t deck[], char value, char suite);

void sort (void);
int sort_bubble (int sortMode);
int sort_insertion (int sortMode);

void mix (void);
void mix_shuffle (void);
void mix_norepeats (void);
void mix_twister (void);

void export_deck (void);

/* Helper */

void add_card_to_deck (char suite, char value, card_t deck[]);
int load_from_file (char fileName[], card_t deck[]);
int file_exists (char* fileName);
int deck_is_sorted ();
char* get_suite_icon(char suitValue);
int card_exists_in_deck (char cardValue, char cardSuite,  card_t deck[]);
int export_deck_data (char fileName[], card_t deck[]);
char* encrypt_deck (card_t deck[]);
card_t * decrypt_deck (char* deckChar,card_t deck[], int TotalCard);

/*******************************************************************************
 * Base variables:
 *
*******************************************************************************/

card_t currentDeck[52];
int totalCardCount = 0;

int debugMode = 0;

/* Input Validation */
/* JOYCE */
int isValidValue(char input);
int isValidSuite(char input);
int isValidMainMenuChoice(char input);
int isValidActionChoice(char input,int num);
int isValidIntroChoice(char input);
int isValidRemoveType(char input);
int isValidSortType(char input);
int isValidSortMethod(char input);
int isValidMixMethod(char input);
int isValidPresetChoice(char input);

/* Sorting Helper*/
/* JOYCE */
void swap(card_t *xp, card_t *yp);
int getCardValue(card_t card);
char getCardChar(int value);
int getSuiteValue(card_t card);
char getSuiteChar(int value);
















/*****************************************************************************************************/
/*****************************************************************************************************/
/*****************************************************************************************************
 *********** MAIN
 *****************************************************************************************************/
/*****************************************************************************************************/
/*****************************************************************************************************/


/*******************************************************************************
	*  Intro:
	*
	* 	This method is the main program
	*
	* 	Author: N/A
	*
	* 	[Inputs = None]
	* 	[Outputs = None]
*******************************************************************************/

int main(int argc, char * argv[])
{
 if(argc > 1){
     if(strcmp(argv[1], "debug")){
     	debugMode = 1;
     } else {
     	printf("ERROR: Invalid command line input\n");
     	return 1;
     }
 }
	intro();
	main_menu();
  return 0;
}


void header(void)
{
	/* makes 100 new lines */
 int i;
 int blankLines = 100;
 if (debugMode) blankLines = 5;

	for(i=0;i<blankLines;i++)
		printf("\n");

 if(debugMode) {
 		printf("POCKET CARDS [DEVELOPER]\n");
			printf("------------------------\n");
	} else {
			printf("POCKET CARDS\n");
			printf("------------\n");
			}
}




/*****************************************************************************************************/
/*****************************************************************************************************/
/*****************************************************************************************************
 *********** BASE FUNCTIONS
 *********** These functions are used for the core engine of the program.
 *****************************************************************************************************/
/*****************************************************************************************************/
/*****************************************************************************************************/


/*******************************************************************************
	*  Intro:
	*
	* 	This method asks the user to load a deck of cards or to use a
	*   preset deck of cards
	*
	* 	Author: Sean
	*
	* 	[Inputs = None]
	* 	[Outputs = None]
*******************************************************************************/

void intro (void) {
				header();

    int invalid = 0;
    char choice;
    printf("Welcome to Pocket Cards\n\n");
    printf("Do you want to: \n\n");

    do
    {
        if (invalid != 0) printf("Invalid choice\n\n");
        printf("[N]ew deck \n");
        printf("[L]oad a deck \n");
        printf("[S]elect a preset \n");
        printf("> ");
        scanf(" %c",    &choice);
        invalid ++;
    }while(!isValidIntroChoice(choice)); /*J*/

    switch (choice)
    {
        case 'N':
            main_menu();
            break;

        case 'L':
            load_deck();
            break;

        case 'S':
            select_preset();
            break;
    }






}

/*******************************************************************************
	*  Deck Loading:
	*
	* 	This method asks the user for the filename of the deck.
  *   It then decompresses and loads the contents of the file into the deck.
	*
	* 	Author: Sean
	*
	* 	[Inputs = None]
	* 	[Outputs = None]
*******************************************************************************/

void load_deck (void) {
	header();
  printf("- LOAD A DECK -\n\n");

	int errorCode = -1, errorCounter = 0;
  char fileName[100];
  printf("What is the filename?\n\n");


  do
  {
    if (errorCode == 0) printf("Invalid file\n\n");

    printf("> ");
    scanf("%s", fileName);
    errorCode = load_from_file(fileName, currentDeck);
    errorCounter ++;
    if (errorCounter > 6)
      break;
  } while (errorCode == 0);

  /* Return to intro menu if user inputs incorrectly too many times */
  if (errorCounter > 6) {
    printf("Input timed out\n\n");
    intro();
  }

}

/*******************************************************************************
	*  Select Preset:
	*
	* 	This method allows the user to select a specific card preset
	*
	* 	Author: Sean + ????
	*
	* 	[Inputs = None]
	* 	[Outputs = None]
*******************************************************************************/

void select_preset (void) {
    header();
    printf("- SELECT PRESET -\n\n");

    int invalid = 0;
    char choice;
    printf("Select your starting deck\n\n");

    /*I chose here to put a do-while loop for one specific reason : we have
     to do the action (asking the user to enter a value for choice) before
     checking the condition which is the integer entered by the user must be
     either S, H, C or A. If not, we go back to the beginning of the do
     loop, displaying the error message.*/
    do
    {
        /*Each time we start this while loop again, we set the value of invalid to 0
         so that it doesn't display the message "Invalid choice" directly while
         the user didn't even enter a value yet. However, if he enters a wrong
         choice, invalid would have been incremented before and therefore not be equals
         to 0 anymore what would display the message.*/
        printf("[S]tandard playing cards (52) \n");
        printf("[H]alf a deck (26) \n");
        printf("[C]lub of each value (13) \n");
        printf("[A]ce of each suite (4) \n");
        printf("> ");
        scanf(" %c",    &choice);
        invalid ++;
    } while (!isValidPresetChoice(choice));

    int iter = 0;
    switch (choice)
    {
        case 'S': /* Adds standard playing cards */
            add_card_to_deck('c', 'a', currentDeck); add_card_to_deck('h', 'a', currentDeck); add_card_to_deck('d', 'a', currentDeck); add_card_to_deck('s', 'a', currentDeck);
            for (iter = 2; iter <= 9; iter++) {
                add_card_to_deck('c', iter +'0', currentDeck);
                add_card_to_deck('h', iter +'0', currentDeck);
                add_card_to_deck('d', iter +'0', currentDeck);
                add_card_to_deck('s', iter +'0', currentDeck);
            }
            add_card_to_deck('c', 't', currentDeck); add_card_to_deck('h', 't', currentDeck); add_card_to_deck('d', 't', currentDeck); add_card_to_deck('s', 't', currentDeck);
            add_card_to_deck('c', 'j', currentDeck); add_card_to_deck('h', 'j', currentDeck); add_card_to_deck('d', 'j', currentDeck); add_card_to_deck('s', 'j', currentDeck);
            add_card_to_deck('c', 'q', currentDeck); add_card_to_deck('h', 'q', currentDeck); add_card_to_deck('d', 'q', currentDeck); add_card_to_deck('s', 'q', currentDeck);
            add_card_to_deck('c', 'k', currentDeck); add_card_to_deck('h', 'k', currentDeck); add_card_to_deck('d', 'k', currentDeck); add_card_to_deck('s', 'k', currentDeck);
            break;

        case 'H':
            add_card_to_deck('c', 'a', currentDeck); add_card_to_deck('d', 'a', currentDeck);
            for (iter = 2; iter <= 9; iter++) {
                add_card_to_deck('c', iter +'0', currentDeck);
                add_card_to_deck('d', iter +'0', currentDeck);
            }
            add_card_to_deck('c', 't', currentDeck);  add_card_to_deck('d', 't', currentDeck);
            add_card_to_deck('c', 'j', currentDeck); add_card_to_deck('d', 'j', currentDeck);
            add_card_to_deck('c', 'q', currentDeck); add_card_to_deck('d', 'q', currentDeck);
            add_card_to_deck('c', 'k', currentDeck); add_card_to_deck('d', 'k', currentDeck);
            break;

        case 'C':
            add_card_to_deck('c', 'a', currentDeck);
            for (iter = 2; iter <= 9; iter++) {
                add_card_to_deck('c', iter +'0', currentDeck);
            }
            add_card_to_deck('c', 't', currentDeck);
            add_card_to_deck('c', 'j', currentDeck);
            add_card_to_deck('c', 'q', currentDeck);
            add_card_to_deck('c', 'k', currentDeck);
            break;

        case 'A':
            add_card_to_deck('c', 'a', currentDeck);
            add_card_to_deck('d', 'a', currentDeck);
            add_card_to_deck('h', 'a', currentDeck);
            add_card_to_deck('s', 'a', currentDeck);
            break;
    }
}


/*******************************************************************************
	*  Main Menu:
	*
	* 	This method is the main part of the program that lets the user
  *   explore and utilise various functionality.
	*
	* 	Author: Sean
	*
	* 	[Inputs = None]
	* 	[Outputs = None]
*******************************************************************************/


void main_menu (void) {
   header();
    printf("- MAIN MENU -\n\n");

    printf("There are %d cards in the deck \n\n", totalCardCount);

    int invalid = 0;
    char choice;
    printf("* What would you like to do? \n\n");

    do
    {
        if (invalid != 0) printf("Invalid choice\n\n");
        printf("* [V]iew my deck \n");
        printf("* [A]dd a card \n");
        printf("* [R]emove from deck \n");
        printf("* [M]ix my deck \n");
        printf("* [S]ort my deck \n");
        printf("* [E]xport my deck \n");
        printf("* [Q]uit \n");
        printf("* > ");
        scanf(" %c",    &choice);
        invalid ++;
    }while(!isValidMainMenuChoice(choice)); /*J*/

    switch (choice)
    {
        case 'V':
            view_deck(1);
            break;

        case 'A':
            add_card(currentDeck, totalCardCount);
            break;

        case 'R':
            remove_card(currentDeck);
            break;

        case 'M':
            mix();
            break;

        case 'S':
            sort();
            break;

        case 'E':
            export_deck();
            break;

        case 'Q':
            break;
    }
}



/*******************************************************************************
	*  View Deck:
	*
	* 	This method shows the user their deck of cards
	*
	* 	Author: Charles + Sean
	*
	* 	[Inputs = View mode (0 = simplified, 1 = ask for user input)]
	* 	[Outputs = None]
*******************************************************************************/

void view_deck (int viewMode) {
  header();
  printf("- VIEW MY DECK -\n\n");

  int invalid = 0;
		char choice;
  printf("Here is your deck: \n\n");

  printf("[============================]\n");
    int i = 0, count = 0;
    for(i=0;i<totalCardCount;i++)
      {
        if(count%5==0 && i != 0)
        {
          printf("\n");
        }


    				if(!debugMode) { printf("{ %c %s } ", toupper(currentDeck[i].value), get_suite_icon(currentDeck[i].suite)); }
        else { printf("(%c,%c) ", currentDeck[i].value, currentDeck[i].suite);}

        count++;
      }


  printf("\n[============================]\n");
  printf("Cool. \n\n");

  do
  {
    if (invalid != 0) printf("Invalid choice\n\n");
    printf("[B]ack to menu \n");
    printf("> ");
    scanf(" %c",	&choice);
    invalid ++;
  } while (choice !='B');

  switch (choice)
  {
    case 'B':
      main_menu();
      break;
  }
}



/*******************************************************************************
	* Do this comment
*******************************************************************************/


int add_card (card_t cards[], int cardsCount) {
    header();
    printf("- ADD A CARD -\n\n");

    int bool2=0;

    char cardValue, cardSuite;

    if(cardsCount >= MAX_DECK)
    {
        bool2=1;
        printf("Cannot add more cards.\n");
        printf("Deck is full.\n");
        main_menu();
    }
    if(bool2==0)
    {
            do{
            printf("Here is your card:\n"
                   "[=======]\n"
                   "  {   }   \n"
                   "[=======]\n"
                   "\nWhat is the card’s value?\n\n"
                   "[A][2][3][4][5][6][7][8][9][T][J][Q][K]\n> ");
            scanf(" %c",&cardValue);
            }while (!isValidValue(cardValue)); /*J*/


        header();
        printf("- ADD A CARD -\n\n");
        do{
            printf(    "Here is your card:\n"
                   "[=======]\n"
                   "  { %c }   \n"
                   "[=======]\n\n"
                   "What is the card’s suite?\n\n"
                   "◆ [D]iamonds\n"
                   "♣ [C]lub\n"
                   "♥ [H]earts\n"
                   "♠ [S]pades\n> ", cardValue);

            scanf(" %c",&cardSuite);
        }while(!isValidSuite(cardSuite)); /*J*/
        cardSuite = tolower(cardSuite);

        char choice[10];
        if (card_exists_in_deck(cardSuite, cardValue, currentDeck) == 1) {
            bool2=1;
            header();
            printf("- ADD A CARD -\n\n");
            printf("This card is already in the deck\n\n");
            printf("[OK] \n");
            printf("> ");
            scanf("%s",    choice);
            main_menu();
            return 0;
        }

        if (bool2 == 0) {
            header();
            printf("- ADD A CARD -\n\n");

            add_card_to_deck(cardSuite, cardValue, currentDeck);


            int invalid = 0;
            char choice;
            do
            {
                if (invalid != 0) printf("Invalid choice\n\n");

                printf("[=======]\n"
                       "  {%c%s}   \n"
                       "[=======]\n"
                       "\nThis card has been\n"
                       "added to your deck.\n\n"
                       "[A]dd another card\n"
                       "[V]iew the deck\n"
                       "[R]emove this card\n"
                       "[B]ack to menu\n> "
                       ,cardValue, get_suite_icon(cardSuite));
                scanf(" %c",&choice);
                invalid ++;
            } while (!isValidActionChoice(choice, 4)); /*J*/

            switch (choice)
            {
                case 'A':
                    add_card(cards, cardsCount);
                    break;

                case 'V':
                    view_deck(1);
                    break;

                case 'R':
                    remove_card_specific(currentDeck, cardValue, cardSuite);
                    main_menu();
                    break;

                case 'B':
                    main_menu();
                    break;
            }
        }
    }

    return cardsCount;
}


/*******************************************************************************
	* Do this comment
*******************************************************************************/


void remove_card (card_t deck[]) {

	int invalid=0;
	char suite,value,choice;
	printf("value\n");
	scanf(" %c",&value);
	value = tolower(value);

	 printf("suite\n");
	scanf(" %c",&suite);
	suite = tolower(suite);

	remove_card_specific(deck, value, suite);


	 while (choice !='V'&&choice!='B')
      {
         printf( "[V]iew the deck\n"
                "[B]ack to menu\n> ");
        scanf(" %c",&choice);

	if(choice !='V'&&choice!='B'){
		invalid++;
	}

	 if (invalid != 0) printf("Invalid choice\n\n");

      }


      switch (choice)
      {
        case 'V':
          view_deck(1);
          return;

        case 'B':
          main_menu();
          return;
      }

}


void remove_card_specific (card_t deck[], char value, char suite) {
int i, k;
	if(  totalCardCount<=52&& totalCardCount>0)
	{
		for(i=0;i<totalCardCount;i++)
		{

			if(deck[i].value==value&&deck[i].suite==suite)
			{
				for (k = i ; k < totalCardCount- 1; k++)
				{
					deck[k].value = deck[k+1].value;
					deck[k].suite = deck[k+1].suite;

				}
				totalCardCount --;
				break;

			}
		}

	}
}




/*******************************************************************************
	*  Sort Deck:
	*
	* 	This method allows users to choose how to sort the deck
	*
	* 	Author: Sean
	*
	* 	[Inputs = None]
	* 	[Outputs = None]
*******************************************************************************/

void sort (void) {
    header();
    printf("- SORT MY DECK -\n\n");

    int invalid = 0;
    int iterations = 0;
    char choice = 'B';
    printf("How would you like to sort the deck? \n\n");

    do
    {
        if (invalid != 0) printf("Invalid choice\n\n");
        printf("[S]uite \n");
        printf("[V]alue \n");
        printf("> ");
        scanf(" %c",    &choice);
        invalid ++;
    } while (!isValidSortType(choice));

    char choice2[10];
    if (deck_is_sorted() == 1) {
        printf("\nThe deck is already sorted\n\n");
        printf("[OK] \n");
        printf("> ");
        scanf("%s",    choice2);
        main_menu();
        return;
    }

    int sortMode = 2;
    switch (choice)
    {
        case 'S':
            sortMode = 1;
            break;

        case 'V':
            sortMode = 2;
            break;
    }

    invalid = 0;
    printf("\nWhat algorithm would you like to use? \n\n");

    do
    {
        if (invalid != 0) printf("Invalid choice\n\n");
        printf("[B]ubble Sort \n");
        printf("[I]nsertion Sort \n");
        printf("> ");
        scanf(" %c",    &choice);
        invalid ++;
    } while (!isValidSortMethod(choice));

    switch (choice)
    {
        case 'B':
            iterations = sort_bubble(sortMode);
            break;

        case 'I':
            iterations = sort_insertion(sortMode);
            break;
    }


    header();
    printf("- SORT MY DECK -\n\n");

    printf("\nAll sorted now.\n");
    printf("Steps - %d\n", iterations);
    printf("Cards - %d\n\n", totalCardCount);

    invalid = 0;
    do
    {
        if (invalid != 0) printf("Invalid choice\n\n");
        printf("[V]iew my deck \n");
        printf("[B]ack to menu \n");
        printf("> ");
        scanf(" %c",    &choice);
        invalid ++;
    } while (!isValidActionChoice(choice, 2));

    switch (choice)
    {
        case 'B':
            main_menu();
            return;

        case 'V':
            view_deck(1);
            return;
    }

}


/*******************************************************************************
 *  Bubble Sort:
 *
 *     This method sorts the cards via bubble sort
 *
 *     Author: Joyce
 *
 *     [Inputs = Sort Mode (1 = by suite, 2 = by value)]
 *     [Outputs = Iterations it took to sort]
 *******************************************************************************/

int sort_bubble (int sortMode) {
        int i = 0,j = 0;
        int n = totalCardCount;
        switch (sortMode) {
            case 1:
                for (i = 0; i < n-1; i++)
                    for (j = 0; j < n-i-1; j++)
                        if (getSuiteValue(currentDeck[j]) > getSuiteValue(currentDeck[j+1])){
                            swap(&currentDeck[j], &currentDeck[j+1]);

                        }
                break;
            case 2:
                for (i = 0; i < n-1; i++)
                    for (j = 0; j < n-i-1; j++)
                        if (getCardValue(currentDeck[j]) > getCardValue(currentDeck[j+1]))
                            swap(&currentDeck[j], &currentDeck[j+1]);
                break;
            default:
                break;
        }
        return i*j;

}




/*******************************************************************************
 *  Insertion Sort:
 *
 *     This method sorts the cards via insertion sort
 *
 *     Author: Joyce
 *
 *     [Inputs = Sort Mode (1 = by suite, 2 = by value)]
 *     [Outputs = Iterations it took to sort]
 *******************************************************************************/

int sort_insertion (int sortMode) {
    int i = 0,j = 0;
    int key;
    card_t keyCard;
    int n = totalCardCount;
    switch (sortMode){
        case 1:
            for (i = 1; i < n; i++) {
                key = getSuiteValue(currentDeck[i]);
                keyCard = currentDeck[i];
                j = i - 1;
                while (j >= 0 && getSuiteValue(currentDeck[j]) > key) {
                    currentDeck[j + 1] = currentDeck[j];
                    j = j - 1;
                }
                currentDeck[j + 1] = keyCard;
            }
            break;
        case 2:
            for (i = 1; i < n; i++) {
                key = getCardValue(currentDeck[i]);
                keyCard = currentDeck[i];
                j = i - 1;
                while (j >= 0 && getCardValue(currentDeck[j]) > key) {
                    currentDeck[j + 1] = currentDeck[j];
                    j = j - 1;
                }
                currentDeck[j + 1] = keyCard;
            }
            break;
        default:
            break;

    }

    return i*j;
}

/*******************************************************************************
 *  Swap Tool:
 *
 *     This method swap 2 cards
 *
 *     Author: Joyce
 *
 *     [Inputs = address of 2 cards]
 *     [Outputs = N/A]
 *******************************************************************************/
void swap(card_t *xp, card_t *yp){

    if(debugMode) printf("DEBUG: swapping (%c,%c) <-> (%c,%c)\n", xp->value, xp->suite, yp->value, yp->suite);
    card_t temp = *xp;
    *xp = *yp;
    *yp = temp;
}



/*******************************************************************************
	* Do this comment
*******************************************************************************/

void mix (void) {

			/* ask which mixing option */

}




/*******************************************************************************
	* Do this comment
*******************************************************************************/

void mix_shuffle (void) {


}


/*******************************************************************************
	* Do this comment
*******************************************************************************/

void mix_norepeats (void) {


}


/*******************************************************************************
	* Do this comment
*******************************************************************************/

void mix_twister (void) {


}


/*******************************************************************************
	* Do this comment
*******************************************************************************/

void export_deck (void) {

		header();
  printf("- EXPORT DECK -\n\n");

		int errorCode = -1, errorCounter = 0;
  char fileName[100];
  printf("What is the filename (with extension)?\n\n");


  do
  {
    if (errorCode == 0) printf("Invalid file\n\n");

    printf("> ");
    scanf("%s", fileName);
    errorCode = export_deck_data(fileName, currentDeck);
    errorCounter ++;
    if (errorCounter > 6)
      break;
  } while (errorCode == 0);

  /* Return to intro menu if user inputs incorrectly too many times */
  if (errorCounter > 6) {
    printf("Input timed out\n\n");
    intro();
  }
  else
  {
  	main_menu();
  }






}






/*****************************************************************************************************/
/*****************************************************************************************************/
/*****************************************************************************************************
 *********** HELPER FUNCTIONS
 *********** These functions perform a simple task.
 *****************************************************************************************************/
/*****************************************************************************************************/
/*****************************************************************************************************/



/*int decrypt (card_t deck[] , char filename[] ) {
			return 1;
}*/



/*******************************************************************************
	*  Add Card to Deck:
	*
	* 	This method generates a new card given the suite and value,
	*  and then it adds it to the deck.
	*
	* 	Author: Sean
	*
	* 	[Inputs = Suite ('c = club', 'h = heart', 'd = diamond', 's = spade'),
  *             Value ('a = ace', '2', '3', '4', '5', '6', '7', '8', '9',
  *                    't = ten', 'j = jack', 'q = queen', 'k = king')]
	* 	[Outputs = None ]
*******************************************************************************/
void add_card_to_deck (char suite, char value, card_t deck[]) {
    if(debugMode) printf("DEBUG: Adding (%c, %c)\n", suite, value);
  deck[totalCardCount].value = value;
  deck[totalCardCount].suite = suite;
  totalCardCount ++;
}


/*******************************************************************************
	*  Load From File:
	*
	* 	This method retreives the card file the user requests.
	*  It checks if the file exists
	*  It then loads in all the cards from the file into the card deck
	*
	* 	Author: Sean
	*
	* 	[Inputs = Deck of cards, Filename of encrypted file]
	* 	[Outputs = Success code (0 = failed, 1 = success) ]
*******************************************************************************/

int load_from_file (char fileName[], card_t deck[])
{
    if (file_exists(fileName) == 0)
	{
	    return 0;
	}
	char* deckChar  = malloc(150*sizeof(char));
    totalCardCount = Decompress(fileName, FileLetterTab, FileBitTab, deckChar);
    deck = decrypt_deck(deckChar,deck, totalCardCount);
    int i;
    for(i=0;i<totalCardCount;i++)
    {
        printf("%c%s ", deck[i].value, get_suite_icon(deck[i].suite));
    }


  /*
    Find the file,
 	Decompress file
 	Unencrypt
 	Add each card to the deck

  */

  return 1;
}

int file_exists (char* fileName) {


  		if (debugMode) printf("DEBUG: Checking if file '%s' exists\n", fileName);
  		FILE *fp;
    if ((fp = fopen(fileName, "r")))
    {
        fclose(fp);
        return 1;
    }
    return 0;
}


/*******************************************************************************
	*  Export Deck Data:
	*
	* 	Add comment here
*******************************************************************************/

int export_deck_data (char fileName[], card_t deck[])
{
	char* encryptedDeckPointer = encrypt_deck(deck);
	Compress(encryptedDeckPointer, fileName, FileLetterTab, FileBitTab);
	return 1;
}

char* encrypt_deck (card_t deck[])
{
    int cardIndex, i=0;
    char* encryptedDeckPointer = malloc(150*sizeof(char));
    for(cardIndex = 0; cardIndex < totalCardCount; cardIndex++)
    {
        encryptedDeckPointer[i] = deck[cardIndex].value;
        encryptedDeckPointer[i+1] = deck[cardIndex].suite;
        i+=2;
    }
    encryptedDeckPointer[2*totalCardCount] = '\0';
    newEncrypt = PolyEncryption(encryptedDeckPointer, KEYS);
    FILE* file = NULL;
    char* nameEncryptedFile="encrypted.txt";
	file = fopen(nameEncryptedFile, "w");
	if(file==NULL)
	{
		printf("Error! Can't open file.\n");
	}
	else
	{
	    for(i=0;i<totalCardCount*2;i++)
        {
            fprintf(file,"%c",newEncrypt[i]);
        }
        fclose(file);
	}
    return newEncrypt;
}

card_t * decrypt_deck (char* deckChar, card_t deck[], int totalCard)
{
    int cardIndex;
    char* decryptedDeckPointer = malloc(totalCard*sizeof(card_t));
    decryptedDeckPointer=PolyDecryption(deckChar, KEYS);
    for (cardIndex = 0; cardIndex < totalCard; cardIndex+=2)
    {
        deck[cardIndex].value=decryptedDeckPointer[cardIndex];
        deck[cardIndex].suite=decryptedDeckPointer[cardIndex+1];
    }
    return deck;
}


/*******************************************************************************
	*  Is Deck Sorted?:
	*
	* 	This method checks if the deck is sorted
	*
	* 	Author: ???
	*
	* 	[Inputs =None]
	* 	[Outputs = Success code (0 = unsorted, 1 = sorted) ]
*******************************************************************************/

int deck_is_sorted () {
 /* printf("\n** TODO: Check if the deck is sorted \n");*/
  return 0;
}


/*******************************************************************************
	*   Card Exists in Deck?:
	*
	* 	This method checks if the card exists in the deck
	*
	* 	Author: Charles
	*
	* 	[Inputs = suite]
	* 	[Outputs = Success code (0 = doesnt exit, 1 = exists) ]
*******************************************************************************/

int card_exists_in_deck (char cardSuite, char cardValue,  card_t deck[]) {

  int i = 0;
  for(i=0;i<totalCardCount;i++)
  {
  	if (debugMode) printf("%c + %c   |  %c + %c \n", deck[i].value, deck[i].suite, cardValue, cardSuite);
    if(cardValue == deck[i].value && cardSuite == deck[i].suite)
    {
      return 1;
    }
  }
  return 0;
}



/*******************************************************************************
	*   Get Suite Icon
	*
	* 	This method returns the char of the given suite
	*
	* 	Author: Charles
	*
	* 	[Inputs = None]
	* 	[Outputs = Suite icon ]
*******************************************************************************/

char* get_suite_icon(char suitValue)
{
		static char displaySuite[5];
		if(suitValue=='h')
		{
      strcpy(displaySuite, "♥");
		}
		else if(suitValue=='s')
		{
      strcpy(displaySuite, "♠");
		}
		else if(suitValue=='c')
		{
      strcpy(displaySuite, "♣");
		}
		else if(suitValue=='d')
		{
      strcpy(displaySuite, "♦");
		}
	return displaySuite;
}



/*******************************************************************************
 *  Get card value :
 *
 *     This method returns the int of the given card value
 *
 *     Author: Joyce
 *
 *     [Inputs = card_t card]
 *     [Outputs = value of card (int) ]
 *******************************************************************************/
int getCardValue(card_t card){

				char cardValue = tolower(card.value);
    switch (cardValue){
        case 'a':
            return 1;
            break;
        case 't':
            return 10;
            break;
        case 'j':
            return 11;
            break;
        case 'q':
            return 12;
            break;
        case 'k':
            return 13;
            break;
        default:
            return card.value -'0';
            break;
    }

}

/*******************************************************************************
 *  Get card char:
 *
 *     This method returns the char of the given card value (int)
 *
 *     Author: Joyce
 *
 *     [Inputs = int value]
 *     [Outputs = char value ]
 *******************************************************************************/
char getCardChar(int value){
    switch (value){
        case 1:
            return 'a';
            break;
        case 10:
            return 't';
            break;
        case 11:
            return 'j';
            break;
        case 12:
            return 'q';
            break;
        case 13:
            return 'k';
            break;
        default:
            return value +'0';
            break;
    }

}

/*******************************************************************************
 *  Get suite value :
 *
 *     This method returns the int of the given card suite
 *
 *     Author: Joyce
 *
 *     [Inputs = card_t card]
 *     [Outputs = value of suite (int) ]
 *******************************************************************************/
int getSuiteValue(card_t card){
    switch (card.suite){
        case 's':
            return 4;
            break;
        case 'h':
            return 3;
            break;
        case 'c':
            return 2;
            break;
        case 'd':
            return 1;
            break;
        default:
            return 0;
            break;
    }

}


/*******************************************************************************
 *  Get suite char:
 *
 *     This method returns the char of the given card value (int)
 *
 *     Author: Joyce
 *
 *     [Inputs = int value]
 *     [Outputs = char value ]
 *******************************************************************************/
char getSuiteChar(int value){
    switch (value){
        case 4:
            return 's';
            break;
        case 3:
            return 'h';
            break;
        case 2:
            return 'c';
            break;
        case 1:
            return 'd';
            break;
        default:
            return 'E';
            break;
    }

}


int isValidValue(char input){
    int i;

    /*Loop through DECK_VALUE and compare with input*/
    for(i=0;i<DECK_VALUE_LEN;i++){
            if(input==DECK_VALUE[i])
                return 1;
    }

    printf(INVALID_VALUE_ERROR);
    return 0;
}

int isValidSuite(char input){
    int i;
    /*Loop through DECK_SUITE and compare with input*/
    for(i=0;i<DECK_SUITE_LEN;i++){
            if(input==DECK_SUITE[i])
                return 1;
    }

    printf(INVALID_SUITE_ERROR);
    return 0;
}

int isValidMainMenuChoice(char input){
    int i;
    /*Loop through MAIN_MENU_CHOICES and compare with input*/
    for(i=0;i<7;i++){
            if(input==MAIN_MENU_CHOICES[i])
                return 1;
    }
    printf(INVALID_ACTION_ERROR);
    return 0;
}

/*******************************************************************************
 * This function checks whether the action choice input by user is valid,
 * Sort and Mix menu action choice = [V,B]
 * Add and remove cards by value/suite menu action choice = [V,B,R]
 * Remove 1 card menu action choice = [V,B,R,A]
 * Return 1 if input is valid, 0 if not
 * inputs:
 * - char input (user input)
 * - int num (number of avalible action)
 * outputs:
 * - return 1 or 0
 *******************************************************************************/

int isValidActionChoice(char input,int num){
    int i;
    /*Loop through ACTION_CHOICES and compare with input*/
    for(i=0;i<num;i++){
            if(input==ACTION_CHOICES[i])
                return 1;
    }
    printf(INVALID_ACTION_ERROR);
    return 0;
}


int isValidIntroChoice(char input){
    int i;
    /*Loop through ACTION_CHOICES and compare with input*/
    for(i=0;i<3;i++){
            if(input==INTRO_CHOICES[i])
                return 1;
    }
    printf(INVALID_ACTION_ERROR);
    return 0;
}

int isValidRemoveType(char input){
    int i;
    /*Loop through ACTION_CHOICES and compare with input*/
    for(i=0;i<3;i++){
            if(input==REMOVE_TYPE[i])
                return 1;
    }
    printf(INVALID_REMOVE_TYPE_ERROR);
    return 0;
}

int isValidSortType(char input){
    /*Compare input with 2 char [S/V]*/
    if(input=='S'||input=='V')
        return 1;
    printf(INVALID_SORT_TYPE_ERROR);
    return 0;
}

int isValidSortMethod(char input){
    int i;
    /*Loop through SORT_METHOD and compare with input*/
    for(i=0;i<2;i++){
            if(input==SORT_METHOD[i])
                return 1;
    }
    printf(INVALID_SORT_METHOD_ERROR);
    return 0;
}

int isValidMixMethod(char input){
    int i;
    /*Loop through MIX_TYPE and compare with input*/
    for(i=0;i<3;i++){
            if(input==MIX_TYPE[i])
                return 1;
    }
    printf(INVALID_MIX_TYPE_ERROR);
    return 0;
}

int isValidPresetChoice(char input){
    int i,j;
    /*Loop through MIX_TYPE and compare with input*/
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            if(input==PRESET_CHOICE[i])
                return 1;
        }
    }
    printf("Invalid Choice\n\n");
    return 0;
}









