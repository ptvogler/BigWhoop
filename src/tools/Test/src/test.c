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
 *        @file test.c
 *
 *        This file defines a simple command line tool that uses the Big Whoop library to
 *        (de) compress a 2- to 4-dimensional IEEE 754 floating point array. For further
 *        information use the --help (-h) argument in the command-line or consult the
 *        appropriate README file.
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
#include <argp.h>
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/// @cond DO_NOT_DOCUMENT
typedef unsigned char         uchar;
typedef unsigned short        ushort;
typedef unsigned int          uint;

typedef int8_t                int8;
typedef uint8_t               uint8;
typedef int16_t               int16;
typedef uint16_t              uint16;
typedef int32_t               int32;
typedef uint32_t              uint32;
typedef int64_t               int64;
typedef uint64_t              uint64;

typedef double                bwc_float;
typedef uint64                bwc_raw;

/*================================================================================================*/
/**
 * @details Structure used to store the user supplied command-line options and arguments.
 */
/*===========================|=========================|==========================================*/
typedef enum
{
  bwc_dwt_9_7,                                          // Cohen Daubechies Feauveau 9/7 Wavelet
  bwc_dwt_5_3,                                          // LeGall 5/3 Wavelet
  bwc_dwt_haar                                          // Haar Wavelet
} bwc_dwt_filter;

/*================================================================================================*/
/**
 * @details Structure used to store the user supplied command-line options and arguments.
 */
/*===========================|=========================|==========================================*/
typedef enum
{
  bwc_prog_LRCP,                                        // Layer / Resolution / Parameter / Packet
  bwc_prog_RLCP,                                        // Resolution / Layer / Parameter / Packet
  bwc_prog_RPCL,                                        // Resolution / Packet / Parameter / Layer
  bwc_prog_PCRL,                                        // Packet / Parameter / Resolution / Layer
  bwc_prog_CPRL                                         // Parameter / Packet / Resolution / Layer
} bwc_prog_ord;

/*================================================================================================*/
/**
 * @details Structure used to store the user supplied command-line options and arguments.
 */
/*===========================|=========================|==========================================*/
typedef enum
{
  bwc_qt_none,                                          // No quantization
  bwc_qt_derived,                                       // Derrived according to Taubman/Marcellin
} bwc_quant_st;

/*================================================================================================*/
/**
 * @details Structure used to store the user supplied command-line options and arguments.
 */
/*===========================|=========================|==========================================*/
typedef enum
{
  bwc_tile_sizeof,                                      // Tiling defined by size of one tile
  bwc_tile_numbof,                                      // Tiling defined by the number of tiles
} bwc_tile_instr;

/*================================================================================================*/
/**
 * @details Structure used to store the user supplied command-line options and arguments.
 */
/*===========================|=========================|==========================================*/
typedef enum
{
  bwc_precision_half = 2,
  bwc_precision_single = 4,
  bwc_precision_double = 8,
} bwc_precision;

/*================================================================================================*/
/**
 * @details Structure used to store the user supplied command-line options and arguments.
 */
/*===========================|=========================|==========================================*/
typedef enum
{
  bwc_cmp,
  bwc_dcp,
} bwc_mode;

/*================================================================================================*/
/**
 * @details Structure used to store the user supplied command-line options and arguments.
 */
/*===========================|=========================|==========================================*/
typedef struct
{
  uint64                      nX, nY, nZ;               // Spatial size of uncompressed data-set.
  uint64                      nTS;                      // Temp. size of uncompressed data-set.

  uint8                       nPar;                     // Number of parameters.

  bwc_precision               data_prec;                // Data type of uncompressed field data.
  bwc_precision               codec_prec;               // Encoder/decoder bit precision.
} bwc_gl_inf;

/*================================================================================================*/
/**
 * @details Structure used to store the user supplied command-line options and arguments.
 */
/*===========================|=========================|==========================================*/
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

  uint64                      headerSize;               // Size estimation of the global header.
  uint64                      codestreamSize;           // Size of entire code-stream.

  bwc_quant_st                quantization_style;       // Quantization style.
  bwc_prog_ord                progression;              // Packet progression order.
} bwc_gl_ctrl;

/*================================================================================================*/
/**
 * @details Structure used to store the user supplied command-line options and arguments.
 */
/*===========================|=========================|==========================================*/
typedef struct
{
  uint64                      size;                     // Size of packed stream.
  uint64                      position;                 // Reading/Writing pos. in the stream.

  uchar                      *access;                   // Pointer used to parse packed stream.
  uchar                      *memory;                   // Memory handle for the packed stream.
} bwc_span;

/*================================================================================================*/
/**
 * @details Structure used to store the user supplied command-line options and arguments.
 */
