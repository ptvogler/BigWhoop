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
||        This file describes a set of functions that can be used to de-/encode bwc               ||
||        codeblocks described by the bwc_field structure according to the embedded block         ||
||        coding paradigm described by the JPEG 2000 standard. For more information please        ||
||        refere to JPEG2000 by D. S. Taubman and M. W. Marcellin.                                ||
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
#include <math.h>
#include <omp.h>
#include <time.h>

#include "codestream.h"
#include "bitstream.h"
#include "constants.h"
#include "macros.h"
#include "types.h"
#include "tagtree.h"
#include "tier2.h"

/************************************************************************************************************\
||                                      _  _ ____ ____ ____ ____ ____                                       ||
||                                      |\/| |__| |    |__/ |  | [__                                        ||
||                                      |  | |  | |___ |  \ |__| ___]                                       ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                These macros define a variable length encoder and decoder for the delta z (new coding       !
!                passes) information.                                                                        !
!                                                                                                            !
!   Macros:                                                                                                  !
!   -------                                                                                                  !
!                Macro                                Description                                            !
!                -----                                -----------                                            !
!                encode_delta_z                     - Encoding algorithm used to store the number of new     !
!                                                     coding passes in a packet header.                      !
!                                                                                                            !
!                dencode_delta_z                    - Decoding algorithm used to extract the number of new   !
!                                                     coding passes form a packet header.                    !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                13.05.2019  Patrick Vogler     B87D120     V 0.1.0     Macros created                       !
\*----------------------------------------------------------------------------------------------------------*/
#define encode_delta_z(stream, delta_z)                     \
{                                                           \
   if(delta_z == 1)                                         \
   {                                                        \
      emit_bit(stream, 0);                                  \
   }                                                        \
   else if(delta_z == 2)                                    \
   {                                                        \
      emit_bit(stream, 1);                                  \
      emit_bit(stream, 0);                                  \
   }                                                        \
   else if(delta_z < 6)                                     \
   {                                                        \
      delta_z -= 3;                                         \
      emit_bit(stream, 1);                                  \
      emit_bit(stream, 1);                                  \
      emit_bit(stream, 0x02 & delta_z);                     \
      emit_bit(stream, 0x01 & delta_z);                     \
   }                                                        \
   else if(delta_z < 37)                                    \
   {                                                        \
      delta_z -= 6;                                         \
      emit_bit(stream, 1);                                  \
      emit_bit(stream, 1);                                  \
      emit_bit(stream, 1);                                  \
      emit_bit(stream, 1);                                  \
      emit_bit(stream, 0x10 & delta_z);                     \
      emit_bit(stream, 0x08 & delta_z);                     \
      emit_bit(stream, 0x04 & delta_z);                     \
      emit_bit(stream, 0x02 & delta_z);                     \
      emit_bit(stream, 0x01 & delta_z);                     \
   }                                                        \
   else if(delta_z < 293)                                   \
   {                                                        \
      delta_z -= 37;                                        \
      emit_bit(stream, 1);                                  \
      emit_bit(stream, 1);                                  \
      emit_bit(stream, 1);                                  \
      emit_bit(stream, 1);                                  \
      emit_bit(stream, 1);                                  \
      emit_bit(stream, 1);                                  \
      emit_bit(stream, 1);                                  \
      emit_bit(stream, 1);                                  \
      emit_bit(stream, 1);                                  \
      emit_bit(stream, 0x080 & delta_z);                    \
      emit_bit(stream, 0x040 & delta_z);                    \
      emit_bit(stream, 0x020 & delta_z);                    \
      emit_bit(stream, 0x010 & delta_z);                    \
      emit_bit(stream, 0x008 & delta_z);                    \
      emit_bit(stream, 0x004 & delta_z);                    \
      emit_bit(stream, 0x002 & delta_z);                    \
      emit_bit(stream, 0x001 & delta_z);                    \
   }                                                        \
}

#define decode_delta_z(stream, delta_z)                     \
{                                                           \
   if(!get_bit(stream))                                     \
   {                                                        \
      delta_z = 1;                                          \
   }                                                        \
   else if(!get_bit(stream))                                \
   {                                                        \
      delta_z = 2;                                          \
   }                                                        \
   else                                                     \
   {                                                        \
      delta_z = (get_bit(stream) << 1) |                    \
                 get_bit(stream);                           \
                                                            \
      if(delta_z < 3)                                       \
      {                                                     \
         delta_z += 3;                                      \
      }                                                     \
      else                                                  \
      {                                                     \
         delta_z = (get_bit(stream) << 4) |                 \
                   (get_bit(stream) << 3) |                 \
                   (get_bit(stream) << 2) |                 \
                   (get_bit(stream) << 1) |                 \
                    get_bit(stream);                        \
                                                            \
         if(delta_z < 31)                                   \
         {                                                  \
            delta_z += 6;                                   \
         }                                                  \
         else                                               \
         {                                                  \
            delta_z = (get_bit(stream) << 7) |              \
                      (get_bit(stream) << 6) |              \
                      (get_bit(stream) << 5) |              \
                      (get_bit(stream) << 4) |              \
                      (get_bit(stream) << 3) |              \
                      (get_bit(stream) << 2) |              \
                      (get_bit(stream) << 1) |              \
                       get_bit(stream);                     \
                                                            \
            delta_z += 37;                                  \
         }                                                  \
      }                                                     \
   }                                                        \
}

