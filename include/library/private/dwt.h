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
||        This file describes a set of functions that can be used to performe the forward/        ||
||        inverse discrete wavelet transform on 1- to 4-dimensional IEEE 754 data-sets.           ||
||        For more information please refere to JPEG2000 by D. S. Taubman and M. W.               ||
||        Marcellin.                                                                              ||
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
#ifndef DWT_H
#define DWT_H
  /************************************************************************************************\
  ||                               _ _  _ ____ _    _  _ ___  ____                                ||
  ||                               | |\ | |    |    |  | |  \ |___                                ||
  ||                               | | \| |___ |___ |__| |__/ |___                                ||
  ||                                                                                              ||
  \************************************************************************************************/
  #include "types.h"

  /************************************************************************************************\
  ||                                _  _ ____ ____ ____ ____ ____                                 ||
  ||                                |\/| |__| |    |__/ |  | [__                                  ||
  ||                                |  | |  | |___ |  \ |__| ___]                                 ||
  ||                                                                                              ||
  \************************************************************************************************/
  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         Maximum number of wavelet layers for which the energy gain is calculated.              !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  #define MAX_DECOMP_LEVELS     4

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         These macros define the irrational coefficients for the high and low pass              !
  !         synthesis filters associated with the (9-7) Cohen-Daubechies-Feauveau-Wavelet          !
  !         as well as the coefficients for its coressponding lifting scheme.                      !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  #define DWT_9X7_H0            1.115087052457000f        // Low pass synthesis filter ...
  #define DWT_9X7_H1            0.591271763114250f        // ... coefficients
  #define DWT_9X7_H2           -0.057543526228500f
  #define DWT_9X7_H3           -0.091271763114250f

  #define DWT_9X7_G0            1.205898036472721f        // High pass synthesis filter ...
  #define DWT_9X7_G1           -0.533728236885750f        // ... coefficients
  #define DWT_9X7_G2           -0.156446533057980f
  #define DWT_9X7_G3            0.033728236885750f
  #define DWT_9X7_G4            0.053497514821620f

  #define ALPHA                -1.586134342059924f        // Lifting coefficients.
  #define BETA                 -0.052980118572961f
  #define GAMMA                 0.882911075530934f
  #define DELTA                 0.360523644801462f
  #define KAPPA_H               1.230174104914001f
  #define KAPPA_L               0.812893066115961f

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         These macros define the irrational coefficients for the high and low pass              !
  !         synthesis filters associated with the (5-3) LeGall-Wavelet.                            !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  #define DWT_5X3_H0            1.0f                      // Low pass synthesis filter ...
  #define DWT_5X3_H1            0.5f                      // ... coefficients

  #define DWT_5X3_G0            0.75f                     // High pass synthesis filter
  #define DWT_5X3_G1           -0.25f                     // ... coefficients
  #define DWT_5X3_G2           -0.125f

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         These macros define the irrational coefficients for the high and low pass              !
  !         synthesis filters associated with the Haar-Wavelet.                                    !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  #define DWT_HAAR_H0           1                         // Low pass synthesis filter ...
  #define DWT_HAAR_H1           1                         // ... coefficients

  #define DWT_HAAR_G0           0.5                       // High pass synthesis filter ...
  #define DWT_HAAR_G1          -0.5                       // ... coefficients

  /************************************************************************************************\
  ||     ____ _  _ ___ ____ ____ _  _ ____ _       _  _ ____ ____ _ ____ ___  _    ____ ____      ||
  ||     |___  \/   |  |___ |__/ |\ | |__| |       |  | |__| |__/ | |__| |__] |    |___ [__       ||
  ||     |___ _/\_  |  |___ |  \ | \| |  | |___     \/  |  | |  \ | |  | |__] |___ |___ ___]      ||
  ||                                                                                              ||
  \************************************************************************************************/
  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This array defines a lookup table used to store the energy gain factors for the        !
  !         one dimensional CDF(9/7), LeGall-(5/3) and Haar wavelet transform. Each LUT            !
  !         contains energy gain factors for the 6 low-pass and 6 high-pass decomposition          !
  !         including level zero.                                                                  !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  extern double DWT_ENERGY_GAIN_LUT[3][2 * MAX_DECOMP_LEVELS + 2];


  /************************************************************************************************\
  ||            ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____            ||
  ||            |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__             ||
  ||            |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]            ||
  ||                                                                                              ||
  \************************************************************************************************/
  uchar        initialize_gain_lut        ();
  //==========|==========================|======================|======|=======|====================
  bwc_float    get_dwt_energy_gain        (bwc_field                    *const  field, 
                                           uchar                 const          highband_flag, 
                                           uint16                const          level);
  //==========|==========================|======================|======|=======|====================
  uchar        forward_wavelet_transform  (bwc_field                    *const  field, 
                                           bwc_parameter                *const  parameter);
  //==========|==========================|======================|======|=======|====================
  uchar        inverse_wavelet_transform  (bwc_field                    *const  field, 
                                           bwc_parameter                *const  parameter);
#endif