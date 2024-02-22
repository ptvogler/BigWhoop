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

  /************************************************************************************************\
  ||            ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____            ||
  ||            |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__             ||
  ||            |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]            ||
  ||                                                                                              ||
  \************************************************************************************************/
  //==========|==========================|======================|======|=======|====================
  bwc_data*    bwc_initialize_data        (double                       *const  field,
                                           uint64                const          nX,
                                           uint64                const          nY,
                                           uint64                const          nZ,
                                           uint16                const          nTS,
                                           uint8                 const          nPar,
                                           char                         *const  file_extension);
  //==========|==========================|======================|======|=======|====================
  void         bwc_add_param              (bwc_data                     *const  data, 
                                           char                         *const  name,
                                           uint8                 const          precision);
  //==========|==========================|======================|======|=======|====================
  void         bwc_get_data               (bwc_data                     *const  data,
                                           uchar                        *const  buffer,
                                           uint64                const          size);
  //==========|==========================|======================|======|=======|====================
  void         bwc_free_data              (bwc_data                     *const  data);
  //==========|==========================|======================|======|=======|====================
  uchar        create_field               (bwc_field                    *const  field);
  //==========|==========================|======================|======|=======|====================
  void         bwc_kill_compression       (bwc_field                    *const  field);
  //==========|==========================|======================|======|=======|====================
  bwc_field*   bwc_initialize_field       (bwc_data                     *const  data);
  //==========|==========================|======================|======|=======|====================
  void         bwc_set_error_resilience   (bwc_field                    *const  field);
  //==========|==========================|======================|======|=======|====================
  void         bwc_set_quant_style        (bwc_field                    *const  field, 
                                           bwc_quant_st          const          quantization_style);
  //==========|==========================|======================|======|=======|====================
  void         bwc_set_quant_step_size    (bwc_field                    *const  field,
                                           double                const          delta);
  //==========|==========================|======================|======|=======|====================
  void         bwc_set_progression        (bwc_field                    *const  field,
                                           bwc_prog_ord          const          progression);
  //==========|==========================|======================|======|=======|====================
  void         bwc_set_kernels            (bwc_field                    *const  field,
                                           bwc_dwt_filter        const          KernelX,
                                           bwc_dwt_filter        const          KernelY,
                                           bwc_dwt_filter        const          KernelZ,
                                           bwc_dwt_filter        const          KernelTS);
  //==========|==========================|======================|======|=======|====================
  void         bwc_set_decomp             (bwc_field                    *const  field,
                                           uint8                 const          decompX,
                                           uint8                 const          decompY,
                                           uint8                 const          decompZ,
                                           uint8                 const          decompTS);
  //==========|==========================|======================|======|=======|====================
  void         bwc_set_precincts          (bwc_field                    *const  field,
                                           uint8                 const          pX,
                                           uint8                 const          pY,
                                           uint8                 const          pZ,
                                           uint8                 const          pTS);
  //==========|==========================|======================|======|=======|====================
  void         bwc_set_codeblocks         (bwc_field                    *const  field,
                                           uint8                 const          cbX,
                                           uint8                 const          cbY,
                                           uint8                 const          cbZ,
                                           uint8                 const          cbTS);
  //==========|==========================|======================|======|=======|====================
  void         bwc_set_qm                 (bwc_field                    *const  field,
                                           uint8                 const          Qm);
  //==========|==========================|======================|======|=======|====================
  void         bwc_set_tiles              (bwc_field                    *const  field, 
                                           uint64                const          tilesX,
                                           uint64                const          tilesY,
                                           uint64                const          tilesZ,
                                           uint64                const          tilesTS,
                                           bwc_tile_instr        const          instr);
  //==========|==========================|======================|======|=======|====================
  uchar        bwc_create_compression     (bwc_field                    *const  field,
                                           char                         *const  rate_control);
  //==========|==========================|======================|======|=======|====================
  uchar        bwc_compress               (bwc_field                    *const  field,
                                           bwc_data                     *const  data);
  //==========|==========================|======================|======|=======|====================
  bwc_field*   bwc_create_decompression   (bwc_data                     *const  data,
                                           uint8                 const          layer);
  //==========|==========================|======================|======|=======|====================
  uchar        bwc_decompress             (bwc_field                    *const  field,
                                           bwc_data                     *const  data);
#endif