/**
 * @file gba_image_helpers.cpp
 * @author Adrian Padin (padin.adrian@gmail.com)
 * @brief Helper functions for decompressing images from a GBA ROM file.
 * @version 0.1
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 */

/* ===== Includes ===== */
#include "gba_image_helpers.hpp"
#include "bitmap/bitmap_image.hpp"

namespace gbahelpers {

const Palette4 gray_palette = {
    Pixel{0x00, 0x00, 0x00},
    Pixel{0x11, 0x11, 0x11},
    Pixel{0x22, 0x22, 0x22},
    Pixel{0x33, 0x33, 0x33},
    Pixel{0x44, 0x44, 0x44},
    Pixel{0x55, 0x55, 0x55},
    Pixel{0x66, 0x66, 0x66},
    Pixel{0x77, 0x77, 0x77},
    Pixel{0x88, 0x88, 0x88},
    Pixel{0x99, 0x99, 0x99},
    Pixel{0xAA, 0xAA, 0xAA},
    Pixel{0xBB, 0xBB, 0xBB},
    Pixel{0xCC, 0xCC, 0xCC},
    Pixel{0xDD, 0xDD, 0xDD},
    Pixel{0xEE, 0xEE, 0xEE},
    Pixel{0xFF, 0xFF, 0xFF},
};

const Palette4 teal_palette = {
    Pixel{0x00, 0x00, 0x00},
    Pixel{0x00, 0x11, 0x11},
    Pixel{0x00, 0x22, 0x22},
    Pixel{0x00, 0x33, 0x33},
    Pixel{0x00, 0x44, 0x44},
    Pixel{0x00, 0x55, 0x55},
    Pixel{0x00, 0x66, 0x66},
    Pixel{0x00, 0x77, 0x77},
    Pixel{0x00, 0x88, 0x88},
    Pixel{0x00, 0x99, 0x99},
    Pixel{0x00, 0xAA, 0xAA},
    Pixel{0x00, 0xBB, 0xBB},
    Pixel{0x00, 0xCC, 0xCC},
    Pixel{0x00, 0xDD, 0xDD},
    Pixel{0x00, 0xEE, 0xEE},
    Pixel{0x00, 0xFF, 0xFF},
};

/**
 * @brief Convert a raw byte array into a list of pixels (RGB).
 * @param[in]   source  Input buffer containing the raw data.
 * @param[in]   palette List of pixels used to look up the correct color.
 * @param[out]  pixels  Resulting array of pixels.
 */
void image_decode_4bpp(
    const Buffer& source,
    const Palette4& palette,
    std::vector<Pixel>& pixels
)
{
    // First determine how many Pixels there will be to allocate space
    // in the output vector.
    // Each byte is 2 pixels, so reserve a space twice the size of the source buffer.
    pixels.reserve(2 * source.size());

    // Used as the lookup index into the palette. Range 0-15
    uint8_t nibble;

    for (size_t i = 0; i < source.size(); ++i) {

        // In 16-color mode (4bpp mode) each group of 4 bits represents
        // a single pixel. The 4 bits are used as an index into the palette
        // to determine the RGB values.
        // Each byte contains 2 pixels; the upper nibble and the lower nibble.

        // Lower nibble
        nibble = source[i] & 0xF;
        pixels.push_back(palette.colors[nibble]);

        // Upper nibble
        nibble = source[i] >> 4;
        pixels.push_back(palette.colors[nibble]);
    }
}

/**
 * @brief Convert a list of Pixels into a bitmap.
 * @param[in]   pixels      Array of pixels
 * @param[in]   filename    Name of output bitmap file.
 * @return Zero on success, nonzero on failure.
 */
int export_to_bitmap(
    const std::vector<Pixel>& pixels,
    const std::string filename
)
{
    // Write to bitmap file
    bitmap_image image(256, 256);

    size_t x = 0;
    size_t y = 0;
    size_t row = 0;
    size_t col = 0;

    for (size_t i = 0; i < pixels.size(); ++i) {

        // Copy pixel values into the image.
        const Pixel& pixel = pixels[i];
        image.set_pixel(
            (col * 8) + x,
            (row * 8) + y,
            pixel.red,
            pixel.green,
            pixel.blue
        );

        // Pixels are arranged in groups of 8x8 called tiles.
        // Each row has 32 tiles.
        x += 1;
        if (x > 7) {
            x = 0;
            y += 1;
            if (y > 7) {
                y = 0;
                col += 1;
                if (col > 31) {
                    col = 0;
                    row += 1;
                }
            }
        }
    }

    image.save_image(filename);

    return 0;
}

}