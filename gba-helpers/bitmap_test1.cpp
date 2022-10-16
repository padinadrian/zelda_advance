#include "bitmap/bitmap_image.hpp"
#include <cstdio>

const uint8_t palette[16][3] = {
    {0xFF, 0xFF, 0xFF},     // 0
    {0xEE, 0xEE, 0xEE},     // 1
    {0xDD, 0xDD, 0xDD},     // 2
    {0xCC, 0xCC, 0xCC},     // 3
    {0xBB, 0xBB, 0xBB},     // 4
    {0xAA, 0xAA, 0xAA},     // 5
    {0x99, 0x99, 0x99},     // 6
    {0x88, 0x88, 0x88},     // 7
    {0x77, 0x77, 0x77},     // 8
    {0x66, 0x66, 0x66},     // 9
    {0x55, 0x55, 0x55},     // A
    {0x44, 0x44, 0x44},     // B
    {0x33, 0x33, 0x33},     // C
    {0x22, 0x22, 0x22},     // D
    {0x11, 0x11, 0x11},     // E
    {0x00, 0x00, 0x00},     // F
    // {0xFF, 0xFF, 0xFF},     // 0
    // {0x80, 0xEE, 0xEE},     // 1
    // {0x60, 0xDD, 0xDD},     // 2
    // {0x40, 0xCC, 0xCC},     // 3
    // {0x30, 0xBB, 0xBB},     // 4
    // {0x20, 0xAA, 0xAA},     // 5
    // {0x18, 0x99, 0x99},     // 6
    // {0x10, 0x88, 0x88},     // 7
    // {0x0C, 0x77, 0x77},     // 8
    // {0x08, 0x66, 0x66},     // 9
    // {0x06, 0x55, 0x55},     // A
    // {0x04, 0x44, 0x44},     // B
    // {0x03, 0x33, 0x33},     // C
    // {0x02, 0x22, 0x22},     // D
    // {0x01, 0x11, 0x11},     // E
    // {0x00, 0x00, 0x00},     // F
};

int main() {

    printf("test\n");

    bitmap_image image(256, 256);
    size_t x = 0;
    size_t y = 0;
    uint8_t red;

    for (size_t i = 0; i < 0x4000; ++i) {

        x = i % 256;
        y = i / 256;

        red = palette[y % 16][0];
        image.set_pixel(x, y, 0xff - red, 0, 0xff - red);

    }

    printf("Writing to bitmap file: output.bmp\n");

    image.save_image("./output.bmp");
}