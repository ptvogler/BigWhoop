/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||                                                                                                ||
||       /$$$$$$$  /$$                  /$$      /$$ /$$                                          ||
||      | $$__  $$|__/                 | $$  /$ | $$| $$                                          ||
||      | $$  \ $$ /$$  /$$$$$$        | $$ /$$$| $$| $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$       ||
||      | $$$$$$$ | $$ /$$__  $$       | $$/$$ $$ $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$      ||
||      | $$__  $$| $$| $$  \ $$       | $$$$_  $$$$| $$  \ $$| $$  \ $$| $$  \ $$| $$  \ $$      ||
||      | $$  \ $$| $$| $$  | $$       | $$$/ \  $$$| $$  | $$| $$  | $$| $$  | $$| $$  | $$      ||
||      | $$$$$$$/| $$|  $$$$$$$       | $$/   \  $$| $$  | $$|  $$$$$$/|  $$$$$$/| $$$$$$$/      ||
||      |_______/ |__/ \____  $$       |__/     \__/|__/  |__/ \______/  \______/ | $$____/       ||
||                     /$$  \ $$                                                  | $$            ||
||                    |  $$$$$$/                                                  | $$            ||
||                     \______/                                                   |__/            ||
||                                                                                                ||
\*  --------------------------------------------------------------------------------------------  */
/**
 *        @file test_bitstream.cpp
 *
 *        This file defines unit tests for functionality defined in src/library/bitstream.c
 *                                                                                                */
