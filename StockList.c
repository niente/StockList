/***************************************************************************************
This program reads a text file of stocks and quotes to create a circularly 
doubly-linked list of stacks (in alphabetical order).
The list is printed to screen, forwards and backwards.
The user can search for two stocks in the list, and print the sub-list between them.
The user can search for a stock and input the number of quotes to be read from its stack.
****************************************************************************************/
#define _CRT_SECURE_NO_DEPRECATE // allow use of fopen in Visual Studio
#define _CRTDBG_MAP_ALLOC // locate memory leaks
#define FLUSH while( getchar() != '\n') // clean user input
#define MEM_ERROR printf("Not enough memory!\n")
#define SIZE 10
#define DEFAULT_INPUT_FILENAME "stocks.txt"
#define DUMMY_TRAILER '\177'

#ifdef _MSC_VER
#include <crtdbg.h>  // needed to check for memory leaks (Windows only!)
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct stacknode STACKNODE;
struct stacknode
{
	double price;
	struct stacknode *next;
};

typedef struct listnode LISTNODE;
struct listnode
{
	char name[SIZE + 1];
	LISTNODE *forw;
	LISTNODE *back;
	STACKNODE *quotes;
	int stack_count;
};

// function prototypes
FILE *openFile(char *infilename);
LISTNODE *init_list(void);
void insert(LISTNODE *list, char *name, double price);
void traverse_forw(LISTNODE *list);
void traverse_back(LISTNODE *list);
void delete_list(LISTNODE **head);
STACKNODE *push(STACKNODE *quotes, double price);
void delete_stack(LISTNODE *item);
void build_list(FILE *inFile, LISTNODE *stockList);
void display_manager(LISTNODE *stockList);
void search_manager(LISTNODE *stockList);
LISTNODE *search(LISTNODE *stockList, char target[]);

int main( void )
{
	int i = 0;
	char infilename[100];
	FILE *inFile = openFile(infilename); // get user input for file & open

	LISTNODE *stockList;
	stockList = init_list(); // initialize & build the list
	build_list(inFile, stockList);

	traverse_forw(stockList); // print forwards & backwards
	traverse_back(stockList);

	display_manager(stockList); // accept user input to print sub-lists
	search_manager(stockList); // accept user input to print quotes

	// close file validation
	if (fclose(inFile) == EOF)
	{
		printf("Error closing input file.\n");
		exit(301);
	}

	delete_list(&stockList); // free memory

    #ifdef _MSC_VER
		_CrtDumpMemoryLeaks();
        printf( _CrtDumpMemoryLeaks() ? "Memory Leak\n" : "No Memory Leak\n");
		system("pause");
    #endif

    return 0;
}

/***********************OPENFILE****************************
The openFile function takes user input for a filename
and opens the file. It returns a pointer to the file stream.
Pre   infilename - char ptr returns this to main()
Post  returns FILE * to the opened input file
*/
FILE *openFile(char *infilename)
{
	printf("Enter input file name, or type Q to run default stocks.txt: ");
	scanf("%s[^\n]", infilename);
	FLUSH;
	if (strcmp(infilename, "Q") == 0 || strcmp(infilename, "q") == 0)
	{
		strcpy(infilename, DEFAULT_INPUT_FILENAME);
	}
	printf("Input file name: %s\n\n", infilename);
	FILE *inFile = fopen(infilename, "r");
	if (!inFile)
	{
		printf("Unable to open %s!\n", infilename);
		exit(101);
	}
}

/*****************INIT_LIST***********************
Initialization of a circularly doubly-linked
list with one sentinel node
*/
LISTNODE *init_list(void)
{
	LISTNODE *list;

	// allocate the sentinel node
	list = (LISTNODE *)malloc(sizeof (LISTNODE));
	if (!list)
	{
		printf("Error in init_list!\n");
		exit(1);
	}

  	list->name[0] = DUMMY_TRAILER;
	list->name[1] = '\0';
	list->quotes = push(list->quotes, -1);
	list->forw = list;
	list->back = list;

	return list;
}

