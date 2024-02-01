/*==================================================================================================================================*\
||                                                                                                                                  ||
||                         /$$$$$$$  /$$                 /$$      /$$ /$$                                                           ||
||                        | $$__  $$|__/                | $$  /$ | $$| $$                                                           ||
||                        | $$  \ $$ /$$  /$$$$$$       | $$ /$$$| $$| $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$                        ||
||                        | $$$$$$$ | $$ /$$__  $$      | $$/$$ $$ $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$                       ||
||                        | $$__  $$| $$| $$  \ $$      | $$$$_  $$$$| $$  \ $$| $$  \ $$| $$  \ $$| $$  \ $$                       ||
||                        | $$  \ $$| $$| $$  | $$      | $$$/ \  $$$| $$  | $$| $$  | $$| $$  | $$| $$  | $$                       ||
||                        | $$$$$$$/| $$|  $$$$$$$      | $$/   \  $$| $$  | $$|  $$$$$$/|  $$$$$$/| $$$$$$$/                       ||
||                        |_______/ |__/ \____  $$      |__/     \__/|__/  |__/ \______/  \______/ | $$____/                        ||
||                                       /$$  \ $$                                                 | $$                             ||
||                                      |  $$$$$$/                                                 | $$                             ||
||                                       \______/                                                  |__/                             ||
||                                                                                                                                  ||
||                                                                                                                                  ||
||                                                           Version 0.1.1                                                          ||
||                                                                                                                                  ||
||      DESCRIPTION:                                                                                                                ||
||      ------------                                                                                                                ||
||      Big Whoop is a compression codec for the lossy compression of IEEE 754 floating point arrays defined on curvelinear         ||
||      compute grids.                                                                                                              ||
||                                                                                                                                  ||
||      FILE REFERENCES:                                                                                                            ||
||      ----------------                                                                                                            ||
||                                                                                                                                  ||
||                         Name              I/O             Description                                                            ||
||                         ----              ---             -----------                                                            ||
||                         none               -                   -                                                                 ||
||                                                                                                                                  ||
||                                                                                                                                  ||
||      PRIVATE FUNCTIONS:                                                                                                          ||
||      ------------------                                                                                                          ||
||      - initialize_precinct                                                                                                       ||
||      - subband_gain                                                                                                              ||
||      - initialize_subband                                                                                                        ||
||      - create_field                                                                                                              ||
||                                                                                                                                  ||
||      PUBLIC FUNCTIONS:                                                                                                           ||
||      -----------------                                                                                                           ||
||      - bwc_initialize_field                                                                                                      ||
||      - bwc_add_param                                                                                                             ||
||      - bwc_set_error_resilience                                                                                                  ||
||      - bwc_set_quantization_style                                                                                                ||
||      - bwc_set_progression                                                                                                       ||
||      - bwc_set_kernels                                                                                                           ||
||      - bwc_set_decomp                                                                                                            ||
||      - bwc_set_precincts                                                                                                         ||
||      - bwc_set_codeblocks                                                                                                        ||
||      - bwc_set_qm                                                                                                                ||
||      - bwc_set_tiles                                                                                                             ||
||      - bwc_create_compression                                                                                                    ||
||      - bwc_kill_compression                                                                                                      ||
||                                                                                                                                  ||
||      DEVELOPMENT HISTORY:                                                                                                        ||
||      --------------------                                                                                                        ||
||                                                                                                                                  ||
||                            Date        Author             Change Id   Release     Description Of Change                          ||
||                            ----        ------             ---------   -------     ---------------------                          ||
||                            10.10.2017  Patrick Vogler     B87D120     V 0.1.0     source file created                            ||
||                                                                                                                                  ||
||       --------------------------------------------------------------------------------------------------------------------       ||
||                                                                                                                                  ||
||       Copyright (c) 2023, High Performance Computing Center - University of Stuttgart                                            ||
||                                                                                                                                  ||
||       Redistribution and use in source and binary forms, with or without modification, are permitted provided that the           ||
||       following conditions are met:                                                                                              ||
||                                                                                                                                  ||
||          (1)   Redistributions of source code must retain the above copyright notice, this list of conditions and                ||
||                the following disclaimer.                                                                                         ||
||                                                                                                                                  ||
||          (2)   Redistributions in binary form must reproduce the above copyright notice, this list of conditions                 ||
||                and the following disclaimer in the documentation and/or other materials provided with the                        ||
||                distribution.                                                                                                     ||
||                                                                                                                                  ||
||       THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,         ||
||       INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          ||
||       DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,          ||
||       SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR            ||
||       SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,          ||
||       WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE           ||
||       USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                   ||
||                                                                                                                                  ||
\*==================================================================================================================================*/

/************************************************************************************************************\
||                                      _ _  _ ____ _    _  _ ___  ____                                     ||
||                                      | |\ | |    |    |  | |  \ |___                                     ||
||                                      | | \| |___ |___ |__| |__/ |___                                     ||
||                                                                                                          ||
\************************************************************************************************************/
#include <assert.h>
#include <math.h>
#include <omp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bitstream.h"
#include "constants.h"
#include "macros.h"
#include "types.h"
#include "codestream.h"
#include "dwt.h"
#include "tier1.h"
#include "tier2.h"
#include "tagtree.h"

/************************************************************************************************************\
||                ___  ____ _ _  _ ____ ___ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                ||
||                |__] |__/ | |  | |__|  |  |___    |___ |  | |\ | |     |  | |  | |\ | [__                 ||
||                |    |  \ |  \/  |  |  |  |___    |    |__| | \| |___  |  | |__| | \| ___]                ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uint8 initialize_precinct(bwc_field *const field, bwc_precinct *precinct,                 !
!   --------------                                         const uint32 dX, const uint32 dY,                 !
!                                                          const uint32 dZ, const uint32 dTS)                !
!                                                                                                            !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                precinct                    bwc_precinct*         - Structure defining a bwc precinct.      !
!                                                                                                            !
!                dX, dY, dZ, dTS             unsigned int(32 bit)  - Defines the offset of the codeblock     !
!                                                                    with regards to the current subband.    !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                unsigned int (8 bit)      - Subband gain factor in log2 factor.                             !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                24.05.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uint8
initialize_precinct(bwc_field *const field, bwc_precinct *precinct, const uint32 dX, const uint32 dY, 
                                                                    const uint32 dZ, const uint32 dTS)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   cb, cbSizeX, cbSizeY, cbSizeZ, cbSizeTS;
   uint16   cb_X, cb_Y, cb_Z, cb_TS;

   /*-----------------------*\
    ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;
   bwc_prec_ctrl     *prec_control;
   bwc_prec_inf      *prec_info;
   bwc_codeblock     *codeblock;
   bwc_cblk_ctrl     *cblk_control;
   bwc_cblk_inf      *cblk_info;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(precinct);

   /*--------------------------------------------------------*\
   ! Save the global control as well as the precinct control  !
   ! and info structure to temporary variables to make the    !
   ! code more readable.                                      !
   \*--------------------------------------------------------*/
   control      = &field->control;

   prec_control = &precinct->control;
   prec_info    = &precinct->info;

   /*--------------------------------------------------------*\
   ! Evaluate the global codeblock size for easy access.      !
   \*--------------------------------------------------------*/
   cbSizeX  = 1 << control->cbX ;
   cbSizeY  = 1 << control->cbY ;
   cbSizeZ  = 1 << control->cbZ ;
   cbSizeTS = 1 << control->cbTS;

   /*--------------------------------------------------------*\
   ! Calculate the number of codeblock to cover precinct p in !
   ! subband s according to the corresponding equation from   !
   ! JPEG2000 by D. S. Taubman and M. W. Marcellin (p.462).   !
   \*--------------------------------------------------------*/
   prec_control->numCbX  = (uint16)((prec_info->X0  == prec_info->X1)  ? 0 : (ceil((float)prec_info->X1 /cbSizeX) -
                                                                             floor((float)prec_info->X0 / cbSizeX)));
   prec_control->numCbY  = (uint16)((prec_info->Y0  == prec_info->Y1)  ? 0 : (ceil((float)prec_info->Y1 /cbSizeY) -
                                                                             floor((float)prec_info->Y0 / cbSizeY)));
   prec_control->numCbZ  = (uint16)((prec_info->Z0  == prec_info->Z1)  ? 0 : (ceil((float)prec_info->Z1 /cbSizeZ) -
                                                                             floor((float)prec_info->Z0 / cbSizeZ)));
   prec_control->numCbTS = (uint16)((prec_info->TS0 == prec_info->TS1) ? 0 : (ceil((float)prec_info->TS1/cbSizeTS)-
                                                                             floor((float)prec_info->TS0/ cbSizeTS)));

   prec_control->number_of_codeblocks = (uint64)prec_control->numCbX * prec_control->numCbY * prec_control->numCbZ * prec_control->numCbTS;

   /*--------------------------------------------------------*\
   ! Evaluate if the current precinct features any codeblocks.!
   \*--------------------------------------------------------*/
   if(prec_control->number_of_codeblocks)
   {
      /*--------------------------------------------------------*\
      ! Initialize the msbs and inclusion tagtree for the cur-   !
      ! rent precinct.                                           !
      \*--------------------------------------------------------*/
      prec_control->tag_msbs      = initialize_tagtree(prec_control->numCbX, prec_control->numCbY, 
                                                       prec_control->numCbZ, prec_control->numCbTS);
      prec_control->tag_inclusion = initialize_tagtree(prec_control->numCbX, prec_control->numCbY, 
                                                       prec_control->numCbZ, prec_control->numCbTS);

      /*--------------------------------------------------------*\
      ! Allocate, walk through and setup the codeblock structure !
      ! according to the specified compression parameters.       !
      \*--------------------------------------------------------*/
      precinct->codeblock = calloc(prec_control->number_of_codeblocks, sizeof(bwc_codeblock));
      if(!precinct->codeblock)
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         return 1;
      }
      for(cb_TS = 0, cb = 0; cb_TS < prec_control->numCbTS; ++cb_TS)
      {
         for(cb_Z = 0; cb_Z < prec_control->numCbZ; ++cb_Z)
         {
            for(cb_Y = 0; cb_Y < prec_control->numCbY; ++cb_Y)
            {
               for(cb_X = 0; cb_X < prec_control->numCbX; ++cb_X, ++cb)
               {
                  /*--------------------------------------------------------*\
                  ! Allocate the info and control structure for the current  !
                  ! codeblock.                                               !
                  \*--------------------------------------------------------*/
                  codeblock                = &precinct->codeblock[cb];
                  codeblock->encoded_block = calloc(1, sizeof(bwc_encoded_cblk));
                  if(!codeblock->encoded_block)
                  {
                     // memory allocation error
                     fprintf(stderr, MEMERROR);
                     return 1;
                  }

                  cblk_info    = &codeblock->info;
                  cblk_control = &codeblock->control;

                  /*--------------------------------------------------------*\
                  ! Allocate the memory block used to store the coding pass  !
                  ! contributions for the specified quality layers and setup !
                  ! the pointer used to access the integer array.            !
                  \*--------------------------------------------------------*/
                  cblk_control->memory = calloc(control->nLayers + 1, sizeof(int16));
                  if(!cblk_control->memory)
                  {
                     // memory allocation error
                     fprintf(stderr, MEMERROR);
                     return 1;
                  }

                  cblk_control->cp_contr = &cblk_control->memory[1];

                  cblk_control->beta     = 3;
                  cblk_control->beta_est = 3;

                  /*--------------------------------------------------------*\
                  ! Calculate the boundaries for the current codeblock ac-   !
                  ! cording to the corresponding equation from JPEG2000 by   !
                  ! David S. Taubman and Michael W. Marcellin (p.460).       !
                  \*--------------------------------------------------------*/
                  cblk_info->X0  = dX  + (uint32)MAX(prec_info->X0 , cbSizeX  * (cb_X  + (uint32)floor((float)prec_info->X0 / cbSizeX )));
                  cblk_info->Y0  = dY  + (uint32)MAX(prec_info->Y0 , cbSizeY  * (cb_Y  + (uint32)floor((float)prec_info->Y0 / cbSizeY )));
                  cblk_info->Z0  = dZ  + (uint32)MAX(prec_info->Z0 , cbSizeZ  * (cb_Z  + (uint32)floor((float)prec_info->Z0 / cbSizeZ )));
                  cblk_info->TS0 = dTS + (uint16)MAX(prec_info->TS0, cbSizeTS * (cb_TS + (uint32)floor((float)prec_info->TS0/ cbSizeTS)));
                  cblk_info->X1  = dX  + (uint32)MIN(prec_info->X1 , cbSizeX  * (cb_X  + (uint32)floor((float)prec_info->X0 / cbSizeX ) + 1));
                  cblk_info->Y1  = dY  + (uint32)MIN(prec_info->Y1 , cbSizeY  * (cb_Y  + (uint32)floor((float)prec_info->Y0 / cbSizeY ) + 1));
                  cblk_info->Z1  = dZ  + (uint32)MIN(prec_info->Z1 , cbSizeZ  * (cb_Z  + (uint32)floor((float)prec_info->Z0 / cbSizeZ ) + 1));
                  cblk_info->TS1 = dTS + (uint16)MIN(prec_info->TS1, cbSizeTS * (cb_TS + (uint32)floor((float)prec_info->TS0/ cbSizeTS) + 1));
               }
            }
         }
      }
   }
   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uint8 subband_gain(const uint8 highband_flag)                                             !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function calculates the gain for a specific subband in log2 format according to the    !
