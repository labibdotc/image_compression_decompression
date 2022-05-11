#include "getword.h" 
Except_T FileError = {"An error was encountered reading in compressed file."};


/* Purpose: DCT to word (four pixels per call)
*  Parameters: int i and j: coordinates: col and row respectively
*  A2Methods_UArray2 array2: has the array of DCT 
*  A2Methods_Object *elem: pointer to element at index i and j
*  void * cl: closure that has nothing
*  Effect: get DCT onto disk in form of 32 bit words 
*  Return: none
*/
extern void DCTtoWordMap(int i, int j, A2Methods_UArray2 array2, 
                             A2Methods_Object *elem, void *cl) {
            //A2Methods_T methods = cl;
            (void)cl; 
            (void)i;
            (void)j;
            (void)array2;
            struct fourPix *DCT = elem;
            //pass these to bitpacking get them smaller
            //32 bits to store the 5 bits
            uint32_t word = 0;

            int64_t b = quantize(DCT->b); //5bits signed
            assert(Bitpack_fitss(b, 5) == true);

            int64_t c = quantize(DCT->c); //5bits signed
            assert(Bitpack_fitss(c, 5) == true);

            int64_t d = quantize(DCT->d);//5bits signed
            assert(Bitpack_fitss(d, 5) == true);

            uint64_t a = (unsigned)round(511*DCT->a); //9 bits unsigned
            assert(Bitpack_fitsu(a, 9) == true);

            uint64_t pb = Arith40_index_of_chroma(DCT->pb); //4bits unsigned
            uint64_t pr = Arith40_index_of_chroma(DCT->pr); //4bits unsigned
            ////4bits unsigned Arith40_index_of_chroma
            
            word = Bitpack_newu(word, 4, 0, pr);
            word = Bitpack_newu(word, 4, 4, pb);
            word = Bitpack_news(word, 5, 8, d);
            word = Bitpack_news(word, 5, 13, c);
            word = Bitpack_news(word, 5, 18, b);
            word = Bitpack_newu(word, 9, 23, a);
            if(i == 1 && j == 0) {
                fprintf(stderr, "%u\n", word);
            }
            writeAword(word);
            
}


/*
    Compression last step
    Purpose: write to stdout a 32 bit word per call
    Parameters: a 32 bit word
    Return: none
*/
void writeAword(int32_t word) {
    for (int i = 3; i >= 0; i--) {
        putchar(Bitpack_getu(word, 8, 8*i));
    }
}



/*
    Purpose: quantize floating points of value to an int in the int range
    Parameters: float value: value to be quantized
    Illustration:
        Ranges: -15->15
        {-.3, -.28, -.26, -.24, -.22, -.2, -.1, 0}. *50 we can cast
        {-15,  -14,   -13,  -12, -11   -10, -5, 0}
    Return: quantized int value
*/
int quantize(float value) {
    if(value > 0.3){
        value = 0.3;
    } 
    if(value < -0.3) {
        value = -0.3;
    }
    return (int) (round(value*50));
}


/*
    Purpose: read in image in binary words and put them onto DCT objects
    Parameters: A2Methods_T methods: methods
    FILE* in: pointer to where binary words to be read
    Return: array2 of DCT of whole image
*/
A2Methods_UArray2 compressedFiletoDCT(FILE* in, A2Methods_T methods) {
    //getting header of file 

    unsigned height, width;
    int read = fscanf(in, "COMP40 Compressed image format 2\n%u %u", &width, &height); assert(read == 2);
    int c = getc(in);

    assert(c == '\n');

    assert(width %2 == 0 && height % 2 == 0);
    //create a new array holding DCT, which should have width and height be half of what the input says
    A2Methods_UArray2 fourPixArr = methods->new( ( width) / 2, 
        ( height) / 2, sizeof( struct fourPix) );

    methods -> map_row_major(fourPixArr, compressedFiletoDCTMap, in);

    return fourPixArr;
}



/*
    Purpose: read in one word of binary at a time 
    Parameters: A2Methods_T methods: methods
    int i and j: coordinates
    closure has FILE* in which is pointer to where binary words to be read
    array2: For the consistency of apply function contracts, we void it
    Return: none.
*/
void compressedFiletoDCTMap(int i, int j, A2Methods_UArray2 array2, 
                             A2Methods_Object *elem, void *cl) {


        (void)i;
        (void)j;
        (void)array2;
        FILE* in = cl;
        //A2Methods_T methods = uarray2_methods_plain; 
        uint32_t currword = 0;
        unsigned currchar;
        for(int x = 3; x >=0 ; x-- ) {
            currchar = getc(in);
            currword = Bitpack_newu(currword, 8, 0+(8*x), currchar );
            if(ferror(in) != 0 || (int)currchar == EOF) {
                RAISE(FileError);
            }
        }
        if(i == 1 && j == 0) {
            fprintf(stderr, "%u\n", currword);
        }
        //we put 00000100 10000000 00000000 10001000 
        //we get 111100000000      00000000        01110111
         
        //unpack currword into a,b,c,d, pb, pr
        //be really careful about the word not being garbage

        //put all that info at the new array, at i,j
        struct fourPix* currfourPix = elem;

        currfourPix -> a = (( (float)(Bitpack_getu(currword, 9, 23) ) )/511.0 );
        currfourPix -> b = (( (float)(Bitpack_gets(currword, 5, 18) ) )/50 );
        currfourPix -> c = (( (float)(Bitpack_gets(currword, 5, 13) ) )/50 );
        currfourPix -> d = (( (float)(Bitpack_gets(currword, 5, 8) ) )/50 );

        currfourPix -> pb = Arith40_chroma_of_index(Bitpack_getu(currword, 4, 4));
        currfourPix -> pr = Arith40_chroma_of_index(Bitpack_getu(currword, 4, 0));

    
}