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


/************************************************************************************************************\
||                                      _  _ ____ ____ ____ ____ ____                                       ||
||                                      |\/| |__| |    |__/ |  | [__                                        ||
||                                      |  | |  | |___ |  \ |__| ___]                                       ||
||                                                                                                          ||
\************************************************************************************************************/
#define MEMERROR "o##########################################################o\n"\
                 "|                   ERROR: Out of memory                   |\n"\
                 "o##########################################################o\n"

#define RDERROR   "o##########################################################o\n"\
                  "|      ERROR: Invalid Number of Bytes Read from File.      |\n"\
                  "o##########################################################o\n"
 
#define WRTERROR  "o##########################################################o\n"\
                  "|      ERROR: Invalid Number of Bytes Written to File.     |\n"\
                  "o##########################################################o\n"

/************************************************************************************************************\
||                ___  ____ _ _  _ ____ ___ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                ||
||                |__] |__/ | |  | |__|  |  |___    |___ |  | |\ | |     |  | |  | |\ | [__                 ||
||                |    |  \ |  \/  |  |  |  |___    |    |__| | \| |___  |  | |__| | \| ___]                ||
||                                                                                                          ||
\************************************************************************************************************/
/*================================================================================================*/
/**
 * @details Enqueues a chunck of size length to the auxilliary information.
 */
/*================================================================================================*/
#define aux_enqueue(aux, chunck, chunck_len)      \
{                                                 \
   if (aux.pos + chunck_len > aux.len)            \
   {                                              \
      while(aux.pos + chunck_len > aux.len)       \
      {                                           \
        aux.len += aux.len / 2;                   \
      }                                           \
      aux.ptr = realloc(aux.ptr, aux.len);        \
   }                                              \
   memcpy(aux.ptr + aux.pos, chunck, chunck_len); \
   aux.pos += chunck_len;                         \
}

/*================================================================================================*/
/**
 * @details Dequeues a chunck of size length from the auxilliary information.
 */
/*================================================================================================*/
#define aux_dequeue(aux, chunck, chunck_len)         \
{                                                    \
   if(aux.pos + chunck_len <= aux.len) {             \
      memcpy(chunck, aux.ptr + aux.pos, chunck_len); \
      aux.pos += chunck_len;                         \
   } else {                                          \
      fprintf(stderr, MEMERROR);                     \
   }                                                 \
}

/*================================================================================================*/
/**
 * @details Converts the endianess of half, single, or double precision values.
 *
 * @param[inout]   value     Pointer to the parameter to be converted.
 * @param[in]      accuracy  Precision/accuracy of the pointed parameter.
 */
/*================================================================================================*/
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

/*================================================================================================*/
/**
 * @details Deallocates the provided eas3_data structure including all contained data.
 *
 * @param[in]   data     Pointer to eas3_data structure to be filled.
 */
/*================================================================================================*/
void
eas3_free_data(eas3_data* data)
{
  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  eas3_param_names *param, *temp;
  
  if(data != NULL)
  {
    if (data->param_names != NULL)
      {
        param = data->param_names->root;
  
        while(param != NULL)
          {
            temp  = param;
            param = param -> next;
            free(temp);
          }
      }

    if (data->field.d != NULL)
      free(data->field.d);

    if (data->field.f != NULL)
      free(data->field.f);

    if(data->aux.ptr != NULL)
      free(data->aux.ptr);

    free(data);
  }
}

/*================================================================================================*/
/**
 * @details Adds a parameter name to the linked list inside the eas3_data structure.
 *
 * @param[in]   data     Pointer to eas3_data structure to be filled.
 * @param[in]   name     Name to be added to the linked list.
 */