/*===========================|=========================|==========================================*/
typedef struct
{
  struct codestream
  {
    bwc_span               *aux;                        // Auxiliary info. codestream block.
    bwc_span               *com;                        // Comment codestream block.
  }codestream;

  void                     *inp;                        // User managed buffer for input
  void                     *out;                        // User managed buffer for output

  bwc_mode                  mode;                       // Flag to signal (de-)compression
} bwc_stream;

/*================================================================================================*/
/**
 * @details Structure used to store the user supplied command-line options and arguments.
 */
/*===========================|=========================|==========================================*/
typedef struct
{
  bwc_gl_inf                  info;                     // Global info structure
  bwc_gl_ctrl                 control;                  // Global control structure

  bwc_mode                    mode;                     // Flag to signal (de-)compression
} bwc_codec;
/// @endcond

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||                                 _  _ ____ ____ ____ ____ ____                                  ||
||                                 |\/| |__| |    |__/ |  | [__                                   ||
||                                 |  | |  | |___ |  \ |__| ___]                                  ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/// @cond DO_NOT_DOCUMENT
/*================================================================================================*/
/**
 * @details These macros define common error messages used throughout the BigWhoop cli.
 */
/*================================================================================================*/
#define MEMERROR "o##########################################################o\n"\
                 "|                   ERROR: Out of memory                   |\n"\
                 "o##########################################################o\n"

#define CSERROR  "o##########################################################o\n"\
                 "|                ERROR: Invalid Codestream                 |\n"\
                 "o##########################################################o\n"

/*================================================================================================*/
/**
 * @details These macros are used to set and probe the cli_arguments optSet variable
 */
/*================================================================================================*/
#define BITRT   0x8000
#define CBLKS   0x4000
#define CMPRT   0x2000
#define DCLVL   0x1000
#define DWTKL   0x0800
#define FLOUT   0x0400
#define OMPTH   0x0200
#define PRORD   0x0100
#define PRECS   0x0090
#define QFRMT   0x0040
#define QTSIZ   0x0020
#define QTSTL   0x0010
#define TILES   0x0008
#define USQLY   0x0004

/*================================================================================================*/
/**
 * @details This macro defines a simple operation to remove a supplied deliminator from a string.
 */
/*===========================================================================|====================*/
#define remove_deliminator(arg, end, delim)                                   \
{                                                                             \
  for(end = arg; *end; end++)                                                 \
    *end = (*end == delim ? ' ' : *end);                                      \
}
/// @endcond

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||                           ____ ____ _  _ ____ ___ ____ _  _ ___ ____                           ||
||                           |    |  | |\ | [__   |  |__| |\ |  |  [__                            ||
||                           |___ |__| | \| ___]  |  |  | | \|  |  ___]                           ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*================================================================================================*/
/**
 * @details Character strings containing the cli version and bug report e-mail address.
 */
/*================================================================================================*/
const char *argp_program_version      = "bwc 0.1.0";
const char *argp_program_bug_address  = "hpcpvogl@hlrs.de";

/*================================================================================================*/
/**
 * @details Docummentation string displayed at the start of the help section.
 */
/*================================================================================================*/
static char doc[] = "\n"\
                    "bwc is a simple command line tool that leverages the Big Whoop library to"
                    " (de)compress a 2- to 4-dimensional IEEE 754 "
                    "floating point array.\n"\
                    "\n"\
                    "Available use cases:\n"\
                    "\n"\
                    "  Compression:       bwc -C [INPUT] [OPTIONS]\n"\
                    "  Decompression:     bwc -D [INPUT] [OPTIONS]\n"\
                    "  Analysis:          bwc -A [INPUT] -R [REFERENCE]\n"\
                    "  Information:       bwc -H [INPUT]\n"\
                    "\n"\
                    "Valid Option Values:\n"\
                    "\n"
                    "  <input>            String that defines: input = path/to/an/input/file.\n"
                    "  <output>           String that defines: output = path/to/an/output/file.\n"
                    "\n"
                    "  <num>              Single numerical value: num = *.\n"
                    "  <narr>             One or more numerical values seperated by commas:\n"
                    "                     narr = *,*,...\n"
                    "\n"
                    "  <ndir>             Numerical values that can be specified globally or for\n"
                    "                     all spacial and temporal directions individually:\n"
                    "                     ndir = * or ndir = x,y,z,ts.\n";
                    /*"\n"
                    "  <str>              Single string.\n"
                    "  <sarr>             One or more strings seperated by commas: sarr = *,*,...\n"
                    "  <sdir>             Strings that can be specified globally or for all\n"
                    "                     spacial and temporal directions individually:\n"
                    "                     sdir = * or sdir = x,y,z,ts.\n"*/

//=================================================================================================|
/**
 * @details Structure specifing the BigWhoop command line interface options supported by the argp 
 *          parser.
 */
