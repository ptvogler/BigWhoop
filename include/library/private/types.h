/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
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
\*  --------------------------------------------------------------------------------------------  */
/**                                                                                               
 *        @file types.h
 *
 *        This header defines a set of derrived types that are used to describe and
 *        instruct the bwc (de-)compression library.
 *                                                                                                */
/*  --------------------------------------------------------------------------------------------  *\
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
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#ifndef BWC_TYPES_H
#define BWC_TYPES_H
  /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
  ||                               _ _  _ ____ _    _  _ ___  ____                                ||
  ||                               | |\ | |    |    |  | |  \ |___                                ||
  ||                               | | \| |___ |___ |__| |__/ |___                                ||
  ||                                                                                              ||
  \*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
  #include <stdio.h>                                      //!< Standard I/O

  #include "constants.h"                                  //!< BWC constants
  #include "mq_types.h"                                   //!< BWC MQ coder types

  #ifdef BWC_SINGLE_PRECISION
    #include "prim_types_single.h"                        //!< BWC single precision prim types
  #else
    #include "prim_types_double.h"                        //!< BWC double precision prim types
  #endif

  /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
  ||               ___  ____ ____ ____ _ _  _ ____ ___     ___ _   _ ___  ____ ____               ||
  ||               |  \ |___ |__/ |__/ | |  | |___ |  \     |   \_/  |__] |___ [__                ||
  ||               |__/ |___ |  \ |  \ |  \/  |___ |__/     |    |   |    |___ ___]               ||
  ||                                                                                              ||
  \*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
  /*==============================================================================================*/
  /**
   * @details Encapsulates a numerical floating-point value of type @c bwc_float and provides 
   *          direct access to its underlying bit representation for low level operations.
   */
  /*=====================================================|========================================*/
  typedef union
  {
    bwc_raw                     raw;                      //!< Raw data sample access
    bwc_float                   f;                        //!< IEEE 754 float
  } bwc_sample;

  /*==============================================================================================*/
  /**
   * @details Stores the metadata and body of a packed stream. Tile and and paramter index can be 
   *          supplied to uniquely identify the stream.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      size;                     //!< Size in bytes
    uint64                      position;                 //!< Current read/write offset

    uchar                      *access;                   //!< Pointer used to parsing
    uchar                      *memory;                   //!< Memory handle
  } bwc_span;

  /*==============================================================================================*/
  /**
   * @details Represents a single node in a tagtree data structure, storing the node's tag value, 
   *          threshold, and parent reference. Used in progressive encoding schemes where nodes
   *          are traversed hierarchically.
   */
  /*=====================================================|========================================*/
  typedef struct node
  {
    uint64                      index;                    //!< Node index/tagtree position

    uint16                      value;                    //!< Node value (tag)
    uint16                      threshold;                //!< Threshold for tag comparison

    struct node*                parent;                   //!< Pointer to parent (NULL for root)
  } bwc_tagtree_node;

  /*==============================================================================================*/
  /**
   * @details This structure manages a tagtree with spatial (X, Y) and temporal (Z, T) dimensions. 
   *          It encodes codeblock tags—such as the number of significant bitplanes—efficiently 
   *          for storage in the compressed codestream.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      leavesX, leavesY;         //!< Number of spatial tagtree leaves
    uint64                      leavesZ, leavesTS;        //!< Number of temporal tagtree leaves

    bwc_tagtree_node           *nodes;                    //!< Pointer to tagtree nodes array
  } bwc_tagtree;

  /*==============================================================================================*/
  /**
   * @details This structure holds essential configuration and dimension information for an 
   *          uncompressed dataset, including spatial (X, Y, Z) and temporal (TS) extents, the
   *          number of parameters, and precision settings for both the raw data and the codec.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      nX, nY, nZ;               //!< Spatial size
    uint64                      nTS;                      //!< Temporal size (steps/slices)

    uint8                       nPar;                     //!< Number of parameters

    bwc_precision               data_prec;                //!< Precision of uncompressed data
    bwc_precision               codec_prec;               //!< Precision of encoder/decoder
  } bwc_gl_inf;

  /*==============================================================================================*/
  /**
   * @details This structure acts as the primary interface for passing data to and from the
   *          compressor/decompressor. It holds pointers to auxiliary and comment blocks,
   *          as well as user-managed buffers for input and output streams.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    struct codestream
    {
      bwc_span                 *aux;                    //!< Auxiliary information block.
      bwc_span                 *com;                    //!< Comment block.
    }codestream;                                        //!< Nested structure for codestream blocks

    void                       *inp;                    //!< User managed buffer for input (source)
    void                       *out;                    //!< User managed buffer for output (dest)

    bwc_mode                    mode;                   //!< Mode: compression or decompression
  } bwc_stream;

  /*==============================================================================================*/
  /**
   * @details Stores all necessary data for a compressed codeblock, including the compressed 
   *          bitstream and the rate-distortion (RD) slope information required for post-
   *          compression rate control. The coding pass lengths (@c L) and distortion 
   *          increments (@c S) define the convex hull of the RD curve, enabling optimal
   *          bit allocation across the codestream.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      L[MAXIMUM_NO_PASSES];     //!< Coding pass lengths in bits
    uint16                      S[MAXIMUM_NO_PASSES + 1]; //!< Distortion reduction per pass

    uint8                       Kmsbs;                    //!< N.o. insignificant leading bitplanes
    uint8                       K;                        //!< Index of first significant bitplane

    uint8                       Z;                        //!< Number of coding passes generated

    uchar                      *data;                     //!< Pointer to compressed bitstream
  } bwc_encoded_cblk;

  /*==============================================================================================*/
  /**
   * @details Holds all necessary state and configuration parameters to instruct the @c bwc codec
   *          on how to (de)compress a floating-point array for a specific precinct. It maintains
   *          unique state variables used during length encoding for both quality layer estimation
   *          and packet assembly.
   *
   *          The @c cp_contr pointer provides access to the memory chunk storing coding pass
   *          contributions for the current quality layer. The parameter @c K tracks the most
   *          significant bitplane (MSB) present in the associated codeblock.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint16                      beta;                     //!< State var for rate-distortion opt.
    uint16                      beta_est;                 //!< State var for quality layer est.

    uint16                      K;                        //!< MSB index in codeblock

    int16                      *memory;                   //!< Base ptr for coding pass data
    int16                      *cp_contr;                 //!< Access ptr for pass contribution
  } bwc_cblk_ctrl;

  /*==============================================================================================*/
  /**
   * @details This structure holds spatial (X, Y, Z) and temporal (TS) extents for a codeblock,
   *          as well as its index within the parent precinct.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint32                      idx;                      //!< Codeblock index within precinct

    uint64                      X0, Y0, Z0, TS0;          //!< Start coordinate
    uint64                      X1, Y1, Z1, TS1;          //!< End coordinate
  } bwc_cblk_inf;

  /*==============================================================================================*/
  /**
   * @details This structure aggregates the dimensional information, control parameters,
   *          and encoded data for all codeblocks in a precinct and serves as their
   *          primary processing unit.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    bwc_cblk_inf                info;                     //!< Dimensional info for the codeblock
    bwc_cblk_ctrl               control;                  //!< Control params for (de)compression
    bwc_encoded_cblk           *encoded_block;            //!< Pointer to encoded data structure
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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      number_of_codeblocks;     //!< Number of codeblocks w.r.t res. level.

    uint16                      numCbX, numCbY;           //!< Number of spaatial codeblocks.
    uint16                      numCbZ, numCbTS;          //!< Number of temporal codeblocks.

    bwc_tagtree                *tag_inclusion;            //!< Quality layer contribution tagtree.
    bwc_tagtree                *tag_msbs;                 //!< Most significant bit-planes tagtree.
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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint32                      X0, Y0, Z0, TS0;          //!< Precinct starting point.
    uint32                      X1, Y1, Z1, TS1;          //!< Precinct end point.
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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    bwc_prec_inf                info;                     //!< Precinct info structure.
    bwc_prec_ctrl               control;                  //!< Precinct control structure.
    bwc_codeblock              *codeblock;                //!< Precinct specific codeblocks.
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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uchar                       highband_flag;            //!< Current wavelet highband type

    uint16                      Kmax;                     //!< Dynamic ranger after transformation.

    uint16                      qt_mantissa;              //!< Quantization step size mantissa.
    uint8                       qt_exponent;              //!< Quantization step size exponent.

    bwc_float                   qt_effective_step_size;   //!< Effective quantization step size.
    bwc_float                   qt_step_size;             //!< Quantization step size.
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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      X0, Y0, Z0, TS0;          //!< Subband starting point.
    uint64                      X1, Y1, Z1, TS1;          //!< Subband end point.

    bwc_float                   dwt_gain;                 //!< Subband energy gain factor.
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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    bwc_subb_inf                info;                     //!< Subband info structure.
    bwc_subb_ctrl               control;                  //!< Subband control structure.

    bwc_precinct               *precinct;                 //!< Subband specific precincts.
  } bwc_subband;

  /*----------------------------------------------------------------------------------------------*\
  !                                                                                                !
  !   DESCRIPTION:                                                                                 !
  !   ------------                                                                                 !
  !                                                                                                !
  !         This structure holds all the parameters used to assemble a codestream packet.          !
  !                                                                                                !
  \*----------------------------------------------------------------------------------------------*/
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    bwc_span                    header;                   //!< Packed stream header.
    bwc_span                    body;                     //!< Packed stream body.
    uint8                       e;                        //!< Indicator for packet cb. contributions.

    uint32                      size;                     //!< Codestream packet size.

    uint32                      p;                        //!< Position of curr. packet in codestream.
    uint8                       c, l, r;                  //!< Parameter, Quality Layer and Resolution
                                                          //!< codestream position markers.
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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint16                      numPX, numPY;             //!< Spatial number of precincts.
    uint16                      numPZ, numPTS;            //!< Temporal number of precincts.

    uint8                       rcbX,  rcbY;              //!< Real spatial codeblock size.
    uint8                       rcbZ,  rcbTS;             //!< Real temporal codeblock size.

    uint64                      number_of_precincts;      //!< N.o. precincts in resolution level.
    uint8                       number_of_subbands;       //!< N.o. subbands in resolution level.
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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      X0, Y0, Z0, TS0;          //!< Resolution level starting point.
    uint64                      X1, Y1, Z1, TS1;          //!< Resolution level end point.
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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    bwc_res_inf                 info;                     //!< Resolution level info structure.
    bwc_res_ctrl                control;                  //!< Resolution level control structure.

    bwc_subband                *subband;                  //!< Structure defining a bwc subband.
    bwc_packet                 *packet;                   //!< Structure defining a bwc packet.
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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    bwc_codeblock              *codeblock;                //!< Access to all tile param. codeblocks.
    bwc_precinct               *precinct;                 //!< Access to all tile param. precincts.
    bwc_subband                *subband;                  //!< Access to all tile param. subbands.
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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      number_of_codeblocks;     //!< Number of codeblocks in the tile param.
    bwc_float                   alpha, beta;              //!< Parameters used for normalization.
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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      X0, Y0, Z0, TS0;          //!< Tile parameter starting point.
    uint64                      X1, Y1, Z1, TS1;          //!< Tile parameter end point.

    bwc_float                   parameter_min;            //!< Min. value of tile parameter.
    bwc_float                   parameter_max;            //!< Max. value of tile parameter.
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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      size;                     //!< Size of the bwc_float data structure.
    bwc_sample                  *data;                    //!< Tile parameter values.

    bwc_param_inf               info;                     //!< Parameter info structure.
    bwc_param_ctrl              control;                  //!< Parameter control structure.

    bwc_resolution             *resolution;               //!< Tile parameter specific resol. levels.

    bwc_cblk_access            *access;                   //!< Access to all tile param. codeblocks.
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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      body_size;                //!< Size of the tile parameter body.
    uint64                      Ltp;                      //!< Tile part size.
    uint32                      header_size;              //!< Approx. size of the tile param. header.

    uint64                      nPackets;                 //!< Number of packets assembled for tile.
    uint32                      max_Prec;                 //!< Max. N.o. precincts in all tile params.

    uint16                      dflt_param_c;             //!< Default idx when writing COD in header.
    uint16                      dflt_param_q;             //!< Default idx when writing QCD in header.

    uint16                      slope_min, slope_max;     //!< Min/Max for the convex hull slp.
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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      size;                     //!< Tile size.
    uint32                      tile_index;               //!< Unique tile index.

    uint64                      X0, Y0, Z0, TS0;          //!< Tile starting point.
    uint64                      X1, Y1, Z1, TS1;          //!< Tile end point.

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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    bwc_tile_inf                info;                     //!< Tile info structure.
    bwc_tile_ctrl               control;                  //!< Tile control structure.

    bwc_parameter              *parameter;                //!< Tile specific parameter structure.
    bwc_packet                **packet_sequence;          //!< Tile specific packet sequence.
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
  /*==============================================================================================*/
  /**
   * @details 
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint16                      CSsgc;                    //!< Flag signaling user control variable.
    uchar                       error_resilience;         //!< Flag signalling error resilience.

    uint64                      tileSizeX,  tileSizeY;    //!< Spatial tile size.
    uint64                      tileSizeZ,  tileSizeTS;   //!< Temporal tile size.

    uint64                      nTiles;                   //!< Global number of tiles.

    uint8                       precSizeX,  precSizeY;    //!< Spatial precinct size.
    uint8                       precSizeZ,  precSizeTS;   //!< Temporal precinct size.

    uint8                       cbX,        cbY;          //!< Spatial codeblock size.
    uint8                       cbZ,        cbTS;         //!< Temporal codeblock size.

    bwc_dwt_filter              KernelX,    KernelY;      //!< Spatial wavelet kernels.
    bwc_dwt_filter              KernelZ,    KernelTS;     //!< Temporal wavelet kernels.

    uint8                       decompX,    decompY;      //!< Number of wavelet decompositions ...
    uint8                       decompZ,    decompTS;     //!< ... for each of the four dimensions.
    uint8                       nDecomp;                  //!< Maximum No. wavelet decompositions.

    uint32                      qt_exponent, qt_mantissa; //!< Global qunatization exponent/mantissa.
    uint8                       Qm;                       //!< Q number format range (m).

    float                      *bitrate;                  //!< Quality layers defined by bitrate.

    uint8                       nLayers;                  //!< Number of quality layers.
    uint8                       useLayer;                 //!< Quality layer used for decompression.

    uint8                       guard_bits;               //!< Number of guard bits during quant.

    uint64                      headerSize;               //!< Size estimation of the global header.
    uint64                      codestreamSize;           //!< Size of entire code-stream.

    bwc_quant_st                quantization_style;       //!< Quantization style.
    bwc_prog_ord                progression;              //!< Packet progression order.
  } bwc_gl_ctrl;

/*================================================================================================*/
/**
 * @details Structure that is used to probe header information from a compressed data set.
 */
/*================================================================================================*/
  typedef struct
  {
    bwc_gl_inf                  info;                     //!< Global info structure
    bwc_gl_ctrl                 control;                  //!< Global control structure
    bwc_span                    aux;                      //!< Auxiliary info. codestream block.
    bwc_span                    com;                      //!< Comment codestream block.
  } bwc_header;

/*================================================================================================*/
/**
 * @details Structure holding all the necessary parameters defining and controlling a bwc
 *          (de-)compression run.
 */
/*================================================================================================*/
  typedef struct
  {
    bwc_gl_inf                  info;                     //!< Global info structure
    bwc_gl_ctrl                 control;                  //!< Global control structure

    bwc_tile                   *tile;                     //!< Structure defining bwc tile.

    bwc_mode                    mode;                     //!< Flag to signal (de-)compression
  } bwc_codec;
#endif