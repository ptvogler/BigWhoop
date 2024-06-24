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
||        This header defines a set of derrived types that are used to describe and               ||
||        instruct the bwc (de-)compression library.                                              ||
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
#ifndef BWC_TYPES_H
#define BWC_TYPES_H

  #include <stdio.h>
  /************************************************************************************************\
  ||                               _ _  _ ____ _    _  _ ___  ____                                ||
  ||                               | |\ | |    |    |  | |  \ |___                                ||
  ||                               | | \| |___ |___ |__| |__/ |___                                ||
  ||                                                                                              ||
  \************************************************************************************************/
  #ifdef BWC_SINGLE_PRECISION
    #include "prim_types_single.h"
  #else
    #include "prim_types_double.h"
  #endif

  #include "constants.h"
  #include "mq_types.h"

  /************************************************************************************************\
  ||               ___  ____ ____ ____ _ _  _ ____ ___     ___ _   _ ___  ____ ____               ||
  ||               |  \ |___ |__/ |__/ | |  | |___ |  \     |   \_/  |__] |___ [__                ||
  ||               |__/ |___ |  \ |  \ |  \/  |___ |__/     |    |   |    |___ ___]               ||
  ||                                                                                              ||
  \************************************************************************************************/
  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This union describes a simple data type used to store a single sample of a             !
  !         numerical dataset. The union allows access to the raw bit representation of the        !
  !         data sample.                                                                           !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef union
  {
    bwc_raw                     raw;                      // Raw data sample access.
    bwc_float                   f;                        // IEEE 754 representation of sample.
  } bwc_sample;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure is used to store the metadata and boy of a packed stream. Tile and      !
  !         and paramter index can be supplied to uniquely identify the stream.                    !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint64                      size;                     // Size of packed stream.
    uint64                      position;                 // Reading/Writing pos. in the stream.

    uchar                      *access;                   // Pointer used to parse packed stream.
    uchar                      *memory;                   // Memory handle for the packed stream.
  } bwc_stream;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure defines a single tagtree node which stores the node value (tag),        !
  !         node threshold as well as the address of the parent node.                              !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct node
  {
    uint64                      index;                    // Index of current node.

    uint16                      value;                    // Tagtree node value.
    uint16                      threshold;                // Tagtree node threshold.

    struct node*                parent;                   // Parent of current node.
  } bwc_tagtree_node;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure defines a tagtree instance with spatial and temporal leaves that        !
  !         can be used to encode codeblock tags - e.g. number of significant bitplanes - to       !
  !         efficiently store them in the compressed codestream.                                   !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint64                      leavesX, leavesY;         // Number of spatial tagtree leafs.
    uint64                      leavesZ, leavesTS;        // Number of temporal tagtree leafs.

    bwc_tagtree_node           *nodes;                    // Pointer to the tagtree nodes.
  } bwc_tagtree;


  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure defines a linked list which stores the size, index, and bit pre-        !
  !         cision  The string name is used to store the parameter name supplied by the            !
  !         function caller.                                                                       !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct opt
  {
    char                        name[24];                 // Parameter name.
    uint8                       id;                       // Parameter index.

    uint64                      size;                     // Parameter size after sub-sampling.
    uint8                       precision;                // Parameter precision.

    struct opt                 *next;                     // Next element in linked-list.
    struct opt                 *root;                     // Linked-list root.
  } bwc_cmd_opts_ll;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the global information of an uncompressed data set            !
  !         including its spatial and temporal dimensions, number of parameters, file              !
  !         extension and parameter information as well as the codec precision.                    !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint64                      nX, nY, nZ;               // Spatial size of uncompressed data-set.
    uint64                      nTS;                      // Temp. size of uncompressed data-set.

    uint8                       nPar;                     // Number of parameters.

    uint8                       precision;                // Flag defining codec precision.

    uint8                       codec_prec;               // Encoder/decoder bit precision.
    char                        f_ext[20];                // Uncompressed data set file extension.

    bwc_cmd_opts_ll            *parameter;                // Command options linked-list.
  } bwc_gl_inf;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure stores the numerical dataset/compressed bitstream passed to or          !
  !         returned from the (de)compressor.                                                      !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    bwc_gl_inf                  info;                     // Gloabal info structure.
    FILE                       *fp;                       // File point to (un)compr. data-set.

    struct codestream
    {
      bwc_stream               *data;                     // Data codestream block.
      bwc_stream               *aux;                      // Auxiliary info. codestream block.
      bwc_stream               *com;                      // Comment codestream block.
    }codestream;

    struct field
    {
      double                   *d;                        // Double precision numerical data-set.
      float                    *f;                        // Single precision numerical data-set.
    }field;
  } bwc_data;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure defines and stores all necessary data for a compressed codeblock.       !
  !         The coding pass lengths and distortion rate arrays define the convex hull used         !
  !         for during rate controlling.                                                           !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint64                      L[MAXIMUM_NO_PASSES];     // Coding pass lengths of an ecnoded blck.
    uint16                      S[MAXIMUM_NO_PASSES + 1]; // Distortion rate of an encoded block.

    uint8                       Kmsbs;                    // N.o. insignificant leading bitplanes.
    uint8                       K;                        // First significant bitplane.

    uint8                       Z;                        // Number of coding passes generated.

    uchar                      *data;                     // Memory handle to compressed bitstream.
  } bwc_encoded_cblk;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the control parameters for the current precinct used to       !
  !         instruct the bwc codec to (de)compress a floating point array. The unique state        !
  !         variables are used during length encoding both for quality layer estimation as         !
  !         well as packet assembly. The cp_contr access pointer allows access to the memory       !
  !         chunck employed to store the coding passes contributing to a specific quality          !
  !         layer. The parameter K represents the most significant bitplane present in the         !
  !         current codeblock.                                                                     !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint16                      beta;                     // Codeblock unique state variable.
    uint16                      beta_est;                 // State variable used for ql estimation.

    uint16                      K;                        // Codeblock spec. significant bitplane.

    int16                      *memory;                   // Coding pass contribution to q. layer.
    int16                      *cp_contr;                 // Coding pass contribution access ptr.
  } bwc_cblk_ctrl;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure is used to store the start and end points as well as the index          !
  !         for a specific codeblock.                                                              !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint32                      idx;                      // Codeblock index w.r.t. precinct.

    uint64                      X0, Y0, Z0, TS0;          // Codeblock starting point.
    uint64                      X1, Y1, Z1, TS1;          // Codeblock end point.
  } bwc_cblk_inf;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the necessary parameters used to define and control a         !
  !         bwc codeblock.                                                                         !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    bwc_cblk_inf                info;                     // Codeblock info structure.
    bwc_cblk_ctrl               control;                  // Codeblock control structure.
    bwc_encoded_cblk            *encoded_block;           // Encoded block structure.
  } bwc_codeblock;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the control parameters for the current precinct used to       !
  !         instruct the bwc codec to (de)compress a floating point array. The number              !
  !         codeblocks are stored for all spatial and temporal dimensions as well as for the       !
  !         entire precinct. The two tagtrees are employed to record specific properties of        !
  !         the underlying data that is to be compressed.                                          !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint64                      number_of_codeblocks;     // Number of codeblocks w.r.t res. level.

    uint16                      numCbX, numCbY;           // Number of spaatial codeblocks.
    uint16                      numCbZ, numCbTS;          // Number of temporal codeblocks.

    bwc_tagtree                *tag_inclusion;            // Quality layer contribution tagtree.
    bwc_tagtree                *tag_msbs;                 // Most significant bit-planes tagtree.
  } bwc_prec_ctrl;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !         DESCRIPTION NEEDED                                                                     !
  !         This structure is used to store the start and end points for the current               !
  !         precinct.                                                                              !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint32                      X0, Y0, Z0, TS0;          // Precinct starting point.
    uint32                      X1, Y1, Z1, TS1;          // Precinct end point.
  } bwc_prec_inf;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the necessary parameters used to define and control a         !
  !         bwc precinct.                                                                          !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    bwc_prec_inf                info;                     // Precinct info structure.
    bwc_prec_ctrl               control;                  // Precinct control structure.
    bwc_codeblock              *codeblock;                // Precinct specific codeblocks.
  } bwc_precinct;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the control parameters for the current subband used to        !
  !         instruct the bwc codec to (de)compress a floating point array. The highband flag       !
  !         is used to identify the type of highband a specific wavelet band belongs to            !
  !         (e.g. HHHH). The quantization step size mantissa and exponent for a subband is         !
  !         stored alongside its actual value as well as the effective step size that is           !
  !         applied to the wavelet coefficients.                                                   !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uchar                       highband_flag;            // Current wavelet highband type

    uint16                      Kmax;                     // Dynamic ranger after transformation.

    uint16                      qt_mantissa;              // Quantization step size mantissa.
    uint8                       qt_exponent;              // Quantization step size exponent.

    bwc_float                   qt_effective_step_size;   // Effective quantization step size.
    bwc_float                   qt_step_size;             // Quantization step size.
  } bwc_subb_ctrl;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure is used to store the start and end points as well as the energy         !
  !         gain factor of the wavelet decomposition that has been applied to the subband          !
  !         data samples during transformation.                                                    !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint64                      X0, Y0, Z0, TS0;          // Subband starting point.
    uint64                      X1, Y1, Z1, TS1;          // Subband end point.

    bwc_float                   dwt_gain;                 // Subband energy gain factor.
  } bwc_subb_inf;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the necessary parameters used to define and control a         !
  !         bwc subband.                                                                           !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    bwc_subb_inf                info;                     // Subband info structure.
    bwc_subb_ctrl               control;                  // Subband control structure.

    bwc_precinct               *precinct;                 // Subband specific precincts.
  } bwc_subband;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the parameters used to assemble a codestream packet.          !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    bwc_stream                  header;                   // Packed stream header.
    bwc_stream                  body;                     // Packed stream body.
    uint8                       e;                        // Indicator for packet cb. contributions.

    uint32                      size;                     // Codestream packet size.

    uint32                      p;                        // Position of curr. packet in codestream.
    uint8                       c, l, r;                  // Parameter, Quality Layer and Resolution
                                                          // codestream position markers.
  } bwc_packet;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the control parameters for the current resolution level       !
  !         used to instruct the bwc codec to (de)compress a floating point array.                 !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint16                      numPX, numPY;             // Spatial number of precincts.
    uint16                      numPZ, numPTS;            // Temporal number of precincts.

    uint8                       rcbX,  rcbY;              // Real spatial codeblock size.
    uint8                       rcbZ,  rcbTS;             // Real temporal codeblock size.

    uint64                      number_of_precincts;      // N.o. precincts in resolution level.
    uint8                       number_of_subbands;       // N.o. subbands in resolution level.
  } bwc_res_ctrl;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure is used to store the name, start and end points of the current          !
  !         resolution level.                                                                      !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint64                      X0, Y0, Z0, TS0;          // Resolution level starting point.
    uint64                      X1, Y1, Z1, TS1;          // Resolution level end point.
  } bwc_res_inf;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the necessary parameters used to define and control a         !
  !         bwc resolution level.                                                                  !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    bwc_res_inf                 info;                     // Resolution level info structure.
    bwc_res_ctrl                control;                  // Resolution level control structure.

    bwc_subband                *subband;                  // Structure defining a bwc subband.
    bwc_packet                 *packet;                   // Structure defining a bwc packet.
  } bwc_resolution;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure is used to directly access the parameter codeblocks during              !
  !         entropy (de-)encoding to facilitate shared memory parallelization.                     !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    bwc_codeblock              *codeblock;                // Access to all tile param. codeblocks.
    bwc_precinct               *precinct;                 // Access to all tile param. precincts.
    bwc_subband                *subband;                  // Access to all tile param. subbands.
  } bwc_cblk_access;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the control parameters for the current tile param. used       !
  !         to instruct the bwc codec to (de)compress a floating point array.                      !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint64                      number_of_codeblocks;     // Number of codeblocks in the tile param.
    bwc_float                   alpha, beta;              // Parameters used for normalization.
  } bwc_param_ctrl;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure is used to store the name, start and end points as well as the          !
  !         overall size of a parameter tile. Furthermore, the minimum and maximum sample          !
  !         values for the current parameter tile are stored.                                      !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint64                      size;                     // Parameter size.
    char                       *name;                     // Parameter name.

    uint64                      X0, Y0, Z0, TS0;          // Tile parameter starting point.
    uint64                      X1, Y1, Z1, TS1;          // Tile parameter end point.

    uint8                       precision;                // Tile parameter precision.

    bwc_float                   parameter_min;            // Min. value of tile parameter.
    bwc_float                   parameter_max;            // Max. value of tile parameter.
  } bwc_param_inf;


  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the necessary parameters used to define and control the       !
  !         (de)compression of a bwc tile parameter.                                               !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint64                      size;                     // Size of the bwc_float data structure.
    bwc_sample                  *data;                    // Tile parameter values.

    bwc_param_inf               info;                     // Parameter info structure.
    bwc_param_ctrl              control;                  // Parameter control structure.

    bwc_resolution             *resolution;               // Tile parameter specific resol. levels.

    bwc_cblk_access            *access;                   // Access to all tile param. codeblocks.
  } bwc_parameter;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the control parameters for the current tile used to in-       !
  !         struct the bwc codec to (de)compress a floating point array.                           !
  !         The slope_min and slope_max values represent the minimum and maximum distortion        !
  !         rates for the compressed data-samples of a specific tile. These values define          !
  !         the distortion-bitrate convex hull used for rate control.                              !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint64                      body_size;                // Size of the tile parameter body.
    uint64                      Ltp;                      // Tile part size.
    uint32                      header_size;              // Approx. size of the tile param. header.

    uint64                      nPackets;                 // Number of packets assembled for tile.
    uint32                      max_Prec;                 // Max. N.o. precincts in all tile params.

    uint16                      dflt_param_c;             // Default idx when writing COD in header.
    uint16                      dflt_param_q;             // Default idx when writing QCD in header.

    uint16                      slope_min, slope_max;     // Min/Max for the convex hull slp.
  } bwc_tile_ctrl;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure is used to store the start and end points of the tile as well as        !
  !         its overall size and index.                                                            !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint64                      size;                     // Tile size.
    uint32                      tile_index;               // Unique tile index.

    uint64                      X0, Y0, Z0, TS0;          // Tile starting point.
    uint64                      X1, Y1, Z1, TS1;          // Tile end point.

  } bwc_tile_inf;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the necessary parameters used to define and control the       !
  !         (de)compression of a bwc tile. The packet_sequence pointer array is used to            !
  !         store the bitstream packet memory hanldes of the underlying tile in a prede-           !
  !         fined sequence according to the bwc_progression_ord parameter. The pointer array       !
  !         is employed during codestream assembly.                                                !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    bwc_tile_inf                info;                     // Tile info structure.
    bwc_tile_ctrl               control;                  // Tile control structure.

    bwc_parameter              *parameter;                // Tile specific parameter structure.
    bwc_packet                **packet_sequence;          // Tile specific packet sequence.
  } bwc_tile;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the global control parameters, including the tile,            !
  !         precinct and codeblock sizes as well as the global number of tiles, used to            !
  !         instruct the bwc codec to (de)compress a floating point array.                         !
  !         The number of decompositions (numDecompX etc.) define the number of times the          !
  !         selected wavelet filter (kernelX etc.) is applied alongside each spatial and           !
  !         temporal direction. The global number of decomposition levels is stored in the         !
  !         appropriate variable.                                                                  !
  !         The parameters qt_mantissa and qt_exponent define the mantissa and exponent of         !
  !         the global quantization step size appropriate for the specified number of              !
  !         wavelet decomposition levels. The quantization_style signals if the quantization       !
  !         step size for the subsequent subbands should be derrived from the global               !
  !         parameter, or not. The variable Qm represents the dynamic range of the                 !
  !         float-to-fixed point transformation employed pre-compression.                          !
  !         The bitrate array stores up to 10 bitrate values - average bits per datasample -       !
  !         that define the codestream quality layers. The largest bitrates defines the            !
  !         compression ratio, while the total number of defined bitrates represent the            !
  !         number of (quality)layers. The parameter use_layer stores the index of the             !
  !         quality layer that is used during decompression. The progression order defines         !
  !         the packet sequence as they will appear in the compressed codestream.                  !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    uint16                      CSsgc;                    // Flag signaling user control variable.
    uchar                       error_resilience;         // Flag signalling error resilience.

    uint64                      tileSizeX,  tileSizeY;    // Spatial tile size.
    uint64                      tileSizeZ,  tileSizeTS;   // Temporal tile size.

    uint64                      nTiles;                   // Global number of tiles.

    uint8                       precSizeX,  precSizeY;    // Spatial precinct size.
    uint8                       precSizeZ,  precSizeTS;   // Temporal precinct size.

    uint8                       cbX,        cbY;          // Spatial codeblock size.
    uint8                       cbZ,        cbTS;         // Temporal codeblock size.

    bwc_dwt_filter              KernelX,    KernelY;      // Spatial wavelet kernels.
    bwc_dwt_filter              KernelZ,    KernelTS;     // Temporal wavelet kernels.

    uint8                       decompX,    decompY;      // Number of wavelet decompositions ...
    uint8                       decompZ,    decompTS;     // ... for each of the four dimensions.
    uint8                       nDecomp;                  // Maximum No. wavelet decompositions.

    uint32                      qt_exponent, qt_mantissa; // Global qunatization exponent/mantissa.
    uint8                       Qm;                       // Q number format range (m).

    float                      *bitrate;                  // Quality layers defined by bitrate.

    uint8                       nLayers;                  // Number of quality layers.
    uint8                       useLayer;                 // Quality layer used for decompression.

    uint8                       guard_bits;               // Number of guard bits during quant.

    bwc_stream                  header;                   // Main codestream header.

    bwc_quant_st                quantization_style;       // Quantization style.
    bwc_prog_ord                progression;              // Packet progression order.
  } bwc_gl_ctrl;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the necessary parameters defining and controling a bwc        !
  !         (de-)compression run.                                                                  !
  !                                                                                                !
  !         The meter structure is used to store measurements, including important time            !
  !         measurements, for a particular compression run.                                        !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  typedef struct
  {
    bwc_gl_inf                 *info;                     // Gloabal info structure
    bwc_gl_ctrl                 control;                  // Global control structure

    bwc_tile                   *tile;                     // Structure defining bwc tile.
  } bwc_field;
#endif