/************************************************************************************************************\
||                ___  ____ _ _  _ ____ ___ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                ||
||                |__] |__/ | |  | |__|  |  |___    |___ |  | |\ | |     |  | |  | |\ | [__                 ||
||                |    |  \ |  \/  |  |  |  |___    |    |__| | \| |___  |  | |__| | \| ___]                ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void encode_length(bitstream *const header, bwc_codeblock *const codeblock,               !
!   --------------                                                   int8  const quality_layer,              !
!                                                                    uchar const estimate)                   !
!                                                                                                            !
!                                                                                                            !
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
!                29.04.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
encode_length(bitstream *const header, bwc_codeblock *const codeblock, 
                                                int8  const quality_layer,
                                                uchar const estimate)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint32   L;
   int32    L_bits;
   uint16   beta, max_beta;
   uint16   cp_bits;
   int16   *cp_contr;
   int16    z_curr, z_prev;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_cblk_ctrl     *cblk_ctrl;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(codeblock);
   assert(header);

   /*--------------------------------------------------------*\
   ! Save the global control and info structure to temporary  !
   ! variables to make the code more readable.                !
   \*--------------------------------------------------------*/
   cblk_ctrl = &codeblock->control;
   cp_contr  =  cblk_ctrl->cp_contr;

   z_curr = cp_contr[quality_layer    ] - 1;
   z_prev = cp_contr[quality_layer - 1] - 1;

   beta     = (!estimate) ? cblk_ctrl->beta : cblk_ctrl->beta_est;
   max_beta = beta;

   L        = codeblock->encoded_block->L[z_curr] - 
              ((z_prev >= 0) ? codeblock->encoded_block->L[z_prev] : 0);

   cp_bits  = (uint16)log2(z_curr - z_prev);
   L_bits   = (uint16)log2(L) + 1 - cp_bits;

   max_beta = (max_beta < L_bits) ? L_bits : max_beta;

   while(beta < max_beta)
   {
      emit_bit(header, 1);
      beta++;
   }

   emit_bit(header, 0);

   L_bits = 1 << (beta + cp_bits - 1);

   while(L_bits > 0)
   {
      emit_bit(header, L & L_bits);
      L_bits >>= 1;
   }

   if(estimate == 0)
   {
      cblk_ctrl->beta     = beta;
      cblk_ctrl->beta_est = beta;
   }
   else if(estimate == 1)
   {
      cblk_ctrl->beta_est = beta;
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void create_quality_layers(bwc_field *const field, bwc_tile *const tile)                  !
!   --------------                                                                                           !
!                                                                                                            !
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
!                29.04.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
decode_length(bitstream *const header, bwc_codeblock *const codeblock, int8 const quality_layer)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint32   i;
   uint16   L_bits;
   int16   *cp_contr;
   int16    z_curr, z_prev;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_cblk_ctrl     *cblk_ctrl;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(codeblock);
   assert(header);

   cblk_ctrl   = &codeblock->control;
   cp_contr    =  cblk_ctrl->cp_contr;

   while(get_bit(header))
   {
      cblk_ctrl->beta++;
   }

   z_curr = cp_contr[quality_layer    ] - 1;
   z_prev = cp_contr[quality_layer - 1] - 1;

   L_bits = cblk_ctrl->beta + (uint16)log2(z_curr - z_prev);

   for(i = 0; L_bits > 0; --L_bits)
   {
      i <<= 1;
      i  |= get_bit(header);
   }

   codeblock->encoded_block->L[z_curr] = ((z_prev >= 0) ? codeblock->encoded_block->L[z_prev] : 0) + i;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: int64 create_packet(bwc_field *const field, bwc_tile       *const tile,                   !
!   --------------                                             bwc_resolution *const resolution,             !
!                                                              uint32          const prec_idx,               !
!                                                              int8            const q_layer,                !
!                                                              uchar           const est)                    !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to assemble a codestream packet for a given precinct (prec_idx)       !
!                and quality layer (q_layer). In order to facilitate a fast quality layer creation,          !
!                the est flag can be used to instruct the function to merely estimate the packet head-       !
!                er size and circumvent the packet body creation. If est > 1 only the header size is         !
!                evaluated, while for est = 1 the packet body size is calculated as well.                    !
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
!                resolution                  bwc_resolution*       - Structure defining a bwc resolution     !
!                                                                    level.                                  !
!                                                                                                            !
!                prec_idx                    unsigned int(32 bit)  - Index of the current precinct.          !
!                                                                                                            !
!                q_layer                     signed int(16 bit)    - Index of the current quality layer.     !
!                                                                                                            !
!                est                         unsigned char         - Flag used to instruct the function on   !
!                                                                    wether a header byte size estimation or !
!                                                                    a final packet assembly is required.    !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                integer(16 bit)           - Returns the packet header size if successful or -1 if an        !
!                                            error occurred.                                                 !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                29.04.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static int32
create_packet(bwc_field *const field, bwc_tile       *const tile, 
                                      bwc_resolution *const resolution,
                                      uint32          const prec_idx,
                                      int16           const q_layer,
                                      uchar           const est)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint32   size, start, end;
   uint32   k;
   uint16   cb;
   int16   *cp_contr;
   int16    delta_z;
   int8     m;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_packet        *packet;
   bwc_precinct      *precinct;
   bwc_codeblock     *codeblock;
   bitstream         *header;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(tile);
   assert(resolution);

   /*--------------------------------------------------------*\
   ! Save the current packet structure to a temporary varia-  !
   ! ble to make the code more readable.                      !
   \*--------------------------------------------------------*/
   packet = &resolution->packet[q_layer * resolution->control.number_of_precincts + prec_idx];

   /*--------------------------------------------------------*\
   ! Check if the current packet has any codeblock contribu-  !
   ! tion.                                                    !
   \*--------------------------------------------------------*/
   if(packet->e)
   {
      /*--------------------------------------------------------*\
      ! Initialize the stream that is used to assemble the pack- !
      ! et header.                                               !
      \*--------------------------------------------------------*/
      header = init_stream(NULL, PACKET_HEADER_SIZE, 'c');
      if(!header)
      {
         // memory allocation error
         return -1;
      }

      /*--------------------------------------------------------*\
      ! If the function caller does not specify a packet size es-!
      ! timation, allocate the packet body packed stream that is !
      ! used to hold the codeblock contributions to the current  !
      ! qualitylayer.                                            !
      \*--------------------------------------------------------*/
      if(!est)
      {
         packet->body.access = 
         packet->body.memory = calloc(packet->body.size, sizeof(bwc_stream));
         if(!packet->body.memory)
         {
            // memory allocation error
            fprintf(stderr, MEMERROR);
            return -1;
         }
      }

      /*--------------------------------------------------------*\
      ! Emit the contribution bit to the packet header that sig- !
      ! nals a non-empty packet body.                            !
      \*--------------------------------------------------------*/
      emit_bit(header, 1);

      /*--------------------------------------------------------*\
      ! Iterate over all codeblocks in all subbands to assemble  !
      ! the packet header and, if no estimation has been spec-   !
      ! ified, body.                                             !
      \*--------------------------------------------------------*/
      for(m = 0; m < resolution->control.number_of_subbands; ++m)
      {
         /*--------------------------------------------------------*\
         ! Save the precinct structure to a temporary variable to   !
         ! make the code more readable.                             !
         \*--------------------------------------------------------*/
         precinct = &resolution->subband[m].precinct[prec_idx];

         for(cb = 0; cb < precinct->control.number_of_codeblocks; ++cb)
         {
            /*--------------------------------------------------------*\
            ! Save the codeblock and coding pass contribution struc-   !
            ! tures to temporary variables to make the code more       !
            ! readable.                                                !
            \*--------------------------------------------------------*/
            codeblock = &precinct->codeblock[cb];
            cp_contr  = codeblock->control.cp_contr;

            /*--------------------------------------------------------*\
            ! Evaluate the new coding passes added to the packet body  !
            ! for the current quality layer.                           !
            \*--------------------------------------------------------*/
            delta_z = cp_contr[q_layer] - cp_contr[q_layer - 1];

            /*--------------------------------------------------------*\
            ! Evaluate if the current codeblock already contributed to !
            ! a packet.                                                !
            \*--------------------------------------------------------*/
            if(cp_contr[q_layer - 1] == 0)
            {
               /*--------------------------------------------------------*\
               ! If no contribution has yet been made, encode the inclu-  !
               ! sion tagtree for the current codeblock and...            !
               \*--------------------------------------------------------*/
               encode_tagtree(precinct->control.tag_inclusion, header, q_layer + 1, cb, est);

               /*--------------------------------------------------------*\
               ! If the codeblock contributes to the current packet, en-  !
               ! code the tagtree for the non-significant most signifi-   !
               ! cant bit planes.                                         !
               \*--------------------------------------------------------*/
               if(cp_contr[q_layer])
               {
                  for(k = 1; k <= codeblock->encoded_block->Kmsbs + 1u; ++k)
                  {
                     encode_tagtree(precinct->control.tag_msbs, header, k, cb, est);
                  }
               }
            }
            else
            {
               /*--------------------------------------------------------*\
               ! If a contribution has already been made, transmit a flag !
               ! to signal if new coding passes are to be contributed.    !
               \*--------------------------------------------------------*/
               emit_bit(header, delta_z);
            }

            /*--------------------------------------------------------*\
            ! Check if new coding passes contributions are available.  !
            \*--------------------------------------------------------*/
            if(delta_z)
            {
               /*--------------------------------------------------------*\
               ! If new coding passes are available, encode delta_z and   !
               ! the corresponding coding pass lengths in the packet      !
               ! header.                                                  !
               \*--------------------------------------------------------*/
               encode_delta_z(header, delta_z);
               encode_length(header, codeblock, q_layer, est);

               /*--------------------------------------------------------*\
               ! If the final quality layer (est = 1) or packet (est = 0) !
               ! are assembled, evaluate the start and end points as well !
               ! as the size of the contribution in the encoded block     !
               ! memory block and...                                      !
               \*--------------------------------------------------------*/
               if(est <= 1)
               {
                  start = (cp_contr[q_layer - 1] == 0) ? 0 : codeblock->encoded_block->L[cp_contr[q_layer - 1] - 1];
                  end   = (cp_contr[q_layer]     == 0) ? 0 : codeblock->encoded_block->L[cp_contr[q_layer]     - 1];
                  size  = end - start;

                  /*--------------------------------------------------------*\
                  ! ...add up the packet body size for the final quality     !
                  ! layer assembly or...                                     !
                  \*--------------------------------------------------------*/
                  if(est == 1)
                  {
                     packet->body.size += size;
                  }
                  /*--------------------------------------------------------*\
                  ! ...copy the contribution to the packet body for the fi-  !
                  ! nal packet assembly.                                     !
                  \*--------------------------------------------------------*/
                  else
                  {
                     memcpy(packet->body.access, codeblock->encoded_block->data + start, size);
                     packet->body.access   += size;
                     packet->body.position += size;
                  }
               }
            }
         }
      }

      /*--------------------------------------------------------*\
      ! Flush the remaining bits from the buffer to the header   !
      ! stream.                                                  !
      \*--------------------------------------------------------*/
      flush_stream(header);

      /*--------------------------------------------------------*\
      ! If a packet header size estimation has been performed    !
      ! (est > 0), reset the non-empty packet flag, free the     !
      ! header stream and return the estimated number of header  !
      ! bytes to the function caller.                            !
      \*--------------------------------------------------------*/
      if(est)
      {
         if(est == 2)
         {
            packet->e = 0;
         }

         k = header->L;
         terminate_stream(header, NULL);
         return k;
      }
      /*--------------------------------------------------------*\
      ! If the final packet has been assembled, terminate the    !
      ! header stream, evaluate the overall packet size and re-  !
      ! turn the total number of header bytes to the function    !
      ! caller.                                                  !
      \*--------------------------------------------------------*/
      else
      {
         if(terminate_stream(header, &packet->header))
         {
            // memory allocation error
            return -1;
         }

         packet->size = packet->body.size + packet->header.size;

         return packet->header.size;
      }
   }
   else
   {
      /*--------------------------------------------------------*\
      ! If the packet has no codeblock contributions, reset the  !
      ! non-empty packet flag and return a header size of 1 for  !
      ! a packet header size estimation or...                    !
      \*--------------------------------------------------------*/
      if(est)
      {
         if(est == 2)
         {
            packet->e = 0;
         }
         
         return 1;
      }
      /*--------------------------------------------------------*\
      ! ... set the overall packet and header size to 1 and re-  !
      ! turn the value to the function caller.                   !
      \*--------------------------------------------------------*/
      else
      {
         packet->size          = 1;
         packet->header.size   = 1;

         packet->header.access =
         packet->header.memory = calloc(1, sizeof(uchar));
         if(packet->header.memory == NULL)
         {
            // memory allocation error
            fprintf(stderr, MEMERROR);
            return -1;
         }

         return packet->header.size;
      }
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uchar create_packets(bwc_field *const field, bwc_tile *const tile)                        !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function iterates overall quality layers for every precinct in all resolution          !
!                levels to create the codestream packets for all tile parameters. If an error occurres       !
!                during packet creation the function returns to the function caller with an error flag.      !
!                If packet creation for all quality layers belonging to a specific precinct is success-      !
!                ful, the function iterates over all codeblocks that are a part of the current precinct,     !
!                to free all encoded memory blocks and reset the encoded block structure for the next        !
!                compression run.                                                                            !
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
!                29.04.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uchar
create_packets(bwc_field *const field, bwc_tile *const tile)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   cb, p;
   uint16   j, r;
   uint8    l, m;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;
   bwc_gl_inf        *info;
   bwc_parameter     *parameter;
   bwc_resolution    *resolution;
   bwc_codeblock     *codeblock;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(tile);

   /*--------------------------------------------------------*\
   ! Save the global control and info structure to temporary  !
   ! variables to make the code more readable.                !
   \*--------------------------------------------------------*/
   control = &field->control;
   info    =  field->info;

   /*--------------------------------------------------------*\
   ! Iterate overall quality layers for every precinct in all !
   ! resolution levels to create the codestream packets for   !
   ! all tile parameters.                                     !
   \*--------------------------------------------------------*/
   for(j = 0; j < info->nPar; ++j)
   {
      /*--------------------------------------------------------*\
      ! Save the parameter structure to a temporary variable to  !
      ! make the code more readable.                             !
      \*--------------------------------------------------------*/
      parameter = &tile->parameter[j];

      for(r = 0; r < control->nDecomp + 1; ++r)
      {
         /*--------------------------------------------------------*\
         ! Save the resolution structure to a temporary variable to !
         ! make the code more readable.                             !
         \*--------------------------------------------------------*/
         resolution = &parameter->resolution[r];

         for(p = 0; p < resolution->control.number_of_precincts; ++p)
         {
            for(l = 0; l < control->nLayers; ++l)
            {
               if(create_packet(field, tile, resolution, p, l, 0) < 0)
               {
                  // memory allocation error
                  return 1;
               }
            }

            /*--------------------------------------------------------*\
            ! If packet creation for all quality layers belonging to   !
            ! the current precinct is successful, iterate over all     !
            ! codeblocks that are a part of the current precinct and   !
            ! reset the encoded block structure for another compres-   !
            ! sion run.                                                !
            \*--------------------------------------------------------*/
            for(m = 0; m < resolution->control.number_of_subbands; ++m)
            {
               for(cb = 0; cb < resolution->subband[m].precinct[p].control.number_of_codeblocks; ++cb)
               {
                  /*--------------------------------------------------------*\
                  ! Save the codeblock structure and to a temporary variable !
                  ! to make the code more readable.                          !
                  \*--------------------------------------------------------*/
                  codeblock = &resolution->subband[m].precinct[p].codeblock[cb];

                  /*--------------------------------------------------------*\
                  ! Free the encoded block memory block that holds the com-  !
                  ! pressed codeblock bitstream.                             !
                  \*--------------------------------------------------------*/
                  free(codeblock->encoded_block->data);

                  /*--------------------------------------------------------*\
                  ! Reset the encoded block and coding pass contribution     !
                  ! structure for the next compression run.                  !
                  \*--------------------------------------------------------*/
                  memset(codeblock->encoded_block,    0, sizeof(bwc_encoded_cblk));
                  memset(codeblock->control.cp_contr, 0, sizeof(int16) * control->nLayers);
               }
            }
         }
      }
   }
   return 0;
}


/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: int64 create_quality_layer(bwc_field *const field, bwc_tile *const tile,                  !
!   --------------                                                    uint16    const threshold,             !
!                                                                     int16     const q_layer,               !
!                                                                     uchar     const est)                   !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function iterates over all codeblocks for every precinct in all resolution levels      !
!                to assemble the quality layers for all tile parameters. If an error occurres during         !
!                packet creation the function returns to the function caller with an error flag.             !
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
!                threshold                   unsigned int(16 bit)  - Index of the current quality layer.     !
!                                                                                                            !
!                q_layer                     signed int(16 bit)    - Index of the current quality layer.     !
!                                                                                                            !
!                est                         unsigned char         - Flag used to instruct the function on   !
!                                                                    wether a quality layer size estimation  !
!                                                                    or final assembly is required.          !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                Type                        Description                                                     !
!                ----                        -----------                                                     !
!                integer(16 bit)           - Returns the quality layer size if successful or -1 if an        !
!                                            error occurred.                                                 !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                29.04.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static int64
create_quality_layer(bwc_field *const field, bwc_tile *const tile,
                                             uint16    const threshold,
                                             int16     const q_layer,
                                             uchar     const est)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   cb, nPrec, p;
   int64    estimated_ql_size, estimated_ph_size;
   uint16   j, r;
   int16   *cp_contr;
   int16    z;
   uint8    l, m;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;
   bwc_gl_inf        *info;
   bwc_parameter     *parameter;
   bwc_resolution    *resolution;
   bwc_subband       *subband;
   bwc_codeblock     *codeblock;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(tile);

   /*--------------------------------------------------------*\
   ! Save the global control and info structure to temporary  !
   ! variables to make the code more readable.                !
   \*--------------------------------------------------------*/
   control = &field->control;
   info    =  field->info;

   for(j = 0, estimated_ql_size = 0; j < info->nPar; ++j)
   {
      /*--------------------------------------------------------*\
      ! Save the parameter structure to a temporary variable to  !
      ! make the code more readable.                             !
      \*--------------------------------------------------------*/
      parameter = &tile->parameter[j];

      for(r = 0; r < control->nDecomp + 1; ++r)
      {
         /*--------------------------------------------------------*\
         ! Save the resolution structure to a temporary variable to !
         ! make the code more readable.                             !
         \*--------------------------------------------------------*/
         resolution = &parameter->resolution[r];
         nPrec      = resolution->control.number_of_precincts;

         for(m = 0; m < resolution->control.number_of_subbands; ++m)
         {
            /*--------------------------------------------------------*\
            ! Save the subband structure to a temporary variable to    !
            ! make the code more readable.                             !
            \*--------------------------------------------------------*/
            subband = &resolution->subband[m];

            for(p = 0; p < resolution->control.number_of_precincts; ++p)
            {
               for(cb = 0; cb < subband->precinct[p].control.number_of_codeblocks; ++cb)
               {
                  /*--------------------------------------------------------*\
                  ! Save the codeblock structure and the coding pass contri- !
                  ! bution array to temporary variables to make the code     !
                  ! readable.                                                !
                  \*--------------------------------------------------------*/
                  codeblock = &subband->precinct[p].codeblock[cb];
                  cp_contr  = codeblock->control.cp_contr;
                 
                  /*--------------------------------------------------------*\
                  ! Loop through all coding passes until the first value is  !
                  ! reached that is smaller than the prescribed slope thresh-!
                  ! old.                                                     !
                  \*--------------------------------------------------------*/
                  for(z = codeblock->encoded_block->Z; z >= 0 && codeblock->encoded_block->S[z] < threshold; --z);

                  /*--------------------------------------------------------*\
                  ! Check if the calculated coding pass has a smaller index  !
                  ! than the previous quality layer. If true, no additional  !
                  ! information will be encoded. The coding pass for the     !
                  ! current quality layer is therefore set to the coding     !
                  ! pass of the previous quality layer. If no previous qual- !
                  ! ity layer has been generated, the length of the first    !
                  ! coding pass is added to the estimated quality layer      !
                  ! length. ...                                              !
                  \*--------------------------------------------------------*/
                  if(z < cp_contr[q_layer - 1])
                  {
                     cp_contr[q_layer] = cp_contr[q_layer - 1];

                     if(cp_contr[q_layer] == 1)
                     {
                        estimated_ql_size += codeblock->encoded_block->L[0];
                     }
                  }
                  /*--------------------------------------------------------*\
                  ! If false, the coding pass index is added to the coding   !
                  ! pass contribution list. If no information has been in-   !
                  ! cluded in the previous quality layer, or the coding pass !
                  ! index is equal to 1, the length of the previous coding   !
                  ! pass is added to the estimated quality layer length. If  !
                  ! the previous quality layer contains information, the     !
                  ! length difference between the two evaluated coding pass- !
                  ! es is added.                                             !
                  \*--------------------------------------------------------*/
                  else
                  {
                     cp_contr[q_layer] = z;

                     if(z > 0)
                     {
                        estimated_ql_size += codeblock->encoded_block->L[z - 1];

                        if(z > 1 && (cp_contr[q_layer - 1] > 0))
                        {
                           estimated_ql_size -= codeblock->encoded_block->L[cp_contr[q_layer - 1] - 1];
                        }
                     }
                  }

                  /*--------------------------------------------------------*\
                  ! Check if the current packet has any codeblock contribu-  !
                  ! tion and set the empty header indicator and its coordi-  !
                  ! nates accordingly.                                       !
                  \*--------------------------------------------------------*/
                  if(cp_contr[q_layer] > cp_contr[q_layer - 1])
                  {
                     resolution->packet[q_layer * nPrec + p].e  = 1;
                  }

                  /*--------------------------------------------------------*\
                  ! If the last, valid quality layer has been correctly cal- !
                  ! culated, add the information to the appropriate tagtree. !
                  \*--------------------------------------------------------*/
                  if(!est)
                  {
                     /*--------------------------------------------------------*\
                     ! Set the value for the number of non-significant, leading !
                     ! bitplanes in the appropriate tagtree.                    !
                     \*--------------------------------------------------------*/
                     if(q_layer == 0)
                     {
                        tagtree_set_value(subband->precinct[p].control.tag_msbs, cb, codeblock->encoded_block->Kmsbs);
                     }

                     /*--------------------------------------------------------*\
                     ! If the last, valid quality layer has been correctly cal- !
                     ! culated, add the information to the appropriate tagtree. !
                     \*--------------------------------------------------------*/
                     if(q_layer + 1 == control->nLayers)
                     {
                        for(l = 0; (l < control->nLayers) && (cp_contr[l] == 0); ++l);

                        tagtree_set_value(subband->precinct[p].control.tag_inclusion, cb, l);
                     }
                  }
               }  
            }
         }

         for(p = 0; p < resolution->control.number_of_precincts; ++p)
         {
            estimated_ph_size = create_packet(field, tile, resolution, p, q_layer, est + 1);

            if(estimated_ph_size < 0)
            {
               return -1;
            }
            else
            {
               estimated_ql_size += estimated_ph_size;
            }
            
         }
      }
   }
   return estimated_ql_size;
}


