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
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||                                _ _  _ ____ _    _  _ ___  ____                                 ||
||                                | |\ | |    |    |  | |  \ |___                                 ||
||                                | | \| |___ |___ |__| |__/ |___                                 ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#include <assert.h>
#include <math.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "enc_CxtVLC_tables.h"
#include "ht_block_encoding.h"
#include "types.h"

uint64 orgSize;
uint64 compSize;

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||                                 _  _ ____ ____ ____ ____ ____                                  ||
||                                 |\/| |__| |    |__/ |  | [__                                   ||
||                                 |  | |  | |___ |  \ |__| ___]                                  ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#define Q0             0
#define Q1             1

#define MAX_Lcup       16834 * 16
#define MAX_Scup       4072 * 8
#define MEL_SIZE       192
#define VLC_SIZE       MAX_Scup - MEL_SIZE

#define round_up(x, n) (((x) + (n) - 1) & (-n))
#define ceil_int(a, b) ((a) + ((b) - 1)) / (b)

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||           ___  ____ _ _  _ ____ ___ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____           ||
||           |__] |__/ | |  | |__|  |  |___    |___ |  | |\ | |     |  | |  | |\ | [__            ||
||           |    |  \ |  \/  |  |  |  |___    |    |__| | \| |___  |  | |__| | \| ___]           ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

uint8 find_max(uint8 x0, uint8 x1, uint8 x2, uint8 x3)
{
  uint8 v0 = ((x0 > x1) ? x0 : x1);
  uint8 v1 = ((x2 > x3) ? x2 : x3);
  return (v0 > v1) ? v0 : v1;
}

uint8 int_log2(const bwc_raw x)
{
  uint8 y;
  y = (uint8)(PREC_BIT - __builtin_clz(x));
  return (x == 0) ? 0 : y;
}

uint8 count_leading_zeros(const bwc_raw x)
{
  uint8 y;
  y = PREC_BIT - int_log2(x);
  return (x == 0) ? (PREC_BIT + 1) : y;
}

typedef struct mel_struct
{
  //storage
  uint8 *buf;         //pointer to data buffer
  uint32 pos;         //position of next writing within buf
  uint32 buf_size;    //size of buffer, which we must not exceed

  // all these can be replaced by bytes
  int8  k;                 //state
  uint8 run;               //number of 0 run
  uint8 threshold;         //threshold
  uint8 remaining_bits;    //number of empty bits in tmp
  uint8 tmp;               //temporary storage of coded bits
} mel_struct;

struct dec_mel_st
{
  // data decoding machinery
  uint8 *buf;        //!<the address of data (or bitstream)
  uint64 tmp;        //!<temporary buffer for read data
  int    bits;       //!<number of bits stored in tmp
  int    size;       //!<number of bytes in MEL code
  bool   unstuff;    //!<true if the next bit needs to be unstuffed
  int    k;          //!<state of MEL decoder

  // queue of decoded runs
  int    num_runs;    //!<number of decoded runs left in runs (maximum 8)
  uint64 runs;        //!<runs of decoded MEL codewords (7 bits/run)
};

void mel_init(mel_struct *mel, uint32 buffer_size, uint8 *data)
{
  static const int mel_exp[13] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 4, 5};
  mel->buf                     = data;
  mel->pos                     = 0;
  mel->buf_size                = buffer_size;
  mel->k                       = 0;
  mel->run                     = 0;
  mel->threshold               = (uint8)(1 << mel_exp[mel->k]);
  mel->remaining_bits          = 8;
  mel->tmp                     = 0;
}

void mel_emit_bit(mel_struct *mel, int v)
{
  assert(v == 0 || v == 1);
  mel->tmp = (mel->tmp << 1) + v;
  mel->remaining_bits--;
  if(mel->remaining_bits == 0)
    {
      if(mel->pos >= mel->buf_size)
        {
          fprintf(stderr, "mel encoder's buffer is full\n");
          return;
        }
      mel->buf[mel->pos++] = (uint8)mel->tmp;
      mel->remaining_bits  = (uint8)(mel->tmp == 0xFF ? 7 : 8);
      mel->tmp             = 0;
    }
}

