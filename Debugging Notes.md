# Debugging

## Intro
* BG Mode 1
* BG0 is light rays
  * Map base:  0x0600E800
  * Char base: 0x06000000
  * Flickers a bit by flipping between two backgrounds (?)
  * 16 colors
  * Priority 2
* BG1 is forest scene
  * Map base:  0x0600F000
  * Char base: 0x06000000
  * 16 colors
  * Priority 3
* BG2 is sword
  * Map base:  0x0600E000
  * Char base: 0x06008000
  * 256 colors
  * Priority 1
* No BG3

* sprite0 is copyright (R)
  * Tile 640
* sprite1 is sword hilt
* sprite2 is "The"
  * Tile 560
* sprite3 is "Mini"
  * Tile 576
* sprite4 is "ish"
  * Tile 592
* sprite5 is "Cap"
  * Tile 608
* sprite6 is ezlo silhouette
  * Tile 624
* sprite7 is "Z"
* sprite8 is bottom of "Z"
* sprite9 is bottom of "Z"
* sprite10 is "THE LEGEN" and "ELD"
* sprite11 is "D OF" and top of "A"
* sprite12 is bottom half of "A"
* sprite13 is bottom serif of "A"

Then all the sprites shift up by 7
* sprite0 is "(C)200"
  * Tile 32
* sprite1 is "4,200"
  * 40
* sprite2 is "5 Nin"
  * 48
* sprite3 is "tendo"
  * 56
* sprite4 is "PRE"
  * 0
* sprite5 is "SS ST"
  * 8
* sprite6 is "ART"
  * 16

* when PRESS START flashes those sprites disappear for some reason
* Counter at 0x02000088 counts down to trigger the end of the splash screen and show the Capcom logo

* Logic:
  *



## Save Select

