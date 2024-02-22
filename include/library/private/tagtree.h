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
||        This file defines a tagtree procedure used to encode/decode two types of                ||
||        information found defining in a codeblock in specific quality layer:                    ||
||                                                                                                ||
||                - The inclusion tag records if a codeblock has any contribution                 ||
||                  to a quality layer.                                                           ||
||                - The number of leading bitplanes that are not significant/only                 ||
||                  populated by zero bits.                                                       ||
||                                                                                                ||
||        For more information on the encoding/decoding process please refere to JPEG2000         ||
||        by D. S. Taubman and M. W. Marcellin (p. 384).                                          ||
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
#ifndef TAGTREE_H
#define TAGTREE_H
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
  void         kill_tagtree               (bwc_tagtree                  *const  tagtree);
  //==========|==========================|======================|======|=======|====================
  void         reset_tagtree              (bwc_tagtree                  *const  tagtree);
  //==========|==========================|======================|======|=======|====================
  bwc_tagtree* initialize_tagtree         (uint64                const          leafsX,
                                           uint64                const          leafsY,
                                           uint64                const          leafsZ,
                                           uint64                const          leafsTS);
  //==========|==========================|======================|======|=======|====================
  uint16       tagtree_get_value          (bwc_tagtree           const  *const  tagtree, 
                                           uint64                const          leaf_index);
  //==========|==========================|======================|======|=======|====================
  void         tagtree_set_value          (bwc_tagtree                  *const  tagtree, 
                                           uint64                const          leaf_index,
                                           uint16                const          value);
  //==========|==========================|======================|======|=======|====================
  void         encode_tagtree             (bwc_tagtree                  *const  tagtree,
                                           bwc_stream                   *const  stream,
                                           uint32                const          threshold, 
                                           uint32                const          leaf_index, 
                                           uchar                 const          estimate);
  //==========|==========================|======================|======|=======|====================
  uchar        decode_tagtree             (bwc_tagtree                  *const  tagtree,
                                           bwc_stream                   *const  stream, 
                                           uint32                const          threshold, 
                                           uint32                const          leaf_index);
#endif