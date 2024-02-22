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
||        This file describes a set of functions that can be used to create, manipulate           ||
||        and terminate a bitstream. These functions facilitate the creation or reading           ||
||        of a compressed bwc codestream and can emit/extract information on a per bit,           ||
||        symbol (64-bit) or string basis.                                                        ||
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
#ifndef BITSTREAM_H
#define BITSTREAM_H
  /************************************************************************************************\
  ||                               _ _  _ ____ _    _  _ ___  ____                                ||
  ||                               | |\ | |    |    |  | |  \ |___                                ||
  ||                               | | \| |___ |___ |__| |__/ |___                                ||
  ||                                                                                              ||
  \************************************************************************************************/
  #include "types.h"

  /************************************************************************************************\
  ||            ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____            ||
  ||            |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__             ||
  ||            |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]            ||
  ||                                                                                              ||
  \************************************************************************************************/
  uint64       bytes_used                 (bwc_stream            const  *const  stream);
  //==========|==========================|======================|======|======|=====================
  bwc_stream*  bwc_init_stream            (uchar                        *const  memory, 
                                           uint32                const          size, 
                                           char                  const          instr);
  //==========|==========================|======================|======|======|=====================
  void         bwc_emit_chunck            (bwc_stream                   *const  stream, 
                                           uchar                 const  *const  chunck, 
                                           uint64                const          size);
  //==========|==========================|======================|======|======|=====================
  void         bwc_emit_symbol            (bwc_stream                   *const  stream, 
                                           uint64                const          symbol,
                                           uint8                 const          size);
  //==========|==========================|======================|======|======|=====================
  void         bwc_emit_bit               (bwc_stream                   *const  stream, 
                                           uint64                const          bit);
  //==========|==========================|======================|======|======|=====================
  void         flush_stream               (bwc_stream                   *const  stream);
  //==========|==========================|======================|======|======|=====================
  uchar*       bwc_get_chunck             (bwc_stream                   *const  stream, 
                                           uint64                const          length);
  //==========|==========================|======================|======|======|=====================
  uint64       bwc_get_symbol             (bwc_stream                   *const  stream, 
                                           uint8                 const          length);
  //==========|==========================|======================|======|======|=====================
  uchar        bwc_get_bit                (bwc_stream                   *const  stream);
  //==========|==========================|======================|======|======|=====================
  uchar        bwc_terminate_stream       (bwc_stream                          *stream, 
                                           bwc_packed_stream            *const  packed_stream);
  //==========|==========================|======================|======|======|=====================
  void         release_packed_stream      (bwc_packed_stream            *const  stream);
#endif