/*================================================================================================*/
static struct argp_option options[] = {
//--------------------|-----|------------|--------------------|--------------------------------|---|
//     long name      | key |  argument  |     argp flag      |         documentation          | g |
//--------------------|-----|------------|--------------------|--------------------------------|---|
//====================|=====|============|====================|================================|===|
{0,                    0,    0,           0,                  " [FILE OPTIONS]\n",              1},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"analysis",          'A',  "<input>",    0,                  "Analyze Peak Signal to Noise"
                                                              " Ratio (PSNR) and Mean Square" 
                                                              " Error (MSE) between input and" 
                                                              " reference file.\n",             1},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"comp",              'C',  "<input>",    0,                  "Compress a numerical dataset.",  1},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"decomp",            'D',  "<input>",    0,                  "Decompress a BigWhoop file.",    1},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"header",            'H',  "<input>",    0,                  "Display the header information"
                                                              " of a BigWhoop file.\n",         1},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"output",            'O',  "<output>",   0,                  "Defines output file.",           1},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"reference",         'R',  "<input>",    0,                  "Reference file used for PSNR"
                                                              " and MSE calculation.",          1},
//====================|=====|============|====================|================================|===|
//--------------------|-----|------------|--------------------|--------------------------------|---|
//     long name      | key |  argument  |     argp flag      |         documentation          | g |
//--------------------|-----|------------|--------------------|--------------------------------|---|
//====================|=====|============|====================|================================|===|
{0,                    0,    0,           0,                  " [LITERAL OPTIONS]\n",           2},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"erresilience",      'e',   0,           0,                  "Instrument bitstream to allow "
                                                              "for error resilient decoding of"
                                                              " compressed dataset.\n",         2},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"verbose",           'v',   0,           0,                  "Display compression statistics "
                                                              "and applied compression "
                                                              "parameters.",                    2},
//====================|=====|============|====================|================================|===|
//--------------------|-----|------------|--------------------|--------------------------------|---|
//     long name      | key |  argument  |     argp flag      |         documentation          | g |
//--------------------|-----|------------|--------------------|--------------------------------|---|
//====================|=====|============|====================|================================|===|
{0,                    0,    0,           0,                  " [NUMERICAL OPTIONS]\n",         3},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"bitrate",           'b',  "<narr>",     0,                  "Quality layers present in the "
                                                              "code-stream as a function of "
                                                              "the average bits per data-"
                                                              "point. Accepts real numbers in "
                                                              "the range of 0 < * < 64.\n",     3},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"codeblock",         'c',  "<ndir>",     0,                  "Codeblock size in log2 format. "
                                                              "Accepts natural numbers in the "
                                                              "range of 1 <= * <= 10 with the "
                                                              "sum having to lie in the range "
                                                              "of 4 < sum* < 20.\n",            3},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"decomplvl",         'd',  "<ndir>",     0,                  "Number of wavelet decomposi"
                                                              "tions applied to the data "
                                                              "arrays. Accepts natural numbers"
                                                              " in the range of 1 <= * <= 63."
                                                              "\n",                             3},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"layer",             'l',  "<num>",      OPTION_HIDDEN,      "Quality layer used to recon"
                                                              "struct the numerical data set. "
                                                              "Accepts natural numbers in the "
                                                              "range of 0 < * <= number_of_"
                                                              "quality_layers.\n",              3},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"nthreads",          'n',  "<num>",      0,                  "Number of OpenMP threads used "
                                                              "to (de)compress the <input> "
                                                              "file. Accepts natural numbers "
                                                              "in the range of 1 <= * <= 255."
                                                              "\n",                             3},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"precinct",          'p',  "<ndir>",     OPTION_HIDDEN,      "Precinct size in log2 format. "
                                                              "Accepts natural numbers in the"
                                                              " range of 1 <= * <= 15.\n",      3},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"qstep",             'q',  "<num>",      OPTION_HIDDEN,      "Quantization step size applied "
                                                              "to the wavelet coefficients. "
                                                              "Accepts real numbers in the "
                                                              "range of 0 < * < 2.\n",          3},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"qformat",           'm',  "<num>",      0,                  "Fractional bits of the Q number"
                                                              " format used in the floating-to"
                                                              "-fixed point transfomration. "
                                                              "Accepts natural numbers in the "
                                                              "range of 1 <= * <= 62.\n",       3},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"compratio",         'r',  "<num>",      0,                  "Target ratio between the uncom"
                                                              "presssed and compressed file "
                                                              "size. Accepts positive real "
                                                              "numbers.\n",                     3},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"tile",              't',   "<ndir>",    0,                  "Size of tiles the dataset is "
                                                              "subdivided into. Accepts "
                                                              "natural numbers in the range "
                                                              "of 16 <= * <= domain size.",     3},