void mel_encode(mel_struct *mel, bool smel)
{
  static const uint8 mel_exp[13] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 4, 5};
  if(smel)
    {
      ++mel->run;
      if(mel->run >= mel->threshold)
        {
          mel_emit_bit(mel, 1);
          mel->run       = 0;
          mel->k         = 12 < (mel->k + 1) ? (int8)12 : (mel->k + 1);
          mel->threshold = (uint8)(1 << mel_exp[mel->k]);
        }
    }
  else
    {
      mel_emit_bit(mel, 0);
      int t = mel_exp[mel->k];
      while(t > 0)
        {
          mel_emit_bit(mel, (mel->run >> --t) & 1);
        }
      mel->run       = 0;
      mel->k         = 0 > (mel->k - 1) ? (int8)0 : (mel->k - 1);
      mel->threshold = (uint8)(1 << mel_exp[mel->k]);
    }
}

// VLC encoding
typedef struct vlc_struct
{
  //storage
  uint8 *buf;                 //pointer to data buffer
  uint32 pos;                 //position of next writing within buf
  uint32 buf_size;            //size of buffer, which we must not exceed

  uint8 tmp;                  //temporary storage of coded bits
  uint8 used_bits;            //number of occupied bits in tmp
  bool  last_greater_0x8F;    //true if last byte is greater than 0x8F
} vlc_struct;

static void
vlc_init(vlc_struct *vlc, uint32 buffer_size, uint8 *data)
{
  vlc->buf               = data + buffer_size - 1;    //points to last byte
  vlc->pos               = 1;                         //locations will be all -pos
  vlc->buf_size          = buffer_size;
  vlc->buf[0]            = 0xFF;
  vlc->used_bits         = 4;
  vlc->tmp               = 0xF;
  vlc->last_greater_0x8F = true;
}

//////////////////////////////////////////////////////////////////////////
static void
vlc_encode(vlc_struct *vlc, uint16 cwd, uint8 len)
{
  while(len > 0)
    {
      if(vlc->pos >= vlc->buf_size)
        {
          fprintf(stderr, "vlc encoder's buffer is full\n");
          return;
        }
      int32 available_bits   = 8 - vlc->last_greater_0x8F - vlc->used_bits;
      int32 t                = available_bits < len ? available_bits : len;
      vlc->tmp              |= (uint8)((cwd & ((1 << t) - 1)) << vlc->used_bits);
      vlc->used_bits        += t;
      available_bits        -= t;
      len                   -= t;
      cwd                  >>= t;
      if(available_bits == 0)
        {
          if(vlc->last_greater_0x8F && vlc->tmp != 0x7F)
            {
              vlc->last_greater_0x8F = false;
              continue;
            }
          *(vlc->buf - vlc->pos) = (uint8)(vlc->tmp);
          vlc->pos++;
          vlc->last_greater_0x8F = vlc->tmp > 0x8F;
          vlc->tmp               = 0;
          vlc->used_bits         = 0;
        }
    }
}

static void
mel_vlc_terminate(mel_struct *melp, vlc_struct *vlcp)
{
  if(melp->run > 0)
    {
      mel_emit_bit(melp, 1);
    }

  melp->tmp    = melp->tmp << melp->remaining_bits;
  int mel_mask = (0xFF << melp->remaining_bits) & 0xFF;
  int vlc_mask = 0xFF >> (8 - vlcp->used_bits);
  if((mel_mask | vlc_mask) == 0)
    {
      return;    //last mel byte cannot be 0xFF, since then
                 //melp->remaining_bits would be < 8
    }
  if(melp->pos >= melp->buf_size)
    {
      fprintf(stderr, "mel encoder's buffer is full, while terminating\n");
      return;
    }
  int fuse = melp->tmp | vlcp->tmp;
  if((((fuse ^ melp->tmp) & mel_mask) | ((fuse ^ vlcp->tmp) & vlc_mask)) == 0 && (fuse != 0xFF) && vlcp->pos > 1)
    {
      melp->buf[melp->pos++] = (uint8)fuse;
    }
  else
    {
      if(vlcp->pos >= vlcp->buf_size)
        {
          fprintf(stderr, "vlc encoder's buffer is full, while terminating\n");
          return;
        }
      melp->buf[melp->pos++]   = (uint8)melp->tmp;    //melp->tmp cannot be 0xFF
      *(vlcp->buf - vlcp->pos) = (uint8)vlcp->tmp;
      vlcp->pos++;
    }
}

