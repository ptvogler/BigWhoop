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
||      FILE NAME:   eas3.c                                                                                                         ||
||                                                                                                                                  ||
||                                                                                                                                  ||
||      DESCRIPTION:                                                                                                                ||
||      ------------                                                                                                                ||
||                         This file defines simple read and write functions used to access conforming eas3 datasets.               ||
||                                                                                                                                  ||
||      FILE REFERENCES:                                                                                                            ||
||      ----------------                                                                                                            ||
||                                                                                                                                  ||
||                         Name              I/O             Description                                                            ||
||                         ----              ---             -----------                                                            ||
||                         fp                input/output  - Numerical dataset stored in the EAS3 data format.                      ||
||                                                                                                                                  ||
||                                                                                                                                  ||
||      PUBLIC FUNCTIONS:                                                                                                           ||
||      -----------------                                                                                                           ||
||      - read_eas3                                                                                                                 ||
||      - write_eas3                                                                                                                ||
||                                                                                                                                  ||
||      DEVELOPMENT HISTORY:                                                                                                        ||
||      --------------------                                                                                                        ||
||                                                                                                                                  ||
||                            Date        Author             Change Id   Release     Description Of Change                          ||
||                            ----        ------             ---------   -------     ---------------------                          ||
||                            20.6.2018   Patrick Vogler     B87D120     V 0.1.0     source file created                            ||
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
#include <bwc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eas3.h"
#include "bwccmdl.h"

