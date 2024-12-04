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
||        DESCRIPTION NEEDED.                                                                     ||
||       |                                                                                |       ||
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
/**************************************************************************************************\
||                                _ _  _ ____ _    _  _ ___  ____                                 ||
||                                | |\ | |    |    |  | |  \ |___                                 ||
||                                | | \| |___ |___ |__| |__/ |___                                 ||
||                                                                                                ||
\**************************************************************************************************/
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include "types.h"
#include "ht_block_encoding.h"

/**************************************************************************************************\
||      ____ _  _ ___ ____ ____ _  _ ____ _       _  _ ____ ____ _ ____ ___  _    ____ ____       ||
||      |___  \/   |  |___ |__/ |\ | |__| |       |  | |__| |__/ | |__| |__] |    |___ [__        ||
||      |___ _/\_  |  |___ |  \ | \| |  | |___     \/  |  | |  \ | |  | |__] |___ |___ ___]       ||
||                                                                                                ||
\**************************************************************************************************/
/**************************************************************************************************\
||      ____ _  _ ___ ____ ____ _  _ ____ _       ____ ____ _  _ ____ ___ ____ _  _ ___ ____      ||
||      |___  \/   |  |___ |__/ |\ | |__| |       |    |  | |\ | [__   |  |__| |\ |  |  [__       ||
||      |___ _/\_  |  |___ |  \ | \| |  | |___    |___ |__| | \| ___]  |  |  | | \|  |  ___]      ||
||                                                                                                ||
\**************************************************************************************************/
/**************************************************************************************************\
||          ____ _    ____ ___  ____ _       ____ ____ _  _ ____ ___ ____ _  _ ___ ____           ||
||          | __ |    |  | |__] |__| |       |    |  | |\ | [__   |  |__| |\ |  |  [__            ||
||          |__] |___ |__| |__] |  | |___    |___ |__| | \| ___]  |  |  | | \|  |  ___]           ||
||                                                                                                ||
\**************************************************************************************************/
/**************************************************************************************************\
||           ___  ____ _ _  _ ____ ___ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____           ||
||           |__] |__/ | |  | |__|  |  |___    |___ |  | |\ | |     |  | |  | |\ | [__            ||
||           |    |  \ |  \/  |  |  |  |___    |    |__| | \| |___  |  | |__| | \| ___]           ||
||                                                                                                ||
\**************************************************************************************************/
static void
quantize(bwc_ht_codeblock *const destination, bwc_sample *const source,
         bwc_cblk_access  *const cblkaccess,              const uint64 width,
                           const uint64 height,           const uint64 depth, const uint64 delta)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   bwc_raw  buff, sign_mask;
   uint64   bit_mask, limit;
   uint64   cblk_width, cblk_height, cblk_depth, cblk_dt, cblk_stripe;
   uint64   incrX, incrY, incrZ;
   uint64   i, x, y, z, t;
   uint64   X0, Y0, Z0, TS0;
   uint64   X1, Y1, Z1, TS1;
   int64    idx_u, idx_r, idx_d, idx_l;
   uint8    b, Kmax, s;

   /*-----------------------*\
   ! DEFINE FLOAT VARIABLES: !
   \*-----------------------*/
   bwc_float   qt_step_size;
   bwc_float   qt_scale;

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
   qt_scale     = (bwc_float)(1.0/qt_step_size);
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
   bwc_raw *nu = destination->nu;
   uint8 *sigma = destination->sigma;

   /*--------------------------------------------------------*\
   ! Walk through all wavelet coefficients in the current     !
   ! codeblock in a stripe pattern and save the sign and      !
   ! significant magnitude bits in the bwc_coder_stripe       !
   ! structure. Here, two adjacent stripes are stored         !
   ! in one 8 bit integer.                                    !
   \*--------------------------------------------------------*/
   for(t = 0; t < cblk_dt; ++t)
   {
      for(z = 0; z < cblk_depth; ++z)
      {
         for(y = 0; y < cblk_height; ++y)
         {
            for(x = 0; x < cblk_width; ++x)
            {
               bwc_raw temp;
               bwc_raw sign = tmp[x].raw & SIGN;

               /*--------------------------------------------------------*\
               ! Calculate the absolute value of the wavelet coefficient  !
               ! and store the quantized value in the temporary buffer.   !
               \*--------------------------------------------------------*/
               tmp[x].raw &= sign_mask;
               temp        = (bwc_raw)(tmp[x].f * qt_scale);

               /*--------------------------------------------------------*\
               ! Save the unsigned wavelet coefficient for distortion     !
               ! calculation.                                             !
               \*--------------------------------------------------------*/
               if (temp) {
                  temp--;
                  temp <<= 1;
                  temp += (bwc_raw)(sign >> PREC_BIT);
                  *nu = temp;
                  *sigma |= 1;
               }
               ++nu;
               ++sigma;
            }
            /*--------------------------------------------------------*\
            ! Increment the temporary data pointer to the next row.    !
            \*--------------------------------------------------------*/
            tmp += incrX;
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
/**************************************************************************************************\
||             ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____             ||
||             |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__              ||
||             |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]             ||
||                                                                                                ||
\**************************************************************************************************/
uchar
t1_encode(bwc_codec *const codec, bwc_tile *const tile, bwc_parameter *const parameter)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   c;
   uint64   cbSizeX, cbSizeY, cbSizeZ, cbSizeTS;
   uint64   width, height, depth, delta;
   uint64   buff_size;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl        *control;
   bwc_codeblock      *codeblock;
   bwc_cblk_inf       *cblk_info;
   bwc_param_inf      *param_info;
   bwc_ht_codeblock   *working_buffer;

   control = &codec->control;

   param_info = &parameter->info;

   /*--------------------------------------------------------*\
   ! Evaluate the width, height and depth of the current      !
   ! parameter.                                               !
   \*--------------------------------------------------------*/
   width  = parameter->info.X1 - parameter->info.X0;
   height = parameter->info.Y1 - parameter->info.Y0;
   depth  = parameter->info.Z1 - parameter->info.Z0;
   delta  = parameter->info.TS1 - parameter->info.TS0;

   buff_size = (uint64)(1 << (control->cbX + control->cbY + control->cbZ + control->cbTS));
   working_buffer = calloc(1, sizeof(bwc_ht_codeblock));
   working_buffer->nu = calloc(buff_size, sizeof(bwc_raw));
   working_buffer->sigma = calloc(buff_size, sizeof(uint8));

   for(c = 0; c < parameter->control.number_of_codeblocks; ++c)
   {
      codeblock =  parameter->access[c].codeblock;
      cblk_info = &codeblock->info;

      cbSizeX  = cblk_info->X1  - cblk_info->X0;
      cbSizeY  = cblk_info->Y1  - cblk_info->Y0;
      cbSizeZ  = cblk_info->Z1  - cblk_info->Z0;
      cbSizeTS = cblk_info->TS1 - cblk_info->TS0;

      quantize(working_buffer,        parameter->data,
               &parameter->access[c], width,
               height,                depth, delta);
   }

   free(working_buffer->nu);
   free(working_buffer->sigma);
   free(working_buffer);

   return 1;
}

uchar
t1_decode(bwc_codec *const codec, bwc_tile *const tile, bwc_parameter *const parameter)
{
   return 0;
}