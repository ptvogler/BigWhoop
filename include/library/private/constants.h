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
 *        @file constants.h
 *
 *        This file defines simple constants that are used to make the code more readable.
 *                                                                                                */
/*  --------------------------------------------------------------------------------------------  *\
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
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#ifndef CONSTANTS_H
#define CONSTANTS_H

  /*==============================================================================================*/
  /**
   * @details 1-dimensional wavelet kernels supported by the BWC library.
   */
  /*=====================================================|========================================*/
  typedef enum
  {
    bwc_dwt_9_7,                                          //!< Cohen Daubechies Feauveau 9/7
    bwc_dwt_5_3,                                          //!< LeGall 5/3
    bwc_dwt_haar                                          //!< Haar
  } bwc_dwt_filter;

  /*==============================================================================================*/
  /**
   * @details BWC codestream packing order:   Resolution: (RES)   Layer:  (LYR)
   *                                          Parameter:  (PRM)   Packet: (PKT)
   */
  /*=====================================================|========================================*/
  typedef enum
  {
    bwc_prog_LRCP,                                        //!< (LYR) > (RES) > (RRM) > (PKT)
    bwc_prog_RLCP,                                        //!< (RES) > (LYR) > (RRM) > (PKT)
    bwc_prog_RPCL,                                        //!< (RES) > (PKT) > (RRM) > (LYR)
    bwc_prog_PCRL,                                        //!< (PKT) > (RRM) > (RES) > (LYR)
    bwc_prog_CPRL                                         //!< (RRM) > (PKT) > (RES) > (LYR)
  } bwc_prog_ord;

  /*==============================================================================================*/
  /**
   * @details Pre/Post entropy stage quantization during (de)/compression.
   */
  /*=====================================================|========================================*/
  typedef enum
  {
    bwc_qt_none,                                          //!< None
    bwc_qt_derived,                                       //!< Derrived
  } bwc_quant_st;

  /*==============================================================================================*/
  /**
   * @details Tile definition style used to instruct codec structure.
   */
  /*=====================================================|========================================*/
  typedef enum
  {
    bwc_tile_sizeof,                                      //!< By tile size
    bwc_tile_numbof,                                      //!< By number of tiles
  } bwc_tile_instr;

  /*==============================================================================================*/
  /**
   * @details Data set sample precision.
   */
  /*=====================================================|========================================*/
  typedef enum
  {
    bwc_precision_half = 2,                               //!< Half precision
    bwc_precision_single = 4,                             //!< Single Precision
    bwc_precision_double = 8,                             //!< Double Precision
  } bwc_precision;

  /*==============================================================================================*/
  /**
   * @details BWC library coding mode.
   */
  /*=====================================================|========================================*/
  typedef enum
  {
    comp,                                                 //!< Compression
    decomp,                                               //!< Decompression
  } bwc_mode;
#endif