//====================|=====|============|====================|================================|===|
//--------------------|-----|------------|--------------------|--------------------------------|---|
//     long name      | key |  argument  |     argp flag      |         documentation          | g |
//--------------------|-----|------------|--------------------|--------------------------------|---|
//====================|=====|============|====================|================================|===|
//{0,                    0,    0,           0,                  " [NUMERICAL OPTIONS]\n",         4},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"wkernels",          'k',  "<sdir>",     OPTION_HIDDEN,      "Wavelet kernels applied along "
                                                              "spatial and temporal aixs. "
                                                              "Accepts the strings CDF, LeGall"
                                                              " and Haar.",                     4},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{0}
};

//=================================================================================================|
/**
 * @details Constants used to signal the command_line interface mode.
 */
/*=====================================================|==========================================*/
typedef enum
{
  cli_ini,                                              //!< Undefined command line mode
  cli_cmp,                                              //!< Compression run
  cli_dcp,                                              //!< Decompression run
  cli_anl,                                              //!< Analyse distortion of reconstr. file
  cli_inf,                                              //!< Display BWC Header Information
} cli_mode;

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||                ___  ____ ____ ____ _ _  _ ____ ___     ___ _   _ ___  ____ ____                ||
||                |  \ |___ |__/ |__/ | |  | |___ |  \     |   \_/  |__] |___ [__                 ||
||                |__/ |___ |  \ |  \ |  \/  |___ |__/     |    |   |    |___ ___]                ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*================================================================================================*/
/**
 * @details Structure used to store the user supplied command-line options and arguments.
 */
/*===========================|=========================|==========================================*/
typedef struct
{
  char *args[2];
  cli_mode                    mode;                     //!< Current state of the cli tool
  uint16_t                    optSet;                   //!< Flag signaling witch opt has been set

  FILE                       *fpIn, *fpOut,*fpRef;      //!< Pointer to in/out/ref file
  char                       *nIn,  *nOut, *nRef;       //!< Name of the in/out/ref file
  void                       *in,   *out,  *ref;        //!< In/Ou/Ref buffer

  bool                        erresilience;             //!< Flag signalling error resilience
  bool                        verbose;                  //!< Flag signalling verbose output

  uint64_t                    tileSize[4];              //!< Spatial/Temporal tile size
  uint8_t                     precSize[4];              //!< Spatial/Temporal precinct size
  uint8_t                     cblkSize[4];              //!< Spatial/Temporal codeblock size

  //bwc_dwt_filter              dwtKernel[4];           //!< Spatial/Temporal wavelet kernels

  double                      rate[10];                 //!< Quality layers defining rate ctrl.
  uint8_t                     decompLevel[4];           //!< N.o. Spatial/temporal dwt decompo.

  double                      qt_step_size;             //!< Global qunatization step size
  uint8_t                     Qm;                       //!< Q number format range (m)

  uint8_t                     useLayer;                 //!< Quality layer used for decompression

  uint64_t                    nThreads;                 //!< Number of OpenMP threads

  //bwc_quant_st                quantization_style;       //!< Quantization style
  //bwc_prog_ord                progression;              //!< Packet progression order
} cli_arguments;


/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||           ___  ____ _ _  _ ____ ___ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____           ||
||           |__] |__/ | |  | |__|  |  |___    |___ |  | |\ | |     |  | |  | |\ | [__            ||
||           |    |  \ |  \/  |  |  |  |___    |    |__| | \| |___  |  | |__| | \| ___]           ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*================================================================================================*/
/**
 * @details This function takes the argument string, removes the supplied deliminators and 
 *          verifies that the values are valid. If decimal is set to true the decimal point
 *          in a floating point value will be ignored.
 *
 * @param[in]   arg   Argument corresponding to the option key.
 * @param[in]   delim Character used to deliminate between values.
 * @param[in]   dec   Bool signaling if string contains floating point values.
 *
 * @retval -1  Error
 * @retval  0  OK
 */
/*================================================================================================*/
static error_t
verify_opt(char *arg, char deliminator, bool decimal)
{
  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  char                   *parse;

  remove_deliminator(arg, parse, deliminator);

  for(parse = arg; *parse; parse++)
    {
      if(decimal == true)
        {
          if(!isdigit(*parse) && *parse != ' ' && *parse != '.')
            return EXIT_FAILURE;
        }
      else
        {
          if(!isdigit(*parse) && *parse != ' ')
            return EXIT_FAILURE;
        }
    }
   return EXIT_SUCCESS;
}

/*================================================================================================*/
/**
 * @details This function analyzes one option at a time and sets the corresponding value in
 *          the cli arguments struct.
 *
 * @param[in]     key   Option's key corresponding to the field value in the arg_option struct.
 * @param[in]     arg   Argument corresponding to the option key.
 * @param[in,out] state Struct containing useful information about the current parsing state.
 *
 * @retval -1  Error
 * @retval  0  OK
 */
