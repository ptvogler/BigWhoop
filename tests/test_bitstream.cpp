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

#include <iostream>
#include <bitset>

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

  REQUIRE ((stream && !stream->error));
  REQUIRE (stream->memory);
  REQUIRE (stream->Lmax == size);
  REQUIRE (stream->size_incr == size / 2);
  REQUIRE (((uchar *)stream->memory)[0] == 1);         // First byte
  REQUIRE (((uchar *)stream->memory)[size - 1] == 44); // Last byte
  REQUIRE (stream->t == 8);
  REQUIRE (stream->L == 0);
  REQUIRE (stream->L == bytes_used (stream));
  REQUIRE (stream->T == 0);
  REQUIRE (stream->error == 0);

  free (stream);

  instr  = 'd';
  stream = init_bitstream (inp_mem, size, instr);

  REQUIRE ((stream && !stream->error));
  REQUIRE (stream->memory);
  REQUIRE (stream->Lmax == size);
  REQUIRE (stream->size_incr == size / 2);
  REQUIRE (((uchar *)stream->memory)[0] == 1);         // First byte
  REQUIRE (((uchar *)stream->memory)[size - 1] == 44); // Last byte
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

  REQUIRE ((stream && !stream->error));
  REQUIRE (stream->L == chunk_size);
  REQUIRE (((uchar *)stream->memory)[0] == 1);
  REQUIRE (((uchar *)stream->memory)[1] == 2);
  REQUIRE (((uchar *)stream->memory)[2] == 3);
  REQUIRE (((uchar *)stream->memory)[3] == 4);

  emit_chunck (stream, chunk2, chunk_size);

  REQUIRE ((stream && !stream->error));
  REQUIRE (stream->L == 2 * chunk_size);
  REQUIRE (((uchar *)stream->memory)[4] == 4);
  REQUIRE (((uchar *)stream->memory)[5] == 3);
  REQUIRE (((uchar *)stream->memory)[6] == 2);
  REQUIRE (((uchar *)stream->memory)[7] == 1);

  emit_chunck (stream, chunk2, chunk_size);

  REQUIRE ((stream && stream->error));
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
  REQUIRE ((stream && !stream->error));
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
  REQUIRE ((stream && !stream->error));
  REQUIRE (stream->L == 2 * sizeof (symbol));
  REQUIRE (test_symbol == symbol);

  // Test invalidation of stream
  emit_symbol (stream, symbol, sizeof (symbol));
  REQUIRE ((stream && stream->error));
  REQUIRE (stream->Lmax == 0);

  free (stream);
}

TEST_CASE ("Test emitting alternating bits", "[emit_bit]")
{
  uint32     size    = 1;
  uchar     *inp_mem = (uchar *)calloc (size, sizeof (uchar));
  uchar      byte    = 0x00;
  char       instr   = 'c';
  bitstream *stream;

  stream          = init_bitstream (inp_mem, size, instr);
  uint64 org_Lmax = stream->Lmax;

  // 0b01010101
  // Emitting 8 bits
  for (int i = 4; i >= 1; --i)
    {
      emit_bit (stream, 0);
      emit_bit (stream, 1);
      byte |= (1 << 2 * i - 2);
      REQUIRE ((stream && !stream->error));
      if (i != 1)
        {
          REQUIRE (stream->t == 2 * i - 2);    // expected bit position
          REQUIRE (stream->T == byte);         // expected state of T
        }
      else if (i == 1)
        {
          REQUIRE (stream->t == 8);               // reset bit position
          REQUIRE (stream->T == 0x00);            // reset T buffer
          REQUIRE (stream->memory[0] == byte);    // filled buffer was transferred
          REQUIRE (stream->L == 1);               // updated used bytes
        }
    }
  REQUIRE (stream->Lmax == org_Lmax);

  // 0b10101010
  // Emitting 8 bits
  byte = 0x00;
  for (int i = 4; i >= 1; --i)
    {
      emit_bit (stream, 1);
      emit_bit (stream, 0);
      byte |= (1 << 2 * i - 1);
      REQUIRE ((stream && !stream->error));
      if (i != 1)
        {
          REQUIRE (stream->t == 2 * i - 2);    // current bit position
          REQUIRE (stream->T == byte);         // state of T
        }
      else if (i == 1)
        {
          REQUIRE (stream->t == 8);               // reset bit position
          REQUIRE (stream->T == 0x00);            // reset T buffer
          REQUIRE (stream->memory[1] == byte);    // filled buffer was transferred
          REQUIRE (stream->L == 2);               // updated used bytes
          REQUIRE (stream->Lmax != org_Lmax);     // updated memory size
        }
    }
}

