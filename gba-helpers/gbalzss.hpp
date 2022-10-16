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

/** @file gbalzss.hpp
 *  @brief GBA LZSS Encoder/Decoder
 */

/**
 * Modified for use as a library by: padin.adrian@gmail.com
 * Copyright (c) 2022
 */

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <getopt.h>
#include <libgen.h>
#include <cstddef>

namespace gbalzss
{

/** @brief LZSS maximum encodable size */
#define LZSS_MAX_ENCODE_LEN 0x00FFFFFF

/** @brief LZSS maximum (theoretical) decodable size
 *  (LZSS_MAX_ENCODE_LEN+1)*9/8 + 4 - 1
 */
#define LZSS_MAX_DECODE_LEN 0x01B00003

/** @brief LZ10 maximum match length */
#define LZ10_MAX_LEN  18

/** @brief LZ10 maximum displacement */
#define LZ10_MAX_DISP 4096

/** @brief LZ11 maximum match length */
#define LZ11_MAX_LEN  65808

/** @brief LZ11 maximum displacement */
#define LZ11_MAX_DISP 4096

/** @brief LZ compression mode */
enum LZSS_t
{
  LZ10 = 0x10, ///< LZ10 compression
  LZ11 = 0x11, ///< LZ11 compression
};

/** @brief Buffer object */
typedef std::vector<uint8_t> Buffer;

/** @brief Find last instance of a byte in a buffer
 *  @param[in] first Beginning of buffer
 *  @param[in] last  End of buffer
 *  @param[in] val   Byte to find
 *
 *  @returns iterator to found byte
 *  @retval last if no match found
 */
Buffer::const_iterator
rfind(Buffer::const_iterator first, Buffer::const_iterator last,
      const uint8_t &val);

/** @brief Find best buffer match
 *  @param[in]  source   Source buffer
 *  @param[in]  it       Position in source buffer
 *  @param[in]  len      Maximum length to match
 *  @param[in]  max_disp Maximum displacement
 *  @param[in]  vram     VRAM-safe
 *  @param[out] outlen   Length of match
 *  @returns Iterator to best match
 *  @retval source.cend() for no match
 */
Buffer::const_iterator
find_best_match(const Buffer &source, Buffer::const_iterator it, size_t len,
                size_t max_disp, bool vram, size_t &outlen);

/** @brief Output a GBA-style compression header
 *  @param[out] header Output header
 *  @param[in]  type   Compression type
 *  @param[in]  size   Uncompressed data size
 */
void
header(Buffer &buffer, uint8_t type, size_t size);

/** @brief LZ10/LZ11 compression
 *  @param[in] source Source buffer
 *  @param[in] mode   LZ mode
 *  @param[in] vram   VRAM-safe
 *  @returns Compressed buffer
 */
Buffer
lzss_encode(const Buffer &source, LZSS_t mode, bool vram);

/** @brief LZ10 compression
 *  @param[in] source Source buffer
 *  @param[in] vram   VRAM-safe
 *  @returns Compressed buffer
 */
Buffer
lz10_encode(const Buffer &source, bool vram);

/** @brief LZ11 compression
 *  @param[in] source Source buffer
 *  @param[in] vram   VRAM-safe
 *  @returns Compressed buffer
 */
Buffer
lz11_encode(const Buffer &source, bool vram);

/** @brief LZ10 Decompression
 *  @param[in] source Source buffer
 *  @param[in] vram   VRAM-safe
 *  @returns Decompressed buffer
 */
Buffer
lz10_decode(const Buffer &source, bool vram);

/** @brief LZ11 Decompression
 *  @param[in] source Source buffer
 *  @param[in] vram   VRAM-safe
 *  @returns Decompressed buffer
 */
Buffer
lz11_decode(const Buffer &source, bool vram);

/** @brief Read input file
 *  @param[in] fp    Input file stream
 *  @param[in] limit Maximum file size to read
 *  @returns Buffer containing file contents
 */
Buffer read_file(FILE *fp, size_t limit);

/** @brief Write output file
 *  @param[in] fp    Output file stream
 *  @param[in] limit Maximum file size to write
 *  @returns Whether successfully written
 */
bool write_file(FILE *fp, const Buffer &buffer);

}
