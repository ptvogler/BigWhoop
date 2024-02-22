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
||        DESCRIPTION NEEDED.                                                                     ||
||       |                                                                                |       ||
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
#ifndef MQ_H
#define MQ_H
  /************************************************************************************************\
  ||                               _ _  _ ____ _    _  _ ___  ____                                ||
  ||                               | |\ | |    |    |  | |  \ |___                                ||
  ||                               | | \| |___ |___ |__| |__/ |___                                ||
  ||                                                                                              ||
  \************************************************************************************************/
  #include "tier1.h"
  #include "types.h"

  /************************************************************************************************\
  ||                          ____ ____ _  _ ____ ___ ____ _  _ ___ ____                          ||
  ||                          |    |  | |\ | [__   |  |__| |\ |  |  [__                           ||
  ||                          |___ |__| | \| ___]  |  |  | | \|  |  ___]                          ||
  ||                                                                                              ||
  \************************************************************************************************/
  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         These macros define the context state indices used by the mq coder for                 !
  !         probability estimation. For a more thorough treatment of the context assignments       !
  !         see page 487 JPEG2000 by David S. Taubman and Michael W. Marcellin.                    !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  #define CONTEXT_SIG           0                         // Sig. context labels starting indices
  #define CONTEXT_RUN           9                         // Indices for the run context label
  #define CONTEXT_SIGN          10                        // Sign context labels starting indices
  #define CONTEXT_MAG           15                        // Mag. context labels starting indices
  #define CONTEXT_UNI           18                        // Indices for the uni context label
  #define CONTEXT_TOTAL         19                        // Total number of context labels

  /************************************************************************************************\
  ||            ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____            ||
  ||            |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__             ||
  ||            |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]            ||
  ||                                                                                              ||
  \************************************************************************************************/
  uchar        initialize_mq_encoder      (bwc_coder                    *const  coder,
                                           uint8                 const          number_of_contexts);
  //==========|==========================|======================|======|=======|====================
  uchar        mq_next_run                (bwc_bit_coder                *const  bitcoder);
  //==========|==========================|======================|======|=======|====================
  void         mq_bit_encode              (bwc_bit_coder                *const  bitcoder,
                                           uint8                 const          s, 
                                           uint8                 const          k);
  //==========|==========================|======================|======|=======|====================
  void         mq_truncation_length_min   (bwc_coder_state              *const  state);
  //==========|==========================|======================|======|=======|====================
  void         mq_termination             (bwc_bit_coder                *const  bitcoder);
  //==========|==========================|======================|======|=======|====================
  void         free_mq_encoder            (bwc_coder                    *const  coder);
  //==========|==========================|======================|======|=======|====================
  uchar        initialize_mq_decoder      (bwc_coder                    *const  coder,
                                           uint8                 const          number_of_contexts,
                                           int64                 const          Lmax);
  //==========|==========================|======================|======|=======|====================
  uint8        mq_bit_decode              (bwc_bit_coder                *const  bitcoder,
                                           uint8                 const          k);
  //==========|==========================|======================|======|=======|====================
  uint64       mq_get_no_bytes            (bwc_bit_coder                *const  bitcoder);
  //==========|==========================|======================|======|=======|====================
  void         mq_get_pass_lengths        (bwc_bit_coder                *const  bitcoder,
                                           bwc_encoded_cblk             *const  encoded_cblk);
  //==========|==========================|======================|======|=======|====================
  void         mq_reset_ptr               (bwc_bit_coder                *const  bitcoder,
                                           uchar                        *const  memory);
#endif