/*****************BUILD_LIST***********************
Accepts a filestream and list, reads each line of
file into the list with a call to insert()
*/
void build_list(FILE *inFile, LISTNODE *stockList)
{
	char name[SIZE + 1];
	double price;
	while (fscanf(inFile, "%s %lf", name, &price) != EOF)
	{
		//printf("%s\t%f\n", name, price);
		insert(stockList, name, price);
	}
}

/*****************INSERT***********************
Inserts a listnode into the circle list.
*/
void insert(LISTNODE *list, char *name, double price)
{
	LISTNODE *curr = list->forw;
	LISTNODE *prev = list;
	LISTNODE *pnew;

	// traverse list to insert new entry alphabetically
	while (strcmp(name, curr->name) > 0)
		curr = curr->forw;
	prev = curr->back;

	if (strcmp(name, curr->name) < 0) // if the new stock is after the previous... insert
	{
		pnew = (LISTNODE *)malloc(sizeof (LISTNODE)); // allocate a piece of the list
		pnew->quotes = push(pnew->quotes, price); // push price value to its stack
		if (!pnew)
		{
			printf("Fatal memory allocation error in insert!\n");
			exit(3);
		}
		strcpy(pnew->name, name);
		pnew->forw = curr;
		pnew->back = prev;
		prev->forw = pnew;
		curr->back = pnew;

		pnew->stack_count = 1; // initialize count
		//printf("INSERTING: %s\t%6.2f\n", pnew->name, pnew->quotes->price);
	}
	else // if this stock is already in the list, push a new quote:
	{
		curr->quotes = push(curr->quotes, price);
		curr->stack_count++;
	}
}

/***********************PUSH************************
Stack Insert: insert in the beginning
*/
STACKNODE *push(STACKNODE *stack, double price)
{
	STACKNODE *pnew;

	pnew = (STACKNODE *)malloc(sizeof(STACKNODE)); // allocate a new part of stack
	if (!pnew)
	{
		printf("... error in push!\n");
		exit(1);
	}
	pnew->price = price; // assign values
	pnew->next = stack;
	stack = pnew; // point "top" to the newest element
	//printf("Pushing: %6.2f\n", stack->price);
	return stack;
}

/**************TRAVERSE_FORW**********************
Traverses forward a circularly doubly-linked
list with one sentinel node to print out the
contents of each node
*/
void traverse_forw(LISTNODE *list)
{
	putchar('\n');
	list = list->forw; // skip the dummy node
	printf("TRAVERSING FORWARD\n------------------\n");
	while (list->name[0] != DUMMY_TRAILER)
	{
		printf("%s\t%6.2f\n", list->name, list->quotes->price);
		list = list->forw;
	}
	return;
}

/**************TRAVERSE_BACK**********************
Traverses backward a circularly doubly-linked
list with one sentinel node to print out the
contents of each node
*/
void traverse_back(LISTNODE *list)
{
	putchar('\n');
	list = list->back; // skip the dummy node
	printf("TRAVERSING BACK\n---------------\n");
	while (list->name[0] != DUMMY_TRAILER)
	{
		printf("%s\t%6.2f\n", list->name, list->quotes->price);
		list = list->back;
	}
	return;
}

