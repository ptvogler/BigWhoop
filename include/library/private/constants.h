/*================================================================================================*\
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
||  DESCRIPTION:                                                                                  ||
||  ------------                                                                                  ||
||                                                                                                ||
||        This file defines simple constants that are used to make the code more readable.        ||
||                                                                                                ||
||  --------------------------------------------------------------------------------------------  ||
||  Copyright (c) 2023, High Performance Computing Center - University of Stuttgart               ||
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
\*================================================================================================*/
#ifndef CONSTANTS_H
#define CONSTANTS_H
  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         These constants are used to signal spatial or temporal the wavelet filter.             !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef enum
  {
    bwc_dwt_9_7,                                          // Cohen Daubechies Feauveau 9/7 Wavelet
    bwc_dwt_5_3,                                          // LeGall 5/3 Wavelet
    bwc_dwt_haar                                          // Haar Wavelet
  } bwc_dwt_filter;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         These constants are used to signal the packing order of the codestream.                !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef enum
  {
    bwc_prog_LRCP,                                        // Layer / Resolution / Parameter / Packet
    bwc_prog_RLCP,                                        // Resolution / Layer / Parameter / Packet
    bwc_prog_RPCL,                                        // Resolution / Packet / Parameter / Layer
    bwc_prog_PCRL,                                        // Packet / Parameter / Resolution / Layer
    bwc_prog_CPRL                                         // Parameter / Packet / Resolution / Layer
  } bwc_prog_ord;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         These constants are used to signal the quantisation style during                       !
  !         (de)coompression.                                                                      !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef enum
  {
    bwc_qt_none,                                          // No quantization
    bwc_qt_derived,                                       // Derrived according to Taubman/Marcellin
  } bwc_quant_st;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         These constants are used to signal dataset tiling by the function caller.              !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef enum
  {
    bwc_tile_sizeof,                                      // Tiling defined by size of one tile
    bwc_tile_numbof,                                      // Tiling defined by the number of tiles
  } bwc_tile_instr;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         These constants are used to signal the dataset sample precision.                       !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef enum
  {
    bwc_precision_half = 2,
    bwc_precision_single = 4,
    bwc_precision_double = 8,
  } bwc_precision;

  typedef enum
  {
    comp,
    decomp,
  } bwc_mode;
#endif