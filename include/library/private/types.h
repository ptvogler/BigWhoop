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
   *          on how to (de)compress a floating-point array for a generic codeblock. It maintains
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

  /*==============================================================================================*/
  /**
   * @details This structure holds state and configuration parameters to instruct the @c bwc codec
   *          on how to (de)compress a floating-point array for a precinct.It maintains the total
   *          number of codeblocks and counts per spatial/temporal direction.
   * 
   *          The @c tag_inclusion and @c tag_msbs pointers provide access to the quality layer
   *          inclusion and MSB tag-tree structures respectively.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      number_of_codeblocks;     //!< Number of total codeblocks

    uint16                      numCbX, numCbY;           //!< Number of spaatial codeblocks
    uint16                      numCbZ, numCbTS;          //!< Number of temporal codeblocks

    bwc_tagtree                *tag_inclusion;            //!< Quality layer inclusion tagtree
    bwc_tagtree                *tag_msbs;                 //!< MSB tag-tree structure
  } bwc_prec_ctrl;

  /*==============================================================================================*/
  /**
   * @details This structure holds spatial (X, Y, Z) and temporal (TS) extents for a precinct.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint32                      X0, Y0, Z0, TS0;          //!< Start coordinate
    uint32                      X1, Y1, Z1, TS1;          //!< End coordinate
  } bwc_prec_inf;

  /*==============================================================================================*/
  /**
   * @details This structure aggregates the dimensional information, control parameters, and
   *          codeblock structures for all precincts in a resolution level and serves as their
   *          primary processing unit.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    bwc_prec_inf                info;                     //!< Dimensional info for the precinct
    bwc_prec_ctrl               control;                  //!< Control params for (de)compression
    bwc_codeblock              *codeblock;                //!< Pointer to underlying codeblocks
  } bwc_precinct;

  /*==============================================================================================*/
  /**
   * @details This structure holds state and configuration parameters to instruct the @c bwc codec
   *          on how to (de)compress a floating-point array for a subband.It maintains the subband
   *          type, dynamic range, and quantization parameters including mantissa, exponent,
   *          and effective step sizes.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uchar                       highband_flag;            //!< Subband type (high-/lowband)

    uint16                      Kmax;                     //!< Dynamic ranger after transformation

    uint16                      qt_mantissa;              //!< Quantization step mantissa
    uint8                       qt_exponent;              //!< Quantization step exponent

    bwc_float                   qt_effective_step_size;   //!< Effective quantization step size
    bwc_float                   qt_step_size;             //!< Raw quantization step size
  } bwc_subb_ctrl;


  /*==============================================================================================*/
  /**
   * @details This structure holds spatial (X, Y, Z) and temporal (TS) extents for a subband,
   *          along with the energy gain factor from the wavelet transformation.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      X0, Y0, Z0, TS0;          //!< Start coordinate
    uint64                      X1, Y1, Z1, TS1;          //!< End coordinate

    bwc_float                   dwt_gain;                 //!< Subband energy gain factor
  } bwc_subb_inf;


  /*==============================================================================================*/
  /**
   * @details This structure aggregates the dimensional information, control parameters, and
   *          precinct structures for all subbands in a resolution level and serves as their
   *          primary processing unit.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    bwc_subb_inf                info;                     //!< Dimensional info for the subband
    bwc_subb_ctrl               control;                  //!< Control params for (de)compression

    bwc_precinct               *precinct;                 //!< Pointer to underlying precincts
  } bwc_subband;

  /*==============================================================================================*/
  /**
   * @details Aggregates size info, control params, and header/body bitstreams to assemble 
   *          codestream packets.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    bwc_span                    header;                   //!< Packed stream header
    bwc_span                    body;                     //!< Packed stream body
    uint8                       e;                        //!< Codeblock contribution flag

    uint32                      size;                     //!< Total packet size in bytes

    uint32                      p;                        //!< Packet index in codestream
    uint8                       c, l, r;                  //!< Precinct, layer, resolution
                                                          //!< position markers
  } bwc_packet;


  /*==============================================================================================*/
  /**
   * @details Holds state and config parameters to instruct @c bwc codec on (de)compression of 
   *          floating-point arrays per resolution level. Maintains number of subbands and 
   *          precincts as well as the codeblock size information.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint16                      numPX, numPY, numPZ;      //!< Number of spatial precincts
    uint16                      numPTS;                   //!< Number of temporal precincts

    uint8                       rcbX,  rcbY, rcbZ;        //!< Actual spatial codeblock size
    uint8                       rcbTS;                    //!< Actual temporal codeblock size

    uint64                      number_of_precincts;      //!< Total precincts at this resolution
    uint8                       number_of_subbands;       //!< Total subbands at this resolution
  } bwc_res_ctrl;

  /*==============================================================================================*/
  /**
   * @details This structure holds spatial (X, Y, Z) and temporal (TS) extents for a resolution
   *          level.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      X0, Y0, Z0, TS0;          //!< Start coordinate
    uint64                      X1, Y1, Z1, TS1;          //!< End coordinate
  } bwc_res_inf;

  /*==============================================================================================*/
  /**
   * @details Aggregates the dimensional information, control parameters, and associated subbands 
   *          and packets for all resolution levels in a tile parameter and serves as their
   *          primary processing unit.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    bwc_res_inf                 info;                     //!< Dimensional info for resolution
    bwc_res_ctrl                control;                  //!< Control params for (de)compression

    bwc_subband                *subband;                  //!< Pointer to subband array
    bwc_packet                 *packet;                   //!< Pointer to packet array
  } bwc_resolution;

  /*==============================================================================================*/
  /**
   * @details Provides direct access to parameter codeblocks, as well as the associated subbands
   *          and precincts, during entropy (de-)encoding to facilitate shared memory
   *          parallelization.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    bwc_codeblock              *codeblock;                //!< Pointer to tile param codeblocks
    bwc_precinct               *precinct;                 //!< Pointer to tile param precincts
    bwc_subband                *subband;                  //!< Pointer to tile param subbands
  } bwc_cblk_access;

  /*==============================================================================================*/
  /**
   * @details Holds state and config parameters to instruct @c bwc codec on (de)compression of 
   *          floating-point arrays per tile parameter. Maintains number of codeblocks and 
   *          array normalization factors.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      number_of_codeblocks;     //!< Number of tile param codeblocks
    bwc_float                   alpha, beta;              //!< Parameters used for normalization
  } bwc_param_ctrl;

  /*==============================================================================================*/
  /**
   * @details Holds spatial (X, Y, Z) and temporal (TS) extents for a tile parameter, as well as 
   *          minimum and maximum value bounds.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      X0, Y0, Z0, TS0;          //!< Start coordinate
    uint64                      X1, Y1, Z1, TS1;          //!< End coordinate

    bwc_float                   parameter_min;            //!< Minimum tile parameter value
    bwc_float                   parameter_max;            //!< Maximum tile parameter value
  } bwc_param_inf;

  /*==============================================================================================*/
  /**
   * @details Aggregates dimension info, control params, resolution levels, and codeblock access
   *          for a tile parameter. Serves as the primary processing unit. Also maintains the
   *          parameter array and size.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      size;                     //!< Size of the data array
    bwc_sample                  *data;                    //!< Tile parameter values array

    bwc_param_inf               info;                     //!< Dimensional info for parameter
    bwc_param_ctrl              control;                  //!< Control params for (de)compression

    bwc_resolution             *resolution;               //!< Array of resolution levels

    bwc_cblk_access            *access;                   //!< Codeblock access pointers
  } bwc_parameter;

  /*==============================================================================================*/
  /**
   * @details Holds control parameters for the current tile to instruct the bwc codec on 
   *          (de)compression of floating-point arrays. slope_min and slope_max define the
   *          distortion-rate range for tile samples, establishing the convex hull for 
   *          rate control.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      body_size;                //!< Size of tile parameter body
    uint64                      Ltp;                      //!< Tile part size
    uint32                      header_size;              //!< Approx. tile param header size

    uint64                      nPackets;                 //!< Packets assembled for tile
    uint32                      max_Prec;                 //!< Max precincts across all tiles

    uint16                      dflt_param_c;             //!< Default COD index in header
    uint16                      dflt_param_q;             //!< Default QCD index in header

    uint16                      slope_min, slope_max;     //!< Convex hull slope bounds
  } bwc_tile_ctrl;

  /*==============================================================================================*/
  /**
   * @details Holds spatial (X, Y, Z) and temporal (TS) extents for a tile, as well as the tile
   *          tile index and size.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint64                      size;                     //!< Total tile size in bytes
    uint32                      tile_index;               //!< Unique tile index

    uint64                      X0, Y0, Z0, TS0;          //!< Start coordinate
    uint64                      X1, Y1, Z1, TS1;          //!< End coordinate

  } bwc_tile_inf;

  /*==============================================================================================*/
  /**
   * @details Aggregates dimension info, control params, tile parameters and packet sequence for
   *          for a tile. Serves as the primary processing unit.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    bwc_tile_inf                info;                     //!< Dimensional info for parameter
    bwc_tile_ctrl               control;                  //!< Control params for (de)compression

    bwc_parameter              *parameter;                //!< Pointer to tile parameters
    bwc_packet                **packet_sequence;          //!< Array of packet pointers
  } bwc_tile;

  /*==============================================================================================*/
  /**
   * @details Holds global control parameters for the bwc codec, including tile, precinct, and
   *          codeblock sizes, and the number of tiles. Defines wavelet decomposition levels,
   *          filter kernels, and quantization parameters (mantissa/exponent, style, Qm). 
   *          Manages quality layers via bitrate arrays and specifies the progression order
   *          for the compressed codestream.
   */
  /*=====================================================|========================================*/
  typedef struct
  {
    uint16                      CSsgc;                    //!< User control flag
    uchar                       error_resilience;         //!< Error resilience flag

    uint64                      tileSizeX,  tileSizeY;    //!< 1st & 2nd tile dimensions
    uint64                      tileSizeZ,  tileSizeTS;   //!< 3rd & 4th tile dimensions

    uint64                      nTiles;                   //!< Total number of tiles

    uint8                       precSizeX,  precSizeY;    //!< 1st & 2nd precinct dimensions
    uint8                       precSizeZ,  precSizeTS;   //!< 3rd & 4th precinct dimensions

    uint8                       cbX,        cbY;          //!< 1st & 2nd codeblock dimensions
    uint8                       cbZ,        cbTS;         //!< 3rd & 4th codeblock dimensions

    bwc_dwt_filter              KernelX,    KernelY;      //!< Wavelet kernels (1st & 2nd dim)
    bwc_dwt_filter              KernelZ,    KernelTS;     //!< Wavelet kernels (3rd & 4th dim)

    uint8                       decompX,    decompY;      //!< Wavelet levels (1st & 2nd dim)
    uint8                       decompZ,    decompTS;     //!< Wavelet levels (3rd & 4th dim)
    uint8                       nDecomp;                  //!< Max decomposition levels

    uint32                      qt_exponent, qt_mantissa; //!< Global quantization exp/mantissa
    uint8                       Qm;                       //!< Float-to-fixed dynamic range

    float                      *bitrate;                  //!< Bitrates for quality layers

    uint8                       nLayers;                  //!< Number of quality layers
    uint8                       useLayer;                 //!< Decompression layer index

    uint8                       guard_bits;               //!< Guard bits for quantization

    uint64                      headerSize;               //!< Estimated global header size
    uint64                      codestreamSize;           //!< Total codestream size

    bwc_quant_st                quantization_style;       //!< Quantization strategy
    bwc_prog_ord                progression;              //!< Packet progression order
  } bwc_gl_ctrl;

/*================================================================================================*/
/**
 * @details Probes header information from a compressed data set. Contains global info, control
 *          settings, and auxiliary/comment blocks.
 */
/*================================================================================================*/
  typedef struct
  {
    bwc_gl_inf                  info;                     //!< Global dimensional information
    bwc_gl_ctrl                 control;                  //!< Control params for (de)compression
    bwc_span                    aux;                      //!< Auxiliary codestream block
    bwc_span                    com;                      //!< Comment codestream block
  } bwc_header;

/*================================================================================================*/
/**
 * @details Aggregates dimension info, control params, tile, and mode for the BigWhoop codec.
 *          Serves as the primary processing unit.
 */
/*================================================================================================*/
  typedef struct
  {
    bwc_gl_inf                  info;                     //!< Global dimensional information
    bwc_gl_ctrl                 control;                  //!< Control params for (de)compression

    bwc_tile                   *tile;                     //!< Pointer to bwc tile structure

    bwc_mode                    mode;                     //!< (De-)compression operation flag
  } bwc_codec;
#endif