/*================================================================================================*/
void
eas3_add_param_name(eas3_data *const data, char *name)
{
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
   if(data->param_names == NULL)
   {
      /*--------------------------------------------------------*\
      ! If eas3_add_param_name function is called for the first  !
      ! time, allocate the parameter structure and save the root !
      ! node address.                                            !
      \*--------------------------------------------------------*/
      data->param_names       = calloc(1, sizeof(eas3_param_names));
      data->param_names->root = data->param_names;
   }
   else
   {
      /*--------------------------------------------------------*\
      ! If a new parameter is added, allocate the nex linked     !
      ! list node, save the root node address in its structure   !
      ! and set the linked list access pointer to the new node.  !
      \*--------------------------------------------------------*/
      data->param_names->next       = calloc(1, sizeof(eas3_param_names));
      data->param_names->next->root = data->param_names->root;
      data->param_names->next->id   = data->param_names->id + 1;
      data->param_names             = data->param_names->next;
   }

   /*--------------------------------------------------------*\
   ! Save the name of the new parameter its precision in the  !
   ! structure of the new node.                               !
   \*--------------------------------------------------------*/
   strcpy(data->param_names->name, name ? name : "undefined");
}

/*================================================================================================*/
/**
 * @details Parses the uncompressed output from bwc_stream into the eas3_data.
 *
 * @param[in]      stream   Pointer to uncompressed data set.
 * @param[inout]   data     Pointer to eas3_data structure to be filled.
 *
 * @retval uchar
 */
/*================================================================================================*/
uchar
bwc_to_eas3(bwc_stream *const stream, eas3_data *const data)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   size;
   uint64   i;

   /*-----------------------*\
   ! DEFINE CHAR VARIABLES:  !
   \*-----------------------*/
   uchar   *buffer_char;
   char     param_name[ATTRLEN + 1] = {};

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   eas3_std_params *params;

   params = &data->params;

   data->aux.ptr = calloc(stream->codestream.aux->size, sizeof(uchar));
   data->aux.pos = 0;
   data->aux.len = stream->codestream.aux->size;
   if(!data->aux.ptr)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      free(data->aux.ptr);
      return 1;
   }
   memcpy(data->aux.ptr, stream->codestream.aux->memory,
          stream->codestream.aux->size);
   
   aux_dequeue(data->aux, (uchar*)params, 176);

   endian_conversion(&params->nts, 8);
   endian_conversion(&params->npar, 8);
   endian_conversion(&params->ndim1, 8);
   endian_conversion(&params->ndim2, 8);
   endian_conversion(&params->ndim3, 8);
   endian_conversion(&params->accuracy, 8);
   if(params->accuracy != 1 && params->accuracy != 2)
   {
      fprintf(stderr, "o##########################################################o\n"\
                      "| ERROR: The accuracy of the specified dataset is not sup- |\n"\
                      "|        ported by the compression algorithm.              |\n"\
                      "o##########################################################o\n");
      return 1;
   }

   buffer_char = calloc(params->nts, sizeof(uint64));
   if(!buffer_char)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      free(buffer_char);
      return 1;
   }
   aux_dequeue(data->aux, buffer_char, params->nts * sizeof(uint64));

   if(params->attribute_mode == EAS3_ALL_ATTR)
   {
      buffer_char = realloc(buffer_char, params->nts * ATTRLEN * sizeof(char));
      if(!buffer_char)
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         free(buffer_char);
         return 1;
      }
      aux_dequeue(data->aux, buffer_char, params->nts * ATTRLEN * sizeof(char));

      for(i = 0; i < params->npar; ++i)
      {
         aux_dequeue(data->aux, param_name, ATTRLEN * sizeof(char));
         eas3_add_param_name(data, param_name);
         memset(param_name, 0, ATTRLEN + 1);
      }
   }

   size = params->ndim1 * params->ndim2 * params->ndim3 *
          params->nts * params->npar;

   if(params->accuracy == 1)
   {
     data->field.d = NULL;
     data->field.f = calloc(size, sizeof(float));
     memcpy(data->field.f, stream->out, size*sizeof(float));
   }
   else if(params->accuracy == 2)
   {
     data->field.f = NULL;
     data->field.d = calloc(size, sizeof(double));
     memcpy(data->field.d, stream->out, size*sizeof(double));
   }

   return 0;
}

/*================================================================================================*/
/**
 * @details Reads the header from an open eas3 file pointer parsing the header information into
 *          the eas3_data structure argument.
 *
 * @param[in]      fp       Readily opened file pointer.
 * @param[inout]   data     Structure to store eas3 data.
 *
 * @retval uchar
 */
