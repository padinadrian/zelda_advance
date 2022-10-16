/*------------------------------------------------------------------------------
 * Copyright (c) 2017
 *     Michael Theall (mtheall)
 *
 * This file is part of gba-tools.
 *
 * gbalzss is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gbalzss is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gbalzss.  If not, see <http://www.gnu.org/licenses/>.
 *----------------------------------------------------------------------------*/
/** @file gbalzss_main.cpp
 *  @brief GBA LZSS Encoder/Decoder
 */

/**
 * Modified for use as a library by padin.adrian@gmail.com
 * Copyright (c) 2022
 */

#include "gbalzss.hpp"
using namespace gbalzss;

namespace gbalzss
{

/** @brief Print program usage
 *  @param[in] fp      File stream to write usage
 *  @param[in] program Program name
 */
void usage(FILE *fp, const char *program)
{
  std::fprintf(fp,
    "Usage: %s [-h|--help] [--lz11] [--vram] <d|e> <infile> <outfile>\n"
    "\tOptions:\n"
    "\t\t-h, --help\tShow this help\n"
    "\t\t--lz11    \tCompress using LZ11 instead of LZ10\n"
    "\t\t--vram    \tGenerate VRAM-safe output (required by GBA BIOS)\n"
    "\n"
    "\tArguments\n"
    "\t\te         \tCompress <infile> into <outfile>\n"
    "\t\td         \tDecompress <infile> into <outfile>\n"
    "\t\t<infile>  \tInput file (use - for stdin)\n"
    "\t\t<outfile> \tOutput file (use - for stdout)\n",
    program);
}

/** @brief Program long options */
const struct option long_options[] =
{
  { "help",    no_argument, nullptr, 'h', },
  { "lz11",    no_argument, nullptr, '1', },
  { "vram",    no_argument, nullptr, 'v', },
  { nullptr,   no_argument, nullptr,   0, },
};

}

int main(int argc, char *argv[])
{
  // get program name
  const char *program = ::basename(argv[0]);

  bool lz11 = false;
  bool vram = false;

  // parse options
  int c;
  while((c = ::getopt_long(argc, argv, "h", long_options, nullptr)) != -1)
  {
    switch(c)
    {
      case 'h':
        usage(stdout, program);
        return EXIT_SUCCESS;

      case '1':
        lz11 = true;
        break;

      case 'v':
        vram = true;
        break;

      default:
        std::fprintf(stderr, "Error: Invalid option '%c'\n", optopt);
        usage(stderr, program);
        return EXIT_FAILURE;
    }
  }

  // check for valid encode/decode non-option
  if(argc - optind != 3
  || std::strlen(argv[optind]) > 1
  || (std::tolower(*argv[optind]) != 'e' && std::tolower(*argv[optind]) != 'd'))
  {
    usage(stderr, program);
    return EXIT_FAILURE;
  }

  // get program non-options
  bool encode = std::tolower(*argv[optind++]) == 'e';
  const char *infile = argv[optind++];
  const char *outfile = argv[optind++];

  // open input file
  FILE *fp;
  if(std::strlen(infile) == 1 && *infile == '-')
    fp = stdin;
  else
    fp = std::fopen(infile, "rb");
  if(!fp)
  {
    std::fprintf(stderr, "Error: Failed to open '%s' for reading\n", infile);
    return EXIT_FAILURE;
  }

  Buffer buffer;

  // read input file
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
  std::fclose(fp);

  // process input file
  try
  {
    if(encode)
      buffer = (lz11 ? lz11_encode : lz10_encode)(buffer, vram);
    else
      buffer = (lz11 ? lz11_decode : lz10_decode)(buffer, vram);
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
  if(!write_file(fp, buffer))
  {
    std::fprintf(stderr, "Error: Failed to write '%s'\n", outfile);
    std::fclose(fp);
    return EXIT_FAILURE;
  }

  // close output file
  std::fclose(fp);

  return EXIT_SUCCESS;
}
