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
||      File:  prim_types_single.h                                                                                                  ||
||      -----                                                                                                                       ||
||                                                                                                                                  ||
||      DESCRIPTION:                                                                                                                ||
||      ------------                                                                                                                ||
||                      This header defines a set of basic arithmetic types with specified widths to be used in                     ||
||                      the big whoop compression algorithm. The width of an arithmetic type is defined as the                      ||
||                      number of bits used to store its value.                                                                     ||
||                                                                                                                                  ||
||      DEVELOPMENT HISTORY:                                                                                                        ||
||      --------------------                                                                                                        ||
||                                                                                                                                  ||
||                            Date        Author             Change Id   Release     Description Of Change                          ||
||                            ----        ------             ---------   -------     ---------------------                          ||
||                            07.12.2017  Patrick Vogler     B87D120     V 0.1.0     header file created                            ||
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
#ifndef BWC_PRIM_TYPES_SINGLE_H
#define BWC_PRIM_TYPES_SINGLE_H

   /************************************************************************************************************\
   ||                                      _ _  _ ____ _    _  _ ___  ____                                     ||
   ||                                      | |\ | |    |    |  | |  \ |___                                     ||
   ||                                      | | \| |___ |___ |__| |__/ |___                                     ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   #include <stdint.h>

   /************************************************************************************************************\
   ||                      ___  ____ _ _  _ _ ___ _ _  _ ____    ___ _   _ ___  ____ ____                      ||
   ||                      |__] |__/ | |\/| |  |  | |  | |___     |   \_/  |__] |___ [__                       ||
   ||                      |    |  \ | |  | |  |  |  \/  |___     |    |   |    |___ ___]                      ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   typedef unsigned char    uchar;
   typedef unsigned short   ushort;
   typedef unsigned int     uint;

   typedef int8_t           int8;
   typedef uint8_t          uint8;
   typedef int16_t          int16;
   typedef uint16_t         uint16;
   typedef int32_t          int32;
   typedef uint32_t         uint32;
   typedef int64_t          int64;
   typedef uint64_t         uint64;

   typedef float            bwc_float;
   typedef uint32           bwc_raw;

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
   !                These constants describe the minimum and maximum values for a double precision IEEE 754     !
   !                floating point variable.                                                                    !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                07.12.2018  Patrick Vogler     B87D120     V 0.1.0     Constants created                    !
   \*----------------------------------------------------------------------------------------------------------*/
   #define FLT_MAX   1.70141183e+38
   #define FLT_MIN   1.17549435e-38

   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !                These constants describe the precision (in bits and bytes) of the primitive floating point  !
   !                type used during (de)compression as well as the number of bits used to represent the        !
   !                mantissa and exponent fields.                                                               !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                07.12.2018  Patrick Vogler     B87D120     V 0.1.0     Constants created                    !
   \*----------------------------------------------------------------------------------------------------------*/
   #define PREC_BIT      31
   #define PREC_MANTISSA 23
   #define PREC_EXPONENT  8
   #define PREC_BYTE      4

   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !                These constants describe the bit masks used to access the sign, mantissa and exponent of    !
   !                the primitive floating point type used during (de)compression.                              !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                07.12.2018  Patrick Vogler     B87D120     V 0.1.0     Constants created                    !
   \*----------------------------------------------------------------------------------------------------------*/
   #define SIGN     0x80000000
   #define MANTISSA 0x007FFFFF
   #define EXPONENT 0x7F800000

   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !              This constants describe the maximum number of encoding passes during the entropy encoding     !
   !              stage.                                                                                        !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                03.07.2018  Patrick Vogler     B87D120     V 0.1.0     Constants created                    !
   \*----------------------------------------------------------------------------------------------------------*/
   #define MAXIMUM_NO_PASSES  (32 * 3) - 2
#endif