/*================================================================================================*/
static uchar
read_eas3_header(FILE *const fp, eas3_data *const data)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   Lread;
   uint64   i;

   /*-----------------------*\
   ! DEFINE CHAR VARIABLES:  !
   \*-----------------------*/
   uchar   *buffer_char;
   char     param_name[ATTRLEN + 1] = {};

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   eas3_std_params   *params;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(data);

   /*--------------------------------------------------------*\
   ! Save the eas3_std_params structure in temporary          !
   ! variables to make the code more readable.                !
   \*--------------------------------------------------------*/
   params = &data->params;

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
   ! Initialize the stream for the auxiliary information mem- !
   ! ory block. The initial size of the auxiliary memory      !
   ! block has been chosen arbitrarily and should be large    !
   ! enough to prevent excessive reallocation.                !
   \*--------------------------------------------------------*/
   data->aux.ptr = calloc(AUX_SIZE, sizeof(uchar));
   data->aux.pos = 0;
   data->aux.len = AUX_SIZE;
   if(!data->aux.ptr)
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
   if(fread(params, sizeof(uint64), 22, fp) != 22)
   {
      // invalid read
      fprintf(stderr, RDERROR);
      free(buffer_char);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Check if the specified file is of the EAS3 type.         !
   \*--------------------------------------------------------*/
   if(params->file_type == EAS2_TYPE)
   {       
      // invalid file format
      fprintf(stderr, "o##########################################################o\n"\
                      "|       ERROR: The EAS2 file format is not supported       |\n"\
                      "o##########################################################o\n"); 
      free(buffer_char);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Emit the standard parameters to the auxiliary informa-   !
   ! tion information memory block.                           !
   \*--------------------------------------------------------*/
   aux_enqueue(data->aux, (uchar*)params, 176);

   /*--------------------------------------------------------*\
   ! Convert the parameters required for the bwc compression  !
   ! stage to little endian and store them in the file info   !
   ! structure.                                               !
   \*--------------------------------------------------------*/
   endian_conversion(&params->nts, 8);

   endian_conversion(&params->npar, 8);

   endian_conversion(&params->ndim1, 8);

   endian_conversion(&params->ndim2, 8);

   endian_conversion(&params->ndim3, 8);

   endian_conversion(&params->accuracy, 8);
   if(params->accuracy != 1 && params->accuracy != 2)
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
   endian_conversion(&params->size_time,      8);
   endian_conversion(&params->size_parameter, 8);
   endian_conversion(&params->size_dim1,      8);
   endian_conversion(&params->size_dim2,      8);
   endian_conversion(&params->size_dim3,      8);
   endian_conversion(&params->udef_char_size, 8);
   endian_conversion(&params->udef_int_size,  8);
   endian_conversion(&params->udef_real_size, 8);

   /*--------------------------------------------------------*\
   ! Allocate the time step array. If successful, read the    !
   ! timesteps from the file stream.                          !
   \*--------------------------------------------------------*/
   buffer_char = realloc(buffer_char, params->nts * sizeof(uint64));
   if(!buffer_char)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   if(fread(buffer_char, sizeof(uint64), params->nts, fp) != params->nts)
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
   aux_enqueue(data->aux, buffer_char, params->nts * sizeof(uint64));

   /*--------------------------------------------------------*\
   ! Check if any attributes have been specified in the eas3  !
   ! file.                                                    !
   \*--------------------------------------------------------*/
   if(params->attribute_mode == EAS3_ALL_ATTR)
   {
      /*--------------------------------------------------------*\
      ! Allocate the buffer character array. If successful, read !
      ! the timestep attributes from the file stream.            !
      \*--------------------------------------------------------*/
      buffer_char = realloc(buffer_char, params->nts * ATTRLEN * sizeof(char));
      if(!buffer_char)
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         free(buffer_char);
         return 1;
      }

      if(fread(buffer_char, sizeof(char), params->nts * ATTRLEN, fp) != (params->nts * ATTRLEN))
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
      aux_enqueue(data->aux, buffer_char, params->nts * ATTRLEN * sizeof(char));

      for(i = 0; i < params->npar; ++i)
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

         aux_enqueue(data->aux, param_name, ATTRLEN * sizeof(char));
         eas3_add_param_name(data, param_name);

         memset(param_name, 0, ATTRLEN + 1);
      }
   }

   /*--------------------------------------------------------*\
   ! Evaluate the number of bytes that remain to be read from !
   ! the eas3 file header.                                    !
   \*--------------------------------------------------------*/
   Lread  = 0;
   Lread += (params->attribute_mode == EAS3_ALL_ATTR) ? 3 * ATTRLEN                                         : 0;
   Lread += (params->gmode_time     >  EAS3_NO_G)     ? params->size_time      * sizeof(uint64)              : 0;
   Lread += (params->gmode_param    >  EAS3_NO_G)     ? params->size_parameter * sizeof(uint64)              : 0;
   Lread += (params->gmode_dim1     >  EAS3_NO_G)     ? params->size_dim1      * sizeof(uint64)              : 0;
   Lread += (params->gmode_dim2     >  EAS3_NO_G)     ? params->size_dim2      * sizeof(uint64)              : 0;
   Lread += (params->gmode_dim3     >  EAS3_NO_G)     ? params->size_dim3      * sizeof(uint64)              : 0;
   Lread += (params->udef_param     == EAS3_ALL_UDEF) ? params->udef_char_size * sizeof(char)   * UDEFLEN +
                                                       params->udef_int_size  * sizeof(uint64)           +
                                                       params->udef_real_size * sizeof(double)              : 0;

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
   aux_enqueue(data->aux, buffer_char, Lread);
   
   /*--------------------------------------------------------*\
   ! Free the buffer character array.                         !
   \*--------------------------------------------------------*/
   free(buffer_char);

   /*--------------------------------------------------------*\
   ! Terminate the auxiliary information stream. If success-  !
   ! ful, the address to the aux memory block stored is       !
   ! stored in the file structure alongside its size.         !
   \*--------------------------------------------------------*/
    if(data->aux.pos != data->aux.len)
    {
      data->aux.len = data->aux.pos;
      data->aux.ptr = realloc(data->aux.ptr, data->aux.len);
      if(!data->aux.ptr)
      {
        // memory allocation error
        fprintf(stderr, MEMERROR);
        data->aux.len = 0;
        return 1;
      }
    }

   return 0;
}

