/* LZW - encode and decode : a compression and decompression program that uses the LZW algorithm
 by Jared Katzman (9/16/14)

 encode() reads a stream of characters from the standard input, compresses it
 using the Lempel-Ziv-Welch algorithm, and writes the stream of codes to the
 standard output as a stream of bits packed into 8-bit bytes. 
 
 decode() reads from the standard input a byte stream written by encode, 
 decompresses the stream of codes, and writes the stream of characters 
 to the standard output.

 Both methods use the same data structure of a Trie hashed by CODE to a dynamically sized array

*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include "StringTable.h"
#include "code.h"
//Print essage to stderr and exit
#define DIE(msg)			exit( fprintf(stderr, "%s\n", msg))
#define ORDIE(cond)			if(cond) ; else DIE("Arguments specified incorrectly")
#define MAX(a,b)            ((a) > (b)) ? (a) : (b)
#define ungetchar(c)        ungetc ((c), stdin)

// == Access to public variables of StringTable ==
extern int MAXBITS;                     // Maximum bits
extern int PRUNE;                       // State of prune flag
extern int ESCAPE;                      // State of escape flag`

// encode() reads input from stdin and outputs MAXBITS:PRUNE:ESCAPE:<<compressed_data>>
void encode (void);
// decode() reads input from stdin in the form MAXBITS:PRUNE:ESCAPE:<<compressed_data>>
void decode (void);

// Debug Flag - refers to Stage Implementation written in Homework 2 spec
int MODE = 3;

int main(int argc, char *argv[])
{
	if(!strcmp("encode", basename(argv[0]))) {          // Run Encode on input
        for(int i = 1; i < argc; i++) {
			char *temp;
			if(!strcmp ("-m", argv[i])) {                // -m : MaxBits 
				MAXBITS = strtol (argv[++i], &temp, 10);
				ORDIE (MAXBITS >= 0);
				if (MAXBITS <= CHAR_BIT || MAXBITS > 3*CHAR_BIT-4)
					MAXBITS = 12;
			}
			else if (!strcmp("-e", argv[i]))             // -e : Initialize Table
				ESCAPE = true;
			else if (!strcmp("-p", argv[i]))             // -p : Prune Flag
				PRUNE = true;
			else
				DIE("encode: arguments specified incorrectly");
		}
        printf("%d:%d:%d:",MAXBITS,PRUNE,ESCAPE);       // Prune Flags :
                                                        // MAXBITS:PRUNE:ESCAPE
        encode();
        flushBits();
        destroyTable();                                  // Run Clean-up
	}
	else if(!strcmp ("decode", basename(argv[0]))) {     // Run Decode on input
		ORDIE (argc == 1);
        
        char k;                                         // Flag extraction
        int *flags[3] = {&MAXBITS, &PRUNE, &ESCAPE}, temp;
        for (int i = 0; i < 3; i++) {
            temp = 0;                                   // Calculate # until :
            while (true) {
                if(isdigit ((k = getchar()))) {
                    temp = temp * 10;
                    temp = temp + k - '0';
                }
                else if (k == ':') break;
                else        DIE("decode: invalid input");
            }
            *flags[i] = temp;
        }
        ORDIE (MAXBITS > CHAR_BIT && MAXBITS <= 3*CHAR_BIT-4);
        ORDIE (PRUNE == 1 || PRUNE == 0);
        ORDIE (ESCAPE == 1 || ESCAPE == 0);

        decode();                           
        destroyTable();                                 // Clean-up
	}
	return 0;
}

void encode (void) {
    
    int C = EMPTY, newC;
    int K;
    
    initializeTable (ESCAPE);                   // MAXBITS = 2^MAXBITS max table size
                                                // PRUNE = (if -p flag present in encode)
    while ((K = fgetc(stdin)) != EOF) {           // ESCAPE = (if -e flag present in encode)
        unsigned char k = K;                        

        if ((newC = find (C, k)) != -1)         // if (C,k) is in table 
            C = newC;
        else {
            if (C == EMPTY) {                   // -e flag: K not initialized
                output(ESC,0);
                output((int)k,8);               // Replace with putbits(8,char)?;
                insert (C, k, true);            // Does it need the 3rd argument?
                C = EMPTY;
                continue;
            }
            output(C,0);
            insert (C, k, true);
            C = find (EMPTY, k);
            if (C == -1) {                      // -e flag: k is uninitialized
                ungetchar (k);                  // unget K and re-start loop
                C = EMPTY;
            }
        }
    }

    if (C != EMPTY)                             // Flush Codes
        output(C,0);
    output(END,0);                             // End Flag
}

void decode (void) {

    int oldC = EMPTY, C;
    unsigned char finalK;

    initializeTable(ESCAPE);

    while ((C = getcode(0)) != END) {            
        if (C == ESC) {                                 // -e: uninitalized code flag
            ORDIE (ESCAPE);
            finalK = (unsigned char) getcode(8);        //      Value K
            if (oldC != EMPTY)                           
                insert (oldC, finalK, true);            // insert (oldC,k)

            insert (EMPTY, finalK, true);               // insert (EMPTY, k)
            putchar (finalK);
            oldC = EMPTY;
            continue;
        }
        if (findCode (C))                               // if C in table - rewrite finalK
            finalK = findK (C, false);                  // else: KwKwK
                                                        // assume finalK is previous code firstK
        if (oldC != EMPTY)
           insert (oldC, finalK, true);
        
        oldC = codeToChar (C, 1);                       // output(C) and check if 
    }                                                   // LOOK_AHEAD additions cause a prune
}

