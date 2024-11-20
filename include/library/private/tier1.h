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
||        This file describes a set of functions that can be used to de-/encode bwc               ||
||        codeblocks described by the bwc_codec structure according to the embedded block         ||
||        coding paradigm described by the JPEG 2000 standard. For more information please        ||
||        refere to JPEG2000 by D. S. Taubman and M. W. Marcellin.                                ||
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
#ifndef TIER1_H
#define TIER1_H
  /************************************************************************************************\
  ||                               _ _  _ ____ _    _  _ ___  ____                                ||
  ||                               | |\ | |    |    |  | |  \ |___                                ||
  ||                               | | \| |___ |___ |__| |__/ |___                                ||
  ||                                                                                              ||
  \************************************************************************************************/
  #include "types.h"

  /************************************************************************************************\
  ||                                _  _ ____ ____ ____ ____ ____                                 ||
  ||                                |\/| |__| |    |__/ |  | [__                                  ||
  ||                                |  | |  | |___ |  \ |__| ___]                                 ||
  ||                                                                                              ||
  \************************************************************************************************/
  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         The macros listed indicate how many bits, from the current coding position,            !
  !         are used for error evaluation in the coding passes.                                    !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  #define DISTORTION_SIG        5                         // Clean up/significance propagation pass.
  #define DISTORTION_MAG        6                         // Magnitude refinement pass.

  /************************************************************************************************\
  ||               ___  ____ ____ ____ _ _  _ ____ ___     ___ _   _ ___  ____ ____               ||
  ||               |  \ |___ |__/ |__/ | |  | |___ |  \     |   \_/  |__] |___ [__                ||
  ||               |__/ |___ |  \ |  \ |  \/  |___ |__/     |    |   |    |___ ___]               ||
  ||                                                                                              ||
  \************************************************************************************************/
  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds the sign (xi), bitfield (bit) and state (delta, sigma and         !
  !         pi) information for four vertically adjacent samples for easy access during the        !
  !         entropy encoding stage. Here, the delayed significance is set once the first bit       !
  !         is encoded during the magnitude refinement pass, while the significance state          !
  !         sigma is set once the first non-zero bit is encoded for a specific sample.             !
  !         The stripe_* pointers are used to store the address of the left(l), upper(u),          !
  !         right(r) and lower(d) neighbour of a specific stripe for easy access. To               !
  !         facilitate distortion estimation the magnitude of the wavelet coefficients is          !
  !         stored in an appropriate sample array.                                                 !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct stripe
  {
    uint64                     *sample;                   // Wavelet coef. for two adjacent stripes.

    uint8                       delta;                    // Delayed sig. for two adjacent stripes.
    uint8                       sigma;                    // Significance for two adjacent stripes. 
    uint8                       pi;                       // Cdng pass membership for adj. stripes.

    uint8                       codingpass;               // Last decoded coding pass.

    uint8                       bitplane;                 // Last decoded bitplane.
    uint8                      *bit;                      // Bitplanes for vertically adj. stripes.
    uint8                       xi;                       // Wvlt coef. sign bit for adj. stripes.

    struct stripe              *stripe_u;                 // Upper stripe.
    struct stripe              *stripe_r;                 // Right stripe.
    struct stripe              *stripe_d;                 // Lower stripe.
    struct stripe              *stripe_l;                 // Left stripe.
  } bwc_coder_stripe;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure accumulates all necessary structures used to (de-)compress a            !
  !         codeblock of wavelet coefficients. The spatial and temporal dimension of the           !
  !         codeblock is defined by its width, height and number of slices (depth * dt). The       !
  !         codeblock itself is subdivided into so-called stripes that represent 4 vertical-       !
  !         ly adjacent coefficients. The parameter no_full_stripes stores overnumber of           !
  !         full stripes present in one slice.                                                     !
  !         The look-up table sig2context is used to determine the coding context of a cur-        !
  !         ren bit according to its significance context within the codeblock bitplane.           !
  !         The look-up table is specific to a certain highband and is set accordingly.            !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uchar                       highband_flag;            // Flag indicating current wavelet subbnd.
    uint8                       K;                        // Idx of the first significant bitplane.
    uchar                       erres;                    // Flag signaling error resilience.

    uint64                      no_full_stripe;           // Number of full, vert. adjacent stripes.
    uint64                      width, height;            // Codeblock width and height.
    uint64                      no_slice;                 // N.o. slices in the spec. codeblock.

    uint8                const *sig2context;              // Signifance-to-context loop-up table.
    bwc_bit_coder              *bitcoder;                 // BWC bitcoder.

    bwc_coder_stripe           *data;                     // BWC coder stripe.

    uint64                      buff_size;                // Size of packed stream.
    uint64                      buff_incr;                // Increment for packed stream assembly.
    uchar                      *compressed;               // Compressed data chunck.
  } bwc_coder;

  /************************************************************************************************\
  ||            ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____            ||
  ||            |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__             ||
  ||            |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]            ||
  ||                                                                                              ||
  \************************************************************************************************/
  uchar        t1_encode                  (bwc_codec                    *const  codec,
                                           bwc_tile                     *const  tile,
                                           bwc_parameter                *const  parameter);
  //==========|==========================|======================|======|=======|====================
  uchar        t1_decode                  (bwc_codec                    *const  codec,
                                           bwc_tile                     *const  tile,
                                           bwc_parameter                *const  parameter);


#endif