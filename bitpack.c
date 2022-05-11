#include <stdio.h>
#include <stdlib.h>
#include "bitpack.h"
#include "assert.h" 

Except_T Bitpack_Overflow = { "Overflow packing bits" };
/*
    Purpose: checks if a given uint can fit in a given bits length
    Parameters: uint n: number to check, unsigned width: length of bits
    Returns: true if it fits, false otherwise
    Cases: width = 1 can fit n = 0, 1. w = 2, can fit n = 0, 1, 2, 3. 
     w = 1 returns true with 0 and 1
     w= 2 returns true with 0, 1, 2 and 3
     w= 3 returns true with 0, 1, 2, 3, 4, 5, 6, 7


*/
bool Bitpack_fitsu(uint64_t n, unsigned width) {
    uint64_t maxFit = 1 << width; //2powerWidth 
    //printf("current n is %lu maxwidth is %lu and width is %u \n", n, maxFit, width);
    return (width != 0 && n < maxFit);
}
/*
    Purpose: checks if a given int can fit in a given bits length
    Parameters: int n: number to check, unsigned width: length of bits
    Returns: true if it fits, false otherwise
    Cases: 
     w = 1 returns true with -1, 0
     w= 2 returns true with -2, -1, 0, 1
     w= 3 returns true with -4, -3, -2, -1, 0, 1, 2, 3
    -10 should fit in 5 but not 4 
    -10 is 1111111111110110
*/
bool Bitpack_fitss(int64_t n, unsigned width) {
    //2 to the width is like 2 << width -1 which is 
    int64_t maxFit = 1 << (width) >> 1; //that's 2 power width / 2
    return (width != 0 && n >= -1*maxFit && n < maxFit);
}

/*
    Purpose: extracts a specific part of bits from a word
    Parameters: width: how many bits to extract, lsb: signifies where
                word: is the word to be worked on
    Returns: extracted value
*/
//first question: why we need "-1"
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb) {
    assert(width <= 64);
    assert(width+lsb <= 64);
    uint64_t mask= 0x7FFFFFFFFFFFFFFF; //64 bits of 1's
    //printf("%lu\n", mask);
    mask = mask >> (64 - width - 1); //gets "width" bits on the left side
    //printf("mask for getu is %lu\n", mask);
    mask = mask << lsb; //gets "width" bits on the left side
    return (word & mask) >> lsb;
}
/*
* 
*
*
*/
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb) {
    assert(width <= 64);
    assert(width+lsb <= 64);
    //propogates with leading 1's for -ve signed ints and 0's for 
    //using c shifting
    int64_t signedResult = Bitpack_getu(word, width, lsb);
    signedResult = signedResult << (64 - width) >> (64 - width);
    //printf("%lu is a %ld in signed and makes signed gets result %ld\n", unsignedResult, sign, signedResult);
    return signedResult;
    //Bitpack_getu(word, width, lsb) - (uint64_t)pow(2, width + 1)/2;

    // uint64_t check = Bitpack_getu(word, width, lsb);
    
    
    
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                   uint64_t value) {
    // (void)word; (void)width; (void)lsb;
    // (void)value;
    assert(width <= 64);
    assert(width + lsb <= 64);
    if(Bitpack_fitsu(value, width) == false) {
        RAISE(Bitpack_Overflow);
    }
    
    //and word has zeros where the insertion will happen
    uint64_t mask = 0xFFFFFFFFFFFFFFFF;
    mask = mask << width; 
    
    for(unsigned i = 0; i < lsb; i++) {
        mask = mask << 1;
        mask ++; 
        //pushes 1 to the right of the insertion-equivalent field in mask
    }
    //printf("mask for %lu in newu will be %lu\n", value, mask);
    //word with zero bits where insertion will happen
    uint64_t word_mod = word & mask; 

    //get word by & with a mask that has zeros only in the insertion field
    //one can XOR
    //printf("%lu", word_mod | value);
    
    //if value is alligned
    value = value << lsb;
    return word_mod | value;
}
//11111111110110


//1111111110000011 
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  
                    int64_t value) {
    assert(width <= 64);
    assert(width + lsb <= 64);
    //printf("%lu %ld\n",(uint64_t)value, value);
    //fprintf(stderr, "value: %ld width: %u\n", value, width);
    if(Bitpack_fitss(value, width) == false) {
        RAISE(Bitpack_Overflow);
    }

    uint64_t uvalue = value;
    uvalue = uvalue << (64 - width) >> (64 - width); //gets rid of unneccessary 0's
    //printf("unsigned is %lu and signed is %ld\n", uvalue, value);

    return Bitpack_newu(word, width, lsb, uvalue); 
}
//000110 lsb 1, width 3 -> 011
//000110 
//000110 << 2, 011000 >> 000011



//-10
//(11111111)10110

//word: 10101010111101101011
//insert -10: 10110 (lsb = 4) width = 5
//000000000010110000

//signed 64-width
//width 

//100111 news 10(bin: 1010)
//should be 101010

//in signed int 110110 is equivalent to a 10110


// we get:  10101010111000101101
// we want: 10101010111101101011