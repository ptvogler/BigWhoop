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
||  Copyright (c) 2019, Aous Naman                                                                ||
||  Copyright (c) 2019, Kakadu Software Pty Ltd, Australia                                        ||
||  Copyright (c) 2019, The University of New South Wales, Australia                              ||
||  Copyright (c) 2024, High Performance Computing Center - University of Stuttgart               ||
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
#include <stdalign.h>
#include <stdbool.h>

#include "types.h"
#include "enc_CxtVLC_tables.h"
#include "ht_block_encoding.h"

#define Q0 0
#define Q1 1

#define MAX_Scup 3072
#define MEL_SIZE 192

#define round_up(x, n) (((x) + (n) - 1) & (-n))
#define ceil_int(a, b) ((a) + ((b) - 1)) / (b)

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

inline uint8 find_max(uint8 x0, uint8 x1, uint8 x2, uint8 x3) {
  uint8 v0 = ((x0 > x1) ? x0 : x1);
  uint8 v1 = ((x2 > x3) ? x2 : x3);
  return (v0 > v1) ? v0 : v1;
}

inline uint8 int_log2(const bwc_raw x) {
  uint8 y;
  y = (uint8)(PREC_BIT - __builtin_clz(x));
  return (x == 0) ? 0 : y;
}

inline uint8 count_leading_zeros(const bwc_raw x) {
  uint8 y;
  y = PREC_BIT - int_log2(x);
  return (x == 0) ? (PREC_BIT+1) : y;
}

typedef struct mel_struct {
   //storage
   uint8 *buf;           //pointer to data buffer
   uint32 pos;           //position of next writing within buf
   uint32 buf_size;      //size of buffer, which we must not exceed

   // all these can be replaced by bytes
   int8 k;               //state
   uint8 run;            //number of 0 run
   uint8 threshold;      //threshold
   uint8 remaining_bits; //number of empty bits in tmp
   uint8 tmp;            //temporary storage of coded bits
} mel_struct;

void mel_init(mel_struct* mel, uint32 buffer_size, uint8* data)
{
   static const int mel_exp[13] = {0,0,0,1,1,1,2,2,2,3,3,4,5};
   mel->buf = data;
   mel->pos = 0;
   mel->buf_size = buffer_size;
   mel->k = 0;
   mel->run = 0;
   mel->threshold = (uint8)(1 << mel_exp[mel->k]);
   mel->remaining_bits = 8;
   mel->tmp = 0;
}

inline void mel_emit_bit(mel_struct* mel, int v)
{
   assert(v == 0 || v == 1);
   mel->tmp = (mel->tmp << 1) + v;
   mel->remaining_bits--;
   if(mel->remaining_bits == 0)
   {
      mel->buf[mel->pos++] = (uint8)mel->tmp;
      mel->remaining_bits = (uint8)(mel->tmp == 0xFF ? 7 : 8);
      mel->tmp = 0;
   }
}

inline void
mel_encode(mel_struct* mel, bool smel)
{
   static const int mel_exp[13] = {0,0,0,1,1,1,2,2,2,3,3,4,5};
   if(smel)
   {
      ++mel->run;
      if(mel->run >= mel->threshold)
      {
         mel_emit_bit(mel, 1);
         mel->run = 0;
         mel->k = 12 < (mel->k+1) ? (int8)12 : (mel->k+1);
         mel->threshold = (uint8)(1 << mel_exp[mel->k]);
      }
   }
   else
   {
      mel_emit_bit(mel, 0);
      int t = mel_exp[mel->k];
      while (t > 0)
      {
        mel_emit_bit(mel, (mel->run >> --t) & 1);
      }
      mel->run = 0;
      mel->k = 0 > (mel->k-1) ? (int8)0 : (mel->k-1);
      mel->threshold = (uint8)(1 << mel_exp[mel->k]);
   }
}

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