typedef struct magsgn_struct
{
  //storage
  uint8 *buf;         //pointer to data buffer
  uint32 pos;         //position of next writing within buf
  uint32 buf_size;    //size of buffer, which we must not exceed

  uint8 max_bits;     //maximum number of bits that can be store in tmp
  uint8 used_bits;    //number of occupied bits in tmp
  uint8 tmp;          //temporary storage of coded bits
} magsgn_struct;

//////////////////////////////////////////////////////////////////////////
static void
magsgn_init(magsgn_struct *magsgn, uint32 buffer_size, uint8 *data)
{
  magsgn->buf       = data;
  magsgn->pos       = 0;
  magsgn->buf_size  = buffer_size;
  magsgn->max_bits  = 8;
  magsgn->used_bits = 0;
  magsgn->tmp       = 0;
}

//////////////////////////////////////////////////////////////////////////
void magsgn_encode(magsgn_struct *magsgn, bwc_raw cwd, int len)
{
  while(len > 0)
    {
      if(magsgn->pos >= magsgn->buf_size)
        {
          fprintf(stderr, "magsgn encoder's buffer is full\n");
          return;
        }
      int t               = len < (magsgn->max_bits - magsgn->used_bits) ? len : (magsgn->max_bits - magsgn->used_bits);
      magsgn->tmp        |= (cwd & ((1U << t) - 1)) << magsgn->used_bits;
      magsgn->used_bits  += t;
      cwd               >>= t;
      len                -= t;
      if(magsgn->used_bits >= magsgn->max_bits)
        {
          magsgn->buf[magsgn->pos] = magsgn->tmp;
          magsgn->pos++;
          magsgn->max_bits  = (magsgn->tmp == 0xFF) ? 7 : 8;
          magsgn->tmp       = 0;
          magsgn->used_bits = 0;
        }
    }
}

void magsgn_terminate(magsgn_struct *magsgn)
{
  if(magsgn->used_bits)
    {
      int t              = magsgn->max_bits - magsgn->used_bits;    //unused bits
      magsgn->tmp       |= (0xFF & ((1U << t) - 1)) << magsgn->used_bits;
      magsgn->used_bits += t;
      if(magsgn->tmp != 0xFF)
        {
          if(magsgn->pos >= magsgn->buf_size)
            {
              fprintf(stderr, "magsgn encoder's buffer is full, while terminating\n");
              return;
            }
          magsgn->buf[magsgn->pos++] = (uint8)magsgn->tmp;
        }
    }
  else if(magsgn->max_bits == 7)
    {
      magsgn->pos--;
    }
}

void quantize_sample(bwc_raw *nu, uint8 *sigma, bwc_sample const *const sample,
                     bwc_raw const sign_mask, bwc_float const qt_scale)
{
  bwc_raw    temp, sign;
  bwc_sample smp;

  /*--------------------------------------------------------*\
  ! Extract the sign bit, calculate the absolute value of    !
  ! the wavelet coefficient, and store the quantized value   !
  ! in the temporary buffer.                                 !
  \*--------------------------------------------------------*/
  sign = sample->raw & SIGN;
  smp  = (bwc_sample)(sample->raw & sign_mask);
  temp = (bwc_raw)floor(smp.f * qt_scale);

  /*--------------------------------------------------------*\
  ! Save the unsigned wavelet coefficient for distortion     !
  ! calculation.                                             !
  \*--------------------------------------------------------*/
  if(temp)
    {
      temp--;
      temp   <<= 1;
      temp    += (bwc_raw)(sign >> PREC_BIT);
      *nu      = temp;
      *sigma  |= 1;
    }
}