/*================================================================================================*/
static error_t
parse_opt(int                key,
          char              *arg,
          struct argp_state *state)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  int64_t                 buff;
  uint8_t                 i;


  /*-----------------------*\
  ! DEFINE REAL VARIABLES:  !
  \*-----------------------*/
  double                  compRatio;
  double                  qt_step_size;
  float                   bitrate;


  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  char                   *end;
  char                   *token, *ptr;

  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  cli_arguments          *arguments;
  bwc_codec              *codec;
  bwc_stream             *stream;

  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(state);

  /* Save frequently used variables/structures to         *
   * temporary variables.                                 */
  arguments = state->input;

  /* Parse the cli arguments.                             */
  switch(key)
    {
      /* Ingest compression argument.                         */
      case 'C':
        {
          if(arguments->mode != cli_ini)
            {
              argp_error(state, "Arguments define multiple use cases.\n");
            }
          else if ((arg == NULL) || (arg[0] == '-'))
            {
              argp_error(state, "file names that start with an '-'"
                                " are not supported.\n");
            }
          else
            {
              arguments->mode = cli_cmp;
              arguments->nIn =  arg;
            }
          break;
        }

      /* Ingest decompression argument.                       */
      case 'D':
        {
          if(arguments->mode != cli_ini)
            {
              argp_error(state, "Arguments define multiple use cases.\n");
            }
          else if ((arg == NULL) || (arg[0] == '-'))
            {
              argp_error(state, "file names that start with an '-'"
                                " are not supported.\n");
            }
          else
            {
              arguments->mode = cli_dcp;
              arguments->nIn =  arg;
            }
          break;
        }

      /* Ingest analysis argument.                            */
      case 'A':
        {
          if(arguments->mode != cli_ini)
            {
              argp_error(state, "Arguments define multiple use cases.\n");
            }
          else if ((arg == NULL) || (arg[0] == '-'))
            {
              argp_error(state, "file names that start with an '-'"
                                " are not supported.\n");
            }
          else
            {
              arguments->mode = cli_anl;
              arguments->nIn =  arg;
            }

          break;
        }

      /* Ingest header info argument.                         */
      case 'H':
        {
          if(arguments->mode != cli_ini)
            {
              argp_error(state, "Arguments define multiple use cases.\n");
            }
          else if ((arg == NULL) || (arg[0] == '-'))
            {
              argp_error(state, "file names that start with an '-'"
                                " are not supported.\n");
            }
          else
            {
              arguments->mode = cli_inf;
              arguments->nIn =  arg;
            }

          break;
        }

      /* Ingest output argument.                              */
      case 'O':
        {
          if ((arg == NULL) || (arg[0] == '-'))
            {
              argp_error(state, "file names that start with an '-'"
                                " are not supported.\n");
            }
          else
            {
              arguments->nOut    = arg;
              arguments->optSet |= FLOUT;
            }
            
          break;
        }

      /* Ingest reference argument.                           */
      case 'R':
        {
          arguments->nRef = arg;
          break;
        }

      /* Ingest verbose argument.                             */
      case 'v':
        {
          arguments->verbose = true;
          break;
        }

      /* Ingest bit rates.                                    */
      case 'b':
        {
          if(arguments->optSet & BITRT)
            {
              argp_error(state, "The bitrate can only be defined once.\n");
            }
          else if(arguments->optSet & CMPRT)
            {
              argp_error(state, "The size reduction requires to be specified "
                                "either by a bit rate or a compression ratio, "
                                "never both.\n");
            }

          if(verify_opt(arg, ',', true) == EXIT_FAILURE)
            {
              argp_error(state, "Invalid delimitnator in the bitrate option\n");
            }

          for(bitrate = strtod(arg, &end), i = 0; arg != end && i < 10;
              bitrate = strtod(arg, &end), i++)
            {
              arg = end;

              if(bitrate > 0 && bitrate <= 64 && errno != ERANGE)
                {
                  arguments->rate[i] = (double) bitrate;
                }
              else
                {
                  errno = 0;
                  argp_error(state, "The specified bitrate (%f) is "
                                    "out of the supported range.\n", bitrate);
                }
            }

          arguments->optSet |= BITRT;
          break;
        }

      /* Ingest codeblock size.                               */
      case 'c':
        {
          if(arguments->optSet & CBLKS)
            {
              argp_error(state, "The codeblock size can only be defined once.\n");
            }

          if(verify_opt(arg, ',', false) == EXIT_FAILURE)
            {
              argp_error(state, "Invalid delimitnator in the codeblock"
                                " size option.\n");
            }

          for(buff = strtoll(arg, &end, 10), i = 0; arg != end && i < 4;
              buff = strtoll(arg, &end, 10), i++)
            {
              arg = end;

              if(buff >= 0 && buff <= 10 && errno != ERANGE)
                {
                  arguments->cblkSize[i] = (uint8_t) buff;
                }
              else
                {
                  errno = 0;
                  argp_error(state, "The specified codeblock size (%ld) "
                                    "is out of the supported range.\n", buff);
                  break;
                }
            }

          if(i == 1)
            {
              arguments->cblkSize[1] =
              arguments->cblkSize[2] =
              arguments->cblkSize[3] = arguments->cblkSize[0];
            }
          else if(i != 4)
            {
              argp_error(state, "The codeblock argument expects either a "
                                "single global or 4 directional values\n");
            }

          arguments->optSet |= CBLKS;
          break;
        }

      /* Ingest compression ratio.                            */
      case 'r':
        {
          if(arguments->optSet & CMPRT)
            {
              argp_error(state, "The compression ratio can only be defined once.\n");
            }
          else if(arguments->optSet & BITRT)
            {
              argp_error(state, "The size reduction requires to be specified "
                                "either by a bit rate or a compression ratio, "
                                "never both.\n");
            }

          if(verify_opt(arg, ',', true) == EXIT_FAILURE)
            {
              argp_error(state, "Invalid delimitnator in the compression"
                                " ratio option \n");
            }

          for(compRatio = strtod(arg, &end), i = 0; arg != end && i < 10;
              compRatio = strtod(arg, &end), i++)
            {
              arg = end;

              if(compRatio > 0 && compRatio < 65536 && errno != ERANGE)
                {
                  arguments->rate[i] = (double) compRatio;
                }
              else
                {
                  errno = 0;
                  argp_error(state, "The specified bitrate (%d) is "
                                    "out of the supported range.\n", compRatio);
                }
            }

          arguments->optSet |= CMPRT;
          break;
        }

      /* Ingest decomposition level.                          */
      case 'd':
        {
          if(arguments->optSet & DCLVL)
            {
              argp_error(state, "The compression ratio can only be defined once.\n");
            }

          if(verify_opt(arg, ',', false) == EXIT_FAILURE)
            {
              argp_error(state, "Invalid delimitnator in the decomposition"
                                " level option.\n");
            }

          for(buff = strtoll(arg, &end, 10), i = 0; arg != end && i < 4;
              buff = strtoll(arg, &end, 10), i++)
            {
              arg = end;

              if(buff > 0 && buff <= 63 && errno != ERANGE)
                {
                  arguments->decompLevel[i] = (uint8_t) buff;
                }
              else
                {
                  errno = 0;
                  argp_error(state, "The specified decomposition level (%ld) "
                                    "is out of the supported range.\n", buff);
                  break;
                }
            }

          if(i == 1)
            {
              arguments->decompLevel[1] =
              arguments->decompLevel[2] =
              arguments->decompLevel[3] = arguments->decompLevel[0];
            }
          else if(i != 4)
            {
              argp_error(state, "The decomposition level argument expects either a "
                                "single global or 4 directional values\n");
            }

          arguments->optSet |= DCLVL;
          break;
        }

      /* Ingest number of OpenMP threads.                     */
      case 'n':
        {
          buff = strtoll(arg, &end, 10);

          if (errno == ERANGE)
            {
              errno = 0;
              argp_error(state, "The specified number of OpenMP threads (%ld) "
                                "is out of the supported range.\n", buff);
            }
          else
          {
            arguments->nThreads = (uint64_t) buff;
          }

          arguments->optSet |= OMPTH;
          break;
        }

      /* Ingest quality layer to be used for decompression.   */
      case 'l':
        {
          buff = strtoll(arg, &end, 10);

          if (buff > 0 && buff <= 255 && errno != ERANGE)
            {
              errno = 0;
              argp_error(state, "The specified quality layer (%ld) "
                                "is out of the supported range.\n", buff);
            }
          else
          {
            arguments->useLayer = (uint8_t) buff;
          }

          arguments->optSet |= USQLY;
          break;
        }

      /* Ingest wavelet kernels.                              */
      /*case 'k':
        {
          if(arguments->optSet & DWTKL)
            {
              argp_error(state, "The wavelet kernels can only be defined once.\n");
            }

          for(token =  strtok_r(str, ",", &ptr), i = 0;
              token != NULL, i < 4;
              token = strtok_r(NULL, ",", &ptr), i++)
            {
              if(strcasecmp(token, "leGall") == 0)
                arguments->dwtKernel[i] =  bwc_dwt_5_3;
              else if (strcasecmp(token, "CDF") == 0)
                arguments->dwtKernel[i] = bwc_dwt_9_7;
              else if (strcasecmp(token, "Haar") == 0)
                arguments->dwtKernel[i] = bwc_dwt_haar;
              else
                argp_error(state, "Waveket kernel %s is unknown "
                                  "to the library.\n", token);
            }

          if(i == 1)
            {
              arguments->dwtKernel[1] =
              arguments->dwtKernel[2] =
              arguments->dwtKernel[3] = arguments->dwtKernel[0];
            }
          else if(i != 4)
            {
              argp_error(state, "The wavelet kernel argument expects either a "
                                "single global or 4 directional values\n");
            }

          arguments->optSet |= DWTKL;
          break;
        }*/

      /* Ingest quantization step size.                       */
      case 'q':
        {
          qt_step_size = strtod(arg, &end);

          if(qt_step_size > 0 && qt_step_size < 2 && errno != ERANGE)
            {
              arguments->qt_step_size = (double) qt_step_size;
            }
          else
            {
              errno = 0;
              argp_error(state, "The specified quantization step size (%f) is "
                                "out of the supported range.\n", qt_step_size);
            }

          arguments->optSet |= QTSIZ;
          break;
        }

      /* Ingest Q number format range.                        */
      case 'm':
        {
          if(arguments->optSet & QFRMT)
            {
              argp_error(state, "The Q format range can only be defined once.\n");
            }

          buff = strtoll(arg, &end, 10);
          if(buff > 0 && buff < 63 && errno != ERANGE)
            {
              arguments->Qm = (uint8_t) buff;
            }
          else
            {
              errno = 0;
              argp_error(state, "The specified Q number format range (%ld) "
                                "is out of the supported range.\n", buff);
            }

          arguments->optSet |= QFRMT;
          break;
        }

      /* Ingest error resilience setting.                     */
      case 'e':
        {
          arguments->erresilience = true;
          break;
        }

      /* Ingest tile size.                                    */
      case 't':
        {
          if(arguments->optSet & TILES)
            {
              argp_error(state, "The tile size can only be defined once.\n");
            }

          if(verify_opt(arg, ',', false) == EXIT_FAILURE)
            {
              argp_error(state, "Invalid delimitnator in the tile"
                                " size option.\n");
            }

          for(buff = strtoll(arg, &end, 10), i = 0; arg != end && i < 4;
              buff = strtoll(arg, &end, 10), i++)
            {
              arg = end;

              if(buff >= 16 && errno != ERANGE)
                {
                  arguments->tileSize[i] = (uint64_t) buff;
                }
              else
                {
                  errno = 0;
                  argp_error(state, "The specified tile size (%ld) "
                                    "is out of the supported range.\n", buff);
                  break;
                }
            }

          if(i == 1)
            {
              arguments->tileSize[1] =
              arguments->tileSize[2] =
              arguments->tileSize[3] = arguments->tileSize[0];
            }
          else if(i != 4)
            {
              argp_error(state, "The tile argument expects either a "
                                "single global or 4 directional values\n");
            }

          arguments->optSet |= TILES;
          break;
        }

      /* Ingest precinct size.                                */
      case 'p':
        {
          if(arguments->optSet & PRECS)
            {
              argp_error(state, "The precinct size can only be defined once.\n");
            }

          if(verify_opt(arg, ',', false) == EXIT_FAILURE)
            {
              argp_error(state, "Invalid delimitnator in the precinct"
                                " size option.\n");
            }

          for(buff = strtoll(arg, &end, 10), i = 0; arg != end && i < 4;
              buff = strtoll(arg, &end, 10), i++)
            {
              arg = end;

              if(buff >= 0 && buff <= 15 && errno != ERANGE)
                {
                  arguments->precSize[i] = (uint8_t) buff;
                }
              else
                {
                  errno = 0;
                  argp_error(state, "The specified precinct size (%ld) "
                                    "is out of the supported range.\n", buff);
                  break;
                }
            }

          if(i == 1)
            {
              arguments->precSize[1] =
              arguments->precSize[2] =
              arguments->precSize[3] = arguments->precSize[0];
            }
          else if(i != 4)
            {
              argp_error(state, "The precinct argument expects either a "
                                "single global or 4 directional values\n");
            }

          arguments->optSet |= PRECS;
          break;
        }

      /* Check if the user supplied options fit the supported *
       * use case.                                            */
      case ARGP_KEY_END:
        {
          if(((arguments->mode == cli_cmp                                                         || 
               arguments->mode == cli_dcp) && (arguments->nIn == NULL || arguments->nRef != NULL))||
              (arguments->mode == cli_inf  && (arguments->nIn == NULL || arguments->nRef != NULL
                                                                      || arguments->nOut != NULL))||
              (arguments->mode == cli_anl  && (arguments->nIn == NULL || arguments->nRef == NULL 
                                                                      || arguments->nOut != NULL)))
            {
              argp_error(state, "The User supplied options do not fit the "
                                "supported use cases.\n");
            }
          else if (arguments->mode == cli_ini)
            {
              argp_usage(state);
            }
          break;
        }

      /* Output standard usage message if no arg is defined.  */
      case ARGP_KEY_NO_ARGS:
        if(arguments->mode == cli_ini)
          argp_usage (state);

      /* Return error if key is unknown.                      */
      default:
        return ARGP_ERR_UNKNOWN;
    }
  return EXIT_SUCCESS;
}

