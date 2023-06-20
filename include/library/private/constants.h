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
||      File:  constants.h                                                                                                          ||
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
||                            16.10.2017  Patrick Vogler     B87D120     V 0.1.0     header file created                            ||
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
#ifndef CONSTANTS_H
#define CONSTANTS_H
   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !              These constants are used to identify the spatial and temporal dimensions.                     !
   !                                                                                                            !
   !   Constants:                                                                                               !
   !   -----------                                                                                              !
   !                Constant                             Description                                            !
   !                --------                             -----------                                            !
   !                DIM_X                                1st Spatial Dimension                                  !
   !                DIM_Y                                2nd Spatial Dimension                                  !
   !                DIM_Z                                3rd Spatial Dimension                                  !
   !                DIM_TS                               Temporal Dimension                                     !
   !                DIM_ALL                              All Dimensions                                         !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                04.12.2017  Patrick Vogler     B87D120     V 0.1.0     Constants created                    !
   \*----------------------------------------------------------------------------------------------------------*/
   #define DIM_X                    1
   #define DIM_Y                    2
   #define DIM_Z                    4
   #define DIM_TS                   8
   #define DIM_ALL                  15

   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !              These constants describe the maximum allowable wavelet decompositions for the spatial and     !
   !              temporal dimensions and the maximum allowable subbands.                                       !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                01.12.2017  Patrick Vogler     B87D120     V 0.1.0     Constants created                    !
   \*----------------------------------------------------------------------------------------------------------*/
   #define MAXIMUM_NUMBER_OF_SPATIAL_DECOMP 64
   #define MAXIMUM_NUMBER_OF_TEMPORAL_DECOMP 32
   #define MAXIMUM_NUMBER_OF_SUBBANDS  (1+(15*(64-32))) + (1+(7*32))

   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !                These Constants describe the codestream markers used to create the embedded codestream.     !
   !                                                                                                            !
   !   CONSTANTS:                                                                                               !
   !   -----------                                                                                              !
   !                Constant                             Description                                            !
   !                --------                             -----------                                            !
   !                SOC                                - Start of code-stream                                   !
   !                SGI                                - Global data-set information                            !
   !                SGC                                - Global control parameters                              !
   !                SAX                                - Auxiliary data-set information                         !
   !                TLM                                - Packet lengths: main header                            !
   !                PLM                                - Packet lengths: tile-part                              !
   !                PPM                                - Quantization default                                   !
   !                COM                                - Comment                                                !
   !                EOH                                - End of header                                          !
   !                PLT                                - Packed packet headers: main header                     !
   !                PPT                                - Packed packet headers: tile-part                       !
   !                SOT                                - Start of tile                                          !
   !                SOP                                - Start of packet                                        !
   !                EPH                                - End of packet header                                   !
   !                SOD                                - Start of data                                          !
   !                EOC                                - End of code-stream                                     !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                01.12.2017  Patrick Vogler     B87D120     V 0.1.0     Constants created                    !
   \*----------------------------------------------------------------------------------------------------------*/
   #define SOC 0xFF50
   #define SGI 0xFF51
   #define SGC 0xFF52
   #define SAX 0xFF53
   #define TLM 0xFF54
   #define PLM 0xFF55
   #define PPM 0xFF56
   #define COM 0xFF57
   #define EOH 0xFF58
   #define PLT 0xFF60
   #define PPT 0xFF61
   #define SOT 0xFF90
   #define SOP 0xFF91
   #define EPH 0xFF92
   #define SOD 0xFF93
   #define EOC 0xFFFF

   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !                These Constants are used for codestream parsing.                                            !
   !                                                                                                            !
   !   CONSTANTS:                                                                                               !
   !   -----------                                                                                              !
   !                Constant                             Description                                            !
   !                --------                             -----------                                            !
   !                CODESTREAM_OK                        No errors detectet in Codestream                       !
   !                CODESTREAM_ERROR                     Error detectet in Codestream                           !
   !                CODESTREAM_SGI_READ                  Global data-set information read                       !
   !                CODESTREAM_SGC_READ                  Global control parameters read                         !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                01.08.2019  Patrick Vogler     B87D120     V 0.1.0     Constants created                    !
   \*----------------------------------------------------------------------------------------------------------*/
   #define CODESTREAM_OK         0x00
   #define CODESTREAM_ERROR      0x80
   #define CODESTREAM_SGI_READ   0x01
   #define CODESTREAM_SGC_READ   0x02
   #define CODESTREAM_SAX_READ   0x04
   #define CODESTREAM_COM_READ   0x08

   /*----------------------------------------------------------------------------------------------------------*\
   !   ENUM NAME: bwc_dwt_filter                                                                                !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !              These constants describe the wavelet filter used during the transform stage.                  !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Constant                    Description                                                     !
   !                --------                    -----------                                                     !
   !                bwc_dwt_9_7               - Cohen Daubechies Feauveau 9/7 Wavelet                           !
   !                bwc_dwt_5_3               - LeGall 5/3 Wavelet                                              !
   !                bwc_dwt_haar              - Haar Wavelet                                                    !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                07.12.2017  Patrick Vogler     B87D120     V 0.1.0     Enum created                         !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef enum{
      bwc_dwt_9_7,
      bwc_dwt_5_3,
      bwc_dwt_haar
   } bwc_dwt_filter;

   /*----------------------------------------------------------------------------------------------------------*\
   !   ENUM NAME: bwc_prog_ord                                                                                  !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !              These constants describe the progression order used to pack the codestream.                   !
   !              The organisation of the codesstream is carried out according to the following 5 cases.        !
   !                                                                                                            !
   !                 LRCP:                                  RLCP:                                               !
   !                 -----  for each Quality Layer:         -----  for each Resolution:                         !
   !                            for each Resolution:                   for each Quality Layer:                  !
   !                                for each Parameter:                    for each Parameter:                  !
   !                                    for each Precinct:                     for each Precinct:               !
   !                                          include Packet                         include Packet             !
   !                                                                                                            !
   !                 RPCL:                                  PCRL:                                               !
   !                 -----  for each Resolution:            -----  for each Precinct:                           !
   !                            for each Precinct:                     for each Parameter:                      !
   !                                for each Parameter:                    for each Resolution:                 !
   !                                    for each Quality Layer:                 for each Quality Layer:         !
   !                                                 include Packet                          include Packet     !
   !                                                                                                            !
   !                 CPRL:                                                                                      !
   !                 -----  for each Quality Layer:                                                             !
   !                            for each Resolution:                                                            !
   !                                for each Parameter:                                                         !
   !                                    for each Precinct:                                                      !
   !                                        include Packet                                                      !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Constant                    Description                                                     !
   !                --------                    -----------                                                     !
   !                bwc_prog_LRCP             - LRCP progression order                                          !
   !                bwc_prog_RLCP             - RLCP progression order                                          !
   !                bwc_prog_RPCL             - RPCL progression order                                          !
   !                bwc_prog_PCRL             - PCRL progression order                                          !
   !                bwc_prog_CPRL             - CPRL progression order                                          !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                07.12.2017  Patrick Vogler     B87D120     V 0.1.0     Enum created                         !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef enum{
      bwc_prog_LRCP,
      bwc_prog_RLCP,
      bwc_prog_RPCL,
      bwc_prog_PCRL,
      bwc_prog_CPRL
   } bwc_prog_ord;

   /*----------------------------------------------------------------------------------------------------------*\
   !   ENUM NAME: bwc_quant_st                                                                                  !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                DESCRIPTION NEEDED                                                                          !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Constant                    Description                                                     !
   !                --------                    -----------                                                     !
   !                -                           -                                                               !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                07.12.2017  Patrick Vogler     B87D120     V 0.1.0     Enum created                         !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef enum{
      bwc_qt_none,
      bwc_qt_derived,
   } bwc_quant_st;

   /*----------------------------------------------------------------------------------------------------------*\
   !   ENUM NAME: bwc_tile_instr                                                                                !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                DESCRIPTION NEEDED                                                                          !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Constant                    Description                                                     !
   !                --------                    -----------                                                     !
   !                -                           -                                                               !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                17.04.2019  Patrick Vogler     B87D120     V 0.1.0     Enum created                         !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef enum{
      bwc_tile_sizeof,
      bwc_tile_numbof,
   } bwc_tile_instr;
#endif