static void
quantize(bwc_ht_codeblock *const destination, const bwc_sample *const source,
         bwc_cblk_access *const cblkaccess, const uint64 width,
         const uint64 height, const uint64 depth, const uint64 delta)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  bwc_raw buff, sign_mask;
  uint64  bit_mask, limit;
  uint64  cblk_width, cblk_height, cblk_depth, cblk_dt, cblk_stripe;
  uint64  incrX, incrY, incrZ;
  uint64  i, x, y, z, t;
  uint64  X0, Y0, Z0, TS0;
  uint64  X1, Y1, Z1, TS1;
  int64   idx_u, idx_r, idx_d, idx_l;
  uint8   b, Kmax, s;

  /*-----------------------*\
  ! DEFINE FLOAT VARIABLES: !
  \*-----------------------*/
  bwc_float qt_step_size;
  bwc_float qt_scale;

  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  bwc_cblk_inf     *cblk_info;
  const bwc_sample *tmp;

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
  sign_mask = ~((bwc_raw)0x01 << PREC_BIT);

  /*--------------------------------------------------------*\
  ! Initialize the quantization step size and the index of   !
  ! the most significant bitfield.                           !
  \*--------------------------------------------------------*/
  qt_step_size = cblkaccess->subband->control.qt_effective_step_size;
  qt_scale     = (bwc_float)(1.0 / qt_step_size);
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
  cblk_stripe = ceil((double)cblk_height / 4);

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
  tmp            = &source[X0 + width * (Y0 + height * (Z0 + depth * TS0))];
  bwc_raw *nu    = destination->nu;
  uint8   *sigma = destination->sigma;

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
                  quantize_sample(nu, sigma, tmp + x, sign_mask, qt_scale);
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
                 uint8 *const sigma_n, bwc_raw *const v_n, int32 *const E_n,
                 uint8 *const rho_q)
{
  const bwc_raw *const sp0  = nu + 2U * (qx + qy * QWx2);
  const bwc_raw *const sp1  = sp0 + QWx2;
  const uint8 *const   ssp0 = sigma + (2U * qy + 1U) * QWx2 + 2U * qx + 1U;
  const uint8 *const   ssp1 = ssp0 + QWx2;

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

  E_n[0] = (int32)(PREC_BIT + 1 - count_leading_zeros(v_n[0]) * sigma_n[0]);
  E_n[1] = (int32)(PREC_BIT + 1 - count_leading_zeros(v_n[1]) * sigma_n[1]);
  E_n[2] = (int32)(PREC_BIT + 1 - count_leading_zeros(v_n[2]) * sigma_n[2]);
  E_n[3] = (int32)(PREC_BIT + 1 - count_leading_zeros(v_n[3]) * sigma_n[3]);
  E_n[4] = (int32)(PREC_BIT + 1 - count_leading_zeros(v_n[4]) * sigma_n[4]);
  E_n[5] = (int32)(PREC_BIT + 1 - count_leading_zeros(v_n[5]) * sigma_n[5]);
  E_n[6] = (int32)(PREC_BIT + 1 - count_leading_zeros(v_n[6]) * sigma_n[6]);
  E_n[7] = (int32)(PREC_BIT + 1 - count_leading_zeros(v_n[7]) * sigma_n[7]);
}

