PA1: Partial Tokenizer
Updated 25 seconds ago
A partial tokenizer that takes in a stream and returns the tokenized equivalent of the different numbers found within.

These are grouped into the following: Decimal Hexadecimal Octal Float MAL

GRADE: 100/100

If you are currently taking CS211 and are trying to get help with this assignment, please consider the following:

1) There are many methods of solving this assignment. 2) My method is likely not the shortest, fastest, most efficient, or best way to do it. 3) Reading my code WILL throw off your thought process, and will crutch you for the rest of the assignment. 4) If you directly copy my code you will get caught and you will get a zero. 5) If you use strtok, you are a coward and don't deserve to gaze upon my code, please exit immediately.

Musings on the assignment: This was a really intersting first assignment, juding by the grades in the class, it seems to be one that really "separates the men from the boys" so to speak. One of the hardest parts of this was constantly finding new test cases to work with, and having to go back and rework parts of the code. Despite this, I'm pretty sure I have every case down pat and ready to go. For my first program in C (beyond a HelloWorld), I have to say, I'm pretty proud of what I have here. There are likely ways I can trim the code down, or make it more efficient, however it serves its purpose well, and the logic is as good as it can get.



OFFICIAL README






Tokenizer.c is a program that takes in a stream of assorted characters and groups/separates them into tokens, based on the type of number they are. 

This program is able to differentiate between Decimal numbers, octal numbers, floats, hexadecimal numbers, escape characters, and whitespace. 

My program runs chiefly in two main parts, TKGetNextToken and the main.

TKGetNextToken is a function that serves to eliminate white space from the 
stream, as well as separate strings tokens by other delimiters, such as "\n", "hello", or "%". In order to do this, TKGetNextToken runs a while loop through the stream, and depending on the current character, either A, terminates the loop and returns a token, or B continues on to the next character. Certain 
scenarios in the stream require specific cases to occur, which leads to
two extremely complex if statements that make the majority of comparisons in the function. These serve as the delimiters for every character, sans Hex, E, ., +, and -. In the end, TKGetNextToken will either return a single escape character, or a full numerical token ready to be checked.

This token is them brought into the main where the finite state machine is located. This 9 case switch statement is located within a loop and serves to
categorize the token into its given type, while also having a case for a 
malformed token that TKGetNextToken was not able to detect.

For instance, case -2 is capable of seeing that "\n" is its own character and submitting the single hex value for it, rather than the hex value of '\' and 'n'.

Octal, Decimal, and Hex all contain their own cases, while Float requires 3.
This is due to the number of different permutations that floats can contain.

Each case responds to a different type of float, namely floats with just a .
i.e. [0.1]
floats with a decimal and an order of magnitude
i.e. [0.1e1]
and floats with a decimal, an order of magnitude, and a direction
i.e. [0.1e+1]

Because of how TKGetNextToken and the main interact, there are a few cases
that may seem out of the ordinary.

Take 079
This will pass through TKGetNextToken, because 0,7, and 9 individually are
valid digits in some arbitrary token. While 079 as a whole is NOT a valid octal token, and is returned [MAL 079], since its individual parts CAN be used in a valid token, 079 is not delimited into [Octal 07][Decimal 9].

Conversely, take 0x0g 
This will NOT pass through TKGetNextToken whole, because G, is not a valid character in any for of token. Instead TkGetNextToken will take the 0x0 and return it as [Hexadecimal 0x0] and separately display the [MAL 0x67].

As far as my code is concerned, a TOKEN is defined as a group of characters that contains ONLY 0-9,A-F,a-f,x,X,+,-, and .
Anything else is declared invalid and delimited.



//Question for Grader
//Line 47 returns "stream", a pointer. For some reason, the actual
//value for "stream" changes when it gets returned, meaning I have to use
//the void pointer in order to actually free up the memory mallocced by
//stream. Why is the value of stream on line 47 and on line 155 different?

