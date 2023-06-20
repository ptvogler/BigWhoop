/*==================================================================================================================================*\
||                                                                                                                                  ||
||                         /$$$$$$$  /$$                 /$$      /$$ /$$                                                           ||
||                        | $$__  $$|__/                | $$  /$ | $$| $$                                                           ||
||                        | $$  \ $$ /$$  /$$$$$$       | $$ /$$$| $$| $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$                        ||
||                        | $$$$$$$ | $$ /$$__  $$      | $$/$$ $$ $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$                       ||
||                        | $$__  $$| $$| $$  \ $$      | $$$$_  $$$$| $$  \ $$| $$  \ $$| $$  \ $$| $$  \ $$                       ||
||                        | $$  \ $$| $$| $$  | $$      | $$$/ \  $$$| $$  | $$| $$  | $$| $$  | $$| $$  | $$                       ||
||                        | $$$$$$$/| $$|  $$$$$$$      | $$/   \  $$| $$  | $$|  $$$$$$/|  $$$$$$/| $$$$$$$/                       ||
||                        |_______/ |__/ \____  $$      |__/     \__/|__/  |__/ \______/  \______/ | $$____/                        ||
||                                       /$$  \ $$                                                 | $$                             ||
||                                      |  $$$$$$/                                                 | $$                             ||
||                                       \______/                                                  |__/                             ||
||                                                                                                                                  ||
||      File:  dwt.h                                                                                                                ||
||      -----                                                                                                                       ||
||                                                                                                                                  ||
||      DESCRIPTION:                                                                                                                ||
||      ------------                                                                                                                ||
||      DESCRIPTION NEEDED.                                                                                                         ||
||                                                                                                                                  ||
||      STRUCTS:                                                                                                                    ||
||      --------                                                                                                                    ||
||                                                                                                                                  ||
||      PUBLIC FUNCTIONS:                                                                                                           ||
||      -----------------                                                                                                           ||
||                                                                                                                                  ||
||      DEVELOPMENT HISTORY:                                                                                                        ||
||      --------------------                                                                                                        ||
||                                                                                                                                  ||
||                            Date        Author             Change Id   Release     Description Of Change                          ||
||                            ----        ------             ---------   -------     ---------------------                          ||
||                            21.03.2018  Patrick Vogler     B87D120     V 0.1.0     header file created                            ||
||                                                                                                                                  ||
||       --------------------------------------------------------------------------------------------------------------------       ||
||                                                                                                                                  ||
||       Copyright (c) 2023, High Performance Computing Center - University of Stuttgart                                            ||
||                                                                                                                                  ||
||       Redistribution and use in source and binary forms, with or without modification, are permitted provided that the           ||
||       following conditions are met:                                                                                              ||
||                                                                                                                                  ||
||          (1)   Redistributions of source code must retain the above copyright notice, this list of conditions and                ||
||                the following disclaimer.                                                                                         ||
||                                                                                                                                  ||
||          (2)   Redistributions in binary form must reproduce the above copyright notice, this list of conditions                 ||
||                and the following disclaimer in the documentation and/or other materials provided with the                        ||
||                distribution.                                                                                                     ||
||                                                                                                                                  ||
||       THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,         ||
||       INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          ||
||       DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,          ||
||       SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR            ||
||       SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,          ||
||       WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE           ||
||       USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                   ||
||                                                                                                                                  ||
\*==================================================================================================================================*/
#ifndef DWT_H
#define DWT_H
   /************************************************************************************************************\
   ||                                      _ _  _ ____ _    _  _ ___  ____                                     ||
   ||                                      | |\ | |    |    |  | |  \ |___                                     ||
   ||                                      | | \| |___ |___ |__| |__/ |___                                     ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   #include "types.h"

   /************************************************************************************************************\
   ||                                      _  _ ____ ____ ____ ____ ____                                       ||
   ||                                      |\/| |__| |    |__/ |  | [__                                        ||
   ||                                      |  | |  | |___ |  \ |__| ___]                                       ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !              This macro defines the maximum decomposition level up to which the wavelet filer energy gain  !
   !              is calculated. Beyond the maximum decomposition level the energy gain will only be            !
   !              approximated.                                                                                 !
   !                                                                                                            !
   !   Macros:                                                                                                  !
   !   -------                                                                                                  !
   !                Macro                                Description                                            !
   !                -----                                -----------                                            !
   !                MAX_DECOMPOSITION_LEVELS           - Maximum number of wavelet layers for which the energy  !
   !                                                     gain is calculated.                                    !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                20.03.2018  Patrick Vogler     B87D120     V 0.1.0     Macros created                       !
   \*----------------------------------------------------------------------------------------------------------*/
   #define MAX_DECOMPOSITION_LEVELS 4

   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !              These macros define the irrational coefficients for the high and low pass synthesis filters   !
   !              associated with the (9-7) Cohen-Daubechies-Feauveau-Wavelet as well as the coefficients for   !
   !              its coressponding lifting scheme.                                                             !
   !                                                                                                            !
   !   Macros:                                                                                                  !
   !   -------                                                                                                  !
   !                Macro                                Description                                            !
   !                -----                                -----------                                            !
   !                DWT_9X7_H0                                                                                  !
   !                DWT_9X7_H1                                                                                  !
   !                DWT_9X7_H2                         - Coefficients for the (9-7)                             !
   !                DWT_9X7_H3                           low pass synthesis filter.                             !
   !                                                                                                            !
   !                DWT_9X7_G0                                                                                  !
   !                DWT_9X7_G1                                                                                  !
   !                DWT_9X7_G2                                                                                  !
   !                DWT_9X7_G3                         - Coefficients for the (9-7)                             !
   !                DWT_9X7_G4                           high pass synthesis filter.                            !
   !                                                                                                            !
   !                ALPHA                                                                                       !
   !                BETA                                                                                        !
   !                GAMMA                                                                                       !
   !                DELTA                                                                                       !
   !                KAPPA_H                            - Coefficients for the (9-7)                             !
   !                KAPPA_L                              lifting scheme.                                        !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                20.03.2018  Patrick Vogler     B87D120     V 0.1.0     Macros created                       !
   \*----------------------------------------------------------------------------------------------------------*/
   #define DWT_9X7_H0  1.115087052457000f
   #define DWT_9X7_H1  0.591271763114250f
   #define DWT_9X7_H2 -0.057543526228500f
   #define DWT_9X7_H3 -0.091271763114250f

   #define DWT_9X7_G0  1.205898036472721f
   #define DWT_9X7_G1 -0.533728236885750f
   #define DWT_9X7_G2 -0.156446533057980f
   #define DWT_9X7_G3  0.033728236885750f
   #define DWT_9X7_G4  0.053497514821620f

   #define ALPHA      -1.586134342059924f
   #define BETA       -0.052980118572961f
   #define GAMMA       0.882911075530934f
   #define DELTA       0.360523644801462f
   #define KAPPA_H     1.230174104914001f
   #define KAPPA_L     0.812893066115961f

   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !              These macros define the irrational coefficients for the high and low pass synthesis filters   !
   !              associated with the (5-3) LeGall-Wavelet.                                                     !
   !                                                                                                            !
   !   Macros:                                                                                                  !
   !   -------                                                                                                  !
   !                Macro                                Description                                            !
   !                -----                                -----------                                            !
   !                DWT_5X3_H0                         - Coefficients for the (9-7)                             !
   !                DWT_5X3_H1                           low pass synthesis filter.                             !
   !                                                                                                            !
   !                DWT_5X3_G0                                                                                  !
   !                DWT_5X3_G1                         - Coefficients for the (9-7)                             !
   !                DWT_5X3_G2                           high pass synthesis filter.                            !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                20.03.2018  Patrick Vogler     B87D120     V 0.1.0     Macros created                       !
   \*----------------------------------------------------------------------------------------------------------*/
   #define DWT_5X3_H0  1.0f
   #define DWT_5X3_H1  0.5f

   #define DWT_5X3_G0  0.75f
   #define DWT_5X3_G1 -0.25f
   #define DWT_5X3_G2 -0.125f

   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !              These macros define the irrational coefficients for the high and low pass synthesis filters   !
   !              associated with the Haar-Wavelet.                                                             !
   !                                                                                                            !
   !   Macros:                                                                                                  !
   !   -------                                                                                                  !
   !                Macro                                Description                                            !
   !                -----                                -----------                                            !
   !                DWT_HAAR_G0                        - Coefficients for the Haar                              !
   !                DWT_HAAR_G1                          low pass synthesis filter.                             !
   !                                                                                                            !
   !                DWT_HAAR_H1                        - Coefficients for the Haar                              !
   !                DWT_HAAR_H2                          high pass synthesis filter.                            !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                20.03.2018  Patrick Vogler     B87D120     V 0.1.0     Macros created                       !
   \*----------------------------------------------------------------------------------------------------------*/
   #define DWT_HAAR_H0  1
   #define DWT_HAAR_H1  1

   #define DWT_HAAR_G0  0.5
   #define DWT_HAAR_G1 -0.5

   /************************************************************************************************************\
   ||            ____ _  _ ___ ____ ____ _  _ ____ _       _  _ ____ ____ _ ____ ___  _    ____ ____           ||
   ||            |___  \/   |  |___ |__/ |\ | |__| |       |  | |__| |__/ | |__| |__] |    |___ [__            ||
   ||            |___ _/\_  |  |___ |  \ | \| |  | |___     \/  |  | |  \ | |  | |__] |___ |___ ___]           ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !              This array defines a lookup table used to store the energy gain factors for the one dimen-    !
   !              sional, dyadic tree structured CDF-(9/7), LeGall-(5/3) and Haar wavelet transform. Each LUT   !
   !              contains energy gain factors for the 6 low-pass and 6 high-pass decomposition including level !
   !              zero.                                                                                         !
   !                                                                                                            !
   !   VARIABLES:                                                                                               !
   !   -----------                                                                                              !
   !                Variable                             Description                                            !
   !                --------                             -----------                                            !
   !                DWT_ENERGY_GAIN_LUT                - Lookup-table for the CDF-(9/7), LG-(5/3) and Haar      !
   !                                                     dwt energy gain factor.                                !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                21.03.2018  Patrick Vogler     B87D120     V 0.1.0     Variables created                    !
   \*----------------------------------------------------------------------------------------------------------*/
   extern double DWT_ENERGY_GAIN_LUT[3][2 * MAX_DECOMPOSITION_LEVELS + 2];

   /************************************************************************************************************\
   ||                  ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                  ||
   ||                  |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__                   ||
   ||                  |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]                  ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: uchar initialize_gain_lut()                                                               !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function calculates the energy gain factor Gb for the one dimensional, dyadic tree     !
   !                structured CDF-(9/7), LeGall-(5/3) and Haar wavelet transform with 5 levels. The energy     !
   !                gain factors are calculated according to equation (4.39) from JPEG2000 by David S. Taubman  !
   !                and Michael W. Marcellin (p. 193):                                                          !
   !                                                                                                            !
   !                                            s_L1[n] = g0[n],   s_H1[n] = h0[n],                             !
   !                         s_Ld[n] = ∑(s_Ld-1[k]*g0[n-2k], k),   s_Hd[n] = ∑(s_Hd-1[k]*g0[n-2k], k).          !
   !                                                                                                            !
   !                The energy gain factors are stored in their corresponding lookup tables and used to calcu-  !
   !                late the energy gain for the multi dimensional wavelet transforms according to equation     !
   !                (4.40) from JPEG2000 by David S. Taubman and Michael W. Marcellin (p.193):                  !
   !                                                                                                            !
   !                               s_LLD[n1,n2] = s_LD[n1] * s_LD[n2]   =>   G_LLD = G_LD * G_LD                !
   !                               s_HLD[n1,n2] = s_LD[n1] * s_HD[n2]   =>   G_HLD = G_LD * G_HD                !
   !                               s_LHD[n1,n2] = s_HD[n1] * s_LD[n2]   =>   G_LHD = G_HD * G_LD                !
   !                               s_HHD[n1,n2] = s_HD[n1] * s_HD[n2]   =>   G_HHD = G_HD * G_HD                !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   uchar
   initialize_gain_lut();

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: bwc_float get_dwt_energy_gain(bwc_field *field, uchar highband_flag, uint level)          !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function evaluates the energy gain factor according to the the specified decomposition !
   !                level. For decomposition levels larger than MAX_DECOMPOSITION_LEVELS the filter gain for    !
   !                the extra levels is approximated by multiplying the energy gain factor by 2.                !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   bwc_float
   get_dwt_energy_gain(bwc_field *field, uchar highband_flag, uint16 level);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: uchar forward_discrete_wavelet_transform(bwc_field *const field,                          !
   !   --------------                                          bwc_parameter *const parameter)                  !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function performs the forward discrete wavelet transform on the current tile param-    !
   !                eter. After loading the flow field samples for a row, column or spatial/temporal slice      !
   !                into a working buffer, a boundary extension operation is performed to ensure an approp.     !
   !                sample base. The working buffer is then transform using the wavelet kernel selected for     !
   !                the spatial or temporal dimension, sorted into high- and low-frequency samples and flushed  !
   !                back into the tile parameter memory block. This operation is performed for every row,       !
   !                column and spatial/temporal slice for ndecomp number of decomposition levels.               !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   uchar
   forward_discrete_wavelet_transform(bwc_field *const field, bwc_parameter *const parameter);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: uchar inverse_discrete_wavelet_transform(bwc_field *const field,                          !
   !   --------------                                          bwc_parameter *const parameter)                  !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function performs the inverse discrete wavelet transform on the current tile param-    !
   !                eter. After loading the interweaved wavelet coefficients for a row, column or spatial/      !
   !                temporal slice into a working buffer, a boundary extension operation is performed to        !
   !                ensure an appropriate sample base. The working buffer is then transform using the wave-     !
   !                let kernel selected for the spatial or temporal dimension and flushed back into the tile    !
   !                parameter memory block. This operation is performed for every row, column and spatial/      !
   !                temporal slice for ndecomp number of decomposition levels.                                  !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   uchar
   inverse_discrete_wavelet_transform(bwc_field *const field, bwc_parameter *const parameter);
#endif