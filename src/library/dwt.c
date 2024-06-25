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
||        This file describes a set of functions that can be used to performe the forward/        ||
||        inverse discrete wavelet transform on 1- to 4-dimensional IEEE 754 data-sets.           ||
||        For more information please refere to JPEG2000 by D. S. Taubman and M. W.               ||
||        Marcellin.                                                                              ||
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
#include <omp.h>

#include "constants.h"
#include "macros.h"
#include "types.h"
#include "dwt.h"

/************************************************************************************************************\
||            ____ _  _ ___ ____ ____ _  _ ____ _       _  _ ____ ____ _ ____ ___  _    ____ ____           ||
||            |___  \/   |  |___ |__/ |\ | |__| |       |  | |__| |__/ | |__| |__] |    |___ [__            ||
||            |___ _/\_  |  |___ |  \ | \| |  | |___     \/  |  | |  \ | |  | |__] |___ |___ ___]           ||
||                                                                                                          ||
\************************************************************************************************************/
double DWT_ENERGY_GAIN_LUT[3][2 * MAX_DECOMP_LEVELS + 2];

/************************************************************************************************************\
||                ___  ____ _ _  _ ____ ___ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                ||
||                |__] |__/ | |  | |__|  |  |___    |___ |  | |\ | |     |  | |  | |\ | [__                 ||
||                |    |  \ |  \/  |  |  |  |___    |    |__| | \| |___  |  | |__| | \| ___]                ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uint8 get_max_taps(bwc_gl_ctrl *const control)                                            !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function returns the rounded up maximum half length of the specified wavelet filter.   !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                dwt_filter                  bwc_dwt_filter        - Specifies the wavelet kernels used for  !
!                                                                    spatial decomposition.                  !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                uint8                     - Specifies the rounded up maximum half-length of a wavelet fil-  !
!                                            ter.                                                            !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                22.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uint8
get_filter_taps(bwc_dwt_filter dwt_filter)
{
   /*--------------------------------------------------------*\
   ! Emit the rounded up maximum half-length of the specified !
   ! wavelet filter.                                          !
   \*--------------------------------------------------------*/
   switch(dwt_filter)
   {
      case bwc_dwt_9_7:
      {
         return 4;
      }
      case bwc_dwt_5_3:
      {
         return 2;
      }
      case bwc_dwt_haar:
      {
         return 1;
      }
      default:
      {
         assert(0);
         return 0;
      }
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void fill_forward_buffer(bwc_sample *const data, bwc_sample *const working_buffer,        !
!   ------------                                                                         uint64 res0,        !
!                                                                                        uint64 res1,        !
!                                                                                   uint64 increment)        !
!                                                                                                            !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to fill the working buffer with the appropriate flow field data for   !
!                the forward one dimensional wavelet transform.                                              !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                data                        bwc_sample*           - Pointer to the flow field data of the   !
!                                                                    current tile parameter.                 !
!                                                                                                            !
!                working_buffer              bwc_sample*           - Working buffer used to store flow field !
!                                                                    data for the one dimensional dwt.       !
!                                                                                                            !
!                res0                        unsigned int(64 bit)  - Starting point for the current resolu-  !
!                                                                    tion and dimension.                     !
!                                                                                                            !
!                res1                        unsigned int(64 bit)  - End point for the current resolution    !
!                                                                    and dimension.                          !
!                                                                                                            !
!                increment                   unsigned int(64 bit)  - Data increment for strided memory ac-   !
!                                                                    cess.                                   !
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
!                24.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
fill_forward_buffer(bwc_sample *const data, bwc_sample *const working_buffer, uint64 res0, uint64 res1, uint64 increment)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   i, j;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(data);
   assert(working_buffer);

   /*--------------------------------------------------------*\
   ! Copy the strided memory for the current resolution and   !
   ! dimension from the tile parameter buffer to the working  !
   ! buffer for the one dimensional wavelet transform.        !
   \*--------------------------------------------------------*/
   for(i = 0, j = 0; i < res1 - res0; ++i)
   {
      working_buffer[i] = data[j];
      j += increment;
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void fill_inverse_buffer(bwc_sample *const data, bwc_sample *const working_buffer,        !
!   ------------                                                                         uint64 res0,        !
!                                                                                        uint64 res1,        !
!                                                                                   uint64 increment)        !
!                                                                                                            !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to fill the working buffer with the appropriate flow field data for   !
!                the inverse one dimensional wavelet transform.                                              !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                data                        bwc_sample*           - Pointer to the flow field data of the   !
!                                                                    current tile parameter.                 !
!                                                                                                            !
!                working_buffer              bwc_sample*           - Working buffer used to store flow field !
!                                                                    data for the one dimensional dwt.       !
!                                                                                                            !
!                res0                        unsigned int(64 bit)  - Starting point for the current resolu-  !
!                                                                    tion and dimension.                     !
!                                                                                                            !
!                res1                        unsigned int(64 bit)  - End point for the current resolution    !
!                                                                    and dimension.                          !
!                                                                                                            !
!                increment                   unsigned int(64 bit)  - Data increment for strided memory ac-   !
!                                                                    cess.                                   !
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
!                24.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
fill_inverse_buffer(bwc_sample *const data, bwc_sample *const working_buffer, uint64 res0, uint64 res1, uint64 increment)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int64   i, length, half_length;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_sample        *lowband, *highband;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(data);
   assert(working_buffer);

   /*--------------------------------------------------------*\
   ! Calculate the length and half_length of the buffer.      !
   \*--------------------------------------------------------*/
   length = res1 - res0;
   half_length = (res0 % 2) ? length >> 1 : (length + 1) >> 1;

   /*--------------------------------------------------------*\
   ! Associate the data pointers to the starting point of the !
   ! low- and highband.                                       !
   \*--------------------------------------------------------*/
   lowband = data;
   highband = data + half_length * increment;

   /*--------------------------------------------------------*\
   ! Check if the starting point is odd.                      !
   \*--------------------------------------------------------*/
   if((res0 % 2) == 0)
   {
      /*--------------------------------------------------------*\
      ! Loop over the lowband wavelet coefficients in the tile   !
      ! parameter memory and interleave them into the working    !
      ! buffer.                                                  !
      \*--------------------------------------------------------*/
      for(i = 0; i < half_length; ++i)
      {
         working_buffer[i << 1].f = lowband->f;
         lowband    += increment;
      }

      /*--------------------------------------------------------*\
      ! Decrease the half length in case of an odd resolution    !
      ! length to account for the half-length - 1 number of      !
      ! highband wavelet coefficients.                           !
      \*--------------------------------------------------------*/
      if(length % 2) --half_length;

      /*--------------------------------------------------------*\
      ! Loop over the highband wavelet coefficients in the tile  !
      ! parameter memory and interleave them into the working    !
      ! buffer.                                                  !
      \*--------------------------------------------------------*/
      for(i = 0; i < half_length; ++i)
      {
         working_buffer[(i << 1) + 1].f = highband->f;
         highband    += increment;
      }
   }
   else
   {
      /*--------------------------------------------------------*\
      ! Loop over the lowband wavelet coefficients in the tile   !
      ! parameter memory and interleave them into the working    !
      ! buffer.                                                  !
      \*--------------------------------------------------------*/
      for(i = 0; i < half_length; ++i)
      {
         working_buffer[i << 1].f = lowband->f;
         lowband    += increment;
      }

      /*--------------------------------------------------------*\
      ! Increase the half length in case of an even resolution   !
      ! length to account for the half-length + 1 number of      !
      ! highband wavelet coefficients.                           !
      \*--------------------------------------------------------*/
      if(length % 2) ++half_length;

      /*--------------------------------------------------------*\
      ! Loop over the highband wavelet coefficients in the tile  !
      ! parameter memory and interleave them into the working    !
      ! buffer.                                                  !
      \*--------------------------------------------------------*/
      for(i = 0; i < half_length; ++i)
      {
         working_buffer[(i << 1) + 1].f = highband->f;
         highband    += increment;
      }
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void whole_point_symmetric_extend(bwc_sample *const working_buffer, uint64 res0,          !
!   ------------                                                                       uint64 res1,          !
!                                                                                      uint8 nTaps)          !
!                                                                                                            !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to fill the boundary values of the working buffer with appropriate    !
!                flow field data using a symmetric whole point extension:                                    !
!                         ___ ___ ___  ___ ___ ___ ___ ___       ___ ___ ___ ___ ___  ___ ___ ___            !
!                        |___|___|___||_A_|_B_|_C_|_D_|_E_| ... |_V_|_W_|_X_|_Y_|_Z_||___|___|___|           !
!                                                            |                                               !
!                                                            |                                               !
!                                                           \|/                                              !
!                         ___ ___ ___  ___ ___ ___ ___ ___   V   ___ ___ ___ ___ ___  ___ ___ ___            !
!                        |_E_|_C_|_B_||_A_|_B_|_C_|_D_|_E_| ... |_V_|_W_|_X_|_Y_|_Z_||_Y_|_X_|_W_|           !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                working_buffer              bwc_sample*           - Working buffer used to store flow field !
!                                                                    data for the one dimensional dwt.       !
!                                                                                                            !
!                res0                        unsigned int(64 bit)  - Starting point for the current resolu-  !
!                                                                    tion and dimension.                     !
!                                                                                                            !
!                res1                        unsigned int(64 bit)  - End point for the current resolution    !
!                                                                    and dimension.                          !
!                                                                                                            !
!                nTaps                       unsigned int(8 bit)   - Maximum half length of a wavelet filter.!
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
!                25.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
whole_point_symmetric_extend(bwc_sample *const working_buffer, uint64 res0, uint64 res1, uint8 nTaps)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   i, length;
   int64    j, min, max;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(working_buffer);

   length = res1 - res0;

   if(nTaps < length)
   {
      for(i = 0; i < nTaps; ++i) 
      {
         working_buffer[i - nTaps] = working_buffer[nTaps - i];
         working_buffer[length + i] = working_buffer[length - (i + 2)];
      }
   }
   else
   {
      min = nTaps;
      max = nTaps + length - 1;

      for(i = 0; i < nTaps; ++i)
      {
         j = i;

         while (j < min || j > max)
         {
            if(j < min) j = (min << 1) - j;
            else j = (max << 1) - j;
         }
         working_buffer[i - nTaps] = working_buffer[j - nTaps];

         j = i + length + nTaps;

         while (j < min || j > max)
         {
            if(j < min) j = (min << 1) - j;
            else j = (max << 1) - j;
         }
         working_buffer[i + length] = working_buffer[j - nTaps];
      }
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void forward_9x7_CDF_wavelet_transform(bwc_sample *const working_buffer, uint64 res0,     !
!   ------------                                                                            uint64 res1)     !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function applies the forward (9-7) Cohen-Daubechies-Feauveau wavelet transform on the  !
!                data samples stored in the working buffer.                                                  !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                working_buffer              bwc_sample*           - Working buffer used to store flow field !
!                                                                    data for the one dimensional dwt.       !
!                                                                                                            !
!                res0                        unsigned int(64 bit)  - Starting point for the current resolu-  !
!                                                                    tion and dimension.                     !
!                                                                                                            !
!                res1                        unsigned int(64 bit)  - End point for the current resolution    !
!                                                                    and dimension.                          !
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
!                25.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
forward_9x7_CDF_wavelet_transform(bwc_sample *const working_buffer, uint64 res0, uint64 res1)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int64   i, length;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(working_buffer);

   length = res1 - res0;

   /*--------------------------------------------------------*\
   ! Perform the first predictor step on the odd samples.     !
   \*--------------------------------------------------------*/
   for(i = -3; i < length + 3; i += 2)
   {
      working_buffer[i].f += ALPHA * (working_buffer[i - 1].f + working_buffer[i + 1].f);
   }

   /*--------------------------------------------------------*\
   ! Perform the first update step on the even samples.       !
   \*--------------------------------------------------------*/
   for(i = -2; i < length + 2; i += 2)
   {
      working_buffer[i].f += BETA * (working_buffer[i - 1].f + working_buffer[i + 1].f);
   }

   /*--------------------------------------------------------*\
   ! Perform the second predictor and scaling step on the odd !
   ! samples.                                                 !
   \*--------------------------------------------------------*/
   for(i = -1; i < length + 1; i += 2)
   {
      working_buffer[i].f += GAMMA * (working_buffer[i - 1].f + working_buffer[i + 1].f);
      working_buffer[i].f  = KAPPA_H * working_buffer[i].f;
   }

   /*--------------------------------------------------------*\
   ! Perform the second update and scaling step on the even   !
   ! samples. The delta coefficient has been modified to      !
   ! account for the scaling step on the even samples.        !
   \*--------------------------------------------------------*/
   for(i = 0; i < length; i += 2)
   {
      working_buffer[i].f += DELTA * (working_buffer[i - 1].f + working_buffer[i + 1].f);
      working_buffer[i].f  = KAPPA_L * working_buffer[i].f;
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void inverse_9x7_CDF_wavelet_transform(bwc_sample *const working_buffer, uint64 res0,     !
!   ------------                                                                            uint64 res1)     !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function applies the inverse (9-7) Cohen-Daubechies-Feauveau wavelet transform on the  !
!                data samples stored in the working buffer.                                                  !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                working_buffer              bwc_sample*           - Working buffer used to store flow field !
!                                                                    data for the one dimensional dwt.       !
!                                                                                                            !
!                res0                        unsigned int(64 bit)  - Starting point for the current resolu-  !
!                                                                    tion and dimension.                     !
!                                                                                                            !
!                res1                        unsigned int(64 bit)  - End point for the current resolution    !
!                                                                    and dimension.                          !
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
!                25.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
inverse_9x7_CDF_wavelet_transform(bwc_sample *const working_buffer, uint64 res0, uint64 res1)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int64   i, length;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(working_buffer);

   length = res1 - res0;

   /*--------------------------------------------------------*\
   ! Perform the second update and scaling step on the even   !
   ! samples. The delta coefficient has been modified to      !
   ! account for the scaling step on the even samples.        !
   \*--------------------------------------------------------*/
   for(i = -4; i < length + 4; i += 2)
   {
      working_buffer[i].f  = KAPPA_H * working_buffer[i].f;
      working_buffer[i].f -= DELTA * (working_buffer[i - 1].f + working_buffer[i + 1].f);
   }

   /*--------------------------------------------------------*\
   ! Perform the second predictor and scaling step on the odd !
   ! samples.                                                 !
   \*--------------------------------------------------------*/
   for(i = -5; i < length + 3; i += 2)
   {
      working_buffer[i].f  = KAPPA_L * working_buffer[i].f;
      working_buffer[i].f -= GAMMA * (working_buffer[i - 1].f + working_buffer[i + 1].f);
   }

   /*--------------------------------------------------------*\
   ! Perform the first update step on the even samples.       !
   \*--------------------------------------------------------*/
   for(i = -4; i < length + 2; i += 2)
   {
      working_buffer[i].f -= BETA * (working_buffer[i - 1].f + working_buffer[i + 1].f);
   }

   /*--------------------------------------------------------*\
   ! Perform the first predictor step on the odd samples.     !
   \*--------------------------------------------------------*/
   for(i = -3; i < length + 1; i += 2)
   {
      working_buffer[i].f -= ALPHA * (working_buffer[i - 1].f + working_buffer[i + 1].f);
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void forward_5x3_LeGall_wavelet_transform(bwc_sample *const working_buffer, uint64 res0,  !
!   ------------                                                                               uint64 res1)  !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function applies the forward (5-3) LeGall wavelet transform on the data samples stored !
!                in the working buffer.                                                                      !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                working_buffer              bwc_sample*           - Working buffer used to store flow field !
!                                                                    data for the one dimensional dwt.       !
!                                                                                                            !
!                res0                        unsigned int(64 bit)  - Starting point for the current resolu-  !
!                                                                    tion and dimension.                     !
!                                                                                                            !
!                res1                        unsigned int(64 bit)  - End point for the current resolution    !
!                                                                    and dimension.                          !
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
!                25.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
forward_5x3_LeGall_wavelet_transform(bwc_sample *const working_buffer, uint64 res0, uint64 res1)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int64   i, length;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(working_buffer);

   length = res1 - res0;

   /*--------------------------------------------------------*\
   ! Perform the predictor step on the odd samples.           !
   \*--------------------------------------------------------*/
   for(i = -1; i < length + 1; i += 2)
   {
      working_buffer[i].f -= (working_buffer[i - 1].f + working_buffer[i + 1].f) / 2;
   }

   /*--------------------------------------------------------*\
   ! Perform the update step on the even samples.             !
   \*--------------------------------------------------------*/
   for(i = 0; i < length; i += 2)
   {
      working_buffer[i].f += (working_buffer[i - 1].f + working_buffer[i + 1].f + 2) / 4;
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void inverse_5x3_LeGall_wavelet_transform(bwc_sample *const working_buffer, uint64 res0,  !
!   ------------                                                                               uint64 res1)  !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function applies the inverse (5-3) LeGall wavelet transform on the data samples stored !
!                in the working buffer.                                                                      !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                working_buffer              bwc_sample*           - Working buffer used to store flow field !
!                                                                    data for the one dimensional dwt.       !
!                                                                                                            !
!                res0                        unsigned int(64 bit)  - Starting point for the current resolu-  !
!                                                                    tion and dimension.                     !
!                                                                                                            !
!                res1                        unsigned int(64 bit)  - End point for the current resolution    !
!                                                                    and dimension.                          !
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
!                25.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
inverse_5x3_LeGall_wavelet_transform(bwc_sample *const working_buffer, uint64 res0, uint64 res1)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int64   i, length;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(working_buffer);

   length = res1 - res0;

   /*--------------------------------------------------------*\
   ! Perform the update step on the even samples.             !
   \*--------------------------------------------------------*/
   for(i = -2; i < length + 1; i += 2)
   {
      working_buffer[i].f -= (working_buffer[i - 1].f + working_buffer[i + 1].f + 2) / 4;
   }

   /*--------------------------------------------------------*\
   ! Perform the predictor step on the odd samples.           !
   \*--------------------------------------------------------*/
   for(i = -1; i < length; i += 2)
   {
      working_buffer[i].f += (working_buffer[i - 1].f + working_buffer[i + 1].f) / 2;
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void forward_Haar_wavelet_transform(bwc_sample *const working_buffer, uint64 res0,        !
!   ------------                                                                         uint64 res1)        !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function applies the forward Haar wavelet transform on the data samples stored in the  !
!                working buffer.                                                                             !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                working_buffer              bwc_sample*           - Working buffer used to store flow field !
!                                                                    data for the one dimensional dwt.       !
!                                                                                                            !
!                res0                        unsigned int(64 bit)  - Starting point for the current resolu-  !
!                                                                    tion and dimension.                     !
!                                                                                                            !
!                res1                        unsigned int(64 bit)  - End point for the current resolution    !
!                                                                    and dimension.                          !
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
!                25.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
forward_Haar_wavelet_transform(bwc_sample *const working_buffer, uint64 res0, uint64 res1)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int64   i, length;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(working_buffer);

   length = res1 - res0;

   /*--------------------------------------------------------*\
   ! Perform the predictor step on the odd samples.           !
   \*--------------------------------------------------------*/
   for(i = 1; i < length; i += 2)
   {
      working_buffer[i].f -= working_buffer[i - 1].f;
   }

   /*--------------------------------------------------------*\
   ! Perform the update step on the even samples.             !
   \*--------------------------------------------------------*/
   for(i = 0; i < length; i += 2)
   {
      working_buffer[i].f += working_buffer[i + 1].f / 2;
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void inverse_Haar_wavelet_transform(bwc_sample *const working_buffer, uint64 res0,        !
!   ------------                                                                         uint64 res1)        !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function applies the inverse Haar wavelet transform on the data samples stored in the  !
!                working buffer.                                                                             !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                working_buffer              bwc_sample*           - Working buffer used to store flow field !
!                                                                    data for the one dimensional dwt.       !
!                                                                                                            !
!                res0                        unsigned int(64 bit)  - Starting point for the current resolu-  !
!                                                                    tion and dimension.                     !
!                                                                                                            !
!                res1                        unsigned int(64 bit)  - End point for the current resolution    !
!                                                                    and dimension.                          !
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
!                25.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
inverse_Haar_wavelet_transform(bwc_sample *const working_buffer, uint64 res0, uint64 res1)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int64   i, length;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(working_buffer);

   length = res1 - res0;

   /*--------------------------------------------------------*\
   ! Perform the update step on the even samples.             !
   \*--------------------------------------------------------*/
   for(i = 0; i < length; i += 2)
   {
      working_buffer[i].f -= working_buffer[i + 1].f / 2;
   }

   /*--------------------------------------------------------*\
   ! Perform the predictor step on the odd samples.           !
   \*--------------------------------------------------------*/
   for(i = 1; i < length; i += 2)
   {
      working_buffer[i].f += working_buffer[i - 1].f;
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void buffer_flush_forward(bwc_sample *const data, bwc_sample *const working_buffer,       !
!   ------------                                                                          uint64 res0,       !
!                                                                                         uint64 res1,       !
!                                                                                    uint64 increment)       !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function creates the low- and highbands in the tile parameter after a one dimensional  !
!                wavelet transform by copying the coefficients in the working buffer to their appropriate    !
!                position in the tile parameter memory. In case of an odd length working buffer there are    !
!                half_length + 1 lowband coefficients present in the buffer memory. Therefore, this func-    !
!                tion loops through half_length - 1 wavelet coefficients and handles the last (two) coeffi-  !
!                cient(s) separately.                                                                        !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                working_buffer              bwc_sample*           - Working buffer used to store flow field !
!                                                                    data for the one dimensional dwt.       !
!                                                                                                            !
!                res0                        unsigned int(64 bit)  - Starting point for the current resolu-  !
!                                                                    tion and dimension.                     !
!                                                                                                            !
!                res1                        unsigned int(64 bit)  - End point for the current resolution    !
!                                                                    and dimension.                          !
!                                                                                                            !
!                increment                   unsigned int(64 bit)  - Data increment for strided memory ac-   !
!                                                                    cess.                                   !
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
!                25.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
buffer_flush_forward(bwc_sample *const data, bwc_sample *const working_buffer, uint64 res0, uint64 res1, uint64 increment)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int64   i, length, half_length;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_sample        *lowband, *highband;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(data);
   assert(working_buffer);

   /*--------------------------------------------------------*\
   ! Calculate the length and half_length of the buffer.      !
   \*--------------------------------------------------------*/
   length = res1 - res0;
   half_length = (res0 % 2) ? length >> 1 : (length + 1) >> 1;

   /*--------------------------------------------------------*\
   ! Associate the data pointers to the starting point of the !
   ! low- and highband.                                       !
   \*--------------------------------------------------------*/
   lowband = data;
   highband = data + half_length * increment;

   /*--------------------------------------------------------*\
   ! In case of an odd starting point copy the first highband !
   ! coefficient from the working buffer.                     !
   \*--------------------------------------------------------*/
   if(res0 % 2)
   {
      highband->f = working_buffer[-1].f;
      highband   += increment;
   }

   /*--------------------------------------------------------*\
   ! Loop over the interleaved values in the working buffer   !
   ! and fill the high- and lowbands in the tile parameter    !
   ! memory.                                                  !
   \*--------------------------------------------------------*/
   for(i = 0; i < half_length - 1; ++i)
   {
      lowband->f  = working_buffer[i << 1].f;
      highband->f = working_buffer[(i << 1) + 1].f;
      lowband    += increment;
      highband   += increment;
   }

   /*--------------------------------------------------------*\
   ! Copy the remaining lowband coefficient in case of an odd !
   ! length working buffer.                                   !
   \*--------------------------------------------------------*/
   if(res1 % 2)
   {
      lowband->f  = working_buffer[i << 1].f;
   }

   /*--------------------------------------------------------*\
   ! Copy the remaining high- and lowband coefficient in case !
   ! of an even length working buffer.                        !
   \*--------------------------------------------------------*/
   else
   {
      lowband->f  = working_buffer[i << 1].f;
      highband->f = working_buffer[(i << 1) + 1].f;
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void buffer_flush_inverse(bwc_sample *const data, bwc_sample *const working_buffer,       !
!   ------------                                                                          uint64 res0,       !
!                                                                                         uint64 res1,       !
!                                                                                    uint64 increment)       !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function copies the decompressed flow field data for the current resolution level from !
!                the working buffer to the tile parameter memory.                                            !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                working_buffer              bwc_sample*           - Working buffer used to store flow field !
!                                                                    data for the one dimensional dwt.       !
!                                                                                                            !
!                res0                        unsigned int(64 bit)  - Starting point for the current resolu-  !
!                                                                    tion and dimension.                     !
!                                                                                                            !
!                res1                        unsigned int(64 bit)  - End point for the current resolution    !
!                                                                    and dimension.                          !
!                                                                                                            !
!                increment                   unsigned int(64 bit)  - Data increment for strided memory ac-   !
!                                                                    cess.                                   !
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
!                25.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
buffer_flush_inverse(bwc_sample *const data, bwc_sample *const working_buffer, uint64 res0, uint64 res1, uint64 increment)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int64   i, j, length;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_sample        *memory;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(data);
   assert(working_buffer);

   /*--------------------------------------------------------*\
   ! Associate the data pointer to the starting point of the  !
   ! tile parameter resolution.                               !
   \*--------------------------------------------------------*/
   memory = data;
   
   /*--------------------------------------------------------*\
   ! Calculate the length of the buffer.                      !
   \*--------------------------------------------------------*/
   length = res1 - res0;

   /*--------------------------------------------------------*\
   ! In case of an odd starting point copy the first sample   !
   ! to the tile parameter memory.                            !
   \*--------------------------------------------------------*/
   if(res0 % 2)
   {
      memory->f = working_buffer[-1].f;
      memory   += increment;
      length--;
   }

   /*--------------------------------------------------------*\
   ! Copy the decompressed flow field data from the working   !
   ! buffer to the strided tile parameter memory for the      !
   ! current resolution level.                                 !
   \*--------------------------------------------------------*/
   for(i = 0, j = 0; i < length; ++i)
   {
      memory[j].f = working_buffer[i].f;
      j += increment;
   }
}
/************************************************************************************************************\
||                  ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                  ||
||                  |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__                   ||
||                  |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]                  ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: int initialize_gain_lut()                                                                 !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function calculates the energy gain factor Gb for the one dimensional, dyadic tree     !
!                structured CDF-(9/7), LeGall-(5/3) and Haar wavelet transform with 5 levels. The energy     !
!                gain factors are calculated according to equation (4.39) from JPEG2000 by David S. Taubman  !
!                and Michael W. Marcellin (p. 193):                                                          !
!                                                                                                            !
!                                            s_L1[n] = g0[n],   s_H1[n] = h0[n],                             !
!                         s_Ld[n] = ∑(s_Ld-1[k]*g0[n-2k], k),   s_Hd[n] = ∑(s_Hd-1[k]*g0[n-2k], k).          !
!                                                                                                            !
!                The energy gain factors are stored in their corresponding lookup tables and used to calcu-  !
!                late the energy gain for the multi dimensional wavelet transforms according to equation     !
!                (4.40) from JPEG2000 by David S. Taubman and Michael W. Marcellin (p.193):                  !
!                                                                                                            !
!                               s_LLD[n1,n2] = s_LD[n1] * s_LD[n2]   =>   G_LLD = G_LD * G_LD                !
!                               s_HLD[n1,n2] = s_LD[n1] * s_HD[n2]   =>   G_HLD = G_LD * G_HD                !
!                               s_LHD[n1,n2] = s_HD[n1] * s_LD[n2]   =>   G_LHD = G_HD * G_LD                !
!                               s_HHD[n1,n2] = s_HD[n1] * s_HD[n2]   =>   G_HHD = G_HD * G_HD                !
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
!                uchar                     - Error flag used to determine wether the LUT initialization      !
!                                            was successful.                                                 !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                21.03.2017  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
initialize_gain_lut()
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int32    l,m;
   int8     i,j,k;
   int8     Length_FB, Length_Gb;    

   /*-----------------------*\
   ! DEFINE FLOAT VARIABLES: !
   \*-----------------------*/
   double  *buff1, *buff2, *temp;
   double  *LUT; 

   /*-----------------------*\
   ! DEFINE INT CONSTANTS:   !
   \*-----------------------*/
   static const int* Length;  
   static const int  DWT_HAAR_FILTER_LENGTH[2] = {2,2};
   static const int  DWT_5X3_FILTER_LENGTH[2]  = {1,2};
   static const int  DWT_9X7_FILTER_LENGTH[2]  = {3,4};

   /*-----------------------*\
   ! DEFINE FLOAT CONSTANTS: !
   \*-----------------------*/
   const double*       filter_taps;

   static const double DWT_HAAR_FILTER[2][5] = {{DWT_HAAR_H1, DWT_HAAR_H0, 0,          0,          0}, 
                                                {DWT_HAAR_G1, DWT_HAAR_G0, 0,          0,          0}};
   static const double DWT_5X3_FILTER[2][5]  = {{DWT_5X3_H1,  DWT_5X3_H0,  0,          0,          0}, 
                                                {DWT_5X3_G2,  DWT_5X3_G1,  DWT_5X3_G0, 0,          0}};
   static const double DWT_9X7_FILTER[2][5]  = {{DWT_9X7_H3,  DWT_9X7_H2,  DWT_9X7_H1, DWT_9X7_H0, 0}, 
                                                {DWT_9X7_G4,  DWT_9X7_G3,  DWT_9X7_G2, DWT_9X7_G1, DWT_9X7_G0}};

   /*--------------------------------------------------------*\
   ! Allocate memory for the work buffers used to calculate   !
   ! the energy gain factors.                                 !
   \*--------------------------------------------------------*/
   buff1   = calloc(512, sizeof(double));
   buff2   = calloc(512, sizeof(double));
   if (!buff1 || !buff2) 
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      free(buff1);
      free(buff2);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Initialize the lookup tables for the CDF-(9/7), LeGall-  !
   ! (5/3) and Haar wavelet transform by setting the energy   !
   ! gain factor for level zero to one.                       !
   \*--------------------------------------------------------*/
   DWT_ENERGY_GAIN_LUT[bwc_dwt_9_7][0]  = DWT_ENERGY_GAIN_LUT[bwc_dwt_9_7][MAX_DECOMP_LEVELS + 1] = 1.0f;
   DWT_ENERGY_GAIN_LUT[bwc_dwt_5_3][0]  = DWT_ENERGY_GAIN_LUT[bwc_dwt_5_3][MAX_DECOMP_LEVELS + 1] = 1.0f;
   DWT_ENERGY_GAIN_LUT[bwc_dwt_haar][0] = DWT_ENERGY_GAIN_LUT[bwc_dwt_haar][MAX_DECOMP_LEVELS + 1] = 1.0f;
   
   /*--------------------------------------------------------*\
   ! Loop through the symmetric wavelet filter banks and as-  !
   ! semble their respective energy gain factor lookup table. !
   \*--------------------------------------------------------*/
   for(i = 0; i < 2; i++)
   {
      /*--------------------------------------------------------*\
      ! Assign the Lookup Table (LUT), Length and filter_taps    !
      ! pointer to their respective  wavelet kernel and set the  !
      ! filter_bank (Length_FB) and work buffer (Length_Gb)      !
      ! length to the corresponding low-pass synthesis filter    !
      ! length.                                                  !
      \*--------------------------------------------------------*/
      if(i == 0)
      {
         LUT = &DWT_ENERGY_GAIN_LUT[bwc_dwt_9_7][1];
         Length = &DWT_9X7_FILTER_LENGTH[0];
         Length_FB = Length_Gb = Length[0];
         filter_taps = &DWT_9X7_FILTER[0][0];
      }
      else if(i == 1)
      {
         LUT = &DWT_ENERGY_GAIN_LUT[bwc_dwt_5_3][1];
         Length = &DWT_5X3_FILTER_LENGTH[0];
         Length_FB = Length_Gb = Length[0];
         filter_taps = &DWT_5X3_FILTER[0][0];
      }
      /*--------------------------------------------------------*\
      ! Loop through the low- and high-pass synthesis filter     !
      ! banks of the corresponding wavelet kernel.               !
      \*--------------------------------------------------------*/
      for(j = 0; j < 2; j++)
      {
         /*--------------------------------------------------------*\
         ! Initialize work buffer 1 with the low-pass filter for    !
         ! the low-pass energy gain factor, or the high-pass filter !
         ! for the high-pass energy gain factor.                    !
         \*--------------------------------------------------------*/
         for(k = 0; k < Length_Gb + 1; ++k)
         {
            buff1[k] = buff1[2*Length_Gb-k] = filter_taps[k+5*j];
         }
         /*--------------------------------------------------------*\
         ! Calculate the energy gain factor for the first decomp.   !
         ! level by evaluating the square norm of work buffer 1.    !
         \*--------------------------------------------------------*/
         for(k = 0; k < (2 * Length_Gb + 1); ++k)
         {
            LUT[0] += buff1[k] * buff1[k];
         }
         /*--------------------------------------------------------*\
         ! Loop through the remaining decomposition levels.         !
         \*--------------------------------------------------------*/
         for(k = 1; k < MAX_DECOMP_LEVELS; ++k)
         {
            /*--------------------------------------------------------*\
            ! Assemble the low- or high-pass synthesis sequence for    !
            ! decomposition level i in work buffer 2.                  !
            \*--------------------------------------------------------*/
            for(l = 0; l < (2 * Length_Gb + 1); ++l)
            {
               buff2[2 * l + Length_FB]  += buff1[l] * filter_taps[Length_FB];
               for(m = 0; m < Length_FB; ++m)
               {
                  buff2[2 * l + m]               += buff1[l] * filter_taps[m];
                  buff2[2 * l + (2*Length_FB-m)] += buff1[l] * filter_taps[m];
               }
            }
            /*--------------------------------------------------------*\
            ! Assign the memory of work buffer 2 to work buffer 1 and  !
            ! vice versa. Set work buffer 2 to 0 and reevaluate the    !
            ! length of work buffer 1.                                 !
            \*--------------------------------------------------------*/
            temp = buff1;
            buff1 = buff2;
            buff2 = temp;
            memset(buff2, 0, 512 * sizeof(double));
            Length_Gb = 2* Length_Gb + Length_FB;
            /*--------------------------------------------------------*\
            ! Calculate the energy gain factor for decomposition level !
            ! i by evaluating the square norm of work buffer 1.        !
            \*--------------------------------------------------------*/
            for(l = 0; l < (2 * Length_Gb + 1); ++l)
            {
               LUT[k] += buff1[l] * buff1[l];
            }
         }
         /*--------------------------------------------------------*\
         ! Advance the LUT Pointer to the memory reserved for the   !
         ! high-pass energy gain factor and set the length of work  !
         ! buffer 1 to the corresponding high-pass filter length.   !
         \*--------------------------------------------------------*/
         LUT += MAX_DECOMP_LEVELS + 1;
         Length_Gb = Length[1];
      }
      /*--------------------------------------------------------*\
      ! Set work buffer 1 to 0.                                  !
      \*--------------------------------------------------------*/
      memset(buff1, 0, 512 * sizeof(double));
   }

   /*--------------------------------------------------------*\
   ! Loop through the asymmetric wavelet filter banks and as- !
   ! semble their respective energy gain factor lookup table. !
   \*--------------------------------------------------------*/
   for(i = 0; i < 1; ++i)
   {
      /*--------------------------------------------------------*\
      ! Initialize work buffer 1 with the low-pass filter for    !
      ! the low-pass energy gain factor, or the high-pass filter !
      ! for the high-pass energy gain factor.                    !
      \*--------------------------------------------------------*/
      if(i == 0)
      {
         LUT = &DWT_ENERGY_GAIN_LUT[bwc_dwt_haar][1];
         Length = &DWT_HAAR_FILTER_LENGTH[0];
         Length_FB = Length_Gb = Length[0];
         filter_taps = &DWT_HAAR_FILTER[0][0];
      }
      /*--------------------------------------------------------*\
      ! Loop through the low- and high-pass synthesis filter     !
      ! banks of the corresponding wavelet kernel.               !
      \*--------------------------------------------------------*/
      for(j = 0; j < 2; ++j)
      {
         /*--------------------------------------------------------*\
         ! Initialize work buffer 1 with the low-pass filter for    !
         ! the low-pass energy gain factor, or the high-pass filter !
         ! for the high-pass energy gain factor.                    !
         \*--------------------------------------------------------*/
         for(k = 0; k < Length_Gb; ++k)
         {
            buff1[k] = filter_taps[k+5*j];
         }
         /*--------------------------------------------------------*\
         ! Calculate the energy gain factor for the first decomp.   !
         ! level by evaluating the square norm of work buffer 1.    !
         \*--------------------------------------------------------*/
         for(k = 0; k < Length_Gb; ++k)
         {
            LUT[0] += buff1[k] * buff1[k];
         }
         /*--------------------------------------------------------*\
         ! Loop through the remaining decomposition levels.         !
         \*--------------------------------------------------------*/
         for(k = 1; k < MAX_DECOMP_LEVELS; ++k)
         {
            /*--------------------------------------------------------*\
            ! Assemble the low- or high-pass synthesis sequence for    !
            ! decomposition level i in work buffer 2.                  !
            \*--------------------------------------------------------*/
            for(l = 0; l < Length_Gb; ++l)
            {
               for(m = 0; m < Length_FB; m++)
               {
                  buff2[2 * l + m] += buff1[l] * filter_taps[m];
               }
            }
            /*--------------------------------------------------------*\
            ! Assign the memory of work buffer 2 to work buffer 1 and  !
            ! vice versa. Set work buffer 2 to 0 and reevaluate the    !
            ! length of work buffer 1.                                 !
            \*--------------------------------------------------------*/
            temp = buff1;
            buff1 = buff2;
            buff2 = temp;
            memset(buff2, 0, 512 * sizeof(double));
            Length_Gb = Length_Gb * Length_FB;
            /*--------------------------------------------------------*\
            ! Calculate the energy gain factor for decomposition level !
            ! i by evaluating the square norm of work buffer 1.        !
            \*--------------------------------------------------------*/
            for(l = 0; l < Length_Gb; ++l)
            {
               LUT[k] += buff1[l] * buff1[l];
            }
         }
         /*--------------------------------------------------------*\
         ! Calculate the energy gain factor for decomposition level !
         ! i by evaluating the square norm of work buffer 1.        !
         \*--------------------------------------------------------*/
         LUT += MAX_DECOMP_LEVELS + 1;
         Length_Gb = Length[1];
      }
      /*--------------------------------------------------------*\
      ! Set work buffer 1 to 0.                                  !
      \*--------------------------------------------------------*/
      memset(buff1, 0, 512 * sizeof(double));
   }
   /*--------------------------------------------------------*\
   ! Free work buffer 1 and 2 and exit the subroutine.        !
   \*--------------------------------------------------------*/
   free(buff1);
   free(buff2);
   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: bwc_float get_dwt_energy_gain(bwc_field *const field, uchar highband_flag, uint16 level)  !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function evaluates the energy gain factor according to the the specified decom-        !
!                position level. For decomposition levels larger than MAX_DECOMP_LEVELS the filter gain      !
!                for the extra levels is approximated by multiplying the energy gain factor by 2.            !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                highband_flag               uchar                 - Defines the subband for which Gb is     !
!                                                                    calculated.                             !
!                                                                                                            !
!                level                       uint16                - Defines the decomposition level for     !
!                                                                    which Gb is calculated                  !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                double                    - Energy gain factor for two dimensional tree-structured subband  !
!                                            transforms.                                                     !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                13.04.2017  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
bwc_float
get_dwt_energy_gain(bwc_field *const field, uchar highband_flag, uint16 level)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int8           level_X, level_Y, level_Z;
   int8           level_TS;

   /*-----------------------*\
   ! DEFINE FLOAT VARIABLES: !
   \*-----------------------*/
   double         Gb = 1.0f;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_ctrl   *control;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(level <= field->control.nDecomp + 1);
   assert(highband_flag <= DIM_ALL);

   /*--------------------------------------------------------*\
   ! Save the global control structure to a temporary varia-  !
   ! ble to make the code more readable.                      !
   \*--------------------------------------------------------*/
   control = &field->control;

   /*--------------------------------------------------------*\
   ! Evaluate the number of decompositions in each temporal & !
   ! spatial directions according to the the global decompo - !
   ! sition level for which the energy gain factor is calcu-  !
   ! lated for.                                               !
   \*--------------------------------------------------------*/
   level_X  = (level > control->decompX ) ? control->decompX  : level;
   level_Y  = (level > control->decompY ) ? control->decompY  : level;
   level_Z  = (level > control->decompZ ) ? control->decompZ  : level;
   level_TS = (level > control->decompTS) ? control->decompTS : level;

   /*--------------------------------------------------------*\
   ! Multiply the energy gain factor with the filter gain of  !
   ! the wavelet kernel applied along the X-axis. For decom-  !
   ! position levels larger than MAX_DECOMP_LEVELS the energy !
   ! gain for the extra levels is approximated by multiplying !
   ! it by 2.                                                 !
   \*--------------------------------------------------------*/
   if(level_X  != 0)
   {
      while(level_X > MAX_DECOMP_LEVELS)
      {
         Gb *= 2.0f;
         level_X--;
      }

      Gb *= DWT_ENERGY_GAIN_LUT[control->KernelX][level_X + ((highband_flag & DIM_X) * (MAX_DECOMP_LEVELS + 1))];
   } 

   /*--------------------------------------------------------*\
   ! Multiply the energy gain factor with the filter gain of  !
   ! the wavelet kernel applied along the Y-axis. For decom-  !
   ! position levels larger than MAX_DECOMP_LEVELS the energy !
   ! gain for the extra levels is approximated by multiplying !
   ! it by 2.                                                 !
   \*--------------------------------------------------------*/
   if(level_Y  != 0)
   {
      while(level_Y > MAX_DECOMP_LEVELS)
      {
         Gb *= 2.0f;
         level_Y--;
      }

      Gb *= DWT_ENERGY_GAIN_LUT[control->KernelY][level_Y + (((highband_flag & DIM_Y) >> 1) * (MAX_DECOMP_LEVELS + 1))];
   }

   /*--------------------------------------------------------*\
   ! Multiply the energy gain factor with the filter gain of  !
   ! the wavelet kernel applied along the Z-axis. For decom-  !
   ! position levels larger than MAX_DECOMP_LEVELS the energy !
   ! gain for the extra levels is approximated by multiplying !
   ! it by 2.                                                 !
   \*--------------------------------------------------------*/
   if(level_Z  != 0)
   {
      while(level_Z > MAX_DECOMP_LEVELS)
      {
         Gb *= 2.0f;
         level_Z--;
      }

      Gb *= DWT_ENERGY_GAIN_LUT[control->KernelZ][level_Z + (((highband_flag & DIM_Z) >> 2) * (MAX_DECOMP_LEVELS + 1))];
   }

   /*--------------------------------------------------------*\
   ! Multiply the energy gain factor with the filter gain of  !
   ! the wavelet kernel applied along the TS-axis. For decom- !
   ! position levels larger than MAX_DECOMP_LEVELS the energy !
   ! gain for the extra levels is approximated by multiplying !
   ! it by 2.                                                 !
   \*--------------------------------------------------------*/
   if(level_TS != 0)
   {
      while(level_TS > MAX_DECOMP_LEVELS)
      {
         Gb *= 2.0f;
         level_TS--;
      }

      Gb *= DWT_ENERGY_GAIN_LUT[control->KernelTS][level_TS + (((highband_flag & DIM_TS) >> 3) * (MAX_DECOMP_LEVELS + 1))];   
   }
   return (bwc_float)Gb;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uchar forward_discrete_wavelet_transform(bwc_field *const field,                          !
!   --------------                                          bwc_parameter *const parameter)                  !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function performs the forward discrete wavelet transform on the current tile param-    !
!                eter. After loading the flow field samples for a row, column or spatial/temporal slice      !
!                into a working buffer, a boundary extension operation is performed to ensure an approp.     !
!                sample base. The working buffer is then transform using the wavelet kernel selected for     !
!                the spatial or temporal dimension, sorted into high- and low-frequency samples and flushed  !
!                back into the tile parameter memory block. This operation is performed for every row,       !
!                column and spatial/temporal slice for nDecomp number of decomposition levels.               !
!                                                                                                            !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                parameter                   bwc_parameter*        - Structure defining a bwc parameter.     !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                uchar                     - Returns an unsigned char for error handling.                    !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                13.04.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
forward_wavelet_transform(bwc_field *const field, bwc_parameter *const parameter)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   incr_X, incr_Y, incr_Z;
   uint64   rX0, rY0, rZ0;
   uint64   rX1, rY1, rZ1;
   uint64   width, height, depth;
   uint64   x, y, z;
   uint32   buff_size;
   int16    i;
   uint16   incr_TS;
   uint16   rTS0;
   uint16   rTS1;
   uint16   dt;
   uint16   t;
   uint8    id;
   uint8    filter_tapsX, filter_tapsY, filter_tapsZ;
   uint8    filter_tapsTS;
   uint8    level;
   uint8    nThreads;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_ctrl       *control;
   bwc_param_inf     *param_info;
   bwc_sample        *data, *working_buffer;
   bwc_sample       **memory;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(parameter);

   /*-----------------------*\
   ! DEFINE FUNCTION PTR:    !
   \*-----------------------*/
   void (*forward_wavelet_transform[3])(bwc_sample*, uint64, uint64) = {forward_9x7_CDF_wavelet_transform, 
                                                                        forward_5x3_LeGall_wavelet_transform, 
                                                                        forward_Haar_wavelet_transform};

   void (*boundary_extension[3])(bwc_sample *, uint64, uint64, uint8) = {whole_point_symmetric_extend,
                                                                         whole_point_symmetric_extend,
                                                                         NULL};

   /*--------------------------------------------------------*\
   ! Save the global control and parameter info structure to  !
   ! temporary variables to make the code more readable.      !
   \*--------------------------------------------------------*/
   control       = &field->control;
   param_info    = &parameter->info;

   /*--------------------------------------------------------*\
   ! Calculate the width, height, depth and temporal size dt  !
   ! of the current tile parameter.                           !
   \*--------------------------------------------------------*/
   width  = param_info->X1  - param_info->X0;
   height = param_info->Y1  - param_info->Y0;
   depth  = param_info->Z1  - param_info->Z0;
   dt     = param_info->TS1 - param_info->TS0;

   /*--------------------------------------------------------*\
   ! Calculate the increments used to fill the working buffer !
   ! for the spacial/temporal wavelet transformation.         !
   \*--------------------------------------------------------*/
   incr_X  = 1;  
   incr_Y  = width;
   incr_Z  = width * height;
   incr_TS = width * height * depth;

   /*--------------------------------------------------------*\
   ! Evaluate the filter taps for the spatial and temporal    !
   ! wavelet filters. Additionally, the filter taps are in-   !
   ! creased by 2 to account for an increase in highband      !
   ! coefficients in case of an odd-length dataset.           !
   \*--------------------------------------------------------*/
   filter_tapsX  = get_filter_taps(control->KernelX)  + 2;
   filter_tapsY  = get_filter_taps(control->KernelY)  + 2;
   filter_tapsZ  = get_filter_taps(control->KernelZ)  + 2;
   filter_tapsTS = get_filter_taps(control->KernelTS) + 2;

   /*--------------------------------------------------------*\
   ! Evaluate the number of threads for the current compres-  !
   ! sion run and the buffer size for the current tile param- !
   ! eter.                                                    !
   \*--------------------------------------------------------*/
   #if defined (_OPENMP)
      nThreads = omp_get_max_threads();
   #else
      nThreads = 1;
   #endif

   buff_size = MAX(MAX(width, height), MAX(depth, dt)) + 2 * 
                  MAX(MAX(filter_tapsX, filter_tapsY), 
                     MAX(filter_tapsZ, filter_tapsTS));

   /*--------------------------------------------------------*\
   ! Allocate the memory array and memory blocks for the      !
   ! working buffer for each OpenMP thread. In case of a      !
   ! serial run only one memory block will be allocated.      !
   \*--------------------------------------------------------*/
   memory = calloc(nThreads, sizeof(bwc_sample*));
   if(!memory) 
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   for(i = 0; i < nThreads; ++i)
   {
         memory[i] = calloc(buff_size, sizeof(bwc_sample));
         if(!memory[i]) 
         {
            // memory allocation error
            fprintf(stderr, MEMERROR);
            for(i--; i > 0; --i)
            {
               free(memory[i]);
            }
            free(memory);
            return 1;
         }
   }

   /*--------------------------------------------------------*\
   ! Walk through all decomposition levels and apply the 1D   !
   ! wavelet transform along the spatial and temporal dimen-  !
   ! sions specified in the control structure.                !
   \*--------------------------------------------------------*/
   #if defined (_OPENMP)
      #pragma omp parallel private(data, id, level, working_buffer, rX0, rX1, rY0, rY1, rZ0, rZ1, rTS0, rTS1)
   #endif
   {
      /*--------------------------------------------------------*\
      ! Evaluate the id of the current OpenMP thread. In case of !
      ! a serial run the thread id is set to 0.                  !
      \*--------------------------------------------------------*/
      #if defined (_OPENMP)
         id = omp_get_thread_num();
      #else
         id = 0;
      #endif

      for(level = 0; level < control->nDecomp; ++level)
      {
         /*--------------------------------------------------------*\
         ! Save the resolution starting and end points to a tempo-  !
         ! rary variable to make the code more readable.            !
         \*--------------------------------------------------------*/
         rX0  = (uint64)parameter->resolution[control->nDecomp - level].info.X0;
         rX1  = (uint64)parameter->resolution[control->nDecomp - level].info.X1;
         rY0  = (uint64)parameter->resolution[control->nDecomp - level].info.Y0;
         rY1  = (uint64)parameter->resolution[control->nDecomp - level].info.Y1;
         rZ0  = (uint64)parameter->resolution[control->nDecomp - level].info.Z0;
         rZ1  = (uint64)parameter->resolution[control->nDecomp - level].info.Z1;
         rTS0 = (uint16)parameter->resolution[control->nDecomp - level].info.TS0;
         rTS1 = (uint16)parameter->resolution[control->nDecomp - level].info.TS1;

         if(level < control->decompX  && (rX1 - rX0) > 1)
         {
            /*--------------------------------------------------------*\
            ! Associate the working buffer pointer to the allocated    !
            ! memory block. The working buffer points to the appropri- !
            ! ate starting point in the memory block, accounting for   !
            ! the taps of the wavelet kernel selected for the first    !
            ! spatial dimension.                                       !
            \*--------------------------------------------------------*/
            working_buffer = &memory[id][filter_tapsX];

            /*--------------------------------------------------------*\
            ! Adjust the working buffer pointer in case of an odd      !
            ! length column starting point.                            !
            \*--------------------------------------------------------*/
            if(rX0 % 2)
            {
               ++working_buffer;
            }

            /*--------------------------------------------------------*\
            ! Walk trough all the temporal and spatial slices as well  !
            ! as rows.                                                 !
            \*--------------------------------------------------------*/
            #if defined (_OPENMP)
               #pragma omp for collapse(3)
            #endif
            for(t = 0; t < (rTS1 - rTS0); ++t)
            {
               for(z = 0; z < (rZ1 - rZ0); ++z)
               {
                  for(y = 0; y < (rY1 - rY0); ++y)
                  {
                     /*--------------------------------------------------------*\
                     ! Advance the data pointer to the next column.             !
                     \*--------------------------------------------------------*/
                     data = parameter->data + y * incr_Y + z * incr_Z + t * incr_TS;

                     /*--------------------------------------------------------*\
                     ! In the event that the current column and resolution only !
                     ! encompasses 1 value, the sample will remain unaltered.   !
                     ! In case of an odd starting point the sample will be      !
                     ! doubled.                                                 !
                     \*--------------------------------------------------------*/
                     if((rX1 - rX0) == 1)
                     {
                        if(rX0 & 2)
                        {
                           data[0].f *=  2;
                        }
                        continue;
                     }

                     /*--------------------------------------------------------*\
                     ! Fill the working buffer with the y-th tile parameter     !
                     ! column.                                                  !
                     \*--------------------------------------------------------*/
                     fill_forward_buffer(data, &memory[id][filter_tapsX], rX0, rX1, incr_X);

                     /*--------------------------------------------------------*\
                     ! Apply the boundary extension on the working buffer ap-   !
                     ! propriate to the specified wavelet kernel.               !
                     \*--------------------------------------------------------*/
                     boundary_extension[control->KernelX](&memory[id][filter_tapsX], rX0, rX1, filter_tapsX);

                     /*--------------------------------------------------------*\
                     ! Perform the forward wavelet transform on the current     !
                     ! column.                                                  !
                     \*--------------------------------------------------------*/
                     forward_wavelet_transform[control->KernelX](working_buffer, rX0, rX1);

                     /*--------------------------------------------------------*\
                     ! Copy the interleaved wavelet coefficients from the work- !
                     ! ing buffer to the tile parameter memory.                 !
                     \*--------------------------------------------------------*/
                     buffer_flush_forward(data, working_buffer, rX0, rX1, incr_X);
                  }
               }
            }
         }

         if(level < control->decompY && (rY1 - rY0) > 1)
         {
            /*--------------------------------------------------------*\
            ! Associate the working buffer pointer to the allocated    !
            ! memory block. The working buffer points to the appropri- !
            ! ate starting point in the memory block, accounting for   !
            ! for the taps of the wavelet kernel selected for the      !
            ! second spatial dimension.                                !
            \*--------------------------------------------------------*/
            working_buffer = &memory[id][filter_tapsY];
            
            /*--------------------------------------------------------*\
            ! Adjust the working buffer pointer in case of an odd      !
            ! length row starting point.                               !
            \*--------------------------------------------------------*/
            if(rY0 % 2)
            {
               working_buffer++;
            }

            /*--------------------------------------------------------*\
            ! Walk trough all the temporal and spatial slices as well  !
            ! as columns.                                              !
            \*--------------------------------------------------------*/
            #if defined (_OPENMP)
               #pragma omp for collapse(3)
            #endif
            for(t = 0; t < (rTS1 - rTS0); ++t)
            {
               for(z = 0; z < (rZ1 - rZ0); ++z)
               {
                  for(x = 0; x < (rX1 - rX0); ++x)
                  {
                     /*--------------------------------------------------------*\
                     ! Advance the data pointer to the next row.                !
                     \*--------------------------------------------------------*/
                     data = parameter->data + x * incr_X + z * incr_Z + t * incr_TS;

                     /*--------------------------------------------------------*\
                     ! In the event that the current row and resolution only    !
                     ! encompasses 1 value, the sample will remain unaltered.   !
                     ! In case of an odd starting point the sample will be      !
                     ! doubled.                                                 !
                     \*--------------------------------------------------------*/
                     if((rY1 - rY0) == 1)
                     {
                        if(rY0 & 2)
                        {
                           data[0].f *=  2;
                        }
                        continue;
                     }

                     /*--------------------------------------------------------*\
                     ! Fill the working buffer with the x-th tile parameter     !
                     ! row.                                                     !
                     \*--------------------------------------------------------*/
                     fill_forward_buffer(data, &memory[id][filter_tapsY], rY0, rY1, incr_Y);

                     /*--------------------------------------------------------*\
                     ! Apply the boundary extension on the working buffer ap-   !
                     ! propriate to the specified wavelet kernel.               !
                     \*--------------------------------------------------------*/
                     boundary_extension[control->KernelY](&memory[id][filter_tapsY], rY0, rY1, filter_tapsY);

                     /*--------------------------------------------------------*\
                     ! Perform the forward wavelet transform on the current     !
                     ! row.                                                     !
                     \*--------------------------------------------------------*/
                     forward_wavelet_transform[control->KernelY](working_buffer, rY0, rY1);

                     /*--------------------------------------------------------*\
                     ! Copy the interleaved wavelet coefficients from the work- !
                     ! ing buffer to the tile parameter memory.                 !
                     \*--------------------------------------------------------*/
                     buffer_flush_forward(data, working_buffer, rY0, rY1, incr_Y);
                  }
               }
            }
         }

         if(level < control->decompZ && (rZ1 - rZ0) > 1)
         {
            /*--------------------------------------------------------*\
            ! Associate the working buffer pointer to the allocated    !
            ! memory block. The working buffer points to the appropri- !
            ! ate starting point in the memory block, accounting for   !
            ! for the taps of the wavelet kernel selected for the      !
            ! third spatial dimension.                                 !
            \*--------------------------------------------------------*/
            working_buffer = &memory[id][filter_tapsZ];

            /*--------------------------------------------------------*\
            ! Adjust the working buffer pointer in case of an odd      !
            ! length starting point of the spatial slice.              !
            \*--------------------------------------------------------*/
            if(rZ0 % 2)
            {
               working_buffer++;
            }

            /*--------------------------------------------------------*\
            ! Walk trough all the temporal slices, rows and columns.   !
            \*--------------------------------------------------------*/
            #if defined (_OPENMP)
               #pragma omp for collapse(3)
            #endif
            for(t = 0; t < (rTS1 - rTS0); ++t)
            {
               for(y = 0; y < (rY1 - rY0); ++y)
               {
                  for(x = 0; x < (rX1 - rX0); ++x)
                  {
                     /*--------------------------------------------------------*\
                     ! Advance the data pointer to the next spatial slice.      !
                     \*--------------------------------------------------------*/
                     data = parameter->data + x * incr_X + y * incr_Y + t * incr_TS;

                     /*--------------------------------------------------------*\
                     ! In the event that the current slice and resolution only  !
                     ! encompasses 1 value, the sample will remain unaltered.   !
                     ! In case of an odd starting point the sample will be      !
                     ! doubled.                                                 !
                     \*--------------------------------------------------------*/
                     if((rZ1 - rZ0) == 1)
                     {
                        if(rZ0 & 2)
                        {
                           data[0].f *=  2;
                        }
                        continue;
                     }

                     /*--------------------------------------------------------*\
                     ! Fill the working buffer with the x-th spatial tile para- !
                     ! meter slice.                                             !
                     \*--------------------------------------------------------*/
                     fill_forward_buffer(data, &memory[id][filter_tapsZ], rZ0, rZ1, incr_Z);

                     /*--------------------------------------------------------*\
                     ! Apply the boundary extension on the working buffer ap-   !
                     ! propriate to the specified wavelet kernel.               !
                     \*--------------------------------------------------------*/
                     boundary_extension[control->KernelZ](&memory[id][filter_tapsZ], rZ0, rZ1, filter_tapsZ);

                     /*--------------------------------------------------------*\
                     ! Perform the forward wavelet transform on the current     !
                     ! spatial slice.                                           !
                     \*--------------------------------------------------------*/
                     forward_wavelet_transform[control->KernelZ](working_buffer, rZ0, rZ1);

                     /*--------------------------------------------------------*\
                     ! Copy the interleaved wavelet coefficients from the work- !
                     ! ing buffer to the tile parameter memory.                 !
                     \*--------------------------------------------------------*/
                     buffer_flush_forward(data, working_buffer, rZ0, rZ1, incr_Z);
                  }
               }
            }
         }

         if(level < control->decompTS && (rTS1 - rTS0) > 1)
         {
            /*--------------------------------------------------------*\
            ! Associate the working buffer pointer to the allocated    !
            ! memory block. The working buffer points to the appropri- !
            ! ate starting point in the memory block, accounting for   !
            ! for the taps of the wavelet kernel selected for the      !
            ! temporal dimension.                                      !
            \*--------------------------------------------------------*/
            working_buffer = &memory[id][filter_tapsTS];

            /*--------------------------------------------------------*\
            ! Adjust the working buffer pointer in case of an odd      !
            ! length starting point of the temporal slice.             !
            \*--------------------------------------------------------*/
            if(rTS0 % 2)
            {
               working_buffer++;
            }

            /*--------------------------------------------------------*\
            ! Walk trough all the spatial slices, rows and columns.    !
            \*--------------------------------------------------------*/
            #if defined (_OPENMP)
               #pragma omp for collapse(3)
            #endif
            for(z = 0; z < (rZ1 - rZ0); ++z)
            {
               for(y = 0; y < (rY1 - rY0); ++y)
               {
                  for(x = 0; x < (rX1 - rX0); ++x)
                  {
                     /*--------------------------------------------------------*\
                     ! Advance the data pointer to the next temporal slice.     !
                     \*--------------------------------------------------------*/
                     data = parameter->data + x * incr_X + y * incr_Y + z * incr_Z;

                     /*--------------------------------------------------------*\
                     ! In the event that the current slice and resolution only  !
                     ! encompasses 1 value, the sample will remain unaltered.   !
                     ! In case of an odd starting point the sample will be      !
                     ! doubled.                                                 !
                     \*--------------------------------------------------------*/
                     if((rTS1 - rTS0) == 1)
                     {
                        if(rTS0 & 2)
                        {
                           data[0].f *=  2;
                        }
                        continue;
                     }

                     /*--------------------------------------------------------*\
                     ! Fill the working buffer with the x-th temporal tile para-!
                     ! meter slice.                                             !
                     \*--------------------------------------------------------*/
                     fill_forward_buffer(data, &memory[id][filter_tapsTS], rTS0, rTS1, incr_TS);

                     /*--------------------------------------------------------*\
                     ! Apply the boundary extension on the working buffer ap-   !
                     ! propriate to the specified wavelet kernel.               !
                     \*--------------------------------------------------------*/
                     boundary_extension[control->KernelTS](&memory[id][filter_tapsTS], rTS0, rTS1, filter_tapsTS);

                     /*--------------------------------------------------------*\
                     ! Perform the forward wavelet transform on the current     !
                     ! temporal slice.                                          !
                     \*--------------------------------------------------------*/
                     forward_wavelet_transform[control->KernelTS](working_buffer, rTS0, rTS1);

                     /*--------------------------------------------------------*\
                     ! Copy the interleaved wavelet coefficients from the work- !
                     ! ing buffer to the tile parameter memory.                 !
                     \*--------------------------------------------------------*/
                     buffer_flush_forward(data, working_buffer, rTS0, rTS1, incr_TS);
                  }
               }
            }
         }
      }
   }

   /*--------------------------------------------------------*\
   ! Deallocate the memory array and memory blocks.           !
   \*--------------------------------------------------------*/
   for(i = 0; i < nThreads; ++i)
   {
      free(memory[i]);
   }
   free(memory);

   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uchar inverse_discrete_wavelet_transform(bwc_field *const field,                          !
!   --------------                                          bwc_parameter *const parameter)                  !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function performs the inverse discrete wavelet transform on the current tile param-    !
!                eter. After loading the interweaved wavelet coefficients for a row, column or spatial/      !
!                temporal slice into a working buffer, a boundary extension operation is performed to        !
!                ensure an appropriate sample base. The working buffer is then transform using the wave-     !
!                let kernel selected for the spatial or temporal dimension and flushed back into the tile    !
!                parameter memory block. This operation is performed for every row, column and spatial/      !
!                temporal slice for nDecomp number of decomposition levels.                                  !
!                                                                                                            !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                parameter                   bwc_parameter*        - Structure defining a bwc parameter.     !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                uchar                     - Returns an unsigned char for error handling.                    !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                13.04.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
inverse_wavelet_transform(bwc_field *const field, bwc_parameter *const parameter)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   incr_X, incr_Y, incr_Z;
   uint64   rX0, rY0, rZ0;
   uint64   rX1, rY1, rZ1;
   uint64   width, height, depth;
   uint64   x, y, z;
   uint32   buff_size;
   int16    i;
   uint16   incr_TS;
   uint16   rTS0;
   uint16   rTS1;
   uint16   dt;
   uint16   t;
   uint8    id;
   uint8    filter_tapsX, filter_tapsY, filter_tapsZ;
   uint8    filter_tapsTS;
   uint8    level;
   uint8    nThreads;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_ctrl       *control;
   bwc_param_inf     *param_info;
   bwc_sample        *data, *working_buffer;
   bwc_sample       **memory;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(parameter);

   /*-----------------------*\
   ! DEFINE FUNCTION PTR:    !
   \*-----------------------*/
   void (*inverse_wavelet_transform[3])(bwc_sample*, uint64, uint64) = {inverse_9x7_CDF_wavelet_transform, 
                                                                        inverse_5x3_LeGall_wavelet_transform, 
                                                                        inverse_Haar_wavelet_transform};

   void (*boundary_extension[3])(bwc_sample *, uint64, uint64, uint8) = {whole_point_symmetric_extend,
                                                                         whole_point_symmetric_extend,
                                                                         NULL};

   /*--------------------------------------------------------*\
   ! Save the global control and parameter info structure to  !
   ! temporary variables to make the code more readable.      !
   \*--------------------------------------------------------*/
   control    = &field->control;
   param_info = &parameter->info;

   /*--------------------------------------------------------*\
   ! Calculate the width, height, depth and temporal size dt  !
   ! of the current tile parameter.                           !
   \*--------------------------------------------------------*/
   width  = param_info->X1  - param_info->X0;
   height = param_info->Y1  - param_info->Y0;
   depth  = param_info->Z1  - param_info->Z0;
   dt     = param_info->TS1 - param_info->TS0;

   /*--------------------------------------------------------*\
   ! Calculate the increments used to fill the working buffer !
   ! for the spacial/temporal wavelet transforms.             !
   \*--------------------------------------------------------*/
   incr_X  = 1;  
   incr_Y  = width;
   incr_Z  = width * height;
   incr_TS = width * height * depth;

   /*--------------------------------------------------------*\
   ! Evaluate the filter taps for the spatial and temporal    !
   ! wavelet filters. Additionally, the filter taps are in-   !
   ! creased by 2 to account for an increase in highband      !
   ! coefficients in case of an odd-length dataset.           !
   \*--------------------------------------------------------*/
   filter_tapsX  = get_filter_taps(control->KernelX)  + 2;
   filter_tapsY  = get_filter_taps(control->KernelY)  + 2;
   filter_tapsZ  = get_filter_taps(control->KernelZ)  + 2;
   filter_tapsTS = get_filter_taps(control->KernelTS) + 2;

   /*--------------------------------------------------------*\
   ! Evaluate the number of threads for the current compres-  !
   ! sion run and the buffer size for the current tile param- !
   ! eter.                                                    !
   \*--------------------------------------------------------*/
   #if defined (_OPENMP)
      nThreads = omp_get_max_threads();
   #else
      nThreads = 1;
   #endif

   buff_size = MAX(MAX(width, height), MAX(depth, dt)) + 2 * 
                  MAX(MAX(filter_tapsX, filter_tapsY), 
                     MAX(filter_tapsZ, filter_tapsTS));

   /*--------------------------------------------------------*\
   ! Allocate the memory array and memory blocks for the      !
   ! working buffer of each OpenMP thread. In case of a       !
   ! serial run only one memory block will be allocated.      !
   \*--------------------------------------------------------*/
   memory = calloc(nThreads, sizeof(bwc_sample*));
   if(!memory) 
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   for(i = 0; i < nThreads; ++i)
   {
         memory[i] = calloc(buff_size, sizeof(bwc_sample));
         if(!memory[i]) 
         {
            // memory allocation error
            fprintf(stderr, MEMERROR);
            for(i--; i > 0; --i)
            {
               free(memory[i]);
            }
            free(memory);
            return 1;
         }
   }

   /*--------------------------------------------------------*\
   ! Walk through all decomposition levels and apply the 1D   !
   ! wavelet transform along the spatial and temporal dimen-  !
   ! sions specified in the control structure.                !
   \*--------------------------------------------------------*/
   #if defined (_OPENMP)
      #pragma omp parallel private(data, id, level, working_buffer, rX0, rX1, rY0, rY1, rZ0, rZ1, rTS0, rTS1)
   #endif
   {
      /*--------------------------------------------------------*\
      ! Evaluate the id of the current OpenMP thread. In case of !
      ! a serial run the thread id is set to 1.                  !
      \*--------------------------------------------------------*/
      #if defined (_OPENMP)
         id = omp_get_thread_num();
      #else
         id = 0;
      #endif

      for(level = control->nDecomp; level --> 0;)
      {
         /*--------------------------------------------------------*\
         ! Save the resolution starting and end points to a tempo-  !
         ! rary variable to make the code more readable.            !
         \*--------------------------------------------------------*/
         rX0  = (uint64)parameter->resolution[control->nDecomp - level].info.X0;
         rX1  = (uint64)parameter->resolution[control->nDecomp - level].info.X1;
         rY0  = (uint64)parameter->resolution[control->nDecomp - level].info.Y0;
         rY1  = (uint64)parameter->resolution[control->nDecomp - level].info.Y1;
         rZ0  = (uint64)parameter->resolution[control->nDecomp - level].info.Z0;
         rZ1  = (uint64)parameter->resolution[control->nDecomp - level].info.Z1;
         rTS0 = (uint16)parameter->resolution[control->nDecomp - level].info.TS0;
         rTS1 = (uint16)parameter->resolution[control->nDecomp - level].info.TS1;

         if(level < control->decompTS && (rTS1 - rTS0) > 1)
         {
            /*--------------------------------------------------------*\
            ! Associate the working buffer pointer to the allocated    !
            ! memory block. The working buffer points to the appropri- !
            ! ate starting point in the memory block, accounting for   !
            ! the taps of the wavelet kernel selected for the temporal !
            ! dimension.                                               !
            \*--------------------------------------------------------*/
            working_buffer = &memory[id][filter_tapsTS];

            /*--------------------------------------------------------*\
            ! Adjust the working buffer pointer in case of an odd      !
            ! length starting point of the temporal slice.             !
            \*--------------------------------------------------------*/
            if(rTS0 % 2)
            {
               working_buffer++;
            }

            /*--------------------------------------------------------*\
            ! Walk trough all the spatial slices, rows and columns.    !
            \*--------------------------------------------------------*/
            #if defined (_OPENMP)
               #pragma omp for collapse(3)
            #endif
            for(z = 0; z < (rZ1 - rZ0); ++z)
            {
               for(y = 0; y < (rY1 - rY0); ++y)
               {
                  for(x = 0; x < (rX1 - rX0); ++x)
                  {
                     /*--------------------------------------------------------*\
                     ! Advance the data pointer to the next temporal slice.     !
                     \*--------------------------------------------------------*/
                     data = parameter->data + x * incr_X + y * incr_Y + z * incr_Z;

                     /*--------------------------------------------------------*\
                     ! In the event that the current slice and resolution only  !
                     ! encompasses 1 value, the sample will remain unaltered.   !
                     ! In case of an odd starting point the sample will be      !
                     ! doubled.                                                 !
                     \*--------------------------------------------------------*/
                     if((rTS1 - rTS0) == 1)
                     {
                        if(rTS0 & 2)
                        {
                           data[0].f *=  2;
                        }
                        continue;
                     }

                     /*--------------------------------------------------------*\
                     ! Fill the working buffer with the x-th temporal tile para-!
                     ! meter slice.                                             !
                     \*--------------------------------------------------------*/
                     fill_inverse_buffer(data, &memory[id][filter_tapsTS], rTS0, rTS1, incr_TS);

                     /*--------------------------------------------------------*\
                     ! Apply the boundary extension on the working buffer ap-   !
                     ! propriate to the specified wavelet kernel.               !
                     \*--------------------------------------------------------*/
                     boundary_extension[control->KernelTS](&memory[id][filter_tapsTS], rTS0, rTS1, filter_tapsTS);

                     /*--------------------------------------------------------*\
                     ! Perform the inverse wavelet transform on the current     !
                     ! temporal slice.                                          !
                     \*--------------------------------------------------------*/
                     inverse_wavelet_transform[control->KernelTS](working_buffer, rTS0, rTS1);

                     /*--------------------------------------------------------*\
                     ! Copy the interleaved wavelet coefficients from the work- !
                     ! ing buffer to the tile parameter memory.                 !
                     \*--------------------------------------------------------*/
                     buffer_flush_inverse(data, working_buffer, rTS0, rTS1, incr_TS);
                  }
               }
            }
         }

         if(level < control->decompZ && (rZ1 - rZ0) > 1)
         {
            /*--------------------------------------------------------*\
            ! Associate the working buffer pointer to the allocated    !
            ! memory block. The working buffer points to the appropri- !
            ! ate starting point in the memory block, accounting for   !
            ! for the taps of the wavelet kernel selected for the      !
            ! third spatial dimension.                                 !
            \*--------------------------------------------------------*/
            working_buffer = &memory[id][filter_tapsZ];

            /*--------------------------------------------------------*\
            ! Adjust the working buffer pointer in case of an odd      !
            ! length starting point of the spatial slice.              !
            \*--------------------------------------------------------*/
            if(rZ0 % 2)
            {
               working_buffer++;
            }

            /*--------------------------------------------------------*\
            ! Walk trough all the temporal slices, rows and columns.   !
            \*--------------------------------------------------------*/
            #if defined (_OPENMP)
               #pragma omp for collapse(3)
            #endif
            for(t = 0; t < (rTS1 - rTS0); ++t)
            {
               for(y = 0; y < (rY1 - rY0); ++y)
               {
                  for(x = 0; x < (rX1 - rX0); ++x)
                  {
                     /*--------------------------------------------------------*\
                     ! Advance the data pointer to the next spatial slice.      !
                     \*--------------------------------------------------------*/
                     data = parameter->data + x * incr_X + y * incr_Y + t * incr_TS;

                     /*--------------------------------------------------------*\
                     ! In the event that the current slice and resolution only  !
                     ! encompasses 1 value, the sample will remain unaltered.   !
                     ! In case of an odd starting point the sample will be      !
                     ! doubled.                                                 !
                     \*--------------------------------------------------------*/
                     if((rZ1 - rZ0) == 1)
                     {
                        if(rZ0 & 2)
                        {
                           data[0].f /=  2;
                        }
                        continue;
                     }

                     /*--------------------------------------------------------*\
                     ! Fill the working buffer with the x-th spatial tile para- !
                     ! meter slice.                                             !
                     \*--------------------------------------------------------*/
                     fill_inverse_buffer(data, &memory[id][filter_tapsZ], rZ0, rZ1, incr_Z);

                     /*--------------------------------------------------------*\
                     ! Apply the boundary extension on the working buffer ap-   !
                     ! propriate to the specified wavelet kernel.               !
                     \*--------------------------------------------------------*/
                     boundary_extension[control->KernelZ](&memory[id][filter_tapsZ], rZ0, rZ1, filter_tapsZ);

                     /*--------------------------------------------------------*\
                     ! Perform the inverse wavelet transform on the current     !
                     ! spatial slice.                                           !
                     \*--------------------------------------------------------*/
                     inverse_wavelet_transform[control->KernelZ](working_buffer, rZ0, rZ1);

                     /*--------------------------------------------------------*\
                     ! Copy the decompressed flow field samples from the work-  !
                     ! ing buffer to the tile parameter memory.                 !
                     \*--------------------------------------------------------*/
                     buffer_flush_inverse(data, working_buffer, rZ0, rZ1, incr_Z);
                  }
               }
            }
         }

         if(level < control->decompY && (rY1 - rY0) > 1)
         {
            /*--------------------------------------------------------*\
            ! Associate the working buffer pointer to the allocated    !
            ! memory block. The working buffer points to the appropri- !
            ! ate starting point in the memory block, accounting for   !
            ! for the taps of the wavelet kernel selected for the      !
            ! second spatial dimension.                                !
            \*--------------------------------------------------------*/
            working_buffer = &memory[id][filter_tapsY];

            /*--------------------------------------------------------*\
            ! Adjust the working buffer pointer in case of an odd      !
            ! length row starting point.                               !
            \*--------------------------------------------------------*/
            if(rY0 % 2)
            {
               working_buffer++;
            }

            /*--------------------------------------------------------*\
            ! Walk trough all the temporal and spatial slices as well  !
            ! as columns.                                              !
            \*--------------------------------------------------------*/
            #if defined (_OPENMP)
               #pragma omp for collapse(3)
            #endif
            for(t = 0; t < (rTS1 - rTS0); ++t)
            {
               for(z = 0; z < (rZ1 - rZ0); ++z)
               {
                  for(x = 0; x < (rX1 - rX0); ++x)
                  {
                     /*--------------------------------------------------------*\
                     ! Advance the data pointer to the next row.                !
                     \*--------------------------------------------------------*/
                     data = parameter->data + x * incr_X + z * incr_Z + t * incr_TS;

                     /*--------------------------------------------------------*\
                     ! In the event that the current row and resolution only    !
                     ! encompasses 1 value, the sample will remain unaltered.   !
                     ! In case of an odd starting point the sample will be      !
                     ! doubled.                                                 !
                     \*--------------------------------------------------------*/
                     if((rY1 - rY0) == 1)
                     {
                        if(rY0 & 2)
                        {
                           data[0].f /=  2;
                        }
                        continue;
                     }

                     /*--------------------------------------------------------*\
                     ! Fill the working buffer with the x-th tile parameter     !
                     ! row.                                                     !
                     \*--------------------------------------------------------*/
                     fill_inverse_buffer(data, &memory[id][filter_tapsY], rY0, rY1, incr_Y);

                     /*--------------------------------------------------------*\
                     ! Apply the boundary extension on the working buffer ap-   !
                     ! propriate to the specified wavelet kernel.               !
                     \*--------------------------------------------------------*/
                     boundary_extension[control->KernelY](&memory[id][filter_tapsY], rY0, rY1, filter_tapsY);

                     /*--------------------------------------------------------*\
                     ! Perform the inverse wavelet transform on the current     !
                     ! column.                                                  !
                     \*--------------------------------------------------------*/
                     inverse_wavelet_transform[control->KernelY](working_buffer, rY0, rY1);

                     /*--------------------------------------------------------*\
                     ! Copy the decompressed flow field samples from the work-  !
                     ! ing buffer to the tile parameter memory.                 !
                     \*--------------------------------------------------------*/
                     buffer_flush_inverse(data, working_buffer, rY0, rY1, incr_Y);
                  }
               }
            }
         }

         if(level < control->decompX  && (rX1 - rX0) > 1)
         {
            /*--------------------------------------------------------*\
            ! Associate the working buffer pointer to the allocated    !
            ! memory block. The working buffer points to the appropri- !
            ! ate starting point in the memory block, accounting for   !
            ! for the taps of the wavelet kernel selected for the      !
            ! first spatial dimension.                                 !
            \*--------------------------------------------------------*/
            working_buffer = &memory[id][filter_tapsX];

            /*--------------------------------------------------------*\
            ! Adjust the working buffer pointer in case of an odd      !
            ! length column starting point.                            !
            \*--------------------------------------------------------*/
            if(rX0 % 2)
            {
               working_buffer++;
            }

            /*--------------------------------------------------------*\
            ! Walk trough all the temporal and spatial slices as well  !
            ! as rows.                                                 !
            \*--------------------------------------------------------*/
            #if defined (_OPENMP)
               #pragma omp for collapse(3)
            #endif
            for(t = 0; t < (rTS1 - rTS0); ++t)
            {
               for(z = 0; z < (rZ1 - rZ0); ++z)
               {
                  for(y = 0; y < (rY1 - rY0); ++y)
                  {
                     /*--------------------------------------------------------*\
                     ! Advance the data pointer to the next column.             !
                     \*--------------------------------------------------------*/
                     data = parameter->data + y * incr_Y + z * incr_Z + t * incr_TS;

                     /*--------------------------------------------------------*\
                     ! In the event that the current column and resolution only !
                     ! encompasses 1 value, the sample will remain unaltered.   !
                     ! In case of an odd starting point the sample will be      !
                     ! doubled.                                                 !
                     \*--------------------------------------------------------*/
                     if((rX1 - rX0) == 1)
                     {
                        if(rX0 & 2)
                        {
                           data[0].f /=  2;
                        }
                        continue;
                     }

                     /*--------------------------------------------------------*\
                     ! Fill the working buffer with the y-th tile parameter     !
                     ! column.                                                  !
                     \*--------------------------------------------------------*/
                     fill_inverse_buffer(data, &memory[id][filter_tapsX], rX0, rX1, incr_X);

                     /*--------------------------------------------------------*\
                     ! Apply the boundary extension on the working buffer ap-   !
                     ! propriate to the specified wavelet kernel.               !
                     \*--------------------------------------------------------*/
                     boundary_extension[control->KernelX](&memory[id][filter_tapsX], rX0, rX1, filter_tapsX);

                     /*--------------------------------------------------------*\
                     ! Perform the inverse wavelet transform on the current     !
                     ! column.                                                  !
                     \*--------------------------------------------------------*/
                     inverse_wavelet_transform[control->KernelX](working_buffer, rX0, rX1);

                     /*--------------------------------------------------------*\
                     ! Copy the decompressed flow field samples from the work-  !
                     ! ing buffer to the tile parameter memory.                 !
                     \*--------------------------------------------------------*/
                     buffer_flush_inverse(data, working_buffer, rX0, rX1, incr_X);
                  }
               }
            }
         }
      }
   }

   /*--------------------------------------------------------*\
   ! Deallocate the memory array and memory blocks.           !
   \*--------------------------------------------------------*/
   for(i = 0; i < nThreads; ++i)
   {
      free(memory[i]);
   }
   free(memory);

   return 0;
}