/*****************DISPLAY_MANAGER********************
Accepts user input for 2 stocks. Checks to see if the stocks
are in the list. Prints the sub-list of stocks between the 2
choices, forwards and backwards, excluding the sentinel node.
*/
void display_manager(LISTNODE *stockList)
{
	printf("\nDISPLAY MANAGER\n---------------\nPrint a sublist of stocks, forwards and backwards.\n");
	char stockA[SIZE + 1] = "test";
	char stockB[SIZE + 1] = "test";
	while (strcmp(stockA, "Q") != 0 && strcmp(stockA, "q") != 0
		   && strcmp(stockB, "Q") != 0 && strcmp(stockB, "q") != 0)
	{
		// accept user input
		printf("Enter a stock symbol, or type Q to quit: ");
		scanf("%s[^\n]", stockA);
		FLUSH;
		if (strcmp(stockA, "Q") == 0 || strcmp(stockA, "q") == 0)
			break;
		printf("Enter a stock symbol: ");
		scanf("%s[^\n]", stockB);
		FLUSH;
		if (strcmp(stockB, "Q") == 0 || strcmp(stockB, "q") == 0)
			break;

		printf("%s to %s:\n", stockA, stockB);

		// search list for stockA and stockB
		LISTNODE *ptrA;
		LISTNODE *ptrB;
		LISTNODE *pWalk;
		ptrA = search(stockList, stockA);
		ptrB = search(stockList, stockB);
		pWalk = ptrA;
		// if both stocks were found, print the sublist
		if (ptrA->name[0] != DUMMY_TRAILER && ptrB->name[0] != DUMMY_TRAILER)
		{
			printf("Printing forwards:\n");
			while (pWalk != ptrB->forw)
			{
				if (pWalk->name[0] == DUMMY_TRAILER)
					pWalk = pWalk->forw;
				else
				{
					printf("%s\t%6.2f\n", pWalk->name, pWalk->quotes->price);
					pWalk = pWalk->forw;
				}
			}
			printf("\nPrinting backwards:\n");
			pWalk = ptrB;
			while (pWalk != ptrA->back)
			{
				if (pWalk->name[0] == DUMMY_TRAILER)
					pWalk = pWalk->back;
				else
				{
					printf("%s\t%6.2f\n", pWalk->name, pWalk->quotes->price);
					pWalk = pWalk->back;
				}
			}
			printf("\n");
		}
		else // if either stock was not found, print an error
		{
			if (ptrA->name[0] == DUMMY_TRAILER)
				printf("%s, ", stockA);
			if (ptrB->name[0] == DUMMY_TRAILER)
				printf("%s ", stockB);
			printf("not found!\n\n");
		}
	}
}

/**********************SEARCH***************************
Searches the list for a stock. Returns a pointer to either 
the target, or the sentinel node if not found.
*/
LISTNODE *search(LISTNODE *stockList, char target[])
{
	stockList = stockList->forw; // skip sentinel
	while (strcmp(stockList->name, target) != 0 && stockList->name[0] != DUMMY_TRAILER)
		stockList = stockList->forw;

	return stockList;
}


/*****************SEARCH_MANAGER********************
Searches the list for a stock, then prints the desired
number of quotes from the stack.
*/
void search_manager(LISTNODE *stockList)
{
	printf("SEARCH MANAGER\n-------------\nSearch for a stock and desired # of quotes.\n");
	char stock[SIZE + 1] = "test";
	int n = 0, i = 0;
	//STACKNODE *temp = stockList->quotes;
	
	while (strcmp(stock, "Q") != 0 && strcmp(stock, "q") != 0)
	{
		// accept user input
		printf("Enter a stock symbol, or type Q to quit: ");
		scanf("%s[^\n]", stock);
		FLUSH;
		if (strcmp(stock, "Q") == 0 || strcmp(stock, "q") == 0)
			break;
		// search for the stock
		printf("Searching for %s:\n", stock);
		LISTNODE *temp;
		STACKNODE *hold; // COPY
		temp = search(stockList, stock);
		hold = temp->quotes;
		if (temp->name[0] == DUMMY_TRAILER)
			printf("Couldn't find %s\n\n", stock);
		else // if it was found:
		{
			// accept user input for quotes
			printf("Please enter the desired number of quotes: ");
			scanf("%d[^\n]", &n);
			FLUSH;
			if (n > temp->stack_count)
				printf("Sorry, only able to print %d quotes.\n\n", temp->stack_count);
			else
			{
				for (i = 1; i <= n; i++)
				{
					printf("%6.2f\t", hold->price); // print each price
					hold = hold->next; // move to next element of stack
				}
				printf("\n\n");
			}
		}
	}
}

