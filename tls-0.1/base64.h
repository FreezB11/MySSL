#ifndef BASE64_H
#define BASE64_H

/*

    the base64 divides the input into 
    6-bit chunks { 2^6 = 64 }
    64 -> 52 (a-z) + (A-Z)
       -> 10 (0-9)
       -> 2(left to map) arbitarily (+,/)

    input is a multiple of 8bit 
    24 is the least common multiple of 6 & 8

    so the input has to be padded in multiple of 24(3bytes)

*/

void base64_encode( const unsigned char *input, int len, unsigned char *output );
int base64_decode( const unsigned char *input, int len, unsigned char *output );

#endif
