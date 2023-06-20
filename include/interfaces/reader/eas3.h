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
||      File:  header.h                                                                                                             ||
||      -----                                                                                                                       ||
||                                                                                                                                  ||
||      DESCRIPTION:                                                                                                                ||
||      ------------                                                                                                                ||
||                         This file defines simple read and write functions used to access conforming eas3 datasets.               ||
||                                                                                                                                  ||
||      STRUCTS:                                                                                                                    ||
||      --------                                                                                                                    ||
||      - eas3_std_params                                                                                                           ||
||                                                                                                                                  ||
||      PUBLIC FUNCTIONS:                                                                                                           ||
||      -----------------                                                                                                           ||
||      - read_eas3                                                                                                                 ||
||      - write_eas3                                                                                                                ||
||                                                                                                                                  ||
||      DEVELOPMENT HISTORY:                                                                                                        ||
||      --------------------                                                                                                        ||
||                                                                                                                                  ||
||                            Date        Author             Change Id   Release     Description Of Change                          ||
||                            ----        ------             ---------   -------     ---------------------                          ||
||                            -           Patrick Vogler     B87D120     V 0.1.0     header file created                            ||
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
#ifndef EAS3_H
#define EAS3_H

   /************************************************************************************************************\
   ||                                      _ _  _ ____ _    _  _ ___  ____                                     ||
   ||                                      | |\ | |    |    |  | |  \ |___                                     ||
   ||                                      | | \| |___ |___ |__| |__/ |___                                     ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   #include <bwc.h>
   #include <stdio.h>
   #include <stdint.h>

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
   !                   These macros are used to identify the spatial and temporal dimensions.                   !
   !                                                                                                            !
   !   MACROS:                                                                                                  !
   !   -------                                                                                                  !
   !                   Name                                            Description                              !
   !                   ----                                            -----------                              !
   !                   DIM_X, DIM_Y, DIM_Z                           - Spatial Dimensions                       !
   !                                                                                                            !
   !                   DIM_TS                                        - Temporal Dimension                       !
   !                                                                                                            !
   !                   DIM_ALL                                       - All Dimensions                           !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                   Date        Author             Change Id   Release     Description                       !
   !                   ----        ------             ---------   -------     -----------                       !
   !                   04.12.2017  Patrick Vogler     B87D120     V 0.1.0     macros created                    !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   #define DIM_X                   1
   #define DIM_Y                   2
   #define DIM_Z                   4
   #define DIM_TS                  8
   #define DIM_ALL                 15

   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !              These macros are used to instruct the read_eas3_header function.                              !
   !                                                                                                            !
   !   Macros:                                                                                                  !
   !   -------                                                                                                  !
   !                Macro                                Description                                            !
   !                -----                                -----------                                            !
   !                EAS3_NO_ATTR    1                  - Specifies that no attributes are present in the bit-   !
   !                                                     stream.                                                !
   !                                                                                                            !
   !                EAS3_ALL_ATTR   2                  - Specifies that all attributes are present in the bit-  !
   !                                                     bitstream.                                             !
   !                                                                                                            !
   !                ATTRLEN        10                  - Defines the number of bytes used to define an attrib-  !
   !                                                     ute.                                                   !
   !                                                                                                            !
   !                UDEFLEN        20                  - Defines the number of bytes used to define a user de-  !
   !                                                     fined data.                                            !
   !                                                                                                            !
   !                EAS3_NO_G       1                  - Specifies that no geometry data is present in the bit- !
   !                                                     stream.                                                !
   !                                                                                                            !
   !                EAS3_X0DX_G     2                  - Specifies that a start value and step size are present !
   !                                                     in the bitstream.                                      !
   !                                                                                                            !
   !                EAS3_UDEF_G     3                  - Specifies that ng = number of coordinate data is pres- !
   !                                                     ent in the bitstream.                                  !
   !                                                                                                            !
   !                EAS3_ALL_G      4                  - Specifies that an element for every appropriate di-    !
   !                                                     mension is present in the bitstream.                   !
   !                                                                                                            !
   !                EAS3_FULL_G     5                  - Specifies that an element for every dimension is pres- !
   !                                                     ent in the bitstream.                                  !
   !                                                                                                            !
   !                EAS3_NO_UDEF    1                  - Specifies that no user defined data is present in the  !
   !                                                     bitstream.                                             !
   !                                                                                                            !
   !                EAS3_ALL_UDEF   2                  - Specifies that all user defined fields are present in  !
   !                                                     the bitstream.                                         !
   !                                                                                                            !
   !                EAS3_INT_UDEF   3                  - Specifies that an user defined integer field is pres-  !
   !                                                     ent in the bitstream.                                  !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                20.06.2018  Patrick Vogler     B87D120     V 0.1.0     Macros created                       !
   \*----------------------------------------------------------------------------------------------------------*/
   #define EAS3_NO_ATTR    0x100000000000000
   #define EAS3_ALL_ATTR   0x200000000000000

   #define EAS2_TYPE       0x100000000000000
   #define EAS3_TYPE       0x200000000000000

   #define ATTRLEN         10
   #define UDEFLEN         20

   #define EAS3_NO_G       0x100000000000000
   #define EAS3_X0DX_G     0x200000000000000
   #define EAS3_UDEF_G     0x300000000000000
   #define EAS3_ALL_G      0x400000000000000
   #define EAS3_FULL_G     0x500000000000000

   #define EAS3_NO_UDEF    0x100000000000000
   #define EAS3_ALL_UDEF   0x200000000000000
   #define EAS3_INT_UDEF   0x300000000000000

   #define AUX_SIZE        0x8000

   /************************************************************************************************************\
   ||                                         ___ _   _ ___  ____ ____                                         ||
   ||                                          |   \_/  |__] |___ [__                                          ||
   ||                                          |    |   |    |___ ___]                                         ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: eas3_header                                                                                 !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure is used to store the eas3 header information. For a more thorough discussion !
   !                of the eas3 datatype see:                                                                   !
   !                                                                                                            !
   !                         https://wiki.iag.uni-stuttgart.de/eas3wiki/index.php/EAS3_File_Format/de           !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                file_type                   unsigned int(64 bit)  - Defines an identifier for the EAS type. !
   !                                                                                                            !
   !                accuracy                    unsigned int(64 bit)   - Defines the accuracy of an eas3 file.  !
   !                                                                                                            !
   !                nzs                         unsigned int(64 bit)  - Variable defining the temporal size of  !
   !                                                                    the uncompressed dataset.               !
   !                                                                                                            !
   !                npar                        unsigned int(64 bit)  - Defines the number of parameters in the !
   !                                                                    eas3 dataset.                           !
   !                                                                                                            !
   !                ndim1, -2, -3               unsigned int(64 bit)  - Variables defining the spatial size of  !
   !                                                                    the eas3 dataset.                       !
   !                                                                                                            !
   !                attribute_mode;             unsigned int(64 bit)  - Defines an identifier used to signal    !
   !                                                                    the attribute mode of the eas3 file.    !
   !                                                                                                            !
   !                gmode_time                  unsigned int(64 bit)  - Defines the geometry mode for the temp- !
   !                                                                    oral dimension.                         !
   !                                                                                                            !
   !                gmode_param                 unsigned int(64 bit)  - Defines the geometry mode for the para- !
   !                                                                    meters.                                 !
   !                                                                                                            !
   !                gmode_dim1                  unsigned int(64 bit)  - Defines the geometry mode for the first !
   !                                                                    spatial dimension.                      !
   !                                                                                                            !
   !                gmode_dim2                  unsigned int(64 bit)  - Defines the geometry mode for the sec-  !
   !                                                                    ond spatial dimension.                  !
   !                                                                                                            !
   !                gmode_dim3                  unsigned int(64 bit)  - Defines the geometry mode for the third !
   !                                                                    spatial dimension.                      !
   !                                                                                                            !
   !                size_time                   unsigned int(64 bit)  - Defines the geometry array size for the !
   !                                                                    temporal dimension.                     !
   !                                                                                                            !
   !                size_parameter              unsigned int(64 bit)  - Defines the geometry array size for the !
   !                                                                    parameters.                             !
   !                                                                                                            !
   !                size_dim1                   unsigned int(64 bit)  - Defines the geometry array size for the !
   !                                                                    first spatial dimension.                !
   !                                                                                                            !
   !                size_dim2                   unsigned int(64 bit)  - Defines the geometry array size for the !
   !                                                                    second spatial dimension.               !
   !                                                                                                            !
   !                size_dim3                   unsigned int(64 bit)  - Defines the geometry array size for the !
   !                                                                    third spatial dimension.                !
   !                                                                                                            !
   !                udef_param                  unsigned int(64 bit)  - Defines a marker used to signal which   !
   !                                                                    user defined parameters are present in  !
   !                                                                    the eas3 file.                          !
   !                                                                                                            !
   !                udef_char_size              unsigned int(64 bit)  - Defines the size for the user defined   !
   !                                                                    char array.                             !
   !                                                                                                            !
   !                udef_int_size               unsigned int(64 bit)  - Defines the size for the user defined   !
   !                                                                    int array.                              !
   !                                                                                                            !
   !                udef_real_size              unsigned int(64 bit)  - Defines the size for the user defined   !
   !                                                                    real array.                             !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                -                           -                       -                                       !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                20.06.2018  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint64_t              file_type;
      uint64_t              accuracy;
      uint64_t              nzs;
      uint64_t              npar;
      uint64_t              ndim1;
      uint64_t              ndim2;
      uint64_t              ndim3;
      uint64_t              attribute_mode;
      uint64_t              gmode_time;
      uint64_t              gmode_param;
      uint64_t              gmode_dim1;
      uint64_t              gmode_dim2;
      uint64_t              gmode_dim3;
      uint64_t              size_time;
      uint64_t              size_parameter;
      uint64_t              size_dim1;
      uint64_t              size_dim2;
      uint64_t              size_dim3;
      uint64_t              udef_param;
      uint64_t              udef_char_size;
      uint64_t              udef_int_size;
      uint64_t              udef_real_size;
   } eas3_std_params;
   
   /************************************************************************************************************\
   ||                  ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                  ||
   ||                  |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__                   ||
   ||                  |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]                  ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: bwc_data* read_eas3(const char* const filename)                                           !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function opens an eas3 file and checks it for its validity. Once the specified file    !
   !                has been verified, its header and flow field data is read and stored in the bwc_data        !
   !                structure.                                                                                  !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   bwc_data*
   read_eas3(char *const filename);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: uchar write_eas3(bwc_data *const file, char *const filename)                              !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function creates a valid eas3 file from the information stored in the bwc_data         !
   !                structure.                                                                                  !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   uchar
   write_eas3(bwc_data *const file, char *const filename);
#endif