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
  !         Library version information                                                            !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  #define BWC_VERSION_MAJOR     0
  #define BWC_VERSION_MINOR     1
  #define BWC_VERSION_PATCH     0

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         These constants are used to identify the spatial and temporal dimensions.              !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  #define DIM_X                 1                         // 1st. spacial direction
  #define DIM_Y                 2                         // 2nd. spacial direction
  #define DIM_Z                 4                         // 3rd. spacial direction
  #define DIM_TS                8                         // temporal direction
  #define DIM_ALL               15                        // all dimensions

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         These Constants define codestream markers used to create the embedded codestream.      !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  #define SOC                   0xFF50                    // Start of codestream
  #define SGI                   0xFF51                    // Global data-set information
  #define SGC                   0xFF52                    // Global control parameters
  #define SAX                   0xFF53                    // Auxiliary data-set information
  #define TLM                   0xFF54                    // Packet lengths: main header
  #define PLM                   0xFF55                    // Packet lengths: tile-part
  #define PPM                   0xFF56                    // Quantization default
  #define COM                   0xFF57                    // Comment
  #define EOH                   0xFF58                    // End of header
  #define PLT                   0xFF60                    // Packed packet headers: main header
  #define PPT                   0xFF61                    // Packed packet headers: tile-part
  #define SOT                   0xFF90                    // Start of tile
  #define SOP                   0xFF91                    // Start of packet
  #define EPH                   0xFF92                    // End of packet header
  #define SOD                   0xFF93                    // Start of data
  #define EOC                   0xFFFF                    // End of code-stream


  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         These macros define flags used for codestream parsing.                                 !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
   #define CODESTREAM_OK        0x00                      // No errors detected in Codestream
   #define CODESTREAM_ERROR     0x80                      // Error detexted in Codestream
   #define CODESTREAM_SGI_READ  0x01                      // Global data-set information read
   #define CODESTREAM_SGC_READ  0x02                      // Global control parameters read
   #define CODESTREAM_SAX_READ  0x08                      // Auxiliary information block read
   #define CODESTREAM_COM_READ  0x10                      // Comment block read

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

  #define CSERROR  "o##########################################################o\n"\
                   "|                ERROR: Invalid Codestream                 |\n"\
                   "o##########################################################o\n"
#endif