/************************************************************************************************************\
||                ___  ____ _ _  _ ____ ___ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                ||
||                |__] |__/ | |  | |__|  |  |___    |___ |  | |\ | |     |  | |  | |\ | [__                 ||
||                |    |  \ |  \/  |  |  |  |___    |    |__| | \| |___  |  | |__| | \| ___]                ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                    This function converts the endianess of half, single or double precision values.        !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                    Variable                 Type                   Description                             !
!                    --------                 ----                   -----------                             !
!                    value                    void*                - Memory address of the parame-           !
!                                                                    ter to be converted.                    !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                    Type                                            Description                             !
!                    ----                                            -----------                             !
!                    -                                               -                                       !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                    Date        Author             Change Id   Release     Description                      !
!                    ----        ------             ---------   -------     -----------                      !
!                    30.04.2019  Patrick Vogler     B87D120     V 0.1.0     function created                 !
!                    21.11.2019  Patrick Vogler     B87E7E4     V 0.1.0     functionality expanded           !
!                                                                           to 32 bit integers               !
!                    21.11.2019  Patrick Vogler     B87E7E4     V 0.1.0     functionality expanded           !
!                                                                           to 16 bit integers               !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
endian_conversion(void          *value,
                  uint8_t const  accuracy)
{
  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(value);

  switch(accuracy)
    {
      case 2:
        {
          uint16_t *tmp  = (uint16_t*)value;

          *tmp           = (uint16_t)( *tmp << 8) | 
                           (uint16_t)( *tmp >> 8);
          break;
        }

      case 4:
        {
          uint32_t *tmp  = (uint32_t*)value;

          *tmp           = (uint32_t)((*tmp << 8) & 0xFF00FF00) |
                           (uint32_t)((*tmp >> 8) & 0x00FF00FF);

          *tmp           = (uint32_t)( *tmp << 16)              | 
                           (uint32_t)( *tmp >> 16);
          break;
        }

      case 8:
        {
          uint64_t *tmp  = (uint64_t*)value;

          *tmp           = (uint64_t)((*tmp << 8)  & 0xFF00FF00FF00FF00ULL) | 
                           (uint64_t)((*tmp >> 8)  & 0x00FF00FF00FF00FFULL);

          *tmp           = (uint64_t)((*tmp << 16) & 0xFFFF0000FFFF0000ULL) | 
                           (uint64_t)((*tmp >> 16) & 0x0000FFFF0000FFFFULL);

          *tmp           = (uint64_t)( *tmp << 32)                          | 
                           (uint64_t)( *tmp >> 32);
          break;
        }
      default:
        {
          break;
        }
    }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uchar read_eas3_header(bwc_data *const data)                                              !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function opens an eas3 file and checks it for its validity. Once the specified file    !
!                has been verified, its header and flow field data is read and stored in the bwc_data        !
!                structure.                                                                                  !
!                                                                                                            !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                filename                    char*                 - Defines the filename of the eas3 file   !
!                                                                    that is to be opened and read.          !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                file                      - Defines a structure used to store all                           !
!                                            the relevant parameters and the data                            !
!                                            field of an eas3 file.                                          !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                20.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uchar
read_eas3_header(bwc_data *const data)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   Lread;
   uint64   i;
   uint8    precision;

   /*-----------------------*\
   ! DEFINE CHAR VARIABLES:  !
   \*-----------------------*/
   uchar   *buffer_char;
   char     param_name[ATTRLEN + 1] = {};

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_inf        *info;
   bwc_stream        *aux;
   eas3_std_params    params;

   /*-----------------------*\
   ! DEFINE FILE POINTER:    !
   \*-----------------------*/
   FILE    *fp;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(data);

   /*--------------------------------------------------------*\
   ! Save the file pointer and data info structure in tempo-  !
   ! rary variables to make the code more readable.           !
   \*--------------------------------------------------------*/
   fp   = data->file.fp;
   info = &data->info;

   /*--------------------------------------------------------*\
   ! Allocate the character buffer used to store chunks of    !
   ! memory read from the specified file.                     !
   \*--------------------------------------------------------*/
   buffer_char = calloc(20, sizeof(uchar));
   if(!buffer_char)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Read the first 20 bits of the specified file that store  !
   ! the identifier of an eas3 file.                          !
   \*--------------------------------------------------------*/
   if(fread(buffer_char, sizeof(uchar) , 20, fp) != 20)
   {
      // invalid read
      fprintf(stderr, RDERROR);
      free(buffer_char);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Check if the identifier corresponds to a valid EAS3 file.!
   ! If this is not the case, close the file pointer and exit !
   ! the bwc command-line tool.                               !
   \*--------------------------------------------------------*/
   if(!strstr((char*)buffer_char, "EAS3_I8R8"))
   {
      // invalid file type
      fprintf(stderr, "o##########################################################o\n"\
                      "|              ERROR: Invalid EAS3 file format             |\n"\
                      "o##########################################################o\n");
      free(buffer_char);
      return 1;
   }
 
   /*--------------------------------------------------------*\
   ! Allocate the auxiliary information packed stream.        !
   \*--------------------------------------------------------*/
   data->codestream.aux = calloc(1, sizeof(bwc_packed_stream));
   if(!data->codestream.aux)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      free(buffer_char);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Initialize the stream for the auxiliary information mem- !
   ! ory block. The initial size of the auxiliary memory      !
   ! block has been chosen arbitrarily and should be large    !
   ! enough to prevent excessive reallocation.                !
   \*--------------------------------------------------------*/
   aux = bwc_init_stream(NULL, AUX_SIZE, 'c');
   if(!aux)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      free(buffer_char);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Read the next 22 bytes from the specified file that re-  !
   ! present the eas3 standard parameters. The information    !
   ! is stored in the eas3_std_params structure.              !
   \*--------------------------------------------------------*/
   if(fread(&params, sizeof(uint64), 22, fp) != 22)
   {
      // invalid read
      fprintf(stderr, RDERROR);
      free(buffer_char);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Check if the specified file is of the EAS3 type.         !
   \*--------------------------------------------------------*/
   if(params.file_type == EAS2_TYPE)
   {       
      // invalid file format
      fprintf(stderr, "o##########################################################o\n"\
                      "|       ERROR: The EAS2 file format is not supported       |\n"\
                      "o##########################################################o\n"); 
      free(buffer_char);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Emit a file format hash to the aux stream to identify it !
   ! as a eas3 auxiliary information block. This hash can be  !
   ! used to properly handle decompression into an eas3 file  !
   ! or conversion to other file formats.                     !
   \*--------------------------------------------------------*/
   // emit_symbol(aux, hash("eas3"), 8);

   /*--------------------------------------------------------*\
   ! Emit the standard parameters to the auxiliary informa-   !
   ! tion information memory block.                           !
   \*--------------------------------------------------------*/
   bwc_emit_chunck(aux, (uchar*)&params, 176);

   /*--------------------------------------------------------*\
   ! Convert the parameters required for the bwc compression  !
   ! stage to little endian and store them in the file info   !
   ! structure.                                               !
   \*--------------------------------------------------------*/
   endian_conversion(&params.nzs, 8);
   info->nTS = (uint16)params.nzs;

   endian_conversion(&params.npar, 8);
   info->nPar = (uint8)params.npar;

   endian_conversion(&params.ndim1, 8);
   info->nX = (uint64)params.ndim1;

   endian_conversion(&params.ndim2, 8);
   info->nY = (uint64)params.ndim2;

   endian_conversion(&params.ndim3, 8);
   info->nZ = (uint64)params.ndim3;

   endian_conversion(&params.accuracy, 8);
   if(params.accuracy == 1)
   {
      precision = 4;
   }
   else if(params.accuracy == 2)
   {
      precision = 8;
   }
   else
   {
      fprintf(stderr, "o##########################################################o\n"\
                      "| ERROR: The accuracy of the specified dataset is not sup- |\n"\
                      "|        ported by the compression algorithm.              |\n"\
                      "o##########################################################o\n");
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Convert the size parameters, used to load the rest of the!
   ! header, to little endian.                                !
   \*--------------------------------------------------------*/
   endian_conversion(&params.size_time,      8);
   endian_conversion(&params.size_parameter, 8);
   endian_conversion(&params.size_dim1,      8);
   endian_conversion(&params.size_dim2,      8);
   endian_conversion(&params.size_dim3,      8);
   endian_conversion(&params.udef_char_size, 8);
   endian_conversion(&params.udef_int_size,  8);
   endian_conversion(&params.udef_real_size, 8);

   /*--------------------------------------------------------*\
   ! Allocate the time step array. If successful, read the    !
   ! timesteps from the file stream.                          !
   \*--------------------------------------------------------*/
   buffer_char = realloc(buffer_char, info->nTS * sizeof(uint64));
   if(!buffer_char)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   if(fread(buffer_char, sizeof(uint64), info->nTS, fp) != info->nTS)
   {
      // invalid read
      fprintf(stderr, RDERROR);
      free(buffer_char);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Emit the time step array to the auxiliary information    !
   ! memory block.                                            !
   \*--------------------------------------------------------*/
   bwc_emit_chunck(aux, buffer_char, info->nTS * sizeof(uint64));

   /*--------------------------------------------------------*\
   ! Check if any attributes have been specified in the eas3  !
   ! file.                                                    !
   \*--------------------------------------------------------*/
   if(params.attribute_mode == EAS3_ALL_ATTR)
   {
      /*--------------------------------------------------------*\
      ! Allocate the buffer character array. If successful, read !
      ! the timestep attributes from the file stream.            !
      \*--------------------------------------------------------*/
      buffer_char = realloc(buffer_char, info->nTS * ATTRLEN * sizeof(char));
      if(!buffer_char)
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         free(buffer_char);
         return 1;
      }

      if(fread(buffer_char, sizeof(char), info->nTS * ATTRLEN, fp) != (info->nTS * ATTRLEN))
      {
         // invalid read
         fprintf(stderr, RDERROR);
         free(buffer_char);
         return 1;
      }

      /*--------------------------------------------------------*\
      ! Emit the timestep attribute array to the auxiliary infor-!
      ! mation memory block.                                     !
      \*--------------------------------------------------------*/
      bwc_emit_chunck(aux, buffer_char, info->nTS * ATTRLEN * sizeof(char));

      for(i = 0; i < info->nPar; ++i)
      {
         /*--------------------------------------------------------*\
         ! Read the parameter name from the file stream and add all !
         ! the necessary parameter information to the paramter      !
         ! linked list.                                             !
         \*--------------------------------------------------------*/
         if(fread(param_name, sizeof(char), ATTRLEN, fp) != ATTRLEN)
         {
            // invalid read
            fprintf(stderr, RDERROR);
            free(buffer_char);
            return 1;
         }

         bwc_add_param(data, param_name, precision);

         /*--------------------------------------------------------*\
         ! Read the parameter name from the file stream and add all !
         ! the necessary parameter information to the paramter      !
         ! linked list.                                             |
         \*--------------------------------------------------------*/
         memset(param_name, 0, ATTRLEN + 1);
      }
   }

   /*--------------------------------------------------------*\
   ! Evaluate the number of bytes that remain to be read from !
   ! the eas3 file header.                                    !
   \*--------------------------------------------------------*/
   Lread  = 0;
   Lread += (params.attribute_mode == EAS3_ALL_ATTR) ? 3 * ATTRLEN                                         : 0;
   Lread += (params.gmode_time     >  EAS3_NO_G)     ? params.size_time      * sizeof(uint64)              : 0;
   Lread += (params.gmode_param    >  EAS3_NO_G)     ? params.size_parameter * sizeof(uint64)              : 0;
   Lread += (params.gmode_dim1     >  EAS3_NO_G)     ? params.size_dim1      * sizeof(uint64)              : 0;
   Lread += (params.gmode_dim2     >  EAS3_NO_G)     ? params.size_dim2      * sizeof(uint64)              : 0;
   Lread += (params.gmode_dim3     >  EAS3_NO_G)     ? params.size_dim3      * sizeof(uint64)              : 0;
   Lread += (params.udef_param     == EAS3_ALL_UDEF) ? params.udef_char_size * sizeof(char)   * UDEFLEN +
                                                       params.udef_int_size  * sizeof(uint64)           +
                                                       params.udef_real_size * sizeof(double)              : 0;

   /*--------------------------------------------------------*\
   ! Reallocate the buffer character array to allow for the   !
   ! storage of the remaining header bytes of the eas3 file.  !
   \*--------------------------------------------------------*/
   buffer_char = realloc(buffer_char, Lread * sizeof(uchar));
   if(!buffer_char)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      free(buffer_char);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Read the remaining header bytes from the file stream.    !
   \*--------------------------------------------------------*/
   if(fread(buffer_char, sizeof(uchar), Lread, fp) != Lread)
   {
      // invalid read
      fprintf(stderr, RDERROR);
      free(buffer_char);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Emit the remaining header information the the auxiliary  !
   ! information stream.                                      !
   \*--------------------------------------------------------*/
   bwc_emit_chunck(aux, buffer_char, Lread);
   
   /*--------------------------------------------------------*\
   ! Free the buffer character array.                         !
   \*--------------------------------------------------------*/
   free(buffer_char);

   /*--------------------------------------------------------*\
   ! Terminate the auxiliary information stream. If success-  !
   ! ful, the address to the aux memory block stored is       !
   ! stored in the file structure alongside its size.         !
   \*--------------------------------------------------------*/
   if(bwc_terminate_stream(aux, data->codestream.aux))
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uchar read_eas3_header(bwc_data *const data)                                              !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function opens an eas3 file and checks it for its validity. Once the specified file    !
!                has been verified, its header and flow field data is read and stored in the bwc_data        !
!                structure.                                                                                  !
!                                                                                                            !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                filename                    char*                 - Defines the filename of the eas3 file   !
!                                                                    that is to be opened and read.          !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                file                      - Defines a structure used to store all                           !
!                                            the relevant parameters and the data                            !
!                                            field of an eas3 file.                                          !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                20.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uchar
write_eas3_header(bwc_data *const data)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   Lwrite;

   /*-----------------------*\
   ! DEFINE CHAR VARIABLES:  !
   \*-----------------------*/
   uchar   *buffer_char;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_inf        *info;
   bwc_stream        *aux;
   eas3_std_params   *params;
   bwc_cmd_opts_ll   *param;

   /*-----------------------*\
   ! DEFINE FILE POINTER:    !
   \*-----------------------*/
   FILE    *fp;
   
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(data);

   /*--------------------------------------------------------*\
   ! Save the file pointer and data info structure in tempo-  !
   ! rary variables to make the code more readable.           !
   \*--------------------------------------------------------*/
   fp   = data->file.fp;
   info = &data->info;

   /*--------------------------------------------------------*\
   ! Write the valid EAS3 identifier to the specified file.   !
   \*--------------------------------------------------------*/
   if(fwrite("EAS3_I8R8           ", sizeof(char), 20, fp) != 20)
   {
      // invalid read
      fprintf(stderr, WRTERROR);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Initialize the auxiliary information stream.             !
   \*--------------------------------------------------------*/
   aux = bwc_init_stream(data->codestream.aux->memory, 
                         data->codestream.aux->size,   'd');

   /*--------------------------------------------------------*\
   ! Get the standard parameters from the auxiliary informa-  !
   ! memory block and write them to the file stream.          !
   \*--------------------------------------------------------*/
   params = (eas3_std_params*)bwc_get_chunck(aux, 176);

   if(fwrite(params, sizeof(uint64), 22, fp) != 22)
   {
      // invalid read
      fprintf(stderr, WRTERROR);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Convert the size parameters, used to write the rest of   !
   ! the header, to little endian.                            !
   \*--------------------------------------------------------*/
   endian_conversion(&params->accuracy,       8);
   endian_conversion(&params->size_time,      8);
   endian_conversion(&params->size_parameter, 8);
   endian_conversion(&params->size_dim1,      8);
   endian_conversion(&params->size_dim2,      8);
   endian_conversion(&params->size_dim3,      8);
   endian_conversion(&params->udef_char_size, 8);
   endian_conversion(&params->udef_int_size,  8);
   endian_conversion(&params->udef_real_size, 8);

   /*--------------------------------------------------------*\
   ! Allocate the buffer character array. If successful, get  !
   ! the timestep array from the auxiliary information block  !
   ! and write it to the file stream.                         !
   \*--------------------------------------------------------*/
   buffer_char = bwc_get_chunck(aux, info->nTS * sizeof(uint64));
   if(!buffer_char)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      free(buffer_char);
      return 1;
   }

   if(fwrite(buffer_char, sizeof(uint64), info->nTS, fp) != info->nTS)
   {
      // invalid read
      fprintf(stderr, WRTERROR);
      free(buffer_char);
      return 1;
   }
   free(buffer_char);

   /*--------------------------------------------------------*\
   ! Check if any attributes have been specified in the aux-  !
   ! iliary information block.                                !
   \*--------------------------------------------------------*/
   if(params->attribute_mode == EAS3_ALL_ATTR)
   {
      /*--------------------------------------------------------*\
      ! Allocate the buffer character array. If successful, get  !
      ! the timestep attribute array from the auxiliary informa- !
      ! tion block and write it to the file stream.              !
      \*--------------------------------------------------------*/
      buffer_char = bwc_get_chunck(aux, info->nTS * ATTRLEN);
      if(!buffer_char)
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         free(buffer_char);
         return 1;
      }

      if(fwrite(buffer_char, sizeof(uchar), info->nTS * ATTRLEN, fp) != (info->nTS * ATTRLEN))
      {
         // invalid read
         fprintf(stderr, WRTERROR);
         free(buffer_char);
         return 1;
      }
      free(buffer_char);

      /*--------------------------------------------------------*\
      ! Loop through the parameter array and...                  !
      \*--------------------------------------------------------*/
      if(data->info.parameter)
      {
         param = data->info.parameter->root;

         while(param != NULL)
         {
            /*--------------------------------------------------------*\
            ! ... write the parameter name from the info structure to  !
            ! the file stream.                                         !
            \*--------------------------------------------------------*/
            if(fwrite(param->name, sizeof(char), ATTRLEN, fp) != ATTRLEN)
            {
               // invalid read
               fprintf(stderr, WRTERROR);
               free(buffer_char);
               return 1;
            }

            param = param -> next;
         }
      }
   }

   /*--------------------------------------------------------*\
   ! Evaluate the number of bytes that remain to be read from !
   ! the auxiliary information block.                         !
   \*--------------------------------------------------------*/
   Lwrite  = 0;
   Lwrite += (params->attribute_mode == EAS3_ALL_ATTR) ? 3 * ATTRLEN                                         : 0;
   Lwrite += (params->gmode_time     >  EAS3_NO_G)     ? params->size_time      * sizeof(uint64)              : 0;
   Lwrite += (params->gmode_param    >  EAS3_NO_G)     ? params->size_parameter * sizeof(uint64)              : 0;
   Lwrite += (params->gmode_dim1     >  EAS3_NO_G)     ? params->size_dim1      * sizeof(uint64)              : 0;
   Lwrite += (params->gmode_dim2     >  EAS3_NO_G)     ? params->size_dim2      * sizeof(uint64)              : 0;
   Lwrite += (params->gmode_dim3     >  EAS3_NO_G)     ? params->size_dim3      * sizeof(uint64)              : 0;
   Lwrite += (params->udef_param     == EAS3_ALL_UDEF) ? params->udef_char_size * sizeof(char)   * UDEFLEN +
                                                         params->udef_int_size  * sizeof(uint64)           +
                                                         params->udef_real_size * sizeof(double)              : 0;

   /*--------------------------------------------------------*\
   ! Allocate the buffer character array. If successful, get  !
   ! the remaining eas header bytes from the auxiliary infor- !
   ! mation block and write it to the file stream.            !
   \*--------------------------------------------------------*/
   buffer_char = bwc_get_chunck(aux, Lwrite);
   if(!buffer_char)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      free(buffer_char);
      return 1;
   }

   if(fwrite(buffer_char, sizeof(uchar), Lwrite, fp) != Lwrite)
   {
      // invalid read
      fprintf(stderr, WRTERROR);
      free(buffer_char);
      return 1;
   }
   free(buffer_char);

   /*--------------------------------------------------------*\
   ! Free the auxiliary information memory block stream and   !
   ! params structure.                                        !
   \*--------------------------------------------------------*/
   free(aux);
   free(params);

   return 0;
}

/************************************************************************************************************\
||                  ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                  ||
||                  |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__                   ||
||                  |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]                  ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: bwc_data* read_eas3(const char* const filename)                                           !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function opens an eas3 file and checks it for its validity. Once the specified file    !
!                has been verified, its header and flow field data is read and stored in the bwc_data        !
!                structure.                                                                                  !
!                                                                                                            !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                filename                    char*                 - Defines the filename of the eas3 file   !
!                                                                    that is to be opened and read.          !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                file                      - Defines a structure used to store all                           !
!                                            the relevant parameters and the data                            !
!                                            field of an eas3 file.                                          !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                20.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
bwc_data*
read_eas3(char *const filename)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   Lfield;
   uint64   i;
   uint32   root;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_data          *data;

   /*--------------------------------------------------------*\
   ! Allocate the data structure.                             !
   \*--------------------------------------------------------*/
   data = calloc(1,  sizeof(bwc_data));
   if(!data)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! Set the file identifier used to select the appropriate   !
   ! write operation during decompression.                    !
   \*--------------------------------------------------------*/   
   strncpy(data->info.f_ext, "eas", 4);

   /*--------------------------------------------------------*\
   ! Open the specified file for reading. If the file doesn't !
   ! exist, exit the bwc command-line tool.                   !
   \*--------------------------------------------------------*/
   if((data->file.fp = fopen(filename, "rb")) == NULL)
   {
      // error opening file
      fprintf(stderr, "o##########################################################o\n"\
                      "|   ERROR: Could not open or read %-25s|\n"\
                      "o##########################################################o\n", filename);
      bwc_free_data(data);
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! Parse the eas3 header and store the information in the   !
   ! data structure.                                          !
   \*--------------------------------------------------------*/
   if(read_eas3_header(data))
   {
      //error reading eas3 header
      bwc_free_data(data);
   }

   /*--------------------------------------------------------*\
   ! Determine the size of the dataset present in the eas3    !
   ! file and store the information in the bwc_gl_data struc- !
   ! ture.                                                    !
   \*--------------------------------------------------------*/
   root   = ftell(data->file.fp);
   fseek(data->file.fp, 0L,   SEEK_END); 
   Lfield = (ftell(data->file.fp) - root) / sizeof(double);
   fseek(data->file.fp, root, SEEK_SET);

   /*--------------------------------------------------------*\
   ! Check if the file_size coincide with the specified dimen-!
   ! sions, timesteps number of parameters or bitdepth speci- !
   ! fied in the eas3 file header.                            !
   \*--------------------------------------------------------*/
   if(Lfield != data->info.nX   * data->info.nY  * 
                data->info.nZ   * data->info.nTS * 
                                  data->info.nPar)
   {
      // error in file size
      fprintf(stderr, "o##########################################################o\n"\
                      "| ERROR: Number of bytes present in the input file doesn't |\n"\
                      "|        coincide with the specified dimensions, timesteps |\n"\
                      "|        and number of parameters specified in the file    |\n"\
                      "|        header.                                           |\n"\
                      "o##########################################################o\n");
      bwc_free_data(data);
      return NULL;
   }

   if(data->info.parameter->precision == 4)
   {
      /*--------------------------------------------------------*\
      ! Allocate the real field that will hold the numerical     !
      ! dataset.                                                 !
      \*--------------------------------------------------------*/
      data->field.d = NULL;
      data->field.f = calloc(Lfield, sizeof(float));
      if(!data->field.d)
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         bwc_free_data(data);
         return NULL;
      }

      /*--------------------------------------------------------*\
      ! Read the flow field data from the specified eas3 file.   !
      \*--------------------------------------------------------*/
      if(fread(data->field.f, sizeof(float), Lfield, data->file.fp) != Lfield)
      {
         // invalid read
         fprintf(stderr, RDERROR);
         bwc_free_data(data);
         return NULL;
      }

      /*--------------------------------------------------------*\
      ! Convert the flow field data read from the eas3 file from !
      ! big endian to little endian.                             !
      \*--------------------------------------------------------*/
      for(i = Lfield; i --> 0;)
      {
         endian_conversion(&data->field.f[i], 4);
      }
   }
   else if(data->info.parameter->precision == 8)
   {
      /*--------------------------------------------------------*\
      ! Allocate the real field that will hold the numerical     !
      ! dataset.                                                 !
      \*--------------------------------------------------------*/
      data->field.f = NULL;
      data->field.d = calloc(Lfield, sizeof(double));
      if(!data->field.d)
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         bwc_free_data(data);
         return NULL;
      }

      /*--------------------------------------------------------*\
      ! Read the flow field data from the specified eas3 file.   !
      \*--------------------------------------------------------*/
      if(fread(data->field.d, sizeof(double), Lfield, data->file.fp) != Lfield)
      {
         // invalid read
         fprintf(stderr, RDERROR);
         bwc_free_data(data);
         return NULL;
      }

      /*--------------------------------------------------------*\
      ! Convert the flow field data read from the eas3 file from !
      ! big endian to little endian.                             !
      \*--------------------------------------------------------*/
      for(i = Lfield; i --> 0;)
      {
         endian_conversion(&data->field.d[i], 8);
      }
   }

   /*--------------------------------------------------------*\
   ! Close the file pointer and return the bwc_data structure !
   ! to the function caller.                                  !
   \*--------------------------------------------------------*/
   fclose(data->file.fp);
   data->file.fp = NULL;
   return data;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uchar write_eas3(bwc_data *const file, char *const filename)                              !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function creates a valid eas3 file from the information stored in the bwc_data         !
!                structure.                                                                                  !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                filename                    char*                 - Defines the filename of the eas3 file   !
!                                                                    that is to be opened and read.          !
!                                                                                                            !
!                file                        bwc_data*            - Defines a structure used to store all    !
!                                                                   the relevant parameters and the data     !
!                                                                   field of an eas3 file.                   !
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
!                20.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
write_eas3(bwc_data *const data, char *const filename)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   Lfield;
   uint64   i;
   
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(data);

   /*--------------------------------------------------------*\
   ! Open the specified file for writing. If the file already !
   ! exist, discard its content. If the file cannot be creat- !
   ! ed, exit the bwc command-line tool.                      !
   \*--------------------------------------------------------*/
   if((data->file.fp = fopen(filename, "wb")) == NULL)
   {
      // error opening file
      fprintf(stderr, "o##########################################################o\n"\
                      "|   ERROR: Could not open or write %-24s|\n"\
                      "o##########################################################o\n", filename);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Write the eas3 header to the specified file.             !
   \*--------------------------------------------------------*/
   if(write_eas3_header(data))
   {
      //error reading eas3 header
      return 1;
   }


   /*--------------------------------------------------------*\
   ! Calculate the size of the data field used for the endian !
   ! conversion and write operations.                         !
   \*--------------------------------------------------------*/
   Lfield = data->info.nX  * data->info.nY  * 
            data->info.nZ  * data->info.nTS * data->info.nPar;

   if(data->info.parameter->precision == 4)
   {
      /*--------------------------------------------------------*\
      ! Convert the flow field data from big endian to endian.   !
      \*--------------------------------------------------------*/
      for(i = Lfield; i --> 0;)
      {
         endian_conversion(&data->field.f[i], 4);
      }

      /*--------------------------------------------------------*\
      ! Write the flow field data to the specified eas3 file.    !
      \*--------------------------------------------------------*/
      if(fwrite(data->field.f, sizeof(float), Lfield, data->file.fp) != Lfield)
      {
         // invalid read
         fprintf(stderr, WRTERROR);
         return 1;
      }
   }
   else if(data->info.parameter->precision == 8)
   {
      /*--------------------------------------------------------*\
      ! Convert the flow field data from big endian to endian.   !
      \*--------------------------------------------------------*/
      for(i = Lfield; i --> 0;)
      {
         endian_conversion(&data->field.d[i], 8);
      }

      /*--------------------------------------------------------*\
      ! Write the flow field data to the specified eas3 file.    !
      \*--------------------------------------------------------*/
      if(fwrite(data->field.d, sizeof(double), Lfield, data->file.fp) != Lfield)
      {
         // invalid read
         fprintf(stderr, WRTERROR);
         return 1;
      }
   }

   /*--------------------------------------------------------*\
   ! Close the file pointer and return to the function caller.!
   \*--------------------------------------------------------*/
   fclose(data->file.fp);
   data->file.fp = NULL;
   return 0;
}