/*  --------------------------------------------------------------------------------------------  *\
||  Copyright (c) 2025, High Performance Computing Center - University of Stuttgart               ||
||                                                                                                ||
||  Redistribution and use in source and binary forms, with or without modification, are          ||
||  permitted provided that the following conditions are met:                                     ||
||                                                                                                ||
||     (1)   Redistributions of source code must retain the above copyright notice, this list of  ||
||           conditions and the following disclaimer.                                             ||
||                                                                                                ||
||     (2)   Redistributions in binary form must reproduce the above copyright notice, this list  ||
||           of conditions and the following disclaimer in the documentation and/or other         ||
||           materials provided with the distribution.                                            ||
||                                                                                                ||
||  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS   ||
||  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF               ||
||  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE    ||
||  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,     ||
||  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF            ||
||  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)        ||
||  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR      ||
||  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  ||
||  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                            ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

#include <catch2/catch_test_macros.hpp>

#include <cstdint>

#ifdef __cplusplus
extern "C"
{
#endif
#include "../include/library/private/bitstream.h"
#include "../include/library/private/types.h"
#ifdef __cplusplus
}
#endif

TEST_CASE ("Bit stream initialization", "[init_bitstream]")
{
  constexpr uint32 size = 5;
  uchar  inp_mem[size];
  inp_mem[0]       = 1;
  inp_mem[1]       = 2;
  inp_mem[2]       = 3;
  inp_mem[3]       = 4;
  inp_mem[4]       = 44;
  char       instr = 'c';
  bitstream *stream;

  stream = init_bitstream (inp_mem, size, instr);

  REQUIRE (stream);
  REQUIRE (stream->memory);
  REQUIRE (stream->Lmax == size);
  REQUIRE (stream->size_incr == size / 2);
  REQUIRE (((uchar *)stream->memory)[0] == 1);
  REQUIRE (((uchar *)stream->memory)[1] == 2);
  REQUIRE (((uchar *)stream->memory)[2] == 3);
  REQUIRE (((uchar *)stream->memory)[3] == 4);
  REQUIRE (((uchar *)stream->memory)[4] == 44);
  REQUIRE (stream->t == 8);
  REQUIRE (stream->L == 0);
  REQUIRE (stream->L == bytes_used (stream));
  REQUIRE (stream->T == 0);
  REQUIRE (stream->error == 0);

  free (stream);

  instr  = 'd';
  stream = init_bitstream (inp_mem, size, instr);

  REQUIRE (stream);
  REQUIRE (stream->memory);
  REQUIRE (stream->Lmax == size);
  REQUIRE (stream->size_incr == size / 2);
  REQUIRE (((uchar *)stream->memory)[0] == 1);
  REQUIRE (((uchar *)stream->memory)[1] == 2);
  REQUIRE (((uchar *)stream->memory)[2] == 3);
  REQUIRE (((uchar *)stream->memory)[3] == 4);
  REQUIRE (((uchar *)stream->memory)[4] == 44);
  REQUIRE (stream->t == 0);
  REQUIRE (stream->L == 0);
  REQUIRE (stream->L == bytes_used (stream));
  REQUIRE (stream->T == 0);
  REQUIRE (stream->error == 0);

  free (stream);
}

TEST_CASE ("Check used bytes in bitstream buffer", "[bytes_used]")
{
  uint32 size = 5;
  uchar  inp_mem[size];
  inp_mem[0]       = 1;
  inp_mem[1]       = 2;
  inp_mem[2]       = 3;
  inp_mem[3]       = 4;
  inp_mem[4]       = 44;
  char       instr = 'c';
  bitstream *stream;

  stream = init_bitstream (inp_mem, size, instr);

  REQUIRE (bytes_used (stream) == stream->L);

  stream->T = 0xFF;
  REQUIRE (bytes_used (stream) == (stream->L + 1));

  stream->T = 0xFE;
  REQUIRE (bytes_used (stream) == stream->L);

  free (stream);
}

TEST_CASE ("Pass chunk to bitstream", "[emit_chunck]")
{
  uint32     size               = 10;
  uchar      inp_mem[size]      = {0};
  uint64     chunk_size         = 4;
  uchar      chunk1[chunk_size] = {1, 2, 3, 4};
  uchar      chunk2[chunk_size] = {4, 3, 2, 1};
  char       instr              = 'c';
  bitstream *stream;

  stream = init_bitstream (inp_mem, size, instr);

  emit_chunck (stream, chunk1, chunk_size);

  REQUIRE (stream);
  REQUIRE (!stream->error);
  REQUIRE (stream->memory);
  REQUIRE (stream->L == chunk_size);
  REQUIRE (((uchar *)stream->memory)[0] == 1);
  REQUIRE (((uchar *)stream->memory)[1] == 2);
  REQUIRE (((uchar *)stream->memory)[2] == 3);
  REQUIRE (((uchar *)stream->memory)[3] == 4);

  emit_chunck (stream, chunk2, chunk_size);

  REQUIRE (stream);
  REQUIRE (!stream->error);
  REQUIRE (stream->memory);
  REQUIRE (stream->L == 2 * chunk_size);
  REQUIRE (((uchar *)stream->memory)[4] == 4);
  REQUIRE (((uchar *)stream->memory)[5] == 3);
  REQUIRE (((uchar *)stream->memory)[6] == 2);
  REQUIRE (((uchar *)stream->memory)[7] == 1);

  emit_chunck (stream, chunk2, chunk_size);

  REQUIRE (stream);
  REQUIRE (stream->error);
  REQUIRE (stream->Lmax == 0);

  free (stream);
}

TEST_CASE ("Pass symbol to bitstream", "[emit_symbol]")
{
  // Variables for stream creation
  uint32     size          = 10;
  uchar      inp_mem[size] = {0};
  uint32     symbol;
  char       instr = 'c';
  bitstream *stream;

  // Variables for testing
  int    i = 0;
  uint8  byte;
  uint32 test_symbol = 0;

  // bitstream initialization
  stream = init_bitstream (inp_mem, size, instr);

  // Test stream validity & first symbol in stream
  symbol = UINT32_MAX;
  emit_symbol (stream, symbol, sizeof (symbol));
  for (i = sizeof (symbol), test_symbol = 0; i > 0; --i)
    {
      byte         = (uint8)stream->memory[sizeof (symbol) - i];
      test_symbol |= ((uint64)byte << ((i - 1) * 8));
    }
  REQUIRE (stream);
  REQUIRE (!stream->error);
  REQUIRE (stream->memory);
  REQUIRE (stream->L == sizeof (symbol));
  REQUIRE (test_symbol == symbol);

  // Test stream validity & second symbol in stream
  symbol = (uint32)31415;
  emit_symbol (stream, symbol, sizeof (symbol));
  for (i = sizeof (symbol), test_symbol = 0; i > 0; --i)
    {
      byte         = (uint8)stream->memory[2 * sizeof (symbol) - i];
      test_symbol |= ((uint64)byte << ((i - 1) * 8));
    }
  REQUIRE (stream);
  REQUIRE (!stream->error);
  REQUIRE (stream->memory);
  REQUIRE (stream->L == 2 * sizeof (symbol));
  REQUIRE (test_symbol == symbol);

  // Test invalidation of stream
  emit_symbol (stream, symbol, sizeof (symbol));
  REQUIRE (stream);
  REQUIRE (stream->error);
  REQUIRE (stream->Lmax == 0);

  free (stream);
}

TEST_CASE ("Retrieve chunk from bitstream", "[get_chunck]")
{
  uint32     size = 10;
  uchar      inp_mem[size];
  inp_mem[0] = 1;
  inp_mem[1] = 2;
  inp_mem[2] = 3;
  inp_mem[3] = 4;
  inp_mem[4] = 4;
  inp_mem[5] = 3;
  inp_mem[6] = 2;
  inp_mem[7] = 1;
  inp_mem[8] = 99;
  inp_mem[9] = 88;

  char       instr = 'd';
  bitstream *stream;
  uint64     chunk_size = 4;
  uchar     *retrieved_chunk;

  stream = init_bitstream (inp_mem, size, instr);

  REQUIRE(stream);
  REQUIRE(stream->memory);
  REQUIRE(stream->L == 0);
  REQUIRE(stream->Lmax == size);

  // TEST 1: Retrieve first chunk and verify content
  retrieved_chunk = get_chunck(stream, chunk_size);
  REQUIRE(stream);
  REQUIRE(!stream->error);
  REQUIRE(stream->L == chunk_size);
  REQUIRE(retrieved_chunk != NULL);
  for(uint64 i = 0; i < chunk_size; i++) {
    REQUIRE(retrieved_chunk[i] == inp_mem[i]);
  }
  free(retrieved_chunk);

  // TEST 2: Retrieve second chunk
  retrieved_chunk = get_chunck(stream, chunk_size);
  REQUIRE(stream);
  REQUIRE(!stream->error);
  REQUIRE(stream->L == 2*chunk_size);
  REQUIRE(retrieved_chunk != NULL);
  for(uint64 i = 0; i < chunk_size; i++) {
    REQUIRE(retrieved_chunk[i] == inp_mem[i+chunk_size]);
  }
  free(retrieved_chunk);

  // TEST 3: Try to read beyond available data - should return NULL
  retrieved_chunk = get_chunck(stream, 5);  // Only 2 bytes left
  REQUIRE(stream);
  REQUIRE(stream->error);  // Error flag should be set
  REQUIRE(retrieved_chunk == NULL);
  REQUIRE(stream->L == 8);  // L should not change on failure
  free(retrieved_chunk);

  // Reset bitstream to try retrieving exact full size chunk
  stream->L = 0;
  stream->error = 0;

  // TEST 4: Read exact full size
  retrieved_chunk = get_chunck(stream, size);
  REQUIRE(retrieved_chunk != NULL);
  REQUIRE(stream->L == 10);
  REQUIRE(stream->L == stream->Lmax);
  for(uint64 i = 0; i < size; i++) {
    REQUIRE(retrieved_chunk[i] == inp_mem[i]);
  }
  free(retrieved_chunk);

  // TEST 5: Try to read when L == Lmax (no data left)
  retrieved_chunk = get_chunck(stream, 1);
  REQUIRE(retrieved_chunk == NULL);
  REQUIRE(stream->error);

  free(stream);
}

TEST_CASE("Retrieve symbol from bitstream", "[get_symbol]")
{
  uint32 size = 20;
  uchar inp_mem[size];

  // Initialize with simple sequential values
  for(int i = 0; i < size; i++) {
    inp_mem[i] = i + 1;
  }

  char instr = 'd';
  bitstream *stream = init_bitstream(inp_mem, size, instr);

  REQUIRE ((stream && !stream->error));
  REQUIRE(stream->L == 0);

  // TEST 1: Read 4-byte symbol (bytes 1,2,3,4)
  uint64 symbol = get_symbol(stream, 4);
  REQUIRE((stream && !stream->error));
  REQUIRE(stream->L == 4);
  REQUIRE(symbol == 0x01020304);

  // TEST 2: Read 2-byte symbol (bytes 5,6)
  symbol = get_symbol(stream, 2);
  REQUIRE((stream && !stream->error));
  REQUIRE(stream->L == 6);
  REQUIRE(symbol == 0x0506);

  // TEST 3: Read 1-byte symbol (byte 7)
  symbol = get_symbol(stream, 1);
  REQUIRE((stream && !stream->error));
  REQUIRE(stream->L == 7);
  REQUIRE(symbol == 7);

  // TEST 4: Read 8-byte symbol (bytes 8,9,10,11,12,13,14,15)
  symbol = get_symbol(stream, 8);
  REQUIRE((stream && !stream->error));
  REQUIRE(stream->L == 15);
  REQUIRE(symbol == 0x08090A0B0C0D0E0F);

  // TEST 5: Try to read beyond available data
  symbol = get_symbol(stream, 10);  // Only 5 bytes left
  REQUIRE((stream && stream->error));
  REQUIRE(stream->L == 15);  // L unchanged on failure
  REQUIRE(symbol == 0);  // Returns 0 on error

  free(stream);
}

TEST_CASE("Retrieve bits from bitstream", "[get_bit]") {
  uint32 size = 5;
  uchar inp_mem[size];

  inp_mem[0] = 0xAA;  // 1010 1010
  inp_mem[1] = 0x55;  // 0101 0101
  inp_mem[2] = 0xFF;  // 1111 1111 (special case!)
  inp_mem[3] = 0x40;  // 0100 0000 (only 7 bits read after 0xFF)
  inp_mem[4] = 0x00;  // 0000 0000

  char instr = 'd';
  bitstream *stream = init_bitstream(inp_mem, size, instr);

  REQUIRE(stream);
  REQUIRE(stream->L == 0);
  REQUIRE(stream->t == 0);  // Decode mode starts with t=0

  // TEST 1: Read 0xAA = 1010 1010 (MSB first: bit 7 → bit 0)
  REQUIRE(get_bit(stream) == 1);  // bit 7
  // updated L and t after first bit from next byte
  REQUIRE(stream->T == 0xAA); // fetched next byte
  REQUIRE(stream->L == 1);    // incremented byte position
  REQUIRE(stream->t == 7);    // decremented bit position t by 1 from 8
  REQUIRE(get_bit(stream) == 0);  // bit 6
  REQUIRE(get_bit(stream) == 1);  // bit 5
  REQUIRE(get_bit(stream) == 0);  // bit 4
  REQUIRE(get_bit(stream) == 1);  // bit 3
  REQUIRE(get_bit(stream) == 0);  // bit 2
  REQUIRE(get_bit(stream) == 1);  // bit 1
  REQUIRE(get_bit(stream) == 0);  // bit 0

  // TEST 2: Read 0x55 = 0101 0101
  REQUIRE(get_bit(stream) == 0); // bit 7
  REQUIRE(stream->T == 0x55); // fetched next byte
  REQUIRE(stream->L == 2);    // incremented byte position
  REQUIRE(stream->t == 7);    // decremented bit position t by 1 from 8
  REQUIRE(get_bit(stream) == 1); // bit 6
  REQUIRE(get_bit(stream) == 0); // bit 5
  REQUIRE(get_bit(stream) == 1); // bit 4
  REQUIRE(get_bit(stream) == 0); // bit 3
  REQUIRE(get_bit(stream) == 1); // bit 2
  REQUIRE(get_bit(stream) == 0); // bit 1
  REQUIRE(get_bit(stream) == 1); // bit 0
  REQUIRE(stream->t == 0);

  // TEST 3: Read 0xFF = 1111 1111
  REQUIRE(get_bit(stream) == 1); // bit 7
  REQUIRE(stream->T == 0xFF); // fetched next byte
  REQUIRE(stream->L == 3);    // incremented byte position
  REQUIRE(stream->t == 7);    // decremented bit position t by 1 from 8
  REQUIRE(get_bit(stream) == 1); // bit 6
  REQUIRE(get_bit(stream) == 1); // bit 5
  REQUIRE(get_bit(stream) == 1); // bit 4
  REQUIRE(get_bit(stream) == 1); // bit 3
  REQUIRE(get_bit(stream) == 1); // bit 2
  REQUIRE(get_bit(stream) == 1); // bit 1
  REQUIRE(get_bit(stream) == 1); // bit 0
  REQUIRE(stream->t == 0);

  // TEST 4: After 0xFF, only 7 bits from 0x40 = 0100 0000
  // Bits read: bit 6 → bit 0 (bit 7 is skipped!)
  REQUIRE(get_bit(stream) == 1);  // bit 6
  REQUIRE(stream->T == 0x40);     // fetched next byte
  REQUIRE(stream->L == 4);        // incremented byte position
  REQUIRE(stream->t == 6);        // bit position t decremneted by 1 from 7
  REQUIRE(get_bit(stream) == 0);  // bit 5
  REQUIRE(get_bit(stream) == 0);  // bit 4
  REQUIRE(get_bit(stream) == 0);  // bit 3
  REQUIRE(get_bit(stream) == 0);  // bit 2
  REQUIRE(get_bit(stream) == 0);  // bit 1
  REQUIRE(get_bit(stream) == 0);  // bit 0
  REQUIRE(stream->t == 0);  // Back to normal after non-0xFF byte

  // TEST 5: Read 0x00 = 0000 0000
  REQUIRE(get_bit(stream) == 0); // bit 7
  REQUIRE(stream->T == 0x00);    // fetched next byte
  REQUIRE(stream->L == 5);       // incremented byte position
  REQUIRE(stream->t == 7);       // decremented bit position t by 1 from 8
  REQUIRE(get_bit(stream) == 0); // bit 6
  REQUIRE(get_bit(stream) == 0); // bit 5
  REQUIRE(get_bit(stream) == 0); // bit 4
  REQUIRE(get_bit(stream) == 0); // bit 3
  REQUIRE(get_bit(stream) == 0); // bit 2
  REQUIRE(get_bit(stream) == 0); // bit 1
  REQUIRE(get_bit(stream) == 0); // bit 0
  REQUIRE(stream->t == 0);

  // TEST 6: byte position equals length of bistream memory
  REQUIRE(stream->L == stream->Lmax);
  REQUIRE(get_bit(stream) == 1);

  free(stream);
}