/*================================================================================================*/
/**
 * @details Writes the header information from the eas3_data structure into the open eas3
 *          file pointer.
 *
 * @param[in]      fp       Readily opened file pointer.
 * @param[inout]   data     Structure to store eas3 data.
 *
 * @retval uchar
 */
/*================================================================================================*/
static uchar
write_eas3_header(FILE *const fp, eas3_data *const data)
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
   eas3_std_params   *params;
   eas3_param_names  *param_names;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(data);

   /*--------------------------------------------------------*\
   ! Write the valid EAS3 identifier to the specified file.   !
   \*--------------------------------------------------------*/
   if(fwrite("EAS3_I8R8           ", sizeof(char), 20, fp) != 20)
   {
      // invalid read
      fprintf(stderr, WRTERROR);
      return 1;
   }

   // Rewind aux
   data->aux.pos = 0;

   /*--------------------------------------------------------*\
   ! Get the standard parameters from the auxiliary informa-  !
   ! memory block and write them to the file stream.          !
   \*--------------------------------------------------------*/
   params = calloc(1, sizeof(eas3_std_params));
   if(!params)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      free(params);
      return 1;
   }
   aux_dequeue(data->aux, params, sizeof(eas3_std_params));

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
   endian_conversion(&params->file_type,      8);
   endian_conversion(&params->accuracy,       8);
   endian_conversion(&params->nts,            8);
   endian_conversion(&params->npar,           8);
   endian_conversion(&params->ndim1,          8);
   endian_conversion(&params->ndim2,          8);
   endian_conversion(&params->ndim3,          8);
   endian_conversion(&params->size_time,      8);
   endian_conversion(&params->size_parameter, 8);
   endian_conversion(&params->size_dim1,      8);
   endian_conversion(&params->size_dim2,      8);
   endian_conversion(&params->size_dim3,      8);
   endian_conversion(&params->udef_char_size, 8);
   endian_conversion(&params->udef_int_size,  8);
   endian_conversion(&params->udef_real_size, 8);

   data->params = *params;

   /*--------------------------------------------------------*\
   ! Allocate the buffer character array. If successful, get  !
   ! the timestep array from the auxiliary information block  !
   ! and write it to the file stream.                         !
   \*--------------------------------------------------------*/
   buffer_char = calloc(data->params.nts * sizeof(uint64), sizeof(uchar));
   if(!buffer_char)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      free(buffer_char);
      return 1;
   }
   aux_dequeue(data->aux, buffer_char, data->params.nts * sizeof(uint64));

   if(fwrite(buffer_char, sizeof(uint64), data->params.nts, fp) != data->params.nts)
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
      buffer_char = calloc(data->params.nts * ATTRLEN, sizeof(uchar));
      if(!buffer_char)
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         free(buffer_char);
         return 1;
      }
      aux_dequeue(data->aux, buffer_char, data->params.nts * ATTRLEN);

      if(fwrite(buffer_char, sizeof(uchar), data->params.nts * ATTRLEN, fp) != (data->params.nts * ATTRLEN))
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
      if(data->param_names)
      {
         param_names = data->param_names->root;

         while(param_names != NULL)
         {
            /*--------------------------------------------------------*\
            ! ... write the parameter name from the info structure to  !
            ! the file stream.                                         !
            \*--------------------------------------------------------*/
            if(fwrite(param_names->name, sizeof(char), ATTRLEN, fp) != ATTRLEN)
            {
               // invalid read
               fprintf(stderr, WRTERROR);
               free(buffer_char);
               return 1;
            }

            param_names = param_names->next;
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
   buffer_char = calloc(Lwrite, sizeof(uchar));
   if(!buffer_char)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      free(buffer_char);
      return 1;
   }
   aux_dequeue(data->aux, buffer_char, Lwrite);

   if(fwrite(buffer_char, sizeof(uchar), Lwrite, fp) != Lwrite)
   {
      // invalid read
      fprintf(stderr, WRTERROR);
      free(buffer_char);
      return 1;
   }
   free(buffer_char);

   /*--------------------------------------------------------*\
   ! Free the params structure.                               !
   \*--------------------------------------------------------*/
   free(params);

   return 0;
}

