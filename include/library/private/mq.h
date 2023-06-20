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
||      File:  mq.h                                                                                                                 ||
||      -----                                                                                                                       ||
||                                                                                                                                  ||
||      DESCRIPTION:                                                                                                                ||
||      ------------                                                                                                                ||
||      DESCRIPTION NEEDED.                                                                                                         ||
||                                                                                                                                  ||
||      STRUCTS:                                                                                                                    ||
||      --------                                                                                                                    ||
||                                                                                                                                  ||
||      PUBLIC FUNCTIONS:                                                                                                           ||
||      -----------------                                                                                                           ||
||                                                                                                                                  ||
||      DEVELOPMENT HISTORY:                                                                                                        ||
||      --------------------                                                                                                        ||
||                                                                                                                                  ||
||                            Date        Author             Change Id   Release     Description Of Change                          ||
||                            ----        ------             ---------   -------     ---------------------                          ||
||                            13.02.2019  Patrick Vogler     B87D120     V 0.1.0     header file created                            ||
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
#ifndef MQ_H
#define MQ_H

   /************************************************************************************************************\
   ||                                      _ _  _ ____ _    _  _ ___  ____                                     ||
   ||                                      | |\ | |    |    |  | |  \ |___                                     ||
   ||                                      | | \| |___ |___ |__| |__/ |___                                     ||
   ||                                                                                                          ||
   \************************************************************************************************************/
      #include "types.h"

   /************************************************************************************************************\
   ||                                ____ ____ _  _ ____ ___ ____ _  _ ___ ____                                ||
   ||                                |    |  | |\ | [__   |  |__| |\ |  |  [__                                 ||
   ||                                |___ |__| | \| ___]  |  |  | | \|  |  ___]                                ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !              These macros define the context state indices used by the mq coder for probability estima-    !
   !              tion. For a more thorough treatment of the context assignments see page 487 JPEG2000 by       !
   !              David S. Taubman and Michael W. Marcellin.                                                     !
   !                                                                                                            !
   !   Macros:                                                                                                  !
   !   -------                                                                                                  !
   !                Macro                                Description                                            !
   !                -----                                -----------                                            !
   !                CONTEXT_SIG                        - Starting indices for the significance context labels.  !
   !                CONTEXT_RUN                        - Indices for the run context label.                     !
   !                CONTEXT_SIGN                       - Starting indices for the sign context labels.          !
   !                CONTEXT_MAG                        - Starting indices for the magnitude context labels.     !
   !                CONTEXT_UNI                        - Indices for the uni context label.                     !
   !                CONTEXT_TOTAL                      - Total number of context labels.                        !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                21.02.2019  Patrick Vogler     B87D120     V 0.1.0     Macros created                       !
   \*----------------------------------------------------------------------------------------------------------*/
   #define CONTEXT_SIG    0
   #define CONTEXT_RUN    9
   #define CONTEXT_SIGN  10
   #define CONTEXT_MAG   15
   #define CONTEXT_UNI   18
   #define CONTEXT_TOTAL 19

   /************************************************************************************************************\
   ||                  ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                  ||
   ||                  |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__                   ||
   ||                  |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]                  ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   /*----------------------------------------------------------------------------------------------------------*\
   !   TYPE NAME: Template                                                                                      !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                DESCRIPTION NEEDED                                                                          !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   uchar
   initialize_bit_encoder(bwc_coder *const coder, const uint8 number_of_contexts);

   uchar
   bit_encoder_next_run(bwc_bit_coder *const bitcoder);

   void
   bit_encode(bwc_bit_coder *const bitcoder, const uint8 s, const uint8 k);

   void
   bit_encoder_truncation_length_min(bwc_coder_state *const state);

   void
   bit_encoder_termination(bwc_bit_coder *const bitcoder);

   void
   free_bit_encoder(bwc_coder *const coder);

   uchar
   initialize_bit_decoder(bwc_coder *const coder, const uint8 number_of_contexts, const int64 Lmax);

   uint8
   bit_decode(bwc_bit_coder *const bitcoder, const uint8 k);

   uint64
   bit_coder_get_no_bytes(bwc_bit_coder *const bitcoder);

   void
   bit_coder_get_pass_lengths(bwc_bit_coder *const bitcoder, bwc_encoded_cblk *const encoded_cblk);

   void
   bit_coder_reset_ptr(bwc_bit_coder *const bitcoder, uchar *const memory);
#endif