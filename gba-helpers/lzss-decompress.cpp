/**
 * @file lzss-decompress.cpp
 * @author Adrian Padin (padin.adrian@gmail.com)
 * @brief Tool for extracting LZSS-compressed images from a GBA ROM file.
 * @version 0.1
 * @date 2022-05-21
 *
 * @copyright Copyright (c) 2022
 *
 */

/* ===== Includes ===== */

#include "gbalzss.hpp"
#include "gba_image_helpers.hpp"
#include "bitmap/bitmap_image.hpp"
using namespace gbalzss;
using namespace gbahelpers;

namespace
{

/** @brief Print program usage
 *  @param[in] fp      File stream to write usage
 *  @param[in] program Program name
 */
void usage(FILE *fp, const char *program)
{
    std::fprintf(
        fp,
        "Usage: %s [-h|--help] [--vram] [--lz11] <infile> <offset> <outfile>\n"
        "\tOptions:\n"
        "\t\t-h, --help \tShow this help\n"
        "\t\t--lz11     \tCompress using LZ11 instead of LZ10\n"
        "\t\t--vram     \tGenerate VRAM-safe output (required by GBA BIOS)\n"
        "\t\t--verbose  \tPrint more detailed messages while processing (helpful for debugging)\n"
        "\n"
        "\tArguments\n"
        "\t\t<infile>  \tInput file (use - for stdin)\n"
        "\t\t<outfile> \tOutput file (use - for stdout)\n",
        program
    );
}

/** @brief Program long options */
const struct option long_options[] =
{
    { "help",       no_argument, nullptr, 'h', },
    { "lz11",       no_argument, nullptr, '1', },
    { "vram",       no_argument, nullptr, 'm', },
    { "verbose",    no_argument, nullptr, 'v', },
    { nullptr,      no_argument, nullptr,   0, },
};

const uint8_t palette[16][3] = {
    {0xFF, 0xFF, 0xFF},     // 0
    {0x80, 0xEE, 0xEE},     // 1
    {0x60, 0xDD, 0xDD},     // 2
    {0x40, 0xCC, 0xCC},     // 3
    {0x30, 0xBB, 0xBB},     // 4
    {0x20, 0xAA, 0xAA},     // 5
    {0x18, 0x99, 0x99},     // 6
    {0x10, 0x88, 0x88},     // 7
    {0x0C, 0x77, 0x77},     // 8
    {0x08, 0x66, 0x66},     // 9
    {0x06, 0x55, 0x55},     // A
    {0x04, 0x44, 0x44},     // B
    {0x03, 0x33, 0x33},     // C
    {0x02, 0x22, 0x22},     // D
    {0x01, 0x11, 0x11},     // E
    {0x00, 0x00, 0x00},     // F
};

}

