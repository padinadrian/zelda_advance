#include <cstdio>
#include "gba_image_helpers.hpp"
#include "bitmap/bitmap_image.hpp"
using namespace gbahelpers;

int main() {
    printf("Running tests...\n");

    Palette4 palette = {0};
    palette.colors[0].red   = 0xFF;
    palette.colors[1].green = 0xFF;
    palette.colors[2].blue  = 0xFF;

    Buffer source{0, 1, 2, 3};

    std::vector<Pixel> pixels;

    printf("Decoding image...\n");
    image_decode_4bpp(source, gbahelpers::gray_palette, pixels);
    // image_decode_4bpp(source, palette, pixels);

    bitmap_image image(256, 256);
    size_t x = 0;
    size_t y = 0;

    printf("Converting to bitmap...\n");
    for (size_t i = 0; i < pixels.size(); ++i) {
        // First color (4 bits)
        const Pixel& pixel = pixels[i];
        image.set_pixel(x, y, pixel.red, pixel.green, pixel.blue);

        // Index
        x += 1;
        if (x > 255) {
            x = 0;
            y += 1;
        }
    }

    printf("Writing to bitmap file: output.bmp\n");
    image.save_image("./test.bmp");
}

