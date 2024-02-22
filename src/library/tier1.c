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
||        codeblocks described by the bwc_field structure according to the embedded block         ||
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
/************************************************************************************************************\
||                                      _ _  _ ____ _    _  _ ___  ____                                     ||
||                                      | |\ | |    |    |  | |  \ |___                                     ||
||                                      | | \| |___ |___ |__| |__/ |___                                     ||
||                                                                                                          ||
\************************************************************************************************************/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#include "constants.h"
#include "macros.h"
#include "types.h"
#include "dwt.h"
#include "mq.h"
#include "tier1.h"

/************************************************************************************************************\
||                                      _  _ ____ ____ ____ ____ ____                                       ||
||                                      |\/| |__| |    |__/ |  | [__                                        ||
||                                      |  | |  | |___ |  \ |__| ___]                                       ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   Macros:                                                                                                  !
!   -------                                                                                                  !
!                Macro                                Description                                            !
!                -----                                -----------                                            !
!                                                                                                            !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                01.02.2019  Patrick Vogler     B87D120     V 0.1.0     Macros created                       !
\*----------------------------------------------------------------------------------------------------------*/
#define encode_segmark(bitcoder)                   \
{                                                  \
   mq_bit_encode(bitcoder, 1, CONTEXT_UNI);        \
   mq_bit_encode(bitcoder, 0, CONTEXT_UNI);        \
   mq_bit_encode(bitcoder, 1, CONTEXT_UNI);        \
   mq_bit_encode(bitcoder, 0, CONTEXT_UNI);        \
}