/************************************************************************************************************\
||                  ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                  ||
||                  |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__                   ||
||                  |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]                  ||
||                                                                                                          ||
\************************************************************************************************************/
/*================================================================================================*/
/**
 * @details Opens an eas3 file and checks it for its validity. Then, it reads header and flow
 *          field data and returns a filled instance of the eas3_data structure.
 *
 * @param[in]      filename Name of the eas3 file.
 *
 * @retval eas3_data*
 */
/*================================================================================================*/
eas3_data*
read_eas3(char *const filename)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   Lfield;
   uint64   i;
   uint32   root;
   FILE    *fp;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   eas3_data          *data;

   /*--------------------------------------------------------*\
   ! Allocate the data structure.                             !
   \*--------------------------------------------------------*/
   data = calloc(1,  sizeof(eas3_data));
   if(!data)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! Open the specified file for reading. If the file doesn't !
   ! exist, exit the bwc command-line tool.                   !
   \*--------------------------------------------------------*/
   if((fp = fopen(filename, "rb")) == NULL)
   {
      // error opening file
      fprintf(stderr, "o##########################################################o\n"\
                      "|   ERROR: Could not open or read %-25s|\n"\
                      "o##########################################################o\n", filename);
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! Parse the eas3 header and store the information in the   !
   ! data structure.                                          !
   \*--------------------------------------------------------*/
   if(read_eas3_header(fp, data))
   {
      //error reading eas3 header
      eas3_free_data(data);
   }

   /*--------------------------------------------------------*\
   ! Determine the size of the dataset present in the eas3    !
   ! file and store the information in the bwc_gl_data struc- !
   ! ture.                                                    !
   \*--------------------------------------------------------*/
   root   = ftell(fp);
   fseek(fp, 0L,   SEEK_END);
   Lfield = (ftell(fp) - root) / sizeof(double);
   fseek(fp, root, SEEK_SET);

   /*--------------------------------------------------------*\
   ! Check if the file_size coincide with the specified dimen-!
   ! sions, timesteps number of parameters or bitdepth speci- !
   ! fied in the eas3 file header.                            !
   \*--------------------------------------------------------*/
   if(Lfield != data->params.ndim1 * data->params.ndim2  *
                data->params.ndim3 * data->params.nts *
                                     data->params.npar)
   {
      // error in file size
      fprintf(stderr, "o##########################################################o\n"\
                      "| ERROR: Number of bytes present in the input file doesn't |\n"\
                      "|        coincide with the specified dimensions, timesteps |\n"\
                      "|        and number of parameters specified in the file    |\n"\
                      "|        header.                                           |\n"\
                      "o##########################################################o\n");
      eas3_free_data(data);
      return NULL;
   }

   if(data->params.accuracy == 1)
   {
      /*--------------------------------------------------------*\
      ! Allocate the real field that will hold the numerical     !
      ! dataset.                                                 !
      \*--------------------------------------------------------*/
      data->field.d = NULL;
      data->field.f = calloc(Lfield, sizeof(float));
      if(!data->field.f)
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         eas3_free_data(data);
         return NULL;
      }

      /*--------------------------------------------------------*\
      ! Read the flow field data from the specified eas3 file.   !
      \*--------------------------------------------------------*/
      if(fread(data->field.f, sizeof(float), Lfield, fp) != Lfield)
      {
         // invalid read
         fprintf(stderr, RDERROR);
         eas3_free_data(data);
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
   else if(data->params.accuracy == 2)
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
         eas3_free_data(data);
         return NULL;
      }

      /*--------------------------------------------------------*\
      ! Read the flow field data from the specified eas3 file.   !
      \*--------------------------------------------------------*/
      if(fread(data->field.d, sizeof(double), Lfield, fp) != Lfield)
      {
         // invalid read
         fprintf(stderr, RDERROR);
         eas3_free_data(data);
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
   fclose(fp);
   fp = NULL;
   return data;
}

/*================================================================================================*/
/**
 * @details Opens an eas3 file and writes the content from the provided eas3_data structure.
 *
 * @param[in]      data     Data to be written into the eas3 file.
 * @param[in]      filename Name of the eas3 file.
 *
 * @retval uchar
 */
/*================================================================================================*/
uchar
write_eas3(eas3_data *const data, char *const filename)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   Lfield;
   uint64   i;
   FILE    *fp;
   
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(data);

   /*--------------------------------------------------------*\
   ! Open the specified file for writing. If the file already !
   ! exist, discard its content. If the file cannot be creat- !
   ! ed, exit the bwc command-line tool.                      !
   \*--------------------------------------------------------*/
   if((fp = fopen(filename, "wb")) == NULL)
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
   if(write_eas3_header(fp, data))
   {
      //error reading eas3 header
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Calculate the size of the data field used for the endian !
   ! conversion and write operations.                         !
   \*--------------------------------------------------------*/
   Lfield = data->params.ndim1  * data->params.ndim2  *
            data->params.ndim3  * data->params.nts * data->params.npar;

   if(data->params.accuracy == 1)
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
      if(fwrite(data->field.f, sizeof(float), Lfield, fp) != Lfield)
      {
         // invalid read
         fprintf(stderr, WRTERROR);
         return 1;
      }
   }
   else if(data->params.accuracy == 2)
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
      if(fwrite(data->field.d, sizeof(double), Lfield, fp) != Lfield)
      {
         // invalid read
         fprintf(stderr, WRTERROR);
         return 1;
      }
   }

   /*--------------------------------------------------------*\
   ! Close the file pointer and return to the function caller.!
   \*--------------------------------------------------------*/
   fclose(fp);
   fp = NULL;
   return 0;
}