!                number of applied 1-D wavelet transforms. The subband gain is calculated by evaluating the  !
!                hamming weight of the highband flag. (see https://en.wikipedia.org/wiki/Hamming_weight)     !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                highband_flag               unsigned in (8 bit)   - Flag defining the number and types of   !
!                                                                    1-D wavelet of transforms applied to    !
!                                                                    the subband.                            !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                unsigned int (8 bit)      - Subband gain factor in log2 factor.                             !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                15.05.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uint8 subband_gain(const uint8 highband_flag)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint8 temp;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(highband_flag <= DIM_ALL);

   /*--------------------------------------------------------*\
   ! Save the highband flag in a temporary variable.          !
   \*--------------------------------------------------------*/
   temp  = highband_flag;

   /*--------------------------------------------------------*\
   ! Calculate the subband gain according to the number of    !
   ! applied 1-D wavelet transforms.                          !
   \*--------------------------------------------------------*/  
   temp -= (temp >> 1) & 0x55;
   temp  = (temp & 0x33) + ((temp >> 2) & 0x33);
   return  (temp + (temp >> 4)) & 0x0F;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uchar initialize_subband(bwc_field *const field, bwc_parameter *const parameter,          !
!   --------------                                                  bwc_resolution *const resolution,        !
!                                                                   bwc_subband *const subband,              !
!                                                                   int32 resolution_level,                  !
!                                                                   int16 highband_flag)                     !
!                                                                                                            !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function initializes the bwc_subband structure with all necessary standard parameters  !
!                to (de)compress a floating point array with nX * nY * nZ grid points, nTS timesteps and     !
!                nPar parameters.                                                                            !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                parameter                   bwc_parameter*        - Structure defining a bwc tile parameter.!
!                                                                                                            !
!                resolution                  bwc_resolution*       - Structure defining a bwc resolution     !
!                                                                    level.                                  !
!                                                                                                            !
!                subband                     bwc_subband*          - Structure defining a bwc subband.       !
!                                                                                                            !
!                resolution_level            signed int(32 bit)    - Defines the current resolution level.   !
!                                                                                                            !
!                highband_flag               signed int(16 bit)    - Defines the type of highband the cur-   !
!                                                                    rent subband represents.                !
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
!                12.12.2017  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uchar
initialize_subband(bwc_field *const field, bwc_parameter *const parameter, bwc_resolution *const resolution, 
                                                                           bwc_subband *const subband, 
                                                                           int32 resolution_level, 
                                                                           int16 highband_flag)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   p, pSizeX, pSizeY, pSizeZ, pSizeTS;
   uint64   sb_sX, sb_sY, sb_sZ, sb_sTS;
   uint32   Rb;
   uint16   p_X, p_Y, p_Z, p_TS;
   int8     decomp_level;
   int8     level_X, level_Y, level_Z, level_TS;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;
   bwc_param_ctrl    *param_control;
   bwc_param_inf     *param_info;
   bwc_res_ctrl      *res_control;
   bwc_res_inf       *res_info;
   bwc_subb_ctrl     *subb_control;
   bwc_subb_inf      *subb_info;
   bwc_prec_ctrl     *prec_control;
   bwc_prec_inf      *prec_info;
   
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(resolution);
   assert(subband);
   assert(resolution_level <= field->control.nDecomp + 1);
   assert(highband_flag <= DIM_ALL);

   /*--------------------------------------------------------*\
   ! Save the global, parameter, resolution and subband con-  !
   ! trol and info structures to temporary variables to make  !
   ! the code more readable.                                  !
   \*--------------------------------------------------------*/
   control       = &field->control;

   param_control = &parameter->control;
   param_info    = &parameter->info;

   res_control   = &resolution->control;
   res_info      = &resolution->info;

   subb_control  = &subband->control;
   subb_info     = &subband->info;

   /*--------------------------------------------------------*\
   ! Evaluate the number of decompositions in each temporal & !
   ! spatial directions according to the the global decompo - !
   ! sition level for which the subband is initialized.       !
   \*--------------------------------------------------------*/
   decomp_level = control->nDecomp - resolution_level + 1;

   level_X  = (decomp_level > control->decompX ) ? control->decompX  : decomp_level;
   level_Y  = (decomp_level > control->decompY ) ? control->decompY  : decomp_level;
   level_Z  = (decomp_level > control->decompZ ) ? control->decompZ  : decomp_level;
   level_TS = (decomp_level > control->decompTS) ? control->decompTS : decomp_level;

   /*--------------------------------------------------------*\
   ! Calculate the boundaries for the current subband accord- !
   ! ing to the corresponding equation from JPEG2000 by David !
   ! S. Taubman and Michael W. Marcellin (p.460).             !
   \*--------------------------------------------------------*/
   subb_info->X0  = (uint64)ceil( ((float)param_info->X0  / (1 << level_X))  - 0.5 * ((highband_flag & DIM_X)  >> 0));
   subb_info->Y0  = (uint64)ceil( ((float)param_info->Y0  / (1 << level_Y))  - 0.5 * ((highband_flag & DIM_Y)  >> 1));
   subb_info->Z0  = (uint64)ceil( ((float)param_info->Z0  / (1 << level_Z))  - 0.5 * ((highband_flag & DIM_Z)  >> 2));
   subb_info->TS0 = (uint16)ceil( ((float)param_info->TS0 / (1 << level_TS)) - 0.5 * ((highband_flag & DIM_TS) >> 3));
   subb_info->X1  = (uint64)ceil( ((float)param_info->X1  / (1 << level_X))  - 0.5 * ((highband_flag & DIM_X)  >> 0));
   subb_info->Y1  = (uint64)ceil( ((float)param_info->Y1  / (1 << level_Y))  - 0.5 * ((highband_flag & DIM_Y)  >> 1));
   subb_info->Z1  = (uint64)ceil( ((float)param_info->Z1  / (1 << level_Z))  - 0.5 * ((highband_flag & DIM_Z)  >> 2));
   subb_info->TS1 = (uint16)ceil( ((float)param_info->TS1 / (1 << level_TS)) - 0.5 * ((highband_flag & DIM_TS) >> 3));

   /*--------------------------------------------------------*\
   ! Evaluate the dynamic range (Rb) of the current subband.  !
   \*--------------------------------------------------------*/
   Rb = PREC_BIT + subband_gain(highband_flag);

   /*--------------------------------------------------------*\
   ! Save the highband flag in the subband control structure  !
   ! and evaluate the energy gain factor for the current sub- !
   ! band.                                                    !
   \*--------------------------------------------------------*/
   subb_control->highband_flag = highband_flag;
   subb_info->dwt_gain         = get_dwt_energy_gain(field, highband_flag, decomp_level);

   /*--------------------------------------------------------*\
   ! Evaluate the quantization exponent, mantissa, step size  !
   ! and effective step size according to the specified quan- !
   ! tization style.                                          !
   \*--------------------------------------------------------*/
   if(control->quantization_style == bwc_qt_none)
   {
      /*--------------------------------------------------------*\
      ! If no quantization is wanted the mantissa is set to 0    !
      ! while the exponent is set to the dynamic range of the    !
      ! subband in order to obtain a step size of 1.             !
      \*--------------------------------------------------------*/
      subb_control->qt_exponent            = (uint8)  Rb;
      subb_control->qt_mantissa            = (uint16) 0;
      subb_control->qt_step_size           = 1;
      subb_control->qt_effective_step_size = 1;
   }
   else
   {
      /*--------------------------------------------------------*\
      ! For a derrived quantization style the exponent is a-     !
      ! dapted to the current resolution level and the (effec-   !
      ! tive) step size is evaluated according to equation       !
      ! (10.26) from JPEG2000 by David  S. Taubman and Michael   !
      ! W. Marcellin (p.439).                                    !
      \*--------------------------------------------------------*/
      subb_control->qt_exponent            = (uint8)  control->qt_exponent + ((resolution_level == 0) ? 0 : 1 - resolution_level);
      subb_control->qt_mantissa            = (uint16) control->qt_mantissa;
      subb_control->qt_step_size           = (1.0f + ((double)subb_control->qt_mantissa / (1 << 11))) / pow(2,subb_control->qt_exponent);
      subb_control->qt_effective_step_size = (1.0f + ((double)subb_control->qt_mantissa / (1 << 11)));
      if(subb_control->qt_exponent > Rb)
      {
         subb_control->qt_effective_step_size /= pow(2,subb_control->qt_exponent - Rb);
      }
      else
      {
         subb_control->qt_effective_step_size *= pow(2,Rb - subb_control->qt_exponent);
      }
   }

   /*--------------------------------------------------------*\
   ! Evaluate the dynamic range of the subband after the      !
   ! transformation stage.                                    !
   \*--------------------------------------------------------*/
   subb_control->Kmax = MAX(0, control->guard_bits + subb_control->qt_exponent - 1);

   /*--------------------------------------------------------*\
   ! Evaluate the global precinct size for easy access.       !
   \*--------------------------------------------------------*/
   pSizeX   = 1 << control->precSizeX;
   pSizeY   = 1 << control->precSizeY;
   pSizeZ   = 1 << control->precSizeZ;
   pSizeTS  = 1 << control->precSizeTS;

   /*--------------------------------------------------------*\
   ! Evaluate the start coordinates for the current subband.  !
   \*--------------------------------------------------------*/
   sb_sX  = (highband_flag & DIM_X)  ? (parameter->resolution[resolution_level - 1].info.X1  -
                                        parameter->resolution[resolution_level - 1].info.X0 ) : 0;
   sb_sY  = (highband_flag & DIM_Y)  ? (parameter->resolution[resolution_level - 1].info.Y1  -
                                        parameter->resolution[resolution_level - 1].info.Y0 ) : 0;
   sb_sZ  = (highband_flag & DIM_Z)  ? (parameter->resolution[resolution_level - 1].info.Z1  -
                                        parameter->resolution[resolution_level - 1].info.Z0 ) : 0;
   sb_sTS = (highband_flag & DIM_TS) ? (parameter->resolution[resolution_level - 1].info.TS1 -
                                        parameter->resolution[resolution_level - 1].info.TS0) : 0;

   /*--------------------------------------------------------*\
   ! Evaluate if the current resolution level features any    !
   ! precincts.                                               !
   \*--------------------------------------------------------*/
   if(res_control->number_of_precincts)
   {
      /*--------------------------------------------------------*\
      ! Allocate, walk through and setup the precinct structure  !
      ! according to the specified compression parameters.       !
      \*--------------------------------------------------------*/
      subband->precinct = calloc(res_control->number_of_precincts, sizeof(bwc_precinct));
      if(!subband->precinct)
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         return 1;
      }
      
      for(p_TS = 0, p = 0; p_TS < res_control->numPTS; ++p_TS)
      {
         for(p_Z = 0; p_Z < res_control->numPZ; ++p_Z)
         {
            for(p_Y = 0; p_Y < res_control->numPY; ++p_Y)
            {
               for(p_X = 0; p_X < res_control->numPX; ++p_X, ++p)
               {
                  /*--------------------------------------------------------*\
                  ! Save the precinct info and control structure to tempo-   !
                  ! rary variables to make the code more readable.           !
                  \*--------------------------------------------------------*/
                  prec_control = &subband->precinct[p].control;
                  prec_info    = &subband->precinct[p].info;

                  /*--------------------------------------------------------*\
                  ! Calculate the boundaries for the current precinct accord-!
                  ! ing to the corresponding equation from JPEG2000 by David !
                  ! S. Taubman and Michael W. Marcellin (p.460).             !
                  \*--------------------------------------------------------*/
                  prec_info->X0  = (uint32)MAX(res_info->X0 , pSizeX  * (p_X  + (uint32)floor(res_info->X0 / pSizeX)));
                  prec_info->Y0  = (uint32)MAX(res_info->Y0 , pSizeY  * (p_Y  + (uint32)floor(res_info->Y0 / pSizeY)));
                  prec_info->Z0  = (uint32)MAX(res_info->Z0 , pSizeZ  * (p_Z  + (uint32)floor(res_info->Z0 / pSizeZ)));
                  prec_info->TS0 = (uint16)MAX(res_info->TS0, pSizeTS * (p_TS + (uint32)floor(res_info->TS0/ pSizeTS)));
                  prec_info->X1  = (uint32)MIN(res_info->X1 , pSizeX  * (p_X  + (uint32)floor(res_info->X0 / pSizeX) + 1));
                  prec_info->Y1  = (uint32)MIN(res_info->Y1 , pSizeY  * (p_Y  + (uint32)floor(res_info->Y0 / pSizeY) + 1));
                  prec_info->Z1  = (uint32)MIN(res_info->Z1 , pSizeZ  * (p_Z  + (uint32)floor(res_info->Z0 / pSizeZ) + 1));
                  prec_info->TS1 = (uint16)MIN(res_info->TS1, pSizeTS * (p_TS + (uint32)floor(res_info->TS0/ pSizeTS)+ 1));

                  if((control->nDecomp - control->decompX) < resolution_level)
                  {
                     prec_info->X0 = (uint32) ceil(((float)prec_info->X0 / 2) - 0.5 * ((highband_flag & DIM_X)  >> 0));
                     prec_info->X1 = (uint32) ceil(((float)prec_info->X1 / 2) - 0.5 * ((highband_flag & DIM_X)  >> 0));
                  }

                  if((control->nDecomp - control->decompY) < resolution_level)
                  {
                     prec_info->Y0 = (uint32) ceil(((float)prec_info->Y0 / 2) - 0.5 * ((highband_flag & DIM_Y)  >> 1));
                     prec_info->Y1 = (uint32) ceil(((float)prec_info->Y1 / 2) - 0.5 * ((highband_flag & DIM_Y)  >> 1));
                  }

                  if((control->nDecomp - control->decompZ) < resolution_level)
                  {
                     prec_info->Z0 = (uint32) ceil(((float)prec_info->Z0 / 2) - 0.5 * ((highband_flag & DIM_Z)  >> 2));
                     prec_info->Z1 = (uint32) ceil(((float)prec_info->Z1 / 2) - 0.5 * ((highband_flag & DIM_Z)  >> 2));
                  }

                  if((control->nDecomp - control->decompTS) < resolution_level)
                  {
                     prec_info->TS0 = (uint16) ceil(((float)prec_info->TS0 / 2) - 0.5 * ((highband_flag & DIM_TS)  >> 3));
                     prec_info->TS1 = (uint16) ceil(((float)prec_info->TS1 / 2) - 0.5 * ((highband_flag & DIM_TS)  >> 3));
                  }

                  /*--------------------------------------------------------*\
                  ! Initialize the precinct structure according to the speci-!
                  ! fied compression parameters.                             !
                  \*--------------------------------------------------------*/
                  initialize_precinct(field, &subband->precinct[p], (uint32)sb_sX - subb_info->X0, (uint32)sb_sY  - subb_info->Y0,
                                                                    (uint32)sb_sZ - subb_info->Z0, (uint32)sb_sTS - subb_info->TS0);

                  /*--------------------------------------------------------*\
                  ! Update the number of codeblocks for the current parame-  !
                  ! ter.                                                     !
                  \*--------------------------------------------------------*/
                  param_control->number_of_codeblocks += prec_control->number_of_codeblocks;
               }
            }
         }
      }
   }
   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_header_append_aux(bwc_field *const field, bwc_packed_stream *const aux)          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function amends the main header for the compressed codestream with an auxiliary        !
!                information block.                                                                          !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                aux                         unsigned char*        - Memory handle for the auxiliary infor-  !
!                                                                    mation block.                           !
!                                                                                                            !
!                size                        unsigned int(32 bit)  - Size of the auxiliary information block.!
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
!                12.04.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uchar
header_append_aux(bwc_field *const field, bwc_packed_stream *const aux)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_ctrl       *control;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(aux);

   /*--------------------------------------------------------*\
   ! Save the global control and info structure to temporary  !
   ! variables to make the code more readable.                !
   \*--------------------------------------------------------*/
   control = &field->control;

   /*--------------------------------------------------------*\
   ! Check if the main header is already defined in the field !
   ! structure.                                               !
   \*--------------------------------------------------------*/
   if(!control->header.memory)
   {
      fprintf(stderr,"o==========================================================o\n"\
                     "| ERROR: Main header not defined                           |\n"\
                     "|                                                          |\n"\
                     "|        Unable to append auxiliary information since the  |\n"\
                     "|        main header has not yet been created. Appending   |\n"\
                     "|        information to the end of the main header can     |\n"\
                     "|        only be done after the bwc_create_compression     |\n"\
                     "|        function has been called.                         |\n"\
                     "|                                                          |\n"\
                     "o==========================================================o\n");

      return 1;
   }

   /*--------------------------------------------------------*\
   ! Check if the main header would exceed the maximum number !
   ! of allowable bits after appending the auxiliary          !
   ! information.                                             !
   \*--------------------------------------------------------*/
   if(((uint64)control->header.size + aux->size) >= 0xFFFFFFFF)
   {
      fprintf(stderr,"o==========================================================o\n"\
                     "| ERROR: Main header exceeds maximum size limit            |\n"\
                     "|                                                          |\n"\
                     "|        Appending the auxiliary information to the main   |\n"\
                     "|        header would exceed its maximum size limit of     |\n"\
                     "|        4294967295 bytes.                                 |\n"\
                     "|                                                          |\n"\
                     "o==========================================================o\n");

      return 1;
   }

   if(codestream_write_aux(&control->header, aux))
   {
      return 1;
   }

   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_header_append_com(bwc_field *const field, bwc_packed_stream *const com)          !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function amends the main header for the compressed codestream with a comment           !
