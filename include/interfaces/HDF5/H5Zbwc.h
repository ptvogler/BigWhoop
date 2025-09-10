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
 *        @file   H5Zbwc.h
 *
 *        This header file aims to provide additional functionality to the BigWhoop HDF5
 *        plugin via macros used to manipulate the cd_values arrays. The BigWhoop plugin 
 *        expects a cd_values array of 26 values with the following structure:
 * 
 *        |=|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|
 *        |0| 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|
 *        |-|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|--|
 *        |f|sizeX|sizeY|sizeZ|sizeT|np|dt|er|br|cb|pc|tileX|tileY|tileZ|tileT|DL|QM|UL|
 *        |=|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|==|
 * 
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
  #include <stdint.h>

  /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
  ||                                _  _ ____ ____ ____ ____ ____                                 ||
  ||                                |\/| |__| |    |__/ |  | [__                                  ||
  ||                                |  | |  | |___ |  \ |__| ___]                                 ||
  ||                                                                                              ||
  \*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
  /*==============================================================================================*/
  /**
   * @details Setter macro for code-stream error resilience.
   */
  /*===================================================|==========================================*/
  #define H5Pset_bwc_error_resilience(CN, CD)           \
  do {                                                  \
    if (CN >= 26 && (CD[0] & (0x01 << 0)) == 0)         \
      {                                                 \
        CD[0] ^= (0x01 << 0);                           \
        CD[11] = 1;                                     \
      }                                                 \
  } while(0)

  /*==============================================================================================*/
  /**
   * @details Setter macro for code-stream bit-rate.
   */
  /*===================================================|==========================================*/
  #define H5Pset_bwc_bitrate(BR, CN, CD)                \
  do {                                                  \
    if (CN >= 26 && (CD[0] & (0x01 << 1)) == 0)         \
      {                                                 \
        CD[0] ^= (0x01 << 1);                           \
        CD[12] = *(uint32_t *)&BR;                       \
      }                                                 \
  } while(0)

  /*==============================================================================================*/
  /**
   * @details Setter macro for the code-block size using a single, global value.
   */
  /*===================================================|==========================================*/
  #define H5Pset_bwc_codeblocks(CB, CN, CD)             \
  do {                                                  \
    if (CN >= 26 && ((CD[0] & (0x01 << 2)) == 0) &&     \
        CB >= 0 && CB < 512)                            \
      {                                                 \
        CD[0] ^= (0x01 << 2);                           \
        CD[13] = ((0xFF & CB) << 24)  |                 \
                 ((0xFF & CB) << 16)  |                 \
                 ((0xFF & CB) << 8)   |                 \
                 ((0xFF & CB));                         \
      }                                                 \
  } while(0)

  /*==============================================================================================*/
  /**
   * @details Setter macro for the code-block size.
   */
  /*===================================================|==========================================*/
  #define H5Pset_bwc_codeblocks_d(CX,CY,CZ,CT,CN,CD)    \
  do {                                                  \
    if (CN >= 26 && ((CD[0] & (0x01 << 2)) == 0)   &&   \
        CX >= 0 && CX < 512 && CY >= 0 && CY < 512 &&   \
        CZ >= 0 && CZ < 512 && CT >= 0 && CT < 512)     \
      {                                                 \
        CD[0] ^= (0x01 << 2);                           \
        CD[13] = ((0xFF & CX) << 24)  |                 \
                 ((0xFF & CY) << 16)  |                 \
                 ((0xFF & CZ) << 8)   |                 \
                 ((0xFF & CT));                         \
      }                                                 \
  } while(0)

  /*==============================================================================================*/
  /**
   * @details Setter macro for the precinct size using a single, global value.
   */
  /*===================================================|==========================================*/
  #define H5Pset_bwc_precinct(PC, CN, CD)               \
  do {                                                  \
    if (CN >= 26 && ((CD[0] & (0x01 << 3)) == 0) &&     \
        CB >= 0 && CB < 512)                            \
      {                                                 \
        CD[0] ^= (0x01 << 3);                           \
        CD[14] = ((0xFF & CB) << 24)  |                 \
                 ((0xFF & CB) << 16)  |                 \
                 ((0xFF & CB) << 8)   |                 \
                 ((0xFF & CB));                         \
      }                                                 \
  } while(0)

  /*==============================================================================================*/
  /**
   * @details Setter macro for the precinct size.
   * 
  /*===================================================|==========================================*/
  #define H5Pset_bwc_precinct_d(PX, PY, PZ, PT, CN, CD) \
  do {                                                  \
    if (CN >= 26 && ((CD[0] & (0x01 << 3)) == 0)   &&   \
        PX >= 0 && PX < 512 && PY >= 0 && PY < 512 &&   \
        PZ >= 0 && PZ < 512 && PT >= 0 && PT < 512)     \
      {                                                 \
        CD[0] ^= (0x01 << 3);                           \
        CD[14] = ((0xFF & PX) << 24)  |                 \
                 ((0xFF & PY) << 16)  |                 \
                 ((0xFF & PZ) << 8)   |                 \
                 ((0xFF & PT));                         \
      }                                                 \
  } while(0)

  /*==============================================================================================*/
  /**
   * @details Setter macro for the tile size using a single, global value.
   * 
  /*===================================================|==========================================*/
  #define H5Pset_bwc_tile(TL, CN, CD)                   \
  do {                                                  \
    if (CN >= 26 && ((CD[0] & (0x01 << 4)) == 0))       \
      {                                                 \
        CD[0] ^= (0x01 << 4);                           \
        CD[15] =                                        \
        CD[17] =                                        \
        CD[19] =                                        \
        CD[21] = (unsigned)(TL & 0xFFFFFFFF);           \
        CD[16] =                                        \
        CD[18] =                                        \
        CD[20] =                                        \
        CD[22] = (unsigned int)(((uint64_t)TL >> 32) &  \
                                         0xFFFFFFFF);   \
      }                                                 \
  } while(0)

  /*==============================================================================================*/
  /**
   * @details Setter macro for the tile size.
   * 
  /*===================================================|==========================================*/
  #define H5Pset_bwc_tile_d(TX, TY, TZ, TT, CN, CD)     \
  do {                                                  \
    if (CN >= 26 && ((CD[0] & (0x01 << 4)) == 0))       \
      {                                                 \
        CD[0] ^= (0x01 << 4);                           \
        CD[15] = (unsigned int)(TX & 0xFFFFFFFF);       \
        CD[16] = (unsigned int)(((uint64_t)TX >> 32) &  \
                                         0xFFFFFFFF);   \
        CD[17] = (unsigned int)(TY & 0xFFFFFFFF);       \
        CD[18] = (unsigned int)(((uint64_t)TY >> 32) &  \
                                         0xFFFFFFFF);   \
        CD[19] = (unsigned int)(TZ & 0xFFFFFFFF);       \
        CD[20] = (unsigned int)(((uint64_t)TZ >> 32) &  \
                                         0xFFFFFFFF);   \
        CD[21] = (unsigned int)(TT & 0xFFFFFFFF);       \
        CD[22] = (unsigned int)(((uint64_t)TT >> 32) &  \
                                         0xFFFFFFFF);   \
      }                                                 \
  } while(0)

  /*==============================================================================================*/
  /**
   * @details Setter macro for the decomposition level using a single, global value.
   * 
  /*===================================================|==========================================*/
  #define H5Pset_bwc_decomp(DL, CN, CD)                 \
  do {                                                  \
    if (CN >= 26 && ((CD[0] & (0x01 << 5)) == 0) &&     \
        DL >= 0 && DL < 255)                            \
      {                                                 \
        CD[0] ^= (0x01 << 5);                           \
        CD[23] = ((0xFF & DL) << 24)  |                 \
                 ((0xFF & DL) << 16)  |                 \
                 ((0xFF & DL) << 8)   |                 \
                 ((0xFF & DL));                         \
      }                                                 \
  } while(0)

  /*==============================================================================================*/
  /**
   * @details Setter macro for the decomposition level.
   * 
  /*===================================================|==========================================*/
  #define H5Pset_bwc_decomp_d(DX, DY, DZ, DT, CN, CD)   \
  do {                                                  \
    if (CN >= 13 && ((CD[0] & (0x01 << 5)) == 0)   &&   \
        DX >= 0 && DX < 255 && DY >= 0 && DY < 255 &&   \
        DZ >= 0 && DZ < 255 && DT >= 0 && DT < 255)     \
      {                                                 \
        CD[0] ^= (0x01 << 5);                           \
        CD[23] = ((0xFF & DX) << 24)  |                 \
                 ((0xFF & DY) << 16)  |                 \
                 ((0xFF & DZ) << 8)   |                 \
                 ((0xFF & DT));                         \
      }                                                 \
  } while(0)

  /*==============================================================================================*/
  /**
   * @details Setter macro for fixed point number format Q range.
   * 
  /*===================================================|==========================================*/
  #define H5Pset_bwc_qm(QM, CN, CD)                     \
  do {                                                  \
    if (CN >= 26 && ((CD[0] & (0x01 << 6)) == 0))       \
      {                                                 \
        CD[0] ^= (0x01 << 6);                           \
        CD[24] = (unsigned int)QM;                      \
      }                                                 \
  } while(0)

  /*==============================================================================================*/
  /**
   * @details Setter macro for the quality layer used during decompression.
   * 
  /*===================================================|==========================================*/
  #define H5Pset_bwc_ul(UL, CN, CD)                     \
  do {                                                  \
    if (CN >= 26 && ((CD[0] & (0x01 << 7)) == 0))       \
      {                                                 \
        CD[0] ^= (0x01 << 7);                           \
        CD[25] = (unsigned int)UL;                      \
      }                                                 \
  } while(0)

#endif