/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void create_quality_layers(bwc_field *const field, bwc_tile *const tile)                  !
!   --------------                                                                                           !
!                                                                                                            !
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
!                29.04.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static uchar
create_quality_layers(bwc_field *const field, bwc_tile *const tile)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint64   tile_size, header_size;
   int64    estimated_tile_size, target_ql_size;
   int64    estimated_ql_size;
   uint32   main_header_size;
   uint16   slope, slope_min, slope_max;
   uint16   tmp;
   uint8    l;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_gl_ctrl       *control;
   bwc_gl_inf        *info;
   bwc_tile_ctrl     *tile_control;
   bwc_tile_inf      *tile_info;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(tile);

   /*--------------------------------------------------------*\
   ! Save the global and tile control and info structure to   !
   ! temporary variables to make the code more readable.      !
   \*--------------------------------------------------------*/
   control = &field->control;
   info    =  field->info;

   tile_control = &tile->control;
   tile_info    = &tile->info;

   /*--------------------------------------------------------*\
   ! Initialize the target and estimated quality layer size   !
   ! and the estimated tile size.                             !
   \*--------------------------------------------------------*/
   target_ql_size      = 0;
   estimated_ql_size   = 0;
   estimated_tile_size = 0;
   
   /*--------------------------------------------------------*\
   ! Calculate the size of the main header, including the end !
   ! of header marker segment.                                !
   \*--------------------------------------------------------*/
   main_header_size = control->headerSize;

   /*--------------------------------------------------------*\
   ! Calculate the size of the present tile and the overall   !
   ! impact of the codestream headers per quality layer.      !
   \*--------------------------------------------------------*/
   tile_size   = ((tile_info->X1 - tile_info->X0) * (tile_info->Y1  - tile_info->Y0)   *
                  (tile_info->Z1 - tile_info->Z0) * (tile_info->TS1 - tile_info->TS0)) * (uint64)info->nPar;

   header_size = (uint64)ceilf((float)(main_header_size + tile_control->header_size) / control->nLayers);

   /*--------------------------------------------------------*\
   ! Loop through and create all the user specified quality   !
   ! layers.                                                  !
   \*--------------------------------------------------------*/
   for(l = 0; l < control->nLayers; ++l)
   {
      /*--------------------------------------------------------*\
      ! Calculate the size of target size of quality layer l.    !
      \*--------------------------------------------------------*/
      target_ql_size = (int64)ceilf(control->bitrate[l] * tile_size / 8.0f);

      if(target_ql_size > 0.0f)
      {
         slope_min = tile_control->slope_min;
         slope_max = tile_control->slope_max;
         slope     = slope_min;
         tmp       = slope_min;

         while(slope_min < slope_max)
         {
            slope = (slope_max + slope_min) >> 1;
            
            estimated_ql_size = create_quality_layer(field, tile, slope, l, 1);

            if(estimated_ql_size >= 0)
            {
               estimated_ql_size += estimated_tile_size + header_size;
            }
            else
            {
               return 1;
            }

            if(estimated_ql_size > target_ql_size)
            {
               if(slope_min == slope)
               {
                  slope_max = slope;
               }
               slope_min = slope;
            }
            else if(estimated_ql_size < target_ql_size)
            {
               if(slope_max == slope)
               {
                  slope_min = slope;
               }
               slope_max = slope;
               tmp       = slope;
            }
            else
            {
               slope_max = slope;
               slope_min = slope;
            }
         }

         if(estimated_ql_size > target_ql_size)
         {
            slope = MAX(tmp, tile_control->slope_min);
         }
      }
      else
      {
         slope = 0;
      }

      estimated_ql_size = create_quality_layer(field, tile, slope, l, 0);

      if(estimated_ql_size >= 0)
      {
         estimated_tile_size += estimated_ql_size + header_size;
      }
      else
      {
         return 1;
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
!   FUNCTION NAME: uchar   t2_encode(bwc_field *const field, bwc_tile *const tile)                           !
!   --------------                                                                                           !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function defines the rate control portion of the entropy encoding stage. In            !
!                the first step, the quality layers are evaluated according to the bitrates de-              !
!                fined by the user. The quality layers are then used to create the data packets              !
!                that comprise the bwc codestream.                                                           !
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
!                12.06.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
t2_encode(bwc_field *const field, bwc_tile *const tile)
{
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(tile);

   /*--------------------------------------------------------*\
   ! Create the quality layers according to the bitrate val-  !
   ! ues provided by the user.                                !
   \*--------------------------------------------------------*/
   if(create_quality_layers(field, tile))
   {
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Create the data packets according to the quality layers  !
   ! evaluated in the previous step.                          !
   \*--------------------------------------------------------*/
   if(create_packets(field, tile))
   {
      return 1;
   }

   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: uchar parse_packet(bwc_field *const field, bwc_tile   *const tile,                        !
!   --------------                                            bwc_packet *const packet,                      !
!                                                             uint64      const body_size)                   !
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                This function is used to parse a codestream packet for a given precinct (prec_idx)          !
!                and quality layer (q_layer).                                                                !
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
!                packet                      bwc_packet*           - Structure defining a bwc packet.        !
!                                                                                                            !
!                body_size                   unsigned int(64 bit)  - Size of the remaining bwc codestream.   !
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
!                29.04.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
parse_packet(bwc_field *const field, bwc_tile   *const tile, 
                                     bwc_packet *const packet, 
                                     uint64      const body_size)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint32   size, start, end;
   uint32   k;
   uint16   cb;
   int16   *cp_contr;
   int16    delta_z;
   int8     m;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_resolution    *resolution;
   bwc_precinct      *precinct;
   bwc_codeblock     *codeblock;
   bwc_encoded_cblk  *encoded_block;
   bitstream         *header;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(field);
   assert(tile);
   assert(packet);

   resolution = &tile->parameter[packet->c].resolution[packet->r];

   /*--------------------------------------------------------*\
   ! Initialize the stream that is used to parse the packet   !
   ! codestream.                                              !
   \*--------------------------------------------------------*/
   header = init_stream(packet->header.memory, body_size, 'd');
   if(!header)
   {
      // memory allocation error
      return 1;
   }

   /*--------------------------------------------------------*\
   ! Extract the codeblock contribution marker from the code- !
   ! stream.                                                  !
   \*--------------------------------------------------------*/
   packet->e = get_bit(header);

   /*--------------------------------------------------------*\
   ! Check if the current packet has any codeblock contribu-  !
   ! tion.                                                    !
   \*--------------------------------------------------------*/
   if(packet->e)
   {
      /*--------------------------------------------------------*\
      ! Iterate over all codeblocks in all subbands to parse the !
      ! the packet header and body.                              !
      \*--------------------------------------------------------*/
      for(m = 0; m < resolution->control.number_of_subbands; ++m)
      {
         /*--------------------------------------------------------*\
         ! Save the precinct structure to a temporary variable to   !
         ! make the code more readable.                             !
         \*--------------------------------------------------------*/
         precinct = &resolution->subband[m].precinct[packet->p];

         for(cb = 0; cb < precinct->control.number_of_codeblocks; ++cb)
         {
            /*--------------------------------------------------------*\
            ! Save the codeblock and coding pass contribution struc-   !
            ! tures to temporary variables to make the code more       !
            ! readable.                                                !
            \*--------------------------------------------------------*/
            codeblock = &precinct->codeblock[cb];
            cp_contr  = codeblock->control.cp_contr;

            /*--------------------------------------------------------*\
            ! Initialize the coding passes present in the packet body  !
            ! for the current quality layer.                           !
            \*--------------------------------------------------------*/
            delta_z = 0;

            /*--------------------------------------------------------*\
            ! Evaluate if the current codeblock already contributed to !
            ! a packet.                                                !
            \*--------------------------------------------------------*/
            if(cp_contr[packet->l - 1] == 0)
            {
               /*--------------------------------------------------------*\
               ! If no contribution has yet been made, decode the inclu-  !
               ! sion tagtree for the current codeblock and...            !
               \*--------------------------------------------------------*/
               decode_tagtree(precinct->control.tag_inclusion, header, packet->l + 1, cb);

               /*--------------------------------------------------------*\
               ! Check if the codeblock has any contributions to the cur- !
               ! rent packet. If true, decode the tagtree for the none-   !
               ! significant bit planes.                                  !
               \*--------------------------------------------------------*/
               if(tagtree_get_value(precinct->control.tag_inclusion, cb) <= packet->l)
               {
                  delta_z = 1;
                  k       = 1;

                  while(decode_tagtree(precinct->control.tag_msbs, header, k++, cb));

                  codeblock->encoded_block->Kmsbs = tagtree_get_value(precinct->control.tag_msbs, cb);
               }
            }
            else
            {
               /*--------------------------------------------------------*\
               ! If a contribution has already been made, extract the     !
               ! flag from the codestream that signals if new coding pas- !
               ! ses are available.                                       !
               \*--------------------------------------------------------*/
               delta_z = get_bit(header);
            }


            /*--------------------------------------------------------*\
            ! Initialize the current quality layer with the number of  !
            ! coding pass contributions present in the previous qual-  !
            ! ity layer.                                               !
            \*--------------------------------------------------------*/
            cp_contr[packet->l] = cp_contr[packet->l - 1];

            /*--------------------------------------------------------*\
            ! Check if new coding passes contributions are available.  !
            \*--------------------------------------------------------*/
            if(delta_z)
            {
               /*--------------------------------------------------------*\
               ! If new coding passes are available, decode delta_z, ...  !
               \*--------------------------------------------------------*/
               decode_delta_z(header, delta_z);

               /*--------------------------------------------------------*\
               ! ... add the additional coding passes to the current qual-!
               ! ity layer and update the maximum number of coding passes !
               ! of the current codeblock...                              !
               \*--------------------------------------------------------*/
               cp_contr[packet->l]         += delta_z;
               codeblock->encoded_block->Z  = cp_contr[packet->l];

               /*--------------------------------------------------------*\
               ! ... and decode the corresponding coding pass lengths.    !
               \*--------------------------------------------------------*/
               decode_length(header, codeblock, packet->l);
            }
         }
      }
   }
   else
   {
      /*--------------------------------------------------------*\
      ! Iterate over all codeblocks in all subbands.             !
      \*--------------------------------------------------------*/
      for(m = 0; m < resolution->control.number_of_subbands; ++m)
      {
         /*--------------------------------------------------------*\
         ! Save the precinct structure to a temporary variable to   !
         ! make the code more readable.                             !
         \*--------------------------------------------------------*/
         precinct = &resolution->subband[m].precinct[packet->p];

         for(cb = 0; cb < precinct->control.number_of_codeblocks; ++cb)
         {
            /*--------------------------------------------------------*\
            ! Save the codeblock and coding pass contribution struc-   !
            ! tures to temporary variables to make the code more       !
            ! readable.                                                !
            \*--------------------------------------------------------*/
            codeblock = &precinct->codeblock[cb];
            cp_contr  = codeblock->control.cp_contr;

            /*--------------------------------------------------------*\
            ! Initialize the current quality layer with the number of  !
            ! coding pass contributions present in the previous qual-  !
            ! ity layer.                                               !
            \*--------------------------------------------------------*/
            cp_contr[packet->l] = cp_contr[packet->l - 1];
         }
      }
   }
   
   /*--------------------------------------------------------*\
   ! If the header size has not already been evaluated, safe  !
   ! the number of used bytes in the appropriate variable.    !
   \*--------------------------------------------------------*/
   if(!packet->header.size)
   {
      packet->header.size = bytes_used(header);
   }

   /*--------------------------------------------------------*\
   ! If the packet body memory handle has not been set, safe  !
   ! the current position in the header stream in the memory  !
   ! and access variables.                                    !
   \*--------------------------------------------------------*/
   if(!packet->body.memory)
   {
      packet->body.access =
      packet->body.memory = get_access(header);
   }

   /*--------------------------------------------------------*\
   ! Free the packet header stream.                           !
   \*--------------------------------------------------------*/
   free(header);

   /*--------------------------------------------------------*\
   ! If the error resilience mode is active for the current   !
   ! codestream...                                            !
   \*--------------------------------------------------------*/
   if(field->control.error_resilience)
   {
      /*--------------------------------------------------------*\
      ! check if the next symbol corresponds to the end of pack- !
      ! header (EOH) marker...                                   !
      \*--------------------------------------------------------*/
      if(packet->body.memory[0] != 0xFF ||
         packet->body.memory[1] != 0x92)
      {
         /*--------------------------------------------------------*\
         ! ... If not, return to the function caller with an error  !
         ! marker.                                                  !
         \*--------------------------------------------------------*/
         return 1;
      }
      else
      {
         /*--------------------------------------------------------*\
         ! ... If true, advance the packet body memory and access   !
         ! pointer and increase the header size by 2 bytes.         !
         \*--------------------------------------------------------*/
         packet->body.access  = 
         packet->body.memory += 2;
         packet->header.size += 2;
      }
   }

   /*--------------------------------------------------------*\
   ! Check if the current packet has any codeblock contribu-  !
   ! tion.                                                    !
   \*--------------------------------------------------------*/
   if(packet->e)
   {
      /*--------------------------------------------------------*\
      ! Iterate over all codeblocks in all subbands.             !
      \*--------------------------------------------------------*/
      for(m = 0; m < resolution->control.number_of_subbands; ++m)
      {
         /*--------------------------------------------------------*\
         ! Save the precinct structure to a temporary variable to   !
         ! make the code more readable.                             !
         \*--------------------------------------------------------*/
         precinct = &resolution->subband[m].precinct[packet->p];

         for(cb = 0; cb < precinct->control.number_of_codeblocks; ++cb)
         {
            /*--------------------------------------------------------*\
            ! Save the codeblock, encoded codeblock and coding pass    !
            ! contribution structures to temporary variables to make   !
            ! the code more readable.                                  !
            \*--------------------------------------------------------*/
            codeblock     = &precinct->codeblock[cb];
            encoded_block = codeblock->encoded_block;
            cp_contr      = codeblock->control.cp_contr;

            /*--------------------------------------------------------*\
            ! Check if new contributions are available.                !
            \*--------------------------------------------------------*/
            if(cp_contr[packet->l] > cp_contr[packet->l - 1])
            {
               /*--------------------------------------------------------*\
               ! Evaluate the start and end position, as well as the size !
               ! of the new contribution in the encoded codeblock memory  !
               ! block and.                                               !
               \*--------------------------------------------------------*/
               start = (cp_contr[packet->l - 1] == 0) ? 0 : codeblock->encoded_block->L[cp_contr[packet->l - 1] - 1];
               end   = (cp_contr[packet->l]     == 0) ? 0 : codeblock->encoded_block->L[cp_contr[packet->l]     - 1];
               size  = end - start;

               /*--------------------------------------------------------*\
               ! Reallocate the encoded codeblock memory block to accomo- !
               ! date the coding pass contribution.                       !
               \*--------------------------------------------------------*/
               encoded_block->data = realloc(encoded_block->data, end + size);
               if(!encoded_block->data)
               {
                  // memory allocation error
                  fprintf(stderr, MEMERROR);
                  return 1;
               }

               /*--------------------------------------------------------*\
               ! Copy the data to the encoded block structure.            !
               \*--------------------------------------------------------*/
               memcpy(codeblock->encoded_block->data + start, packet->body.access, size);

               /*--------------------------------------------------------*\
               ! Advance the packet body access pointer to the next cod-  !
               ! ing pass contribution and increase the packet body size  !
               ! accordingly.                                             !
               \*--------------------------------------------------------*/
               packet->body.access += size;
               packet->body.size   += size;
            }
         }
      }
   }

   /*--------------------------------------------------------*\
   ! Evaluate the packet size and return to the function      !
   ! caller.                                                  !
   \*--------------------------------------------------------*/
   packet->size = packet->header.size + packet->body.size;
   return 0;
}