!                block.                                                                                      !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                com                         unsigned char*        - Memory handle for the auxiliary infor-  !
!                                                                    mation block.                           !
!                                                                                                            !
!                size                        unsigned int(32 bit)  - Size of the auxiliary information block.!
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
!                12.04.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uchar
header_append_com(bwc_field *const field, bwc_packed_stream *const com)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_ctrl       *control;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);

   /*--------------------------------------------------------*\
   ! Save the global control and info structure to temporary  !
   ! variables to make the code more readable.                !
   \*--------------------------------------------------------*/
   control = &field->control;

   /*--------------------------------------------------------*\
   ! Check if the main header is already defined in the field !
   ! structure.                                               !
   \*--------------------------------------------------------*/
   if(!control->header.memory)
   {
      fprintf(stderr,"o==========================================================o\n"\
                     "| ERROR: Main header not defined                           |\n"\
                     "|                                                          |\n"\
                     "|        Unable to append auxiliary information since the  |\n"\
                     "|        main header has not yet been created. Appending   |\n"\
                     "|        information to the end of the main header can     |\n"\
                     "|        only be done after the bwc_create_compression     |\n"\
                     "|        function has been called.                         |\n"\
                     "|                                                          |\n"\
                     "o==========================================================o\n");

      return 1;
   }

   /*--------------------------------------------------------*\
   ! Check if the main header would exceed the maximum number !
   ! of allowable bits after appending the auxiliary          !
   ! information.                                             !
   \*--------------------------------------------------------*/
   if(((uint64)control->header.size + com->size) >= 0xFFFFFFFF)
   {
      fprintf(stderr,"o==========================================================o\n"\
                     "| ERROR: Main header exceeds maximum size limit            |\n"\
                     "|                                                          |\n"\
                     "|        Appending the auxiliary information to the main   |\n"\
                     "|        header would exceed its maximum size limit of     |\n"\
                     "|        4294967295 bytes.                                 |\n"\
                     "|                                                          |\n"\
                     "o==========================================================o\n");

      return 1;
   }

   if(codestream_write_com(&control->header, com))
   {
      return 1;
   }

   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void fill_buffer(bwc_field *const field, bwc_tile *const tile,                            !
!   ------------                                  bwc_parameter *const parameter,                            !
!                                               bwc_sample *const working_buffer,                            !
!                                                             double *const data,                            !
!                                                              uint64 param_size,                            !
!                                                                uint16 param_id)                            !
!                                                                                                            !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to fill the working buffer with the appropriate flow field data for   !
!                the specified tile parameter.                                                               !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                tile                        bwc_tile*             - Structure defining a bwc tile.          !
!                                                                                                            !
!                parameter                   bwc_parameter*        - Structure defining a bwc parameter.     !
!                                                                                                            !
!                working_buffer              bwc_sample*           - Working buffer used to store flow field !
!                                                                    data for a specific tile parameter.     !
!                                                                                                            !
!                param_size                  uint64                - Specifies the byte size of one tile     !
!                                                                    parameter.                              !
!                                                                                                            !
!                parameter                   uint16                - Specifies the parameter index for the   !
!                                                                    current tile parameter.                 !
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
!                22.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
fill_buffer(bwc_field *const field, bwc_tile *const tile, bwc_parameter *const parameter,
                                                        bwc_sample *const working_buffer,
                                                                    bwc_data *const data,
                                                                         uint16 param_id)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   width, height, depth, dt;
   uint64   param_offset;
   uint64   x, y, z;
   uint16   t;

   /*-----------------------*\
   ! DEFINE FLOAT VARIABLES: !
   \*-----------------------*/
   bwc_float   max, min;
   double     *src_d, *tmp_d;
   float      *src_f, *tmp_f;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_sample       *dest;
   bwc_gl_inf       *info;
   bwc_tile_inf     *tile_info;
   bwc_param_ctrl   *param_control;
   bwc_param_inf    *param_info;
   bwc_cmd_opts_ll  *param;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(tile);
   assert(parameter);
   assert(working_buffer);
   assert(data);

   /*--------------------------------------------------------*\
   ! Save the global, tile and parameter control and info     !
   ! structures to temporary variables to make the code more  !
   ! readable.                                                !
   \*--------------------------------------------------------*/
   info          = field->info;

   tile_info     = &tile->info;

   param_control = &parameter->control;
   param_info    = &parameter->info;

   /*--------------------------------------------------------*\
   ! Calculate the offset of the current parameter in the da- !
   ! ta array.                                                !
   \*--------------------------------------------------------*/
   param_offset = 0;
   param        = data->info.parameter->root;

   while(param != NULL && param->id < param_id)
   {
      if(param->precision == param_info->precision)
      {
         param_offset += param->size;
      }
      param = param -> next;
   }

   /*--------------------------------------------------------*\
   ! Associate the working buffer with the data pointer in    !
   ! the bwc_parameter structure.                             !
   \*--------------------------------------------------------*/
   parameter->data = working_buffer;

   /*--------------------------------------------------------*\
   ! Initialize the maximum and minimum parameter value.      !
   \*--------------------------------------------------------*/
   max  = param_info->parameter_max;
   min  = param_info->parameter_min;

   /*--------------------------------------------------------*\
   ! Calculate the width, height, depth and dt of the current !
   ! tile parameter.                                          !
   \*--------------------------------------------------------*/
   width  = param_info->X1 - param_info->X0;
   height = param_info->Y1 - param_info->Y0;
   depth  = param_info->Z1 - param_info->Z0;
   dt     = param_info->TS1 - param_info->TS0;

   /*--------------------------------------------------------*\
   ! Check if the parameter is single or double precision and !
   ! handle the field accordingly.                            !
   \*--------------------------------------------------------*/
   if(param_info->precision == 8)
   {
      /*--------------------------------------------------------*\
      ! Safe the field pointer to a temporary variable to make   !
      ! the code more readable.                                  !
      \*--------------------------------------------------------*/
      tmp_d = data->field.d;

      /*--------------------------------------------------------*\
      ! Walk through the tile parameter working buffer.          !
      \*--------------------------------------------------------*/
      #if defined(_OPENMP)
         #pragma omp parallel for collapse(3) private(dest, src_d, x) reduction(max:max) reduction(min:min)
      #endif
      for(t = 0; t < dt; ++t)
      {
         for(z = 0; z < depth; ++z)
         {
            for(y = 0; y < height; ++y)
            {
               /*--------------------------------------------------------*\
               ! Advance the temporary data and buffer pointers to their  !
               ! appropriate positions in the working buffer and data     !
               ! arrays.                                                  !
               \*--------------------------------------------------------*/
               src_d = &tmp_d[param_offset + tile_info->X0 + info->nX * 
                                            (tile_info->Y0 + y + info->nY * 
                                            (tile_info->Z0 + z + info->nZ * t))];

               dest = &working_buffer[(uint64) width * (y + height * (z + depth * t))];

               for(x = 0; x < width; ++x)
               {
                  /*--------------------------------------------------------*\
                  ! Copy the data sample to the working buffer.              !
                  \*--------------------------------------------------------*/
                  dest[x].f = (bwc_float)src_d[x];

                  /*--------------------------------------------------------*\
                  ! Update the maximum and minimum parameter value.          !
                  \*--------------------------------------------------------*/
                  max  = MAX(max, dest[x].f);
                  min  = MIN(min, dest[x].f);
               }
            }
         }
      }
   }
   else
   {
      /*--------------------------------------------------------*\
      ! Safe the field pointer to a temporary variable to make   !
      ! the code more readable.                                  !
      \*--------------------------------------------------------*/
      tmp_f = data->field.f;

      /*--------------------------------------------------------*\
      ! Walk through the tile parameter working buffer.          !
      \*--------------------------------------------------------*/
      #if defined(_OPENMP)
         #pragma omp parallel for collapse(3) private(dest, src_f, x) reduction(max:max) reduction(min:min)
      #endif
      for(t = 0; t < dt; ++t)
      {
         for(z = 0; z < depth; ++z)
         {
            for(y = 0; y < height; ++y)
            {
               /*--------------------------------------------------------*\
               ! Advance the temporary data and buffer pointers to their  !
               ! appropriate positions in the working buffer and data     !
               ! arrays.                                                  !
               \*--------------------------------------------------------*/
               src_f = &tmp_f[param_offset + tile_info->X0 + info->nX * 
                                            (tile_info->Y0 + y + info->nY * 
                                            (tile_info->Z0 + z + info->nZ * t))];

               dest = &working_buffer[(uint64) width * (y + height * (z + depth * t))];

               for(x = 0; x < width; ++x)
               {
                  /*--------------------------------------------------------*\
                  ! Copy the data sample to the working buffer.              !
                  \*--------------------------------------------------------*/
                  dest[x].f = (bwc_float)src_f[x];

                  /*--------------------------------------------------------*\
                  ! Update the maximum and minimum parameter value.          !
                  \*--------------------------------------------------------*/
                  max  = MAX(max, dest[x].f);
                  min  = MIN(min, dest[x].f);
               }
            }
         }
      }
   }

   /*--------------------------------------------------------*\
   ! Update the maximum and minimum parameter value in the    !
   ! parameter structure.                                     !
   \*--------------------------------------------------------*/
   param_info->parameter_max  = max;
   param_info->parameter_min  = min;

   /*--------------------------------------------------------*\
   ! Calculate the constants used to normalize the tile pa-   !
   ! rameter to the range of [-1,1).                          !
   \*--------------------------------------------------------*/
   param_control->alpha = (param_info->parameter_max + param_info->parameter_min)/2.0f;
   param_control->beta  = 2.0f/(param_info->parameter_max - param_info->parameter_min);
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void fill_buffer(bwc_field *const field, bwc_tile *const tile,                            !
!   ------------                                  bwc_parameter *const parameter,                            !
!                                               bwc_sample *const working_buffer,                            !
!                                                             double *const data,                            !
!                                                              uint64 param_size,                            !
!                                                                uint16 param_id)                            !
!                                                                                                            !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to flush the working buffer to the flow field data memory block.      !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                tile                        bwc_tile*             - Structure defining a bwc tile.          !
!                                                                                                            !
!                parameter                   bwc_parameter*        - Structure defining a bwc parameter.     !
!                                                                                                            !
!                working_buffer              bwc_sample*           - Working buffer used to store flow field !
!                                                                    data for a specific tile parameter.     !
!                                                                                                            !
!                param_size                  uint64                - Specifies the byte size of one tile     !
!                                                                    parameter.                              !
!                                                                                                            !
!                parameter                   uint16                - Specifies the parameter index for the   !
!                                                                    current tile parameter.                 !
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
!                22.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
flush_buffer(bwc_field *const field, bwc_tile *const tile, bwc_parameter *const parameter,
                                                         bwc_sample *const working_buffer,
                                                                     bwc_data *const data,
                                                                          uint16 param_id)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   width, height, depth, dt;
   uint64   nX, nY, nZ;
   uint64   param_offset;
   uint64   x, y, z;
   uint16   t;

   /*-----------------------*\
   ! DEFINE FLOAT VARIABLES: !
   \*-----------------------*/
   double     *dst_d, *tmp_d;
   float      *dst_f, *tmp_f;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_sample       *src;
   bwc_gl_inf       *info;
   bwc_param_ctrl   *param_control;
   bwc_param_inf    *param_info;
   bwc_cmd_opts_ll  *param;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(tile);
   assert(parameter);
   assert(working_buffer);
   assert(data);

   /*--------------------------------------------------------*\
   ! Save the global, tile and parameter control and info     !
   ! structures to temporary variables to make the code more  !
   ! readable.                                                !
   \*--------------------------------------------------------*/
   info          =  field->info;

   param_control = &parameter->control;
   param_info    = &parameter->info;

   nX            = info->nX;
   nY            = info->nY;
   nZ            = info->nZ;

   /*--------------------------------------------------------*\
   ! Calculate the offset of the current parameter in the da- !
   ! ta array.                                                !
   \*--------------------------------------------------------*/
   param_offset = 0;
   param        = data->info.parameter->root;

   while(param != NULL && param->id < param_id)
   {
      if(param->precision == param_info->precision)
      {
         param_offset += param->size;
      }
      param = param -> next;
   }

   /*--------------------------------------------------------*\
   ! Calculate the width, height, depth and dt of the current !
   ! tile parameter.                                          !
   \*--------------------------------------------------------*/
   width  = param_info->X1  - param_info->X0;
   height = param_info->Y1  - param_info->Y0;
   depth  = param_info->Z1  - param_info->Z0;
   dt     = param_info->TS1 - param_info->TS0;

   /*--------------------------------------------------------*\
   ! Check if the parameter is single or double precision and !
   ! handle the field accordingly.                            !
   \*--------------------------------------------------------*/
   if(param_info->precision == 8)
   {
      /*--------------------------------------------------------*\
      ! Safe the field pointer to a temporary variable to make   !
      ! the code more readable.                                  !
      \*--------------------------------------------------------*/
      tmp_d = data->field.d;

      /*--------------------------------------------------------*\
      ! Walk through the tile parameter working buffer.          !
      \*--------------------------------------------------------*/
      #if defined(_OPENMP)
         #pragma omp parallel for collapse(3) private(dst_d, src, x)
      #endif
      for(t = 0; t < dt; ++t)
      {
         for(z = 0; z < depth; ++z)
         {
            for(y = 0; y < height; ++y)
            {
               /*--------------------------------------------------------*\
               ! Advance the temporary data and buffer pointers to their  !
               ! appropriate positions in the working buffer and data     !
               ! arrays.                                                  !
               \*--------------------------------------------------------*/
               dst_d = &tmp_d[param_offset + param_info->X0 + nX * 
                                            (param_info->Y0 + y  + nY * 
                                            (param_info->Z0 + z  + nZ * t ))];

               src = &working_buffer[(uint64) width * (y + height * (z + depth * t))];

               for(x = 0; x < width; ++x)
               {
                  /*--------------------------------------------------------*\
                  ! Copy the sample to the data memory block.                !
                  \*--------------------------------------------------------*/
                  dst_d[x] = (double)src[x].f;
               }
            }
         }
      }
   }
   else
   {
      /*--------------------------------------------------------*\
      ! Safe the field pointer to a temporary variable to make   !
      ! the code more readable.                                  !
      \*--------------------------------------------------------*/
      tmp_f = data->field.f;

      /*--------------------------------------------------------*\
      ! Walk through the tile parameter working buffer.          !
      \*--------------------------------------------------------*/
      #if defined(_OPENMP)
         #pragma omp parallel for collapse(3) private(dst_f, src, x)
      #endif
      for(t = 0; t < dt; ++t)
      {
         for(z = 0; z < depth; ++z)
         {
            for(y = 0; y < height; ++y)
            {
               /*--------------------------------------------------------*\
               ! Advance the temporary data and buffer pointers to their  !
               ! appropriate positions in the working buffer and data     !
               ! arrays.                                                  !
               \*--------------------------------------------------------*/
               dst_f = &tmp_f[param_offset + param_info->X0 + nX * 
                                          (param_info->Y0 + y  + nY * 
                                          (param_info->Z0 + z  + nZ * t ))];

               src = &working_buffer[(uint64) width * (y + height * (z + depth * t))];

               for(x = 0; x < width; ++x)
               {
                  /*--------------------------------------------------------*\
                  ! Copy the sample to the data memory block.                !
                  \*--------------------------------------------------------*/
                  dst_f[x] = (float)src[x].f;
               }
            }
         }
      }
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void buff_normalize(bwc_field *const field, bwc_parameter *const parameter)               !
!                                                                                                            !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to normalize the values v[i] of the current tile parameter to the     !
!                range of [-1, 1) and scale them to a desired dynamic range:                                 !
!                                                                                                            !
!                                              q[i] = [(v[i] - α)/β] * 2^Qm.                                 !
!                                                                                                            !
!                Here, α and β define the normalization constants and Qm represents the dynamic range of     !
!                the Q number format.                                                                        !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                parameter                   bwc_parameter*        - Structure defining a bwc parameter.     !
!                                                                                                            !
!                working_buffer              bwc_sample*           - Working buffer used to store flow field !
!                                                                    data for a specific tile parameter.     !
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
!                24.10.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
normalize_param(bwc_field *const field, bwc_parameter *const parameter)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   bwc_raw  shift;
   uint64   param_size;
   uint64   x;

   /*-----------------------*\
   ! DEFINE FLOAT VARIABLES: !
   \*-----------------------*/
   bwc_float   alpha, beta;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;
   bwc_param_ctrl    *param_control;
   bwc_param_inf     *param_info;
   bwc_sample        *tmp;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(parameter);
   assert(parameter->data);

   /*--------------------------------------------------------*\
   ! Save the global control and the parameter control and    !
   ! info structures to temporary variables to make the code  !
   ! more readable.                                           !
   \*--------------------------------------------------------*/
   control       = &field->control;

   param_control = &parameter->control;
   param_info    = &parameter->info;

   /*--------------------------------------------------------*\
   ! Save the normalization constants to temporary variables  !
   ! to make the code more readable.                          !
   \*--------------------------------------------------------*/
   alpha = param_control->alpha;
   beta  = param_control->beta;

   /*--------------------------------------------------------*\
   ! Calculate the size of the current tile parameter.        !
   \*--------------------------------------------------------*/
   param_size  = (param_info->X1 - param_info->X0) *
                 (param_info->Y1 - param_info->Y0) *
                 (param_info->Z1 - param_info->Z0) *
                 (param_info->TS1 - param_info->TS0);

   /*--------------------------------------------------------*\
   ! Associate the temporary pointer with the data pointer in !
   ! the bwc_parameter structure.                             !
   \*--------------------------------------------------------*/
   tmp = parameter->data;

   /*--------------------------------------------------------*\
   ! Shift the Q number range to the appropriate position for !
   ! the IEEE 754 floating point number.                      !
   \*--------------------------------------------------------*/
   shift = (uint64)control->Qm << PREC_MANTISSA;

   /*--------------------------------------------------------*\
   ! Walk through the tile parameter working buffer.          !
   \*--------------------------------------------------------*/
   #if defined(_OPENMP)
      #pragma omp parallel for
   #endif
   for(x = 0; x < param_size; ++x)
   {
      /*--------------------------------------------------------*\
      ! Normalize the flow field values to the range of [-1,1).  !
      \*--------------------------------------------------------*/
      tmp[x].f            = (bwc_float)(tmp[x].f - alpha) * beta;

      /*--------------------------------------------------------*\
      ! Scale the flow field values to the dynamic range speci-  !
      ! fied by the Qm parameter.                                !
      \*--------------------------------------------------------*/
      tmp[x].raw += (((tmp[x].raw & EXPONENT) == 0)&& 
                     ((tmp[x].raw & MANTISSA) == 0)) ? 0 : shift;
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void denormalize_param(bwc_field *const field, bwc_parameter *const parameter)            !
!                                                                                                            !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to scale the decompressed values to their original dynamic range and  !
!                denormalize the flow field values.                                                          !
!                                                                                                            !
!                                              q[i] = [(v[i]/2^Qm) * β] + α.                                 !
!                                                                                                            !
!                Here, α and β define the normalization constants and Qm represents the dynamic range of     !
!                the Q number format.                                                                        !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                parameter                   bwc_parameter*        - Structure defining a bwc parameter.     !
!                                                                                                            !
!                working_buffer              bwc_sample*           - Working buffer used to store flow field !
!                                                                    data for a specific tile parameter.     !
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
!                24.10.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
denormalize_param(bwc_field *const field, bwc_parameter *const parameter)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   bwc_raw  shift;
   uint64   param_size;
   uint64   x;

   /*-----------------------*\
   ! DEFINE FLOAT VARIABLES: !
   \*-----------------------*/
   bwc_float   alpha, beta;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;
   bwc_param_inf     *param_info;
   bwc_sample        *tmp;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(parameter);
   assert(parameter->data);

   /*--------------------------------------------------------*\
   ! Save the global control and the parameter info struc-    !
   ! tures to temporary variables to make the code more       !
   ! readable.                                                !
   \*--------------------------------------------------------*/
   control       = &field->control;

   param_info    = &parameter->info;

   /*--------------------------------------------------------*\
   ! Save the normalization constants to temporary variables  !
   ! to make the code more readable.                          !
   \*--------------------------------------------------------*/
   alpha = (param_info->parameter_max + param_info->parameter_min)/2.0f;
   beta  = (param_info->parameter_max - param_info->parameter_min)/2.0f;

   /*--------------------------------------------------------*\
   ! Calculate the size of the current tile parameter.        !
   \*--------------------------------------------------------*/
   param_size  = (param_info->X1 - param_info->X0) *
                 (param_info->Y1 - param_info->Y0) *
                 (param_info->Z1 - param_info->Z0) *
                 (param_info->TS1 - param_info->TS0);

   /*--------------------------------------------------------*\
   ! Associate the temporary pointer with the data pointer in !
   ! the bwc_parameter structure.                             !
   \*--------------------------------------------------------*/
   tmp = parameter->data;

   /*--------------------------------------------------------*\
   ! Shift the Q number range to the appropriate position for !
   ! the IEEE 754 floating point number.                      !
   \*--------------------------------------------------------*/
   shift = (uint64)control->Qm << PREC_MANTISSA;

   /*--------------------------------------------------------*\
   ! Walk through the tile parameter working buffer.          !
   \*--------------------------------------------------------*/
   #if defined(_OPENMP)
      #pragma omp parallel for
   #endif
   for(x = 0; x < param_size; ++x)
   {
      /*--------------------------------------------------------*\
      ! Scale the flow field values to their original dynamic    !
      ! range.                                                   !
      \*--------------------------------------------------------*/
      tmp[x].raw -= (((tmp[x].raw & EXPONENT) == 0)&& 
                     ((tmp[x].raw & MANTISSA) == 0)) ? 0 : shift;

      /*--------------------------------------------------------*\
      ! Denormalize the flow field values.                       !
      \*--------------------------------------------------------*/
      tmp[x].f            = (bwc_float)(tmp[x].f * beta) + alpha;
   }
}

/************************************************************************************************************\
||                  ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                  ||
||                  |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__                   ||
||                  |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]                  ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: bwc_field *bwc_initialize_data(...)                                                       !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function initializes the bwc_data structure with all necessary parameters.             !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                nX ,nY, nZ                  unsigned int(64 bit)  - Number of spatial Datapoints            !
!                                                                                                            !
!                nTS                         unsigned int(16 bit)  - Number of Timesteps.                    !
!                                                                                                            !
!                nPar                        unsigned int(8 bit)   - Number of Parameters                    !
!                                                                                                            !
!                file_extension              char*                 - File extension associated               !
!                                                                    with the numerical dataset.             !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                bwc_data*                 - Structure used to store a numerical                             !
!                                            dataset/compressed bitstream.                                   !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                09.06.2021  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
bwc_data*
bwc_initialize_data(double* field, uint64 const nX, uint64 const nY, uint64 const nZ, uint16 const nTS, uint8 const nPar, char *file_extension)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_inf *info;
   bwc_data   *file;

   file = calloc(1, sizeof(bwc_data));
   if(!file)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return NULL;      
   }

   info = &file->info;

   info->nX  = nX;
   info->nY  = nY;
   info->nZ  = nZ;
   info->nTS = nTS;
   info->nPar = nPar;
   memcpy(&info->f_ext, file_extension, sizeof(char) * 19);

   file->field.d = field;

   return file;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_add_param(bwc_data* data, char *name, uint16 sample, uchar dim, uint8 precision) !
!   --------------                                                                                           !
!                                                                                                            !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function initializes and adds new parameters to the parameter linked list. The linked  !
!                list stores the parameter name, its precision, sampling factor and the dimension for which  !
!                the sampling is active.                                                                     !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                name                        char*                 - Name of parameter(id).                  !
!                                                                                                            !
!                sample                      unsigned int(16 bit)  - Sampling factor for parameter(id).      !
!                                                                                                            !
!                dim                         unsigned char         - Dimension(s) for which the sampling     !
!                                                                    factor is active.                       !
!                                                                                                            !
!                precision                   unsigned int(8 bit)   - Bit precision of the parameter.         !
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
!                19.10.2017  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                03.12.2019  Patrick Vogler     B87E7E4     V 0.1.0     redefined the function for the       !
!                                                                       bwc_data structure.                  !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bwc_add_param(bwc_data* data, char *name, uint8 precision)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_inf  *info;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(data);
   assert(dim <= DIM_ALL);

   /*--------------------------------------------------------*\
   ! Save the global info structure to a temporary variable   !
   ! to make the code more readable.                          !
   \*--------------------------------------------------------*/
   info = &data->info;

   /*--------------------------------------------------------*\
   ! Check if the specified parameter name has the proper     !
   ! length.                                                  !
   \*--------------------------------------------------------*/
   if((strlen(name) > 24) && name)
   {
      fprintf(stderr, "o==========================================================o\n"\
                      "| WARNING: Invalid parameter name: %-24s|\n"\
                      "|                                                          |\n"\
                      "|          Parameter names cannot exceed 24 characters.    |\n"\
                      "|                                                          |\n"\
                      "o==========================================================o\n",name);
   }

   /*--------------------------------------------------------*\
   ! Check if the parameter structure has already been allo-  !
   ! cated.                                                   !
   \*--------------------------------------------------------*/
   if(info->parameter == NULL)
   {
      /*--------------------------------------------------------*\
      ! If the bwc_add_param function is called for the first    !
      ! time, allocate the parameter structure and save the root !
      ! node address.                                            !
      \*--------------------------------------------------------*/
      info->parameter       = calloc(1, sizeof(bwc_cmd_opts_ll));
      info->parameter->root = info->parameter;
   }
   else
   {
      /*--------------------------------------------------------*\
      ! If a new parameter is added, allocate the nex linked     !
      ! list node, save the root node address in its structure   !
      ! and set the linked list access pointer to the new node.  !
      \*--------------------------------------------------------*/
      info->parameter->next       = calloc(1, sizeof(bwc_cmd_opts_ll));
      info->parameter->next->root = info->parameter->root;
      info->parameter->next->id   = info->parameter->id + 1;
      info->parameter             = info->parameter->next;
   }

   /*--------------------------------------------------------*\
   ! Save the name of the new parameter its precision in the  !
   ! structure of the new node.                               !
   \*--------------------------------------------------------*/
   strcpy(info->parameter->name, name ? name : "undefined");
   info->parameter->precision = precision     ? precision : PREC_BYTE;

   /*--------------------------------------------------------*\
   ! Evaluate the parameter size the information in the       !
   ! linked list.                                             !
   \*--------------------------------------------------------*/
   info->parameter->size = (info->nX * info->nY * info->nZ * info->nTS);
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: bwc_field *bwc_initialize_data(...)                                                       !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to copy the numerical dataset stored in the bwc_data                  !
!                structure to a user supplied memory block. A size argument is necessary                     !
!                to verify that the memory block has the correct size.                                       !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                data                        bwc_data*             - Structure used to store a numerical     !
!                                                                    dataset/compressed bitstream.           !
!                                                                                                            !
!                buffer                      unsigned char*        - Memory block supplied by                !
!                                                                    the function caller.                    !
!                                                                                                            !
!                data                        bwc_data*             - Size of memory block.                   !
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
!                09.06.2021  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bwc_get_data(bwc_data* data, uchar* buffer, uint64 size)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_inf *info;

   info = &data->info;

   if(size != (uint64)(data->info.nX * data->info.nY * data->info.nZ * data->info.nTS * data->info.nPar))
      {
         fprintf(stderr, "Incorrect buffer size\n");
      }
   else
      {
         memcpy(buffer, data->field.d, size * sizeof(double));
      }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_free_data(bwc_data* file)                                                        !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function deallocates the data structure used to store an numerical dataset/compressed  !
!                and can be called if an error occurs or once the data is no longer needed is to be closed.  !
!                The deallocation will be carried out down to the structure levels that have been allocated. !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                                                                                                            !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                file                        bwc_data              - Defines a structure used to store all   !
!                                                                    the relevant parameters and the data    !
!                                                                    field of a numerical dataset or com-    !
!                                                                    pressed codestream.                     !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                                                                                                            !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                -                           -                       -                                       !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                20.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                04.12.2019  Patrick Vogler     B87E7E4     V 0.1.0     
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bwc_free_data(bwc_data* data)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_cmd_opts_ll   *param, *temp;

   if(data)
   {   
      if(data->info.parameter)
      {
         param = data->info.parameter->root;

         while(param != NULL)
         {
            temp  = param;
            param = param -> next;
            free(temp);
         }
      }

      if(data)
      {
         if(data->codestream.data)
         {
            release_packed_stream(data->codestream.data);
         }
         if(data->codestream.aux)
         {
            release_packed_stream(data->codestream.aux);
         }
         if(data->codestream.com)
         {
            release_packed_stream(data->codestream.com);
         }
         if(data->file.fp)
         {
            fclose(data->file.fp);
         }
         free(data->codestream.data);
         free(data->codestream.aux);
         free(data->codestream.com);
         free(data->file.d_root);
         free(data->file.f_root);
         free(data->field.d);
         free(data->field.f);
         free(data);
      }
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uchar create_field(bwc_field *const field)                                                !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function creates the field structure used to (de)compress a floating point array de-   !
!                fined by the bwc_initialize function.                                                       !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                unsigned char             - Returns an unsigned char for error handling.                    !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                15.03.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
create_field(bwc_field *const field)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   cblk, nPackets;
   uint32   num_tiles_X, num_tiles_Y, num_tiles_Z, num_tiles_TS;
   uint32   c, i, p, t, x, y, z;
   uint32   max_Prec;
   int16    r;
   uint8    j;
   int8     l,m, n;

   /*-----------------------*\
   ! DEFINE CHAR VARIABLES:  !
   \*-----------------------*/
   uchar    highband_flag = 0;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl      *control;
   bwc_gl_inf       *info;
   bwc_cmd_opts_ll  *param;
   bwc_parameter    *parameter;
   bwc_param_ctrl   *param_control;
   bwc_param_inf    *param_info;
   bwc_packet       *packet;
   bwc_resolution   *resolution;
   bwc_res_ctrl     *res_control;
   bwc_res_inf      *res_info;
   bwc_tile         *tile;
   bwc_tile_ctrl    *tile_control;
   bwc_tile_inf     *tile_info;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);

   /*--------------------------------------------------------*\
   ! Save the global control and info structure to temporary  !
   ! variables to make the code more readable.                !
   \*--------------------------------------------------------*/
   control = &field->control;
   info    =  field->info;

   /*--------------------------------------------------------*\
   ! Calculate the number of tiles in all spatial and tempo-  !
   ! ral directions and allocate the tile structure accord-   !
   ! ingly.                                                   !
   \*--------------------------------------------------------*/
   num_tiles_X  = (int)ceil(((float)info->nX / control->tileSizeX));
   num_tiles_Y  = (int)ceil(((float)info->nY / control->tileSizeY));
   num_tiles_Z  = (int)ceil(((float)info->nZ / control->tileSizeZ));
   num_tiles_TS = (int)ceil(((float)info->nTS/ control->tileSizeTS));

   field->tile  = calloc(control->nTiles, sizeof(bwc_tile));
   if(!field->tile) 
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Walk through and setup the tile structure according to   !
   ! the specified compression parameters.                    !
   \*--------------------------------------------------------*/
   for(t = 0, i = 0; t < num_tiles_TS; ++t)
   {
      for(z = 0; z < num_tiles_Z; ++z)
      {
         for(y = 0; y < num_tiles_Y; ++y)
         {
            for(x = 0; x < num_tiles_X; ++x, ++i)
            {
               /*--------------------------------------------------------*\
               ! Initialize the number of packets and maximum number of   !
               ! precincts variables.                                     !
               \*--------------------------------------------------------*/
               nPackets = 0;
               max_Prec = 0;

               /*--------------------------------------------------------*\
               ! Save the tile and its info and control structure to tem- !
               ! porary variables to make the code more readable.         !
               \*--------------------------------------------------------*/
               tile         = &field->tile[i];
               tile_control = &tile->control;
               tile_info    = &tile->info;

               /*--------------------------------------------------------*\
               ! Save the index for the current tile.                     !
               \*--------------------------------------------------------*/
               tile_info->tile_index = i;

               /*--------------------------------------------------------*\
               ! Calculate the boundaries for the current tile.           !
               \*--------------------------------------------------------*/
               tile_info->X0  = (uint64)MAX(0, x * control->tileSizeX);
               tile_info->Y0  = (uint64)MAX(0, y * control->tileSizeY);
               tile_info->Z0  = (uint64)MAX(0, z * control->tileSizeZ);
               tile_info->TS0 = (uint16)MAX(0, t * control->tileSizeTS);
               tile_info->X1  = (uint64)MIN(info->nX , (x + 1) * control->tileSizeX);
               tile_info->Y1  = (uint64)MIN(info->nY , (y + 1) * control->tileSizeY);
               tile_info->Z1  = (uint64)MIN(info->nZ , (z + 1) * control->tileSizeZ);
               tile_info->TS1 = (uint16)MIN(info->nTS, (t + 1) * control->tileSizeTS);

               /*--------------------------------------------------------*\
               ! Initialize the tile header size.                         !
               \*--------------------------------------------------------*/
               tile_control->header_size = 14;

               /*--------------------------------------------------------*\
               ! Initialize the convex hull slope threshold.              !
               \*--------------------------------------------------------*/
               tile_control->slope_max = 0x0000;
               tile_control->slope_min = 0xFFFF;

               /*--------------------------------------------------------*\
               ! Allocate, walk through and setup the parameter structure !
               ! according to the specified compression parameters.       !
               \*--------------------------------------------------------*/
               tile->parameter = calloc(info->nPar, sizeof(bwc_parameter));
               if(!tile->parameter) 
               {
                  // memory allocation error 
                  fprintf(stderr, MEMERROR);
                  return 1;
               }

               /*--------------------------------------------------------*\
               ! Initialize the parameter information linked listed.      !
               \*--------------------------------------------------------*/
               param = info->parameter->root;

               for(j = 0; j < info->nPar; ++j)
               {
                  /*--------------------------------------------------------*\
                  ! Check if there is a node in the parameter information    !
                  ! linked list corresponding to the current paramter.       !
                  \*--------------------------------------------------------*/
                  if(!param && (param->id == j))
                  {
                     fprintf(stderr, MEMERROR);
                     return 1;
                  }

                  /*--------------------------------------------------------*\
                  ! Save the parameter and its info and control structure to !
                  ! temporary variables to make the code more readable.      !
                  \*--------------------------------------------------------*/
                  parameter     = &tile->parameter[j];
                  param_control = &parameter->control;
                  param_info    = &parameter->info;

                  /*--------------------------------------------------------*\
                  ! Save the corresponding parameter name and its sampling   !
                  ! factors and precision in the info and control structures.!
                  \*--------------------------------------------------------*/
                  param_info->name      = param->name;
                  param_info->precision = param->precision;

                  /*--------------------------------------------------------*\
                  ! Initialize the number of codeblocks for the current      !
                  ! parameter.                                               !
                  \*--------------------------------------------------------*/
                  param_control->number_of_codeblocks = 0;
                  cblk                                = 0;

                  /*--------------------------------------------------------*\
                  ! Initialize the maximum and minimum parameter value in    !
                  ! the parameter structure.                                 !
                  \*--------------------------------------------------------*/
                  param_info->parameter_max  = -FLT_MAX;
                  param_info->parameter_min  =  FLT_MAX;

                  /*--------------------------------------------------------*\
                  ! Safe the boundaries of the current tile in the parameter !
                  ! info structure.
                  \*--------------------------------------------------------*/
                  param_info->X0  = tile->info.X0;
                  param_info->Y0  = tile->info.Y0;
                  param_info->Z0  = tile->info.Z0;
                  param_info->TS0 = tile->info.TS0;
                  param_info->X1  = tile->info.X1;
                  param_info->Y1  = tile->info.Y1;
                  param_info->Z1  = tile->info.Z1;
                  param_info->TS1 = tile->info.TS1;
                  
                  /*--------------------------------------------------------*\
                  ! Allocate, walk through and setup the resolution struc-   !
                  ! ture according to the specified compression parameters.  !
                  \*--------------------------------------------------------*/
                  parameter->resolution = calloc(control->nDecomp + 1, sizeof(bwc_resolution));
                  if(!parameter->resolution) 
                  {
                     // memory allocation error
                     fprintf(stderr, MEMERROR);
                     return 1;
                  }
                  for(r = 0; r < control->nDecomp + 1; ++r)
                  {
                     /*--------------------------------------------------------*\
                     ! Save the resolution and its info and control structure   !
                     ! to temporary variables to make the code more readable.   !
                     \*--------------------------------------------------------*/
                     resolution  = &parameter->resolution[r];
                     res_control = &resolution->control;
                     res_info    = &resolution->info;

                     /*--------------------------------------------------------*\
                     ! Calculate the boundaries for the current resolution ac-  !
                     ! cording to the corresponding equation from JPEG2000 by   !
                     ! David S. Taubman and Michael W. Marcellin (p.456).       !
                     \*--------------------------------------------------------*/
                     res_info->X0  = (uint64)ceil((float)param_info->X0 / (1 << MIN(control->nDecomp -r, control->decompX)));
                     res_info->Y0  = (uint64)ceil((float)param_info->Y0 / (1 << MIN(control->nDecomp -r, control->decompY)));
                     res_info->Z0  = (uint64)ceil((float)param_info->Z0 / (1 << MIN(control->nDecomp -r, control->decompZ)));
                     res_info->TS0 = (uint16)ceil((float)param_info->TS0/ (1 << MIN(control->nDecomp -r, control->decompTS)));
                     res_info->X1  = (uint64)ceil((float)param_info->X1 / (1 << MIN(control->nDecomp -r, control->decompX)));
                     res_info->Y1  = (uint64)ceil((float)param_info->Y1 / (1 << MIN(control->nDecomp -r, control->decompY)));
                     res_info->Z1  = (uint64)ceil((float)param_info->Z1 / (1 << MIN(control->nDecomp -r, control->decompZ)));
                     res_info->TS1 = (uint16)ceil((float)param_info->TS1/ (1 << MIN(control->nDecomp -r, control->decompTS)));

                     /*--------------------------------------------------------*\
                     ! Calculate the number of precincts to cover tile t in pa- !
                     ! rameter p at resolution r according to equation (11.10)  !
                     ! from JPEG2000 by D. S. Taubman and M. W. Marcellin       !
                     ! (p.459).                                                 !
                     \*--------------------------------------------------------*/
                     res_control->numPX  = (uint16)((res_info->X0  == res_info->X1)  ? 0 : (ceil((float)res_info->X1  / (1 << control->precSizeX) 
                                                                                         - floor((float)res_info->X0  / (1 << control->precSizeX)))));
                     res_control->numPY  = (uint16)((res_info->Y0  == res_info->Y1)  ? 0 : (ceil((float)res_info->Y1  / (1 << control->precSizeY) 
                                                                                         - floor((float)res_info->Y0  / (1 << control->precSizeY)))));
                     res_control->numPZ  = (uint16)((res_info->Z0  == res_info->Z1)  ? 0 : (ceil((float)res_info->Z1  / (1 << control->precSizeZ) 
                                                                                         - floor((float)res_info->Z0  / (1 << control->precSizeZ)))));
                     res_control->numPTS = (uint16)((res_info->TS0 == res_info->TS1) ? 0 : (ceil((float)res_info->TS1 / (1 << control->precSizeTS)
                                                                                         - floor((float)res_info->TS0 / (1 << control->precSizeTS)))));

                     res_control->number_of_precincts = (uint64)res_control->numPX * res_control->numPY * res_control->numPZ * res_control->numPTS;

                     /*--------------------------------------------------------*\
                     ! Add up the global number of packets and evaluate the     !
                     ! maximum number of precincts between all resolution lev-  !
                     ! els for all tile parameters.                             !
                     \*--------------------------------------------------------*/
                     nPackets += (res_control->number_of_precincts * control->nLayers);
                     max_Prec  = MAX(res_control->number_of_precincts * control->nLayers, max_Prec);

                     /*--------------------------------------------------------*\
                     ! Calculate and save the real codeblock size in the reso-  !
                     ! lution control structure.                                !
                     \*--------------------------------------------------------*/
                     if(r == 0)
                     {
                        res_control->rcbX  = MIN(control->cbX,  (uint8)(control->precSizeX));
                        res_control->rcbY  = MIN(control->cbY,  (uint8)(control->precSizeY));
                        res_control->rcbZ  = MIN(control->cbZ,  (uint8)(control->precSizeZ));
                        res_control->rcbTS = MIN(control->cbTS, (uint8)(control->precSizeTS));
                     }
                     else
                     {
                        res_control->rcbX  = MIN(control->cbX,  (uint8)(control->precSizeX  - 1));
                        res_control->rcbY  = MIN(control->cbY,  (uint8)(control->precSizeY  - 1));
                        res_control->rcbZ  = MIN(control->cbZ,  (uint8)(control->precSizeZ  - 1));
                        res_control->rcbTS = MIN(control->cbTS, (uint8)(control->precSizeTS - 1));
                     }

                     /*--------------------------------------------------------*\
                     ! Evaluate in which spatial or temporal directions the     !
                     ! wavelet decomposition has been applied to according to   !
                     ! the current resolution level and amend the highband flag !
                     ! accordingly.                                             !
                     \*--------------------------------------------------------*/
                     highband_flag  = (control->nDecomp - control->decompX  < r && r > 0) ? DIM_X  : 0;
                     highband_flag |= (control->nDecomp - control->decompY  < r && r > 0) ? DIM_Y  : 0;
                     highband_flag |= (control->nDecomp - control->decompZ  < r && r > 0) ? DIM_Z  : 0;
                     highband_flag |= (control->nDecomp - control->decompTS < r && r > 0) ? DIM_TS : 0;

                     /*--------------------------------------------------------*\
                     ! Evaluate the overall number of subbands, for the current !
                     ! resolution level, according to the value of the highband !
                     ! flag.                                                    !
                     \*--------------------------------------------------------*/
                     res_control->number_of_subbands = (1 << (((highband_flag & DIM_X))        +
                                                              ((highband_flag & DIM_Y)  >> 1)  +
                                                              ((highband_flag & DIM_Z)  >> 2)  +
                                                              ((highband_flag & DIM_TS) >> 3)));

                     res_control->number_of_subbands -= (res_control->number_of_subbands & 1) ? 0 : 1;

                     /*--------------------------------------------------------*\
                     ! Allocate the point array used to hold the memory handles !
                     ! of all packets created for the current resolution.       !
                     \*--------------------------------------------------------*/
                     resolution->packet = calloc(res_control->number_of_precincts * control->nLayers, sizeof(bwc_packet));
                     if(!resolution->packet) 
                     {
                        // memory allocation error
                        fprintf(stderr, MEMERROR);
                        return 1;
                     }

                     /*--------------------------------------------------------*\
                     ! Allocate, walk through and setup the subband structure   !
                     ! according to the specified compression parameters.       !
                     \*--------------------------------------------------------*/
                     resolution->subband = calloc(res_control->number_of_subbands, sizeof(bwc_subband));
                     if(!resolution->subband) 
                     {
                        // memory allocation error
                        fprintf(stderr, MEMERROR);
                        return 1;
                     }

                     for(l = 0, m = 0; m < res_control->number_of_subbands; ++l)
                     {
                        if(r == 0 || ((l & highband_flag) == l && l != 0))
                        {
                           if(initialize_subband(field, parameter, resolution, &resolution->subband[m], r, l))
                           {
                              return 1;
                           }

                           /*--------------------------------------------------------*\
                           ! Adjust the size of the codeblock access array.           !
                           \*--------------------------------------------------------*/
                           parameter->access = realloc(parameter->access, param_control->number_of_codeblocks * 
                                                                          sizeof(bwc_cblk_access));

                           /*--------------------------------------------------------*\
                           ! Fill the codeblock access array with the appropriate     !
                           ! pointers for the newly initialized precinct, subband and !
                           ! codeblock structures. Save the codeblock index with re-  !
                           ! gards to the current precinct in the codeblock info      !
                           ! structure. Initialize the packet structure with the indi-!
                           ! ces for the parameter, resolution, precinct and quality  !
                           ! layer the packet occupies.                               !
                           \*--------------------------------------------------------*/
                           for(p = 0; p < res_control->number_of_precincts; ++p)
                           {
                              for(c = 0; c < resolution->subband[m].precinct[p].control.number_of_codeblocks; ++c, ++cblk)
                              {
                                 parameter->access[cblk].subband   = &resolution->subband[m];
                                 parameter->access[cblk].precinct  = &resolution->subband[m].precinct[p];
                                 parameter->access[cblk].codeblock = &resolution->subband[m].precinct[p].codeblock[c];

                                 parameter->access[cblk].codeblock->info.idx = c;

                                 for(n = 0; n < control->nLayers; ++n)
                                 {
                                    packet = &resolution->packet[n * res_control->number_of_precincts + p];

                                    packet->c = j;
                                    packet->r = r;
                                    packet->p = p;
                                    packet->l = n;
                                 }
                              }
                           }
                           m++;
                        }
                     }
                  }

                  /*--------------------------------------------------------*\
                  ! Advance the parameter information linked listed to the   !
                  ! next node.                                               !
                  \*--------------------------------------------------------*/
                  param = param->next;
               }

               /*--------------------------------------------------------*\
               ! Allocate the pointer array used to sequence the code-    !
               ! stream packets.                                          !
               \*--------------------------------------------------------*/
               tile->packet_sequence = calloc(nPackets, sizeof(bwc_packet*));
               if(!tile->packet_sequence)
               {
                  // memory allocation error
                  fprintf(stderr, MEMERROR);
                  return 1;
               }
               
               /*--------------------------------------------------------*\
               ! Adjust the tile header size if the error resilience op-  !
               ! tion has been specified and save the nPackets variable.  !
               ! Store the maximum number of Precincts between all reso-  !
               ! lution levels in the tile control structure.             !
               \*--------------------------------------------------------*/
               tile_control->header_size += control->error_resilience ? 8 * nPackets : 0;
               tile_control->nPackets     = nPackets;
               tile_control->max_Prec     = max_Prec;
            }
         }
      }
   }
   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void kill_compression(bwc_field *const field)                                             !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function deallocates the compression field structure used to define and control the    !
!                bwc codec and can be called if an error occurs during the (de-)compression stage or once    !
!                the codec has finished. The deallocation will be carried out down to the structure levels   !
!                that have been allocated.                                                                   !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                                                                                                            !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
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
!                19.10.2017  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bwc_kill_compression(bwc_field *const field)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   c, k;
   uint32   i;
   uint16   j, r;
   int8     l;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;
   bwc_gl_inf        *info;

   if(field)
   {
      /*--------------------------------------------------------*\
      ! Save the global control and info structure to temporary  !
      ! variables to make the code more readable.                !
      \*--------------------------------------------------------*/
      control = &field->control;
      info    =  field->info;

      if(field->tile)
      {
         for(i = 0; i < control->nTiles; ++i)
         {
            if(field->tile[i].parameter)
            {
               for(j = 0; j < info->nPar; ++j)
               {
                  if(field->tile[i].parameter[j].resolution)
                  {
                     for(r = 0; r < control->nDecomp + 1; ++r)
                     {
                        if(field->tile[i].parameter[j].resolution[r].subband)
                        {
                           for(l = 0; l < field->tile[i].parameter[j].resolution[r].control.number_of_subbands; ++l)
                           {
                              if(field->tile[i].parameter[j].resolution[r].subband[l].precinct)
                              {
                                 for(k = 0; k < field->tile[i].parameter[j].resolution[r].control.number_of_precincts; ++k)
                                 {
                                    if(field->tile[i].parameter[j].resolution[r].subband[l].precinct[k].codeblock)
                                    {
                                       for(c = 0; c < field->tile[i].parameter[j].resolution[r].subband[l].precinct[k].control.number_of_codeblocks; ++c)
                                       {
                                          if(field->tile[i].parameter[j].resolution[r].subband[l].precinct[k].codeblock[c].encoded_block)
                                          {
                                             free(field->tile[i].parameter[j].resolution[r].subband[l].precinct[k].codeblock[c].encoded_block->data);
                                          }
                                          free(field->tile[i].parameter[j].resolution[r].subband[l].precinct[k].codeblock[c].control.memory);
                                          free(field->tile[i].parameter[j].resolution[r].subband[l].precinct[k].codeblock[c].encoded_block);
                                       }
                                    }
                                    if(field->tile[i].parameter[j].resolution[r].subband[l].precinct[k].control.number_of_codeblocks)
                                    {
                                       kill_tagtree(field->tile[i].parameter[j].resolution[r].subband[l].precinct[k].control.tag_inclusion);
                                       kill_tagtree(field->tile[i].parameter[j].resolution[r].subband[l].precinct[k].control.tag_msbs);
                                    }
                                    free(field->tile[i].parameter[j].resolution[r].subband[l].precinct[k].codeblock);
                                 }
                              }
                              free(field->tile[i].parameter[j].resolution[r].subband[l].precinct);
                           }
                        }
                        free(field->tile[i].parameter[j].resolution[r].subband);
                        free(field->tile[i].parameter[j].resolution[r].packet);
                     }
                  }
                  free(field->tile[i].parameter[j].resolution);
                  free(field->tile[i].parameter[j].access);
               }
            }
            free(field->tile[i].packet_sequence);
            free(field->tile[i].parameter);
         }
      }
      free(control->header.memory);
      free(control->bitrate);
      free(field->tile);
      free(field);
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: bwc_field *bwc_initialize_field(bwc_data *const data)                                     !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function initializes the bwc_field structure with all necessary standard parameters    !
!                to (de)compress a floating point array with nX * nY * nZ grid points, nTS timesteps and     !
!                nPar parameters.                                                                            !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                nX ,nY, nZ                  unsigned int(64 bit)  - Structure used to store a numerical     !
!                                                                    dataset/compressed bitstream.           !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                bwc_field*                  Defines the (de)compression data structure.                     !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                14.03.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
bwc_field*
bwc_initialize_field(bwc_data *const data)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint8          levelsX, levelsY, levelsZ;
   uint8          levelsTS;

   /*-----------------------*\
   ! DEFINE FLOAT VARIABLES: !
   \*-----------------------*/
   bwc_float      delta;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_ctrl   *control;
   bwc_gl_inf    *info;

   /*--------------------------------------------------------*\
   ! Allocate the root compression data structure.            !
   \*--------------------------------------------------------*/
   bwc_field *field = calloc(1, sizeof(bwc_field));
   if(!field) 
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      bwc_kill_compression(field);
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! Save the global control and info structure to temporary  !
   ! variables to make the code more readable.                !
   \*--------------------------------------------------------*/
   control = &field->control;
   info    =  field->info = &data->info;

   /*--------------------------------------------------------*\
   ! Set all tile sizes to their maximum allowable value. The !
   ! value for the number of tiles is set to 1.               !
   \*--------------------------------------------------------*/
   control->tileSizeX  = info->nX;
   control->tileSizeY  = info->nY;
   control->tileSizeZ  = info->nZ;
   control->tileSizeTS = info->nTS;
   control->nTiles     = 1;

   /*--------------------------------------------------------*\
   ! Set the spatial and temporal precinct sizes to their     !
   ! standard values. For all valid dimensions the values     !
   ! are set to 15 while all invalid dimensions are set to    !
   ! 0.                                                       !
   \*--------------------------------------------------------*/
   control->precSizeX  = (info->nX  >> 1) ? 15 : 0;
   control->precSizeY  = (info->nY  >> 1) ? 15 : 0;
   control->precSizeZ  = (info->nZ  >> 1) ? 15 : 0;
   control->precSizeTS = (info->nTS >> 1) ? 15 : 0;

   /*--------------------------------------------------------*\
   ! Set the spatial and temporal codeblock sizes to their    !
   ! standard values. For all valid dimensions the values     !
   ! are set to 4 while all invalid dimensions are set to     !
   ! 0.                                                       !
   \*--------------------------------------------------------*/
   control->cbX  = (info->nX  >> 1) ? 5 : 0;
   control->cbY  = (info->nY  >> 1) ? 5 : 0;
   control->cbZ  = (info->nZ  >> 1) ? 5 : 0;
   control->cbTS = (info->nTS >> 1) ? 5 : 0;

   /*--------------------------------------------------------*\
   ! Set the spatial and temporal wavelet kernels to their    !
   ! standard values.                                         !
   \*--------------------------------------------------------*/
   control->KernelX  =
   control->KernelY  =
   control->KernelZ  = 
   control->KernelTS = bwc_dwt_9_7;

   /*--------------------------------------------------------*\
   ! Set the progression order, quantization style, number of !
   ! layers, Q number format value and guard bits to their    !
   ! standard values. Amend the quantization style default    !
   ! parameter for the main header, accordingly.              !
   \*--------------------------------------------------------*/
   control->progression        = bwc_prog_LRCP;
   control->quantization_style = bwc_qt_derived;
   control->nLayers            = 0x00;
   control->Qm                 = PREC_BIT - 1;
   control->bitrate            = 0;
   control->guard_bits         = 2;

   /*--------------------------------------------------------*\
   ! Save the codec precision in the info structure.          !
   \*--------------------------------------------------------*/
   info->precision             = PREC_BYTE;

   /*--------------------------------------------------------*\
   ! Initialize the number of threads used for the current    !
   ! run.                                                     !
   \*--------------------------------------------------------*/
   #ifdef _OPENMP
      control->nThreads           = 1;
   #endif

   /*--------------------------------------------------------*\
   ! Calculate the possible decomposition levels for all      !
   ! spatial and temporal dimensions.                         !
   \*--------------------------------------------------------*/
   levelsX  = log(info->nX) /log(2);
   levelsY  = log(info->nY) /log(2);
   levelsZ  = log(info->nZ) /log(2);
   levelsTS = log(info->nTS)/log(2);

   /*--------------------------------------------------------*\
   ! Check if the decomposition levels are specified for a    !
   ! valid spatial or temporal dimension. For all invalid and !
   ! unspecified (NULL) values the corresponding decomposi-   !
   ! tion level is set to 0. For all valid dimensions the     !
   ! number of decomposition levels is set accordingly.       !
   \*--------------------------------------------------------*/
   control->decompX  = (info->nX  >> 1) ? ((4 < levelsX)  ? 4 : levelsX)  : 0;
   control->decompY  = (info->nY  >> 1) ? ((4 < levelsY)  ? 4 : levelsY)  : 0;
   control->decompZ  = (info->nZ  >> 1) ? ((4 < levelsZ)  ? 4 : levelsZ)  : 0;
   control->decompTS = (info->nTS >> 1) ? ((4 < levelsTS) ? 4 : levelsTS) : 0;
  
   control->nDecomp   = MAX(control->decompX,
                        MAX(control->decompY,
                        MAX(control->decompZ, control->decompTS)));

   /*--------------------------------------------------------*\
   ! Initialize the lookup tables used to calculate the ener- !
   ! gy gain for the multi dimensional wavelet transforms.    !
   \*--------------------------------------------------------*/
   if(initialize_gain_lut()) 
   {
      bwc_kill_compression(field);
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! Calculate the appropriate quantization step size for the !
   ! given number of decomposition levels according to equa-  !
   ! tion (10.19) (epsilon = 6, mu = 16) from JPEG2000 by     !
   ! by David S. Taubman and Michael W. Marcellin (p.437).    !
   \*--------------------------------------------------------*/
   delta = (bwc_float)(1/(pow(2, 2 + PREC_BIT) * sqrt(get_dwt_energy_gain(field, 0, control->nDecomp))));

   for(control->qt_exponent = 0; delta < 1; ++control->qt_exponent, delta *= 2);
   control->qt_mantissa  = (uint16)floor(0.5f + ((delta - 1.0f) * (1 << 16)));

   if(control->qt_exponent > PREC_BIT)
   {
      control->qt_exponent = PREC_BIT;
      control->qt_mantissa = 0;
   }
   if(control->qt_mantissa >= 1 << 16)
   {
      if(control->qt_exponent == 0)
      {
         control->qt_exponent = 0;
         control->qt_mantissa = (1 << 16) - 1;
      }
      else
      {
         control->qt_exponent--;
         control->qt_mantissa = 0;
      }
   }

   return field;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_set_error_resilience(bwc_field *const field)                                     !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function sets the error resilience marker in the bwc_field structure if an error       !
!                resilient compression approach is to be employed.                                           !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
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
!                13.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bwc_set_error_resilience(bwc_field *const field)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);

   /*--------------------------------------------------------*\
   ! Save the global control structure to a temporary varia-  !
   ! ble to make the code more readable.                      !
   \*--------------------------------------------------------*/
   control = &field->control;

   /*--------------------------------------------------------*\
   ! Amend the codeblock style in the bwc_field structure     !
   ! according to the specified value.                        !
   \*--------------------------------------------------------*/
   control->error_resilience ^= 0x01;

   /*--------------------------------------------------------*\
   ! Record the setting of the error resilience marker in the !
   ! CSsgc variable.                                          !
   \*--------------------------------------------------------*/
   control->CSsgc ^= (0x01 << 0);
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_set_quantization_style(bwc_field *const field, bwc_quant_st quantization_style)  !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function amends the quantization style in the bwc_field structure according to the     !
!                specified value.                                                                            !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                quantization_style          bwc_quant_st          - Quantization style used during compres- !
!                                                                    sion.                                   !
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
!                15.03.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bwc_set_quantization_style(bwc_field *const field, bwc_quant_st quantization_style)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert((quantization_style == bwc_qt_derived) || (quantization_style == bwc_qt_none));

   /*--------------------------------------------------------*\
   ! Save the global control structure to a temporary varia-  !
   ! ble to make the code more readable.                      !
   \*--------------------------------------------------------*/
   control = &field->control;

   /*--------------------------------------------------------*\
   ! Amend the quantization style in the bwc_field structure  !
   ! according to the specified value.                        !
   \*--------------------------------------------------------*/
   control->quantization_style = quantization_style;

   /*--------------------------------------------------------*\
   ! Record the setting of the quantization style marker in   !
   ! the CSsgc variable.                                      !
   \*--------------------------------------------------------*/
   control->CSsgc |= (0x01 << 1);
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_set_quantization_step_size(bwc_field *const field, double delta)                 !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function amends the quantization step size in the bwc_field structure according to     !
!                the specified value.                                                                        !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                delta                       double                - Quantization step size used during      !
!                                                                    compression.                            !
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
!                16.04.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bwc_set_quantization_step_size(bwc_field *const field, double delta)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);

   /*--------------------------------------------------------*\
   ! Save the global control structure to a temporary varia-  !
   ! ble to make the code more readable.                      !
   \*--------------------------------------------------------*/
   control = &field->control;

   /*--------------------------------------------------------*\
   ! Check if the quantization step size lies within the ac-  !
   ! ceptable range.                                          !
   \*--------------------------------------------------------*/
   if((delta <= 0) || (delta >= 2))
   {
      fprintf(stderr, "o==========================================================o\n"\
                      "| WARNING: Invalid quantization step size                  |\n"\
                      "|                                                          |\n"\
                      "|          The quantization step size does not lie within  |\n"\
                      "|          the acceptable range of:                        |\n"\
                      "|                                                          |\n"\
                      "|                        0 < step size < 2                 |\n"\
                      "|                                                          |\n"\
                      "o==========================================================o\n");

      return;
   }

   /*--------------------------------------------------------*\
   ! Reevaluate the mantissa and exponent of the user defined !
   ! quantization step size.                                  !
   \*--------------------------------------------------------*/
   for(control->qt_exponent = 0; delta < 1; ++control->qt_exponent, delta *= 2);
   control->qt_mantissa  = (uint16)floor(0.5f + ((delta - 1.0f) * (1 << 16)));

   if(control->qt_exponent > PREC_BIT)
   {
      control->qt_exponent = PREC_BIT;
      control->qt_mantissa = 0;
   }
   if(control->qt_mantissa >= 1 << 16)
   {
      if(control->qt_exponent == 0)
      {
         control->qt_exponent = 0;
         control->qt_mantissa = (1 << 16) - 1;
      }
      else
      {
         control->qt_exponent--;
         control->qt_mantissa = 0;
      }
   }

   /*--------------------------------------------------------*\
   ! Record the setting of the quantization step size in the  !
   ! CSsgc variable.                                          !
   \*--------------------------------------------------------*/
   control->CSsgc |= (0x01 << 2);
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_set_progression(bwc_field *const field, bwc_prog_ord progression)                !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function amends the progression order in the bwc_field structure according to the      !
!                specified value.                                                                            !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                progression                 bwc_prog_ord          - Progression orders employed during com- !
!                                                                    pression (CPRL, LRCP, PCRL, RLCP,       !
!                                                                    RPCL).                                  !
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
!                15.03.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bwc_set_progression(bwc_field *const field, bwc_prog_ord progression)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_ctrl       *control;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert((progression == bwc_prog_CPRL) || (progression == bwc_prog_LRCP) ||
          (progression == bwc_prog_PCRL) || (progression == bwc_prog_RLCP) ||
          (progression == bwc_prog_RPCL));

   /*--------------------------------------------------------*\
   ! Save the global control structure to a temporary varia-  !
   ! ble to make the code more readable.                      !
   \*--------------------------------------------------------*/
   control = &field->control;

   /*--------------------------------------------------------*\
   ! Amend the progression order in the bwc_field structure   !
   ! according to the specified value.                        !
   \*--------------------------------------------------------*/
   control->progression        = progression;

   /*--------------------------------------------------------*\
   ! Record the setting of the progression order marker in    !
   ! the CSsgc variable.                                      !
   \*--------------------------------------------------------*/
   control->CSsgc |= (0x01 << 3);
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_set_kernels(bwc_field *const field, bwc_dwt_filter KernelX,                      !
!   --------------                                              bwc_dwt_filter KernelY,                      !
!                                                               bwc_dwt_filter KernelZ,                      !
!                                                               bwc_dwt_filter KernelTS)                     !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function amends the wavelet kernels in the bwc_field structure according to the        !
!                specified values.                                                                           !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                KernelX, KernelY, KernelZ   bwc_dwt_filter        - Wavelet kernels used for spatial        !
!                                                                    decomposition.                          !
!                                                                                                            !
!                KernelT                     bwc_dwt_filter        - Wavelet kernel used for temporal        !
!                                                                    decomposition.                          !
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
!                15.03.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bwc_set_kernels(bwc_field *const field, bwc_dwt_filter KernelX, bwc_dwt_filter KernelY, 
                                        bwc_dwt_filter KernelZ, bwc_dwt_filter KernelTS)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_ctrl       *control;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);

   /*--------------------------------------------------------*\
   ! Save the global control structure to a temporary varia-  !
   ! ble to make the code more readable.                      !
   \*--------------------------------------------------------*/
   control = &field->control;

   /*--------------------------------------------------------*\
   ! Amend the wavelet kernels in the bwc_field structure ac- !
   ! cording to the specified values. For all unspecified     !
   ! (NULL) values the corresponding wavelet kernel is set to !
   ! bwc_dwt_9_7/bwc_dwt_haar.                                !
   \*--------------------------------------------------------*/
   control->KernelX  = KernelX  ? KernelX  : bwc_dwt_9_7;
   control->KernelY  = KernelY  ? KernelY  : bwc_dwt_9_7;
   control->KernelZ  = KernelZ  ? KernelZ  : bwc_dwt_9_7;
   control->KernelTS = KernelTS ? KernelTS : bwc_dwt_haar;

   /*--------------------------------------------------------*\
   ! Record the setting of the wavelet kernel order markers   !
   ! in the CSsgc variable.                                   !
   \*--------------------------------------------------------*/
   control->CSsgc |= (0x01 << 4);
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_set_decomp(bwc_field *const field, uint8 decompX, uint8 decompY,                 !
!   --------------                                             uint8 decompZ, uint8 decompTS)                !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function amends the decomposition levels in the bwc_field structure according to the   !
!                specified values.                                                                           !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                decompX, decompY, decompZ   unsigned int(8 bit)   - Number of spatial wavelet decomposition !
!                                                                    levels used during compression.         !
!                                                                                                            !
!                decompTS                    unsigned int(8 bit)   - Number of temporal wavelet decomposi-   !
!                                                                    tion level used during compression.     !
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
!                14.03.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bwc_set_decomp(bwc_field *const field, uint8 decompX, uint8 decompY, uint8 decompZ, uint8 decompTS)
{
   /*-----------------------*\
   ! DEFINE FLOAT VARIABLES: !
   \*-----------------------*/
   double   delta;

   /*-----------------------*\
   ! DEFINE CHAR VARIABLES:  !
   \*-----------------------*/
   uint8    levelsX, levelsY, levelsZ, levelsTS;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;
   bwc_gl_inf        *info;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   
   /*--------------------------------------------------------*\
   ! Save the global control and info structure to temporary  !
   ! variables to make the code more readable.                !
   \*--------------------------------------------------------*/
   control = &field->control;
   info    =  field->info;

   /*--------------------------------------------------------*\
   ! Calculate the possible decomposition levels for all      !
   ! spatial and temporal dimensions.                         !
   \*--------------------------------------------------------*/
   levelsX  = log(info->nX) /log(2);
   levelsY  = log(info->nY) /log(2);
   levelsZ  = log(info->nZ) /log(2);
   levelsTS = log(info->nTS)/log(2);

   /*--------------------------------------------------------*\
   ! Check if the decomposition levels are specified for a    !
   ! valid spatial or temporal dimension. For all invalid and !
   ! unspecified (NULL) values the corresponding decomposi-   !
   ! tion level is set to 0. For all valid dimensions the     !
   ! number of decompositions is set accordingly.             !
   \*--------------------------------------------------------*/
   control->decompX  = (info->nX  >> 1) ? ((decompX  < levelsX)  ? decompX  : levelsX)  : 0;
   control->decompY  = (info->nY  >> 1) ? ((decompY  < levelsY)  ? decompY  : levelsY)  : 0;
   control->decompZ  = (info->nZ  >> 1) ? ((decompZ  < levelsZ)  ? decompZ  : levelsZ)  : 0;
   control->decompTS = (info->nTS >> 1) ? ((decompTS < levelsTS) ? decompTS : levelsTS) : 0;

   /*--------------------------------------------------------*\
   ! Calculate the overall number of decomposition levels.    !
   \*--------------------------------------------------------*/
   control->nDecomp = MAX(control->decompX, 
                      MAX(control->decompY, 
                      MAX(control->decompZ, control->decompTS)));

   /*--------------------------------------------------------*\
   ! Check if the decomposition levels have valid values.     !
   \*--------------------------------------------------------*/
   if((control->decompX > 63) || (control->decompY  > 63) ||
      (control->decompZ > 63) || (control->decompTS > 31))
   {
      fprintf(stderr, "o==========================================================o\n"\
                      "| WARNING: Invalid decomposition level value               |\n"\
                      "|                                                          |\n"\
                      "|          The maximum acceptable decomposition level is   |\n"\
                      "|          63 for all spatial and 31 for the temporal      |\n"\
                      "|          dimensions.                                     |\n"\
                      "|                                                          |\n"\
                      "o==========================================================o\n");

      /*--------------------------------------------------------*\
      ! Reset the decomposition levels to their standard values. !
      \*--------------------------------------------------------*/
      control->decompX  = (info->nX  >> 1) ? 4 : 0;
      control->decompY  = (info->nY  >> 1) ? 4 : 0;
      control->decompZ  = (info->nZ  >> 1) ? 4 : 0;
      control->decompTS = (info->nTS >> 1) ? 4 : 0;

      return;
   }

   /*--------------------------------------------------------*\
   ! Initialize the lookup tables used to calculate the ener- !
   ! gy gain for the multi dimensional wavelet transforms.    !
   \*--------------------------------------------------------*/
   if(initialize_gain_lut()) 
   {
      bwc_kill_compression(field);
      return;
   }

   /*--------------------------------------------------------*\
   ! Calculate the appropriate quantization step size for the !
   ! given number of decomposition levels according to equa-  !
   ! tion (10.19) (epsilon = 6, mu = 16) from JPEG2000 by     !
   ! by David S. Taubman and Michael W. Marcellin (p.437).    !
   \*--------------------------------------------------------*/
   delta = 1/(pow(2, 2 + PREC_BIT) * sqrt(get_dwt_energy_gain(field, 0, control->nDecomp)));

   for(control->qt_exponent = 0; delta < 1; ++control->qt_exponent, delta *= 2);
   control->qt_mantissa  = (uint16)floor(0.5f + ((delta - 1.0f) * (1 << 16)));

   if(control->qt_exponent > PREC_BIT)
   {
      control->qt_exponent = PREC_BIT;
      control->qt_mantissa = 0;
   }
   if(control->qt_mantissa >= 1 << 16)
   {
      if(control->qt_exponent == 0)
      {
         control->qt_exponent = 0;
         control->qt_mantissa = (1 << 16) - 1;
      }
      else
      {
         control->qt_exponent--;
         control->qt_mantissa = 0;
      }
   }

   /*--------------------------------------------------------*\
   ! Record the setting of the decomposition level variables  !
   ! in the CSsgc variable.                                   !
   \*--------------------------------------------------------*/
   control->CSsgc |= (0x01 << 5);
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_set_precincts(bwc_field *const field, uint8 pX, uint8 pY, uint8 pZ, uint8 pTS)   !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function amends the precinct size in the bwc_field structure according to the          !
!                specified values.                                                                           !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                px, py, pz, pTS             unsigned int(8 bit)   - Spatial and temporal precinct dimensions!
!                                                                    in log2 exponent format.                !
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
!                15.05.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bwc_set_precincts(bwc_field *const field, uint8 pX, uint8 pY, uint8 pZ, uint8 pTS)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_ctrl       *control;
   bwc_gl_inf        *info;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);

   /*--------------------------------------------------------*\
   ! Save the global control and info structure to temporary  !
   ! variables to make the code more readable.                !
   \*--------------------------------------------------------*/
   control = &field->control;
   info    =  field->info;

   /*--------------------------------------------------------*\
   ! Check if the precinct sizes are specified for a valid    !
   ! spatial or temporal dimension. For all invalid and un-   !
   ! specified (NULL) values the corresponding precinct size  !
   ! is set to 0. For all valid dimensions the precinct size  !
   ! is set accordingly.                                      !
   \*--------------------------------------------------------*/
   control->precSizeX  = (info->nX  >> 1) ? (pX  ? pX  : 15) : 0;
   control->precSizeY  = (info->nY  >> 1) ? (pY  ? pY  : 15) : 0;
   control->precSizeZ  = (info->nZ  >> 1) ? (pZ  ? pZ  : 15) : 0;
   control->precSizeTS = (info->nTS >> 1) ? (pTS ? pTS : 15) : 0;

   /*--------------------------------------------------------*\
   ! Check if the codeblock sizes have valid values.          !
   \*--------------------------------------------------------*/
   if(((control->precSizeX  < 1) && (info->nX   >> 1))  || 
      ((control->precSizeY  < 1) && (info->nY   >> 1))  || 
      ((control->precSizeZ  < 1) && (info->nZ   >> 1))  || 
      ((control->precSizeTS < 1) && (info->nTS  >> 1))  ||
      (control->precSizeX > 15) || (control->precSizeY > 15) || (control->precSizeZ > 15) || (control->precSizeTS > 15))
   {
      fprintf(stderr, "o==========================================================o\n"\
                      "| WARNING: Invalid precinct size                           |\n"\
                      "|                                                          |\n"\
                      "|          The maximum acceptable precinct size is 2^15,   |\n"\
                      "|          the smallest valid precinct size is 2^1.        |\n"\
                      "|                                                          |\n"\
                      "o==========================================================o\n");

      /*--------------------------------------------------------*\
      ! Reset the codeblock sizes to their standard values.      !
      \*--------------------------------------------------------*/
      control->precSizeX  = (info->nX  >> 1) ? 15 : 0;
      control->precSizeY  = (info->nY  >> 1) ? 15 : 0;
      control->precSizeZ  = (info->nZ  >> 1) ? 15 : 0;
      control->precSizeTS = (info->nTS >> 1) ? 15 : 0;

      return;
   }

   /*--------------------------------------------------------*\
   ! Record the setting of the precinct size variables in the !
   ! CSsgc variable.                                          !
   \*--------------------------------------------------------*/
   control->CSsgc |= (0x01 << 6);
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_set_codeblocks(bwc_field *const field, uint8 cbX, uint8 cbY,                     !
!   --------------                                                 uint8 cbZ, uint8 cbTS)                    !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function amends the codeblock size in the bwc_field structure according to the         !
!                specified values.                                                                           !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                cbx, cby, cbz, cbTS         unsigned int(8 bit)   - Spatial and temporal codeblock dimen-   !
!                                                                    sions in log2 exponent format.          !
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
!                14.03.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bwc_set_codeblocks(bwc_field *const field, uint8 cbX, uint8 cbY, uint8 cbZ, uint8 cbTS)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_ctrl       *control;
   bwc_gl_inf        *info;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);

   /*--------------------------------------------------------*\
   ! Save the global control and info structure to temporary  !
   ! variables to make the code more readable.                !
   \*--------------------------------------------------------*/
   control = &field->control;
   info    =  field->info;

   /*--------------------------------------------------------*\
   ! Check if the codeblock sizes are specified for a valid   !
   ! spatial or temporal dimension. For all invalid and un-   !
   ! specified (NULL) values the corresponding codeblock size !
   ! is set to 0. For all valid dimensions the codeblock size !
   ! is set accordingly.                                      !
   \*--------------------------------------------------------*/
   control->cbX  = (info->nX  >> 1) ? (cbX  ? cbX  : 5) : 0;
   control->cbY  = (info->nY  >> 1) ? (cbY  ? cbY  : 5) : 0;
   control->cbZ  = (info->nZ  >> 1) ? (cbZ  ? cbZ  : 5) : 0;
   control->cbTS = (info->nTS >> 1) ? (cbTS ? cbTS : 5) : 0;

   /*--------------------------------------------------------*\
   ! Check if the codeblock sizes have valid values.          !
   \*--------------------------------------------------------*/
   if((control->cbX > 10) || (control->cbY > 10) || (control->cbZ > 10) || (control->cbTS > 10)|| 
     ((control->cbX + control->cbY + control->cbZ + control->cbTS) < 4) || 
     ((control->cbX + control->cbY + control->cbZ + control->cbTS) > 20))
   {
      fprintf(stderr, "o==========================================================o\n"\
                      "| WARNING: Invalid codeblock size                          |\n"\
                      "|                                                          |\n"\
                      "|          The maximum acceptable codeblock size is 2^20   |\n"\
                      "|          with a maximum allowable number of datapoints   |\n"\
                      "|          in each dimension of 2^10. The smallest valid   |\n"\
                      "|          codeblock size is 2^4.                          |\n"\
                      "|                                                          |\n"\
                      "o==========================================================o\n");

      /*--------------------------------------------------------*\
      ! Reset the codeblock sizes to their standard values.      !
      \*--------------------------------------------------------*/
      control->cbX  = (info->nX  >> 1) ? 5 : 0;
      control->cbY  = (info->nY  >> 1) ? 5 : 0;
      control->cbZ  = (info->nZ  >> 1) ? 5 : 0;
      control->cbTS = (info->nTS >> 1) ? 5 : 0;

      return;
   }

   /*--------------------------------------------------------*\
   ! Record the setting of the codeblock size variables in    !
   ! the CSsgc variable.                                      !
   \*--------------------------------------------------------*/
   control->CSsgc |= (0x01 << 7);
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_set_qm(bwc_field *const field, uint8 Qm)                                         !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function amends the Q number formate range in the bwc_field structure according to the !
!                specified value.                                                                            !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                Qm                          unsigned int(8 bit)   - Q number formate range (m).             !
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
!                14.03.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bwc_set_qm(bwc_field *const field, uint8 Qm)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_ctrl       *control;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);

   /*--------------------------------------------------------*\
   ! Save the global control structure to a temporary varia-  !
   ! ble to make the code more readable.                      !
   \*--------------------------------------------------------*/
   control = &field->control;

   /*--------------------------------------------------------*\
   ! Check if the Q number formate range is valid and amend   !
   ! the bwc_field structure accordingly.                     !
   \*--------------------------------------------------------*/
   if((int8)(PREC_BIT - Qm) < 2)
   {
      fprintf(stderr, "o==========================================================o\n"\
                      "| WARNING: Invalid Q number formate range                  |\n"\
                      "|                                                          |\n"\
                      "|          The specified Q number formate range is larger  |\n");
      #ifdef BWC_SINGLE_PRECISION
         fprintf(stderr, "|          than the permitted 30 bits.                     |\n");
      #else
         fprintf(stderr, "|          than the permitted 62 bits.                     |\n");
      #endif
      fprintf(stderr, "|                                                          |\n"\
                      "o==========================================================o\n");
   }
   else
   {
      control->Qm = Qm;

      /*--------------------------------------------------------*\
      ! Record the setting of the Q number format range in the   !
      ! CSsgc variable.                                          !
      \*--------------------------------------------------------*/
      control->CSsgc |= (0x01 << 8);
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_set_tiles(bwc_field *const field, uint32 tilesX, uint32 tilesY, uint32 tilesZ,   !
!   --------------                                                           uint32 tilesTS, uchar instr)    !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function amends the tileSize and num_Tiles values in the bwc_field structure according !
!                to the specified values. The NUMBEROF and SIZEOF constants can be used to either specify    !
!                the tile sizes or the number of tiles in each spatial and temporal directions.              !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                tilesX, tilesY, tilesZ      unsigned int(32 bit)  - Variables defining the size of          !
!                                                                    a spatial tile.                         !
!                                                                                                            !
!                tilesTS                     unsigned int(32 bit)  - Variables defining the size of          !
!                                                                    a temporal tile.                        !
!                                                                                                            !
!                instr                       bwc_tile_instr        - Constants used to instruct the          !
!                                                                    bwc_set_tiles function.                 !
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
!                14.03.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bwc_set_tiles(bwc_field *const field, uint64 tilesX, uint64 tilesY, uint64 tilesZ, uint16 tilesTS, bwc_tile_instr instr)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   num_tiles_X, num_tiles_Y, num_tiles_Z;
   uint16   num_tiles_TS;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_gl_ctrl       *control;
   bwc_gl_inf        *info;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(instr == bwc_tile_sizeof || instr == bwc_tile_numbof);

   /*--------------------------------------------------------*\
   ! Save the global control and info structure to temporary  !
   ! variables to make the code more readable.                !
   \*--------------------------------------------------------*/
   control = &field->control;
   info    =  field->info;

   /*--------------------------------------------------------*\
   ! Check if the size of one tile or the overall number of   !
   ! tiles are defined.                                       !
   \*--------------------------------------------------------*/
   if(instr == bwc_tile_sizeof)
   {
      /*--------------------------------------------------------*\
      ! Check if the tile sizes have valid values.               !
      \*--------------------------------------------------------*/
      if((control->tileSizeX  < 16 && control->tileSizeX  > info->nX)|| 
         (control->tileSizeY  < 16 && control->tileSizeY  > info->nY)|| 
         (control->tileSizeZ  < 16 && control->tileSizeZ  > info->nZ)|| 
         (control->tileSizeTS < 16 && control->tileSizeTS > info->nTS))
      {
         fprintf(stderr,"o==========================================================o\n"\
                        "| WARNING: Invalid Tile Dimensions                         |\n"\
                        "|                                                          |\n"\
                        "|          One or more of the specified tile dimensions    |\n"\
                        "|          has a value that falls outside of its valid     |\n"\
                        "|          range. Please verify that all tile dimension    |\n"\
                        "|          are within the range of:                        |\n"\
                        "|                                                          |\n"\
                        "|               16 ≤ Tile_Size_Xi ≤ Grid_Points_Xi         |\n"\
                        "|                                                          |\n"\
                        "o==========================================================o\n");

         return;
      }

      /*--------------------------------------------------------*\
      ! Check if the tile dimensions are specified for a valid   !
      ! spatial or temporal dimension. All invalid and unspeci-  !
      ! fied (NULL) values are set to the maximum allowable tile !
      ! size.                                                    !
      \*--------------------------------------------------------*/
      control->tileSizeX  = (info->nX  >> 1) ? (tilesX  ? tilesX  : info->nX)  : info->nX;
      control->tileSizeY  = (info->nY  >> 1) ? (tilesY  ? tilesY  : info->nY)  : info->nY;
      control->tileSizeZ  = (info->nZ  >> 1) ? (tilesZ  ? tilesZ  : info->nZ)  : info->nZ;
      control->tileSizeTS = (info->nTS >> 1) ? (tilesTS ? tilesTS : info->nTS) : info->nTS;

      /*--------------------------------------------------------*\
      ! Calculate the number of tiles in all spatial and tempo-  !
      ! ral directions and the overall number of tiles.          !
      \*--------------------------------------------------------*/
      num_tiles_X     = (uint64)ceil(((float)info->nX / control->tileSizeX));
      num_tiles_Y     = (uint64)ceil(((float)info->nY / control->tileSizeY));
      num_tiles_Z     = (uint64)ceil(((float)info->nZ / control->tileSizeZ));
      num_tiles_TS    = (uint16)ceil(((float)info->nTS/ control->tileSizeTS));
      control->nTiles = num_tiles_X * num_tiles_Y * num_tiles_Z * num_tiles_TS;

      /*--------------------------------------------------------*\
      ! Check if the number of tiles exceeds its maximum allowa- !
      ! ble value.                                               !
      \*--------------------------------------------------------*/
      if(((double)num_tiles_X * num_tiles_Y * num_tiles_Z * num_tiles_TS) > 0xFFFFFFFFFFFFFFFF)
      {
         fprintf(stderr,"o==========================================================o\n"\
                        "| WARNING: Invalid Tile Dimensions                         |\n"\
                        "|                                                          |\n"\
                        "|          The number of tiles exceeds its maxmum allowa-  |\n"\
                        "|          ble value. Please adjust all tile dimension so  |\n"\
                        "|          that the number of tiles falls within the range |\n"\
                        "|          of:                                             |\n"\
                        "|                      Number_of_Tiles < 2^64              |\n"\
                        "|                                                          |\n"\
                        "o==========================================================o\n");

         /*--------------------------------------------------------*\
         ! Reset the tile sizes to their standard values.           !
         \*--------------------------------------------------------*/
         control->tileSizeX  = info->nX;
         control->tileSizeY  = info->nY;
         control->tileSizeZ  = info->nZ;
         control->tileSizeTS = info->nTS;

         return;
      }
   }
   else if(bwc_tile_numbof)
   {
      /*--------------------------------------------------------*\
      ! Check if the number of tiles exceeds its maximum allowa- !
      ! ble value.                                               !
      \*--------------------------------------------------------*/
      if(((double)tilesX * tilesY * tilesZ * tilesTS) > 0xFFFFFFFFFFFFFFFF)
      {
         fprintf(stderr,"o==========================================================o\n"\
                        "| WARNING: Invalid Number Of Tiles                         |\n"\
                        "|                                                          |\n"\
                        "|          The number of tiles exceeds its maxmum allowa-  |\n"\
                        "|          ble value of:                                   |\n"\
                        "|                                                          |\n"\
                        "|                      Number_of_Tiles < 2^64              |\n"\
                        "|                                                          |\n"\
                        "o==========================================================o\n");

         return;
      }

      /*--------------------------------------------------------*\
      ! Check if the number of tiles are specified for a valid   !
      ! spatial or temporal dimension. For all invalid and un-   !
      ! specified (NULL) values the corresponding tile size is   !
      ! set to its maximum allowable value. For all valid values !
      ! the tile sizes are calculated accordingly.               !
      \*--------------------------------------------------------*/
      control->tileSizeX  = (info->nX  >> 1) ? (tilesX  ? (uint64)ceil(((float)info->nX / tilesX))  : info->nX)  : info->nX;
      control->tileSizeY  = (info->nY  >> 1) ? (tilesY  ? (uint64)ceil(((float)info->nY / tilesY))  : info->nY)  : info->nY;
      control->tileSizeZ  = (info->nZ  >> 1) ? (tilesZ  ? (uint64)ceil(((float)info->nZ / tilesZ))  : info->nZ)  : info->nZ;
      control->tileSizeTS = (info->nTS >> 1) ? (tilesTS ? (uint16)ceil(((float)info->nTS/ tilesTS)) : info->nTS) : info->nTS;

      /*--------------------------------------------------------*\
      ! Check if the tile sizes have valid values.               !
      \*--------------------------------------------------------*/
      if((control->tileSizeX  < 16 && control->tileSizeX  > info->nX)|| 
         (control->tileSizeY  < 16 && control->tileSizeY  > info->nY)|| 
         (control->tileSizeZ  < 16 && control->tileSizeZ  > info->nZ)|| 
         (control->tileSizeTS < 16 && control->tileSizeTS > info->nTS))
      {
         fprintf(stderr,"o==========================================================o\n"\
                        "| WARNING: Invalid Number Of Tiles                         |\n"\
                        "|                                                          |\n"\
                        "|          One or more of the tile dimensions has a value  |\n"\
                        "|          that falls outside of its valid range. Please   |\n"\
                        "|          verify that the number of tiles for all dimen-  |\n"\
                        "|          sions are set so that the corresponding tile    |\n"\
                        "|          sizes fall within the range of:                 |\n"\
                        "|                                                          |\n"\
                        "|                16 ≤ Tile_Size_Xi ≤ Grid_Points_Xi        |\n"\
                        "|                                                          |\n"\
                        "o==========================================================o\n");

         /*--------------------------------------------------------*\
         ! Reset the tile sizes to their standard values.           !
         \*--------------------------------------------------------*/
         control->tileSizeX  = info->nX;
         control->tileSizeY  = info->nY;
         control->tileSizeZ  = info->nZ;
         control->tileSizeTS = info->nTS;

         return;
      }
      /*--------------------------------------------------------*\
      ! Reevaluate the number of tiles in all spatial and tempo- !
      ! ral directions and the overall number of tiles.          !
      \*--------------------------------------------------------*/
      num_tiles_X     = (uint64)ceil(((float)info->nX / control->tileSizeX));
      num_tiles_Y     = (uint64)ceil(((float)info->nY / control->tileSizeY));
      num_tiles_Z     = (uint64)ceil(((float)info->nZ / control->tileSizeZ));
      num_tiles_TS    = (uint16)ceil(((float)info->nTS/ control->tileSizeTS));
      control->nTiles = num_tiles_X * num_tiles_Y * num_tiles_Z * num_tiles_TS;
   }

   /*--------------------------------------------------------*\
   ! Record the setting of the tile size variables in the     !
   ! CSsgc variable.                                          !
   \*--------------------------------------------------------*/
   control->CSsgc |= (0x01 << 9);
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_set_nThreads(bwc_field *const field, uint16 nThreads)                            !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to indicate the maximum number of threads used during                 !
!                (de)compression.                                                                            !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                nThreads                    unsigned int(8 bit)   - Number of OpenMP threads.               !
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
!                07.08.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
#if defined(_OPENMP)
   void
   bwc_set_nThreads(bwc_field *const field, uint8 nThreads)
   {
      /*-----------------------*\
      ! DEFINE STRUCTS:         !
      \*-----------------------*/ 
      bwc_gl_ctrl       *control;

      /*-----------------------*\
      ! DEFINE ASSERTIONS:      !
      \*-----------------------*/
      assert(field);

      /*--------------------------------------------------------*\
      ! Save the global control and to a temporary variable to   !
      ! make the code more readable.                             !
      \*--------------------------------------------------------*/
      control = &field->control;

      /*--------------------------------------------------------*\
      ! Amend the number of OMP threads the bwc_field structure. !
      \*--------------------------------------------------------*/
      control->nThreads = nThreads;
   }
#endif

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void bwc_create_compression(bwc_field *field, char *rate_control)                         !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function creates the field structure used to compress a floating point array defined   !
!                by the bwc_initialize function. For a compression run, the rate_control and instr arguments !
!                need to be passed to the function to properly set up the lossy compression stage. Here, the !
!                instr parameter defines whether rate control is defined by a BITRATE - a floating point val-!
!                ue defining the average number of bits per datapoint - and ACCURACY - an integer value de-  !
!                fining the exponent of the maximum allowable error (i.e. 15 for err = 1e-15).               !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                rate_control                char*                 - Variable defining the bitrate.          !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                                                                                           !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                15.03.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                05.12.2019  Patrick vogler     B87E7E4     V 0.1.0     streamlined function arguments       !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
bwc_create_compression(bwc_field *field, char *rate_control)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int8     l;
   size_t   size;

   /*-----------------------*\
   ! DEFINE FLOAT VARIABLES: !
   \*-----------------------*/
   float    buf_f;

   /*-----------------------*\
   ! DEFINE CHAR VARIABLES:  !
   \*-----------------------*/
   char    *token, *ptr;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);

   /*--------------------------------------------------------*\
   ! Save the global control structure to a temporary varia-  !
   ! ble to make the code more readable.                      !
   \*--------------------------------------------------------*/
   control = &field->control;
   
   /*--------------------------------------------------------*\
   ! Allocate the array used to hold the quality layer bit-   !
   ! rates with an initial size of 10.                        !
   \*--------------------------------------------------------*/
   size              = 10;
   control->bitrate  = calloc(size, sizeof(float));
   if(!control->bitrate) 
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Read the two additional arguments defining the rate con- !
   ! trol and accuracy parameters.                            !
   \*--------------------------------------------------------*/
   if(rate_control == NULL)
   {
      control->bitrate[0] = PREC_BIT + 1;
      control->nLayers    = 1;
   }
   else
   {
      token = strtok_r(rate_control, "-, ", &ptr);

      if(token)
      {
         for(control->nLayers = 0; token; token = strtok_r(NULL, "-, ", &ptr))
         {
            buf_f = strtof(token, NULL);

            if(buf_f <= 0 || buf_f > 64)
            {
               fprintf(stderr, "o##########################################################o\n"\
                              "|   ERROR: Invalid bitrate value: %-25f|\n"\
                              "|                                                          |\n"\
                              "|          The specified bitrate is not within its valid   |\n"\
                              "|          range of:                                       |\n"\
                              "|                                                          |\n"\
                              "|                        0 < Bitrate ≤ 64                  |\n"\
                              "|                                                          |\n"\
                              "o##########################################################o\n", buf_f);
               return 1;
            }

            if(((int8)size - control->nLayers) == 0)
            {
               size             += 10;
               control->bitrate  = realloc(control->bitrate, size * sizeof(float));
            }

            for(l = control->nLayers; l >= 0; --l)
            {
               if(l && buf_f < control->bitrate[l - 1])
               {
                  control->bitrate[l] = control->bitrate[l - 1];
               }
               else if (l && fabs(buf_f - control->bitrate[l - 1]) < 0.01f)
               {
                  for(; l < control->nLayers; ++l)
                  {
                     control->bitrate[l] = control->bitrate[l + 1];
                  }
                  break;
               }
               else
               {
                  control->bitrate[l] = buf_f;
                  control->nLayers++;
                  break;
               }
            }
         }
      }
      else
      {
         control->bitrate[0] = PREC_BIT + 1;
         control->nLayers    = 1;
      }
   }

   control->bitrate = realloc(control->bitrate, control->nLayers * sizeof(float));

   /*--------------------------------------------------------*\
   ! Create the field structure used to compress a floating   !
   ! point array defined by the bwc_initialize function.      !
   \*--------------------------------------------------------*/
   if(create_field(field))
   {
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Create the main header for the compressed codestream and !
   ! save the memory handle in the field structure.           !
   \*--------------------------------------------------------*/
   if(assemble_main_header(field))
   {
      return 1;
   }

   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uchar bwc_compress(bwc_field *const field, bwc_float *const data)                         !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                Description needed.                                                                         !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
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
!                15.03.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
bwc_compress(bwc_field *const field, bwc_data *const data)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   buff_size  = 0;
   uint64   i;
   uint16   p;

   /*-----------------------*\
   ! DEFINE FLOAT VARIABLES: !
   \*-----------------------*/
   double   start, end;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;
   bwc_gl_inf        *info;
   bwc_tile          *tile;
   bwc_parameter     *parameter;
   bwc_sample        *working_buffer;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(data);

   /*--------------------------------------------------------*\
   ! Initialize the compression time measurement.             !
   \*--------------------------------------------------------*/
   #if defined (_OPENMP)
      field->meter.time.ttl = omp_get_wtime();
   #else
      field->meter.time.ttl = (double)clock();
   #endif

   /*--------------------------------------------------------*\
   ! Save the global control and info structure to temporary  !
   ! variables to make the code more readable.                !
   \*--------------------------------------------------------*/
   control = &field->control;
   info    =  field->info;

   /*--------------------------------------------------------*\
   ! Check if any auxiliary information has been supplied by  !
   ! the function caller and append the header accordingly.   !
   \*--------------------------------------------------------*/
   if(data->codestream.aux && header_append_aux(field, data->codestream.aux))
   {
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Check if any commentary information has been supplied by !
   ! the function caller and append the header accordingly.   !
   \*--------------------------------------------------------*/
   if(data->codestream.com && header_append_com(field, data->codestream.com))
   {
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Set the number of Open MP threads for the current run.   !
   \*--------------------------------------------------------*/
   #if defined (_OPENMP)
      omp_set_num_threads(control->nThreads);
   #endif

   /*--------------------------------------------------------*\
   ! Evaluate the working buffer size and allocate it accord- !
   ! ingly.                                                   !
   \*--------------------------------------------------------*/
   buff_size = control->tileSizeX * control->tileSizeY *
               control->tileSizeZ * control->tileSizeTS;

   working_buffer  = calloc(buff_size, sizeof(bwc_sample));
   if(!working_buffer)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Walk through the tile structure and compress the speci-  !
   ! fied data set.                                           !
   \*--------------------------------------------------------*/
   for(i = 0; i < control->nTiles; ++i)
   {
      /*--------------------------------------------------------*\
      ! Save the tile structure in a temporary variable to make  !
      ! the code more readable.                                  !
      \*--------------------------------------------------------*/
      tile = &field->tile[i];

      for(p = 0; p < info->nPar; ++p)
      {
         /*--------------------------------------------------------*\
         ! Save the parameter structure in a temporary variable to  !
         ! make the code more readable.                             !
         \*--------------------------------------------------------*/
         parameter     = &tile->parameter[p];

         /*--------------------------------------------------------*\
         ! Fill the working buffer with the flow field data for the !
         ! current tile parameter.                                  !
         \*--------------------------------------------------------*/
         #if defined (_OPENMP)
            start = omp_get_wtime();
         #else
            start = (double)clock();
         #endif
         fill_buffer(field, tile, parameter, working_buffer, data, p);
         #if defined (_OPENMP)
            end = omp_get_wtime();
            field->meter.time.cpy += end - start;
         #else
            end = (double)clock();
            field->meter.time.cpy += (end - start)/CLOCKS_PER_SEC;
         #endif

         /*--------------------------------------------------------*\
         ! Normalize the working buffer to a range of [-1,1) and    !
         ! scale it to the dynamic range specified by the Qm param- !
         ! eter.                                                    !
         \*--------------------------------------------------------*/
         #if defined (_OPENMP)
            start = omp_get_wtime();
         #else
            start = (double)clock();
         #endif
         normalize_param(field, parameter);
         #if defined (_OPENMP)
            end = omp_get_wtime();
            field->meter.time.nrm += end - start;
         #else
            end = (double)clock();
            field->meter.time.nrm += (end - start)/CLOCKS_PER_SEC;
         #endif

         /*--------------------------------------------------------*\
         ! Perform the forward discrete wavelet transform.          !
         \*--------------------------------------------------------*/
         #if defined (_OPENMP)
            start = omp_get_wtime();
         #else
            start = (double)clock();
         #endif
         if(forward_discrete_wavelet_transform(field, parameter))
         {
            free(working_buffer);
            return 1;
         }
         #if defined (_OPENMP)
            end = omp_get_wtime();
            field->meter.time.wav += end - start;
         #else
            end = (double)clock();
            field->meter.time.wav += (end - start)/CLOCKS_PER_SEC;
         #endif

         /*--------------------------------------------------------*\
         ! Tier1 encode the current tile parameter.                 !
         \*--------------------------------------------------------*/
         #if defined (_OPENMP)
            start = omp_get_wtime();
         #else
            start = (double)clock();
         #endif
         if(t1_encode(field, tile, parameter))
         {
            free(working_buffer);
            return 1;
         }
         #if defined (_OPENMP)
            end = omp_get_wtime();
            field->meter.time.ent += end - start;
         #else
            end = (double)clock();
            field->meter.time.ent += (end - start)/CLOCKS_PER_SEC;
         #endif

         /*--------------------------------------------------------*\
         ! Reset the working buffer memory block.                   !
         \*--------------------------------------------------------*/
         memset(working_buffer, 0, buff_size * sizeof(bwc_sample));
         parameter->data = NULL;
      }
      /*--------------------------------------------------------*\
      ! Tier2 encode the current tile.                           !
      \*--------------------------------------------------------*/
      #if defined (_OPENMP)
         start = omp_get_wtime();
      #else
         start = (double)clock();
      #endif
      if(t2_encode(field, tile))
      {
         free(working_buffer);
         return 1;
      }
      #if defined (_OPENMP)
         end = omp_get_wtime();
         field->meter.time.ent += end - start;
      #else
         end = (double)clock();
         field->meter.time.ent += (end - start)/CLOCKS_PER_SEC;
      #endif
   }
   /*--------------------------------------------------------*\
   ! Assemble compressed codestream.                          !
   \*--------------------------------------------------------*/
   #if defined (_OPENMP)
      start = omp_get_wtime();
   #else
      start = (double)clock();
   #endif
   data->codestream.data = assemble_codestream(field);
   if(!data->codestream.data)
   {
      free(working_buffer);
      return 1;
   }
   #if defined (_OPENMP)
      end = omp_get_wtime();
      field->meter.time.ass += end - start;
   #else
      end = (double)clock();
      field->meter.time.ass += (end - start)/CLOCKS_PER_SEC;
   #endif

   /*--------------------------------------------------------*\
   ! Free the working buffer.                                 !
   \*--------------------------------------------------------*/
   free(working_buffer);

   /*--------------------------------------------------------*\
   ! Calculate the compression time.                          !
   \*--------------------------------------------------------*/
   #if defined (_OPENMP)
      field->meter.time.ttl = omp_get_wtime() - field->meter.time.ttl;
   #else
      field->meter.time.ttl = ((double)clock() - field->meter.time.ttl)/CLOCKS_PER_SEC;
   #endif

   /*--------------------------------------------------------*\
   ! Return to the function caller.                           !
   \*--------------------------------------------------------*/
   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uchar bwc_create_compression(bwc_field *field_ptr, char *rate_control, uchar instr)       !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function parses the supplied bwc codestream and sets up the field structure used to    !
!                decompress the numerical dataset.                                                           !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                rate_control (opt)          char*                 - Variable defining the bitrate/accuracy. !
!                                                                                                            !
!                instr (opt)                 bwc_rate_instr        - Constants used to instruct the rate     !
!                                                                    control information.                    !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                                                                                           !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                15.03.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
bwc_field *
bwc_create_decompression(bwc_data *const data, uint8 layer)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_field         *field;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(data);

   /*--------------------------------------------------------*\
   ! Parse the codestream and setup the field codestream.     !
   \*--------------------------------------------------------*/
   field  = parse_codestream(data, layer);
   if(!field)
   {
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! If successful, return the field structure to the func-   !
   ! tion caller.                                             !
   \*--------------------------------------------------------*/
   return field;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uchar bwc_create_compression(bwc_field *field, float rate_control, uchar instr)           !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function creates the field structure used to compress a floating point array defined   !
!                by the bwc_initialize function at a prescribed bitrate or accuracy. In this context, the    !
!                bitrate is a floating point value defining the average number of bits per datapoint and     !
!                the accuracy is an integer value defining the exponent of the maximum allowable error       !
!                (i.e. 15 for err = 1e-15).                                                                  !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                rate_control                float                 - Variable defining the bitrate/accuracy. !
!                                                                                                            !
!                instr                       unsigned char         - Constants used to instruct the rate     !
!                                                                    control information.                    !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                -                                                                                           !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                15.03.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
bwc_decompress(bwc_field *const field, bwc_data *const data)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   buff_size;
   uint64   single_size, double_size;
   uint64   i;
   uint16   p;

   /*-----------------------*\
   ! DEFINE FLOAT VARIABLES: !
   \*-----------------------*/
   double   start, end;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;
   bwc_gl_inf        *info;
   bwc_tile          *tile;
   bwc_parameter     *parameter;
   bwc_sample        *working_buffer;
   bwc_cmd_opts_ll   *param;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(data);

   /*--------------------------------------------------------*\
   ! Initialize the decompression time measurement.           !
   \*--------------------------------------------------------*/
   #if defined (_OPENMP)
      field->meter.time.ttl = omp_get_wtime();
   #else
      field->meter.time.ttl = (double)clock();
   #endif
   
   /*--------------------------------------------------------*\
   ! Save the global control and info structure to temporary  !
   ! variables to make the code more readable.                !
   \*--------------------------------------------------------*/
   control = &field->control;
   info    =  field->info;

   /*--------------------------------------------------------*\
   ! Set the number of Open MP threads for the current run.   !
   \*--------------------------------------------------------*/
   #if defined (_OPENMP)
      omp_set_num_threads(control->nThreads);
   #endif

   /*--------------------------------------------------------*\
   ! Calculate the field size after subsampling and allocate  !
   ! the field memory blocks.                                 !
   \*--------------------------------------------------------*/
   if(data->info.parameter)
   {
      param       = data->info.parameter->root;
      double_size =
      single_size = 0;

      while(param != NULL)
      {
         if(param->precision == 8)
         {
            double_size += param->size;
         }
         else if (param->precision == 4)
         {
            single_size += param->size;
         }

         param = param -> next;
      }
   }

   data->field.d = calloc(double_size, sizeof(double));
   data->field.f = calloc(single_size, sizeof(float));
   if(!data->field.d || !data->field.f)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Evaluate the working buffer size and allocate it accord- !
   ! ingly.                                                   !
   \*--------------------------------------------------------*/
   buff_size = control->tileSizeX * control->tileSizeY *
               control->tileSizeZ * control->tileSizeTS;

   working_buffer  = calloc(buff_size, sizeof(bwc_sample));
   if(!working_buffer)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Walk through the tile structure and decompress the speci-!
   ! fied data set.                                           !
   \*--------------------------------------------------------*/
   for(i = 0; i < control->nTiles; ++i)
   {
      /*--------------------------------------------------------*\
      ! Save the tile structure in a temporary variable to make  !
      ! the code more readable.                                  !
      \*--------------------------------------------------------*/
      tile          = &field->tile[i];

      for(p = 0; p < info->nPar; ++p)
      {
         /*--------------------------------------------------------*\
         ! Save the parameter structure in a temporary variable to  !
         ! make the code more readable.                             !
         \*--------------------------------------------------------*/
         parameter     = &tile->parameter[p];

         /*--------------------------------------------------------*\
         ! Associate the working buffer with the data pointer in    !
         ! the bwc_parameter structure.                             !
         \*--------------------------------------------------------*/
         parameter->data = working_buffer;

         /*--------------------------------------------------------*\
         ! Tier1 decode the current tile parameter.                 !
         \*--------------------------------------------------------*/
         #if defined (_OPENMP)
            start = omp_get_wtime();
         #else
            start = (double)clock();
         #endif
         if(t1_decode(field, tile, parameter))
         {
            free(working_buffer);
            return 1;
         }
         #if defined (_OPENMP)
            end = omp_get_wtime();
            field->meter.time.ent += end - start;
         #else
            end = (double)clock();
            field->meter.time.ent += (end - start)/CLOCKS_PER_SEC;
         #endif

         /*--------------------------------------------------------*\
         ! Perform the inverse discrete wavelet transform.          !
         \*--------------------------------------------------------*/
         #if defined (_OPENMP)
            start = omp_get_wtime();
         #else
            start = (double)clock();
         #endif
         if(inverse_discrete_wavelet_transform(field, parameter))
         {
            free(working_buffer);
            return 1;
         }
         #if defined (_OPENMP)
            end = omp_get_wtime();
            field->meter.time.wav += end - start;
         #else
            end = (double)clock();
            field->meter.time.wav += (end - start)/CLOCKS_PER_SEC;
         #endif

         /*--------------------------------------------------------*\
         ! Denormalize the working buffer scale it to the original  !
         ! dynamic range specified by the Qm parameter.             !
         \*--------------------------------------------------------*/
         #if defined (_OPENMP)
            start = omp_get_wtime();
         #else
            start = (double)clock();
         #endif
         denormalize_param(field, parameter);
         #if defined (_OPENMP)
            end = omp_get_wtime();
            field->meter.time.nrm += end - start;
         #else
            end = (double)clock();
            field->meter.time.nrm += (end - start)/CLOCKS_PER_SEC;
         #endif

         /*--------------------------------------------------------*\
         ! Flush the working buffer to the appropriate memory block !
         ! in the flow field data structure for the current tile pa-!
         ! rameter.                                                 !
         \*--------------------------------------------------------*/
         #if defined (_OPENMP)
            start = omp_get_wtime();
         #else
            start = (double)clock();
         #endif
         flush_buffer(field, tile, parameter, working_buffer, data, p);
         #if defined (_OPENMP)
            end = omp_get_wtime();
            field->meter.time.cpy += end - start;
         #else
            end = (double)clock();
            field->meter.time.cpy += (end - start)/CLOCKS_PER_SEC;
         #endif

         /*--------------------------------------------------------*\
         ! Reset the working buffer memory block.                   !
         \*--------------------------------------------------------*/
         memset(working_buffer, 0, buff_size * sizeof(bwc_sample));
         parameter->data = NULL;
      }
   }

   /*--------------------------------------------------------*\
   ! Free the working buffer.                                 !
   \*--------------------------------------------------------*/
   free(working_buffer);

   /*--------------------------------------------------------*\
   ! Calculate the decompression time.                        !
   \*--------------------------------------------------------*/
   #if defined (_OPENMP)
      field->meter.time.ttl = omp_get_wtime() - field->meter.time.ttl;
   #else
      field->meter.time.ttl = ((double)clock() - field->meter.time.ttl)/CLOCKS_PER_SEC;
   #endif

   return 0;
}