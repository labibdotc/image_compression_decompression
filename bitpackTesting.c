#include <stdio.h>
#include <stdlib.h>
#include "bitpack.h"


//void bit (int64_t word);

int main() {
    //loading unsigns to make sure pow works with unsigneds in bitpack
    unsigned n1 = 7;
    unsigned n2 = 8;
    unsigned n3 = 2;
    unsigned n4= 0;
    unsigned w1 = 3;
    unsigned w2 = 1;
    unsigned w3 = 0;

    //some edgecases for fitsu 8 when 7 is the bigest n that can return true
    //...etc.
    Bitpack_fitsu(n1, w1) ? printf("%d can be fit in %d bits\n", n1, 3) :
                          printf("%d can't be fit in %d bits\n", n1, 3);
    Bitpack_fitsu(n2, w1) ? printf("%d can be fit in %d bits\n", n2, 3) :
                          printf("%d can't be fit in %d bits\n", n2, 3);
    Bitpack_fitsu(n3, w2) ? printf("%d can be fit in %d bits\n", n3, 1) :
                          printf("%d can't be fit in %d bits\n", n3, 1);
    
    Bitpack_fitsu(n4, w3) ? printf("%d can be fit in %d bits\n", n4, 0) :
                          printf("%d can't be fit in %d bits\n", n4, 0);

    Bitpack_fitsu(1, 1) ? printf("1 can be fit in 1 bits\n") :
                          printf("1 can't be fit in 1 bits\n");

    

    //some edgecases for fitss -4 when it is 3 bits, 2 bits, -5 when it is 
    //3 bits, 4bits. 4 when it is 3 bits, 4bits too. -1 and 1 when it is 1 bit 
    //etc.
    int s1 = -4;
    w1 = 3;
    w2 = 2;
    Bitpack_fitss(s1, w1) ? printf("%d can be fit in %d bits\n", s1, w1) :
                          printf("%d can't be fit in %d bits\n", s1, w1);
    Bitpack_fitss(s1, w2) ? printf("%d can be fit in %d bits\n", s1, w2) :
                          printf("%d can't be fit in %d bits\n", s1, w2); 
    s1 = -5;
    w2= 3;
    Bitpack_fitss(s1, w2) ? printf("%d can be fit in %d bits\n", s1, w2) :
                          printf("%d can't be fit in %d bits\n", s1, w2);
    s1 = -5;
    w2= 4;
    Bitpack_fitss(s1, w2) ? printf("%d can be fit in %d bits\n", s1, w2) :
                          printf("%d can't be fit in %d bits\n", s1, w2);
    s1 = 4;
    w2= 3;
    Bitpack_fitss(s1, w2) ? printf("%d can be fit in %d bits\n", s1, w2) :
                          printf("%d can't be fit in %d bits\n", s1, w2);
    s1 = 4;
    w2= 4;
    Bitpack_fitss(s1, w2) ? printf("%d can be fit in %d bits\n", s1, w2) :
                          printf("%d can't be fit in %d bits\n", s1, w2);
    s1 = -1;
    w2= 1;
    Bitpack_fitss(s1, w2) ? printf("%d can be fit in %d bits\n", s1, w2) :
                          printf("%d can't be fit in %d bits\n", s1, w2);
    s1 = 0;
    w2= 1;
    Bitpack_fitss(s1, w2) ? printf("%d can be fit in %d bits\n", s1, w2) :
                          printf("%d can't be fit in %d bits\n", s1, w2);
    s1 = 1;
    w2= 1;
    Bitpack_fitss(s1, w2) ? printf("%d can be fit in %d bits\n", s1, w2) :
                          printf("%d can't be fit in %d bits\n", s1, w2);
    s1 = 1;
    w2= 2;
    Bitpack_fitss(s1, w2) ? printf("%d can be fit in %d bits\n", s1, w2) :
                          printf("%d can't be fit in %d bits\n", s1, w2);

    Bitpack_fitss(-10, 5) ?  printf("-10 can be fit in 5 bits\n") :
                          printf("-10 can't be fit in 5 bits\n");




    /*
        Testing that inside bitpack.c
        //printf("mask in line 41 looks like: %lu\n", mask);
        //printf("mask on line 43 looks like: %lu\n", mask);
        //printf("mask on line 45 looks like: %lu\n", mask);
        //printf("%lu\n",(word & mask) >> lsb);








    */
    
    (Bitpack_getu(0x3f4, 6, 2)==61) ? printf("getu success!\n") : 
                                    printf("getu fail!\n");
    

    //word: 1111001110110100110010, lsb = 3 width = 10 this should return 422
    //for both gets and getu
    (Bitpack_getu(3992882, 10, 3)==422) ? printf("getu success!\n") : 
                                    printf("getu fail!\n");
    (Bitpack_gets(3992882, 10, 3)==422) ? printf("gets success!\n") : 
                                    printf("gets fail!\n");
    //1111110100: 111101 is a -3 
    (Bitpack_gets(0x3f4, 6, 2)==-3) ? printf("gets success!\n") : 
                printf("gets fail with %ld!\n", Bitpack_gets(0x3f4, 6, 2));
    //111101
    (Bitpack_getu(0x3f4, 6, 2)==61) ? printf("getu success!\n") : 
                                    printf("getu fail!\n");

    (Bitpack_gets(0x1D5557FAB, 16, 0)==32683) ? printf("gets success!\n") : 
            printf("gets fail with %ld!\n", Bitpack_gets(0x1D5557FAB, 6, 2));
    (Bitpack_getu(0x1D5557FAB, 16, 0)==32683) ? printf("getu success!\n") : 
            printf("getu fail with %ld!\n", Bitpack_getu(0x1D5557FAB, 6, 2));

    Bitpack_gets(0x1D5557FAB, 17, 0) == -32853 ? printf("gets success!\n") : 
            printf("gets fail with %ld!\n", Bitpack_gets(0x1D5557FAB, 17, 0));

    Bitpack_getu(0x1D5557FAB, 17, 0) == 98219 ? printf("getu success!\n") : 
            printf("getu fail with %ld!\n", Bitpack_getu(0x1D5557FAB, 17, 0));
    //10111111110101011
    // Bitpack_gets(,16 ,0)
    Bitpack_newu(0, 4, 1, 10);
    (Bitpack_newu(25, 2, 1, 3) == 31) ? printf("newu success!\n") :
                                        printf("newu fail!\n");
                                        // 0111111110101011
    (Bitpack_newu(0xFAAF, 8, 3, 0) == 63495) ? printf("newu success!\n") :
                                        printf("newu fail!\n");

    (Bitpack_newu(0xFAAF, 8, 3, 29) == 63727) ? printf("newu success!\n") :
                                        printf("newu fail!\n");
    
                //1111111111000111
                //1111100011101111 adding -8 at position 3 in width 8
    Bitpack_news(0xF8EF, 8, 3, -8) == 65479 ? printf("news success!\n") : 
    printf("news fails! with value %lu\n",Bitpack_news(0xF8EF, 8, 3, -8));

    Bitpack_gets(65479, 8, 3) == -8 ? printf("gets success!\n") : 
        printf("gets fail!\n");
    // Bitpack_news(0xF8EF, 8, 3, -8) == -1809 ? printf("news success!\n") : 
    //                                     printf("news fails!\n");
    // fprintf(stderr, "1\n");

    // (Bitpack_news(0xAAE0B, 5, 1, -10) == 700267) ? printf("news success!\n") :
    //                                             printf("news fail with %lu\n", Bitpack_news(0xAAE0B, 5, 1, -10));
    return 0;
}


/*
    Purpose: prints out any 64 bit in binary
    Parameters: 64 bit word
    Return: none
    effect: prints to test functions
*/
// void bit (int64_t word) {
//     uint64_t i;
//     for (i = 1 << 32; i > 0; i = i / 2) {
//         (i & word) ? printf("1") : printf("0");
//     }
// }


//should we get a 10 instead of -10 