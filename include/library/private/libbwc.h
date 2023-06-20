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
||                                                           Version 0.1.1                                                          ||
||                                                                                                                                  ||
||      File:  bwc.h                                                                                                                ||
||      -----                                                                                                                       ||
||                                                                                                                                  ||
||      DESCRIPTION:                                                                                                                ||
||      ------------                                                                                                                ||
||      This header defines DESCRIPTION NEEDED.                                                                                     ||
||                                                                                                                                  ||
||      STRUCTS:                                                                                                                    ||
||      --------                                                                                                                    ||
||                                                                                                                                  ||
||      PUBLIC FUNCTIONS:                                                                                                           ||
||      -----------------                                                                                                           ||
||      - bwc_kill_compression                                                                                                      ||
||      - bwc_add_param                                                                                                             ||
||      - bwc_set_quantization_style                                                                                                ||
||      - bwc_set_progression                                                                                                       ||
||      - bwc_set_kernels                                                                                                           ||
||      - bwc_set_levels                                                                                                            ||
||      - bwc_set_codeblocks                                                                                                        ||
||      - bwc_set_qm                                                                                                                ||
||      - bwc_set_tiles                                                                                                             ||
||      - bwc_initialize_compression                                                                                                ||
||      - bwc_create_compression                                                                                                    ||
||                                                                                                                                  ||
||      DEVELOPMENT HISTORY:                                                                                                        ||
||      --------------------                                                                                                        ||
||                                                                                                                                  ||
||                            Date        Author             Change Id   Release     Description Of Change                          ||
||                            ----        ------             ---------   -------     ---------------------                          ||
||                            -           Patrick Vogler     B87D120     V 0.1.0     header file created                            ||
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
#ifndef BWC_H
#define BWC_H
   /************************************************************************************************************\
   ||                                      _ _  _ ____ _    _  _ ___  ____                                     ||
   ||                                      | |\ | |    |    |  | |  \ |___                                     ||
   ||                                      | | \| |___ |___ |__| |__/ |___                                     ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   #include "bitstream.h"
   #include "codestream.h"
   #include "constants.h"
   #include "macros.h"
   #include "mq_types.h"
   #include "types.h"

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
   \*----------------------------------------------------------------------------------------------------------*/
   bwc_data*
   bwc_initialize_data(double* field, uint64 const nX, uint64 const nY, uint64 const nZ, uint16 const nTS, uint8 const nPar, char *file_extension);

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
   \*----------------------------------------------------------------------------------------------------------*/
   void
   bwc_add_param(bwc_data* data, char *name, uint16 sample, uchar dim, uint8 precision);

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
   \*----------------------------------------------------------------------------------------------------------*/
   void
   bwc_get_data(bwc_data* data, uchar* buffer, uint64 size);

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
   \*----------------------------------------------------------------------------------------------------------*/
   void
   bwc_free_data(bwc_data* data);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: uchar create_field(bwc_field *const field)                                                !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function creates the field structure used to (de)compress a floating point array de-   !
   !                fined by the bwc_initialize function.                                                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   uchar
   create_field(bwc_field *const field);

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
   \*----------------------------------------------------------------------------------------------------------*/
   void
   bwc_kill_compression(bwc_field *const field);

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
   \*----------------------------------------------------------------------------------------------------------*/
   bwc_field*
   bwc_initialize_field(bwc_data *const data);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void bwc_set_error_resilience(bwc_field *const field)                                     !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function sets the error resilience marker in the bwc_field structure if an error       !
   !                resilient compression approach is to be employed.                                           !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   void
   bwc_set_error_resilience(bwc_field *const field);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void bwc_set_quantization_style(bwc_field *const field, bwc_quant_st quantization_style)  !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function amends the quantization style in the bwc_field structure according to the     !
   !                specified value.                                                                            !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   void
   bwc_set_quantization_style(bwc_field *const field, bwc_quant_st quantization_style);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void bwc_set_quantization_step_size(bwc_field *const field, double delta)                 !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function amends the quantization step size in the bwc_field structure according to     !
   !                the specified value.                                                                        !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   void
   bwc_set_quantization_step_size(bwc_field *const field, double delta);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void bwc_set_progression(bwc_field *const field, bwc_prog_ord progression)                !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function amends the progression order in the bwc_field structure according to the      !
   !                specified value.                                                                            !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   void
   bwc_set_progression(bwc_field *const field, bwc_prog_ord progression);

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
   \*----------------------------------------------------------------------------------------------------------*/
   void
   bwc_set_kernels(bwc_field *const field, bwc_dwt_filter KernelX, bwc_dwt_filter KernelY, 
                                          bwc_dwt_filter KernelZ, bwc_dwt_filter KernelTS);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void bwc_set_decomp(bwc_field *const field, uint8 decompX, uint8 decompY,                 !
   !   --------------                                             uint8 decompZ, uint8 decompTS)                !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function amends the decomposition levels in the bwc_field structure according to the   !
   !                specified values.                                                                           !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   void
   bwc_set_decomp(bwc_field *const field, uint8 decompX, uint8 decompY, uint8 decompZ, uint8 decompTS);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void bwc_set_precincts(bwc_field *const field, uint8 pX, uint8 pY, uint8 pZ, uint8 pTS)   !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function amends the precinct size in the bwc_field structure according to the          !
   !                specified values.                                                                           !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   void
   bwc_set_precincts(bwc_field *const field, uint8 pX, uint8 pY, uint8 pZ, uint8 pTS);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void bwc_set_codeblocks(bwc_field *const field, uint8 cbX, uint8 cbY,                     !
   !   --------------                                                 uint8 cbZ, uint8 cbTS)                    !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function amends the codeblock size in the bwc_field structure according to the         !
   !                specified values.                                                                           !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   void
   bwc_set_codeblocks(bwc_field *const field, uint8 cbX, uint8 cbY, uint8 cbZ, uint8 cbTS);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void bwc_set_qm(bwc_field *const field, uint8 Qm)                                         !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function amends the Q number formate range in the bwc_field structure according to the !
   !                specified value.                                                                            !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   void
   bwc_set_qm(bwc_field *const field, uint8 Qm);

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
   \*----------------------------------------------------------------------------------------------------------*/
   void
   bwc_set_tiles(bwc_field *const field, uint64 tilesX, uint64 tilesY, uint64 tilesZ, uint16 tilesTS, bwc_tile_instr instr);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void bwc_set_nThreads(bwc_field *const field, uint16 nThreads)                            !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function is used to indicate the maximum number of threads used during                 !
   !                (de)compression.                                                                            !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   #if defined(_OPENMP)
      void
      bwc_set_nThreads(bwc_field *const field, uint8 nThreads);
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
   \*----------------------------------------------------------------------------------------------------------*/
   uchar
   bwc_create_compression(bwc_field *field, char *rate_control);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: uchar bwc_compress(bwc_field *const field, bwc_float *const data)                         !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                Description needed.                                                                         !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   uchar
   bwc_compress(bwc_field *const field, bwc_data *const data);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void bwc_create_compression(bwc_field **field_ptr, char *rate_control, uchar instr)       !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function parses the supplied bwc codestream and sets up the field structure used to    !
   !                decompress the numerical dataset.                                                           !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   bwc_field *
   bwc_create_decompression(bwc_data *const data, uint8 layer);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void bwc_create_compression(bwc_field **field, float rate_control, uchar instr)           !
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
   \*----------------------------------------------------------------------------------------------------------*/
   uchar
   bwc_decompress(bwc_field *const field, bwc_data *const data);
#endif