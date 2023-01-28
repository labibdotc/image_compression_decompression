# image_compression_decompression
Algorithm compresses image by taking diagonal averages of pixels and storing every four pixels in one pixel which reduces size significantly. Most images tested don't lose more than 2%. Decompression algorithm implemented too. Have fun trying it with the display in a ssh -X. Make sure you install X-quartz :)


# Planout
//Module0: I/O
//coming in: a stream to read from in the beginning of our flow or output to at the end of the flow //effect: a uarray2 of pnmppm data or a print to the output stream at the end of the flow. //module shared responsibility: I/O
//Reading in and printing out will take place in this module not in module 1 or 4. //Module#1: get the pnmppm into an RGB float
//Coming in: a file to read
//Goint out: a uarray2 of rgb ratio floats
//determining secret-keeping: denominator
Step 1: 12 bytes to RGB integers:
● Input: input  le
● Output: array2 of Pnm_rgbs
● Information status: no data lost yet
a) Readincommandsusingreadingalgorithmfromlastassignment
Step 2: RGB integers to  oats:
○ Input: RGB int members from struct in uarray2 (pnm_rgb)
○ Output: RGB int members from structs casted as  oats for step 3
○ Information status: no data lost yet
b) Get these RGB ints into their respective ratio  oats by dividing by their maxvals
//Module#2: get the RGB ratio floats into color space //Coming in: RGB ratio float uarray2
//Goint out: a uarray2 of pixels’ color space
//determining secret-keeping: rgb->color space conversions
Step 3: RGB  oats to color space:
○ Input: RGB int members from struct casted as  oats from step 2
○ Output: color space structs in a uarray2, of members Y, Pb, Pr i.e. in  oating
point form
○ Information status: multiplying  oats by  oats will make us lose some data, for
the trailing digits beyond 7th-decimal digit
c) Getthese oatstovideocolorspaceusingequations-referencedbelow
   
 //Module#3: get the colorspace values into DCT space
//Coming in: a uarray2 of pixels’ color space
//Goint out: a size-reduced uarray2 of DCT space structs
//determining secret-keeping: color-space->DCT space conversions and quantization
Step4: From color space of 4 pixels to Pb Pr a b c d (DCT space):
○ Input: 4 color space struct objects for the 4 pixels
○ Output: a struct containing the Pb Pr a b c d in a uarray2 of 1⁄4 size of these in
steps 1,2 and 3
○ Information status: small amount of data lost counting the averages
d) Get the RGB pixels organized in 2*2 blocks.
e) Gothrough4pixelsinablockandgettheirYPbPrvalues
f) Getaverageof4Pb’s-Pb
g) Getaverageof4Pr’s-Pr
h) Convert Pb and Pr (which at this point represent data of 4 pixels altogether)
into 4-bit Pb and Pr using unsigned Arith40_index_of_chroma(float x);
i) Right-most4bitsofreturnedPbandPrvariablewillbepassedontoour
word structure.
j) Usingy-valuesof4pixels,wewillgetto4values:a=(Y4+Y3+Y2+Y1)/4.0
b = (Y4 + Y3 − Y2 − Y1)/4.0 c = (Y4 − Y3 + Y2 − Y1)/4.0 d = (Y4 −Y3 −Y2 +Y1)/4.0
Step5: Convert b c d to scaled ints :
○ Input: b, c and d in  oats from within the size-reduced uarray2
○ Output: b, c and d in 5-bit scaled ints
○ Information status: more data is lost here as quantization for values over 0.3 or
less than -.3 are lost for b, c and d
k) Change b, c and d to 5 bits each (assuming they lie bet. signed -0.3 and 0.3). a will be 9 bits though.
  
//Module#4: get the (memory-efficient) DCT space out in as compressed output //Coming in: a uarray2 of pixels’ DCT in 4-bits
//Goint out: stream of compressed pictures
//determining secret-keeping: packing bits onto the word
Step6: load bits onto the word :
○ Input: size-reduced uarray2 of (32-bit) elements
○ Output: a uarray2 of words
○ Information status: no more data lost at this step
l) Load a, b, c and d to the 32-bit word. That way our word has all 6 values representing the (4 pixels) block. This representation will is what made us disregard end columns/rows of odd columns/rows. We just trim it. If we do trim, width and height values are updated accordingly.
Step7: To 4-bytes on disk :
○ Input: a uarray2 of words
○ Output: words onto stream
○ Informationstatus:nomoredatalostatthisstep
m) We printf the title printf("COMP40 Compressed image format 2\n%u %u", width, height);
n) And then print out a sequence of 32-bit words one at a time. Note: Width
and height are for decompressed photos (after updates of checking for odd).
o) Determine which byte of the 32-bit (4-byte) word is most signi cant. Print
it out using putchar(). Code should be in row-major order. So  rst, block containing (0,0) and then (2,0).
