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
 *        @file   H5Zbwc_sup.c
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
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||                                _ _  _ ____ _    _  _ ___  ____                                 ||
||                                | |\ | |    |    |  | |  \ |___                                 ||
||                                | | \| |___ |___ |__| |__/ |___                                 ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#include <stdint.h>

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||             ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____             ||
||             |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__              ||
||             |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]             ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*================================================================================================*/
/**
 * @details Setter function for code-stream error resilience.
 *
 * @param[in]     CN    Number of cd_values (cd_nelements).
 * @param[in,out] CD    Values used to instruct compression (cd_values).
 */
/*================================================================================================*/
void
H5Pset_bwc_error_resilience(size_t              CN, 
                            unsigned int const  CD[/*CN*/])
{
  if (CN >= 16 && (CD[0] & (0x01 << 0)) == 0)
    {
      CD[0] ^= (0x01 << 0);

      CD[1]  = 1;
    }
}

/*================================================================================================*/
/**
 * @details Setter function for code-stream bit-rate.
 *
 * @param[in]     BR    Code-stream bit-rate.
 * @param[in]     CN    Number of cd_values (cd_nelements).
 * @param[in,out] CD    Values used to instruct compression (cd_values).
 */
/*================================================================================================*/
void
H5Pset_bwc_bitrate(float               BR,
                   size_t              CN, 
                   unsigned int const  CD[/*CN*/])
{
  if (CN >= 16 && (CD[0] & (0x01 << 1)) == 0)
    {
      CD[0] ^= (0x01 << 1);

      CD[2] = *(uint32_t *)&BR;
    }
}

/*================================================================================================*/
/**
 * @details Setter function for the code-block size using a single, global value.
 *
 * @param[in]     CB    Global code-block size.
 * @param[in]     CN    Number of cd_values (cd_nelements).
 * @param[in,out] CD    Values used to instruct compression (cd_values).
 */
/*================================================================================================*/
void
H5Pset_bwc_codeblocks(uint32_t           CB,
                      size_t             CN, 
                      unsigned int const CD[/*CN*/])
{
  if (CN >= 16 && ((CD[0] & (0x01 << 2)) == 0) && CB >= 0 && CB < 512)
    {
      CD[0] ^= (0x01 << 2);

      CD[4]  = ((0xFF & CB) << 24)  |
               ((0xFF & CB) << 16)  |
               ((0xFF & CB) << 8)   |
               ((0xFF & CB));
    }
}

/*================================================================================================*/
/**
 * @details Setter function for the code-block size.
 *
 * @param[in]     CX    Code-block size in the first dimension.
 * @param[in]     CY    Code-block size in the second dimension.
 * @param[in]     CZ    Code-block size in the third dimension.
 * @param[in]     CT    Code-block size in the fourth dimension.
 * @param[in]     CN    Number of cd_values (cd_nelements).
 * @param[in,out] CD    Values used to instruct compression (cd_values).
 */
/*================================================================================================*/
void
H5Pset_bwc_codeblocks_d(uint32_t           CX, 
                        uint32_t           CY,
                        uint32_t           CZ,
                        uint32_t           CT,
                        size_t             CN,
                        unsigned int const CD[/*CN*/])
{
  if (CN >= 16 && ((CD[0] & (0x01 << 2)) == 0) && CX >= 0 && CX < 512
                                               && CY >= 0 && CY < 512
                                               && CZ >= 0 && CZ < 512
                                               && CT >= 0 && CT < 512)
    {
      CD[0] ^= (0x01 << 2);

      CD[4]  = ((0xFF & CX) << 24)  |
               ((0xFF & CY) << 16)  |
               ((0xFF & CZ) << 8)   |
               ((0xFF & CT));
    }
}

/*================================================================================================*/
/**
 * @details Setter function for the precinct size using a single, global value.
 *
 * @param[in]     PC    Global precinct size.
 * @param[in]     CN    Number of cd_values (cd_nelements).
 * @param[in,out] CD    Values used to instruct compression (cd_values).
 */
