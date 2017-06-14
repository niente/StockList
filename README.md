# StockList
This program reads a text file of stocks and quotes to create a circularly  doubly-linked list of stacks (in alphabetical order). The list is printed to screen, forwards and backwards. The user can search for two stocks in the list, and print the sub-list between them. The user can search for a stock and input the number of quotes to be read from its stack.

Sample Output:
```
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
```