TEST_CASE ("Pass only significant bits to bitstream", "[emit_bit]")
{
  uint32     size    = 1;
  uchar     *inp_mem = (uchar *)calloc (size, sizeof (uchar));
  uchar      byte    = 0x00;
  char       instr   = 'c';
  bitstream *stream;

  stream = init_bitstream (inp_mem, size, instr);

  // 0b11111111
  // Emitting 8 bits
  for (int i = 8; i >= 1; --i)
    {
      emit_bit (stream, 1);
      byte |= (1 << i - 1);
      REQUIRE ((stream && !stream->error));
      if (i != 1)
        {
          REQUIRE (stream->t == i - 1);    // current bit position
          REQUIRE (stream->T == byte);     // state of T
        }
      else if (i == 1)
        {
          REQUIRE (stream->t == 7);               // reset bit position
          REQUIRE (stream->T == 0x00);            // reset T buffer
          REQUIRE (stream->memory[0] == byte);    // filled buffer was transferred
          REQUIRE (stream->L == 1);               // updated used bytes
          REQUIRE (stream->Lmax == 2);            // updated memory size
        }
    }

  // 0b01111111
  // Emitting 7 bits
  byte = 0x00;
  for (int i = 7; i >= 1; --i)
    {
      emit_bit (stream, 1);
      byte |= (1 << i - 1);
      REQUIRE ((stream && !stream->error));
      if (i != 1)
        {
          REQUIRE (stream->t == i - 1);    // current bit position
          REQUIRE (stream->T == byte);     // state of T
        }
      else if (i == 1)
        {
          REQUIRE (stream->t == 8);               // reset bit position
          REQUIRE (stream->T == 0x00);            // reset T buffer
          REQUIRE (stream->memory[1] == byte);    // filled buffer was transferred
          REQUIRE (stream->L == 2);               // updated used bytes
          REQUIRE (stream->Lmax == 2);            // updated memory size
        }
    }

  free (inp_mem);
  free (stream);
}

TEST_CASE ("Error propagation prevents emit_bit operations", "[emit_bit]")
{
  uint32     size    = 1;
  uchar     *inp_mem = (uchar *)calloc (size, sizeof (uchar));
  bitstream *stream  = init_bitstream (inp_mem, size, 'c');

  REQUIRE ((stream && !stream->error));

  // Manually set error flag to simulate a previous error condition
  stream->error = 1;

  // Store current state
  uint8 current_t = stream->t;
  uint8 current_T = stream->T;
  uint64 current_L = stream->L;
  uint64 current_Lmax = stream->Lmax;

  // Try to emit bits - should do nothing due to error state
  emit_bit (stream, 1);
  emit_bit (stream, 0);
  emit_bit (stream, 1);

  // Verify that NOTHING changed due to error state
  REQUIRE (stream->t == current_t);       // Bit counter unchanged
  REQUIRE (stream->T == current_T);       // Bit buffer unchanged
  REQUIRE (stream->L == current_L);       // Bytes written unchanged
  REQUIRE (stream->Lmax == current_Lmax); // Max size unchanged
  REQUIRE (stream->error == 1);           // Error flag still set

  free (inp_mem);
  free (stream);
}