/*================================================================================================*/
void
H5Pset_bwc_precinct(uint32_t           PC,
                    size_t             CN,
                    unsigned int const CD[/*CN*/])
{
  if (CN >= 16 && ((CD[0] & (0x01 << 3)) == 0) && CB >= 0 && CB < 512)
    {
      CD[0] ^= (0x01 << 3);

      CD[5]  = ((0xFF & CB) << 24)  |
               ((0xFF & CB) << 16)  |
               ((0xFF & CB) << 8)   |
               ((0xFF & CB));
    }
}

/*================================================================================================*/
/**
 * @details Setter function for the precinct size.
 *
 * @param[in]     PX    Precinct size in the first dimension.
 * @param[in]     PY    Precinct size in the second dimension.
 * @param[in]     PZ    Precinct size in the third dimension.
 * @param[in]     PT    Precinct size in the fourth dimension.
 * @param[in]     CN    Number of cd_values (cd_nelements).
 * @param[in,out] CD    Values used to instruct compression (cd_values).
 */
/*================================================================================================*/
void
H5Pset_bwc_precinct_d(uint32_t           PX, 
                      uint32_t           PY,
                      uint32_t           PZ,
                      uint32_t           PT,
                      size_t             CN,
                      unsigned int const CD[/*CN*/])
{
  if (CN >= 16 && ((CD[0] & (0x01 << 3)) == 0) && PX >= 0 && PX < 512
                                               && PY >= 0 && PY < 512
                                               && PZ >= 0 && PZ < 512
                                               && PT >= 0 && PT < 512)
    {
      CD[0] ^= (0x01 << 3);

      CD[5]  = ((0xFF & PX) << 24)  |
               ((0xFF & PY) << 16)  |
               ((0xFF & PZ) << 8)   |
               ((0xFF & PT));
    }
}

/*================================================================================================*/
/**
 * @details Setter function for the tile size using a single, global value.
 *
 * @param[in]     TL    Global tile size.
 * @param[in]     CN    Number of cd_values (cd_nelements).
 * @param[in,out] CD    Values used to instruct compression (cd_values).
 */
/*================================================================================================*/
void
H5Pset_bwc_tile(uint64_t           TL,
                size_t             CN,
                unsigned int const CD[/*CN*/])
{
  if (CN >= 16 && ((CD[0] & (0x01 << 4)) == 0))
    {
      CD[0] ^= (0x01 << 4);

      CD[5]  =
      cd[7]  =
      CD[9]  =
      CD[11] = 0xFFFFFFFF & *(int*)&TL;

      CD[6]  =
      cd[8]  =
      CD[10] =
      CD[12] = (int)(0xFFFFFFFF & *(long*)&TL >> 32);
    }
}

/*================================================================================================*/
/**
 * @details Setter function for the tile size.
 *
 * @param[in]     TX    Tile size in the first dimension.
 * @param[in]     TY    Tile size in the second dimension.
 * @param[in]     TZ    Tile size in the third dimension.
 * @param[in]     TT    Tile size in the fourth dimension.
 * @param[in]     CN    Number of cd_values (cd_nelements).
 * @param[in,out] CD    Values used to instruct compression (cd_values).
 */
/*================================================================================================*/
void
H5Pset_bwc_tile_d(uint64_t           TX, 
                  uint64_t           TY,
                  uint64_t           TZ,
                  uint64_t           TT,
                  size_t             CN,
                  unsigned int const CD[/*CN*/])
{
  if (CN >= 16 && ((CD[0] & (0x01 << 4)) == 0))
    {
      CD[0] ^= (0x01 << 4);

      CD[5]  = 0xFFFFFFFF & *(int*)&TX;
      CD[6]  = (int)(0xFFFFFFFF & *(long*)&TX >> 32);

      cd[7]  = 0xFFFFFFFF & *(int*)&TY;
      cd[8]  = (int)(0xFFFFFFFF & *(long*)&TY >> 32);

      CD[9]  = 0xFFFFFFFF & *(int*)&TZ;
      CD[10] = (int)(0xFFFFFFFF & *(long*)&TZ >> 32);

      CD[11] = 0xFFFFFFFF & *(int*)&TT;
      CD[12] = (int)(0xFFFFFFFF & *(long*)&TT >> 32);
    }
}