int main(int argc, char *argv[])
{
    // Get program name
    const char *program = ::basename(argv[0]);

    bool lz11 = false;
    bool vram = false;
    bool verbose = false;

    // Parse options
    int c;
    while ((c = ::getopt_long(argc, argv, "h", long_options, nullptr)) != -1) {
        switch (c) {
            case 'h':
                usage(stdout, program);
                return EXIT_SUCCESS;

            case '1':
                lz11 = true;
                break;

            case 'm':
                vram = true;
                break;

            case 'v':
                verbose = true;
                break;

            default:
                std::fprintf(stderr, "Error: Invalid option '%c'\n", optopt);
                usage(stderr, program);
                return EXIT_FAILURE;
        }
    }

    // Check for correct number of arguments
    if (argc - optind != 3) {
        usage(stderr, program);
        return EXIT_FAILURE;
    }

    // Get program arguments
    bool encode = false;
    const char *infile = argv[optind++];
    const char *offset = argv[optind++];
    const char *outfile = argv[optind++];

    if (verbose) {
        printf("Input file: %s\n", infile);
        printf("Offset: %s\n", offset);
        printf("Ouput file: %s\n", outfile);
    }

    // Open input file
    FILE *fp;
    if (std::strlen(infile) == 1 && *infile == '-') {
        fp = stdin;
    }
    else {
        fp = std::fopen(infile, "rb");
    }
    if (!fp) {
        std::fprintf(stderr, "Error: Failed to open '%s' for reading\n", infile);
        return EXIT_FAILURE;
    }

    // Get offset (base 16)
    uint32_t offset_long = strtol(offset, (char **)NULL, 16);
    if (offset_long < 0x00000000) {
        std::fprintf(stderr, "Error: invalid offset: 0x%x\n", offset_long);
        return EXIT_FAILURE;
    }

    if (verbose) {
        printf("Seeking to offset: 0x%x\n", offset_long);
    }

    // Seek to offset
    int result = fseek(fp, offset_long, SEEK_SET);
    if (result != 0) {
        usage(stderr, program);
        return EXIT_FAILURE;
    }

    Buffer buffer;

    // Read input file
    if (verbose) {
        printf("Reading input file\n");
    }
    try
    {
        buffer = read_file(fp, encode ? LZSS_MAX_ENCODE_LEN : LZSS_MAX_DECODE_LEN);
    }
    catch(const std::runtime_error &e)
    {
        std::fprintf(stderr, "%s: %s\n", infile, e.what());
        std::fclose(fp);
        return EXIT_FAILURE;
    }
    catch(...)
    {
        std::fprintf(stderr, "%s: Error: unhandled exception\n", infile);
        std::fclose(fp);
        return EXIT_FAILURE;
    }

    // close input file
    if (verbose) {
        printf("Closing input file\n");
    }
    std::fclose(fp);

    // process input file
    if (verbose) {
        printf("Processing file\n");
    }
    try
    {
        if (encode) {
            buffer = (lz11 ? lz11_encode : lz10_encode)(buffer, vram);
        }
        else {
            buffer = (lz11 ? lz11_decode : lz10_decode)(buffer, vram);
        }
    }
    catch(const std::runtime_error &e)
    {
        std::fprintf(stderr, "%s: %s\n", infile, e.what());
        return EXIT_FAILURE;
    }
    catch(...)
    {
        std::fprintf(stderr, "%s: Error: unhandled exception\n", infile);
        return EXIT_FAILURE;
    }

    // open output file
    if(std::strlen(outfile) == 1 && *outfile == '-')
        fp = stdout;
    else
        fp = std::fopen(outfile, "wb");
    if(!fp)
    {
        std::fprintf(stderr, "Error: Failed to open '%s' for writing\n", outfile);
        return EXIT_FAILURE;
    }

    // write output file
    if (verbose) {
        printf("Writing to output file: %s\n", outfile);
    }
    if(!write_file(fp, buffer))
    {
        std::fprintf(stderr, "Error: Failed to write '%s'\n", outfile);
        std::fclose(fp);
        return EXIT_FAILURE;
    }

    // close output file
    std::fclose(fp);

    printf("Decoding image...\n");
    std::vector<Pixel> pixels;
    image_decode_4bpp(buffer, gbahelpers::gray_palette, pixels);

    if (verbose) {
        printf("Converting to bitmap\n");
    }
    export_to_bitmap(pixels, "./output.bmp");

    // Write to bitmap file
    // bitmap_image image(256, 256);

    // size_t x = 0;
    // size_t y = 0;
    // size_t row = 0;
    // size_t col = 0;
    // uint8_t red = 0;
    // uint8_t green = 0;
    // uint8_t blue = 0;

    // if (verbose) {
    //     printf("Converting to bitmap\n");
    // }
    // for (size_t i = 0; i < pixels.size(); ++i) {

    //     const Pixel& pixel = pixels[i];
    //     image.set_pixel(
    //         (col * 8) + x,
    //         (row * 8) + y,
    //         pixel.red,
    //         pixel.green,
    //         pixel.blue
    //     );

    //     // Index
    //     x += 1;
    //     if (x > 7) {
    //         x = 0;
    //         y += 1;
    //         if (y > 7) {
    //             y = 0;
    //             col += 1;
    //             if (col > 31) {
    //                 col = 0;
    //                 row += 1;
    //             }
    //         }
    //     }
    // }

    // Pattern of tiles, 8x8 pixels, 16x32 tiles
    // for )
    //     const Pixel& pixel = pixels[i];
    //     image.set_pixel(
    //         row + x,
    //         col + y,
    //         pixel.red,
    //         pixel.green,
    //         pixel.blue
    //     );


    if (verbose) {
        printf("Writing to bitmap file: output.bmp\n");
    }

    // image.save_image("./output.bmp");

    return EXIT_SUCCESS;
}
