# Image compression and decompression algorithm
Algorithm compresses image by taking diagonal averages of pixels and storing every four pixels in one pixel which reduces size significantly. Most images tested don't lose more than 2%. Decompression algorithm implemented too. Have fun trying it with the display in a ssh -X. Make sure you install X-quartz :)
<img width="1190" alt="Screenshot 2023-01-28 at 7 22 30 PM" src="https://user-images.githubusercontent.com/98433990/215297248-0eb0e645-4ca8-48cd-9cab-403856ce8ae2.png">
<img width="1198" alt="Screenshot 2023-01-28 at 7 22 18 PM" src="https://user-images.githubusercontent.com/98433990/215297251-1a7fb41a-0bff-4f17-a782-0712a9803327.png">
<img width="1206" alt="Screenshot 2023-01-28 at 7 22 12 PM" src="https://user-images.githubusercontent.com/98433990/215297253-7fd503e6-4926-49ee-ad88-80ba707171f6.png">

# Plan
## Step 1: 12 bytes to RGB integers:
- input: image file
- output: array of rgb
- no data loss

## Step 2: RGB integers to floats:
- input: RGB integers in form of array of structs
- output: RGB floats i.e. each integer's ratio to maximum value of rgb in the picture
- no data loss

## Step 3: RGB floats to color space
- input: RGB floats
- output: color space structs Y, Pb, Pr i.e. in floating point form
- Formula for output: y = 0.299 * r + 0.587 * g + 0.114 * b;
pb = -0.168736 * r - 0.331264 * g + 0.5 * b; pr= 0.5 *r-0.418688*g-0.081312*b;
- data loss: float x float produces trailing decimals that would eventually get lost after the floating 7 point place.

## Step 4: color space to DCT space
- input: color space structs
- output: a struct containing Pb, Pr, a, b, c, and d -> 1/4 the amount of the color space structs
- Formula for the output a=(Y4 +Y3 +Y2 +Y1)/4.0; b = (Y4 + Y3 − Y2 − Y1)/4.0; c = (Y4 − Y3 + Y2 − Y1)/4.0; d = (Y4 −Y3 −Y2 +Y1)/4.0;
- data loss: averaging out every 4 pixels into 1 would yield in minor data loss

## Step 5: b, c and d to scaled ints
- input: b, c, and d in float from
- output: scaled ints in 5-bit scaled ints
- data loss: data is lost as values are truncated at 0.3 and -0.3


## Step 6: load bits into words
- input: size reduced uarray with 32-bit representation (9-a, 5-b, 5-c, 5-d, 8-pb and 8-pr) for every 4 pixels
- output: one 32 bit word that has all the data for a given 4-pixel
- data loss: no data loss

## Step 7: write to disk compressed image


# Architecture
Decompression happens by reversing every step in the same file it was compressed.
