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

#include <limits.h>

#ifdef __cplusplus
extern "C"
{
#endif
#include "../include/library/private/ht_block_encoding.h"
#include "../include/library/private/types.h"
#ifdef __cplusplus
}
#endif

TEST_CASE("Test initialization of MEL buffer", "[mel_init]")
{
  uint32 buffer_size = 4;
  uint8 *data = (uint8*)calloc(buffer_size, sizeof(uint8));

  mel_struct mel;
  mel_init(&mel, buffer_size, data);

  REQUIRE(mel.buf[0] == 0);
  REQUIRE(mel.buf[1] == 0);
  REQUIRE(mel.buf[2] == 0);
  REQUIRE(mel.buf[3] == 0);
  REQUIRE(mel.pos == 0);
  REQUIRE(mel.buf_size == buffer_size);
  REQUIRE(mel.k == 0);
  REQUIRE(mel.run == 0);
  REQUIRE(mel.threshold == 1);
  REQUIRE(mel.remaining_bits == 8);
  REQUIRE(mel.tmp == 0);

  free(data);
}

TEST_CASE("Test bit emission to MEL buffer", "[mel_emit_bit]")
{
  uint32 buffer_size = 4;
  uint8 *data = (uint8*)calloc(buffer_size, sizeof(uint8));

  mel_struct mel;
  mel_init(&mel, buffer_size, data);

  int seq_size = 8;
  std::vector<int> bit_seq(seq_size, 0);

  // Emit 0's into first byte;
  for (int i = 0; i < seq_size; ++i)
  {
    mel_emit_bit(&mel, bit_seq[i]);
    REQUIRE(mel.tmp == 0);
  }
  REQUIRE(mel.pos == 1);
  REQUIRE(mel.remaining_bits == 8);
  REQUIRE(mel.buf[mel.pos-1] == 0);

  // Emit arbitrary bits into second buffer
  bit_seq = {1,0,1,0,1,0,1,0};
  for (int i = 0; i < seq_size; ++i)
  {
    mel_emit_bit(&mel, bit_seq[i]);
  }
  REQUIRE(mel.tmp == 0);
  REQUIRE(mel.pos == 2);
  REQUIRE(mel.buf[mel.pos-1] == 0xAA);

  // Emit 0xFF byte;
  bit_seq = {1,1,1,1,1,1,1,1};
  for (int i = 0; i < seq_size; ++i)
  {
    mel_emit_bit(&mel, bit_seq[i]);
  }
  REQUIRE(mel.tmp == 0);
  REQUIRE(mel.pos == 3);
  REQUIRE(mel.remaining_bits == 7);
  REQUIRE(mel.buf[mel.pos-1] == 0xFF);

  // When previous byte is 0xFF
  // only 7 bits will be stored in the next
  // byte prepended by a 0 bit.
  for (int i = 0; i < seq_size-1; ++i)
  {
    mel_emit_bit(&mel, bit_seq[i]);
  }
  REQUIRE(mel.tmp == 0);
  REQUIRE(mel.pos == 4);
  REQUIRE(mel.remaining_bits == 8);
  REQUIRE(mel.buf[mel.pos-1] == 0x7F);
}


TEST_CASE ("Quantize individual samples", "[quantize_sample]")
{

  bwc_raw   sign_mask = ~(static_cast<bwc_raw> (0x01) << PREC_BIT);
  bwc_float qt_scale  = static_cast<bwc_float> (0.125);

  std::vector<bwc_sample> samples (13);
  std::vector<bwc_raw>    nu (13, static_cast<bwc_raw> (0));
  std::vector<uint8>      sigma (13, static_cast<uint8> (0));

  samples[0].f = static_cast<bwc_float> (-832.0);
  samples[1].f = static_cast<bwc_float> (832.0);
  samples[2].f = static_cast<bwc_float> (83279793.0);
  samples[3].f = static_cast<bwc_float> (-83279793.0);
  samples[4].f = static_cast<bwc_float> (8327979349850341.0);
  samples[5].f = static_cast<bwc_float> (-8327979349850341.0);
  samples[6].f = static_cast<bwc_float> (-0.0);
  samples[7].f = static_cast<bwc_float> (0.0);
  samples[8].f = static_cast<bwc_float> (UINT64_MAX);
  // Note, tricky test: INT64_MAX 9223372036854775807 is not representable
  // as floating point value. After casting the actual value is 9223372036854775808
  samples[9].f  = static_cast<bwc_float> (INT64_MAX);
  samples[10].f = static_cast<bwc_float> (INT64_MIN);
  samples[11].f = static_cast<bwc_float> (1.0 / qt_scale);
  samples[12].f = static_cast<bwc_float> (FLT_MIN);

  quantize_sample (&nu[0], &sigma[0], &samples[0], sign_mask, qt_scale);
  quantize_sample (&nu[1], &sigma[1], &samples[1], sign_mask, qt_scale);
  quantize_sample (&nu[2], &sigma[2], &samples[2], sign_mask, qt_scale);
  quantize_sample (&nu[3], &sigma[3], &samples[3], sign_mask, qt_scale);
  quantize_sample (&nu[4], &sigma[4], &samples[4], sign_mask, qt_scale);
  quantize_sample (&nu[5], &sigma[5], &samples[5], sign_mask, qt_scale);
  quantize_sample (&nu[6], &sigma[6], &samples[6], sign_mask, qt_scale);
  quantize_sample (&nu[7], &sigma[7], &samples[7], sign_mask, qt_scale);
  quantize_sample (&nu[8], &sigma[8], &samples[8], sign_mask, qt_scale);
  quantize_sample (&nu[9], &sigma[9], &samples[9], sign_mask, qt_scale);
  quantize_sample (&nu[10], &sigma[10], &samples[10], sign_mask, qt_scale);
  quantize_sample (&nu[11], &sigma[11], &samples[11], sign_mask, qt_scale);
  quantize_sample (&nu[12], &sigma[12], &samples[12], sign_mask, qt_scale);

  REQUIRE (nu[0] == 207);
  REQUIRE (sigma[0] == 1);
  REQUIRE (nu[1] == 206);
  REQUIRE (sigma[1] == 1);
  REQUIRE (nu[2] == 20819946);
  REQUIRE (sigma[2] == 1);
  REQUIRE (nu[3] == 20819947);
  REQUIRE (sigma[3] == 1);
  REQUIRE (nu[4] == 2081994837462582);
  REQUIRE (sigma[4] == 1);
  REQUIRE (nu[5] == 2081994837462583);
  REQUIRE (sigma[5] == 1);
  REQUIRE (nu[6] == 0);
  REQUIRE (sigma[6] == 0);
  REQUIRE (nu[7] == 0);
  REQUIRE (sigma[7] == 0);
  REQUIRE (nu[8] == 4611686018427387902);
  REQUIRE (sigma[8] == 1);
  REQUIRE (nu[9] == 2305843009213693950);
  REQUIRE (sigma[9] == 1);
  REQUIRE (nu[10] == 2305843009213693951);
  REQUIRE (sigma[10] == 1);
  REQUIRE (nu[11] == 0);
  REQUIRE (sigma[11] == 1);
  REQUIRE (nu[12] == 0);
  REQUIRE (sigma[12] == 0);
}