static void
encode_codeblock(bwc_codec *const codec, bwc_cblk_access *const access,
                 bwc_ht_codeblock *const working_buffer,
                 const uint64            width,
                 const uint64            height,
                 const uint64            depth,
                 const uint64            delta)
{
  bwc_encoded_cblk *encoded_cblk = access->codeblock->encoded_block;

  const uint16_t QW   = (uint16)(ceil_int((int16)(width), 2));
  const uint16_t QH   = (uint16_t)(ceil_int((int16)(height), 2));
  const uint64   QWx2 = round_up(width, 8U);

  uint8     magsgn_buf[MAX_Lcup];
  uint8     mel_vlc_buf[MAX_Scup];
  uint8    *mel_buf  = mel_vlc_buf;
  const int vlc_size = MAX_Scup - MEL_SIZE;
  uint8    *vlc_buf  = mel_vlc_buf + MEL_SIZE;

  mel_struct mel;
  mel_init(&mel, MEL_SIZE, mel_buf);
  vlc_struct vlc;
  vlc_init(&vlc, VLC_SIZE, vlc_buf);
  magsgn_struct magsgn;
  magsgn_init(&magsgn, MAX_Lcup, magsgn_buf);

  alignas(32) int32            *Eline      = calloc(2U * QW + 6U, sizeof(int32));
  alignas(32) int32            *rholine    = calloc(QW + 3U, sizeof(int32));
  alignas(PREC_BIT + 1) bwc_raw nu_n[8]    = {0};
  alignas(PREC_BIT + 1) uint8   sigma_n[8] = {0}, rho_q[2] = {0}, m_n[8] = {0};
  alignas(PREC_BIT + 1) int32   E_n[8] = {0}, U_q[2] = {0};

  uint8   lw, gamma;
  uint16  context = 0, n_q, CxtVLC, cwd;
  uint8   Emax_q;
  int32_t u_q, uoff, u_min, uvlc_idx, kappa = 1;
  int32_t emb_pattern, embk_0, embk_1, emb1_0, emb1_1;

  for(uint64 t = 0; t < delta; ++t)
    {
      for(uint64 z = 0; z < depth; ++z)
        {
          int32   *E_p   = Eline + 1;
          int32   *rho_p = rholine + 1;
          bwc_raw *nu    = working_buffer->nu + width * height * (z + depth * t);
          uint8   *sigma = working_buffer->sigma + width * height * (z + depth * t);
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
              emb_pattern  = (E_n[0] == Emax_q) ? uoff : 0;
              emb_pattern += (E_n[1] == Emax_q) ? uoff << 1 : 0;
              emb_pattern += (E_n[2] == Emax_q) ? uoff << 2 : 0;
              emb_pattern += (E_n[3] == Emax_q) ? uoff << 3 : 0;
              n_q          = (uint16_t)(emb_pattern + (rho_q[Q0] << 4) + (context << 8));
              CxtVLC       = enc_CxtVLC_table0[n_q];
              embk_0       = CxtVLC & 0xF;
              emb1_0       = emb_pattern & embk_0;
              lw           = (CxtVLC >> 4) & 0x07;
              cwd          = (uint16_t)(CxtVLC >> 7);
              vlc_encode(&vlc, cwd, lw);

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
              n_q          = (uint16_t)(emb_pattern + (rho_q[Q1] << 4) + (context << 8));
              CxtVLC       = enc_CxtVLC_table0[n_q];
              embk_1       = CxtVLC & 0xF;
              emb1_1       = emb_pattern & embk_1;
              lw           = (CxtVLC >> 4) & 0x07;
              cwd          = (uint16_t)(CxtVLC >> 7);
              vlc_encode(&vlc, cwd, lw);
              // UVLC encoding
              int32 tmp = (int32)(enc_UVLC_table0[uvlc_idx]);
              lw        = (uint8)(tmp & 0xFF);
              cwd       = (uint16)(tmp >> 8);
              vlc_encode(&vlc, cwd, lw);

              if(context == 0)
                {
                  if(rho_q[Q1] != 0)
                    {
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
                  if(u_min > 2)
                    {
                      mel_encode(&mel, true);
                    }
                  else
                    {
                      mel_encode(&mel, false);
                    }
                }

              for(uint8 i = 0; i < 4; ++i)
                {
                  m_n[i] = (uint8_t)(sigma_n[i] * U_q[Q0] - ((embk_0 >> i) & 1));
                  magsgn_encode(&magsgn, nu_n[i], m_n[i]);
                }
              for(uint8 i = 0; i < 4; ++i)
                {
                  m_n[4 + i] = (uint8_t)(sigma_n[4 + i] * U_q[Q1] - ((embk_1 >> i) & 1));
                  magsgn_encode(&magsgn, nu_n[4 + i], m_n[4 + i]);
                }

              // context for the next quad
              context = (rho_q[Q1] >> 1) | (rho_q[Q1] & 0x1);
              // update rho_line
              *rho_p++ = rho_q[0];
              *rho_p++ = rho_q[1];
            }
          if(QW & 1)
            {
              uint16 qx = (uint16)(QW - 1);
              fetch_quads(nu, sigma, 0, qx, QWx2, sigma_n, nu_n, E_n, rho_q);
              *E_p++ = E_n[1];
              *E_p++ = E_n[3];

              if(context == 0)
                {
                  mel_encode(&mel, (rho_q[Q0] != 0));
                }

              Emax_q   = find_max(E_n[0], E_n[1], E_n[2], E_n[3]);
              U_q[Q0]  = Emax_q < kappa ? kappa : Emax_q;
              u_q      = U_q[Q0] - kappa;
              uvlc_idx = u_q;
              uoff     = (u_q) ? 1 : 0;

              emb_pattern  = (E_n[0] == Emax_q) ? uoff : 0;
              emb_pattern += (E_n[1] == Emax_q) ? uoff << 1 : 0;
              emb_pattern += (E_n[2] == Emax_q) ? uoff << 2 : 0;
              emb_pattern += (E_n[3] == Emax_q) ? uoff << 3 : 0;
              n_q          = (uint16_t)(emb_pattern + (rho_q[Q0] << 4) + (context << 8));
              // VLC encoding
              CxtVLC = enc_CxtVLC_table0[n_q];
              embk_0 = CxtVLC & 0xF;
              emb1_0 = emb_pattern & embk_0;
              lw     = (CxtVLC >> 4) & 0x07;
              cwd    = (uint16_t)(CxtVLC >> 7);
              vlc_encode(&vlc, cwd, lw);
              // UVLC encoding
              int32 tmp = (int32)(enc_UVLC_table0[uvlc_idx]);
              lw        = (uint8)(tmp & 0xFF);
              cwd       = (uint16)(tmp >> 8);
              vlc_encode(&vlc, cwd, lw);

              for(uint8 i = 0; i < 4; ++i)
                {
                  m_n[i] = (uint8_t)(sigma_n[i] * U_q[Q0] - ((embk_0 >> i) & 1));
                  magsgn_encode(&magsgn, nu_n[i], m_n[i]);
                }

              // update rho_line
              *rho_p++ = rho_q[0];
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
              context  = (uint16_t)(((rho_q[1] & 0x4) << 7) | ((rho_q[1] & 0x8) << 6));    // (w | sw) << 9
              context |= ((rho_p[-1] & 0x8) << 5) | ((rho_p[0] & 0x2) << 7);               // (nw | n) << 8
              context |= ((rho_p[0] & 0x8) << 7) | ((rho_p[1] & 0x2) << 9);
              for(uint16 qx = 0; qx < QW - 1; qx += 2)
                {
                  fetch_quads(nu, sigma, qy, qx, QWx2, sigma_n, nu_n, E_n, rho_q);

                  if(context == 0)
                    {
                      mel_encode(&mel, rho_q[Q0] != 0);
                    }

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
                  n_q          = (uint16_t)(emb_pattern + (rho_q[Q0] << 4) + (context << 0));
                  CxtVLC       = enc_CxtVLC_table1[n_q];
                  embk_0       = CxtVLC & 0xF;
                  emb1_0       = emb_pattern & embk_0;
                  lw           = (CxtVLC >> 4) & 0x07;
                  cwd          = (uint16_t)(CxtVLC >> 7);
                  vlc_encode(&vlc, cwd, lw);

                  // calculate context for the next quad
                  context  = (uint16_t)(((rho_q[0] & 0x4) << 7) | ((rho_q[0] & 0x8) << 6));    // (w | sw) << 9
                  context |= ((rho_p[0] & 0x8) << 5) | ((rho_p[1] & 0x2) << 7);                // (nw | n) << 8
                  context |= ((rho_p[1] & 0x8) << 7) | ((rho_p[2] & 0x2) << 9);
                  if(context == 0)
                    {
                      mel_encode(&mel, rho_q[Q1] != 0);
                    }
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
                  n_q          = (uint16_t)(emb_pattern + (rho_q[Q1] << 4) + (context << 0));
                  CxtVLC       = enc_CxtVLC_table1[n_q];
                  embk_1       = CxtVLC & 0xF;
                  emb1_1       = emb_pattern & embk_1;
                  lw           = (CxtVLC >> 4) & 0x07;
                  cwd          = (uint16_t)(CxtVLC >> 7);
                  // TODO: Check correctness because of seg fault.
                  vlc_encode(&vlc, cwd, lw);
                  // UVLC encoding
                  int32_t tmp = (int32_t)(enc_UVLC_table1[uvlc_idx]);
                  lw          = (uint8_t)(tmp & 0xFF);
                  cwd         = (uint16_t)(tmp >> 8);
                  vlc_encode(&vlc, cwd, lw);

                  for(uint8 i = 0; i < 4; ++i)
                    {
                      m_n[i] = (uint8_t)(sigma_n[i] * U_q[Q0] - ((embk_0 >> i) & 1));
                      magsgn_encode(&magsgn, nu_n[i], m_n[i]);
                    }
                  for(uint8 i = 0; i < 4; ++i)
                    {
                      m_n[4 + i] = (uint8_t)(sigma_n[4 + i] * U_q[Q1] - ((embk_1 >> i) & 1));
                      magsgn_encode(&magsgn, nu_n[4 + i], m_n[4 + i]);
                    }

                  Emax0 = find_max(E_p[3], E_p[4], E_p[5], E_p[6]);
                  Emax1 = find_max(E_p[5], E_p[6], E_p[7], E_p[8]);

                  *E_p++ = E_n[1];
                  *E_p++ = E_n[3];
                  *E_p++ = E_n[5];
                  *E_p++ = E_n[7];

                  // calculate context for the next quad
                  context  = (uint16_t)(((rho_q[1] & 0x4) << 7) | ((rho_q[1] & 0x8) << 6));    // (w | sw) << 9
                  context |= ((rho_p[1] & 0x8) << 5) | ((rho_p[2] & 0x2) << 7);                // (nw | n) << 8
                  context |= ((rho_p[2] & 0x8) << 7) | ((rho_p[3] & 0x2) << 9);                // (ne | nf) << 10

                  *rho_p++ = rho_q[0];
                  *rho_p++ = rho_q[1];
                }
              if(QW & 1)
                {
                  uint16 qx = (uint16)(QW - 1);
                  fetch_quads(nu, sigma, 0, qx, QWx2, sigma_n, nu_n, E_n, rho_q);
                  *E_p++ = E_n[1];
                  *E_p++ = E_n[3];

                  if(context == 0)
                    {
                      mel_encode(&mel, (rho_q[Q0] != 0));
                    }

                  gamma = (__builtin_popcount(rho_q[Q0]) > 1) ? 1 : 0;
                  kappa = (Emax0 - 1) * gamma;
                  kappa = kappa > 1 ? kappa : 1;

                  Emax_q   = find_max(E_n[0], E_n[1], E_n[2], E_n[3]);
                  U_q[Q0]  = Emax_q < kappa ? kappa : Emax_q;
                  u_q      = U_q[Q0] - kappa;
                  uvlc_idx = u_q;
                  uoff     = (u_q) ? 1 : 0;

                  emb_pattern  = (E_n[0] == Emax_q) ? uoff : 0;
                  emb_pattern += (E_n[1] == Emax_q) ? uoff << 1 : 0;
                  emb_pattern += (E_n[2] == Emax_q) ? uoff << 2 : 0;
                  emb_pattern += (E_n[3] == Emax_q) ? uoff << 3 : 0;
                  n_q          = (uint16_t)(emb_pattern + (rho_q[Q0] << 4) + (context << 8));
                  // VLC encoding
                  CxtVLC = enc_CxtVLC_table0[n_q];
                  embk_0 = CxtVLC & 0xF;
                  emb1_0 = emb_pattern & embk_0;
                  lw     = (CxtVLC >> 4) & 0x07;
                  cwd    = (uint16_t)(CxtVLC >> 7);
                  vlc_encode(&vlc, cwd, lw);
                  // UVLC encoding
                  int32 tmp = (int32)(enc_UVLC_table0[uvlc_idx]);
                  lw        = (uint8)(tmp & 0xFF);
                  cwd       = (uint16)(tmp >> 8);
                  vlc_encode(&vlc, cwd, lw);

                  for(uint8 i = 0; i < 4; ++i)
                    {
                      m_n[i] = (uint8_t)(sigma_n[i] * U_q[Q0] - ((embk_0 >> i) & 1));
                      magsgn_encode(&magsgn, nu_n[i], m_n[i]);
                    }

                  // update rho_line
                  *rho_p++ = rho_q[0];
                }
            }
        }
    }

  free(Eline);
  free(rholine);
  mel_vlc_terminate(&mel, &vlc);
  magsgn_terminate(&magsgn);

  encoded_cblk->K     = 0;
  encoded_cblk->Kmsbs = 63;
  encoded_cblk->Z     = 1;
  encoded_cblk->L[0]  = mel.pos + vlc.pos + magsgn.pos;
  encoded_cblk->L[1]  = 0;
  encoded_cblk->L[1]  = 0;

  encoded_cblk->S[0] = 1;
  encoded_cblk->S[1] = 0;
  encoded_cblk->S[2] = 0;

  //orgSize  += width * height * depth * delta * sizeof(bwc_raw);
  //compSize += (mel.pos + vlc.pos + magsgn.pos) * sizeof(uint8);
  //printf("original size %ld\n", orgSize);
  //printf("compressed size %ld\n", compSize);
  //printf("ratio orgSize/compSize %1.2f\n", (double)orgSize / (double)compSize);

  encoded_cblk->data = calloc(mel.pos + vlc.pos + magsgn.pos, sizeof(uint8));
  memcpy(encoded_cblk->data, magsgn.buf, magsgn.pos);
  memcpy(encoded_cblk->data + magsgn.pos, mel.buf, mel.pos);
  memcpy(encoded_cblk->data + magsgn.pos + mel.pos, vlc.buf, vlc.pos);
}

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||             ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____             ||
||             |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__              ||
||             |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]             ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
uchar t1_encode(bwc_codec *const codec, bwc_tile *const tile, bwc_parameter *const parameter)
{
  /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
  uint64 c;
  uint64 cbSizeX, cbSizeY, cbSizeZ, cbSizeTS;
  uint64 width, height, depth, delta;
  uint64 buff_size;
  uint64 z, t;

  /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
  bwc_gl_ctrl      *control;
  bwc_codeblock    *codeblock;
  bwc_cblk_inf     *cblk_info;
  bwc_param_inf    *param_info;
  bwc_ht_codeblock *working_buffer;

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

  buff_size             = (uint64)(1 << (control->cbX + control->cbY + control->cbZ + control->cbTS));
  working_buffer        = calloc(1, sizeof(bwc_ht_codeblock));
  working_buffer->nu    = calloc(buff_size, sizeof(bwc_raw));
  working_buffer->sigma = calloc(buff_size, sizeof(uint8));

  for(c = 0; c < parameter->control.number_of_codeblocks; ++c)
    {
      codeblock = parameter->access[c].codeblock;
      cblk_info = &codeblock->info;

      cbSizeX  = cblk_info->X1 - cblk_info->X0;
      cbSizeY  = cblk_info->Y1 - cblk_info->Y0;
      cbSizeZ  = cblk_info->Z1 - cblk_info->Z0;
      cbSizeTS = cblk_info->TS1 - cblk_info->TS0;

      quantize(working_buffer, parameter->data,
               &parameter->access[c], width,
               height, depth, delta);

      encode_codeblock(codec, &parameter->access[c], working_buffer,
                       cbSizeX, cbSizeY,
                       cbSizeZ, cbSizeTS);
    }

  free(working_buffer->nu);
  free(working_buffer->sigma);
  free(working_buffer);

  return 0;
}
