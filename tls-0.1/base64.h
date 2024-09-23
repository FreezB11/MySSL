#ifndef BASE64_H
#define BASE64_H

/*

    the base64 divides the input into 
    6-bit chunks { 2^6 = 64 }
    64 -> 52 (a-z) + (A-Z)
       -> 10 (0-9)
       -> 2(left to map) arbitarily (+,/)

*/

void base64_encode( const unsigned char *input, int len, unsigned char *output );
int base64_decode( const unsigned char *input, int len, unsigned char *output );

#endif
