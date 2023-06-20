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
||      FILE NAME:   mq.c                                                                                                           ||
||                                                                                                                                  ||
||                                                                                                                                  ||
||      DESCRIPTION:                                                                                                                ||
||      ------------                                                                                                                ||
||      DESCRIPTION NEEDED.                                                                                                         ||
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
||                                                                                                                                  ||
||      PUBLIC FUNCTIONS:                                                                                                           ||
||      -----------------                                                                                                           ||
||                                                                                                                                  ||
||      DEVELOPMENT HISTORY:                                                                                                        ||
||      --------------------                                                                                                        ||
||                                                                                                                                  ||
||                            Date        Author             Change Id   Release     Description Of Change                          ||
||                            ----        ------             ---------   -------     ---------------------                          ||
||                            13.02.2019  Patrick Vogler     B87D120     V 0.1.0     source file created                            ||
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"
#include "mq.h"
#include "types.h"

/************************************************************************************************************\
||                                      _  _ ____ ____ ____ ____ ____                                       ||
||                                      |\/| |__| |    |__/ |  | [__                                        ||
||                                      |  | |  | |___ |  \ |__| ___]                                       ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   Macros:                                                                                                  !
!   -------                                                                                                  !
!                Macro                                Description                                            !
!                -----                                -----------                                            !
!                                                                                                            !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                22.02.2019  Patrick Vogler     B87D120     V 0.1.0     Macros created                       !
\*----------------------------------------------------------------------------------------------------------*/
#define transfer_byte(state)                       \
{                                                  \
   if(state->T == 0xFF)                            \
   {                                               \
      if(state->L >= 0)                            \
      {                                            \
         state->b[state->L] = state->T;            \
      }                                            \
                                                   \
      state->L++;                                  \
      state->T  = (uchar)(state->C >> 0x14);       \
      state->C &= 0xFFFFF;                         \
      state->t  = 0x07;                            \
   }                                               \
   else                                            \
   {                                               \
      if(state->C & 0x8000000)                     \
      {                                            \
         state->T++;                               \
         state->C &= 0x7FFFFFF;                    \
      }                                            \
                                                   \
      if(state->L >= 0)                            \
      {                                            \
         state->b[state->L] = state->T;            \
      }                                            \
                                                   \
      state->L++;                                  \
                                                   \
      if(state->T == 0xFF)                         \
      {                                            \
         state->T  = (uchar)(state->C >> 0x14);    \
         state->C &= 0xFFFFF;                      \
         state->t  = 0x07;                         \
      }                                            \
      else                                         \
      {                                            \
         state->T  = (uchar)(state->C >> 0x13);    \
         state->C &= 0x7FFFF;                      \
         state->t  = 0x08;                         \
      }                                            \
   }                                               \
}

/*----------------------------------------------------------------------------------------------------------*\
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                DESCRIPTION NEEDED                                                                          !
!                                                                                                            !
!   Macros:                                                                                                  !
!   -------                                                                                                  !
!                Macro                                Description                                            !
!                -----                                -----------                                            !
!                                                                                                            !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                22.02.2019  Patrick Vogler     B87D120     V 0.1.0     Macros created                       !
\*----------------------------------------------------------------------------------------------------------*/
#define fill_lsb(state, bitcoder)                  \
{                                                  \
   if((state->L           == bitcoder->Lmax) ||    \
     ((state->T           == 0xFF)           &&    \
      (state->b[state->L] >  0x8f)           ))    \
   {                                               \
      state->t  = 8;                               \
      state->C += 0xFF;                            \
   }                                               \
   else                                            \
   {                                               \
      if((state->T == 0xFF))                       \
      {                                            \
         state->t  = 7;                            \
         state->T  = state->b[state->L];           \
         state->C += (uint32)state->T << 1;        \
         state->L++;                               \
      }                                            \
      else                                         \
      {                                            \
         state->t  = 8;                            \
         state->T  = state->b[state->L];           \
         state->C += (uint32)state->T;             \
         state->L++;                               \
      }                                            \
   }                                               \
}

