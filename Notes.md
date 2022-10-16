Notes

* GBA Architecture: https://www.copetti.org/writings/consoles/game-boy-advance/

  * Uses mix of 32 and 16-bit ARM/Thumb
  * 32-bit ALU/CPU
  * 32KB IWRAM (internal working ram)
  * 256 KB EWRAM (external working ram)
  * 96KB VRAM
  * Game paks are 16-bit ROM and 8-bit RAM

* Graphics
  * 15-bit colors
  * 240x160 pixels
  * 96K VRAM (64K background, 32K sprites)
  * 1K OAM (Object Attribute Memory) - stores up to 128 sprite entries (not the graphics, just the indices and attributes)
  * 1K PAL RAM (Palette RAM): Stores two palettes, one for backgrounds and one for sprites. Each palette contains 256 entries of 15-bit colors, with color 0 being transparent
  * Tiles are 8x8 pixel bitmaps.
  * Can use 16 colors (4bpp) or 256 colors (8bpp). 4bpp tiles use 32 bytes
  * PPU wants tiles stored in charblocks (region of 16k)
  * PPU can draw up to four background layers in different modes
    * Mode 0: Four static layers
    * Mode 1: Only 3 layers, but one is affine (can be rotated or scaled)
    * Mode 2: Only 2 layers, but both can be affine
  * Each layer is 512x512 pixels. Affine layers can be 1024x1024
  * Define layers with Tile Maps  in screenblocks (32x32 tiles)
  * Screenblock is 2KB
  * Sprite can be up to 64x64 pixels
  * PPU can also apply affine transforms to sprites
  * Sprites attributes are 32-bits:
    * X/Y position, H/V flipping, size, shape, type (affine or not), and location of first tile
    * Affine data (scaling/rotation)
  * Other affects:
    * Mosaic - makes tiles look blocky (?)
    * Alpha blending - combine colors of overlapping layers (transparency effects)
    * Windowing - Divide screen into two different windows where each can have it's own separate graphics
  * CPU can access VRAM at any time, BUT this can produce unwanted artifacts. Safer to wait for vblank/hblank
  * DMA controller can be scheduled during vblank/hblank
  *