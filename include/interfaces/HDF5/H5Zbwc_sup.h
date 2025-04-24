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
 *        @file   H5Zbwc_sup.h
 *
 *        This code provides supplemental functionality to BigWhoops HDF5 plugin.
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
#ifndef H5ZBWC_SUP_H
#define H5ZBWC_SUP_H
  /************************************************************************************************\
  ||                               _ _  _ ____ _    _  _ ___  ____                                ||
  ||                               | |\ | |    |    |  | |  \ |___                                ||
  ||                               | | \| |___ |___ |__| |__/ |___                                ||
  ||                                                                                              ||
  \************************************************************************************************/
  #include "stdint.h"

  /************************************************************************************************\
  ||            ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____            ||
  ||            |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__             ||
  ||            |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]            ||
  ||                                                                                              ||
  \************************************************************************************************/
  //==========|==========================|======================|======|=======|====================
  void         H5Pset_bwc_err_resilience  (size_t                               CN, 
                                           unsigned int          const          CD[/*CN*/]);
  //==========|==========================|======================|======|=======|====================
  void         H5Pset_bwc_bitrate         (float                                BR,
                                           size_t                               CN, 
                                           unsigned int          const          CD[/*CN*/]);
  //==========|==========================|======================|======|=======|====================
  void         H5Pset_bwc_codeblocks      (uint32_t                             CB,
                                           size_t                               CN, 
                                           unsigned int          const          CD[/*CN*/]);
  //==========|==========================|======================|======|=======|====================
  void         H5Pset_bwc_codeblocks_d    (uint32_t                             CX, 
                                           uint32_t                             CY,
                                           uint32_t                             CZ,
                                           uint32_t                             CT,
                                           size_t                               CN,
                                           unsigned int          const          CD[/*CN*/]);
  //==========|==========================|======================|======|=======|====================
  void         H5Pset_bwc_precinct        (uint32_t                             PC,
                                           size_t                               CN,
                                           unsigned int          const          CD[/*CN*/]);
  //==========|==========================|======================|======|=======|====================
  void         H5Pset_bwc_precinct_d      (uint32_t                             PX, 
                                           uint32_t                             PY,
                                           uint32_t                             PZ,
                                           uint32_t                             PT,
                                           size_t                               CN,
                                           unsigned int          const          CD[/*CN*/]);
  //==========|==========================|======================|======|=======|====================
  void         H5Pset_bwc_tile            (uint64_t                             TL,
                                           size_t                               CN,
                                           unsigned int          const          CD[/*CN*/]);
  //==========|==========================|======================|======|=======|====================
  void         H5Pset_bwc_tile_d          (uint64_t                             TX, 
                                           uint64_t                             TY,
                                           uint64_t                             TZ,
                                           uint64_t                             TT,
                                           size_t                               CN,
                                           unsigned int          const          CD[/*CN*/]);
  //==========|==========================|======================|======|=======|====================
  void         H5Pset_bwc_decomp          (uint32_t                             DL,
                                           size_t                               CN,
                                           unsigned int          const          CD[/*CN*/]);
  //==========|==========================|======================|======|=======|====================
  void         H5Pset_bwc_decomp_d        (uint32_t                             DX, 
                                           uint32_t                             DY,
                                           uint32_t                             DZ,
                                           uint32_t                             DT,
                                           size_t                               CN,
                                           unsigned int          const          CD[/*CN*/]);
  //==========|==========================|======================|======|=======|====================
  void         H5Pset_bwc_qm              (uint8_t                              QM,
                                           size_t                               CN,
                                           unsigned int          const          CD[/*CN*/]);
  //==========|==========================|======================|======|=======|====================
  void H5Pset_bwc_ul                      (uint8_t                              UL,
                                           size_t                               CN,
                                           unsigned int          const          CD[/*CN*/]);

#endif