/*================================================================================================*/
/**
 * @details Initialize the argp struct. used to parse the command line arguments
 */
/*================================================================================================*/
static struct argp argp = {options, parse_opt, 0, doc};

/*================================================================================================*/
/**
 * @details This function defines the command-line interface for the BigWhoop compression 
 *          library.
 * 
 * @param[in] argc  Number of strings pointed to by argv
 * @param[in] argv  Array of arguments
 * 
 * @retval -1  Error
 * @retval  0  OK
 */
/*================================================================================================*/
int main(int argc, char *argv[])
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint8_t                 i;

  /*-----------------------*\
  ! DEFINE REAL VARIABLES:  !
  \*-----------------------*/
  double                  rtype;

  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  char                    rate[200] = {0};

  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  cli_arguments           arguments = {0};

  /* Parse the command line arguments and invoke the appro- *
   * priate bwccmdl mode.                                   */
  if(argp_parse(&argp, argc, argv, 0, 0, &arguments) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }

  if(arguments.mode == cli_cmp)
    {
      /* Set the Q format range.                              */
      if((arguments.optSet & QFRMT) != 0)
        {
          printf("Q Format: %d\n", arguments.Qm);
        }

      /* Set the wavelet kernels.                             */
      /*if((arguments.optSet & DWTKL) != 0)
        {
          printf("Wavelet Kernel\n");
        }*/

      /* Set the decompositon level option.                   */
      if((arguments.optSet & DCLVL) != 0)
        {
          printf("Decomposition Level:\t x %d\t y %d\t z %d\t t %d\n", arguments.decompLevel[0],
                                                                       arguments.decompLevel[1],
                                                                       arguments.decompLevel[2],
                                                                       arguments.decompLevel[3]);
        }

      /* Set the tile size option.                            */
      if((arguments.optSet & TILES) != 0)
        {
          printf("Tile Size:\t x %d\t y %d\t z %d\t t %d\n", arguments.tileSize[0],
                                                             arguments.tileSize[1],
                                                             arguments.tileSize[2],
                                                             arguments.tileSize[3]);
        }

      /* Set the precinct size option.                        */
      if((arguments.optSet & PRECS) != 0)
        {
          printf("Precinct Size:\t x %d\t y %d\t z %d\t t %d\n", arguments.precSize[0],
                                                                 arguments.precSize[1],
                                                                 arguments.precSize[2],
                                                                 arguments.precSize[3]);
        }

      /* Set the codeblock size option.                       */
      if((arguments.optSet & CBLKS) != 0)
        {
          printf("Codeblock Size:\t x %d\t y %d\t z %d\t t %d\n", arguments.cblkSize[0],
                                                                  arguments.cblkSize[1],
                                                                  arguments.cblkSize[2],
                                                                  arguments.cblkSize[3]);
        }

      /* Initialize the rate control string according to the  *
       * specified bit rate/compression ratio.                */
      if((arguments.optSet & BITRT) != 0)
        {
          rtype = 1.0;
        }
      else if((arguments.optSet & CMPRT) != 0)
        {
          rtype = 64.0;
        }
      else
        {
          rtype = 1.0;
          arguments.rate[0] = 64;
        }

      for(i = 0; i < 10 && strlen(rate) < 192; ++i)
        {
          if(arguments.rate[i] > 0)
            sprintf(rate + strlen(rate), "%05.3f,", arguments.rate[i]/rtype);
        }
      rate[strlen(rate) - 1] = '0';
      printf("bitrate: %s\n", rate);











      if((arguments.optSet & FLOUT) != 0)
        {
          printf("File Out\n");
        }
      if((arguments.optSet & OMPTH) != 0)
        {
          printf("OpenMP Threads\n");
        }
      if((arguments.optSet & QTSIZ) != 0)
        {
          printf("Quantization Step Size\n");
        }
      if((arguments.optSet & USQLY) != 0)
        {
          printf("Use Quality Layer\n");
        }
    }
  else if (arguments.mode == cli_dcp)
    {
      printf("Decompression\n");
    }
  else if (arguments.mode == cli_anl)
    {
      printf("Analysis\n");
    }
  else if (arguments.mode == cli_inf)
    {
      printf("Header Info\n");
    }
  else
    {
      return EXIT_FAILURE;
    }

  if((arguments.optSet & FLOUT) != 0)
    {
      printf("File Out\n");
    }
  if((arguments.optSet & OMPTH) != 0)
    {
      printf("OpenMP Threads\n");
    }
  if((arguments.optSet & QTSIZ) != 0)
    {
      printf("Quantization Step Size\n");
    }
  if((arguments.optSet & USQLY) != 0)
    {
      printf("Use Quality Layer\n");
    }


  //printf("ARG1:    %s", arguments.args[0]);
  //printf("\nVERBOSE: %s", arguments.verbose? "yes" : "no");
  //printf("\nOption1: %s", arguments.option1);
  //printf("\n");
}
