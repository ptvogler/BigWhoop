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
||        This file defines simple macros that are used to make the code more readable.           ||
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
#ifndef BWC_MACROSS_H
#define BWC_MACROSS_H
  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         These macros define simple mathematicall oprators.                                     !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  #define MAX(x, y)             (((x) < (y))?(y):(x))     // Returns maximum between two values
  #define MIN(x, y)             (((x) > (y))?(y):(x))     // Returns minimum between two values

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This macro is used to evaluate the size of an array.                                   !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  #define GET_DIM(x) (sizeof(x)/sizeof(*(x)))

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         These Constants define common error messages used throughout the bwc library.          !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  #define MEMERROR "o##########################################################o\n"\
                   "|                   ERROR: Out of memory                   |\n"\
                   "o##########################################################o\n"

  #define CSERROR "o##########################################################o\n"\
                  "|                ERROR: Invalid Codestream                 |\n"\
                  "o##########################################################o\n"
#endif