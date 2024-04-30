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
||        This file describes a set of function that can be used to create and manipulate         ||
||        a BigWhoop Codestream. They facilitate the assembly and parsing of the main             ||
||        header and tile bitsreams as well as read and write functions used to access            ||
||        conforming bwc datasets.                                                                ||
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

#include "libbwc.h"
#include "bitstream.h"
#include "codestream.h"
#include "tier2.h"

/************************************************************************************************************\
||                ___  ____ _ _  _ ____ ___ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                ||
||                |__] |__/ | |  | |__|  |  |___    |___ |  | |\ | |     |  | |  | |\ | [__                 ||
||                |    |  \ |  \/  |  |  |  |___    |    |__| | \| |___  |  | |__| | \| ___]                ||
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
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                -                                                                                           !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                -           Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uchar
can_read(bitstream *const stream, const uint64 length)
{
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(stream);
   assert(length);
   
   return (stream->L + length > stream->Lmax) ? 0 : 1;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void codestream_write_header(bitstream *const stream, bwc_field *const field)             !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is writes the main header, including the end of header (EOH) marker seg-      !
!                ment to the bwc codestream.                                                                 !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                Variable                    Type                    Description                             !
!                --------                    ----                    -----------                             !
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                stream                      bitstream*           - Structure used to assemble a bwc bit-    !
!                                                                    bitstream.                              !
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
!                12.07.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
codestream_write_header(bitstream *const stream, bwc_field *const field)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint32             t;
   uint16             Leoh;
   uint8              p;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;
   bwc_gl_inf        *info;
   
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(stream);
   assert(field);

   /*--------------------------------------------------------*\
   ! Save the global as well as the subband control and info  !
   ! structure to temporary variables to make the code more   !
   ! readable.                                                !
   \*--------------------------------------------------------*/
   control = &field->control;
   info    =  field->info;

   /*--------------------------------------------------------*\
   ! Calculate the size of the end of header (EOH) maker seg- !
   ! ment - excluding the EOH marker.                         !
   \*--------------------------------------------------------*/
   Leoh = 2 + (control->nTiles * info->nPar * 2 * PREC_BYTE); 

   /*--------------------------------------------------------*\
   ! Emit the portion of the main header already created by   !
   ! the bwc_create_compression function.                     !
   \*--------------------------------------------------------*/
   emit_chunck(stream, control->header.memory, control->header.size);

   /*--------------------------------------------------------*\
   ! Emit the end of header (EOH) marker and EOH marker seg-  !
   ! ment size Leoh.                                          !
   \*--------------------------------------------------------*/
   emit_symbol(stream, EOH,  2);
   emit_symbol(stream, Leoh, 2);

   /*--------------------------------------------------------*\
   ! Loop through all tile parameters and...                  !
   \*--------------------------------------------------------*/
   for(t = 0; t < control->nTiles; ++t)
   {
      for(p = 0; p < info->nPar; ++p)
      {
         /*--------------------------------------------------------*\
         ! ...emit the maximum and minimum parameter value to the   !
         ! header stream.                                           !
         \*--------------------------------------------------------*/
         emit_symbol(stream, *(uint64 *)&field->tile[t].parameter[p].info.parameter_min, PREC_BYTE);
         emit_symbol(stream, *(uint64 *)&field->tile[t].parameter[p].info.parameter_max, PREC_BYTE);

         /*--------------------------------------------------------*\
         ! Reset the maximum and minimum parameter value in the     !
         ! parameter structure.                                     !
         \*--------------------------------------------------------*/
         field->tile[t].parameter[p].info.parameter_max  = -FLT_MAX;
         field->tile[t].parameter[p].info.parameter_min  =  FLT_MAX;
      }
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uchar sequence_packets(bwc_field *const field, bwc_field *const field)                    !
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
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                tile                        bwc_tile*             - Structure defining a bwc tile.          !
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
!                13.06.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uchar
sequence_packets(bwc_field *const field, bwc_tile *const tile)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   tile_size, idx;
   uint32   j, p;
   int32    r;
   uint8    l;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;
   bwc_gl_inf        *info;
   bwc_resolution    *resolution;
   bwc_packet       **packet_sequence;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(tile);

   /*--------------------------------------------------------*\
   ! Save the global control and info as well as the packet   !
   ! sequence structure to temporary variables to make the    !
   ! code more readable.                                      !
   \*--------------------------------------------------------*/
   control = &field->control;
   info    =  field->info;

   packet_sequence = tile->packet_sequence;

   switch(control->progression)
   {
      case bwc_prog_LRCP:
      {
         for(l = 0, tile_size = 0, idx = 0; l < control->nLayers; ++l)
         {
            for(r = 0; r < control->nDecomp + 1; ++r)
            {
               for(j = 0; j < info->nPar; ++j)
               {
                  resolution = &tile->parameter[j].resolution[r];

                  for(p = 0; p < resolution->control.number_of_precincts; ++p, ++idx)
                  {
                     packet_sequence[idx]     = &resolution->packet[l * resolution->control.number_of_precincts + p];
                     tile_size               +=  resolution->packet[l * resolution->control.number_of_precincts + p].size;

                     if(control->error_resilience)
                     {
                        tile_size         += 6;
                     }
                  }
               }      
            }
         }
         break;
      }
      case bwc_prog_RLCP:
      {
         for(r = 0, tile_size = 0, idx = 0; r < control->nDecomp + 1; ++r)
         {
            for(l = 0; l < control->nLayers; ++l)
            {
               for(j = 0; j < info->nPar; ++j)
               {
                  resolution = &tile->parameter[j].resolution[r];

                  for(p = 0; p < resolution->control.number_of_precincts; ++p, ++idx)
                  {
                     packet_sequence[idx]     = &resolution->packet[l * resolution->control.number_of_precincts + p];
                     tile_size               +=  resolution->packet[l * resolution->control.number_of_precincts + p].size;

                     if(control->error_resilience)
                     {
                        tile_size         += 6;
                     }
                  }
               }      
            }
         }
         break;
      }
      case bwc_prog_RPCL:
      {
         for(l = 0, tile_size = 0, idx = 0; l < control->nLayers; ++l)
         {
            for(r = 0; r < control->nDecomp + 1; ++r)
            {
               for(j = 0; j < info->nPar; ++j)
               {
                  resolution = &tile->parameter[j].resolution[r];

                  for(p = 0; p < resolution->control.number_of_precincts; ++p, ++idx)
                  {
                     packet_sequence[idx]     = &resolution->packet[l * resolution->control.number_of_precincts + p];
                     tile_size               +=  resolution->packet[l * resolution->control.number_of_precincts + p].size;

                     if(control->error_resilience)
                     {
                        tile_size         += 6;
                     }
                  }
               }      
            }
         }
         break;
      }
      case bwc_prog_PCRL:
      {
         for(l = 0, tile_size = 0, idx = 0; l < control->nLayers; ++l)
         {
            for(r = 0; r < control->nDecomp + 1; ++r)
            {
               for(j = 0; j < info->nPar; ++j)
               {
                  resolution = &tile->parameter[j].resolution[r];

                  for(p = 0; p < resolution->control.number_of_precincts; ++p, ++idx)
                  {
                     packet_sequence[idx]     = &resolution->packet[l * resolution->control.number_of_precincts + p];
                     tile_size               +=  resolution->packet[l * resolution->control.number_of_precincts + p].size;

                     if(control->error_resilience)
                     {
                        tile_size         += 6;
                     }
                  }
               }      
            }
         }
         break;
      }
      case bwc_prog_CPRL:
      {
         for(l = 0, tile_size = 0, idx = 0; l < control->nLayers; ++l)
         {
            for(r = 0; r < control->nDecomp + 1; ++r)
            {
               for(j = 0; j < info->nPar; ++j)
               {
                  resolution = &tile->parameter[j].resolution[r];

                  for(p = 0; p < resolution->control.number_of_precincts; ++p, ++idx)
                  {
                     packet_sequence[idx]     = &resolution->packet[l * resolution->control.number_of_precincts + p];
                     tile_size               +=  resolution->packet[l * resolution->control.number_of_precincts + p].size;

                     if(control->error_resilience)
                     {
                        tile_size         += 6;
                     }
                  }
               }      
            }
         }
         break;
      }
      default:
      {
         return 1;
      }
   }

   tile->control.body_size = tile_size;
   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uchar sequence_packets(bwc_field *const field, bwc_field *const field)                    !
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
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                tile                        bwc_tile*             - Structure defining a bwc tile.          !
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
!                13.06.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
assemble_tile(bwc_field *const field, bwc_tile *const tile, bitstream *const stream)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   packet_index;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;
   bwc_packet        *packet;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(tile);
   assert(stream);

   /*--------------------------------------------------------*\
   ! Save the global control and info as well as the packet   !
   ! sequence structure to temporary variables to make the    !
   ! code more readable.                                      !
   \*--------------------------------------------------------*/
   control = &field->control;

   emit_symbol(stream, SOT,                        2);
   emit_symbol(stream, 14,                         2);
   emit_symbol(stream, tile->info.tile_index,      4);
   emit_symbol(stream, tile->control.body_size,    8);
   emit_symbol(stream, SOD,                        2);

   for(packet_index = 0; packet_index < tile->control.nPackets; ++packet_index)
   {
      packet = tile->packet_sequence[packet_index];

      if(control->error_resilience)
      {
         emit_symbol(stream, SOP,                        2);
         emit_symbol(stream, 4,                          2);
         emit_symbol(stream, packet_index % 0x100000000, 4);
      }

      if(packet->header.memory)
      {
         emit_chunck(stream, packet->header.memory, packet->header.size);

         if(control->error_resilience)
         {
            emit_symbol(stream, EPH, 2);
         }

         release_packed_stream(&packet->header);
      }

      if(packet->body.memory)
      {
         emit_chunck(stream, packet->body.memory, packet->body.size);

         release_packed_stream(&packet->body);
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
bwc_field*
parse_main_header(bwc_data *const data,bitstream *const stream)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64      buff_long, buffX, buffY, buffZ, buffTS;
   uint64      nX, nY, nZ;
   uint32      buff;
   uint32      bitrate;
   uint32      Lsax;
   uint32      mantissa, exponent;
   uint32      t;
   uint16      CSsgc;
   uint16      Linf, Lctr, Lcom, Leoh, Lunk;
   uint16      marker;
   uint16      nTS;
   uint8       dim;
   uint8       index, l;
   uint8       samp;
   uint8       nPar, p;
   uint8       codec_prec, precision;

   /*-----------------------*\
   ! DEFINE CHAR VARIABLES:  !
   \*-----------------------*/
   char*       buffer_char;
   char        status;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_field         *field;
   bwc_gl_ctrl       *control;
   bwc_gl_inf        *info;
   
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(data);
   assert(stream);
   
   /*--------------------------------------------------------*\
   ! Save the data info structure to a temporary variable to  !
   ! make the code more readable.                             !
   \*--------------------------------------------------------*/
   info = &data->info;

   status = CODESTREAM_OK;
   index  = 0;

   while(!(status & CODESTREAM_ERROR) && can_read(stream, 2))
   {
      marker = (uint16)get_symbol(stream, 2);
      if(marker < 0xFF00)
      {
         // Invalid Codestream
         fprintf(stderr, CSERROR);
         status |= CODESTREAM_ERROR;
      }

      switch(marker)
      {
         case SOC:
         {
            if(index != 0)
            {
               // Invalid Codestream
               fprintf(stderr, CSERROR);
               status |= CODESTREAM_ERROR;
            }
            break;
         }

         case SGI:
         {
            if(index != 1 && !can_read(stream, 2))
            {
               // Invalid Codestream
               fprintf(stderr, CSERROR);
               status |= CODESTREAM_ERROR;
               break;
            }

            Linf = (uint16)get_symbol(stream, 2);
            if(!can_read(stream, Linf - 2))
            {
               // Invalid Codestream
               fprintf(stderr, CSERROR);
               status |= CODESTREAM_ERROR;
               break;
            }

            info->nX        = nX         =         get_symbol(stream,  8);
            info->nY        = nY         =         get_symbol(stream,  8);
            info->nZ        = nZ         =         get_symbol(stream,  8);
            info->nTS       = nTS        = (uint16)get_symbol(stream,  2);
            info->nPar      = nPar       =  (uint8)get_symbol(stream,  1);
            info->precision = codec_prec =  (uint8)get_symbol(stream,  1);

            buffer_char       =  (char*)get_chunck(stream, 10);
            strncpy(info->f_ext, buffer_char, sizeof(buffer_char)/sizeof(*buffer_char));
            free(buffer_char);

            for(p = 0; p < nPar; ++p)
            {
               buffer_char = (char*)get_chunck(stream, 24);
               precision   = (uint8)get_symbol(stream, 1);

               bwc_add_param(data, buffer_char, precision);

               free(buffer_char);
            }

            field = bwc_initialize_field(data);
            if(!field)
            {
               status |= CODESTREAM_ERROR;
               break;
            }

            /*--------------------------------------------------------*\
            ! Save the global control and info structure to temporary  !
            ! variables to make the code more readable.                !
            \*--------------------------------------------------------*/
            info    =  field->info = &data->info;
            control = &field->control;

            status |= CODESTREAM_SGI_READ;
            break;
         }

         case SGC:
         {
            if(index != 2 && !can_read(stream, 2))
            {
               // Invalid Codestream
               fprintf(stderr, CSERROR);
               bwc_kill_compression(field);
               status |= CODESTREAM_ERROR;
               break;
            }

            Lctr  = (uint16)get_symbol(stream, 2);

            if(!can_read(stream, Lctr - 2))
            {
               // Invalid Codestream
               fprintf(stderr, CSERROR);
               bwc_kill_compression(field);
               status |= CODESTREAM_ERROR;
               break;
            }

            CSsgc = (uint16)get_symbol(stream, 2);


            buff_long  = get_symbol(stream, 1);
            if(CSsgc & (0x01 << 0))
            {
               bwc_set_error_resilience(field);
            }

            buff_long = get_symbol(stream, 1);

            if(CSsgc & (0x01 << 1))
            {
               set_quant_style(field, (bwc_quant_st)buff_long);
            }

            buff_long = get_symbol(stream, 1);

            exponent = (uint32)get_symbol(stream, 1);
            mantissa = (uint32)get_symbol(stream, 2);
            if(CSsgc & (0x01 << 2))
            {
               control->qt_exponent = exponent;
               control->qt_mantissa = mantissa;
            }

            buff_long  = get_symbol(stream, 1);
            if(CSsgc & (0x01 << 3))
            {
               set_progression(field, (uint8)buff_long);
            }

            buff_long  = get_symbol(stream, 1);
            if(CSsgc & (0x01 << 4))
            {
               set_kernels(field, (uint8)(0x03 & (buff_long >> 6)), (uint8)(0x03 & (buff_long >> 4)),
                                  (uint8)(0x03 & (buff_long >> 2)), (uint8)(0x03 &  buff_long));
            }

            buff_long  = get_symbol(stream, 4);
            if(CSsgc & (0x01 << 5))
            {
               bwc_set_decomp(field, (uint8)(0xFF & (buff_long >> 24)), (uint8)(0xFF & (buff_long >>  16)),
                                     (uint8)(0xFF & (buff_long >>  8)), (uint8)(0xFF &  buff_long));
            }
            
            buff_long  = get_symbol(stream, 2);
            if(CSsgc & (0x01 << 6))
            {
               bwc_set_precincts(field, (uint8)(0x0F & (buff_long >> 8)), (uint8)(0x0F & (buff_long >> 12)),
                                        (uint8)(0x0F &  buff_long),       (uint8)(0x0F & (buff_long >>  4)));
            }

            buff_long  = get_symbol(stream, 4);
            if(CSsgc & (0x01 << 7))
            {
               bwc_set_codeblocks(field, (uint8)(0xFF & (buff_long >> 24)), (uint8)(0xFF & (buff_long >> 16)),
                                         (uint8)(0xFF & (buff_long >>  8)), (uint8)(0xFF &  buff_long));
            }

            buff_long  = get_symbol(stream, 1);
            if(CSsgc & (0x01 << 8))
            {
               bwc_set_qm(field, (uint8)buff_long);
            }

            buffX  = get_symbol(stream, 8);
            buffY  = get_symbol(stream, 8);
            buffZ  = get_symbol(stream, 8);
            buffTS = get_symbol(stream, 2);
            if(CSsgc & (0x01 << 9))
            {
               bwc_set_tiles(field, buffX, buffY, buffZ, (uint16)buffTS, bwc_tile_sizeof);
            }

            control->nLayers = get_symbol(stream, 1);
            control->bitrate = calloc(control->nLayers, sizeof(float));
            if(!control->bitrate)
            {
               // memory allocation error
               fprintf(stderr, MEMERROR);
               bwc_kill_compression(field);
               status|= CODESTREAM_ERROR;
               break;
            }

            for(l = 0; l < control->nLayers; ++l)
            {
               bitrate = (uint32)get_symbol(stream, 4);
               control->bitrate[l] = *(float *)&bitrate;
            }

            create_field(field);
            if(!field)
            {
               bwc_kill_compression(field);
               status |= CODESTREAM_ERROR;
               break;
            }

            status |= CODESTREAM_SGC_READ;
            break;
         }

         case SAX:
         {
            if(index <= 2 && !can_read(stream, 4))
            {
               // Invalid Codestream
               fprintf(stderr, CSERROR);
               bwc_kill_compression(field);
               status |= CODESTREAM_ERROR;
               break;
            }

            Lsax  = (uint32)get_symbol(stream, 4);
            if(!can_read(stream, Lsax - 4))
            {
               // Invalid Codestream
               fprintf(stderr, CSERROR);
               bwc_kill_compression(field);
               status |= CODESTREAM_ERROR;
               break;
            }

            data->codestream.aux = calloc(1, sizeof(bwc_stream));
            if(!data->codestream.aux)
            {
               // memory allocation error
               fprintf(stderr, MEMERROR);
               bwc_kill_compression(field);
               status |= CODESTREAM_ERROR;
               break;
            }

            data->codestream.aux->memory = get_chunck(stream, Lsax - 4);
            data->codestream.aux->size   = Lsax - 4;

            status |= CODESTREAM_SAX_READ;
            break;
         }

         case COM:
         {
            if(index <= 2 && !can_read(stream, 2))
            {
               // Invalid Codestream
               fprintf(stderr, CSERROR);
               bwc_kill_compression(field);
               status |= CODESTREAM_ERROR;
               break;
            }

            Lcom = (uint16)get_symbol(stream, 2);
            if(!can_read(stream, Lcom - 2))
            {
               // Invalid Codestream
               fprintf(stderr, CSERROR);
               bwc_kill_compression(field);
               status |= CODESTREAM_ERROR;
               break;
            }

            data->codestream.com = calloc(1, sizeof(bwc_stream));
            if(!data->codestream.com)
            {
               // memory allocation error
               fprintf(stderr, MEMERROR);
               bwc_kill_compression(field);
               status |= CODESTREAM_ERROR;
               break;
            }

            data->codestream.com->memory = get_chunck(stream, Lcom - 2);
            data->codestream.com->size   = Lcom -2;

            status |= CODESTREAM_ERROR;
            break;
         }

         case EOH:
         {
            if((status & CODESTREAM_SGI_READ) && (status & CODESTREAM_SGC_READ))
            {
               if(index <= 2 && !can_read(stream, 2))
               {
                  // Invalid Codestream
                  fprintf(stderr, CSERROR);
                  bwc_kill_compression(field);
                  status |= CODESTREAM_ERROR;
                  break;
               }

               Leoh = (uint16)get_symbol(stream, 2);
               if(!can_read(stream, Leoh - 2))
               {
                  // Invalid Codestream
                  fprintf(stderr, CSERROR);
                  bwc_kill_compression(field);
                  status |= CODESTREAM_ERROR;
                  break;
               }

               if(codec_prec == 8)
               {
                  /*--------------------------------------------------------*\
                  ! Loop through all tile parameters and...                  !
                  \*--------------------------------------------------------*/
                  for(t = 0; t < control->nTiles; ++t)
                  {
                     for(p = 0; p < info->nPar; ++p)
                     {
                        /*--------------------------------------------------------*\
                        ! ...get the maximum and minimum parameter value to the    !
                        ! header stream.                                           !
                        \*--------------------------------------------------------*/
                        buff_long = get_symbol(stream, sizeof(double));
                        field->tile[t].parameter[p].info.parameter_min = (bwc_float)*(double*)&buff_long;

                        buff_long = get_symbol(stream, sizeof(double));
                        field->tile[t].parameter[p].info.parameter_max = (bwc_float)*(double*)&buff_long;
                     }
                  }
               }
               else if(codec_prec == 4)
               {
                  /*--------------------------------------------------------*\
                  ! Loop through all tile parameters and...                  !
                  \*--------------------------------------------------------*/
                  for(t = 0; t < control->nTiles; ++t)
                  {
                     for(p = 0; p < info->nPar; ++p)
                     {
                        /*--------------------------------------------------------*\
                        ! ...get the maximum and minimum parameter value to the    !
                        ! header stream.                                           !
                        \*--------------------------------------------------------*/
                        buff = get_symbol(stream, sizeof(float));
                        field->tile[t].parameter[p].info.parameter_min = (bwc_float)*(float*)&buff;

                        buff = get_symbol(stream, sizeof(float));
                        field->tile[t].parameter[p].info.parameter_max = (bwc_float)*(float*)&buff;
                     }
                  }
               }
               return field;
            }
            else
            {
               // Invalid Codestream
               fprintf(stderr, CSERROR);
               bwc_kill_compression(field);
               status |= CODESTREAM_ERROR;
            }
            break;
         }

         default:
         {
            if(!can_read(stream, 2))
            {
               // Invalid Codestream
               fprintf(stderr, CSERROR);
               bwc_kill_compression(field);
               status |= CODESTREAM_ERROR;
               break;
            }

            Lunk  = (uint16)get_symbol(stream, 2);
            if(!can_read(stream, Lcom - 2))
            {
               // Invalid Codestream
               fprintf(stderr, CSERROR);
               bwc_kill_compression(field);
               status |= CODESTREAM_ERROR;
               break;
            }
            
            stream->L += Lunk - 2;
            break;
         }
      }
      index++;
   }
   return NULL;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uchar sequence_packets(bwc_field *const field, bwc_field *const field)                    !
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
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                tile                        bwc_tile*             - Structure defining a bwc tile.          !
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
!                13.06.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uchar
parse_tile(bwc_field *const field, bitstream *const stream)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   body_size;
   uint64   packet_index;
   uint64   buf;
   uint32   L;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;
   bwc_packet        *packet;
   bwc_tile          *tile;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(stream);

   /*--------------------------------------------------------*\
   ! Save the global control and info as well as the packet   !
   ! sequence structure to temporary variables to make the    !
   ! code more readable.                                      !
   \*--------------------------------------------------------*/
   control = &field->control;

   if(!can_read(stream, 2))
   {
      // Invalid Codestream
      fprintf(stderr, CSERROR);
      return 1;
   }

   L = (uint16)get_symbol(stream, 2);

   if(!can_read(stream, L - 2))
   {
      // Invalid Codestream
      fprintf(stderr, CSERROR);
      return 1;
   }

   buf = (uint32)get_symbol(stream, 4);
   if(buf >= control->nTiles)
   {
      // Invalid Codestream
      fprintf(stderr, CSERROR);
      return 1;
   }

   tile                    = &field->tile[buf];
   tile->control.body_size = 
                 body_size = (uint64)get_symbol(stream, 8);
   
   if(SOD != (uint16)get_symbol(stream, 2))
   {
      // Invalid Codestream
      fprintf(stderr, CSERROR);
      return 1;
   }
   else
   {
      /*--------------------------------------------------------*\
      ! Sequence the packets according to the user specified op- !
      ! tion.                                                    !
      \*--------------------------------------------------------*/
      if(sequence_packets(field, tile))
      {
         return 1;
      }

      packet_index = 0;

      while((body_size != 0) && can_read(stream, body_size) && packet_index < tile->control.nPackets)
      {
         if(SOP == (uint16)get_symbol(stream, 2))
         {
            if(!can_read(stream, 6))
            {
               //Invalid Codestream
               fprintf(stderr, CSERROR);
               return 1;
            }

            if(4 != (uint16)get_symbol(stream, 2))
            {
               //Invalid Codestream
               fprintf(stderr, CSERROR);
               return 1;
            }

            buf = (uint32)get_symbol(stream, 4);

            body_size -= 8;
         }
         else
         {
            rewind_stream(stream, 2);
         }

         packet = tile->packet_sequence[packet_index++];

         packet->header.memory = get_access(stream);

         if(parse_packet(field, tile, packet, body_size))
         {
            return 1;
         }

         forward_stream(stream, packet->size);

         body_size -= packet->size;
      }
   }
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
!                -           Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uchar
parse_body(bwc_field *const field, bitstream *const stream)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint16             Lunk;
   uint16             marker;

   /*-----------------------*\
   ! DEFINE CHAR VARIABLES:  !
   \*-----------------------*/
   char        status;
   
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   
   status = CODESTREAM_OK;

   while(!(status & CODESTREAM_ERROR) && can_read(stream, 2))
   {
      marker = (uint16)get_symbol(stream, 2);
      if(marker < 0xFF00)
      {
         // Invalid Codestream
         fprintf(stderr, CSERROR);
         status |= CODESTREAM_ERROR;
      }

      switch(marker)
      {
         case SOT:
         {
            if(parse_tile(field, stream))
            {
               status |= CODESTREAM_ERROR;
            }
            break;
         }

         case EOC:
         {
            return 0;
            break;
         }

         case SOC:
         case SGI:
         case SGC:
         case SAX:
         case COM:
         case EOH:
         {
            // Invalid Codestream
            fprintf(stderr, CSERROR);
            status |= CODESTREAM_ERROR;

            break;
         }

         default:
         {
            if(!can_read(stream, 2))
            {
               // Invalid Codestream
               fprintf(stderr, CSERROR);
               bwc_kill_compression(field);
               status |= CODESTREAM_ERROR;
               break;
            }

            Lunk  = (uint16)get_symbol(stream, 2);
            if(!can_read(stream, Lunk - 2))
            {
               // Invalid Codestream
               fprintf(stderr, CSERROR);
               bwc_kill_compression(field);
               status |= CODESTREAM_ERROR;
               break;
            }
            
            stream->L += Lunk - 2;
            break;
         }
      }
   }
   return 0;
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
!                -           Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
assemble_main_header(bwc_field *const field)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64            size;
   uint16            Linf, Lctr;
   uint8             p, l;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl      *control;
   bwc_gl_inf       *info;
   bwc_tile         *tile;
   bwc_parameter    *parameter;
   bitstream        *stream;
   
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);

   /*--------------------------------------------------------*\
   ! Save the global as well as the subband control and info  !
   ! structure to temporary variables to make the code more   !
   ! readable.                                                !
   \*--------------------------------------------------------*/
   info          =  field->info;
   control       = &field->control;

   tile          = &field->tile[0];

   parameter     = &tile->parameter[0];
   
   Linf = 40 + info->nPar * 25;
   Lctr = 50 + control->nLayers * 4;

   size = 6 + Linf + Lctr;

   stream = init_stream(NULL, size, 'c');
   if(!stream)
   {
      // memory allocation error
      return 1;
   }

   emit_symbol(stream, SOC,                                            2);
   emit_symbol(stream, SGI,                                            2);
   emit_symbol(stream, Linf,                                           2);
   emit_symbol(stream, info->nX,                                       8);
   emit_symbol(stream, info->nY,                                       8);
   emit_symbol(stream, info->nZ,                                       8);
   emit_symbol(stream, info->nTS,                                      2);
   emit_symbol(stream, info->nPar,                                     1);
   emit_symbol(stream, info->precision,                                1);
   emit_chunck(stream, (uchar*)info->f_ext,                            10);

   for(p = 0; p < info->nPar; ++p)
   {
      emit_chunck(stream, (uchar*)parameter[p].info.name,             24);
      emit_symbol(stream, parameter[p].info.precision,                 1);
   }

   emit_symbol(stream, SGC,                                            2);
   emit_symbol(stream, Lctr,                                           2);

   emit_symbol(stream, control->CSsgc,                                 2);

   emit_symbol(stream, control->error_resilience,                      1);

   emit_symbol(stream, control->quantization_style,                    1);
   emit_symbol(stream, control->guard_bits,                            1);

   emit_symbol(stream, control->qt_exponent,                           1);
   emit_symbol(stream, control->qt_mantissa,                           2);

   emit_symbol(stream, control->progression,                           1);
   emit_symbol(stream, control->KernelX << 6 | control->KernelY << 4 | 
                       control->KernelZ << 2 | control->KernelTS,      1);

   emit_symbol(stream, control->decompX,                               1);
   emit_symbol(stream, control->decompY,                               1);
   emit_symbol(stream, control->decompZ,                               1);
   emit_symbol(stream, control->decompTS,                              1);

   emit_symbol(stream, control->precSizeY  << 4 | control->precSizeX,  1);
   emit_symbol(stream, control->precSizeTS << 4 | control->precSizeZ,  1);

   emit_symbol(stream, control->cbX,                                   1);
   emit_symbol(stream, control->cbY,                                   1);
   emit_symbol(stream, control->cbZ,                                   1);
   emit_symbol(stream, control->cbTS,                                  1);

   emit_symbol(stream, control->Qm,                                    1);

   emit_symbol(stream, control->tileSizeX,                             8);
   emit_symbol(stream, control->tileSizeY,                             8);
   emit_symbol(stream, control->tileSizeZ,                             8);
   emit_symbol(stream, control->tileSizeTS,                            2);

   emit_symbol(stream, control->nLayers,                               1);

   for(l = 0; l < control->nLayers; ++l)
   {
      emit_symbol(stream, *(uint32 *)&control->bitrate[l], 4);
   }

   if(terminate_stream(stream, &control->header))
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

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
!                -           Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
codestream_write_aux(bwc_stream *const header, bwc_stream *const aux)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint32             Laux;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bitstream         *stream;
   
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(header);
   assert(aux);

   stream = init_stream(header->memory, header->size, 'c');
   if(!stream)
   {
      // memory allocation error
      return 1;
   }

   Laux = aux->size + 4;

   stream->L      = stream->Lmax;
   stream->Lmax  += Laux + 2;
   stream->memory = realloc(stream->memory, stream->Lmax);
   if(!stream->memory)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   emit_symbol(stream, SAX,         2);
   emit_symbol(stream, Laux,        4);
   emit_chunck(stream, aux->memory, aux->size);
 
   if(terminate_stream(stream, header))
   {
      // memory allocation error
      return 1;
   }

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
!                -           Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
codestream_write_com(bwc_stream *const header, bwc_stream *const com)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint16             Lcom;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bitstream         *stream;
   
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(header);
   assert(com);

   stream = init_stream(header->memory, header->size, 'c');
   if(!stream)
   {
      // memory allocation error
      return 1;
   }

   Lcom = com->size + 2;

   stream->L      = stream->Lmax;
   stream->Lmax  += Lcom + 2;
   stream->memory = realloc(stream->memory, stream->Lmax);
   if(!stream->memory)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   emit_symbol(stream, COM,         2);
   emit_symbol(stream, Lcom,        2);
   emit_chunck(stream, com->memory, com->size);

   if(terminate_stream(stream, header))
   {
      // memory allocation error
      return 1;
   }

   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: bwc_stream* assemble_codestream(bwc_field *const field)                                   !
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
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                bwc_stream*               - Packed stream containing the compressed data set.               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                13.06.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
bwc_stream*
assemble_codestream(bwc_field *const field)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   i, size;
   
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;
   bwc_tile          *tile;
   bitstream         *stream;
   bwc_stream        *packed_stream;
   
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);

   /*--------------------------------------------------------*\
   ! Save the global control and info structure to temporary  !
   ! variables to make the code more readable and initialize  !
   ! the final codestream size with the number of header      !
   ! bytes.                                                   !
   \*--------------------------------------------------------*/
   control = &field->control;
   size    =  control->header.size;

   /*--------------------------------------------------------*\
   ! Walk through the tile structure and assemble the packed  !
   ! data stream.                                             !
   \*--------------------------------------------------------*/
   for(i = 0; i < control->nTiles; ++i)
   {
      /*--------------------------------------------------------*\
      ! Save the tile structure in a temporary variable to make  !
      ! the code more readable.                                  !
      \*--------------------------------------------------------*/
      tile          = &field->tile[i];

      /*--------------------------------------------------------*\
      ! Sequence the packets according to the user specified op- !
      ! tion and iterate the size of the codestream.             !
      \*--------------------------------------------------------*/
      if(sequence_packets(field, tile))
      {
         return NULL;
      }

      size += tile->control.header_size + tile->control.body_size;
   }

   /*--------------------------------------------------------*\
   ! Initialize the final codestream and emit the header      !
   ! bytes.                                                   !
   \*--------------------------------------------------------*/
   stream = init_stream(NULL, size, 'c');
   codestream_write_header(stream, field);

   /*--------------------------------------------------------*\
   ! Walk through the tile structure and assemble the packed  !
   ! data stream.                                             !
   \*--------------------------------------------------------*/
   for(i = 0; i < control->nTiles; ++i)
   {
      /*--------------------------------------------------------*\
      ! Save the tile structure in a temporary variable to make  !
      ! the code more readable.                                  !
      \*--------------------------------------------------------*/
      tile          = &field->tile[i];

      assemble_tile(field, tile, stream);
   }

   emit_symbol(stream, EOC, 2);

   packed_stream = calloc(1, sizeof(bwc_stream));
   if(!packed_stream)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return NULL;
   }

   if(terminate_stream(stream, packed_stream))
   {
      return NULL;
   }
   else
   {
      return packed_stream;
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uchar parse_codestream(bwc_field *const field, bitstream *const stream)                   !
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
!                field                       bwc_field*            - Structure defining the compression/     !
!                                                                    decompression stage.                    !
!                                                                                                            !
!                stream                      bitstream*            - Structure used to assemble a bwc bit-   !
!                                                                    bitstream.                              !
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
!                05.08.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
bwc_field*
parse_codestream(bwc_data *const data, uint8 const layer)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_field         *field;
   bitstream         *stream;
   
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(data);

   /*--------------------------------------------------------*\
   ! Initialize a bitstream used to parse the packed code-    !
   ! stream.                                                  !
   \*--------------------------------------------------------*/
   stream = init_stream(data->codestream.data->memory, 
                        data->codestream.data->size, 'd');

   /*--------------------------------------------------------*\
   ! Parse the main header and set up the field structure for !
   ! the current decompression run.                           !
   \*--------------------------------------------------------*/
   field = parse_main_header(data, stream);
   if(!field)
   {
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! Initialize the useLayer option to decompress the entire  !
   ! codestream.                                              !
   \*--------------------------------------------------------*/
   field->control.useLayer = field->control.nLayers - 1;   

   /*--------------------------------------------------------*\
   ! Check if the layer index supplied by the function caller !
   ! is valid.                                                !
   \*--------------------------------------------------------*/
   if(layer < field->control.nLayers && layer > 0)
   {
      /*--------------------------------------------------------*\
      ! Amend the use layer variable in the global control struc-!
      ! ture.                                                    !
      \*--------------------------------------------------------*/
      field->control.useLayer = layer;
   }

   /*--------------------------------------------------------*\
   ! Parse the rest of the compressed codestream and load the !
   ! body into the field structure.                           !
   \*--------------------------------------------------------*/
   if(parse_body(field, stream))
   {
      bwc_kill_compression(field);
      return NULL;
   }

   /*--------------------------------------------------------*\
   ! Free the bitstream used to parse the codestream and re-  !
   ! turn the field structure to the function caller.         !
   \*--------------------------------------------------------*/
   free(stream);

   return field;
}