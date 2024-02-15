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
||      File:  types.h                                                                                                              ||
||      -----                                                                                                                       ||
||                                                                                                                                  ||
||      DESCRIPTION:                                                                                                                ||
||      ------------                                                                                                                ||
||      This header defines a set of basic arithmetic types with specified widths to be used in the big whoop compression           ||
||      algorithm. The width of an arithmetic type is defined as the number of bits used to store its value.                        ||
||                                                                                                                                  ||
||      STRUCTS:                                                                                                                    ||
||      --------                                                                                                                    ||
||      - IEEE_754_Double_Prec                                                                                                      ||
||      - bwc_sample                                                                                                                ||
||      - bwc_cblk_ctrl                                                                                                             ||
||      - bwc_cblk_inf                                                                                                              ||
||      - bwc_codeblock                                                                                                             ||
||      - bwc_prec_ctrl                                                                                                             ||
||      - bwc_prec_inf                                                                                                              ||
||      - bwc_precinct                                                                                                              ||
||      - bwc_subb_ctrl                                                                                                             ||
||      - bwc_subb_inf                                                                                                              ||
||      - bwc_subband                                                                                                               ||
||      - bwc_res_ctrl                                                                                                              ||
||      - bwc_res_inf                                                                                                               ||
||      - bwc_resolution                                                                                                            ||
||      - bwc_param_ctrl                                                                                                            ||
||      - bwc_param_inf                                                                                                             ||
||      - bwc_parameter                                                                                                             ||
||      - bwc_tile_ctrl                                                                                                             ||
||      - bwc_tile_inf                                                                                                              ||
||      - bwc_tile                                                                                                                  ||
||      - bwc_mpi_ctrl                                                                                                              ||
||      - bwc_gl_ctrl                                                                                                               ||
||      - bwc_cmd_opts_ll                                                                                                           ||
||      - bwc_gl_inf                                                                                                                ||
||      - bwc_field                                                                                                                 ||
||                                                                                                                                  ||
||      DEVELOPMENT HISTORY:                                                                                                        ||
||      --------------------                                                                                                        ||
||                                                                                                                                  ||
||                            Date        Author             Change Id   Release     Description Of Change                          ||
||                            ----        ------             ---------   -------     ---------------------                          ||
||                            10.10.2017  Patrick Vogler     B87D120     V 0.1.0     header file created                            ||
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
#ifndef BWC_TYPES_H
#define BWC_TYPES_H

   #include <stdio.h>
   /************************************************************************************************************\
   ||                                      _ _  _ ____ _    _  _ ___  ____                                     ||
   ||                                      | |\ | |    |    |  | |  \ |___                                     ||
   ||                                      | | \| |___ |___ |__| |__/ |___                                     ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   #ifdef BWC_SINGLE_PRECISION
      #include "prim_types_single.h"
   #else
      #include "prim_types_double.h"
   #endif

   #include "constants.h"
   #include "mq_types.h"

   /************************************************************************************************************\
   ||                     ___  ____ ____ ____ _ _  _ ____ ___     ___ _   _ ___  ____ ____                     ||
   ||                     |  \ |___ |__/ |__/ | |  | |___ |  \     |   \_/  |__] |___ [__                      ||
   ||                     |__/ |___ |  \ |  \ |  \/  |___ |__/     |    |   |    |___ ___]                     ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   /*----------------------------------------------------------------------------------------------------------*\
   !   UNION NAME: bwc_sample                                                                                   !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This union holds a floating point sample of the uncompressed data set and allows access to  !
   !                its raw bit representation.                                                                 !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                raw                         bwc_raw               - Raw access to the data sample.          !
   !                                                                                                            !
   !                f                           bwc_float             - Floating point representation of the    !
   !                                                                    data sample.                            !
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
   !                11.10.2017  Patrick Vogler     B87D120     V 0.1.0     union created                        !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef union
   {
      bwc_raw              raw;
      bwc_float            f;
   } bwc_sample;

   /*----------------------------------------------------------------------------------------------------------*\
   |                                   ___  _ ___ ____ ___ ____ ____ ____ _  _                                  |
   |                                   |__] |  |  [__   |  |__/ |___ |__| |\/|                                  |
   |                                   |__] |  |  ___]  |  |  \ |___ |  | |  |                                  |
   |                                                                                                            |
   \*----------------------------------------------------------------------------------------------------------*/
   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_packed_stream                                                                           !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure is used to store the reading/writing position, overall size and memory       !
   !                handle of a packed stream.                                                                  !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                position                    unsigned int(32 bit)  - Reading/Writing position in the packed  !
   !                                                                    stream.                                 !
   !                                                                                                            !
   !                size                        unsigned int(32 bit)  - Size of packed stream.                  !
   !                                                                                                            !
   !                access                      unsigned char*        - Memory handle used to parse the paced   !
   !                                                                    stream.                                 !
   !                                                                                                            !
   !                memory                      unsigned char*        - Memory handle for the packed stream.    !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                -                                                                                           !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                12.04.2019  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint64                position;
      uint64                size;
      uchar                *access;
      uchar                *memory;
   } bwc_packed_stream;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_stream                                                                                  !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                DESCRIPTION NEEDED                                                                          !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                -                                                                                           !
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
   !                28.05.2018  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint64                L;
      uint64                Lmax;
      uint64                size_incr;
      uint8                 T;
      int8                  t;
      uchar                 error;
      uchar                *memory;
   } bwc_stream;

   /*----------------------------------------------------------------------------------------------------------*\
   |                                     ____ _  _ ____ ____ ___  ____ ____                                     |
   |                                     |___ |\ | |    |  | |  \ |___ |__/                                     |
   |                                     |___ | \| |___ |__| |__/ |___ |  \                                     |
   |                                                                                                            |
   \*----------------------------------------------------------------------------------------------------------*/
   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_coder_stripe                                                                            !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds the sign, bitfield and state information for two vertically adjacent   !
   !                sample stripes for easy access during the entropy encoding stage. To facilitate distortion  !
   !                estimation the magnitude of the wavelet coefficients is stored in an appropriate sample     !
   !                array.                                                                                      !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                sample                      unsigned int*(64 bit) - Array holding two vertically adjacent   !
   !                                                                    wavelet coefficient stripes.            !
   !                                                                                                            !
   !                sigma_u, _r, _d, _l         unsigned int*(16 bit) - Pointer used to hold the address of the !
   !                                                                    sample states for the upper (_u), right !
   !                                                                    (_r), lower (_d) and left (_l) neighbor !
   !                                                                    wavelet coefficient stripe.             !
   !                                                                                                            !
   !                sigma                       unsigned int(16 bit)  - Sample states for the two vertically    !
   !                                                                    adjacent wavelet coefficient stripes.   !
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
   !                15.11.2018  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct stripe{
      uint64         *sample;
      uint8           codingpass;
      uint8           delta;
      uint8           sigma;
      uint8           bitplane;
      uint8           pi;
      uint8           xi;
      uint8          *bit;
      struct stripe  *stripe_u;
      struct stripe  *stripe_r;
      struct stripe  *stripe_d;
      struct stripe  *stripe_l;
   } bwc_coder_stripe;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_coder                                                                                   !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure defines a single tagtree node which stores the node value, node threshold as !
   !                well as the address of the parent node. The tagtree node structure is used in conjuncture   !
   !                with a bwc_tagtree instance to define a tagtree.                                            !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                -                           -                       -                                       !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                02.07.2018  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint64               buff_incr, buff_size;
      uint64               no_full_stripe, no_slice;
      uint64               width, height;
      uint8                K;
      uint8 const         *sig2context;
      uchar                erres;
      uchar                highband_flag;
      uchar               *compressed;
      bwc_bit_coder       *bitcoder;
      bwc_coder_stripe    *data;
   } bwc_coder;

   /*----------------------------------------------------------------------------------------------------------*\
   |                                      ___ ____ ____ ___ ____ ____ ____                                      |
   |                                       |  |__| | __  |  |__/ |___ |___                                      |
   |                                       |  |  | |__]  |  |  \ |___ |___                                      |
   |                                                                                                            |
   \*----------------------------------------------------------------------------------------------------------*/
   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_tagtree_node                                                                            !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure defines a single tagtree node which stores the node value, node threshold as !
   !                well as the address of the parent node. The tagtree node structure is used in conjuncture   !
   !                with a bwc_tagtree instance to define a tagtree.                                            !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                value                       unsigned int(16 bit)  - Tagtree node value.                     !
   !                                                                                                            !
   !                threshold                   unsigned int(16 bit)  - Tagtree node threshold.                 !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                parent                      node*                 - Address of the parent node.             !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                23.05.2018  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct node
   {
      uint64                index;
      uint16                value;
      uint16                threshold;
      struct node*          parent;
   } bwc_tagtree_node;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_tagtree                                                                                 !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure defines a tagtree instance which stores the number of spatial and temporal   !
   !                tagtree leafs as well as the tagtree itself defined by a bwc_tagtree_node linked list.      !
   !                The tagtree structure                                                                       !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                leavesX ,leavesY, leavesZ   unsigned int(64 bit)  - Variables defining the number of spa-   !
   !                                                                    tial tagtree leafs.                     !
   !                                                                                                            !
   !                leavesTS                    unsigned int(64 bit)  - Variable defining the number of tempo-  !
   !                                                                    ral tagtree leafs.                      !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                nodes                       bwc_tagtree_node*     - Linked list storing the tagtree values  !
   !                                                                    and thresholds.                         !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                10.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint64               leavesX, leavesY, leavesZ;
      uint64               leavesTS;
      bwc_tagtree_node    *nodes;
   } bwc_tagtree;

   /*----------------------------------------------------------------------------------------------------------*\
   |                                      ___  ____ ___ ____ ____ ____ ___                                      |
   |                                      |  \ |__|  |  |__| [__  |___  |                                       |
   |                                      |__/ |  |  |  |  | ___] |___  |                                       |
   |                                                                                                            |
   \*----------------------------------------------------------------------------------------------------------*/
   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_cmd_opts_ll                                                                             !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure defines a linked list which stores a parameters name, its index and bit      !
   !                precision.                                                                                  !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                size                        unsigned int(64 bit)  - Size of parameter.                      !
   !                                                                                                            !
   !                id                          unsigned int(8 bit)   - Index of the parameter.                 !
   !                                                                                                            !
   !                precision                   unsigned int(8 bit)   - Defines the precision of a compressed   !
   !                                                                    dataset.                                !
   !                                                                                                            !
   !                name                        char                  - Defines the name of parameter(id).      !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                next                        opt*                  - Address for the next linked list node.  !
   !                                                                                                            !
   !                root                        opt*                  - Address for the root linked list node.  !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                04.12.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                05.12.2019  Patrick Vogler     B87E7E4     V 0.1.0     Removal of unecessary parameters.    !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct opt
   {
      uint64                size;
      uint8                 id;
      uint8                 precision;
      char                  name[24];
      struct opt           *next;
      struct opt           *root;
   } bwc_cmd_opts_ll;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_gl_inf                                                                                  !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds all the global information of the uncompressed data set including its  !
   !                spatial and temporal dimensions, number of parameters, its bit-depth precision and all the  !
   !                parameter information.                                                                      !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                nX ,nY, nZ                  unsigned int(64 bit)  - Variables defining the spatial size of  !
   !                                                                    the uncompressed dataset.               !
   !                                                                                                            !
   !                nTS                         unsigned int(16 bit)  - Variable defining the temporal size of  !
   !                                                                    the uncompressed dataset.               !
   !                                                                                                            !
   !                nPar                        unsigned int(8 bit)   - Defines the number of parameters in the !
   !                                                                    uncompressed dataset.                   !
   !                                                                                                            !
   !                precision                   unsigned int(8 bit)   - Flag used to signal the encoder/decoder !
   !                                                                    precision.                              !
   !                                                                                                            !
   !                f_ext                       uchar*                - Character array used to hold the file   !
   !                                                                    extension                               !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                parameter                   bwc_cmd_opts_ll*      - Linked list storing the parameter names,!
   !                                                                    indices, size and precision.            !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                10.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                05.12.2019  Patrick Vogler     B87E7E4     V 0.1.0     Removal of unecessary parameters.    !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint64               nX, nY, nZ;
      uint16               nTS;
      uint8                nPar;
      uint8                precision;
      char                 f_ext[20];
      bwc_cmd_opts_ll     *parameter;
   } bwc_gl_inf;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_data                                                                                    !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure stores the numerical dataset/compressed bitstream passed to or returned      !
   !                from the (de)compressor.                                                                    !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                field                       field                 - Structure that holds the single (f) or  !
   !                                                                    double (d) precission parameters of a   !
   !                                                                    numerical dataset.                      !
   !                                                                                                            !
   !                file                        file                  - Structure that holds the file pointer   !
   !                                                                    and memory handle for single (f) or     !
   !                                                                    double (d) precission parameters of a   !
   !                                                                    numerical dataset.                      !
   !                                                                                                            !
   !                codestream                  codestream            - Structure that holds the packed data,   !
   !                                                                    auxiliary information (aux) and com-    !
   !                                                                    ment stream of a compressed dataset.    !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                info                        bwc_gl_inf*           - Structure holding all the global infor- !
   !                                                                    mation of the uncompressed data set.    !
   !                                                                                                            !
   !                data                        bwc_packed_stream*    - Stream holding the data memory block.   !
   !                                                                                                            !
   !                aux                         bwc_packed_stream*    - Stream holding the auxiliary informa-   !
   !                                                                    tion memory block.                      !
   !                                                                                                            !
   !                com                         bwc_packed_stream*    - Stream holding the commentary memory    !
   !                                                                    block.                                  !
   !                                                                                                            !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                17.04.2019  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                04.12.2019  Patrick Vogler     B87E7E4     V 0.1.0     Cleaned up the structure and added   !
   !                                                                       functionality to store both single   !
   !                                                                       and double precision datasets.       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      bwc_gl_inf              info;

      struct codestream
      {
         bwc_packed_stream   *data;
         bwc_packed_stream   *aux;
         bwc_packed_stream   *com;
      }codestream;

      struct field
      {
         double              *d;
         float               *f;
      }field;

      struct file
      {
         FILE                *fp;
         uint64              *d_root;
         uint64              *f_root;
      }file;
   } bwc_data;

   /*----------------------------------------------------------------------------------------------------------*\
   |                            ____ ____ _  _ ___  ____ ____ ____ ____ _ ____ _  _                             |
   |                            |    |  | |\/| |__] |__/ |___ [__  [__  | |  | |\ |                             |
   |                            |___ |__| |  | |    |  \ |___ ___] ___] | |__| | \|                             |
   |                                                                                                            |
   \*----------------------------------------------------------------------------------------------------------*/
   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_encoded_block                                                                           !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds all the necessary parameters used to define and control a bwc          !
   !                codeblock.                                                                                  !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                -                           -                       -                                       !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                info                        bwc_cblk_inf*         - Structure holding all the codeblock in- !
   !                                                                    formation.                              !
   !                                                                                                            !
   !                control                     bwc_cblk_ctrl*        - Structure holding all the codeblock     !
   !                                                                    control parameters.                     !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                23.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint64               L[MAXIMUM_NO_PASSES];
      uint16               S[MAXIMUM_NO_PASSES + 1];
      uint8                Kmsbs;
      uint8                K;
      uint8                Z;
      uchar               *data;
   } bwc_encoded_cblk;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_cblk_ctrl                                                                               !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds all the control parameters for the current codeblock used to instruct  !
   !                the bwc codec to (de)compress a floating point array.                                       !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                -                           -                       -                                       !
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
   !                23.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint16               beta;
      uint16               beta_est;
      uint16               K;
      int16               *cp_contr;
      int16               *memory;
   } bwc_cblk_ctrl;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_cblk_inf                                                                                !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds the dimensions for the current codeblock.                              !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                X0, Y0, Z0, TS0             unsigned int(32 bit)  - Variables defining the spatial starting !
   !                                                                    point of a codeblock.                   !
   !                                                                                                            !
   !                X1, Y1, Z1, TS1             unsigned int(32 bit)  - Variables defining the spatial end      !
   !                                                                    point of a codeblock.                   !
   !                                                                                                            !
   !                idx                         unsigned int(32 bit)  - Variable defining codeblock index in    !
   !                                                                    the current precinct.                   !
   !                                                                                                            !
   !                TS0                         unsigned int(16 bit)  - Variable defining the temporal start-   !
   !                                                                    ing point of a codeblock.               !
   !                                                                                                            !
   !                TS1                         unsigned int(16 bit)  - Variable defining the temporal end      !
   !                                                                    point of a codeblock.                   !
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
   !                23.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint64               X0, Y0, Z0;
      uint64               X1, Y1, Z1;
      uint32               idx;
      uint16               TS0;
      uint16               TS1;
   } bwc_cblk_inf;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_codeblock                                                                               !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds all the necessary parameters used to define and control a bwc          !
   !                codeblock.                                                                                  !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                -                           -                       -                                       !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                info                        bwc_cblk_inf*         - Structure holding all the codeblock in- !
   !                                                                    formation.                              !
   !                                                                                                            !
   !                control                     bwc_cblk_ctrl*        - Structure holding all the codeblock     !
   !                                                                    control parameters.                     !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                23.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      bwc_cblk_inf          info;
      bwc_cblk_ctrl         control;
      bwc_encoded_cblk     *encoded_block;
   } bwc_codeblock;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_prec_ctrl                                                                               !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds all the control parameters for the current precinct used to instruct   !
   !                the bwc codec to (de)compress a floating point array.                                       !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                number_of_codeblocks        unsigned int(64 bit)  - Defines the number of codeblocks for    !
   !                                                                    the current resolution level.           !
   !                                                                                                            !
   !                numPX, -PY, -PZ, -PTS       unsigned int(16 bit)  - Variables defining the number of code-  !
   !                                                                    blocks for the current resolution level !
   !                                                                    in all spatial and temporal directions. !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                tag_inclusion               bwc_tagtree*          - Tagtree identifying wether the current  !
   !                                                                    codeblock contributes to a specific     !
   !                                                                    quality layer.                          !
   !                                                                                                            !
   !                tag_msbs                    bwc_tagtree*          - Tagtree identifying the number of mag-  !
   !                                                                    nitude bit planes used to represent the !
   !                                                                    samples of the current codeblock.       !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                23.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint64               number_of_codeblocks;
      uint16               numCbX, numCbY, numCbZ, numCbTS;
      bwc_tagtree         *tag_inclusion;
      bwc_tagtree         *tag_msbs;
   } bwc_prec_ctrl;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_prec_inf                                                                                !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds the dimensions for the current precinct.                               !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                X0, Y0, Z0, TS0             unsigned int(32 bit)  - Variables defining the spatial starting !
   !                                                                    point of a precinct.                    !
   !                                                                                                            !
   !                X1, Y1, Z1, TS1             unsigned int(32 bit)  - Variables defining the spatial end      !
   !                                                                    point of a precinct.                    !
   !                                                                                                            !
   !                TS0                         unsigned int(16 bit)  - Variables defining the temporal start-  !
   !                                                                    ing point of a precinct.                !
   !                                                                                                            !
   !                TS1                         unsigned int(16 bit)  - Variables defining the temporal end     !
   !                                                                    point of a precinct.                    !
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
   !                23.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint32               X0, Y0, Z0;
      uint32               X1, Y1, Z1;
      uint16               TS0;
      uint16               TS1;
   } bwc_prec_inf;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_precinct                                                                                !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds all the necessary parameters used to define and control a bwc          !
   !                precinct.                                                                                   !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                -                           -                       -                                       !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                info                        bwc_prec_inf*         - Structure holding all the precinct in-  !
   !                                                                    formation.                              !
   !                                                                                                            !
   !                control                     bwc_prec_ctrl*        - Structure holding all the precinct con- !
   !                                                                    trol parameters.                        !
   !                                                                                                            !
   !                codeblock                   bwc_codeblock*        - Structure defining a bwc codeblock.     !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                23.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      bwc_prec_inf         info;
      bwc_prec_ctrl        control;
      bwc_codeblock       *codeblock;
   } bwc_precinct;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_subb_ctrl                                                                               !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds all the control parameters for the current subband used to instruct    !
   !                the bwc codec to (de)compress a floating point array.                                       !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                Kmax                        unsigned int(16 bit)  - Defines the dynamic range of the cur-   !
   !                                                                    rent subband after transformation.      !
   !                                                                                                            !
   !                qt_mantissa                 unsigned int(16 bit)  - Defines the mantissa of the subband     !
   !                                                                    quantisation step size.                 !
   !                                                                                                            !
   !                qt_exponent                 unsigned int(8 bit)   - Defines the exponent of the subband     !
   !                                                                    quantisation step size.                 !
   !                                                                                                            !
   !                highband_flag               unsigned char         - Defines the type of highband the cur-   !
   !                                                                    rent subband represents.                !
   !                                                                                                            !
   !                qt_effective_step_size      bwc_float             - Defines the effective quantization step !
   !                                                                    size for the current subband.           !
   !                                                                                                            !
   !                qt_step_size                bwc_float             - Defines the quantization step size for  !
   !                                                                    the current subband.                    !
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
   !                23.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint16               Kmax;
      uint16               qt_mantissa;
      uint8                qt_exponent;
      uchar                highband_flag;
      bwc_float            qt_effective_step_size;
      bwc_float            qt_step_size;
   } bwc_subb_ctrl;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_subb_inf                                                                                !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds the dimensions and the energy gain factor for the current subband.     !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                X0, Y0, Z0, TS0             unsigned int(64 bit)  - Variables defining the spatial starting !
   !                                                                    point of a subband.                     !
   !                                                                                                            !
   !                X1, Y1, Z1, TS1             unsigned int(64 bit)  - Variables defining the spatial end      !
   !                                                                    point of a subband.                     !
   !                                                                                                            !
   !                TS0                         unsigned int(16 bit)  - Variables defining the temporal start-  !
   !                                                                    ing point of a subband.                 !
   !                                                                                                            !
   !                TS1                         unsigned int(16 bit)  - Variables defining the temporal end     !
   !                                                                    point of a subband.                     !
   !                                                                                                            !
   !                dwt_gain                    bwc_float             - Defines the energy gain factor for the  !
   !                                                                    current subband.                        !
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
   !                23.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint64               X0, Y0, Z0;
      uint64               X1, Y1, Z1;
      uint16               TS0;
      uint16               TS1;
      bwc_float            dwt_gain;
   } bwc_subb_inf;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_subband                                                                                 !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds all the necessary parameters used to define and control a bwc subband. !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                -                           -                       -                                       !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                info                        bwc_subb_inf*         - Structure holding all the subband in-   !
   !                                                                    formation.                              !
   !                                                                                                            !
   !                control                     bwc_subb_ctrl*        - Structure holding all the subband con-  !
   !                                                                    trol parameters.                        !
   !                                                                                                            !
   !                precinct                    bwc_precinct*         - Structure defining a bwc precinct.      !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                23.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      bwc_subb_inf         info;
      bwc_subb_ctrl        control;
      bwc_precinct        *precinct;
   } bwc_subband;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_packet                                                                                  !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds the position of the current packet in the compressed bitstream, the    !
   !                total number of bytes occupied by the packet as well as the packed codestreams of the pack- !
   !                et body and header.                                                                         !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                size                        unsigned int(64 bit)  - Overall size of the codestream packet.  !
   !                e                           unsigned int(8 bit)   - Identifier used as an indicator if no   !
   !                                                                    codeblock contributions are present in  !
   !                                                                    current packet.                         !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                body                        bwc_packed_stream     - Structure holding the packed codestream !
   !                                                                    of the current packet body.             !
   !                                                                                                            !
   !                header                      bwc_packed_stream     - Structure holding the packed codestream !
   !                                                                    of the current packet header.           !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                23.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint32               size;
      uint32               p;
      uint8                e;
      uint8                c, l, r;
      bwc_packed_stream    body;
      bwc_packed_stream    header;
   } bwc_packet;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_res_ctrl                                                                                !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds all the control parameters for the current resolution level used to    !
   !                instruct the bwc codec to (de)compress a floating point array.                              !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                number_of_precincts         unsigned int(64 bit)  - Defines the number of precincts for the !
   !                                                                    current resolution level.               !
   !                                                                                                            !
   !                numPX, -PY, -PZ, -PTS       unsigned int(16 bit)  - Variables defining the number of pre-   !
   !                                                                    cinct for the current resolution level  !
   !                                                                    in all spatial and temporal directions. !
   !                                                                                                            !
   !                number_of_subbands          unsigned int(8 bit)   - Defines the number of subbands for the  !
   !                                                                    current resolution level.               !
   !                                                                                                            !
   !                rcbX, rcbY, rcbZ, rcbTS     unsigned int(8 bit)   - Variables defining the real codeblock   !
   !                                                                    size for the current resolution level.  !
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
   !                23.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint64               number_of_precincts;
      uint16               numPX, numPY, numPZ, numPTS;
      uint8                number_of_subbands;
      uint8                rcbX, rcbY, rcbZ, rcbTS;
   } bwc_res_ctrl;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_res_inf                                                                                 !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds the dimensions for the current resolution level.                       !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                X0, Y0, Z0, TS0             unsigned int(64 bit)  - Variables defining the spatial starting !
   !                                                                    point of a resolution level.            !
   !                                                                                                            !
   !                X1, Y1, Z1, TS1             unsigned int(64 bit)  - Variables defining the spatial end      !
   !                                                                    point of a resolution level.            !
   !                                                                                                            !
   !                TS0                         unsigned int(16 bit)  - Variables defining the temporal start-  !
   !                                                                    ing point of a resolution level.        !
   !                                                                                                            !
   !                TS1                         unsigned int(16 bit)  - Variables defining the temporal end     !
   !                                                                    point of a resolution level.            !
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
   !                23.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint64               X0, Y0, Z0;
      uint64               X1, Y1, Z1;
      uint16               TS0;
      uint16               TS1;
   } bwc_res_inf;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_resolution                                                                              !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds all the necessary parameters used to define and control a bwc resolu-  !
   !                tion level.                                                                                 !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                -                           -                       -                                       !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                info                        bwc_res_inf*          - Structure holding all the resolution    !
   !                                                                    level information.                      !
   !                                                                                                            !
   !                control                     bwc_res_ctrl*         - Structure holding all the resolution    !
   !                                                                    level control parameters.               !
   !                                                                                                            !
   !                subband                     bwc_subband*          - Structure defining a bwc subband.       !
   !                                                                                                            !
   !                packet                      bwc_packet**          - Structure defining a bwc packet.        !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                23.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      bwc_res_inf          info;
      bwc_res_ctrl         control;
      bwc_subband         *subband;
      bwc_packet          *packet;
   } bwc_resolution;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_cblk_access                                                                             !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure is used to directly access the parameter codeblocks during entropy de-/      !
   !                encoding to facilitate shared memory parallelization.                                       !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                codeblock                   bwc_codeblock*        - Structure defining a bwc codeblock.     !
   !                                                                                                            !
   !                precinct                    bwc_precinct*         - Structure defining a bwc precinct.      !
   !                                                                                                            !
   !                subband                     bwc_subband*          - Structure defining a bwc subband.       !
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
   !                17.10.2018  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   { 
      bwc_codeblock       *codeblock;
      bwc_precinct        *precinct;
      bwc_subband         *subband;
   } bwc_cblk_access;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_param_ctrl                                                                              !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds all the control parameters for the current tile param. used to in-     !
   !                struct the bwc codec to (de)compress a floating point array.                                !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                number_of_codeblocks        unsigned int(64 bit)  - Defines the number of codeblocks for    !
   !                                                                    the current tile parameter.             !
   !                                                                                                            !
   !                alpha, beta                 bwc_float             - Parameters used to normalized the cur-  !
   !                                                                    rent tile parameter.                    !
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
   !                11.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   { 
      uint64               number_of_codeblocks;
      bwc_float            alpha, beta;
   } bwc_param_ctrl;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_param_inf                                                                               !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds the dimensions, name and maximum exponent for the current tile param-  !
   !                eter of the uncompressed data set.                                                          !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                X0, Y0, Z0, TS0             unsigned int(64 bit)  - Variables defining the spatial starting !
   !                                                                    point of a tile parameter.              !
   !                                                                                                            !
   !                X1, Y1, Z1, TS1             unsigned int(64 bit)  - Variables defining the spatial end      !
   !                                                                    point of a tile parameter.              !
   !                                                                                                            !
   !                TS0                         unsigned int(16 bit)  - Variables defining the temporal start-  !
   !                                                                    ing point of a tile parameter.          !
   !                                                                                                            !
   !                TS1                         unsigned int(16 bit)  - Variables defining the temporal end     !
   !                                                                    point of a tile parameter.              !
   !                                                                                                            !
   !                parameter_max               bwc_float             - Defines the maximum value of the corre- !
   !                                                                    sponding parameter.                     !
   !                                                                                                            !
   !                parameter_min               bwc_float             - Defines the minimum value of the corre- !
   !                                                                    sponding parameter.                     !
   !                                                                                                            !
   !                parameter_name              char                  - Defines the name of the corresponding   !
   !                                                                    parameter.                              !
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
   !                11.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint64               size;
      uint64               X0, Y0, Z0;
      uint64               X1, Y1, Z1;
      uint16               TS0;
      uint16               TS1;
      uint8                precision;
      bwc_float            parameter_max;
      bwc_float            parameter_min;
      char                *name;
   } bwc_param_inf;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_parameter                                                                               !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds all the necessary parameters used to define and control the (de)com-   !
   !                pression of a bwc tile parameter. Furthermore, this structure stores the uncompressed       !
   !                floating point dataset pertaining to the tile parameter.                                    !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                -                           -                       -                                       !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                info                        bwc_param_inf*        - Structure holding all the tile param-   !
   !                                                                    eter information of the uncompressed    !
   !                                                                    data set.                               !
   !                                                                                                            !
   !                control                     bwc_param_ctrl*       - Structure holding all the tile param-   !
   !                                                                    control parameters for the bwc codec.   !
   !                                                                                                            !
   !                resolution                  bwc_resolution*       - Structure defining a bwc resolution     !
   !                                                                    level.                                  !
   !                                                                                                            !
   !                access                      bwc_cblk_access*      - Structure used to directly access the   !
   !                                                                    parameter codeblocks during the entropy !
   !                                                                    de-/encoding.                           !
   !                                                                                                            !
   !                data                        bwc_sample*           - Structure holding the uncompressed data !
   !                                                                    set pertaining to the tile parameter.   !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                11.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      bwc_param_inf        info;
      bwc_param_ctrl       control;
      bwc_cblk_access     *access;
      bwc_resolution      *resolution;
      bwc_sample          *data;
   } bwc_parameter;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_tile_ctrl                                                                               !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds all the control parameters for the current tile used to instruct the   !
   !                bwc codec to (de)compress a floating point array.                                           !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                body_size                   unsigned int(64 bit)  - Defines the size of the tile parameter  !
   !                                                                    body.                                   !
   !                                                                                                            !
   !                Ltp                         unsigned int(64 bit)  - Tile part size.                         !
   !                                                                                                            !
   !                header_size                 unsigned int(32 bit)  - Defines the approximate size of the     !
   !                                                                    tile parameter header.                  !
   !                                                                                                            !
   !                nPackets                    unsigned int(32 bit)  - Number of packets assembled for the     !
   !                                                                    current tile.                           !
   !                                                                                                            !
   !                max_Prec                    unsigned int(32 bit)  - Defines the maximum number of precincts !
   !                                                                    between all tile parameters.            !
   !                                                                                                            !
   !                dflt_param_c, dflt_param_q  unsigned int(16 bit)  - Variables defining the default param-   !
   !                                                                    eter index to use when writing COD (c)  !
   !                                                                    or QCD (q) in main header.              !
   !                                                                                                            !
   !                slope_min, slope_max        unsigned int(16 bit)  - Defines the minimum and maximum value   !
   !                                                                    for the convex hull slope threshold.    !
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
   !                11.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint64               body_size;
      uint64               Ltp;
      uint32               header_size;
      uint32               nPackets;
      uint32               max_Prec;
      uint16               dflt_param_c, dflt_param_q;
      uint16               slope_min, slope_max;
   } bwc_tile_ctrl;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_tile_inf                                                                                !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds the dimensions for the current tile of the uncompressed data set.      !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                size                        unsigned int(64 bit)  - Variable defining the size of a tile    !
   !                                                                                                            !
   !                X0, Y0, Z0, TS0             unsigned int(64 bit)  - Variables defining the spatial starting !
   !                                                                    point of a tile.                        !
   !                                                                                                            !
   !                X1, Y1, Z1, TS1             unsigned int(64 bit)  - Variables defining the spatial end      !
   !                                                                    point of a tile.                        !
   !                                                                                                            !
   !                tile_index                  unsigned int(32 bit)  - Index used to uniquely identify a tile. !
   !                                                                                                            !
   !                TS0                         unsigned int(16 bit)  - Variables defining the temporal start-  !
   !                                                                    ing point of a tile.                    !
   !                                                                                                            !
   !                TS1                         unsigned int(16 bit)  - Variables defining the temporal end     !
   !                                                                    point of a tile.                        !
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
   !                11.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint64               size;
      uint64               X0, Y0, Z0;
      uint64               X1, Y1, Z1;
      uint32               tile_index;
      uint16               TS0;
      uint16               TS1;
   } bwc_tile_inf;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_tile                                                                                    !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds all the necessary parameters used to define and control the (de)com-   !
   !                pression of a bwc tile.                                                                     !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                -                           -                       -                                       !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                info                        bwc_tile_inf*         - Structure holding all the tile infor-   !
   !                                                                    mation of the uncompressed data set.    !
   !                                                                                                            !
   !                control                     bwc_tile_ctrl*        - Structure holding all the tile control  !
   !                                                                                                            !
   !                parameter                   bwc_parameter*        - Structure defining a bwc tile parameter.!
   !                                                                                                            !
   !                packet_sequence             bwc_packet**          - Array of pointers used to sequence the  !
   !                                                                    codestream packets.                     !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                11.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      bwc_tile_inf         info;
      bwc_tile_ctrl        control;
      bwc_parameter       *parameter;
      bwc_packet         **packet_sequence;
   } bwc_tile;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_gl_ctrl                                                                                 !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds all the global control parameters used to instruct the bwc codec to    !
   !                (de)compress a floating point array.                                                        !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                tileSizeX, -Y, -Z           unsigned int(64 bit)  - Variables defining spatial size of a    !
   !                                                                    tile.                                   !
   !                                                                                                            !
   !                nTiles                      unsigned int(32 bit)  - Defines the global number of tiles.     !
   !                                                                                                            !
   !                qt_exponent                 unsigned int(32 bit)  - Defines the exponent of the global      !
   !                                                                    quantisation step size.                 !
   !                                                                                                            !
   !                qt_mantissa                 unsigned int(32 bit)  - Defines the mantissa of the global      !
   !                                                                    quantisation step size.                 !
   !                                                                                                            !
   !                CSsgc                       unsigned int(16 bit)  - Variable used to keep track which con-  !
   !                                                                    trol parameter was set by the user.     !
   !                                                                                                            !
   !                tileSizeTS                  unsigned int(16 bit)  - Defines the max temporal size of a      !
   !                                                                    parameter tile.                         !
   !                                                                                                            !
   !                tileSizeTS                  unsigned int(16 bit)  - Defines the temporal size of a tile.    !
   !                                                                                                            !
   !                cbX, cbY, cbZ, cbTS         unsigned int(8 bit)   - Variables defining spatial and temporal !
   !                                                                    codeblock size in log2 exponent format. !
   !                                                                                                            !
   !                                                                                                            !
   !                decompX, decompY, decompZ   unsigned int(8 bit)   - Variables defining the number of spa-   !
   !                                                                    tial wavelet decompositions used during !
   !                                                                    compression.                            !
   !                                                                                                            !
   !                decompTS                    unsigned int(8 bit)   - Defines the number of temporal wavelet  !
   !                                                                    decompositions used during compression. !
   !                                                                                                            !
   !                guard_bits                  unsigned int(8 bit)   - Defines the number of guard bit used    !
   !                                                                    during quantization.                    !
   !                                                                                                            !
   !                number_of_decomp            unsigned int(8 bit)   - Defines the global number of wavelet    !
   !                                                                    decomposition levels.                   !
   !                                                                                                            !
   !                number_of_layers            unsigned int(8 bit)   - Defines the number of quality layers    !
   !                                                                    used during (de)compression.            !
   !                                                                                                            !
   !                Qm                          unsigned int(8 bit)   - Defines the Q number format range (m).  !
   !                                                                                                            !
   !                quantization_step_size      double                - Defines the global quantization step    !
   !                                                                    size.                                   !
   !                                                                                                            !
   !                bitrate                     float*                - Defines the average number of bits per  !
   !                                                                    datapoint after compression.            !
   !                                                                                                            !
   !                error_resilience            char                  - Specifies if an error resilient comp.   !
   !                                                                    approach is employed.                   !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                KernelX, KernelY, KernelZ   bwc_dwt_filter        - Specifies the wavelet kernels used for  !
   !                                                                    spatial decomposition.                  !
   !                                                                                                            !
   !                KernelT                     bwc_dwt_filter        - Specifies the wavelet kernel used for   !
   !                                                                    temporal decomposition.                 !
   !                                                                                                            !
   !                header                      bwc_header*           - Defines the main header stream.         !
   !                                                                                                            !
   !                progression                 bwc_prog_ord          - Specifies which of the five progression !
   !                                                                    orders is employed (CPRL, LRCP, PCRL,   !
   !                                                                    RLCP, RPCL).                            !
   !                                                                                                            !
   !                quantization_style          bwc_quant_st          - Specifies which of the two quantization !
   !                                                                    styles is employed (none, derived).     !
   !                                                                                                            !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                10.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      uint64                tileSizeX, tileSizeY, tileSizeZ;
      uint32                nTiles;
      uint32                qt_exponent;
      uint32                qt_mantissa;
      uint16                CSsgc;
      uint16                tileSizeTS;
      uint8                 cbX, cbY, cbZ, cbTS;
      uint8                 decompX, decompY, decompZ, decompTS;
      uint8                 guard_bits;
      uint8                 nDecomp;
      uint8                 nLayers, useLayer;
      uint8                 precSizeX, precSizeY, precSizeZ, precSizeTS;
      uint8                 Qm;
      float                *bitrate;
      uchar                 error_resilience;
      bwc_dwt_filter        KernelX, KernelY, KernelZ;
      bwc_dwt_filter        KernelTS;
      bwc_packed_stream     header;
      bwc_prog_ord          progression;
      bwc_quant_st          quantization_style;
   } bwc_gl_ctrl;

   /*----------------------------------------------------------------------------------------------------------*\
   !   STRUCT NAME: bwc_field                                                                                   !
   !   -----------                                                                                              !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This structure holds all the necessary parameters used to define and control the bwc codec  !
   !                to (de)compress a floating point array.                                                     !
   !                                                                                                            !
   !   PARAMETERS:                                                                                              !
   !   -----------                                                                                              !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                codec                       char                  - Determines wether current field struc-  !
   !                                                                    is defined for a compression or decom-  !
   !                                                                    pression stage.                         !
   !                                                                                                            !
   !   DEPENDENCIES:                                                                                            !
   !   -------------                                                                                            !
   !                Variable                    Type                    Description                             !
   !                --------                    ----                    -----------                             !
   !                info                        bwc_gl_inf*           - Structure holding all the global infor- !
   !                                                                    mation of the uncompressed data set.    !
   !                                                                                                            !
   !                control                     bwc_gl_ctrl*          - Structure holding all the global con-   !
   !                                                                    trol parameters for the bwc codec.      !
   !                                                                                                            !
   !                tile                        bwc_tile*             - Structure holding defining a bwc tile   !
   !                                                                                                            !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                10.10.2017  Patrick Vogler     B87D120     V 0.1.0     struct created                       !
   !                05.12.2019  Patrick Vogler     B87E7E4     V 0.1.0     Removal of unecessary parameters.    !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   typedef struct
   {
      bwc_gl_inf          *info;
      bwc_gl_ctrl          control;
      bwc_tile            *tile;

      struct meter
      {
         double            bpd;
         double            cpr;
         double            css;

         struct time
         {
            double         ttl;

            double         cpy;
            double         nrm;

            double         wav;
            double         ent;
            double         ass;
         }  time;
      }  meter;
   } bwc_field;
#endif