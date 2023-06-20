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
||      File:  prim_types_double.h                                                                                                  ||
||      -----                                                                                                                       ||
||                                                                                                                                  ||
||      DESCRIPTION:                                                                                                                ||
||      ------------                                                                                                                ||
||      This header defines a set of derrived types used for the mq encoder during the entropy encoding stage of the big whoop      ||
||      compression algorithm.                                                                                                      ||
||                                                                                                                                  ||
||      STRUCTS:                                                                                                                    ||
||      --------                                                                                                                    ||
||      - bwc_context_state                                                                                                         ||
||      - bwc_coder_state                                                                                                           ||
||      - bwc_bit_coder                                                                                                             ||
||                                                                                                                                  ||
||      DEVELOPMENT HISTORY:                                                                                                        ||
||      --------------------                                                                                                        ||
||                                                                                                                                  ||
||                            Date        Author             Change Id   Release     Description Of Change                          ||
||                            ----        ------             ---------   -------     ---------------------                          ||
||                            20.02.2019  Patrick Vogler     B87D120     V 0.1.0     header file created                            ||
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
#ifndef MQ_TYPES_H
#define MQ_TYPES_H

   /************************************************************************************************************\
   ||                                      _ _  _ ____ _    _  _ ___  ____                                     ||
   ||                                      | |\ | |    |    |  | |  \ |___                                     ||
   ||                                      | | \| |___ |___ |__| |__/ |___                                     ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   #include <stdint.h>

   /************************************************************************************************************\
   ||                     ___  ____ ____ ____ _ _  _ ____ ___     ___ _   _ ___  ____ ____                     ||
   ||                     |  \ |___ |__/ |__/ | |  | |___ |  \     |   \_/  |__] |___ [__                      ||
   ||                     |__/ |___ |  \ |  \ |  \/  |___ |__/     |    |   |    |___ ___]                     ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_context_state                                                                           !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                -                                                                                           !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                p                           unsigned int(16 bit)  - LPS Probability estimate.               !
   !                                                                                                            !
   !                sk                          unsigned int(8 bit)   - Most Probable Symbol.                   !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                MPS                         context*              - New context if the coded symbol is a    !
   !                                                                    Most Probable Symbol (MPS).             !
   !                                                                                                            !
   !                LPS                         context*              - New context if the coded symbol is a    !
   !                                                                    Least Probable Symbol (LPS).            !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                14.02.2019  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct context
   {
      uint16_t                    p;
      uint8_t                     sk;
      const struct context *const MPS;
      const struct context *const LPS;
   } bwc_context_state;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_coder_state                                                                             !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                -                                                                                           !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                L                           int(64 bit)           - Number of code bytes which have been    !
   !                                                                    generated so far.                       !
   !                                                                                                            !
   !                C                           unsigned int(32 bit)  - Lower bound register used to define     !
   !                                                                    the lower bound of the coding interval. !
   !                                                                                                            !
   !                A                           unsigned int(16 bit)  - Length register used to define the up-  !
   !                                                                    bound of the coding interval.           !
   !                                                                                                            !
   !                t                           int(8 bit)            - Down counter which is used to evaluate  !
   !                                                                    when partially generated code bits      !
   !                                                                    should be moved out of the C register   !
   !                                                                    in to the temporary byte buffer b.      !
   !                                                                                                            !
   !                b                           unsigned char*        - Byte buffer.                            !
   !                                                                                                            !
   !                T                           unsigned char         - Temporary byte buffer.                  !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                next                        state*                - Coder state of the next coding pass.    !
   !                                                                                                            !
   !                prev                        state*                - Coder state of the previous coding      !
   !                                                                    pass.                                   !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                14.02.2019  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct state
   {
      int64_t              L;
      uint32_t             C;
      uint16_t             A;
      int8_t               t;
      unsigned char       *b;
      unsigned char        T;
      struct state        *next;
      struct state        *prev;
   } bwc_coder_state;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_bit_coder                                                                               !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                -                                                                                           !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                Lmax                        unsigned int(64 bit)  - Number of code bytes (used by decoder). !
   !                                                                                                            !
   !                nContext                    unsigned int(8 bit)   - Number of context states that need to   !
   !                                                                    be tracked during (de)compression.      !
   !                                                                                                            !
   !                b                           unsigned char*        - Temporary byte buffer.                  !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                state                       bwc_coder_state*      - Coder state for the current coding      !
   !                                                                    pass.                                   !
   !                                                                                                            !
   !                context                     bwc_coder_state*      - Context states for the current coding   !
   !                                                                    pass.                                   !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                14.02.2019  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      int64_t                   Lmax;
      uint8_t                   nContext;
      unsigned char            *b;
      bwc_coder_state          *state;
      bwc_context_state const **context;
   } bwc_bit_coder;
#endif