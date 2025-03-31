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
#include "../include/library/private/ht_block_encoding.h"
#include "../include/library/private/types.h"
#ifdef __cplusplus
}
#endif

TEST_CASE("Quantize a individual samples", "[quantize_sample]")
{

  bwc_raw   sign_mask = ~(static_cast<bwc_raw>(0x01) << PREC_BIT);
  bwc_float qt_scale  = static_cast<bwc_float>(0.125);

  std::vector<bwc_sample> samples(8);
  std::vector<bwc_raw>    nu(8, static_cast<bwc_raw>(0));
  std::vector<uint8>      sigma(8, static_cast<uint8>(0));

  samples[0].f = static_cast<bwc_float>(-832.0);
  samples[1].f = static_cast<bwc_float>(832.0);
  samples[2].f = static_cast<bwc_float>(83279793.0);
  samples[3].f = static_cast<bwc_float>(-83279793.0);
  samples[4].f = static_cast<bwc_float>(8327979349850341.0);
  samples[5].f = static_cast<bwc_float>(-8327979349850341.0);
  samples[6].f = static_cast<bwc_float>(-0.0);
  samples[7].f = static_cast<bwc_float>(0.0);

  quantize_sample(&nu[0], &sigma[0], &samples[0], sign_mask, qt_scale);
  quantize_sample(&nu[1], &sigma[1], &samples[1], sign_mask, qt_scale);
  quantize_sample(&nu[2], &sigma[2], &samples[2], sign_mask, qt_scale);
  quantize_sample(&nu[3], &sigma[3], &samples[3], sign_mask, qt_scale);
  quantize_sample(&nu[4], &sigma[4], &samples[4], sign_mask, qt_scale);
  quantize_sample(&nu[5], &sigma[5], &samples[5], sign_mask, qt_scale);
  quantize_sample(&nu[6], &sigma[6], &samples[6], sign_mask, qt_scale);
  quantize_sample(&nu[7], &sigma[7], &samples[7], sign_mask, qt_scale);

  REQUIRE(nu[0] == 207);
  REQUIRE(sigma[0] == 1);
  REQUIRE(nu[1] == 206);
  REQUIRE(sigma[1] == 1);
  REQUIRE(nu[2] == 20819946);
  REQUIRE(sigma[2] == 1);
  REQUIRE(nu[3] == 20819947);
  REQUIRE(sigma[3] == 1);
  REQUIRE(nu[4] == 2081994837462582);
  REQUIRE(sigma[4] == 1);
  REQUIRE(nu[5] == 2081994837462583);
  REQUIRE(sigma[5] == 1);
  REQUIRE(nu[6] == 0);
  REQUIRE(sigma[6] == 0);
  REQUIRE(nu[7] == 0);
  REQUIRE(sigma[7] == 0);
}