/************************************************************************************************************\
||           ____ _  _ ___ ____ ____ _  _ ____ _       ____ ____ _  _ ____ ___ ____ _  _ ___ ____           ||
||           |___  \/   |  |___ |__/ |\ | |__| |       |    |  | |\ | [__   |  |__| |\ |  |  [__            ||
||           |___ _/\_  |  |___ |  \ | \| |  | |___    |___ |__| | \| ___]  |  |  | | \|  |  ___]           ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                These constants define the distortion estimation for the significance propagation (TS)      !
!                and magnitude refinement (TM) pass as lookup tables. These lookup tables were evaluated     !
!                using the appropriate equations from JPEG2000 by David S. Taubman and Michael W. Marcellin  !
!                (p. 378):                                                                                   !
!                                                                                                            !
!                             𝛥D_i^(p,k) = G_bi × 𝛥_i^2 × 2^2p ×                                             !
!                                          ┌                                                                 !
!                                          | ∑_{j∈P_i^(p,k), ν_i^p = 1} TS(νtilde_i^(p+1)[j]) +              !
!                                          └                                                  ┐              !
!                                              ∑_{j∈P_i^(p,k), ν_i^p>1} TM(νtilde_i^(p+1)[j]) |              !
!                                                                                             ┘              !
!                             TS(νtilde) = (2νtilde)^2 - (2νtilde - 1 - δ)^2                                 !
!                             TM(νtilde) = (2νtilde - 1)^2 - (2νtilde - ⌊2νtilde^(p+1)⌋ - δ)^2               !
!                                                                                                            !
!                here 𝛥D_i^(p,k) describes the aggregate distortion contribution, G_bi the energy gain fac-  !
!                tor for subband b_i, 𝛥_i the quantization step-size, ν_i the quantization magnitude, νtilde !
!                the fraction part of ν_i, ν_i^p the quantization magnitude obtained by dropping the last p  !
!                bits of v_i and TS/TM the respective lookup tables. The index p is used to identify a spe-  !
!                cific bitplane.                                                                             !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                23.01.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static const int32 DISTORTION_TM_LUT[64] = 
{49152, 47104, 45056, 43008,
 40960, 38912, 36864, 34816,
 32768, 30720, 28672, 26624,
 24576, 22528, 20480, 18432,
 16384, 14336, 12288, 10240,
  8192,  6144,  4096,  2048,
     0, -2048, -4096, -6144,
 -8192,-10240,-12288,-14336,
-16384,-14336,-12288,-10240,
 -8192, -6144, -4096, -2048,
     0,  2048,  4096,  6144,
  8192, 10240, 12288, 14336,
 16384, 18432, 20480, 22528,
 24576, 26624, 28672, 30720,
 32768, 34816, 36864, 38912,
 40960, 43008, 45056, 47104};

static const int32 DISTORTION_TS_LUT[32] = 
{49152,  55296,  61440,  67584,
 73728,  79872,  86016,  92160,
 98304, 104448, 110592, 116736,
122880, 129024, 135168, 141312,
147456, 153600, 159744, 165888,
172032, 178176, 184320, 190464,
196608, 202752, 208896, 215040,
221184, 227328, 233472, 239616};

/*----------------------------------------------------------------------------------------------------------*\
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                These constants define a lookup table that is used to calculate the number of leading zeros !
!                in a pseudo 4-bit integer variable.                                                         !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                15.02.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static const int32 DISTORTION_LZ_LUT[15] = 
{0, 3, 0, 2, 0,
 0, 0, 1, 0, 0,
 0, 0, 0, 0, 0};


/*----------------------------------------------------------------------------------------------------------*\
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                -                                                                                           !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                08.03.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static const uint16 frac2log[256] =
{ 0,   1,   2,   4,   5,   7,   8,   9,  11,  12,  14,  15,  16,  18,  19,  21,
 22,  23,  25,  26,  27,  29,  30,  31,  33,  34,  35,  37,  38,  39,  40,  42,
 43,  44,  46,  47,  48,  49,  51,  52,  53,  54,  56,  57,  58,  59,  61,  62,
 63,  64,  65,  67,  68,  69,  70,  71,  73,  74,  75,  76,  77,  78,  80,  81,
 82,  83,  84,  85,  87,  88,  89,  90,  91,  92,  93,  94,  96,  97,  98,  99,
100, 101, 102, 103, 104, 105, 106, 108, 109, 110, 111, 112, 113, 114, 115, 116,
117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 131, 132, 133,
134, 135, 136, 137, 138, 139, 140, 140, 141, 142, 143, 144, 145, 146, 147, 148,
149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 162, 163,
164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 173, 174, 175, 176, 177, 178,
179, 180, 181, 181, 182, 183, 184, 185, 186, 187, 188, 188, 189, 190, 191, 192,
193, 194, 194, 195, 196, 197, 198, 199, 200, 200, 201, 202, 203, 204, 205, 205,
206, 207, 208, 209, 209, 210, 211, 212, 213, 214, 214, 215, 216, 217, 218, 218,
219, 220, 221, 222, 222, 223, 224, 225, 225, 226, 227, 228, 229, 229, 230, 231,
232, 232, 233, 234, 235, 235, 236, 237, 238, 239, 239, 240, 241, 242, 242, 243,
244, 245, 245, 246, 247, 247, 248, 249, 250, 250, 251, 252, 253, 253, 254, 255};

/*----------------------------------------------------------------------------------------------------------*\
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                -                                                                                           !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                08.03.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static const uint8 SIG2CONTEXT_L[45] =
{CONTEXT_SIG + 0, CONTEXT_SIG + 1, CONTEXT_SIG + 2, CONTEXT_SIG + 2, CONTEXT_SIG + 2,
 CONTEXT_SIG + 3, CONTEXT_SIG + 3, CONTEXT_SIG + 3, CONTEXT_SIG + 3, CONTEXT_SIG + 3,
 CONTEXT_SIG + 4, CONTEXT_SIG + 4, CONTEXT_SIG + 4, CONTEXT_SIG + 4, CONTEXT_SIG + 4,
 CONTEXT_SIG + 5, CONTEXT_SIG + 6, CONTEXT_SIG + 6, CONTEXT_SIG + 6, CONTEXT_SIG + 6,
 CONTEXT_SIG + 7, CONTEXT_SIG + 7, CONTEXT_SIG + 7, CONTEXT_SIG + 7, CONTEXT_SIG + 7,
 CONTEXT_SIG + 7, CONTEXT_SIG + 7, CONTEXT_SIG + 7, CONTEXT_SIG + 7, CONTEXT_SIG + 7,
 CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8,
 CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8,
 CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8};

static const uint8 SIG2CONTEXT_H[45] =
{CONTEXT_SIG + 0, CONTEXT_SIG + 1, CONTEXT_SIG + 2, CONTEXT_SIG + 2, CONTEXT_SIG + 2,
 CONTEXT_SIG + 5, CONTEXT_SIG + 6, CONTEXT_SIG + 6, CONTEXT_SIG + 6, CONTEXT_SIG + 6,
 CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8,
 CONTEXT_SIG + 3, CONTEXT_SIG + 3, CONTEXT_SIG + 3, CONTEXT_SIG + 3, CONTEXT_SIG + 3,
 CONTEXT_SIG + 7, CONTEXT_SIG + 7, CONTEXT_SIG + 7, CONTEXT_SIG + 7, CONTEXT_SIG + 7,
 CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8,
 CONTEXT_SIG + 4, CONTEXT_SIG + 4, CONTEXT_SIG + 4, CONTEXT_SIG + 4, CONTEXT_SIG + 4,
 CONTEXT_SIG + 7, CONTEXT_SIG + 7, CONTEXT_SIG + 7, CONTEXT_SIG + 7, CONTEXT_SIG + 7,
 CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8, CONTEXT_SIG + 8};

static const uint8 SIG2CONTEXT_HH[45] =
{CONTEXT_SIG + 0, CONTEXT_SIG + 3, CONTEXT_SIG + 6, CONTEXT_SIG + 8, CONTEXT_SIG + 8,
 CONTEXT_SIG + 1, CONTEXT_SIG + 4, CONTEXT_SIG + 7, CONTEXT_SIG + 8, CONTEXT_SIG + 8,
 CONTEXT_SIG + 2, CONTEXT_SIG + 5, CONTEXT_SIG + 7, CONTEXT_SIG + 8, CONTEXT_SIG + 8,
 CONTEXT_SIG + 1, CONTEXT_SIG + 4, CONTEXT_SIG + 7, CONTEXT_SIG + 8, CONTEXT_SIG + 8,
 CONTEXT_SIG + 2, CONTEXT_SIG + 5, CONTEXT_SIG + 7, CONTEXT_SIG + 8, CONTEXT_SIG + 8,
 CONTEXT_SIG + 2, CONTEXT_SIG + 5, CONTEXT_SIG + 7, CONTEXT_SIG + 8, CONTEXT_SIG + 8,
 CONTEXT_SIG + 2, CONTEXT_SIG + 5, CONTEXT_SIG + 7, CONTEXT_SIG + 8, CONTEXT_SIG + 8,
 CONTEXT_SIG + 2, CONTEXT_SIG + 5, CONTEXT_SIG + 7, CONTEXT_SIG + 8, CONTEXT_SIG + 8,
 CONTEXT_SIG + 2, CONTEXT_SIG + 5, CONTEXT_SIG + 7, CONTEXT_SIG + 8, CONTEXT_SIG + 8};

static const uint8_t SIG2XI[16] =
{1, 2, 2, 2,
 1, 0, 2, 1,
 1, 2, 0, 1,
 1, 0, 0, 0};

static const uint8_t XI2CONT[11][2] =
{{CONTEXT_SIGN + 4, 1}, {CONTEXT_SIGN + 3, 1}, {CONTEXT_SIGN + 2, 1},
                        {0,                0},
 {CONTEXT_SIGN + 1, 1}, {CONTEXT_SIGN    , 0}, {CONTEXT_SIGN + 1, 0},
                        {0,                0},
 {CONTEXT_SIGN + 2, 0}, {CONTEXT_SIGN + 3, 0}, {CONTEXT_SIGN + 4, 0}};

/************************************************************************************************************\
||                ___  ____ _ _  _ ____ ___ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                ||
||                |__] |__/ | |  | |__|  |  |___    |___ |  | |\ | |     |  | |  | |\ | [__                 ||
||                |    |  \ |  \/  |  |  |  |___    |    |__| | \| |___  |  | |__| | \| ___]                ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void cblkreset_fwd(bwc_coder_stripe *const cblk, uint64 cblksize)                         !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to reset the codeblock memory block used to store the quantized       !
!                wavelet samples alongside their sample states. This function should be called after         !
!                encoding a codeblock.                                                                       !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                cblk                        bwc_coder_stripe*     - Pointer to the codeblock memory block.  !
!                cblksize                    unsigned int(64 bit)  - Size of the codeblock.                  !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                10.12.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
cblkreset_fwd(bwc_coder_stripe *const cblk, uint64 cblksize)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   i;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_coder_stripe    buff;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(cblk);

   /*--------------------------------------------------------*\
   ! Walk through the cblk memory block.                      !
   \*--------------------------------------------------------*/
   for(i = 0; i < cblksize; ++i)
   {
      /*--------------------------------------------------------*\
      ! Save the sample bit and context memory address in a tem- !
      ! porary variables.                                        !
      \*--------------------------------------------------------*/
      buff.sample = cblk[i].sample; 
      buff.bit    = cblk[i].bit;

      /*--------------------------------------------------------*\
      ! Reset the bwc_coder_stripe structure and the memory block!
      ! used to hold the sample bits of the wavelet coefficients.!
      \*--------------------------------------------------------*/
      memset(&cblk[i],    0, sizeof(bwc_coder_stripe));
      memset(buff.bit,    0, PREC_BIT * sizeof(uint8));
      memset(buff.sample, 0, 4 * sizeof(uint64));

      /*--------------------------------------------------------*\
      ! Store the memory address of the sample, bit and context  !
      ! variables in the bwc_coder_stripe structure.             !
      \*--------------------------------------------------------*/
      cblk[i].sample = buff.sample;
      cblk[i].bit    = buff.bit;
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void cblkreset_inv(bwc_coder_stripe *const cblk, const uint64 width, const uint64 height, !
!                                                                   const uint64 depth, const uint64 dt)     !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to reset the codeblock memory block used to store the quantized       !
!                wavelet samples alongside their sample states. This function should be called before        !
!                decoding a codeblock.                                                                       !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                cblk                        bwc_coder_stripe*     - Pointer to the codeblock memory block.  !
!                width, height, depth, dt    unsigned int(64 bit)  - Size of the codeblock.                  !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                10.12.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
cblkreset_inv(bwc_coder_stripe *const cblk, const uint64 width, const uint64 height,
                                            const uint64 depth, const uint64 dt)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   i;
   uint64   limit;
   uint64   cblk_stripe;
   uint64   x, y, z;
   int64    idx_u, idx_r, idx_d, idx_l;
   uint16   t;
   uint8    s;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_coder_stripe    buff;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(cblk);

   /*--------------------------------------------------------*\
   ! Calculate the number of stripes that are to be copied.   !
   ! In case the number of rows is not divisible by 8, the    !
   ! number of stripes is rounded up to the nearest integer.  !
   \*--------------------------------------------------------*/
   cblk_stripe = ceil((double)height/4);

   /*--------------------------------------------------------*\
   ! Walk through all wavelet coefficients in the current     !
   ! codeblock in a stripe pattern and save the sign and      !
   ! significant magnitude bits in the bwc_coder_stripe       !
   ! structure. Here, two adjacent stripes are stored         !
   ! in one 8 bit integer.                                    !
   \*--------------------------------------------------------*/
   for(t = 0, i = 0; t < dt; ++t)
   {
      for(z = 0; z < depth; ++z)
      {
         for(y = 0; y < cblk_stripe; i += width, ++y)
         {
            for(s = 0, limit = height - y * 4; s < 4 && s < limit; ++s)
            {
               for(x = 0; x < width; ++x)
               {
                  /*--------------------------------------------------------*\
                  ! Save the sample bit and context memory address in a tem- !
                  ! porary variables.                                        !
                  \*--------------------------------------------------------*/
                  buff.sample = cblk[x + i].sample; 
                  buff.bit    = cblk[x + i].bit;

                  /*--------------------------------------------------------*\
                  ! Reset the bwc_coder_stripe structure and the memory block !
                  ! used to hold the sample bits of the wavelet coefficients.!
                  \*--------------------------------------------------------*/
                  memset(&cblk[x + i], 0, sizeof(bwc_coder_stripe));
                  memset(buff.bit,     0, PREC_BIT * sizeof(uint8));
                  memset(buff.sample,  0, 4 * sizeof(uint64));

                  /*--------------------------------------------------------*\
                  ! Store the memory address of the sample, bit and context  !
                  ! variables in the bwc_coder_stripe structure.             !
                  \*--------------------------------------------------------*/
                  cblk[x + i].sample = buff.sample;
                  cblk[x + i].bit    = buff.bit;

                  /*--------------------------------------------------------*\
                  ! Evaluate the appropriate index for the stripe neighbours.!
                  ! Neighbours that fall outside of the codeblock boundaries !
                  ! are set to dummy variables to uncouple the current block !
                  ! from its neighbors.                                      !
                  \*--------------------------------------------------------*/
                  idx_u = (y == 0)         ? -1 : (int64)(i + x - width);
                  idx_r = (x == width - 1) ? -2 : (int64)(i + x + 1);
                  idx_d = (s == limit - 1) ? -1 : (int64)(i + x + width);
                  idx_l = (x == 0)         ? -2 : (int64)(i + x - 1);

                  /*--------------------------------------------------------*\
                  ! Set the state pointers.                                  !
                  \*--------------------------------------------------------*/
                  cblk[x + i].stripe_u = &cblk[idx_u];
                  cblk[x + i].stripe_r = &cblk[idx_r];
                  cblk[x + i].stripe_d = &cblk[idx_d];
                  cblk[x + i].stripe_l = &cblk[idx_l];
               }
            }
         }
      }
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void cblkcopy_forward(bwc_coder_stripe *const destination, bwc_sample *const source,      !
!                                         bwc_cblk_access *const cblkaccess,              const uint64 width,!
!                                                          const uint64 height,           const uint64 depth)!
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function walks through the wavelet coefficients in the current codeblock in a stripe   !
!                pattern and saves the sign and magnitude bits in the sign and magnitude bitfields in the    !
!                bwc_coder_stripe structure. Here, the 8-bit sign and magnitude bitfields are used to store  !
!                two adjacent stripes to reduce the memory footprint. Furthermore, the pointer to the neigh- !
!                boring state variables are initialized. States that fall outside of the codeblock bounda-   !
!                ries are set to dummy variables to uncouple the current block from its neighbors.           !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                destination                 bwc_coder_stripe*     - Pointer to the destination memory block.!
!                source                      bwc_sample*           - Pointer to the source memory block.     !
!                cblkaccess                  bwc_cblk_access*      - Structure used to directly access the   !
!                                                                    parameter codeblock and subband struc-  !
!                                                                    ture.                                   !
!                width, height, depth        unsigned int(64 bit)  - Size of the destination memory block.   !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                02.07.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
cblkcopy_forward(bwc_coder_stripe *const destination, bwc_sample *const source,
                 bwc_cblk_access  *const cblkaccess,              const uint64 width,
                                   const uint64 height,           const uint64 depth)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   bwc_raw  buff, sign_mask;
   uint64   bit_mask, limit;
   uint64   cblk_width, cblk_height, cblk_depth, cblk_stripe;
   uint64   incrX, incrY, incrZ;
   uint64   i, x, y, z;
   uint64   X0, Y0, Z0;
   uint64   X1, Y1, Z1;
   int64    idx_u, idx_r, idx_d, idx_l;
   uint16   TS0, TS1; 
   uint16   cblk_dt;
   uint16   t;
   uint8    b, Kmax, s;

   /*-----------------------*\
   ! DEFINE FLOAT VARIABLES: !
   \*-----------------------*/
   bwc_float   qt_step_size;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_cblk_inf       *cblk_info;
   bwc_sample         *tmp;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(destination);
   assert(cblkaccess);
   assert(source);

   /*--------------------------------------------------------*\
   ! Save the codeblock info structure to a temporary varia-  !
   ! ble to make the code more readable.                      !
   \*--------------------------------------------------------*/
   cblk_info = &cblkaccess->codeblock->info;

   /*--------------------------------------------------------*\
   ! Setup a bitmask to access the sign bit position.         !
   \*--------------------------------------------------------*/
   sign_mask    = ~((bwc_raw)0x01 << PREC_BIT);

   /*--------------------------------------------------------*\
   ! Initialize the quantization step size and the index of   !
   ! the most significant bitfield.                           !
   \*--------------------------------------------------------*/
   qt_step_size = cblkaccess->subband->control.qt_effective_step_size;
   Kmax         = 0;

   /*--------------------------------------------------------*\
   ! Save the codeblock dimensions to temporary variables to  !
   ! make the code more readable.                             !
   \*--------------------------------------------------------*/
   X0  = cblk_info->X0;
   Y0  = cblk_info->Y0;
   Z0  = cblk_info->Z0;
   TS0 = cblk_info->TS0;

   X1  = cblk_info->X1;
   Y1  = cblk_info->Y1;
   Z1  = cblk_info->Z1;
   TS1 = cblk_info->TS1;

   /*--------------------------------------------------------*\
   ! Calculate the width, height, depth and dt of the code-   !
   ! block to be copied.                                      !
   \*--------------------------------------------------------*/
   cblk_width  = X1 - X0;
   cblk_height = Y1 - Y0;
   cblk_depth  = Z1 - Z0;
   cblk_dt     = TS1 - TS0;

   /*--------------------------------------------------------*\
   ! Calculate the number of stripes that are to be copied.   !
   ! In case the number of rows is not divisible by 8, the    !
   ! number of stripes is rounded up to the nearest integer.  !
   \*--------------------------------------------------------*/
   cblk_stripe = ceil((double)cblk_height/4);

   /*--------------------------------------------------------*\
   ! Calculate the pointer increments used to loop through    !
   ! the source memory.                                       !
   \*--------------------------------------------------------*/
   incrX = width;
   incrY = width * (height - (Y1 - Y0));
   incrZ = width * height * (depth - (Z1 - Z0));

   /*--------------------------------------------------------*\
   ! Associate the temporary pointer to the starting points   !
   ! of the source memory.                                    !
   \*--------------------------------------------------------*/
   tmp = &source[X0 + width * (Y0 + height * (Z0 + depth * TS0))];

   /*--------------------------------------------------------*\
   ! Walk through all wavelet coefficients in the current     !
   ! codeblock in a stripe pattern and save the sign and      !
   ! significant magnitude bits in the bwc_coder_stripe       !
   ! structure. Here, two adjacent stripes are stored         !
   ! in one 8 bit integer.                                    !
   \*--------------------------------------------------------*/
   for(t = 0, i = 0; t < cblk_dt; ++t)
   {
      for(z = 0; z < cblk_depth; ++z)
      {
         for(y = 0; y < cblk_stripe; i += cblk_width, ++y)
         {
            /*--------------------------------------------------------*\
            ! Initialize the stripe bit masks to the first stripe posi-!
            ! tion in the sign, state and sample variables.            !
            \*--------------------------------------------------------*/
            bit_mask = 0x08;

            for(s = 0, limit = cblk_height - y * 4; s < 4 && s < limit; ++s)
            {
               for(x = 0; x < cblk_width; ++x)
               {
                  /*--------------------------------------------------------*\
                  ! Save the sign bit of the current wavelet coefficient in  !
                  ! the appropriate position of the sign field.              !
                  \*--------------------------------------------------------*/
                  destination[x + i].xi |= bit_mask * (tmp[x].raw / ~sign_mask);

                  /*--------------------------------------------------------*\
                  ! Calculate the absolute value of the wavelet coefficient  !
                  ! and store the quantized value in the temporary buffer.   !
                  \*--------------------------------------------------------*/
                  tmp[x].raw &= sign_mask;
                  buff        = (bwc_raw)(tmp[x].f / qt_step_size);

                  /*--------------------------------------------------------*\
                  ! Save the unsigned wavelet coefficient for distortion     !
                  ! calculation.                                             !
                  \*--------------------------------------------------------*/
                  destination[x + i].sample[3 - s] = buff;

                  /*--------------------------------------------------------*\
                  ! set the bitfield index b to zero.                        !
                  \*--------------------------------------------------------*/
                  b = 0;

                  /*--------------------------------------------------------*\
                  ! Save the significant bits of the current wavelet coeffi- !
                  ! cient in the appropriate bitfield and stripe.            !
                  \*--------------------------------------------------------*/
                  while(buff)
                  {
                     destination[x + i].bit[b] |= bit_mask * (buff & 0x01);
                     buff >>= 1;
                     b++;
                  }

                  /*--------------------------------------------------------*\
                  ! Update the index of the most significant bitfield.       !
                  \*--------------------------------------------------------*/
                  Kmax = MAX(Kmax, b);

                  /*--------------------------------------------------------*\
                  ! Evaluate the appropriate index for the stripe neighbours !
                  ! Neighbours that fall outside of the codeblock boundaries !
                  ! are set to dummy variables to uncouple the current block !
                  ! from its neighbors.                                      !
                  \*--------------------------------------------------------*/
                  idx_u = (y == 0)              ? -1 : (int64)(i + x - cblk_width);
                  idx_r = (x == cblk_width - 1) ? -2 : (int64)(i + x + 1);
                  idx_d = (s == limit - 1)      ? -1 : (int64)(i + x + cblk_width);
                  idx_l = (x == 0)              ? -2 : (int64)(i + x - 1);

                  /*--------------------------------------------------------*\
                  ! Set the state pointers.                                  !
                  \*--------------------------------------------------------*/
                  destination[x + i].stripe_u = &destination[idx_u];
                  destination[x + i].stripe_r = &destination[idx_r];
                  destination[x + i].stripe_d = &destination[idx_d];
                  destination[x + i].stripe_l = &destination[idx_l];
               }
               /*--------------------------------------------------------*\
               ! Increment the stripe bit mask to the next stripe posi-   !
               ! tion slice and increment the temporary data pointer to   !
               ! the next row.                                            !
               \*--------------------------------------------------------*/
               bit_mask >>= 1;
               tmp       += incrX;
            }
         }
         /*--------------------------------------------------------*\
         ! Increment to the next column.                            !
         \*--------------------------------------------------------*/
         tmp += incrY;
      }
      /*--------------------------------------------------------*\
      ! Increment to the next spatial slice.                     !
      \*--------------------------------------------------------*/
      tmp += incrZ;
   }
   /*--------------------------------------------------------*\
   ! Save the most significant bitfield of the current sub-   !
   ! band in the appropriate subband structure.               !
   \*--------------------------------------------------------*/
   cblkaccess->codeblock->control.K = Kmax;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void cblkcopy_inverse(bwc_coder_stripe *const destination, bwc_sample *const source,      !
!                                        bwc_cblk_access  *const cblkaccess,              const uint64 width,!
!                                                          const uint64 height,           const uint64 depth)!
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to copy the decompressed wavelet coefficients from the codeblock      !
!                structure to the parameter data memory block.                                               !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                destination                 bwc_coder_stripe*     - Pointer to the destination memory block.!
!                source                      bwc_sample*           - Pointer to the source memory block.     !
!                cblkaccess                  bwc_cblk_access*      - Structure used to directly access the   !
!                                                                    parameter codeblock and subband struc-  !
!                                                                    ture.                                   !
!                width, height, depth        unsigned int(64 bit)  - Size of the destination memory block.   !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                02.07.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
cblkcopy_inverse(bwc_coder_stripe *const source,      bwc_sample *const destination,
                 bwc_cblk_access  *const cblkaccess,              const uint64 width,
                                   const uint64 height,           const uint64 depth)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   cblk_width, cblk_height, cblk_depth, cblk_stripe;
   uint64   bit_shift, buff, limit;
   uint64   incrX, incrY, incrZ;
   uint64   i, x, y, z;
   uint64   X0, Y0, Z0;
   uint64   X1, Y1, Z1;
   uint16   TS0, TS1; 
   uint16   cblk_dt;
   uint16   t;
   uint8    bitplane;
   uint8    codingpass;
   uint8    s;
   int8     b;

   /*-----------------------*\
   ! DEFINE FLOAT VARIABLES: !
   \*-----------------------*/
   bwc_float   qt_step_size;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_cblk_inf       *cblk_info;
   bwc_sample         *tmp;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(destination);
   assert(cblkaccess);
   assert(source);

   /*--------------------------------------------------------*\
   ! Save the codeblock info structure to a temporary varia-  !
   ! ble to make the code more readable.                      !
   \*--------------------------------------------------------*/
   cblk_info = &cblkaccess->codeblock->info;

   /*--------------------------------------------------------*\
   ! Initialize the quantization step size and the index of   !
   ! the most significant bitfield.                           !
   \*--------------------------------------------------------*/
   qt_step_size = cblkaccess->subband->control.qt_effective_step_size;

   /*--------------------------------------------------------*\
   ! Safe the last bitplane and codingpass to temporary vari- !
   ! able to make the code more readable.                     !
   \*--------------------------------------------------------*/
   bitplane    = source->bitplane;
   codingpass  = source->codingpass;

   /*--------------------------------------------------------*\
   ! Save the codeblock dimensions to temporary variables to  !
   ! make the code more readable.                             !
   \*--------------------------------------------------------*/
   X0  = cblk_info->X0;
   Y0  = cblk_info->Y0;
   Z0  = cblk_info->Z0;
   TS0 = cblk_info->TS0;

   X1  = cblk_info->X1;
   Y1  = cblk_info->Y1;
   Z1  = cblk_info->Z1;
   TS1 = cblk_info->TS1;

   /*--------------------------------------------------------*\
   ! Calculate the width, height, depth and dt of the code-   !
   ! block to be copied.                                      !
   \*--------------------------------------------------------*/
   cblk_width  = X1 - X0;
   cblk_height = Y1 - Y0;
   cblk_depth  = Z1 - Z0;
   cblk_dt     = TS1 - TS0;

   /*--------------------------------------------------------*\
   ! Calculate the number of stripes that are to be copied.   !
   ! In case the number of rows is not divisible by 8, the    !
   ! number of stripes is rounded up to the nearest integer.  !
   \*--------------------------------------------------------*/
   cblk_stripe = ceil((double)cblk_height/4);

   /*--------------------------------------------------------*\
   ! Calculate the pointer increments used to loop through    !
   ! the destination memory.                                  !
   \*--------------------------------------------------------*/
   incrX = width;
   incrY = width * (height - (Y1 - Y0));
   incrZ = width * height * (depth - (Z1 - Z0));

   /*--------------------------------------------------------*\
   ! Associate the temporary pointer to the starting points   !
   ! of the destination memory.                               !
   \*--------------------------------------------------------*/
   tmp = &destination[X0 + width * (Y0 + height * (Z0 + depth * TS0))];

   /*--------------------------------------------------------*\
   ! Walk through all wavelet coefficients in the current     !
   ! codeblock in a stripe pattern and save the sign and      !
   ! significant magnitude bits in the bwc_coder_stripe       !
   ! structure. Here, two adjacent stripes are stored         !
   ! in one 8 bit integer.                                    !
   \*--------------------------------------------------------*/
   for(t = 0, i = 0; t < cblk_dt; ++t)
   {
      for(z = 0; z < cblk_depth; ++z)
      {
         for(y = 0; y < cblk_stripe; i += cblk_width, ++y)
         {
            /*--------------------------------------------------------*\
            ! Initialize the stripe bit masks to the first stripe posi-!
            ! tion in the sign, state and sample variables.            !
            \*--------------------------------------------------------*/
            bit_shift = 0x03;

            for(s = 0, limit = cblk_height - y * 4; s < 4 && s < limit; ++s)
            {
               for(x = 0; x < cblk_width; ++x)
               {
                  /*--------------------------------------------------------*\
                  ! Save the significant bits of the current wavelet coeffi- !
                  ! cient in the appropriate bitfield and stripe.            !
                  \*--------------------------------------------------------*/
                  for(b = PREC_BIT, buff = 0; b --> 0;)
                  {
                     buff <<= 1;
                     buff |= (source[x + i].bit[b] >> bit_shift) & 0x01;
                  }

                  /*--------------------------------------------------------*\
                  ! Calculate the absolute value of the wavelet coefficient  !
                  ! and store the quantized value in the temporary buffer.   !
                  \*--------------------------------------------------------*/
                  if(buff)
                  {
                     tmp[x].f    = (bwc_float)buff;
                     tmp[x].f   += (1UL << (bitplane + ((!codingpass || ((source[x + i].pi >> bit_shift) & 0x01)) ? 0 : 1))) * 0.475f;
                     tmp[x].f   *= qt_step_size;
                     tmp[x].raw |= SIGN * ((source[x + i].xi >> bit_shift) & 0x01);
                  }
               }

               /*--------------------------------------------------------*\
               ! Increment the stripe bit mask to the next stripe posi-   !
               ! tion slice and increment the temporary data pointer to   !
               ! the next row.                                            !
               \*--------------------------------------------------------*/
               bit_shift--;
               tmp       += incrX;
            }
         }
         /*--------------------------------------------------------*\
         ! Increment to the next column.                            !
         \*--------------------------------------------------------*/
         tmp += incrY;
      }
      /*--------------------------------------------------------*\
      ! Increment to the next spatial slice.                     !
      \*--------------------------------------------------------*/
      tmp += incrZ;
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void *test(void)                                                                          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                -                           -                       -                                       !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                -           Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uint64
significance_propagation_enc_pass(bwc_coder *const coder, const int8 b)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   i, j;
   uint64   k, x;
   int64    mse;
   uint16   k_h, k_v, k_d;
   uint16   k_sig;
   uint16   xi_h, xi_v;
   uint8    bit_mask, stripe_mask;
   uint8    dist_shift, dist_corr, l;
   uint8    bit;
   uint8    rest;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_coder_stripe   *stripe;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(coder);

   mse         = 0;
   stripe      = coder->data;
   rest        = 4 - (coder->height & 0x03);
   stripe_mask = ~((0x0F >> ((coder->height & 0x03))) | 0xF0);
   dist_shift  = (b < DISTORTION_SIG) ? 0 : (b - DISTORTION_SIG);
   dist_corr   = (b < DISTORTION_SIG) ? (DISTORTION_SIG - b) : 0;

   for(i = 0, k = 0; i < coder->no_slice; ++i)
   {
      for(j = 0; j < coder->no_full_stripe; ++j)
      {
         for(x = 0; x < coder->width; ++x, ++k)
         {
            if(stripe[k].sigma^0xF)
            {
               for(bit = stripe[k].bit[b], bit_mask = 0x08, l = 4; l --> 0; bit_mask >>= 1)
               {
                  if(!(stripe[k].sigma & bit_mask))
                  {
                     k_h = (stripe[k].stripe_l->sigma & bit_mask) + (stripe[k].stripe_r->sigma & bit_mask);

                     k_v = ((bit_mask & (stripe[k].sigma >> 1)) + (bit_mask & (stripe[k].sigma << 1)));

                     k_d = ((bit_mask & (stripe[k].stripe_l->sigma >> 1)) + (bit_mask & (stripe[k].stripe_l->sigma << 1)) +
                            (bit_mask & (stripe[k].stripe_r->sigma >> 1)) + (bit_mask & (stripe[k].stripe_r->sigma << 1)));

                     if(l == 3)
                     {
                        k_v += ((0x01 & stripe[k].stripe_u->sigma) << 3);
                        k_d += ((0x01 & stripe[k].stripe_u->stripe_l->sigma) + 
                                (0x01 & stripe[k].stripe_u->stripe_r->sigma)) << 3;
                     }
                     else if(l == 0)
                     {
                        k_v += ((0x08 & stripe[k].stripe_d->sigma) >> 3);
                        k_d += ((0x08 & stripe[k].stripe_d->stripe_l->sigma) + 
                                (0x08 & stripe[k].stripe_d->stripe_r->sigma)) >> 3;
                     }

                     k_sig = ((15 * k_h + 5 * k_v + k_d) >> l);

                     if(k_sig)
                     {
                        mq_bit_encode(coder->bitcoder, (uchar)((bit >> l) & 0x01), coder->sig2context[k_sig]);
                        
                        if(bit & bit_mask)
                        {
                           xi_h = ((bit_mask & stripe[k].stripe_l->sigma)       | ((bit_mask & stripe[k].stripe_l->xi) << 2) |
                                  ((bit_mask & stripe[k].stripe_r->sigma) << 1) | ((bit_mask & stripe[k].stripe_r->xi) << 3)) >> l;
                           
                           xi_v = ((bit_mask & (stripe[k].sigma >> 1))       | ((bit_mask & (stripe[k].xi >> 1)) << 2) |
                                  ((bit_mask & (stripe[k].sigma << 1)) << 1) | ((bit_mask & (stripe[k].xi << 1)) << 3)) >> l;

                           if(l == 3)
                           {
                              xi_v |= ((0x01 & stripe[k].stripe_u->sigma) | ((0x01 & stripe[k].stripe_u->xi) << 2));
                           }
                           else if(l == 0)
                           {
                              xi_v |= (((0x08 & stripe[k].stripe_d->sigma) >> 2) | (0x08 & stripe[k].stripe_d->xi));
                           }

                           xi_h = SIG2XI[xi_h];
                           xi_v = SIG2XI[xi_v];
                     
                           mq_bit_encode(coder->bitcoder, (uchar)(((bit_mask & stripe[k].xi) >> l)^XI2CONT[(xi_h << 2) | xi_v][1]),
                                                                                                   XI2CONT[(xi_h << 2) | xi_v][0]);

                           mse             += DISTORTION_TS_LUT[((stripe[k].sample[l] >> dist_shift) << dist_corr) & 0x1F];
                           stripe[k].sigma |= bit_mask;
                        }

                        stripe[k].pi |= bit_mask;
                     }
                  }
                  else
                  {
                     stripe[k].pi ^= (stripe[k].pi & bit_mask);
                  }
               }
            }
            else
            {
               stripe[k].pi = 0;
            }
         }
      }

      if(rest < 4)
      {
         for(x = 0; x < coder->width; ++x, ++k)
         {
            if((stripe[k].sigma^0xF) & stripe_mask)
            {
               for(bit = stripe[k].bit[b], bit_mask = 0x08, l = 4; l --> rest; bit_mask >>= 1)
               {
                  if(!(stripe[k].sigma & bit_mask))
                  {
                     k_h = (stripe[k].stripe_l->sigma & bit_mask) + (stripe[k].stripe_r->sigma & bit_mask);

                     k_v = ((bit_mask & (stripe[k].sigma >> 1)) + (bit_mask & (stripe[k].sigma << 1)));

                     k_d = ((bit_mask & (stripe[k].stripe_l->sigma >> 1)) + (bit_mask & (stripe[k].stripe_l->sigma << 1)) +
                            (bit_mask & (stripe[k].stripe_r->sigma >> 1)) + (bit_mask & (stripe[k].stripe_r->sigma << 1)));

                     if(l == 3)
                     {
                        k_v += ((0x01 & stripe[k].stripe_u->sigma) << 3);
                        k_d += ((0x01 & stripe[k].stripe_u->stripe_l->sigma) + 
                                (0x01 & stripe[k].stripe_u->stripe_r->sigma)) << 3;
                     }

                     k_sig = ((15 * k_h + 5 * k_v + k_d) >> l);

                     if(k_sig && (stripe[k].sigma^bit_mask))
                     {
                        mq_bit_encode(coder->bitcoder, (uchar)((bit >> l) & 0x01), coder->sig2context[k_sig]);
                        
                        if(bit & bit_mask)
                        {
                           xi_h = ((bit_mask & stripe[k].stripe_l->sigma)       | ((bit_mask & stripe[k].stripe_l->xi) << 2) |
                                  ((bit_mask & stripe[k].stripe_r->sigma) << 1) | ((bit_mask & stripe[k].stripe_r->xi) << 3)) >> l;
                           
                           xi_v = ((bit_mask & (stripe[k].sigma >> 1))       | ((bit_mask & (stripe[k].xi >> 1)) << 2) |
                                  ((bit_mask & (stripe[k].sigma << 1)) << 1) | ((bit_mask & (stripe[k].xi << 1)) << 3)) >> l;

                           if(l == 3)
                           {
                              xi_v |= ((0x01 & stripe[k].stripe_u->sigma) | ((0x01 & stripe[k].stripe_u->xi) << 2));
                           }

                           xi_h = SIG2XI[xi_h];
                           xi_v = SIG2XI[xi_v];
                     
                           mq_bit_encode(coder->bitcoder, (uchar)(((bit_mask & stripe[k].xi) >> l)^XI2CONT[(xi_h << 2) | xi_v][1]),
                                                                                                   XI2CONT[(xi_h << 2) | xi_v][0]);

                           mse             += DISTORTION_TS_LUT[((stripe[k].sample[l] >> dist_shift) << dist_corr) & 0x1F];
                           stripe[k].sigma |= bit_mask;
                        }

                        stripe[k].pi |= bit_mask;
                     }
                  }
                  else
                  {
                     stripe[k].pi ^= (stripe[k].pi & bit_mask);
                  }
               }
            }
            else
            {
               stripe[k].pi = 0;
            }
         }
      }
   }
   return mse;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void *test(void)                                                                          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                -                           -                       -                                       !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                -           Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uint64
magnitude_refinement_enc_pass(bwc_coder *const coder, const int8 b)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   i, j;
   uint64   k, x;
   int64    mse;
   uint16   k_mag;
   uint8    bit_mask, stripe_mask;
   uint8    dist_shift, dist_corr, l;
   uint8    bit, sig;
   uint8    rest;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_coder_stripe   *stripe;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(coder);

   mse         = 0;
   stripe      = coder->data;
   rest        = 4 - (coder->height & 0x03);
   stripe_mask = ~((0x0F >> ((coder->height & 0x03))) | 0xF0);
   dist_shift  = (b < DISTORTION_MAG) ? 0 : (b - DISTORTION_MAG);
   dist_corr   = (b < DISTORTION_MAG) ? (DISTORTION_MAG - b) : 0;

   for(i = 0, k = 0; i < coder->no_slice; ++i)
   {
      for(j = 0; j < coder->no_full_stripe; ++j)
      {
         for(x = 0; x < coder->width; ++x, ++k)
         {
            sig = stripe[k].sigma & (~stripe[k].pi);

            for(bit = stripe[k].bit[b], bit_mask = 0x08, l = 4; l --> 0; bit_mask >>= 1)
            {
               if(sig & bit_mask)
               {
                  k_mag      = CONTEXT_MAG;

                  if(stripe[k].delta & bit_mask)
                  {
                     k_mag += 2;
                  }
                  else
                  {
                     if((bit_mask & (stripe[k].sigma >> 1))           || (bit_mask & (stripe[k].sigma << 1))            ||
                        (stripe[k].stripe_l->sigma & bit_mask)        || (stripe[k].stripe_r->sigma & bit_mask)         ||
                        (bit_mask & (stripe[k].stripe_l->sigma >> 1)) || (bit_mask & (stripe[k].stripe_l->sigma << 1))  ||
                        (bit_mask & (stripe[k].stripe_r->sigma >> 1)) || (bit_mask & (stripe[k].stripe_r->sigma << 1)))
                     {
                        k_mag += 1;
                     }
                     else if((l == 3) && ((0x08 & (stripe[k].stripe_u->sigma << 3))
                                      ||  (0x08 & (stripe[k].stripe_u->stripe_l->sigma << 3))
                                      ||  (0x08 & (stripe[k].stripe_u->stripe_r->sigma << 3))))
                     {
                        k_mag += 1;
                     }
                     else if((l == 0) && ((0x01 & (stripe[k].stripe_d->sigma >> 3))
                                      ||  (0x01 & (stripe[k].stripe_d->stripe_l->sigma >> 3))
                                      ||  (0x01 & (stripe[k].stripe_d->stripe_r->sigma >> 3))))
                     {
                        k_mag += 1;
                     }
                  }

                  mq_bit_encode(coder->bitcoder, (uchar)((bit >> l) & 0x01), k_mag);

                  mse             += DISTORTION_TM_LUT[((stripe[k].sample[l] >> dist_shift) << dist_corr) & 0x3F];
                  stripe[k].delta |= bit_mask;
               }
            }
         }
      }

      if(rest < 4)
      {
         for(x = 0; x < coder->width; ++x, ++k)
         {
            sig      = (stripe[k].sigma & (~stripe[k].pi)) & stripe_mask;

            for(bit = stripe[k].bit[b], bit_mask = 0x08, l = 4; l --> rest; bit_mask >>= 1)
            {  
               if(sig & bit_mask)
               {
                  k_mag      = CONTEXT_MAG;

                  if(stripe[k].delta & bit_mask)
                  {
                     k_mag += 2;
                  }
                  else
                  {
                     if((bit_mask & (stripe[k].sigma >> 1))           || (bit_mask & (stripe[k].sigma << 1))            ||
                        (stripe[k].stripe_l->sigma & bit_mask)        || (stripe[k].stripe_r->sigma & bit_mask)         ||
                        (bit_mask & (stripe[k].stripe_l->sigma >> 1)) || (bit_mask & (stripe[k].stripe_l->sigma << 1))  ||
                        (bit_mask & (stripe[k].stripe_r->sigma >> 1)) || (bit_mask & (stripe[k].stripe_r->sigma << 1)))
                     {
                        k_mag += 1;
                     }
                     else if((l == 3) && ((0x08 & (stripe[k].stripe_u->sigma << 3))
                                      ||  (0x08 & (stripe[k].stripe_u->stripe_l->sigma << 3))
                                      ||  (0x08 & (stripe[k].stripe_u->stripe_r->sigma << 3))))
                     {
                        k_mag += 1;
                     }
                  }

                  mq_bit_encode(coder->bitcoder, (uchar)((bit >> l) & 0x01), k_mag);

                  mse             += DISTORTION_TM_LUT[((stripe[k].sample[l] >> dist_shift) << dist_corr) & 0x3F];
                  stripe[k].delta |= bit_mask;
               }
            }
         }
      }
   }
   return mse;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void *test(void)                                                                          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                -                           -                       -                                       !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                -           Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uint64
cleanup_enc_pass(bwc_coder *const coder, const int8 b)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   i, j;
   uint64   k, x;
   int64    mse;
   uint16   k_h, k_v, k_d;
   uint16   xi_h, xi_v;
   uint8    bit_mask, l;
   uint8    dist_shift, dist_corr;
   uint8    bit;
   uint8    rest;
   int8     r;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_coder_stripe   *stripe;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(coder);

   mse        = 0;
   stripe     = coder->data;
   rest       = 4 - (coder->height & 0x03);
   dist_shift  = (b < DISTORTION_SIG) ? 0 : (b - DISTORTION_SIG);
   dist_corr   = (b < DISTORTION_SIG) ? (DISTORTION_SIG - b) : 0;

   for(i = 0, k = 0; i < coder->no_slice; ++i)
   {
      for(j = 0; j < coder->no_full_stripe; ++j)
      {
         for(x = 0; x < coder->width; ++x, ++k)
         {
            r = -1;

            if(!(0x08 & stripe[k].stripe_d->stripe_l->sigma) && !(0x08 & stripe[k].stripe_d->sigma) &&
               !(0x08 & stripe[k].stripe_d->stripe_r->sigma) && !(0x01 & stripe[k].stripe_u->sigma) &&
               !(0x01 & stripe[k].stripe_u->stripe_l->sigma) &&         !stripe[k].stripe_l->sigma  &&
               !(0x01 & stripe[k].stripe_u->stripe_r->sigma) &&         !stripe[k].stripe_r->sigma  &&
                                                                        !stripe[k].sigma)
            {
               bit = stripe[k].bit[b];
               if(bit)
               {
                  r  = bit | (bit >> 1);
                  r |= r >> 2;
                  r  = DISTORTION_LZ_LUT[r];

                  mq_bit_encode(coder->bitcoder, (uchar)1,      CONTEXT_RUN);
                  mq_bit_encode(coder->bitcoder, (uchar)r >> 1, CONTEXT_UNI);
                  mq_bit_encode(coder->bitcoder, (uchar)r&0x01, CONTEXT_UNI);
               }
               else
               {
                  r = 4;
                  mq_bit_encode(coder->bitcoder, (uchar)0,      CONTEXT_RUN);
               }
            }
            
            for(bit = stripe[k].bit[b], bit_mask = 0x08, l = 4; l --> 0; bit_mask >>= 1)
            {
               if(!(stripe[k].sigma & bit_mask) && !(stripe[k].pi & bit_mask))
               {
                  if(r >= 0)
                  {
                     r--;
                  }
                  else
                  {
                     k_h = (stripe[k].stripe_l->sigma & bit_mask) + (stripe[k].stripe_r->sigma & bit_mask);

                     k_v = ((bit_mask & (stripe[k].sigma >> 1)) + (bit_mask & (stripe[k].sigma << 1)));

                     k_d = ((bit_mask & (stripe[k].stripe_l->sigma >> 1)) + (bit_mask & (stripe[k].stripe_l->sigma << 1)) +
                            (bit_mask & (stripe[k].stripe_r->sigma >> 1)) + (bit_mask & (stripe[k].stripe_r->sigma << 1)));

                     if(l == 3)
                     {
                        k_v += ((0x01 & stripe[k].stripe_u->sigma) << 3);
                        k_d += ((0x01 & stripe[k].stripe_u->stripe_l->sigma) + 
                                (0x01 & stripe[k].stripe_u->stripe_r->sigma)) << 3;
                     }
                     else if(l == 0)
                     {
                        k_v += ((0x08 & stripe[k].stripe_d->sigma) >> 3);
                        k_d += ((0x08 & stripe[k].stripe_d->stripe_l->sigma) + 
                                (0x08 & stripe[k].stripe_d->stripe_r->sigma)) >> 3;
                     }

                     mq_bit_encode(coder->bitcoder, (uchar)((bit >> l) & 0x01), coder->sig2context[(15 * k_h + 5 * k_v + k_d) >> l]);
                  }
                  if(bit & bit_mask)
                  {
                     xi_h = ((bit_mask & stripe[k].stripe_l->sigma)       | ((bit_mask & stripe[k].stripe_l->xi) << 2) |
                            ((bit_mask & stripe[k].stripe_r->sigma) << 1) | ((bit_mask & stripe[k].stripe_r->xi) << 3)) >> l;
                     
                     xi_v = ((bit_mask & (stripe[k].sigma >> 1))       | ((bit_mask & (stripe[k].xi >> 1)) << 2) |
                            ((bit_mask & (stripe[k].sigma << 1)) << 1) | ((bit_mask & (stripe[k].xi << 1)) << 3)) >> l;

                     if(l == 3)
                     {
                        xi_v |= ((0x01 & stripe[k].stripe_u->sigma) | ((0x01 & stripe[k].stripe_u->xi) << 2));
                     }
                     else if(l == 0)
                     {
                        xi_v |= (((0x08 & stripe[k].stripe_d->sigma) >> 2) | (0x08 & stripe[k].stripe_d->xi));
                     }

                     xi_h = SIG2XI[xi_h];
                     xi_v = SIG2XI[xi_v];

                     mq_bit_encode(coder->bitcoder, (uchar)(((bit_mask & stripe[k].xi) >> l)^XI2CONT[(xi_h << 2) | xi_v][1]),
                                                                                             XI2CONT[(xi_h << 2) | xi_v][0]);

                     mse             += DISTORTION_TS_LUT[((stripe[k].sample[l] >> dist_shift) << dist_corr) & 0x1F];
                     stripe[k].sigma |= bit_mask;
                  }
               }
            }
         }
      }

      if(rest < 4)
      {
         for(x = 0; x < coder->width; ++x, ++k)
         {
            for(bit = stripe[k].bit[b], bit_mask = 0x08, l = 4; l --> rest; bit_mask >>= 1)
            {
               if(!(stripe[k].sigma & bit_mask) && !(stripe[k].pi & bit_mask))
               {
                  k_h = (stripe[k].stripe_l->sigma & bit_mask) + (stripe[k].stripe_r->sigma & bit_mask);

                  k_v = ((bit_mask & (stripe[k].sigma >> 1)) + (bit_mask & (stripe[k].sigma << 1)));

                  k_d = ((bit_mask & (stripe[k].stripe_l->sigma >> 1)) + (bit_mask & (stripe[k].stripe_l->sigma << 1)) +
                         (bit_mask & (stripe[k].stripe_r->sigma >> 1)) + (bit_mask & (stripe[k].stripe_r->sigma << 1)));

                  if(l == 3)
                  {
                     k_v += ((0x01 & stripe[k].stripe_u->sigma) << 3);
                     k_d += ((0x01 & stripe[k].stripe_u->stripe_l->sigma) + 
                             (0x01 & stripe[k].stripe_u->stripe_r->sigma)) << 3;
                  }

                  mq_bit_encode(coder->bitcoder, (uchar)((bit >> l) & 0x01), coder->sig2context[(15 * k_h + 5 * k_v + k_d) >> l]);

                  if(bit & bit_mask)
                  {
                     xi_h = ((bit_mask & stripe[k].stripe_l->sigma)       | ((bit_mask & stripe[k].stripe_l->xi) << 2) |
                            ((bit_mask & stripe[k].stripe_r->sigma) << 1) | ((bit_mask & stripe[k].stripe_r->xi) << 3)) >> l;
                     
                     xi_v = ((bit_mask & (stripe[k].sigma >> 1))       | ((bit_mask & (stripe[k].xi >> 1)) << 2) |
                            ((bit_mask & (stripe[k].sigma << 1)) << 1) | ((bit_mask & (stripe[k].xi << 1)) << 3)) >> l;

                     if(l == 3)
                     {
                        xi_v |= ((0x01 & stripe[k].stripe_u->sigma) | ((0x01 & stripe[k].stripe_u->xi) << 2));
                     }

                     xi_h = SIG2XI[xi_h];
                     xi_v = SIG2XI[xi_v];
               
                     mq_bit_encode(coder->bitcoder, (uchar)(((bit_mask & stripe[k].xi) >> l)^XI2CONT[(xi_h << 2) | xi_v][1]),
                                                                                             XI2CONT[(xi_h << 2) | xi_v][0]);

                     mse             += DISTORTION_TS_LUT[((stripe[k].sample[l] >> dist_shift) << dist_corr) & 0x1F];
                     stripe[k].sigma |= bit_mask;
                  }
               }
            }
         }
      }
   }
   return mse;
}


/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void *test(void)                                                                          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                -                           -                       -                                       !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                -           Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
significance_propagation_dec_pass(bwc_coder *const coder, const int8 b)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   i, j;
   uint64   k, x;
   uint16   k_h, k_v, k_d;
   uint16   k_sig;
   uint16   xi_h, xi_v;
   uint8    bit_mask, stripe_mask;
   uint8    l;
   uint8    bit;
   uint8    rest;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_coder_stripe   *stripe;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(coder);

   stripe      = coder->data;
   stripe_mask = ~((0x0F >> ((coder->height & 0x03))) | 0xF0);
   rest        = 4 - (coder->height & 0x03);

   for(i = 0, k = 0; i < coder->no_slice; ++i)
   {
      for(j = 0; j < coder->no_full_stripe; ++j)
      {
         for(x = 0; x < coder->width; ++x, ++k)
         {
            if(stripe[k].sigma^0xF)
            {
               for(bit_mask = 0x08, l = 4; l --> 0; bit_mask >>= 1)
               {
                  if(!(stripe[k].sigma & bit_mask))
                  {
                     k_h = (stripe[k].stripe_l->sigma & bit_mask) + (stripe[k].stripe_r->sigma & bit_mask);

                     k_v = ((bit_mask & (stripe[k].sigma >> 1)) + (bit_mask & (stripe[k].sigma << 1)));

                     k_d = ((bit_mask & (stripe[k].stripe_l->sigma >> 1)) + (bit_mask & (stripe[k].stripe_l->sigma << 1)) +
                            (bit_mask & (stripe[k].stripe_r->sigma >> 1)) + (bit_mask & (stripe[k].stripe_r->sigma << 1)));

                     if(l == 3)
                     {
                        k_v += ((0x01 & stripe[k].stripe_u->sigma) << 3);
                        k_d += ((0x01 & stripe[k].stripe_u->stripe_l->sigma) + 
                                (0x01 & stripe[k].stripe_u->stripe_r->sigma)) << 3;
                     }
                     else if(l == 0)
                     {
                        k_v += ((0x08 & stripe[k].stripe_d->sigma) >> 3);
                        k_d += ((0x08 & stripe[k].stripe_d->stripe_l->sigma) + 
                                (0x08 & stripe[k].stripe_d->stripe_r->sigma)) >> 3;
                     }

                     k_sig = ((15 * k_h + 5 * k_v + k_d) >> l);

                     if(k_sig)
                     {
                        bit = mq_bit_decode(coder->bitcoder, coder->sig2context[k_sig]);
                        
                        if(bit)
                        {
                           stripe[k].bit[b] |= (bit << l);

                           xi_h = ((bit_mask & stripe[k].stripe_l->sigma)       | ((bit_mask & stripe[k].stripe_l->xi) << 2) |
                                  ((bit_mask & stripe[k].stripe_r->sigma) << 1) | ((bit_mask & stripe[k].stripe_r->xi) << 3)) >> l;
                           
                           xi_v = ((bit_mask & (stripe[k].sigma >> 1))       | ((bit_mask & (stripe[k].xi >> 1)) << 2) |
                                  ((bit_mask & (stripe[k].sigma << 1)) << 1) | ((bit_mask & (stripe[k].xi << 1)) << 3)) >> l;

                           if(l == 3)
                           {
                              xi_v |= ((0x01 & stripe[k].stripe_u->sigma) | ((0x01 & stripe[k].stripe_u->xi) << 2));
                           }
                           else if(l == 0)
                           {
                              xi_v |= (((0x08 & stripe[k].stripe_d->sigma) >> 2) | (0x08 & stripe[k].stripe_d->xi));
                           }

                           xi_h = SIG2XI[xi_h];
                           xi_v = SIG2XI[xi_v];

                           stripe[k].xi |= ((XI2CONT[(xi_h << 2) | xi_v][1] ^ mq_bit_decode(coder->bitcoder, XI2CONT[(xi_h << 2) | xi_v][0])) << l);

                           stripe[k].sigma |= bit_mask;
                        }

                        stripe[k].pi |= bit_mask;
                     }
                  }
                  else
                  {
                     stripe[k].pi ^= (stripe[k].pi & bit_mask);
                  }
               }
            }
            else
            {
               stripe[k].pi = 0;
            }
         }
      }

      if(rest < 4)
      {
         for(x = 0; x < coder->width; ++x, ++k)
         {
            if((stripe[k].sigma^0xF) & stripe_mask)
            {
               for(bit = stripe[k].bit[b], bit_mask = 0x08, l = 4; l --> rest; bit_mask >>= 1)
               {
                  if(!(stripe[k].sigma & bit_mask))
                  {
                     k_h = (stripe[k].stripe_l->sigma & bit_mask) + (stripe[k].stripe_r->sigma & bit_mask);

                     k_v = ((bit_mask & (stripe[k].sigma >> 1)) + (bit_mask & (stripe[k].sigma << 1)));

                     k_d = ((bit_mask & (stripe[k].stripe_l->sigma >> 1)) + (bit_mask & (stripe[k].stripe_l->sigma << 1)) +
                            (bit_mask & (stripe[k].stripe_r->sigma >> 1)) + (bit_mask & (stripe[k].stripe_r->sigma << 1)));

                     if(l == 3)
                     {
                        k_v += ((0x01 & stripe[k].stripe_u->sigma) << 3);
                        k_d += ((0x01 & stripe[k].stripe_u->stripe_l->sigma) + 
                                (0x01 & stripe[k].stripe_u->stripe_r->sigma)) << 3;
                     }

                     k_sig = ((15 * k_h + 5 * k_v + k_d) >> l);

                     if(k_sig && (stripe[k].sigma^bit_mask))
                     {
                        bit = mq_bit_decode(coder->bitcoder, coder->sig2context[k_sig]);
                        
                        if(bit)
                        {
                           stripe[k].bit[b] |= (bit << l);

                           xi_h = ((bit_mask & stripe[k].stripe_l->sigma)       | ((bit_mask & stripe[k].stripe_l->xi) << 2) |
                                  ((bit_mask & stripe[k].stripe_r->sigma) << 1) | ((bit_mask & stripe[k].stripe_r->xi) << 3)) >> l;
                           
                           xi_v = ((bit_mask & (stripe[k].sigma >> 1))       | ((bit_mask & (stripe[k].xi >> 1)) << 2) |
                                  ((bit_mask & (stripe[k].sigma << 1)) << 1) | ((bit_mask & (stripe[k].xi << 1)) << 3)) >> l;

                           if(l == 3)
                           {
                              xi_v |= ((0x01 & stripe[k].stripe_u->sigma) | ((0x01 & stripe[k].stripe_u->xi) << 2));
                           }

                           xi_h = SIG2XI[xi_h];
                           xi_v = SIG2XI[xi_v];
                     
                           stripe[k].xi |= ((XI2CONT[(xi_h << 2) | xi_v][1] ^ mq_bit_decode(coder->bitcoder, XI2CONT[(xi_h << 2) | xi_v][0])) << l);

                           stripe[k].sigma |= bit_mask;
                        }

                        stripe[k].pi |= bit_mask;
                     }
                  }
                  else
                  {
                     stripe[k].pi ^= (stripe[k].pi & bit_mask);
                  }
               }
            }
            else
            {
               stripe[k].pi = 0;
            }
         }
      }
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void *test(void)                                                                          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                -                           -                       -                                       !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                -           Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
magnitude_refinement_dec_pass(bwc_coder *const coder, const int8 b)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   i, j;
   uint64   k, x;
   uint16   k_mag;
   uint8    bit_mask, stripe_mask;
   uint8    l;
   uint8    sig;
   uint8    rest;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_coder_stripe   *stripe;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(coder);

   stripe     = coder->data;
   stripe_mask= ~((0x0F >> ((coder->height & 0x03))) | 0xF0);
   rest       = 4 - (coder->height & 0x03);

   for(i = 0, k = 0; i < coder->no_slice; ++i)
   {
      for(j = 0; j < coder->no_full_stripe; ++j)
      {
         for(x = 0; x < coder->width; ++x, ++k)
         {
            sig = stripe[k].sigma & (~stripe[k].pi);

            for(bit_mask = 0x08, l = 4; l --> 0; bit_mask >>= 1)
            {
               if(sig & bit_mask)
               {
                  k_mag      = CONTEXT_MAG;

                  if(stripe[k].delta & bit_mask)
                  {
                     k_mag += 2;
                  }
                  else
                  {
                     if((bit_mask & (stripe[k].sigma >> 1))           || (bit_mask & (stripe[k].sigma << 1))            ||
                        (stripe[k].stripe_l->sigma & bit_mask)        || (stripe[k].stripe_r->sigma & bit_mask)         ||
                        (bit_mask & (stripe[k].stripe_l->sigma >> 1)) || (bit_mask & (stripe[k].stripe_l->sigma << 1))  ||
                        (bit_mask & (stripe[k].stripe_r->sigma >> 1)) || (bit_mask & (stripe[k].stripe_r->sigma << 1)))
                     {
                        k_mag += 1;
                     }
                     else if((l == 3) && ((0x08 & (stripe[k].stripe_u->sigma << 3))
                                      ||  (0x08 & (stripe[k].stripe_u->stripe_l->sigma << 3))
                                      ||  (0x08 & (stripe[k].stripe_u->stripe_r->sigma << 3))))
                     {
                        k_mag += 1;
                     }
                     else if((l == 0) && ((0x01 & (stripe[k].stripe_d->sigma >> 3))
                                      ||  (0x01 & (stripe[k].stripe_d->stripe_l->sigma >> 3))
                                      ||  (0x01 & (stripe[k].stripe_d->stripe_r->sigma >> 3))))
                     {
                        k_mag += 1;
                     }
                  }

                  stripe[k].bit[b] |= (mq_bit_decode(coder->bitcoder, k_mag) << l);

                  stripe[k].delta |= bit_mask;
               }
            }
         }
      }

      if(rest < 4)
      {
         for(x = 0; x < coder->width; ++x, ++k)
         {
            sig      = (stripe[k].sigma & (~stripe[k].pi)) & stripe_mask;

            for(bit_mask = 0x08, l = 4; l --> rest; bit_mask >>= 1)
            {  
               if(sig & bit_mask)
               {
                  k_mag      = CONTEXT_MAG;

                  if(stripe[k].delta & bit_mask)
                  {
                     k_mag += 2;
                  }
                  else
                  {
                     if((bit_mask & (stripe[k].sigma >> 1))           || (bit_mask & (stripe[k].sigma << 1))            ||
                        (stripe[k].stripe_l->sigma & bit_mask)        || (stripe[k].stripe_r->sigma & bit_mask)         ||
                        (bit_mask & (stripe[k].stripe_l->sigma >> 1)) || (bit_mask & (stripe[k].stripe_l->sigma << 1))  ||
                        (bit_mask & (stripe[k].stripe_r->sigma >> 1)) || (bit_mask & (stripe[k].stripe_r->sigma << 1)))
                     {
                        k_mag += 1;
                     }
                     else if((l == 3) && ((0x08 & (stripe[k].stripe_u->sigma << 3))
                                      ||  (0x08 & (stripe[k].stripe_u->stripe_l->sigma << 3))
                                      ||  (0x08 & (stripe[k].stripe_u->stripe_r->sigma << 3))))
                     {
                        k_mag += 1;
                     }
                  }

                  stripe[k].bit[b] |= (mq_bit_decode(coder->bitcoder, k_mag) << l);

                  stripe[k].delta |= bit_mask;
               }
            }
         }
      }
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void *test(void)                                                                          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                -                           -                       -                                       !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                -           Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
cleanup_dec_pass(bwc_coder *const coder, const int8 b)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   i, j;
   uint64   k, x;
   uint16   k_h, k_v, k_d;
   uint16   xi_h, xi_v;
   uint8    bit_mask, l;
   uint8    bit;
   uint8    rest;
   int8     r;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_coder_stripe   *stripe;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(coder);

   rest       = 4 - (coder->height & 0x03);
   stripe     = coder->data;

   for(i = 0, k = 0; i < coder->no_slice; ++i)
   {
      for(j = 0; j < coder->no_full_stripe; ++j)
      {
         for(x = 0; x < coder->width; ++x, ++k)
         {
            bit = 0;
            r   = -1;

            if(!(0x08 & stripe[k].stripe_d->stripe_l->sigma) && !(0x08 & stripe[k].stripe_d->sigma) &&
               !(0x08 & stripe[k].stripe_d->stripe_r->sigma) && !(0x01 & stripe[k].stripe_u->sigma) &&
               !(0x01 & stripe[k].stripe_u->stripe_l->sigma) &&         !stripe[k].stripe_l->sigma  &&
               !(0x01 & stripe[k].stripe_u->stripe_r->sigma) &&         !stripe[k].stripe_r->sigma  &&
                                                                        !stripe[k].sigma)
            {
               if(mq_bit_decode(coder->bitcoder, CONTEXT_RUN))
               {
                  r    = mq_bit_decode(coder->bitcoder, CONTEXT_UNI);
                  r    = (r << 1) + mq_bit_decode(coder->bitcoder, CONTEXT_UNI);
                  bit |= (0x08 >> r);
               }
               else
               {
                  r = 4;
               }
            }
            
            for(bit_mask = 0x08, l = 4; l --> 0; bit_mask >>= 1)
            {
               if(!(stripe[k].sigma & bit_mask) && !(stripe[k].pi & bit_mask))
               {
                  if(r >= 0)
                  {
                     r--;
                  }
                  else
                  {
                     k_h = (stripe[k].stripe_l->sigma & bit_mask) + (stripe[k].stripe_r->sigma & bit_mask);

                     k_v = ((bit_mask & (stripe[k].sigma >> 1)) + (bit_mask & (stripe[k].sigma << 1)));

                     k_d = ((bit_mask & (stripe[k].stripe_l->sigma >> 1)) + (bit_mask & (stripe[k].stripe_l->sigma << 1)) +
                            (bit_mask & (stripe[k].stripe_r->sigma >> 1)) + (bit_mask & (stripe[k].stripe_r->sigma << 1)));

                     if(l == 3)
                     {
                        k_v += ((0x01 & stripe[k].stripe_u->sigma) << 3);
                        k_d += ((0x01 & stripe[k].stripe_u->stripe_l->sigma) + 
                                (0x01 & stripe[k].stripe_u->stripe_r->sigma)) << 3;
                     }
                     else if(l == 0)
                     {
                        k_v += ((0x08 & stripe[k].stripe_d->sigma) >> 3);
                        k_d += ((0x08 & stripe[k].stripe_d->stripe_l->sigma) + 
                                (0x08 & stripe[k].stripe_d->stripe_r->sigma)) >> 3;
                     }

                     bit |= (mq_bit_decode(coder->bitcoder, coder->sig2context[(15 * k_h + 5 * k_v + k_d) >> l]) << l);
                  }
                  if(bit & bit_mask)
                  {
                     xi_h = ((bit_mask & stripe[k].stripe_l->sigma)       | ((bit_mask & stripe[k].stripe_l->xi) << 2) |
                            ((bit_mask & stripe[k].stripe_r->sigma) << 1) | ((bit_mask & stripe[k].stripe_r->xi) << 3)) >> l;
                     
                     xi_v = ((bit_mask & (stripe[k].sigma >> 1))       | ((bit_mask & (stripe[k].xi >> 1)) << 2) |
                            ((bit_mask & (stripe[k].sigma << 1)) << 1) | ((bit_mask & (stripe[k].xi << 1)) << 3)) >> l;

                     if(l == 3)
                     {
                        xi_v |= ((0x01 & stripe[k].stripe_u->sigma) | ((0x01 & stripe[k].stripe_u->xi) << 2));
                     }
                     else if(l == 0)
                     {
                        xi_v |= (((0x08 & stripe[k].stripe_d->sigma) >> 2) | (0x08 & stripe[k].stripe_d->xi));
                     }

                     xi_h = SIG2XI[xi_h];
                     xi_v = SIG2XI[xi_v];
               
                     stripe[k].xi |= ((XI2CONT[(xi_h << 2) | xi_v][1] ^ mq_bit_decode(coder->bitcoder, XI2CONT[(xi_h << 2) | xi_v][0])) << l);

                     stripe[k].sigma |= bit_mask;
                  }
               }
            }
            stripe[k].bit[b] |= bit;
         }
      }

      if(rest < 4)
      {
         for(x = 0; x < coder->width; ++x, ++k)
         {
            bit = 0;

            for(bit_mask = 0x08, l = 4; l --> rest; bit_mask >>= 1)
            {
               if(!(stripe[k].sigma & bit_mask) && !(stripe[k].pi & bit_mask))
               {
                  k_h = (stripe[k].stripe_l->sigma & bit_mask) + (stripe[k].stripe_r->sigma & bit_mask);

                  k_v = ((bit_mask & (stripe[k].sigma >> 1)) + (bit_mask & (stripe[k].sigma << 1)));

                  k_d = ((bit_mask & (stripe[k].stripe_l->sigma >> 1)) + (bit_mask & (stripe[k].stripe_l->sigma << 1)) +
                         (bit_mask & (stripe[k].stripe_r->sigma >> 1)) + (bit_mask & (stripe[k].stripe_r->sigma << 1)));

                  if(l == 3)
                  {
                     k_v += ((0x01 & stripe[k].stripe_u->sigma) << 3);
                     k_d += ((0x01 & stripe[k].stripe_u->stripe_l->sigma) + 
                             (0x01 & stripe[k].stripe_u->stripe_r->sigma)) << 3;
                  }

                  bit |= (mq_bit_decode(coder->bitcoder, coder->sig2context[(15 * k_h + 5 * k_v + k_d) >> l]) << l);

                  if(bit & bit_mask)
                  {
                     xi_h = ((bit_mask & stripe[k].stripe_l->sigma)       | ((bit_mask & stripe[k].stripe_l->xi) << 2) |
                            ((bit_mask & stripe[k].stripe_r->sigma) << 1) | ((bit_mask & stripe[k].stripe_r->xi) << 3)) >> l;
                     
                     xi_v = ((bit_mask & (stripe[k].sigma >> 1))       | ((bit_mask & (stripe[k].xi >> 1)) << 2) |
                            ((bit_mask & (stripe[k].sigma << 1)) << 1) | ((bit_mask & (stripe[k].xi << 1)) << 3)) >> l;

                     if(l == 3)
                     {
                        xi_v |= ((0x01 & stripe[k].stripe_u->sigma) | ((0x01 & stripe[k].stripe_u->xi) << 2));
                     }

                     xi_h = SIG2XI[xi_h];
                     xi_v = SIG2XI[xi_v];
               
                     stripe[k].xi |= ((XI2CONT[(xi_h << 2) | xi_v][1] ^ mq_bit_decode(coder->bitcoder, XI2CONT[(xi_h << 2) | xi_v][0])) << l);

                     stripe[k].sigma |= bit_mask;
                  }
               }
            }
            stripe[k].bit[b] |= bit;
         }
      }
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void *test(void)                                                                          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                -                           -                       -                                       !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                07.03.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uint16
slope2log(const double lambda)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   exponent, mantissa, raw;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(lambda > 0);

   raw = *(uint64*)&lambda;

   exponent  =  (uint64)((raw & 0x7FF0000000000000) >> 44) - 0x41F00;
   mantissa  =  (uint64)((raw & 0x000FFFFFFFFFFFFF) >> 44);
   raw       =  (exponent + frac2log[mantissa]) + 0x10000;

   if(raw > 0xFFFF)
   {
      return 0xFFFF;
   }
   else if(raw < 2.0)
   {
      return 2;
   }
   else
   {
      return (uint16)raw;
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void *test(void)                                                                          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                -                           -                       -                                       !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                07.03.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
compute_convex_hull(bwc_encoded_cblk *const encoded_codeblock, double *const mse)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   delta_L;
   uint64  *Length;
   uint16  *Slope;
   uint8   *h;
   uint8    hull[MAXIMUM_NO_PASSES] = {0};
   uint8    hlast, i, j;

   /*-----------------------*\
   ! DEFINE FLOAT VARIABLES: !
   \*-----------------------*/
   double      delta_D;
   double      lambda[MAXIMUM_NO_PASSES + 1] = {0};

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(encoded_codeblock);

   Length      = encoded_codeblock->L;
   Slope       = encoded_codeblock->S;

   h           = hull;
   hlast       = 0;
   lambda [0]  = 0xFFFFFFFFFFFFFFFF;

   for(i = 0; i < encoded_codeblock->Z; ++i)
   {
      delta_D = 0;
      for(j = hlast; j <= i; ++j)
      {
         delta_D += mse[j];
      }
      delta_L = (hlast > 0) ? (Length[i] - Length[hlast - 1]) : Length[i];

      if(delta_D > 0)
      {
         while(delta_D >= (lambda[hlast] * delta_L))
         {
            lambda[hlast] = 0;            
            h--;
            hlast   = h[0];
            delta_D = 0;
            for(j = hlast; j <= i; ++j)
            {
               delta_D += mse[j];
            }
            delta_L = (hlast > 0) ? (Length[i] - Length[hlast - 1]) : Length[i];
         }

         h++;
         hlast = i + 1;
         h[0]  = hlast;

         if(delta_L == 0)
         {
            if(delta_D < 0)
            {
               lambda[hlast] = -1;
            }
            else if(delta_D == 0)
            {
               lambda[hlast] = 0;
            }
            else
            {
               lambda[hlast] = 0xFFFFFFFFFFFFFFFF;
            }
         }
         else
         {
            lambda[hlast] = delta_D / delta_L;
         }
      }
      else
      {
         lambda[i + 1] = 0;
      }
   }

   for(i = 0; i <= encoded_codeblock->Z; ++i)
   {
      if(lambda[i] < 0)
      {
         Slope[i] = 1;
      }
      else if (lambda[i] == 0)
      {
         if(i == encoded_codeblock->Z)
         {
            Slope[i] = 1;
         }
         else
         {
            Slope[i] = 0;
         }
      }
      else
      {
         Slope[i] = slope2log(lambda[i]);
      }
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: bwc_encoded_cblk* encode_codeblock(bwc_field *const field, bwc_cblk_access *const access, !
!   --------------                                                        bwc_coder_stripe *const codeblock, !
!                                                                                       const uint64  width, !
!                                                                                       const uint64 height, !
!                                                                                       const uint64  depth, !
!                                                                                       const uint16     dt) !
!                                                                                                            !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                -                           -                       -                                       !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                -           Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
encode_codeblock(bwc_field *const field, bwc_cblk_access  *const access,
                                      bwc_coder_stripe *const codeblock,
                                                    const uint64  width,
                                                    const uint64 height,
                                                    const uint64  depth,
                                                    const uint16     dt)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int64    delta_mse;
   int16    k;
   uint8    i, j;

   /*-----------------------*\
   ! DEFINE FLOAT VARIABLES: !
   \*-----------------------*/
   double  mse[MAXIMUM_NO_PASSES] = {0};
   double  mse_scale;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_ctrl       *control;
   bwc_subb_ctrl     *subb_ctrl;
   bwc_subb_inf      *subb_inf;
   bwc_cblk_ctrl     *cblk_ctrl;
   bwc_encoded_cblk  *encoded_cblk;
   bwc_coder          coder;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(access);
   assert(codeblock);
   assert(access->subband->control.highband_flag <= 15);
   assert(access->subband->control.Kmax >=
          access->codeblock->control.K);

   /*-----------------------*\
   ! DEFINE FUNCTION PTR:    !
   \*-----------------------*/
   uint64 (*coding_pass[3])(bwc_coder*, int8) = {significance_propagation_enc_pass,
                                                 magnitude_refinement_enc_pass, 
                                                 cleanup_enc_pass};

   /*--------------------------------------------------------*\
   ! Save the global, subband and codeblock control, subband  !
   ! info and encoded block structure to temporary variables  !
   ! to make the code more readable.                          !
   \*--------------------------------------------------------*/
   control       = &field->control;

   subb_ctrl     = &access->subband->control;
   subb_inf      = &access->subband->info;

   cblk_ctrl     = &access->codeblock->control;

   encoded_cblk  = access->codeblock->encoded_block;

   /*--------------------------------------------------------*\
   ! Check if there are any significant bits present in the   !
   ! current codeblock. If not, set the proper slope value    !
   ! and return to the function caller.                       !
   \*--------------------------------------------------------*/
   if(cblk_ctrl->K == 0)
   {
      encoded_cblk->S[0] = 0xFFFF;
      return;
   }

   /*--------------------------------------------------------*\
   ! Calculate the buffer increment and initial buffer size   !
   ! used to allocate memory that will hold the compressed    !
   ! bitstream and increment the buffer if the initial buffer !
   ! size is insufficient.                                    !
   \*--------------------------------------------------------*/
   coder.buff_incr      = width * height * depth * dt;
   coder.buff_size      = coder.buff_incr * PREC_BYTE;

   /*--------------------------------------------------------*\
   ! Save the height and width and calculate the number of    !
   ! full stripes and slices for the current codeblockblock.  !
   ! These variables are used to walk through the bwc_coder_  !
   ! stripe structure.                                        !
   \*--------------------------------------------------------*/
   coder.height         = height;
   coder.width          = width;

   coder.no_full_stripe = height >> 2;
   coder.no_slice       = depth * dt;

   /*--------------------------------------------------------*\
   ! Save the index of the first significant bitplane used to !
   ! walk through the sample bitplanes in the bwc_coder_      !
   ! stripe structure.                                        !
   \*--------------------------------------------------------*/
   coder.K              = cblk_ctrl->K;

   /*--------------------------------------------------------*\
   ! Save the error resilience and highband flag in the coder !
   ! structure. These flags are used to signal if an error    !
   ! resilient bitstream is to be created and which subband   !
   ! the current codeblock belongs to.                        !
   \*--------------------------------------------------------*/
   coder.erres          = control->error_resilience;
   coder.highband_flag  = subb_ctrl->highband_flag;

   /*--------------------------------------------------------*\
   ! Associate the significance-to-context pointer with the   !
   ! appropriate lookup table for the current subband.        !
   \*--------------------------------------------------------*/
   switch(coder.highband_flag)
   {
      case  0:
      case  2:
      case  4:
      case  6:
      case  8:
      case 10:
      case 12:
      case 14:
      {
         coder.sig2context = SIG2CONTEXT_L;
         break;
      }
      case  1:
      case  5:
      case  9:
      case 13:
      {
         coder.sig2context = SIG2CONTEXT_H;
         break;
      }
      case  3:
      case  7:
      case 11:
      case 15:
      {
         coder.sig2context = SIG2CONTEXT_HH;
         break;
      }
   }

   /*--------------------------------------------------------*\
   ! Associate the data pointer with the bwc_coder_stripe     !
   ! codeblock structure and allocate the memory block used   !
   ! to hold the compressed bitstream.                        !
   \*--------------------------------------------------------*/
   coder.data           = codeblock;
   coder.compressed     = calloc(coder.buff_size, sizeof(uchar));
   if(!coder.compressed)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return;
   }

   /*--------------------------------------------------------*\
   ! Initialize the entropy encoder used for the current com- !
   ! pression run.                                            !
   \*--------------------------------------------------------*/
   if(initialize_mq_encoder(&coder, CONTEXT_TOTAL))
   {
      return;
   }

   /*--------------------------------------------------------*\
   ! Calculate the weighting factor for the distrotion cal-   !
   ! culation according to the distortion contribution de-    !
   ! scribed in JPEG2000 by David S. Taubman and Michael W.   !
   ! Marcellin (p. 378).                                      !
   \*--------------------------------------------------------*/
   mse_scale  = subb_inf->dwt_gain;
   mse_scale *= (subb_ctrl->qt_step_size * subb_ctrl->qt_step_size);
   mse_scale *= pow(2, coder.K * 2);

   /*--------------------------------------------------------*\
   ! Scale the weighting factor to account for the integer    !
   ! representation of the distortion lookup tables.          !
   \*--------------------------------------------------------*/
   mse_scale /= 0x10000;

   for(i = 2, j = 0, k = coder.K; k --> 0; i = 0)
   {
      for(; i < 3; ++i, ++j)
      {
         /*--------------------------------------------------------*\
         ! Reset the bit encoder for the next coding pass.          !
         \*--------------------------------------------------------*/
         if(mq_next_run(coder.bitcoder))
         {
            return;
         }

         delta_mse = coding_pass[i](&coder, k);
         mse[j]    = delta_mse * mse_scale;
      }
      /*--------------------------------------------------------*\
      ! Evaluate the number of bytes generated so far by the bit !
      ! encoder and increase the buffer size if necessary.       !
      \*--------------------------------------------------------*/
      if(coder.buff_size <= (mq_get_no_bytes(coder.bitcoder) + coder.buff_incr))
      {
         coder.buff_size  += ((uint64)k >> 1) * coder.buff_incr;
         coder.compressed  = realloc(coder.compressed, coder.buff_size * sizeof(uchar));
         if(!coder.compressed)
         {
            // memory allocation error
            fprintf(stderr, MEMERROR);
            return;
         }

         mq_reset_ptr(coder.bitcoder, coder.compressed);
      }
      /*--------------------------------------------------------*\
      ! If the error resilience flag is set, encode a segmark    !
      ! symbol to allow for error detection during decoding.     !
      \*--------------------------------------------------------*/
      if(coder.erres)
      {
         encode_segmark(coder.bitcoder);
      }

      /*--------------------------------------------------------*\
      ! Scale the weighting factor to the next bitplane.         !
      \*--------------------------------------------------------*/
      mse_scale *= 0.25;
   }
   /*--------------------------------------------------------*\
   ! Evalute the number of bytes generated by the bit encoder !
   ! and increase the buffer size if necessary.               !
   \*--------------------------------------------------------*/
   if(coder.buff_size <= (mq_get_no_bytes(coder.bitcoder) + 16))
   {
      coder.buff_size  += 16;
      coder.compressed  = realloc(coder.compressed, coder.buff_size * sizeof(uchar));
      if(!coder.compressed)
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         return;
      }

      mq_reset_ptr(coder.bitcoder, coder.compressed);
   }
   /*--------------------------------------------------------*\
   ! Flush the remaining bits in the byte buffer to the coder !
   ! output and calculate the minimum truncation lengths.     !
   \*--------------------------------------------------------*/
   mq_termination(coder.bitcoder);

   /*--------------------------------------------------------*\
   ! Save the number of significant bitplanes, insignificant  !
   ! leading bitplanes and coding passes in the encoded code- !
   ! block structure.                                         !
   \*--------------------------------------------------------*/
   encoded_cblk->K     = coder.K;
   encoded_cblk->Kmsbs = subb_ctrl->Kmax - coder.K;
   encoded_cblk->Z     = j;

   /*--------------------------------------------------------*\
   ! Resize the coder output buffer to its optimal size and   !
   ! save the memory handle in the encoded codeblock struc-   !
   ! ture.                                                    !
   \*--------------------------------------------------------*/
   encoded_cblk->data  = realloc(coder.compressed, coder.bitcoder->state->L * sizeof(uchar));

   /*--------------------------------------------------------*\
   ! Save the lengths of the coding passes in the encoded     !
   ! codeblock structure.                                     !
   \*--------------------------------------------------------*/
   mq_get_pass_lengths(coder.bitcoder, encoded_cblk);

   /*--------------------------------------------------------*\
   ! Free the entropy encoder structure.                      !
   \*--------------------------------------------------------*/
   free_mq_encoder(&coder);

   /*--------------------------------------------------------*\
   ! Calculate the slope values of the distortion/rate convex !
   ! hull.                                                    !
   \*--------------------------------------------------------*/
   compute_convex_hull(encoded_cblk, mse);
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: bwc_encoded_cblk* encode_codeblock(bwc_field *const field, bwc_cblk_access *const access, !
!                                                                         bwc_coder_stripe *const codeblock, !
!                                                                                       const uint64  width, !
!                                                                                       const uint64 height, !
!                                                                                       const uint64  depth, !
!                                                                                       const uint16     dt) !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                -                           -                       -                                       !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                -           Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
decode_codeblock(bwc_field *const field, bwc_cblk_access *const access,
                                     bwc_coder_stripe *const codeblock,
                                                   const uint64  width,
                                                   const uint64 height,
                                                   const uint64  depth,
                                                   const uint16     dt)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int16    k;
   uint8    i, j;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_ctrl       *control;
   bwc_subb_ctrl     *subb_ctrl;
   bwc_encoded_cblk  *encoded_cblk;
   bwc_coder          coder;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(access);
   assert(codeblock);
   assert(access->subband->control.highband_flag <= 15);
   assert(access->subband->control.Kmax >=
          access->codeblock->control.K);

   /*-----------------------*\
   ! DEFINE FUNCTION PTR:    !
   \*-----------------------*/
   void (*decoding_pass[3])(bwc_coder*, int8) = {significance_propagation_dec_pass,
                                                 magnitude_refinement_dec_pass, 
                                                 cleanup_dec_pass};

   /*--------------------------------------------------------*\
   ! Save the global, subband and codeblock control and info  !
   ! structure to temporary variables to make the code more   !
   ! readable.                                                !
   \*--------------------------------------------------------*/
   control       = &field->control;

   subb_ctrl     = &access->subband->control;

   encoded_cblk  = access->codeblock->encoded_block;

   /*--------------------------------------------------------*\
   ! Save the height and width and calculate the number of    !
   ! full stripes and slices for the current codeblockblock.  !
   ! These variables are used to walk through the bwc_coder_  !
   ! stripe structure.                                        !
   \*--------------------------------------------------------*/
   coder.height         = height;
   coder.width          = width;

   coder.no_full_stripe = height >> 2;
   coder.no_slice       = depth * dt;

   /*--------------------------------------------------------*\
   ! Save the index of the first significant bitplane used to !
   ! walk through the sample bitplanes in the bwc_coder_      !
   ! stripe structure.                                        !
   \*--------------------------------------------------------*/
   coder.K              = encoded_cblk->K;

   /*--------------------------------------------------------*\
   ! Save the error resilience and highband flag in the coder !
   ! structure. These flags are used to signal if an error    !
   ! resilient bitstream is to be created and which subband   !
   ! the current codeblock belongs to.                        !
   \*--------------------------------------------------------*/
   coder.erres          = control->error_resilience;
   coder.highband_flag  = subb_ctrl->highband_flag;

   /*--------------------------------------------------------*\
   ! Associate the significance-to-context pointer with the   !
   ! appropriate lookup table for the current subband.        !
   \*--------------------------------------------------------*/
   switch(coder.highband_flag)
   {
      case  0:
      case  2:
      case  4:
      case  6:
      case  8:
      case 10:
      case 12:
      case 14:
      {
         coder.sig2context = SIG2CONTEXT_L;
         break;
      }
      case  1:
      case  5:
      case  9:
      case 13:
      {
         coder.sig2context = SIG2CONTEXT_H;
         break;
      }
      case  3:
      case  7:
      case 11:
      case 15:
      {
         coder.sig2context = SIG2CONTEXT_HH;
         break;
      }
   }

   /*--------------------------------------------------------*\
   ! Associate the data pointer with the bwc_coder_stripe     !
   ! codeblock structure and allocate the memory block used   !
   ! to hold the compressed bitstream.                        !
   \*--------------------------------------------------------*/
   coder.data           = codeblock;
   coder.compressed     = encoded_cblk->data;

   /*--------------------------------------------------------*\
   ! Initialize the entropy encoder used for the current com- !
   ! pression run.                                            !
   \*--------------------------------------------------------*/
   if(initialize_mq_decoder(&coder, CONTEXT_TOTAL, encoded_cblk->L[encoded_cblk->Z - 1]))
   {
      return;
   }

   for(i = 2, j = encoded_cblk->Z, k = coder.K; k --> 0; i = 0)
   {
      for(; i < 3; ++i, --j)
      {
         if(j == 0)
         {
            goto break_out;
         }
         decoding_pass[i](&coder, k);
      }

      if((coder.erres) && (((mq_bit_decode(coder.bitcoder, CONTEXT_UNI) << 3) |
                            (mq_bit_decode(coder.bitcoder, CONTEXT_UNI) << 2) |
                            (mq_bit_decode(coder.bitcoder, CONTEXT_UNI) << 1) |
                             mq_bit_decode(coder.bitcoder, CONTEXT_UNI)) != 0x0A))
      {
         goto break_out;
      }
   }

   break_out:

   /*--------------------------------------------------------*\
   ! Save the last bitplane and coding pass in the data struc-!
   ! ture for proper reconstruction.                          !
   \*--------------------------------------------------------*/
   coder.data->bitplane   = (k > 0) ? k : 0;
   coder.data->codingpass = i;

   /*--------------------------------------------------------*\
   ! Free the entropy encoder structure.                      !
   \*--------------------------------------------------------*/
   free_mq_encoder(&coder);
}

/************************************************************************************************************\
||                  ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                  ||
||                  |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__                   ||
||                  |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]                  ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void *test(void)                                                                          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                -                           -                       -                                       !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                02.07.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
t1_encode(bwc_field *const field, bwc_tile *const tile, bwc_parameter *const parameter)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   c;
   uint64   cbSizeX, cbSizeY, cbSizeZ;
   uint64   width, height, depth;
   int64    buff_size;
   int64    j;
   uint16   cbSizeTS;
   uint16   slope_max, slope_min;
   int16    i, z;
   uint8    nThreads;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl        *control;
   bwc_codeblock      *codeblock;
   bwc_cblk_inf       *cblk_info;
   bwc_coder_stripe   *working_buffer;
   bwc_coder_stripe  **memory;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(tile);
   assert(parameter);

   /*--------------------------------------------------------*\
   ! Save the global control structure to a temporary varia-  !
   ! ble to make the code more readable.                      !
   \*--------------------------------------------------------*/
   control = &field->control;

   /*--------------------------------------------------------*\
   ! Save the minimum and maximum slope values for the cur-   !
   ! rent parameter tile to temporary variables to make the   !
   ! code more readable.                                      !
   \*--------------------------------------------------------*/
   slope_max = tile->control.slope_max;
   slope_min = tile->control.slope_min;

   /*--------------------------------------------------------*\
   ! Evaluate the width, height and depth of the current      !
   ! parameter.                                               !
   \*--------------------------------------------------------*/
   width  = parameter->info.X1 - parameter->info.X0;
   height = parameter->info.Y1 - parameter->info.Y0;
   depth  = parameter->info.Z1 - parameter->info.Z0;

   /*--------------------------------------------------------*\
   ! Evaluate the number of threads used for the current com- !
   ! pression run.                                            !
   \*--------------------------------------------------------*/
   #if defined (_OPENMP)
      nThreads = omp_get_max_threads();
   #else
      nThreads = 1;
   #endif

   /*--------------------------------------------------------*\
   ! Calculate the size of the buffer that holds the wavelet  !
   ! coefficients for the entropy encoding stage. The size of !
   ! the buffer is divided by 4 since the Structure is organ- !
   ! ized in a (4-sample per) stripe pattern. Additionally,   !
   ! the size is increased by two to account for boundary     !
   ! handling during the entropy encoding stage.              !
   \*--------------------------------------------------------*/
   buff_size = (uint64)(1 << (control->cbX + (control->cbY >= 2 ? control->cbY - 2 : 0) + 
                              control->cbZ + control->cbTS)) + 2;

   /*--------------------------------------------------------*\
   ! Allocate the pointer array which holds the memory addres-!
   ! es for the working buffers that are used by the OpenMP   !
   ! threads during a parallel run. For a serial run the      !
   ! pointer array has the size 1.                            !
   \*--------------------------------------------------------*/
   memory = calloc(nThreads, sizeof(bwc_coder_stripe*));
   if(!memory)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Allocate the working buffers that are used by the OpenMP !
   ! threads during a parallel run. For a serial run only one !
   ! working buffer is allocated.                             !
   \*--------------------------------------------------------*/
   for(i = 0; i < nThreads; ++i)
   {
      memory[i] = calloc(buff_size, sizeof(bwc_coder_stripe));
      if(!memory[i])
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         for(; i --> 0;)
         {
            for(j = buff_size - 1; j --> 0;)
            {
               free(memory[i][j].bit);
               free(memory[i][j].sample);
            }
            free(memory[i]);
         }
         free(memory);
         return 1;
      }
      /*--------------------------------------------------------*\
      ! The left and right neighbours for the dummy stripe, re-  !
      ! presenting the upper and lower boundary of the code-     !
      ! block, are set to the dummy stripe used to describe its  !
      ! vertical boundaries to properly uncouple the evaluation  !
      ! of the context lables from the neighbouring codeblocks.  !
      \*--------------------------------------------------------*/
      memory[i][1].stripe_r = &memory[i][0];
      memory[i][1].stripe_l = &memory[i][0];

      /*--------------------------------------------------------*\
      ! Allocate the sample and bit arrays used to hold the mag- !
      ! nitude of a wavelet coefficient and its bitplane repre-  !
      ! sentation.                                               !
      \*--------------------------------------------------------*/
      for(j = 0; j < buff_size; ++j)
      {
         memory[i][j].bit    = calloc(PREC_BIT, sizeof(uint8));
         memory[i][j].sample = calloc(4, sizeof(uint64));
         if(!memory[i][j].sample || !memory[i][j].bit)
         {
            // memory allocation error
            fprintf(stderr, MEMERROR);
            for(; i >= 0; --i)
            {
               for(; j >= 0; --j)
               {
                  free(memory[i][j].bit);
                  free(memory[i][j].sample);
               }
               j = buff_size;
               free(memory[i]);
            }
            free(memory);
            return 1;
         }
      }
   }

   #if defined(_OPENMP)
      #pragma omp parallel private(working_buffer, codeblock, cblk_info, cbSizeX, cbSizeY, cbSizeZ, cbSizeTS) reduction(max:slope_max) reduction(min:slope_min)
   #endif
   {
      /*--------------------------------------------------------*\
      ! Associate the working buffer pointer to the allocated    !
      ! memory block.                                            !
      \*--------------------------------------------------------*/
      #if defined (_OPENMP)
         working_buffer = &memory[omp_get_thread_num()][2];
      #else
         working_buffer = &memory[0][2];
      #endif

      /*--------------------------------------------------------*\
      ! Loop through and encode all codeblocks for the current   !
      ! parameter.                                               !
      \*--------------------------------------------------------*/
      #if defined(_OPENMP)
         #pragma omp for
      #endif
      for(c = 0; c < parameter->control.number_of_codeblocks; ++c)
      {
         /*--------------------------------------------------------*\
         ! Save the codeblock and codeblock info structure to tempo-!
         ! rary variables to make the code more readable.           !
         \*--------------------------------------------------------*/
         codeblock =  parameter->access[c].codeblock;
         cblk_info = &codeblock->info;

         /*--------------------------------------------------------*\
         ! Evaluate the width, height, depth and dt for the current !
         ! codeblock.                                               !
         \*--------------------------------------------------------*/
         cbSizeX  = cblk_info->X1  - cblk_info->X0;
         cbSizeY  = cblk_info->Y1  - cblk_info->Y0;
         cbSizeZ  = cblk_info->Z1  - cblk_info->Z0;
         cbSizeTS = cblk_info->TS1 - cblk_info->TS0;

         /*--------------------------------------------------------*\
         ! Copy the wavelet coefficients for the current codeblock  !
         ! from the data to the working buffer memory block. The    !
         ! working buffer stores the codeblock in stripes of 4 co-  !
         ! efficients. Additionally, a bitplane representation of   !
         ! each coefficient is evaluated for easy access during the !
         ! entropy encoding stage.                                  !
         \*--------------------------------------------------------*/
         cblkcopy_forward(working_buffer,        parameter->data,
                          &parameter->access[c], width,
                          height,                depth);

         /*--------------------------------------------------------*\
         ! Encode the wavelet coefficients for the current code-    !
         ! block and save the compressed bitstream (data), number   !
         ! of coding passes (Z), number of encoded bitplanes (K),   !
         ! number of non-significant, leading bitplanes (Kmsbs),    !
         ! truncation points (L) and possible slope values (S) in   !
         ! the bwc_encoded_cblk structure.                          !
         \*--------------------------------------------------------*/
         encode_codeblock(field, &parameter->access[c], working_buffer, 
                                                        cbSizeX, cbSizeY,
                                                        cbSizeZ, cbSizeTS);

         /*--------------------------------------------------------*\
         ! Check if a compressed bitstream has been created.        !
         \*--------------------------------------------------------*/
         if(codeblock->encoded_block->data)
         {
            /*--------------------------------------------------------*\
            ! Walk through all coding passes, ...                      !
            \*--------------------------------------------------------*/
            for(z = 0; z <= codeblock->encoded_block->Z; ++z)
            {
               /*--------------------------------------------------------*\
               ! ...check if a viable slope value has been created...     !
               \*--------------------------------------------------------*/
               if(codeblock->encoded_block->S[z])
               {
                  /*--------------------------------------------------------*\
                  ! ...and update the minimum and maximum slope values for   !
                  ! the current parameter tile.                              !
                  \*--------------------------------------------------------*/
                  slope_max = MAX(slope_max, codeblock->encoded_block->S[z]);
                  slope_min = MIN(slope_min, codeblock->encoded_block->S[z]);
               }
            }
         }

         /*--------------------------------------------------------*\
         ! Reset the working buffer for the next entropy encoding   !
         ! pass.                                                    !
         \*--------------------------------------------------------*/
         cblkreset_fwd(working_buffer, cbSizeX * ceil((double)cbSizeY/4) * cbSizeZ * cbSizeTS);
      }
   }

   /*--------------------------------------------------------*\
   ! Update the minimum and maximum slope values for the cur- !
   ! rent parameter tile.                                     !
   \*--------------------------------------------------------*/
   tile->control.slope_max = slope_max;
   tile->control.slope_min = slope_min;

   /*--------------------------------------------------------*\
   ! Deallocate the working buffer pointer array and memory   !
   ! blocks.                                                  !
   \*--------------------------------------------------------*/
   for(i = 0; i < nThreads; ++i)
   {
      for(j = 0; j < buff_size; ++j)
      {
         free(memory[i][j].bit);
         free(memory[i][j].sample);
      }
      free(memory[i]);
   }
   free(memory);

   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void *test(void)                                                                          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                -                           -                       -                                       !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                           -                                                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                02.07.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
t1_decode(bwc_field *const field, bwc_tile *const tile, bwc_parameter *const parameter)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   c;
   uint64   cbSizeX, cbSizeY, cbSizeZ;
   uint64   width, height, depth;
   int64    buff_size;
   int64    j;
   uint16   cbSizeTS;
   int16    i;
   uint8    nThreads;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl        *control;
   bwc_codeblock      *codeblock;
   bwc_cblk_inf       *cblk_info;
   bwc_subb_ctrl      *subb_ctrl;
   bwc_coder_stripe   *working_buffer;
   bwc_coder_stripe  **memory;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(tile);
   assert(parameter);

   /*--------------------------------------------------------*\
   ! Save the global control structure to a temporary varia-  !
   ! ble to make the code more readable.                      !
   \*--------------------------------------------------------*/
   control = &field->control;

   /*--------------------------------------------------------*\
   ! Evaluate the width, height and depth of the current      !
   ! parameter.                                               !
   \*--------------------------------------------------------*/
   width  = parameter->info.X1 - parameter->info.X0;
   height = parameter->info.Y1 - parameter->info.Y0;
   depth  = parameter->info.Z1 - parameter->info.Z0;

   /*--------------------------------------------------------*\
   ! Evaluate the number of threads used for the current de-  !
   ! compression run.                                         !
   \*--------------------------------------------------------*/
   #if defined (_OPENMP)
      nThreads = omp_get_max_threads();
   #else
      nThreads = 1;
   #endif

   /*--------------------------------------------------------*\
   ! Calculate the size of the buffer that holds the decoded  !
   ! wavelet coefficients. The size of the buffer is divided  !
   ! by 4 since the Structure is organized in a (4-sample per)!
   ! stripe pattern. Additionally, the size is increased by   !
   ! two to account for boundary handling during the entropy  !
   ! encoding stage.                                          !
   \*--------------------------------------------------------*/
   buff_size = (uint64)(1 << (control->cbX + (control->cbY >= 2 ? control->cbY - 2 : 0) + 
                              control->cbZ + control->cbTS)) + 2;

   /*--------------------------------------------------------*\
   ! Allocate the pointer array which holds the memory addres-!
   ! es for the working buffers that are used by the OpenMP   !
   ! threads during a parallel run. For a serial run the      !
   ! pointer array has the size 1.                            !
   \*--------------------------------------------------------*/
   memory = calloc(nThreads, sizeof(bwc_coder_stripe*));
   if(!memory)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Allocate the working buffers that are used by the OpenMP !
   ! threads during a parallel run. For a serial run only one !
   ! working buffer is allocated.                             !
   \*--------------------------------------------------------*/
   for(i = 0; i < nThreads; ++i)
   {
      memory[i] = calloc(buff_size, sizeof(bwc_coder_stripe));
      if(!memory[i])
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         for(; i --> 0;)
         {
            for(j = buff_size - 1; j --> 0;)
            {
               free(memory[i][j].bit);
               free(memory[i][j].sample);
            }
            free(memory[i]);
         }
         free(memory);
         return 1;
      }
      /*--------------------------------------------------------*\
      ! The left and right neighbours for the dummy stripe, re-  !
      ! presenting the upper and lower boundary of the code-     !
      ! block, are set to the dummy stripe used to describe its  !
      ! vertical boundaries to properly uncouple the evaluation  !
      ! of the context lables from the neighbouring codeblocks.  !
      \*--------------------------------------------------------*/
      memory[i][1].stripe_r = &memory[i][0];
      memory[i][1].stripe_l = &memory[i][0];

      /*--------------------------------------------------------*\
      ! Allocate the sample and bit arrays used to hold the mag- !
      ! nitude of a wavelet coefficient and its bitplane repre-  !
      ! sentation.                                               !
      \*--------------------------------------------------------*/
      for(j = 0; j < buff_size; ++j)
      {
         memory[i][j].bit    = calloc(PREC_BIT, sizeof(uint8));
         memory[i][j].sample = calloc(4, sizeof(uint64));
         if(!memory[i][j].sample || !memory[i][j].bit)
         {
            // memory allocation error
            fprintf(stderr, MEMERROR);
            for(; i >= 0; --i)
            {
               for(; j >= 0; --j)
               {
                  free(memory[i][j].bit);
                  free(memory[i][j].sample);
               }
               j = buff_size;
               free(memory[i]);
            }
            free(memory);
            return 1;
         }
      }
   }

   #if defined(_OPENMP)
      #pragma omp parallel private(working_buffer, codeblock, cblk_info, subb_ctrl,\
                                   cbSizeX,        cbSizeY,   cbSizeZ,   cbSizeTS)
   #endif
   {
      /*--------------------------------------------------------*\
      ! Associate the working buffer pointer to the allocated    !
      ! memory block.                                            !
      \*--------------------------------------------------------*/
      #if defined (_OPENMP)
         working_buffer = &memory[omp_get_thread_num()][2];
      #else
         working_buffer = &memory[0][2];
      #endif

      /*--------------------------------------------------------*\
      ! Loop through and encode all codeblocks for the current   !
      ! parameter.                                               !
      \*--------------------------------------------------------*/
      #if defined(_OPENMP)
         #pragma omp for
      #endif
      for(c = 0; c < parameter->control.number_of_codeblocks; ++c)
      {
         /*--------------------------------------------------------*\
         ! Save the codeblock, subband info and codeblock info and  !
         ! control structure to temporary variables to make the     !
         ! code more readable.                                      !
         \*--------------------------------------------------------*/
         subb_ctrl = &parameter->access[c].subband->control;
         codeblock =  parameter->access[c].codeblock;
         cblk_info = &codeblock->info;

         /*--------------------------------------------------------*\
         ! Evaluate the width, height, depth and dt for the current !
         ! codeblock.                                               !
         \*--------------------------------------------------------*/
         cbSizeX  = cblk_info->X1  - cblk_info->X0;
         cbSizeY  = cblk_info->Y1  - cblk_info->Y0;
         cbSizeZ  = cblk_info->Z1  - cblk_info->Z0;
         cbSizeTS = cblk_info->TS1 - cblk_info->TS0;

         /*--------------------------------------------------------*\
         ! Evaluate the index of the first significant bitplane for !
         ! the current codeblock.                                   !
         \*--------------------------------------------------------*/
         codeblock->encoded_block->K = subb_ctrl->Kmax - codeblock->encoded_block->Kmsbs;

         /*--------------------------------------------------------*\
         ! Reset the working buffer for the next entropy encoding   !
         ! pass.                                                    !
         \*--------------------------------------------------------*/
         cblkreset_inv(working_buffer, cbSizeX, cbSizeY,
                                       cbSizeZ, cbSizeTS);

         if(codeblock->encoded_block->Z > 0)
         {
            decode_codeblock(field, &parameter->access[c], working_buffer, 
                                                           cbSizeX, cbSizeY,
                                                           cbSizeZ, cbSizeTS);
         }

         /*--------------------------------------------------------*\
         ! Copy the wavelet coefficients for the current codeblock  !
         ! from the working buffer to the data memory block.        !
         \*--------------------------------------------------------*/
         cblkcopy_inverse(working_buffer,        parameter->data,
                          &parameter->access[c], width,
                          height,                depth);
      }
   }

   /*--------------------------------------------------------*\
   ! Deallocate the working buffer pointer array and memory   !
   ! blocks.                                                  !
   \*--------------------------------------------------------*/
   for(i = 0; i < nThreads; ++i)
   {
      for(j = 0; j < buff_size; ++j)
      {
         free(memory[i][j].bit);
         free(memory[i][j].sample);
      }
      free(memory[i]);
   }
   free(memory);

   return 0;
}