void fetch_quads(const bwc_raw *const nu, const uint8 *const sigma,
                 const uint16 qy, const uint16 qx, const uint16 QWx2,
                 uint8 *const sigma_n, bwc_raw *const v_n, uint8 *const E_n,
                 uint8 *const rho_q)
{
  const bwc_raw *const sp0  = nu + 2U * (qx + qy * QWx2);
  const bwc_raw *const sp1  = sp0 + QWx2;
  const uint8   *const ssp0 = sigma + (2U * qy + 1U) * QWx2 + 2U * qx + 1U;
  const uint8   *const ssp1 = ssp0 + QWx2;

  sigma_n[0] = ssp0[0] & 1;
  sigma_n[1] = ssp1[0] & 1;
  sigma_n[2] = ssp0[1] & 1;
  sigma_n[3] = ssp1[1] & 1;
  sigma_n[4] = ssp0[2] & 1;
  sigma_n[5] = ssp1[2] & 1;
  sigma_n[6] = ssp0[3] & 1;
  sigma_n[7] = ssp1[3] & 1;

  rho_q[0] = (uint8)(sigma_n[0] + (sigma_n[1] << 1) + (sigma_n[2] << 2) + (sigma_n[3] << 3));
  rho_q[1] = (uint8)(sigma_n[4] + (sigma_n[5] << 1) + (sigma_n[6] << 2) + (sigma_n[7] << 3));

  v_n[0] = (bwc_raw)(sp0[0]);
  v_n[1] = (bwc_raw)(sp1[0]);
  v_n[2] = (bwc_raw)(sp0[1]);
  v_n[3] = (bwc_raw)(sp1[1]);
  v_n[4] = (bwc_raw)(sp0[2]);
  v_n[5] = (bwc_raw)(sp1[2]);
  v_n[6] = (bwc_raw)(sp0[3]);
  v_n[7] = (bwc_raw)(sp1[3]);

  E_n[0] = (uint8)(PREC_BIT + 1 - count_leading_zeros(v_n[0]) * sigma_n[0]);
  E_n[1] = (uint8)(PREC_BIT + 1 - count_leading_zeros(v_n[1]) * sigma_n[1]);
  E_n[2] = (uint8)(PREC_BIT + 1 - count_leading_zeros(v_n[2]) * sigma_n[2]);
  E_n[3] = (uint8)(PREC_BIT + 1 - count_leading_zeros(v_n[3]) * sigma_n[3]);
  E_n[4] = (uint8)(PREC_BIT + 1 - count_leading_zeros(v_n[4]) * sigma_n[4]);
  E_n[5] = (uint8)(PREC_BIT + 1 - count_leading_zeros(v_n[5]) * sigma_n[5]);
  E_n[6] = (uint8)(PREC_BIT + 1 - count_leading_zeros(v_n[6]) * sigma_n[6]);
  E_n[7] = (uint8)(PREC_BIT + 1 - count_leading_zeros(v_n[7]) * sigma_n[7]);
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
   uint64   z, t;

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

      //printf("cbX etc. %d, %d, %d, %d\n", (1<<control->cbX),
                                              //(1<<control->cbY),
                                              //(1<<control->cbZ),
                                              //(1<<control->cbTS));

      //printf("cbSizeX etc. %ld, %ld, %ld, %ld\n", cbSizeX, cbSizeY, cbSizeZ, cbSizeTS);

      //memset(working_buffer->nu, 0, buff_size);

      quantize(working_buffer,        parameter->data,
               &parameter->access[c], width,
               height,                depth, delta);

      //printf("working_buffer \n");
      //for (int i = 0; i< buff_size; ++i)
      //{
         //printf("%ld ", working_buffer->nu[i]);
      //}
      //printf("\n");

            // COPY FOR DEBUGGING INTO LOOP
            //printf("fetch \n");
            //for(int i = 0; i < cbSizeY * cbSizeX; ++i)
            //{
             //  printf("%ld ", nu[i]);
            //}
            //printf("\n");

      const uint16_t QW = (uint16)(ceil_int((int16)(cbSizeX), 2));
      const uint16_t QH = (uint16_t)(ceil_int((int16)(cbSizeY), 2));

      const uint64 QWx2 = round_up(cbSizeX, 8U);

      const int mel_vlc_size = MAX_Scup;
      uint8 mel_vlc_buf[mel_vlc_size];
      uint8 *mel_buf = mel_vlc_buf;

      mel_struct mel;
      mel_init(&mel, MEL_SIZE, mel_buf);

      alignas(32) uint8 *Eline       = calloc(2U * QW + 6U, sizeof(uint8));
      alignas(32) int32 *rholine     = calloc(QW + 3U, sizeof(int32));
      alignas(PREC_BIT+1) bwc_raw nu_n[8] = {0};
      alignas(PREC_BIT+1) uint8 sigma_n[8] = {0}, E_n[8] = {0}, rho_q[2] = {0};
      alignas(PREC_BIT+1) int32 U_q[2] = {0};

      uint8 lw, gamma;
      uint16 context = 0, n_q, CxtVLC, cwd;
      uint8 Emax_q;
      int32_t u_q, uoff, u_min, uvlc_idx, kappa = 1;
      int32_t emb_pattern, embk_0, embk_1, emb1_0, emb1_1;

      for(t = 0; t < cbSizeTS; ++t)
      {
         for(z = 0; z < cbSizeZ; ++z)
         {
            uint8 *E_p                     = Eline + 1;
            int32 *rho_p                   = rholine + 1;
            bwc_raw *nu  = working_buffer->nu + cbSizeX * cbSizeY * (z + cbSizeZ * t);
            uint8 *sigma = working_buffer->sigma + cbSizeX * cbSizeY * (z + cbSizeZ * t);
            for(uint16 qx = 0; qx < QW - 1; qx += 2)
            {
               uint8 uoff_flag = 1;

               fetch_quads(nu, sigma, 0, qx, QWx2, sigma_n, nu_n, E_n, rho_q);

               *E_p++ = E_n[1];
               *E_p++ = E_n[3];
               *E_p++ = E_n[5];
               *E_p++ = E_n[7];

               if(context == 0)
               {
                  mel_encode(&mel, rho_q[Q0] != 0);
               }

               Emax_q       = find_max(E_n[0], E_n[1], E_n[2], E_n[3]);
               U_q[Q0]      = Emax_q < kappa ? kappa : Emax_q;
               u_q          = U_q[Q0] - kappa;
               u_min        = u_q;
               uvlc_idx     = u_q;
               uoff         = (u_q) ? 1 : 0;
               uoff_flag   &= uoff;
               emb_pattern  = (E_n[0] == Emax_q) ? uoff      : 0;
               emb_pattern += (E_n[1] == Emax_q) ? uoff << 1 : 0;
               emb_pattern += (E_n[2] == Emax_q) ? uoff << 2 : 0;
               emb_pattern += (E_n[3] == Emax_q) ? uoff << 3 : 0;
               n_q = (uint16_t)(emb_pattern + (rho_q[Q0] << 4) + (context << 8));
               // TODO: VLC encoding
               CxtVLC = enc_CxtVLC_table0[n_q];
               embk_0 = CxtVLC & 0xF;
               emb1_0 = emb_pattern & embk_0;
               lw     = (CxtVLC >> 4) & 0x07;
               cwd    = (uint16_t)(CxtVLC >> 7);

               context = (rho_q[Q0] >> 1) | (rho_q[Q0] & 0x1);

               Emax_q       = find_max(E_n[4], E_n[5], E_n[6], E_n[7]);
               U_q[Q1]      = Emax_q < kappa ? kappa : Emax_q;
               u_q          = U_q[Q1] - kappa;
               u_min        = (u_min < u_q) ? u_min : u_q;
               uvlc_idx    += u_q << 5;
               uoff         = (u_q) ? 1 : 0;
               uoff_flag   &= uoff;
               emb_pattern  = (E_n[4] == Emax_q) ? uoff : 0;
               emb_pattern += (E_n[5] == Emax_q) ? uoff << 1 : 0;
               emb_pattern += (E_n[6] == Emax_q) ? uoff << 2 : 0;
               emb_pattern += (E_n[7] == Emax_q) ? uoff << 3 : 0;
               n_q = (uint16_t)(emb_pattern + (rho_q[Q1] << 4) + (context << 8));
               // TODO: VLC encoding
               CxtVLC = enc_CxtVLC_table0[n_q];
               embk_1 = CxtVLC & 0xF;
               emb1_1 = emb_pattern & embk_1;
               lw     = (CxtVLC >> 4) & 0x07;
               cwd    = (uint16_t)(CxtVLC >> 7);

               if(context == 0) {
                  if(rho_q[Q1] != 0) {
                     mel_encode(&mel, true);
                  }
                  else
                  {
                     if(u_min > 2)
                     {
                        mel_encode(&mel, true);
                     }
                     else
                     {
                        mel_encode(&mel, false);
                     }
                  }
               }
               else if(uoff_flag)
               {
                  if(u_min > 2) {
                     mel_encode(&mel, true);
                  } else {
                     mel_encode(&mel, false);
                  }
               }
            }

            int32_t Emax0, Emax1;
            for(uint16_t qy = 1; qy < QH; qy++)
            {
               E_p      = Eline + 1;
               rho_p    = rholine + 1;
               rho_q[1] = 0;

               Emax0 = find_max(E_p[-1], E_p[0], E_p[1], E_p[2]);
               Emax1 = find_max(E_p[1], E_p[2], E_p[3], E_p[4]);

               // calculate context for the next quad
               context  = (uint16_t)(((rho_q[1] & 0x4) << 7) | ((rho_q[1] & 0x8) << 6));  // (w | sw) << 9
               context |= ((rho_p[-1] & 0x8) << 5) | ((rho_p[0] & 0x2) << 7);             // (nw | n) << 8
               context |= ((rho_p[0] & 0x8) << 7) | ((rho_p[1] & 0x2) << 9);
               for(uint16 qx = 0; qx < QW - 1; qx += 2)
               {
                  fetch_quads(nu, sigma, qy, qx, QWx2, sigma_n, nu_n, E_n, rho_q);

                  // TODO : encode MEL if context == 0
                  gamma = (__builtin_popcount(rho_q[Q0]) > 1) ? 1 : 0;
                  kappa = (Emax0 - 1) * gamma;
                  kappa = kappa > 1 ? kappa : 1;

                  Emax_q       = find_max(E_n[0], E_n[1], E_n[2], E_n[3]);
                  U_q[Q0]      = Emax_q > kappa ? Emax_q : kappa;
                  u_q          = U_q[Q0] - kappa;
                  uvlc_idx     = u_q;
                  uoff         = (u_q) ? 1 : 0;
                  emb_pattern  = (E_n[0] == Emax_q) ? uoff : 0;
                  emb_pattern += (E_n[1] == Emax_q) ? uoff << 1 : 0;
                  emb_pattern += (E_n[2] == Emax_q) ? uoff << 2 : 0;
                  emb_pattern += (E_n[3] == Emax_q) ? uoff << 3 : 0;
                  n_q = (uint16_t)(emb_pattern + (rho_q[Q0] << 4) + (context << 0));

                  // calculate context for the next quad
                  context  = (uint16_t)(((rho_q[0] & 0x4) << 7) | ((rho_q[0] & 0x8) << 6));  // (w | sw) << 9
                  context |= ((rho_p[0] & 0x8) << 5) | ((rho_p[1] & 0x2) << 7);              // (nw | n) << 8
                  context |= ((rho_p[1] & 0x8) << 7) | ((rho_p[2] & 0x2) << 9);
                  // TODO : encode MEL if context == 0
                  gamma = (__builtin_popcount(rho_q[Q1]) > 1) ? 1 : 0;
                  kappa = (Emax1 - 1) * gamma;
                  kappa = kappa > 1 ? kappa : 1;

                  Emax_q       = find_max(E_n[4], E_n[5], E_n[6], E_n[7]);
                  u_q          = U_q[Q1] - kappa;
                  uvlc_idx    += u_q << 5;
                  uoff         = (u_q) ? 1 : 0;
                  emb_pattern  = (E_n[4] == Emax_q) ? uoff : 0;
                  emb_pattern += (E_n[5] == Emax_q) ? uoff << 1 : 0;
                  emb_pattern += (E_n[6] == Emax_q) ? uoff << 2 : 0;
                  emb_pattern += (E_n[7] == Emax_q) ? uoff << 3 : 0;
                  n_q = (uint16_t)(emb_pattern + (rho_q[Q1] << 4) + (context << 0));
               }
            }
         }
      }
      free(Eline);
      free(rholine);
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