/******************DELETE_STACK******************
Accepts a LISTNODE which contains a STACKNODE 
structure to delete. LISTNODE contains the number
of quotes in the stack.
*/
void delete_stack(LISTNODE *item)
{
	STACKNODE *temp;
	temp = item->quotes;
	int i;
	for (i = 0; i < item->stack_count; i++)
	{
		item->quotes = item->quotes->next;
		free(temp);
		temp = item->quotes;
	}
}

/*****************DELETE_LIST********************
Accepts a pointer to a linked list, and deletes it.
*/
void delete_list(LISTNODE **head)
{
	printf("\nDELETING LIST\n-------------\n");
	LISTNODE *curr = *head;
	LISTNODE *next;

	curr = curr->forw; // skip sentinel

	while (curr->name[0] != DUMMY_TRAILER) // while we aren't on sentinel node...
	{
		next = curr->forw;
		printf("Deleting %s\t%f\n", curr->name, curr->quotes->price);
		delete_stack(curr);
		free(curr);
		curr = next;
	}
	// delete sentinel node
	printf("Deleting %s\n", curr->name);
	free(curr->quotes);
	free(curr);

	*head = NULL;
}

/***************SAMPLE OUTPUT****************

Enter input file name, or type Q to run default stocks.txt: q
Input file name: stocks.txt


TRAVERSING FORWARD
------------------
AAPL    119.73
AMZN    810.11
CSCO     30.04
EBAY     30.64
FB      127.85
GOOG    804.46
INTC     23.94
MSFT     32.60
NFLX    139.39
ORCL     32.78
ZNGA      2.64

TRAVERSING BACK
---------------
ZNGA      2.64
ORCL     32.78
NFLX    139.39
MSFT     32.60
INTC     23.94
GOOG    804.46
FB      127.85
EBAY     30.64
CSCO     30.04
AMZN    810.11
AAPL    119.73

DISPLAY MANAGER
---------------
Print a sublist of stocks, forwards and backwards.
Enter a stock symbol, or type Q to quit: CSCO
Enter a stock symbol: MSFT
CSCO to MSFT:
Printing forwards:
CSCO     30.04
EBAY     30.64
FB      127.85
GOOG    804.46
INTC     23.94
MSFT     32.60

Printing backwards:
MSFT     32.60
INTC     23.94
GOOG    804.46
FB      127.85
EBAY     30.64
CSCO     30.04

Enter a stock symbol, or type Q to quit: TDF
Enter a stock symbol: CSCO
TDF to CSCO:
TDF, not found!

Enter a stock symbol, or type Q to quit: ZNGA
Enter a stock symbol: FB
ZNGA to FB:
Printing forwards:
ZNGA      2.64
AAPL    119.73
AMZN    810.11
CSCO     30.04
EBAY     30.64
FB      127.85

Printing backwards:
FB      127.85
EBAY     30.64
CSCO     30.04
AMZN    810.11
AAPL    119.73
ZNGA      2.64

Enter a stock symbol, or type Q to quit: q
SEARCH MANAGER
-------------
Search for a stock and desired # of quotes.
Enter a stock symbol, or type Q to quit: AAPL
Searching for AAPL:
Please enter the desired number of quotes: 5
119.73  441.94  442.73  430.12  417.71

Enter a stock symbol, or type Q to quit: XLNX
Searching for XLNX:
Couldn't find XLNX

Enter a stock symbol, or type Q to quit: GOOG
Searching for GOOG:
Please enter the desired number of quotes: 6
Sorry, only able to print 5 quotes.

Enter a stock symbol, or type Q to quit: EBAY
Searching for EBAY:
Please enter the desired number of quotes: 2
30.64   52.60

Enter a stock symbol, or type Q to quit: q

DELETING LIST
-------------
Deleting AAPL   119.730000
Deleting AMZN   810.110000
Deleting CSCO   30.040000
Deleting EBAY   30.640000
Deleting FB     127.850000
Deleting GOOG   804.460000
Deleting INTC   23.940000
Deleting MSFT   32.600000
Deleting NFLX   139.390000
Deleting ORCL   32.780000
Deleting ZNGA   2.640000
Deleting 
No Memory Leak
Press any key to continue . . .

*/