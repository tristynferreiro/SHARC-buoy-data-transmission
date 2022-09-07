// code.h                                         Stan Eisenstat (09/23/08)
//
// Interface to putBits/getBits

#include <limits.h>

// Write code (#bits = nBits) to standard output.
// [Since bits are written as CHAR_BIT-bit characters, any extra bits are
//  saved, so that final call must be followed by call to flushBits().]
void putBits (int nBits, int code);

// Flush any extra bits to standard output
void flushBits (void);

// Return next code (#bits = nBits) from standard input (EOF on end-of-file)
int getBits (int nBits);