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
||        This file defines simple read and write functions used to access conforming             ||
||        netCDF datasets.                                                                        ||
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
#include <bwc.h>
#include <math.h>
#include <netcdf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************************************************************************************************\
||           ___  ____ _ _  _ ____ ___ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____           ||
||           |__] |__/ | |  | |__|  |  |___    |___ |  | |\ | |     |  | |  | |\ | [__            ||
||           |    |  \ |  \/  |  |  |  |___    |    |__| | \| |___  |  | |__| | \| ___]           ||
||                                                                                                ||
\**************************************************************************************************/
/*------------------------------------------------------------------------------------------------*\
!                                                                                                  !
!   DESCRIPTION:                                                                                   !
!   ------------                                                                                   !
!                                                                                                  !
!         This function opens an eas3 file and checks it for its validity. Once the                !
!         specified file has been verified, its header and flow field data is read and             !
!         stored in the bwc_data structure.                                                        !
!                                                                                                  !
!   ARGUMENTS:                                                                                     !
!   ----------                                                                                     !
!                 Name                          Description                                        !
!                 ----                          -----------                                        !
!                 filename                    - Defines the filename of the eas3 file that         !
!                                               is to be opened and read.                          !
!                                                                                                  !
!   RETURN:                                                                                        !
!   -------                                                                                        !
!                                                                                                  !
!                Defines a structure used to store all the relevant parameters and the             !
!                data field of an eas3 file.                                                       !
!                                                                                                  !
\*------------------------------------------------------------------------------------------------*/
uchar
get_attributes(bwc_stream *const aux, const int ncId, const int varId, const int attrId)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int      status;
   uint16   Lchk;
   size_t   attr_len, attr_size;
   nc_type  attrTypeId;
   
   /*-----------------------*\
   ! DEFINE CHAR VARIABLES:  !
   \*-----------------------*/
   char    *attr   = NULL,
           *buffer = NULL;

   /*--------------------------------------------------------*\
   ! Allocate a character array used to get strings from the  !
   ! specified data file.                                     !
   \*--------------------------------------------------------*/
   buffer = calloc(NC_MAX_ATTRS, sizeof(uchar));
   if(!buffer)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      free(buffer);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Read the attribute name from the data file.              !
   \*--------------------------------------------------------*/
   if((status = nc_inq_attname(ncId, varId, attrId, buffer)))
   {
      fprintf(stderr, "%s\n", nc_strerror(status));
      free(buffer);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Read the attribute type and length from the data file.   !
   \*--------------------------------------------------------*/
   if((status = nc_inq_att(ncId, varId, buffer, &attrTypeId, &attr_len)))
   {
      fprintf(stderr, "%s\n", nc_strerror(status));
      free(buffer);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Calculate the attribute size with regard to its specif-  !
   ! ied type.                                                !
   \*--------------------------------------------------------*/
   switch(attrTypeId)
   {
      case NC_BYTE:
      case NC_UBYTE:
      case NC_CHAR:
      case NC_STRING:
      {
         attr_size = attr_len + 1;
         break;
      }

      case NC_SHORT:
      case NC_USHORT:
      {
         attr_size = (attr_len * 2) + 1;
         break;
      }

      case NC_INT:
      case NC_UINT:
      case NC_FLOAT:
      {
         attr_size = (attr_len * 4) + 1;
         break;
      }

      case NC_INT64:
      case NC_UINT64:
      case NC_DOUBLE:
      {
         attr_size = (attr_len * 8) + 1;
         break;
      }
   }

   /*--------------------------------------------------------*\
   ! Allocate a character array used to store the attribute   !
   ! value and null terminate it.                             !
   \*--------------------------------------------------------*/
   attr = calloc(1, attr_size);
   if(!attr)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      free(buffer);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Read the attribute type and length from the data file.   !
   \*--------------------------------------------------------*/
   if((status = nc_get_att(ncId, varId, buffer, attr)))
   {
      fprintf(stderr, "%s\n", nc_strerror(status));
      free(buffer);
      free(attr);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Store the attribute name, value, type and length in the  !
   ! auxiliary bitstream.                                     !
   \*--------------------------------------------------------*/
   Lchk = (uint16)strlen(buffer);
   emit_symbol(aux, Lchk,           2);
   emit_chunck(aux, (uchar*)buffer, Lchk);

   Lchk = (uint16)strlen(attr);
   emit_symbol(aux, Lchk,           2);
   emit_chunck(aux, (uchar*)attr,   Lchk);

   emit_symbol(aux, attrTypeId,     sizeof(nc_type));
   emit_symbol(aux, attr_len,       sizeof(size_t));

   /*--------------------------------------------------------*\
   ! Free the attribute character array and return to the     !
   ! function caller.                                         !
   \*--------------------------------------------------------*/
   free(buffer);
   free(attr);
   return 0;
}

/*------------------------------------------------------------------------------------------------*\
!                                                                                                  !
!   DESCRIPTION:                                                                                   !
!   ------------                                                                                   !
!                                                                                                  !
!         This function opens an eas3 file and checks it for its validity. Once the                !
!         specified file has been verified, its header and flow field data is read and             !
!         stored in the bwc_data structure.                                                        !
!                                                                                                  !
!   ARGUMENTS:                                                                                     !
!   ----------                                                                                     !
!                 Name                          Description                                        !
!                 ----                          -----------                                        !
!                 filename                    - Defines the filename of the eas3 file that         !
!                                               is to be opened and read.                          !
!                                                                                                  !
!   RETURN:                                                                                        !
!   -------                                                                                        !
!                 Defines a structure used to store all the relevant parameters                    !
!                 and the data field of an eas3 file.                                              !
!                                                                                                  !
\*------------------------------------------------------------------------------------------------*/
uchar
put_attributes(bwc_stream *const aux, const int ncId, const int varId)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int      status;
   uint16   Lchk;
   size_t   attr_len;
   nc_type  attrTypeId;
   
   /*-----------------------*\
   ! DEFINE CHAR VARIABLES:  !
   \*-----------------------*/
   char    *attr   = NULL,
           *buffer = NULL;

   /*--------------------------------------------------------*\
   ! Get the attribute name, value, type and length in the    !
   ! auxiliary bitstream.                                     !
   \*--------------------------------------------------------*/
   Lchk       =  (uint16)get_symbol(aux, 2);
   buffer     =   (char*)get_chunck(aux, Lchk);

   Lchk       =  (uint16)get_symbol(aux, 2);
   attr       =   (char*)get_chunck(aux, Lchk);

   attrTypeId = (nc_type)get_symbol(aux, sizeof(nc_type));
   attr_len   =  (size_t)get_symbol(aux, sizeof(size_t));

   /*--------------------------------------------------------*\
   ! Write the attribute name to the data file.               !
   \*--------------------------------------------------------*/
   if((status = nc_put_att(ncId, varId, buffer, attrTypeId, attr_len, attr)))
   {
      fprintf(stderr, "%s\n", nc_strerror(status));
      free(buffer);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Free the attribute character array and return to the     !
   ! function caller.                                         !
   \*--------------------------------------------------------*/
   free(buffer);
   free(attr);
   return 0;
}

/**************************************************************************************************\
||             ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____             ||
||             |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__              ||
||             |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]             ||
||                                                                                                ||
\**************************************************************************************************/
/*------------------------------------------------------------------------------------------------*\
!                                                                                                  !
!   DESCRIPTION:                                                                                   !
!   ------------                                                                                   !
!                                                                                                  !
!         This function opens an eas3 file and checks it for its validity. Once the                !
!         specified file has been verified, its header and flow field data is read and             !
!         stored in the bwc_data structure.                                                        !
!                                                                                                  !
!   ARGUMENTS:                                                                                     !
!   ----------                                                                                     !
!                 Name                          Description                                        !
!                 ----                          -----------                                        !
!                 filename                    - Defines the filename of the eas3 file that         !
!                                               is to be opened and read.                          !
!                                                                                                  !
!   RETURN:                                                                                        !
!   -------                                                                                        !
!                 Defines a structure used to store all the relevant parameters and the            !
!                 data field of an eas3 file.                                                      !
!                                                                                                  !
\*------------------------------------------------------------------------------------------------*/
bwc_data*
read_netCDF(char *const filename)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   Lfield;
   uint64   l;
   int      i, j, k;
   int      nVarDims, varDimsId, nVarAttr;
   int      nDims, nUDims;
   int      nVar, nAttr;
   int      ncId;
   int      status;
   uint16   Lchk;
   size_t  *start, *count;
   size_t   len;
   nc_type  varTypeId;

   /*-----------------------*\
   ! DEFINE CHAR VARIABLES:  !
   \*-----------------------*/
   char    *buffer = NULL;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_data          *file;
   bwc_data_inf      *info;
   bwc_stream        *aux;

   /*--------------------------------------------------------*\
   ! Open the specified file for reading. If the file doesn't !
   ! exist, exit the bwc command-line tool.                   !
   \*--------------------------------------------------------*/
   if((status = nc_open(filename, NC_NOWRITE, &ncId)))
   {
      fprintf(stderr, "%s\n", nc_strerror(status));
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! Allocate the data structure.                             !
   \*--------------------------------------------------------*/
   file = calloc(1,  sizeof(bwc_data));
   if(!file)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      nc_close(ncId);
      return NULL;
   }

  /*--------------------------------------------------------*\
  ! Save frequently used variables/structures to temporary   !
  ! variables to make the code more readable.                !
  \*--------------------------------------------------------*/
   info = &file->info;

   /*--------------------------------------------------------*\
   ! Allocate the auxiliary information packed stream.        !
   \*--------------------------------------------------------*/
   file->aux  = calloc(1,  sizeof(bwc_packed_stream));
   if(!file->aux)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      bwc_free_data(file);
      nc_close(ncId);
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! Initialize the stream for the auxiliary information mem- !
   ! ory block. The initial size of the auxiliary memory      !
   ! block has been chosen arbitrarily and should be large    !
   ! enough to prevent excessive reallocation.                !
   \*--------------------------------------------------------*/
   aux = init_stream(NULL, AUX_SIZE, 'c');
   if(!aux)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      bwc_free_data(file);
      nc_close(ncId);
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! Extract the number of dimensions, parameters, attributes !
   ! and "unlimited dimensions" from the specified netCDF     !
   ! file.                                                    !
   \*--------------------------------------------------------*/
   if((status = nc_inq(ncId, &nDims, &nVar, &nAttr, &nUDims)))
   {
      fprintf(stderr, "%s\n", nc_strerror(status));
      bwc_free_data(file);
      nc_close(ncId);
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! Safe the number of variables and global attributes in    !
   ! the auxiliary information memory block.                  !
   \*--------------------------------------------------------*/
   emit_symbol(aux, nVar,  sizeof(int));
   emit_symbol(aux, nAttr, sizeof(int));

   /*--------------------------------------------------------*\
   ! Allocate a start and count array for the dimensions spe- !
   ! cified in the data file.                                 !
   \*--------------------------------------------------------*/
   start = calloc(4, sizeof(size_t));
   count = calloc(4, sizeof(size_t));
   if(!start || !count)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      bwc_free_data(file);
      nc_close(ncId);
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! Check if the number of specified dimensions does not     !
   ! exceed the number of supported dimensions.               !
   \*--------------------------------------------------------*/
   if(nDims > 4)
   {
      // error in number of dimensions
      fprintf(stderr, "o##########################################################o\n"\
                      "| ERROR: Number of dimensions present in the input file is |\n"\
                      "|        larger than the number of supported dimensions.   |\n"\
                      "o##########################################################o\n");
      bwc_free_data(file);
      nc_close(ncId);
      free(start);
      free(count);
      return NULL;
   }
   else
   {
      /*--------------------------------------------------------*\
      ! Allocate a character array used to get strings from the  !
      ! specified data file.                                     !
      \*--------------------------------------------------------*/
      buffer = calloc(NC_MAX_NAME, sizeof(char));
      if(!buffer)
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         bwc_free_data(file);
         nc_close(ncId);
         free(start);
         free(count);
         return NULL;
      }

      for(i = 0; i < nDims; ++i)
      {
         /*--------------------------------------------------------*\
         ! Get the dimension name and length for the 1st dimension. !
         \*--------------------------------------------------------*/
         if((status = nc_inq_dim(ncId, i, buffer, &len)))
         {
            fprintf(stderr, "%s\n", nc_strerror(status));
            bwc_free_data(file);
            nc_close(ncId);
            free(buffer);
            free(start);
            free(count);
            return NULL;
         }

         /*--------------------------------------------------------*\
         ! Safe the dimension name in the auxiliary information mem-!
         ! ory block and reset the buffer character array.          !
         \*--------------------------------------------------------*/
         Lchk = (uint16)strlen(buffer);
         emit_symbol(aux, Lchk,           2);
         emit_chunck(aux, (uchar*)buffer, Lchk);

         memset(buffer, '\0', NC_MAX_NAME);

         /*--------------------------------------------------------*\
         ! Set up the start and count for the current dimension.    !
         \*--------------------------------------------------------*/
         start[i] = 0;
         count[i] = len;
      }

      /*--------------------------------------------------------*\
      ! Free the buffer character array.                         !
      \*--------------------------------------------------------*/
      free(buffer);
   }

   /*--------------------------------------------------------*\
   ! Update the file info structure with the appropriate val- !
   ! ues and calculate the size of a variable.                !
   \*--------------------------------------------------------*/
   info->nX   = (count[0] == 0) ? 1 : count[0];
   info->nY   = (count[1] == 0) ? 1 : count[1];
   info->nZ   = (count[2] == 0) ? 1 : count[2];
   info->nTS  = (count[3] == 0) ? 1 : count[3];

   Lfield     = info->nX * info->nY * info->nZ * info->nTS;

   /*--------------------------------------------------------*\
   ! Reallocate a start and count array for the dimensions to !
   ! fit the number of dimensions specified in the data file. !
   \*--------------------------------------------------------*/
   start = realloc(start, nDims * sizeof(size_t));
   count = realloc(count, nDims * sizeof(size_t));
   if(!start || !count)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      bwc_free_data(file);
      nc_close(ncId);
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! If the bwc_add_param function is called for the first    !
   ! time, allocate the parameter structure and parameter     !
   ! name character array.                                    !
   \*--------------------------------------------------------*/
   info->param_name = calloc(nVar, sizeof(char*));
   if(!info->param_name)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      bwc_free_data(file);
      nc_close(ncId);
      free(start);
      free(count);
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! Get the global attributes from the specified data file   !
   ! and store their values in the auxiliary information      !
   ! blocks.                                                  !
   \*--------------------------------------------------------*/
   for(i = 0; i < nAttr; ++i)
   {
      if(get_attributes(aux, ncId, NC_GLOBAL, i))
      {
         bwc_free_data(file);
         nc_close(ncId);
         free(start);
         free(count);
         return NULL;
      }
   }

   for(i = 0, k = 0; i < nVar; ++i)
   {
      /*--------------------------------------------------------*\
      ! Allocate a character array used to get strings from the  !
      ! specified data file.                                     !
      \*--------------------------------------------------------*/
      buffer = calloc(NC_MAX_NAME, sizeof(uchar));
      if(!buffer)
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         bwc_free_data(file);
         nc_close(ncId);
         free(start);
         free(count);
         return NULL;
      }

      /*--------------------------------------------------------*\
      ! Extract the variable name and type, number of dimensions !
      ! and number of attributes associated with the current var-!
      ! iable id.                                                !
      \*--------------------------------------------------------*/
      if((status = nc_inq_var(ncId, i, buffer, &varTypeId, 
                                               &nVarDims, 
                                               &varDimsId, 
                                               &nVarAttr)))
      {
         fprintf(stderr, "%s\n", nc_strerror(status));
         bwc_free_data(file);
         nc_close(ncId);
         free(start);
         free(count);
         return NULL;
      }

      /*--------------------------------------------------------*\
      ! Store the number of dimensions and attributes as well as !
      ! the variable name and type for the current variable id   !
      ! in the aux bitstream.                                    !
      \*--------------------------------------------------------*/
      Lchk = (uint16)strlen(buffer);
      emit_symbol(aux, Lchk,           2);
      emit_chunck(aux, (uchar*)buffer, Lchk);

      emit_symbol(aux, varTypeId, sizeof(nc_type));
      emit_symbol(aux, nVarDims,  sizeof(int));
      emit_symbol(aux, nVarAttr,  sizeof(int));

      if((varTypeId == NC_DOUBLE) && (nDims == nVarDims))
      {
         /*--------------------------------------------------------*\
         ! Reallocate the character array to the required string    !
         ! size.                                                    !
         \*--------------------------------------------------------*/
         info->param_name[k] = realloc(buffer, 
                               (strlen(buffer) + 1) * sizeof(char));

         file->field = realloc(file->field, (k + 1) * Lfield * sizeof(bwc_float));

         if((status = nc_get_vara_double(ncId, i, start, count, file->field + (k * Lfield))))
         {
            fprintf(stderr, "%s\n", nc_strerror(status));
            bwc_free_data(file);
            nc_close(ncId);
            free(start);
            free(count);
            return NULL;
         }

         k++;
      }
      else
      {
         free(buffer);
      }
      

      /*--------------------------------------------------------*\
      ! Get the variable attributes from the specified data file !
      ! and store their values in the auxiliary information      !
      ! block.                                                   !
      \*--------------------------------------------------------*/
      for(j = 0; j < nVarAttr; ++j)
      {
         if(get_attributes(aux, ncId, i, j))
         {
            bwc_free_data(file);
            nc_close(ncId);
            free(start);
            free(count);
            return NULL;
         }
      }      
   }

   /*--------------------------------------------------------*\
   ! Update the file info structure with the appropriate val- !
   ! ues.                                                     !
   \*--------------------------------------------------------*/
   info->nPar = k;

   /*--------------------------------------------------------*\
   ! Check for NaN cases.                                     !
   \*--------------------------------------------------------*/
   for(l = 0; l < (k * Lfield); ++l)
   {
      if(isnan(file->field[l]))
      {
         file->field[l] = 0.f;
      }
   }
   /*--------------------------------------------------------*\
   ! Terminate the auxiliary information stream. If success-  !
   ! ful, the address to the aux memory block stored is       !
   ! stored in the file structure alongside its size.         !
   \*--------------------------------------------------------*/
   if(terminate_stream(aux, file->aux) == FAILURE)
   {
      bwc_free_data(file);
      nc_close(ncId);
      free(start);
      free(count);
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! Close the netCDF file and return the bwc_data structure  !
   ! to the function caller.                                  !
   \*--------------------------------------------------------*/
   nc_close(ncId);
   free(start);
   free(count);
   return file;
}

/*------------------------------------------------------------------------------------------------*\
!                                                                                                  !
!   DESCRIPTION:                                                                                   !
!   ------------                                                                                   !
!                                                                                                  !
!         This function creates a valid eas3 file from the information stored in the               !
!         bwc_data structure.                                                                      !
!                                                                                                  !
!   ARGUMENTS:                                                                                     !
!   ----------                                                                                     !
!                 Name                          Description                                        !
!                 ----                          -----------                                        !
!                 filename                    - Defines the filename of the eas3 file that         !
!                                               is to be opened and read.                          !
!                                                                                                  !
!                 file                        - Defines a structure used to store all the          !
!                                               relevant parameters and the data field of          !
!                                               an eas3 file.                                      !
!                                                                                                  !
!   RETURN:                                                                                        !
!   -------                                                                                        !
!                 Returns an unsigned char for error handling.                                     !
!                                                                                                  !
\*------------------------------------------------------------------------------------------------*/
uchar
write_netCDF(bwc_data *const file, char *const filename)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   Lfield;
   int      i, j, k;
   int      nVarDims, nVarAttr;
   int      nDims;
   int      nVar, nAttr;
   int      ncId, *dimId;
   int      varId, *varIdArr;
   int      status;
   uint16   Lchk;
   size_t  *start, *count;
   nc_type  varTypeId;

   /*-----------------------*\
   ! DEFINE CHAR VARIABLES:  !
   \*-----------------------*/
   char    *buffer = NULL;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_data_inf      *info;
   bwc_stream        *aux;

  /*--------------------------------------------------------*\
  ! Save frequently used variables/structures to temporary   !
  ! variables to make the code more readable.                !
  \*--------------------------------------------------------*/
   info = &file->info;

   /*--------------------------------------------------------*\
   ! Open the specified file for reading. If the file doesn't !
   ! exist, exit the bwc command-line tool.                   !
   \*--------------------------------------------------------*/
   if((status = nc_create(filename, NC_CLOBBER, &ncId)))
   {
      fprintf(stderr, "%s\n", nc_strerror(status));
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Initialize the auxiliary information stream.             !
   \*--------------------------------------------------------*/
   aux = init_stream(file->aux->memory, file->aux->size, 'd');

   /*--------------------------------------------------------*\
   ! Get the number of variables and global attributes in the !
   ! auxiliary information memory block.                      !
   \*--------------------------------------------------------*/
   nVar  = (int)get_symbol(aux, sizeof(int));
   nAttr = (int)get_symbol(aux, sizeof(int));

   /*--------------------------------------------------------*\
   ! Calculate the size of the data field used for write oper-!
   ! ations.                                                  !
   \*--------------------------------------------------------*/
   Lfield = info->nX  * info->nY * 
            info->nZ  * info->nTS;

   /*--------------------------------------------------------*\
   ! Allocate a start and count array for the dimensions spe- !
   ! cified in the data file.                                 !
   \*--------------------------------------------------------*/
   start = calloc(4, sizeof(size_t));
   count = calloc(4, sizeof(size_t));
   if(!start || !count)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      bwc_free_data(file);
      nc_close(ncId);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Update the file info structure with the appropriate val- !
   ! ues and calculate the size of a variable.                !
   \*--------------------------------------------------------*/
   count[0] = (info->nX  >> 1) ? info->nX  : 0;
   count[1] = (info->nY  >> 1) ? info->nY  : 0;
   count[2] = (info->nZ  >> 1) ? info->nZ  : 0;
   count[3] = (info->nTS >> 1) ? info->nTS : 0;

   /*--------------------------------------------------------*\
   ! Calculate the number of dimensions that are present in   !
   ! the compressed codestream.                               !
   \*--------------------------------------------------------*/
   nDims = (count[0] ? 1 : 0) +
           (count[1] ? 1 : 0) +
           (count[2] ? 1 : 0) +
           (count[3] ? 1 : 0);

   /*--------------------------------------------------------*\
   ! Allocate a dimension id array.                           !
   \*--------------------------------------------------------*/
   dimId = calloc(nDims, sizeof(int));
   if(!dimId)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      bwc_free_data(file);
      nc_close(ncId);
      free(start);
      free(count);
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Reallocate a start and count array for the dimensions to !
   ! fit the number of dimensions specified in the data file. !
   \*--------------------------------------------------------*/
   start = realloc(start, nDims * sizeof(size_t));
   count = realloc(count, nDims * sizeof(size_t));
   if(!start || !count)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      bwc_free_data(file);
      nc_close(ncId);
      free(dimId);
      return 1;
   }

   for(i = 0; i < nDims; ++i)
   {
      /*--------------------------------------------------------*\
      ! Get the dimension name from the auxiliary information    !
      ! memory block.                                            !
      \*--------------------------------------------------------*/
      Lchk   = (uint16)get_symbol(aux, 2);
      buffer =  (char*)get_chunck(aux, Lchk);
      if(!buffer)
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         bwc_free_data(file);
         nc_close(ncId);
         free(start);
         free(count);
         free(dimId);
         return 1;
      }

      /*--------------------------------------------------------*\
      ! Define the dimension in the netCDF dataset.              !
      \*--------------------------------------------------------*/
      if((status = nc_def_dim(ncId, buffer, count[i], &dimId[i])))
      {
         fprintf(stderr, "%s\n", nc_strerror(status));
         bwc_free_data(file);
         nc_close(ncId);
         free(buffer);
         free(start);
         free(count);
         free(dimId);
         return 1;
      }

      /*--------------------------------------------------------*\
      ! Free the buffer character array.                         !
      \*--------------------------------------------------------*/
      free(buffer);
   }

   /*--------------------------------------------------------*\
   ! Get the global attributes from the specified data file   !
   ! and store their values in the auxiliary information      !
   ! blocks.                                                  !
   \*--------------------------------------------------------*/
   for(i = 0; i < nAttr; ++i)
   {
      if(put_attributes(aux, ncId, NC_GLOBAL))
      {
         bwc_free_data(file);
         nc_close(ncId);
         free(buffer);
         free(start);
         free(count);
         free(dimId);
         return 1;
      }
   }

   /*--------------------------------------------------------*\
   ! Allocate a dimension id array.                           !
   \*--------------------------------------------------------*/
   varIdArr = calloc(nVar, sizeof(int));
   if(!varIdArr)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      bwc_free_data(file);
      nc_close(ncId);
      free(start);
      free(count);
      free(dimId);
      return 1;
   }

   for(i = 0, k = 0; i < nVar; ++i)
   {
      /*--------------------------------------------------------*\
      ! Get the number of dimensions and attributes as well as   !
      ! the variable name and type for the current variable id   !
      ! in the aux bitstream.                                    !
      \*--------------------------------------------------------*/
      Lchk      =  (uint16)get_symbol(aux, 2);
      buffer    =   (char*)get_chunck(aux, Lchk);
      if(!buffer)
      {
         // memory allocation error
         fprintf(stderr, MEMERROR);
         bwc_free_data(file);
         nc_close(ncId);
         free(varIdArr);
         free(start);
         free(count);
         free(dimId);
         return 1;
      }

      varTypeId = (nc_type)get_symbol(aux, sizeof(nc_type));
      nVarDims  =     (int)get_symbol(aux, sizeof(int));
      nVarAttr  =     (int)get_symbol(aux, sizeof(int));

      /*--------------------------------------------------------*\
      ! Define the variable in the netCDF dataset.               !
      \*--------------------------------------------------------*/
      if((status = nc_def_var(ncId, buffer, varTypeId, 
                                            nVarDims, 
                                            dimId, 
                                            &varId)))
      {
         fprintf(stderr, "%s\n", nc_strerror(status));
         bwc_free_data(file);
         nc_close(ncId);
         free(varIdArr);
         free(start);
         free(count);
         free(dimId);
         return 1;
      }

      /*--------------------------------------------------------*\
      ! Put the variable attributes from the specified data file !
      ! and store their values in the auxiliary information      !
      ! block.                                                   !
      \*--------------------------------------------------------*/
      for(j = 0; j < nVarAttr; ++j)
      {
         if(put_attributes(aux, ncId, varId))
         {
            bwc_free_data(file);
            nc_close(ncId);
            free(varIdArr);
            free(start);
            free(count);
            free(dimId);
            return 1;
         }
      }

      if((varTypeId == NC_DOUBLE) && (nDims == nVarDims))
      {
         varIdArr[k] = varId;
         k++;
      }

      free(buffer);
   }

   /*--------------------------------------------------------*\
   ! Deallocate a dimension id array.                         !
   \*--------------------------------------------------------*/
   free(dimId);

   /*--------------------------------------------------------*\
   ! Leave define mode to create the variable.                !
   \*--------------------------------------------------------*/
   if((status = nc_enddef(ncId)))
   {
      fprintf(stderr, "%s\n", nc_strerror(status));
      bwc_free_data(file);
      nc_close(ncId);
      free(varIdArr);
      free(start);
      free(count);
      free(dimId);
      return 1;
   }

   for(i = 0; i < k; ++i)
   {
      /*--------------------------------------------------------*\
      ! Define the variable in the netCDF dataset.               !
      \*--------------------------------------------------------*/
      if((status = nc_put_vara(ncId, varIdArr[i], start, count, file->field + (i * Lfield))))
      {
         fprintf(stderr, "%s\n", nc_strerror(status));
         bwc_free_data(file);
         nc_close(ncId);
         free(start);
         free(count);
         free(dimId);
         return 1;
      }
   }

   /*--------------------------------------------------------*\
   ! Free the auxiliary information stream and variable id    !
   ! array.                                                   !
   \*--------------------------------------------------------*/
   free(varIdArr);
   free(aux);

   /*--------------------------------------------------------*\
   ! Close the netCDF file and return the bwc_data structure  !
   ! to the function caller.                                  !
   \*--------------------------------------------------------*/
   nc_close(ncId);
   free(start);
   free(count);
   return 0;
}