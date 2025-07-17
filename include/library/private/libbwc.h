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
||        Big Whoop is a compression codec for the lossy compression of IEEE 754 floating         ||
||        point arrays defined on curvelinear compute grids.                                      ||
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
#ifndef LIBBWC_H
#define LIBBWC_H
  /************************************************************************************************\
  ||                               _ _  _ ____ _    _  _ ___  ____                                ||
  ||                               | |\ | |    |    |  | |  \ |___                                ||
  ||                               | | \| |___ |___ |__| |__/ |___                                ||
  ||                                                                                              ||
  \************************************************************************************************/
  #include "bitstream.h"
  #include "codestream.h"
  #include "constants.h"
  #include "macros.h"
  #include "mq_types.h"
  #include "types.h"

  /* Definitions used to compile on Windows.                */
  #ifdef _MSC_VER
    #define strtok_r strtok_s
  #endif

  /************************************************************************************************\
  ||            ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____            ||
  ||            |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__             ||
  ||            |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]            ||
  ||                                                                                              ||
  \************************************************************************************************/
  //==========|==========================|======================|======|=======|====================
  bwc_stream*  bwc_init_stream            (void                         *const  inpbuf,
                                           void                         *const  outbuf,
                                           bwc_mode              const          mode);
  //==========|==========================|======================|======|=======|====================
  uchar        bwc_set_com                (bwc_stream                   *const  stream,
                                           char                  const  *const  com, 
                                           uint16                const          size);
  //==========|==========================|======================|======|=======|====================
  uchar        bwc_set_aux                (bwc_stream                   *const  stream,
                                           char                  const  *const  aux, 
                                           uint32                const          size);
  //==========|==========================|======================|======|=======|====================
  uchar        create_codec               (bwc_codec                    *const  codec);
  //==========|==========================|======================|======|=======|====================
  bwc_codec*   configure_codec            (bwc_codec                    *const  codec,
                                           uint64                const          nX,
                                           uint64                const          nY,
                                           uint64                const          nZ,
                                           uint64                const          nTS,
                                           uint8                 const          nPar,
                                           bwc_precision         const          prec);
  //==========|==========================|======================|======|=======|====================
  bwc_codec*   bwc_alloc_coder            (uint64                const          nX,
                                           uint64                const          nY,
                                           uint64                const          nZ,
                                           uint64                const          nTS,
                                           uint8                 const          nPar,
                                           bwc_precision         const          prec);
  //==========|==========================|======================|======|=======|====================
  bwc_codec*   bwc_alloc_decoder          ();
  //==========|==========================|======================|======|=======|====================
  void         bwc_free_codec             (bwc_codec                    *const  codec);
  //==========|==========================|======================|======|=======|====================
  void         bwc_set_error_resilience   (bwc_codec                    *const  codec);
  //==========|==========================|======================|======|=======|====================
  void         set_quant_style            (bwc_codec                    *const  codec, 
                                           bwc_quant_st          const          quantization_style);
  //==========|==========================|======================|======|=======|====================
  void         set_quant_step_size        (bwc_codec                    *const  codec,
                                           double                const          delta);
  //==========|==========================|======================|======|=======|====================
  void         set_progression            (bwc_codec                    *const  codec,
                                           bwc_prog_ord          const          progression);
  //==========|==========================|======================|======|=======|====================
  void         set_kernels                (bwc_codec                    *const  codec,
                                           bwc_dwt_filter        const          KernelX,
                                           bwc_dwt_filter        const          KernelY,
                                           bwc_dwt_filter        const          KernelZ,
                                           bwc_dwt_filter        const          KernelTS);
  //==========|==========================|======================|======|=======|====================
  void         bwc_set_decomp             (bwc_codec                    *const  codec,
                                           uint8                 const          decompX,
                                           uint8                 const          decompY,
                                           uint8                 const          decompZ,
                                           uint8                 const          decompTS);
  //==========|==========================|======================|======|=======|====================
  void         bwc_set_precincts          (bwc_codec                    *const  codec,
                                           uint8                 const          pX,
                                           uint8                 const          pY,
                                           uint8                 const          pZ,
                                           uint8                 const          pTS);
  //==========|==========================|======================|======|=======|====================
  void         bwc_set_codeblocks         (bwc_codec                    *const  codec,
                                           uint8                 const          cbX,
                                           uint8                 const          cbY,
                                           uint8                 const          cbZ,
                                           uint8                 const          cbTS);
  //==========|==========================|======================|======|=======|====================
  void         bwc_set_qm                 (bwc_codec                    *const  codec,
                                           uint8                 const          Qm);
  //==========|==========================|======================|======|=======|====================
  void         bwc_set_tiles              (bwc_codec                    *const  codec, 
                                           uint64                const          tilesX,
                                           uint64                const          tilesY,
                                           uint64                const          tilesZ,
                                           uint64                const          tilesTS,
                                           bwc_tile_instr        const          instr);
  //==========|==========================|======================|======|=======|====================
  bwc_header*  bwc_open_header            (void                         *const  inpbuf);
  //==========|==========================|======================|======|=======|====================
  void         bwc_close_header           (bwc_header                   *const  header);
  //==========|==========================|======================|======|=======|====================
  uchar        bwc_create_compression     (bwc_codec                    *const  codec,
                                           bwc_stream                   *const  data,
                                           char                         *const  rate_control);
  //==========|==========================|======================|======|=======|====================
  size_t       bwc_compress               (bwc_codec                    *const  codec,
                                           bwc_stream                   *const  data);
  //==========|==========================|======================|======|=======|====================
  uchar        bwc_create_decompression   (bwc_codec                    *const  codec,
                                           bwc_stream                   *const  stream,
                                           uint8                 const          layer);
  //==========|==========================|======================|======|=======|====================
  uchar        bwc_decompress             (bwc_codec                    *const  codec,
                                           bwc_stream                   *const  stream);
#endif
