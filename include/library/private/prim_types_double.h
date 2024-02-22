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
||        This header defines a set of basic arithmetic types with specified widths to be         ||
||        used in the big whoop compression algorithm.                                            ||
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
#ifndef BWC_PRIM_TYPES_DOUBLE_H
#define BWC_PRIM_TYPES_DOUBLE_H
  /************************************************************************************************\
  ||                               _ _  _ ____ _    _  _ ___  ____                                ||
  ||                               | |\ | |    |    |  | |  \ |___                                ||
  ||                               | | \| |___ |___ |__| |__/ |___                                ||
  ||                                                                                              ||
  \************************************************************************************************/
  #include <stdint.h>

  /************************************************************************************************\
  ||                ___  ____ _ _  _ _ ___ _ _  _ ____    ___ _   _ ___  ____ ____                ||
  ||                |__] |__/ | |\/| |  |  | |  | |___     |   \_/  |__] |___ [__                 ||
  ||                |    |  \ | |  | |  |  |  \/  |___     |    |   |    |___ ___]                ||
  ||                                                                                              ||
  \************************************************************************************************/
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

  typedef double           bwc_float;
  typedef uint64           bwc_raw;

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
  !         These macros describe the minimum and maximum values for a double precision IEEE       !
  !         754 floating point variable.                                                           !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  #define FLT_MAX               1.7976931348623157e+308   // Maximum finite value of a double
  #define FLT_MIN               2.2250738585072014e-308   // Minimum finite value of a double

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         These macros describe the precision (in bits and bytes) of the derrived floating       !
  !         point type as well as the number of bits used to represent its mantissa and            !
  !         exponent fields.                                                                       !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  #define PREC_BIT              63                        // Double type precision in bits
  #define PREC_MANTISSA         52                        // Mantissa field precision in bits
  #define PREC_EXPONENT         11                        // Exponent field precision in bits
  #define PREC_BYTE             8                         // Double type precision in bytes

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         These macros describe the bit masks used to access the sign, mantissa and              !
  !         exponent of the derrived floating point type.                                          !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  #define SIGN                  0x8000000000000000        // Sign bit mask
  #define MANTISSA              0x000FFFFFFFFFFFFF        // Mantissa bit mask
  #define EXPONENT              0x7FF0000000000000        // Exponent bit mask

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This macro describe the maximum number of possible encoding passes during the          !
  !         entropy encoding stage.                                                                !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  #define MAXIMUM_NO_PASSES    (64 * 3) - 2
#endif