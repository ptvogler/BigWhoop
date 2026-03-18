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
 *        @file tier1.h
 *
 *        This file describes a set of macros, derrived types and functions that define the
 *        fractional bit plane de-/encoding operations that are part of the embedded block
 *        coding paradigm described by the JPEG 2000 standard. For more information please
 *        refere to JPEG2000 by D. S. Taubman and M. W. Marcellin.
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
#ifndef TIER1_H
#define TIER1_H
  /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
  ||                               _ _  _ ____ _    _  _ ___  ____                                ||
  ||                               | |\ | |    |    |  | |  \ |___                                ||
  ||                               | | \| |___ |___ |__| |__/ |___                                ||
  ||                                                                                              ||
  \*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
  #include "bitstream.h"                                  //!< BWC bitstream operations
  #include "types.h"                                      //!< BWC types

  /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
  ||                                _  _ ____ ____ ____ ____ ____                                 ||
  ||                                |\/| |__| |    |__/ |  | [__                                  ||
  ||                                |  | |  | |___ |  \ |__| ___]                                 ||
  ||                                                                                              ||
  \*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
  /*==============================================================================================*/
  /**
   * @details Defines the bit count from the current coding position used for error evaluation in 
   *          the specific coding pass.
   */
  /*=====================================================|========================================*/
  #define DISTORTION_SIG        5                         //!< Clean up/significance propagation
  #define DISTORTION_MAG        6                         //!< Magnitude refinement

  /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
  ||               ___  ____ ____ ____ _ _  _ ____ ___     ___ _   _ ___  ____ ____               ||
  ||               |  \ |___ |__/ |__/ | |  | |___ |  \     |   \_/  |__] |___ [__                ||
  ||               |__/ |___ |  \ |  \ |  \/  |___ |__/     |    |   |    |___ ___]               ||
  ||                                                                                              ||
  \*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
  /*==============================================================================================*/
  /**
   * @details Two adjacent coding stripes (four vertically adjacent samples each), characterized 
   *          by sign (xi), bitfield (bit), coding state (delta/sigma/pi), and neighbourhood.
   */
  /*=====================================================|========================================*/
  typedef struct stripe
  {
    uint64                     *sample;                   //!< Wavelet coefficients

    uint8                       delta;                    //!< Delayed significance
    uint8                       sigma;                    //!< Significance 
    uint8                       pi;                       //!< Membership to coding pass

    uint8                       codingpass;               //!< Last decoded coding pass

    uint8                       bitplane;                 //!< Last decoded bitplane
    uint8                      *bit;                      //!< Wavelet coefficient bitplanes
    uint8                       xi;                       //!< Wavelet coefficient sign 

    struct stripe              *stripe_u;                 //!< Upper neighbour
    struct stripe              *stripe_r;                 //!< Right neighbour
    struct stripe              *stripe_d;                 //!< Lower neighbour
    struct stripe              *stripe_l;                 //!< Left neighbour
  } bwc_coder_stripe;


  /*==============================================================================================*/
  /**
   * @details Structure defining the (de)coding process for a wavelet codeblock. The codeblock 
   *          consists of (depth * dt) slices, each with dimensions width × height. Each slice
   *          is subdivided into stripes containing 4 vertically adjacent coefficients.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uchar                       highband_flag;            //!< Current wavelet subband
    uint8                       K;                        //!< Idx of the first significant bitplane
    uchar                       erres;                    //!< Error resilience flag

    uint64                      no_full_stripe;           //!< Number of full stripes per slice
    uint64                      width, height;            //!< Codeblock dimensions (width × height)
    uint64                      no_slice;                 //!< Number of slices in the codeblock

    uint8                const *sig2context;              //!< Signifance-to-context loopup table
    bwc_bit_coder              *bitcoder;                 //!< Pointer to the BWC bit coder

    bwc_coder_stripe           *data;                     //!< Pointer to the striped data array

    uint64                      buff_size;                //!< Size of packed stream buffer
    uint64                      buff_incr;                //!< Increment for stream assembly
    uchar                      *compressed;               //!< Pointer to compressed data chunck
  } bwc_coder;

  /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
  ||            ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____            ||
  ||            |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__             ||
  ||            |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]            ||
  ||                                                                                              ||
  \*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
  uchar        t1_encode                  (bwc_codec                    *const  codec,
                                           bwc_tile                     *const  tile,
                                           bwc_parameter                *const  parameter);
  //==========|==========================|======================|======|=======|====================
  uchar        t1_decode                  (bwc_codec                    *const  codec,
                                           bwc_tile                     *const  tile,
                                           bwc_parameter                *const  parameter);


#endif