/************************************************************************************************************\
||                                ____ ____ _  _ ____ ___ ____ _  _ ___ ____                                ||
||                                |    |  | |\ | [__   |  |__| |\ |  |  [__                                 ||
||                                |___ |__| | \| ___]  |  |  | | \|  |  ___]                                ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                These constants define the probability state transition table as described in JPEG2000 by   !
!                David S. Taubman and Michael W. Marcellin (p. 75). In order to avoid the the Most Probable  !
!                Symbol and Least Probable Symbol exchange during encoding, the table has been expanded to   !
!                store the Most Probable Symbol sk instead of the function Xs.                               !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                Date        Author             Change Id   Release     Description Of Change                !
!                ----        ------             ---------   -------     ---------------------                !
!                19.02.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static const bwc_context_state context_state[94] = 
{{0x5601, 0, &context_state[ 1], &context_state[48]},
 {0x3401, 0, &context_state[ 2], &context_state[ 6]},
 {0x1801, 0, &context_state[ 3], &context_state[ 9]},
 {0x0AC1, 0, &context_state[ 4], &context_state[12]},
 {0x0521, 0, &context_state[ 5], &context_state[29]},
 {0x0221, 0, &context_state[38], &context_state[33]},
 {0x5601, 0, &context_state[ 7], &context_state[53]},
 {0x5401, 0, &context_state[ 8], &context_state[14]},
 {0x4801, 0, &context_state[ 9], &context_state[14]},
 {0x3801, 0, &context_state[10], &context_state[14]},
 {0x3001, 0, &context_state[11], &context_state[17]},
 {0x2401, 0, &context_state[12], &context_state[18]},
 {0x1C01, 0, &context_state[13], &context_state[20]},
 {0x1601, 0, &context_state[29], &context_state[21]},
 {0x5601, 0, &context_state[15], &context_state[61]},
 {0x5401, 0, &context_state[16], &context_state[14]},
 {0x5101, 0, &context_state[17], &context_state[15]},
 {0x4801, 0, &context_state[18], &context_state[16]},
 {0x3801, 0, &context_state[19], &context_state[17]},
 {0x3401, 0, &context_state[20], &context_state[18]},
 {0x3001, 0, &context_state[21], &context_state[19]},
 {0x2801, 0, &context_state[22], &context_state[19]},
 {0x2401, 0, &context_state[23], &context_state[20]},
 {0x2201, 0, &context_state[24], &context_state[21]},
 {0x1C01, 0, &context_state[25], &context_state[22]},
 {0x1801, 0, &context_state[26], &context_state[23]},
 {0x1601, 0, &context_state[27], &context_state[24]},
 {0x1401, 0, &context_state[28], &context_state[25]},
 {0x1201, 0, &context_state[29], &context_state[26]},
 {0x1101, 0, &context_state[30], &context_state[27]},
 {0x0AC1, 0, &context_state[31], &context_state[28]},
 {0x09C1, 0, &context_state[32], &context_state[29]},
 {0x08A1, 0, &context_state[33], &context_state[30]},
 {0x0521, 0, &context_state[34], &context_state[31]},
 {0x0441, 0, &context_state[35], &context_state[32]},
 {0x02A1, 0, &context_state[36], &context_state[33]},
 {0x0221, 0, &context_state[37], &context_state[34]},
 {0x0141, 0, &context_state[38], &context_state[35]},
 {0x0111, 0, &context_state[39], &context_state[36]},
 {0x0085, 0, &context_state[40], &context_state[37]},
 {0x0049, 0, &context_state[41], &context_state[38]},
 {0x0025, 0, &context_state[42], &context_state[39]},
 {0x0015, 0, &context_state[43], &context_state[40]},
 {0x0009, 0, &context_state[44], &context_state[41]},
 {0x0005, 0, &context_state[45], &context_state[42]},
 {0x0001, 0, &context_state[45], &context_state[43]},
 {0x5601, 0, &context_state[46], &context_state[46]},
 {0x5601, 1, &context_state[48], &context_state[ 1]},
 {0x3401, 1, &context_state[49], &context_state[53]},
 {0x1801, 1, &context_state[50], &context_state[56]},
 {0x0AC1, 1, &context_state[51], &context_state[59]},
 {0x0521, 1, &context_state[52], &context_state[76]},
 {0x0221, 1, &context_state[85], &context_state[80]},
 {0x5601, 1, &context_state[54], &context_state[ 6]},
 {0x5401, 1, &context_state[55], &context_state[61]},
 {0x4801, 1, &context_state[56], &context_state[61]},
 {0x3801, 1, &context_state[57], &context_state[61]},
 {0x3001, 1, &context_state[58], &context_state[64]},
 {0x2401, 1, &context_state[59], &context_state[65]},
 {0x1C01, 1, &context_state[60], &context_state[67]},
 {0x1601, 1, &context_state[76], &context_state[68]},
 {0x5601, 1, &context_state[62], &context_state[14]},
 {0x5401, 1, &context_state[63], &context_state[61]},
 {0x5101, 1, &context_state[64], &context_state[62]},
 {0x4801, 1, &context_state[65], &context_state[63]},
 {0x3801, 1, &context_state[66], &context_state[64]},
 {0x3401, 1, &context_state[67], &context_state[65]},
 {0x3001, 1, &context_state[68], &context_state[66]},
 {0x2801, 1, &context_state[69], &context_state[66]},
 {0x2401, 1, &context_state[70], &context_state[67]},
 {0x2201, 1, &context_state[71], &context_state[68]},
 {0x1C01, 1, &context_state[72], &context_state[69]},
 {0x1801, 1, &context_state[73], &context_state[70]},
 {0x1601, 1, &context_state[74], &context_state[71]},
 {0x1401, 1, &context_state[75], &context_state[72]},
 {0x1201, 1, &context_state[76], &context_state[73]},
 {0x1101, 1, &context_state[77], &context_state[74]},
 {0x0AC1, 1, &context_state[78], &context_state[75]},
 {0x09C1, 1, &context_state[79], &context_state[76]},
 {0x08A1, 1, &context_state[80], &context_state[77]},
 {0x0521, 1, &context_state[81], &context_state[78]},
 {0x0441, 1, &context_state[82], &context_state[79]},
 {0x02A1, 1, &context_state[83], &context_state[80]},
 {0x0221, 1, &context_state[84], &context_state[81]},
 {0x0141, 1, &context_state[85], &context_state[82]},
 {0x0111, 1, &context_state[86], &context_state[83]},
 {0x0085, 1, &context_state[87], &context_state[84]},
 {0x0049, 1, &context_state[88], &context_state[85]},
 {0x0025, 1, &context_state[89], &context_state[86]},
 {0x0015, 1, &context_state[90], &context_state[87]},
 {0x0009, 1, &context_state[91], &context_state[88]},
 {0x0005, 1, &context_state[92], &context_state[89]},
 {0x0001, 1, &context_state[92], &context_state[90]},
 {0x5601, 1, &context_state[93], &context_state[93]}};

/************************************************************************************************************\
||                  ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                  ||
||                  |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__                   ||
||                  |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]                  ||
||                                                                                                          ||
\************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void initialize_bit_encoder(void)                                                         !
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
!                19.02.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
initialize_bit_encoder(bwc_coder *const coder, const uint8 number_of_contexts)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint8   i;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_bit_coder     *bitcoder;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(coder);
   assert(coder->compressed);

   coder->bitcoder = calloc(1, sizeof(bwc_bit_coder));
   if(!coder->bitcoder)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   bitcoder = coder->bitcoder;

   bitcoder->nContext = number_of_contexts;

   bitcoder->b = coder->compressed;

   bitcoder->context =  calloc(bitcoder->nContext, sizeof(bwc_context_state*));
   if(!bitcoder->context)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   for(i = 0; i < number_of_contexts; ++i)
   {
      bitcoder->context[i] = &context_state[0];
   }

   bitcoder->context[CONTEXT_SIG] = &context_state[ 4];
   bitcoder->context[CONTEXT_RUN] = &context_state[ 3];
   bitcoder->context[CONTEXT_UNI] = &context_state[46];

   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void initialize_bit_encoder(void)                                                         !
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
!                19.02.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
bit_encoder_next_run(bwc_bit_coder *const bitcoder)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_coder_state    *state;
   
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(bitcoder);

   state =  calloc(1, sizeof(bwc_coder_state));
   if(!state)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   if(bitcoder->state)
   {
      memcpy(state, bitcoder->state, sizeof(bwc_coder_state));
      state->prev           = bitcoder->state;
      bitcoder->state->next = state;
      bitcoder->state       = state;
   }
   else
   {
      bitcoder->state = state;
      bitcoder->state->b = bitcoder->b;

      state->A =  0x8000;
      state->t =  0x000C;
      state->L = -0x0001;
   }
   
   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void initialize_bit_encoder(void)                                                         !
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
!                19.02.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bit_encode(bwc_bit_coder *const bitcoder, const uint8 s, const uint8 k)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint16  p;
   
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(bitcoder);
   
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_context_state const **context;
   bwc_coder_state          *state;

   context = bitcoder->context;
   state   = bitcoder->state;

   p = context[k]->p;

   state->A -= p;

   if(s == context[k]->sk)
   {
      if(state->A >= 0x8000)
      {
         state->C += p;
      }
      else
      {
         if(state->A < p)
         {
            state->A = p;
         }
         else
         {
            state->C += p;
         }
         context[k] = context[k]->MPS;

         do
         {
            state->A <<= 1;
            state->C <<= 1;
            state->t --;
            if(state->t == 0)
            {
               transfer_byte(state);
            }
         } while (state->A < 0x8000);
      }
   }
   else
   {
      if(state->A < p)
      {
         state->C += p;
      }
      else
      {
         state->A = p;
      }
      context[k] = context[k]->LPS;

      do
      {
         state->A <<= 1;
         state->C <<= 1;
         state->t --;
         if(state->t == 0)
         {
            transfer_byte(state);
         }
      } while (state->A < 0x8000);
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void initialize_bit_encoder(void)                                                         !
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
!                19.02.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bit_encoder_truncation_length_min(bwc_coder_state *const state)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   #ifdef BWC_SINGLE_PRECISION
      int32   buff, CrDec, CrFrac;
      int32   CArDec, CArFrac;
      uint8   s;
   #else
      int64   buff, Cr, CAr;
      int8    s;
   #endif

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(state);
   
   #ifdef BWC_SINGLE_PRECISION
      CrFrac   = (int32)state->C << state->t;
      CArFrac  = ((int32)state->A << state->t) + CrFrac;

      CrDec    = state->T;
      CArDec   = state->T;

      CrDec   += 0x01 & (CrFrac >> 27);
      CArDec  += 0x01 & (CArFrac >> 27);

      CrFrac  &= 0x7FFFFFF;
      CArFrac &= 0x7FFFFFF;

      s        = 8;

      while((CrDec  >  0xff) || 
            (CArDec <= 0xff))
      {
         buff = (state->L >= 0) ? (int32)state->b[state->L] : 0;
         state->L++;

         CrDec    -= buff << (8 - s);
         CArDec   -= buff << (8 - s);

         CrDec   <<= s;
         CArDec  <<= s;

         CrDec    += CrFrac  >> (27 - s);
         CArDec   += CArFrac >> (27 - s);

         CrFrac  <<= s;
         CArFrac <<= s;

         CrFrac   &= 0x7FFFFFF;
         CArFrac  &= 0x7FFFFFF;

         s = (buff == 0xFF) ? 7 : 8;
      }
   #else
      Cr       = ((int64)state->T << 27) + 
                 ((int64)state->C << state->t);
      CAr      = ((int64)state->A << state->t) + Cr;
      s        = 8;

      while(((Cr  >> 27) >  0xff) || 
            ((CAr >> 27) <= 0xff))
      {
         buff = (state->L >= 0) ? (int64)state->b[state->L] : 0;
         state->L++;

         Cr  -= buff << (35 - s);
         CAr -= buff << (35 - s);

         Cr  <<= s;
         CAr <<= s;

         s = (buff == 0xFF) ? 7 : 8;
      }
   #endif

   if(state->L > 0 && (state->b[state->L - 1] == 0xFF))
   {
      state->L--;
   }
   while(state->L > 1 && (state->b[state->L - 2] == 0xFF) &&
                         (state->b[state->L - 1] == 0x7F))
   {
      state->L -= 2;
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void initialize_bit_encoder(void)                                                         !
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
!                19.02.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bit_encoder_termination(bwc_bit_coder *const bitcoder)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   int8    n;
   
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_coder_state     buff;
   bwc_coder_state    *state;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(bitcoder);

   state   = bitcoder->state;

   buff.A = state->A;
   buff.C = state->C;
   buff.L = state->L;
   buff.T = state->T;
   buff.t = state->t;

   n = 12 - state->t;
   state->C <<= state->t;

   while(n > 0)
   {
      transfer_byte(state);
      n -= state->t;
      state->C <<= state->t;
   }
   transfer_byte(state);

   state->A = buff.A;
   state->C = buff.C;
   state->L = buff.L;
   state->T = buff.T;
   state->t = buff.t;

   while(state->prev)
   {
      state = state->prev;
   }

   while(state)
   {
      bit_encoder_truncation_length_min(state);
      state = state->next;
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void initialize_bit_encoder(void)                                                         !
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
!                19.02.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
free_bit_encoder(bwc_coder *const coder)
{
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(coder);

   if(coder->bitcoder)
   {
      while(coder->bitcoder->state->next)
      {
         coder->bitcoder->state = coder->bitcoder->state->next;
      }

      while(coder->bitcoder->state->prev)
      {
         coder->bitcoder->state = coder->bitcoder->state->prev;
         free(coder->bitcoder->state->next);
      }

      free(coder->bitcoder->state);
      free(coder->bitcoder->context);
   }

   free(coder->bitcoder);
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void initialize_bit_encoder(void)                                                         !
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
!                19.02.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uchar
initialize_bit_decoder(bwc_coder *const coder, const uint8 number_of_contexts, const int64 Lmax)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint8   i;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_bit_coder      *bitcoder;
   bwc_coder_state    *state;
   
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(coder);
   assert(coder->compressed);

   coder->bitcoder = calloc(1, sizeof(bwc_bit_coder));
   if(!coder->bitcoder)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   bitcoder = coder->bitcoder;

   bitcoder->Lmax     = Lmax;
   bitcoder->nContext = number_of_contexts;

   bitcoder->b = coder->compressed;

   bitcoder->context =  calloc(bitcoder->nContext, sizeof(bwc_context_state*));
   if(!bitcoder->context)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   for(i = 0; i < number_of_contexts; ++i)
   {
      bitcoder->context[i] = &context_state[0];
   }

   bitcoder->context[CONTEXT_SIG] = &context_state[ 4];
   bitcoder->context[CONTEXT_RUN] = &context_state[ 3];
   bitcoder->context[CONTEXT_UNI] = &context_state[46];

   bitcoder->state =  calloc(1, sizeof(bwc_coder_state));
   if(!bitcoder->state)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return 1;
   }

   state = bitcoder->state;
   state->b = coder->compressed;

   fill_lsb(state, bitcoder);

   state->C <<= 0x08;

   fill_lsb(state, bitcoder);

   state->C <<= 0x07;
   state->t  -= 0x07;
   state->A   = 0x8000;
   
   return 0;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void initialize_bit_encoder(void)                                                         !
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
!                19.02.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uint8
bit_decode(bwc_bit_coder *const bitcoder, const uint8 k)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint16  p;
   uint8   x;
   
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(bitcoder);
   
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/
   bwc_context_state const **context;
   bwc_coder_state          *state;

   context = bitcoder->context;
   state   = bitcoder->state;

   p = context[k]->p;
   x = context[k]->sk;

   state->A -= p;

   if((state->C >> 8) >= p)
   {
      state->C -= p << 8;
      state->C &= 0xFFFFFF;
      if(~state->A & 0x8000)
      {
         if(state->A < p)
         {
            x = 1 - x;
            context[k] = context[k]->LPS;
         }
         else
         {
            context[k] = context[k]->MPS;
         }
         do
         {
            if(state->t == 0)
            {
               fill_lsb(state, bitcoder);
            }
            state->A <<= 1;
            state->C <<= 1;
            state->t  -= 1;
         } while (~state->A & 0x8000);
      }
   }
   else
   {
      if(state->A <p)
      {
         context[k] = context[k]->MPS;
      }
      else
      {
         x = 1 - x;
         context[k] = context[k]->LPS;
      }
      state->A = p;
      do
      {
         if(state->t == 0)
         {
            fill_lsb(state, bitcoder);
         }
         state->A <<= 1;
         state->C <<= 1;
         state->t  -= 1;
      } while (~state->A & 0x8000);
   }

   return x;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void initialize_bit_encoder(void)                                                         !
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
!                19.02.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
uint64
bit_coder_get_no_bytes(bwc_bit_coder *const bitcoder)
{
   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(bitcoder);

   return (uint64)(bitcoder->state->b - bitcoder->b) + bitcoder->state->L;
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void initialize_bit_encoder(void)                                                         !
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
!                19.02.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bit_coder_reset_ptr(bwc_bit_coder *const bitcoder, uchar *const memory)
{
   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_coder_state    *state;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(bitcoder);

   state = bitcoder->state;

   bitcoder->b = memory;

   while(state)
   {
      state->b = memory;
      state    = state->prev;
   }
}

/*----------------------------------------------------------------------------------------------------------*\
!   FUNCTION NAME: void initialize_bit_encoder(void)                                                         !
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
!                19.02.2019  Patrick Vogler     B87D120     V 0.1.0     function created                     !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
void
bit_coder_get_pass_lengths(bwc_bit_coder *const bitcoder, bwc_encoded_cblk *const encoded_cblk)
{
   /*-----------------------*\
   ! DEFINE INT VARIABLES:   !
   \*-----------------------*/
   uint8   j;

   /*-----------------------*\
   ! DEFINE STRUCTS:         !
   \*-----------------------*/ 
   bwc_coder_state    *state;

   /*-----------------------*\
   ! DEFINE ASSERTIONS:      !
   \*-----------------------*/
   assert(bitcoder);
   assert(encoded_cblk);

   state = bitcoder->state;

   for(j = encoded_cblk->Z; (j --> 0) && state; state = state->prev)
   {
      encoded_cblk->L[j] = (uint64)(state->b - bitcoder->b) + state->L;
   }
}