/*================================================================================================*/
/**
 * @details Setter function for the decomposition level using a single, global value.
 *
 * @param[in]     DL    Global decomposition level.
 * @param[in]     CN    Number of cd_values (cd_nelements).
 * @param[in,out] CD    Values used to instruct compression (cd_values).
 */
/*================================================================================================*/
void
H5Pset_bwc_decomp(uint32_t           DL,
                  size_t             CN,
                  unsigned int const CD[/*CN*/])
{
  if (CN >= 16 && ((CD[0] & (0x01 << 5)) == 0) && DL >= 0 && DL < 255)
    {
      CD[0] ^= (0x01 << 5);

      CD[13] = ((0xFF & DL) << 24)  |
               ((0xFF & DL) << 16)  |
               ((0xFF & DL) << 8)   |
               ((0xFF & DL));
    }
}

/*================================================================================================*/
/**
 * @details Setter function for the decomposition level.
 *
 * @param[in]     DX    Decomposition level in the first dimension.
 * @param[in]     DY    Decomposition level in the second dimension.
 * @param[in]     DZ    Decomposition level in the third dimension.
 * @param[in]     DT    Decomposition level in the fourth dimension.
 * @param[in]     CN    Number of cd_values (cd_nelements).
 * @param[in,out] CD    Values used to instruct compression (cd_values).
 */
/*================================================================================================*/
void
H5Pset_bwc_decomp_d(uint32_t           DX, 
                    uint32_t           DY,
                    uint32_t           DZ,
                    uint32_t           DT,
                    size_t             CN,
                    unsigned int const CD[/*CN*/])
{
  if (CN >= 13 && ((CD[0] & (0x01 << 5)) == 0) && DX >= 0 && DX < 255
                                               && DY >= 0 && DY < 255
                                               && DZ >= 0 && DZ < 255
                                               && DT >= 0 && DT < 255)
    {
      CD[0] ^= (0x01 << 5);

      CD[13] = ((0xFF & DX) << 24)  |
               ((0xFF & DY) << 16)  |
               ((0xFF & DZ) << 8)   |
               ((0xFF & DT));
    }
}

/*================================================================================================*/
/**
 * @details Setter function for fixed point number format Q range.
 *
 * @param[in]     QM    Q number formate range (m).
 * @param[in]     CN    Number of cd_values (cd_nelements).
 * @param[in,out] CD    Values used to instruct compression (cd_values).
 */
/*================================================================================================*/
void
H5Pset_bwc_qm(uint8_t            QM,
              size_t             CN,
              unsigned int const CD[/*CN*/])
{
  if (CN >= 16 && ((CD[0] & (0x01 << 6)) == 0))
    {
      CD[0] ^= (0x01 << 6);
      CD[14] = (uint32_t)QM;
    }
}

/*================================================================================================*/
/**
 * @details Setter function for the quality layer used during decompression
 *
 * @param[in]     UL    Quality layer used during decomposition.
 * @param[in]     CN    Number of cd_values (cd_nelements).
 * @param[in,out] CD    Values used to instruct compression (cd_values).
 */
/*================================================================================================*/
void
H5Pset_bwc_ul(uint8_t            UL,
              size_t             CN,
              unsigned int const CD[/*CN*/])
{
  if (CN >= 16 && ((CD[0] & (0x01 << 7)) == 0))
    {
      CD[0] ^= (0x01 << 7);
      CD[14] = (uint32_t)UL;
    }
}