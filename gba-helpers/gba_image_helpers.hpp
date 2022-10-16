/**
 * @file gba_image_helpers.hpp
 * @author Adrian Padin (padin.adrian@gmail.com)
 * @brief Helper functions for decompressing images from a GBA ROM file.
 * @version 0.1
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 */

/* ===== Includes ===== */
#include <cstdint>
#include <vector>
#include <string>

namespace gbahelpers {

// Copied from gbalzss
typedef std::vector<uint8_t> Buffer;

// RGB pixel values (24-bit)
struct Pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

// Palette of 16 colors (4bpp)
struct Palette4 {
    Pixel colors[16];
};

extern const Palette4 gray_palette;
extern const Palette4 teal_palette;

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
);

/**
 * @brief Convert a list of Pixels into a bitmap.
 * @param[in]   pixels      Array of pixels
 * @param[in]   filename    Name of output bitmap file.
 * @return Zero on success, nonzero on failure.
 */
int export_to_bitmap(
    const std::vector<Pixel>& pixels,
    const std::string filename
);

}
