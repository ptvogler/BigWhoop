/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
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
\*  --------------------------------------------------------------------------------------------  */
/**                                                                                               
 *        @file mq_types.h
 *
 *        This header defines a set of derrived types that describe the MQ coder and it's
 *        coder and context state utilized by the fractional bit plane coding stage.
 *                                                                                                */
/*  --------------------------------------------------------------------------------------------  *\
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
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#ifndef MQ_TYPES_H
#define MQ_TYPES_H
  /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
  ||                               _ _  _ ____ _    _  _ ___  ____                                ||
  ||                               | |\ | |    |    |  | |  \ |___                                ||
  ||                               | | \| |___ |___ |__| |__/ |___                                ||
  ||                                                                                              ||
  \*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
  #include <stdint.h>                                     //!< Fixed-width integers

  /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
  ||               ___  ____ ____ ____ _ _  _ ____ ___     ___ _   _ ___  ____ ____               ||
  ||               |  \ |___ |__/ |__/ | |  | |___ |  \     |   \_/  |__] |___ [__                ||
  ||               |__/ |___ |  \ |  \ |  \/  |___ |__/     |    |   |    |___ ___]               ||
  ||                                                                                              ||
  \*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
  /*==============================================================================================*/
  /**
   * @details Context state used for probability estimation by the MQ arithmetic encoder
   *          when encodeing a Most Probablye (MPS) or Least Probable Symbol (LPS).
   */
  /*=====================================================|========================================*/
  typedef struct context
  {
    uint16_t                    p;                        //!< LPS probability estimation
    uint8_t                     sk;                       //!< Most probable symbol (MPS)

    const struct context *const MPS;                      //!< New context for encoded (MPS)
    const struct context *const LPS;                      //!< New context for encoded (LPS)
  } bwc_context_state;

  /*==============================================================================================*/
  /**
   * @details Coder state used for codeword generation and bitstream assembly.
   */
  /*=====================================================|========================================*/
  typedef struct state
  {
    int64_t                     L;                        //!< Number of generated code bytes
    uint32_t                    C;                        //!< Lower bound register
    uint16_t                    A;                        //!< Half open interval length
    int8_t                      t;                        //!< Down counter: partial codeword to buf

    unsigned char              *b;                        //!< Byte buffer
    unsigned char               T;                        //!< Temporary byte buffer

    struct state               *next;                     //!< Next coding phase state
    struct state               *prev;                     //!< Previous coding phase state
  } bwc_coder_state;

  /*==============================================================================================*/
  /**
   * @details MQ coder instance.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    int64_t                     Lmax;                     //!< Number of code bytes
    uint8_t                     nContext;                 //!< Number of context states

    unsigned char              *b;                        //!< Temporary byte buffer

    bwc_coder_state            *state;                    //!< Coder state variables
    bwc_context_state const   **context;                  //!< Probabilty Mapper
  } bwc_bit_coder;
#endif