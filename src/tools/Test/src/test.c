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
                    "  Compression:       bwc -c [INPUT] [OPTIONS]\n"\
                    "  Decompression:     bwc -d [INPUT] [OPTIONS]\n"\
                    "  Analysis:          bwc -a [INPUT] -r [REFERENCE]\n"\
                    "  Information:       bwc -h [INPUT]\n"\
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
                    "                     ndir = * or ndir = x/y/z/ts.\n";
                    /*"\n"
                    "  <str>              Single string.\n"
                    "  <sarr>             One or more strings seperated by commas: sarr = *,*,...\n"
                    "  <sdir>             Strings that can be specified globally or for all\n"
                    "                     spacial and temporal directions individually:\n"
                    "                     sdir = * or sdir = x/y/z/ts.\n"*/

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
{"analysis",          'a',  "<input>",    0,                  "Analyze Peak Signal to Noise"
                                                              " Ratio (PSNR) and Mean Square" 
                                                              " Error (MSE) between input and" 
                                                              " reference file.\n",             1},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"comp",              'c',  "<input>",    0,                  "Compress a numerical dataset.",  1},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"decomp",            'd',  "<input>",    0,                  "Decompress a BigWhoop file.",    1},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"header",            'h',  "<input>",    0,                  "Display the header information"
                                                              " of a BigWhoop file.\n",         1},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"output",            'o',  "<output>",   0,                  "Defines output file.",           1},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"reference",         'r',  "<input>",    0,                  "Reference file used for PSNR"
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
{"stream",            's',   0,           OPTION_HIDDEN,      "Stream data to and from <input>"
                                                              "/<output>.",                     2},
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
{"codeblock",         'B',  "<ndir>",     0,                  "Codeblock size in log2 format. "
                                                              "Accepts natural numbers in the "
                                                              "range of 1 <= * <= 10 with the "
                                                              "sum having to lie in the range "
                                                              "of 4 < sum* < 20.\n",            3},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"decomplvl",         'D',  "<ndir>",     0,                  "Number of wavelet decomposi"
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
{"qformat",           'Q',  "<num>",      0,                  "Fractional bits of the Q number"
                                                              " format used in the floating-to"
                                                              "-fixed point transfomration. "
                                                              "Accepts natural numbers in the "
                                                              "range of 1 <= * <= 62.\n",       3},
//--------------------|-----|------------|--------------------|--------------------------------|---|
{"compratio",         'R',  "<num>",      0,                  "Target ratio between the uncom"
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
  cli_err,                                              //!< Command-line interface error
  cli_cmp,                                              //!< Compression run
  cli_dcp,                                              //!< Decompression run
  cli_anl,                                              //!< Analyse distortion of reconstr. file
  cli_hdr,                                              //!< Display header information
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
  cli_mode                    mode;                     //!< Current state of the cli tool

  FILE                       *fpIn, *fpOut;             //!< Pointer to input/output file
  char*                      *in,   *out;               //!< Name of the input/output files

  float                       bitrate[10];              //!< Quality layers defined by bitrate.

  bwc_stream                 *stream;                   //!< Structure defining the BigWhoop I/O
  bwc_codec                  *codec;                    //!< Structure defining the BigWhoop codec
} cli_arguments;


/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||           ___  ____ _ _  _ ____ ___ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____           ||
||           |__] |__/ | |  | |__|  |  |___    |___ |  | |\ | |     |  | |  | |\ | [__            ||
||           |    |  \ |  \/  |  |  |  |___    |    |__| | \| |___  |  | |__| | \| ___]           ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*================================================================================================*/
/**
 * @details This function analyzes one option at a time and sets the corresponding value and
 *          passes arguments in the bwc_codec struct.
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
  //uint64_t                buff_LL;
  //uint64_t                multiplier;

  //uint16_t                buffI;

  uint8_t                 i;
  //uint8_t                 length, shift;


  /*-----------------------*\
  ! DEFINE REAL VARIABLES:  !
  \*-----------------------*/
  float                   bitrate;

  /*-----------------------*\
  ! DEFINE REAL VARIABLES:  !
  \*-----------------------*/
  char                   *end;

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

  /*--------------------------------------------------------*\
  ! Save frequently used variables/structures to temporary   !
  ! variables to make the code more readable.                !
  \*--------------------------------------------------------*/
  arguments = state->input;
  codec     = arguments->codec;
  stream    = arguments->stream;

  /*--------------------------------------------------------*\
  ! Parse the cli arguments according to the supplied opt.   !
  \*--------------------------------------------------------*/
  switch(key)
    {
      case 'c':
        {
          arguments->mode = bwc_cmp;
          printf("Compress\n");
          if(arg[0] == '-')
            {
              argp_error(state, "No input specified\n");
            }
          else
            {
              arguments->input     = ;
            }
          break;
        }
      case 'd':
        {
          arguments->mode = bwc_dcp;
          printf("Decompress\n");
          break;
        }
      case 'a':
        {
          //arguments->analysis = arg;
          break;
        }
      case 'h':
        {
          //arguments->header = arg;
          break;
        }
      case 'o':
        {
          //arguments->output = arg;
          break;
        }
      case 'r':
        {
          //arguments->reference = arg;
          break;
        }
      case 's':
        {
          //arguments->stream = 1;
          break;
        }
      case 'v':
        {
          //arguments->stream = 1;
          break;
        }
      case 'b':
        {
          remove_deliminator(arg, end, ',');

          for(bitrate = strtod(arg, &end), i = 0; arg != end && i < 10;
              bitrate = strtod(arg, &end), i++)
            {
              arg = end;

              if(bitrate > 0 && bitrate < 64 && errno != ERANGE)
                {
                  arguments->bitrate[i] = (float) bitrate;
                }
              else
                {
                  argp_error(state, "The specified bitrate (%f) is "
                                    "out of the supported range.\n", bitrate);
                  
                  arguments->mode = cli_err;
                  errno = 0;
                  break;
                }
            }
          break;
        }
      case 'B':
        {
          // remove_deliminator(arg, end, '/');

          // for(buff_LL = strtoll(arg, &end, 10), i = 0; arg != end && i < 4;
          //     buff_LL = strtoll(arg, &end, 10), i++)
          //   {
          //     arg = end;
          //     if (errno == ERANGE)
          //       {
          //         printf("range error, got ");
          //         errno = 0;
          //       }

          //     if(buff_LL > 1 && buff_LL < 10)
          //       {
          //         arguments->codeblock[i] = (uint8_t) buff_LL;
          //       }
          //     else
          //       {
          //         argp_error(state, "The specified codeblock size (%ld) "
          //                           "is out of the supported range.\n", buff_LL);
          //       }
          //   }

          // if(i == 1)
          //   {
          //     arguments->codeblock[1] =
          //     arguments->codeblock[2] =
          //     arguments->codeblock[3] = arguments->codeblock[0];
          //   }
          // else if(i != 4)
          //   {
          //     argp_error(state, "The codeblock argument expects either a "
          //                       "single global or 4 dirctional values\n");
          //   }

          // if(((arguments->codeblock[0] + arguments->codeblock[1] +
          //      arguments->codeblock[2] + arguments->codeblock[3]) < 4) || 
          //    ((arguments->codeblock[0] + arguments->codeblock[1] +
          //      arguments->codeblock[2] + arguments->codeblock[3]) > 20))
          //   {
          //     argp_error(state, "The sum of the specified codeblock sizes "
          //                       "is outside of the supported range\n");
          //   }
          break;
        }
      case 'R':
        {
          // arguments->compratio = (uint8_t)strtoll(arg, &end, 10);
          // printf("%d\n", arguments->compratio);
          break;
        }
      case 'D':
        {
/*          remove_deliminator(arg, end, '/');

          for(buff_LL = strtoll(arg, &end, 10), i = 0; arg != end && i < 4;
              buff_LL = strtoll(arg, &end, 10), i++)
            {
              arg = end;
              if (errno == ERANGE)
                {
                  printf("range error, got ");
                  errno = 0;
                }

              if(buff_LL >= 1 && buff_LL <= 63)
                {
                  arguments->codeblock[i] = (uint8_t) buff_LL;
                }
              else
                {
                  argp_error(state, "The specified codeblock size (%ld) "
                                    "is out of the supported range.\n", buff_LL);
                }
            }

          if(i == 1)
            {
              arguments->codeblock[1] =
              arguments->codeblock[2] =
              arguments->codeblock[3] = arguments->codeblock[0];
            }
          else if(i != 4)
            {
              argp_error(state, "The codeblock argument expects either a "
                                "single global or 4 directional values\n");
            }*/
          break;
        }
      case 'n':
        {
          // buff_LL = strtoll(arg, &end, 10);
          // if((buff_LL < 1) || (buff_LL > 255))
          //   {
          //     argp_error(state, "The number of OpenMP threads specified"
          //                       "is out of the supported range.\n");
          //   }
          // else
          //   {
          //     arguments->nthreads = (uint8_t)buff_LL;
          //   }
          break;
        }
      case 'l':
        {
          printf("l\n");
          break;
        }
      case 'k':
        {
          printf("k\n");
          break;
        }
      case 'q':
        {
          printf("q\n");
          break;
        }
      case 'Q':
        {
          printf("Q\n");
          break;
        }
      case 'e':
        {
          printf("e\n");
          break;
        }
      case 't':
        {
          printf("t\n");
          break;
        }
      case 'p':
        {
          printf("p\n");
          break;
        }

      case ARGP_KEY_NO_ARGS:
        {
          argp_usage (state);
          break;
        }  

      case ARGP_KEY_ARG:
      
          // Too many arguments.
          if(state->arg_num > 1)
              argp_usage(state);
          break;

      case ARGP_KEY_END:
          /*if(!((arguments->analysis  != NULL) &&  (arguments->comp      == NULL)
                                              &&  (arguments->decomp    == NULL)
                                              &&  (arguments->header    == NULL)
                                              &&  (arguments->output    == NULL)
                                              &&  (arguments->reference != NULL)) &&
             !((arguments->comp      != NULL) &&  (arguments->analysis  == NULL)
                                              &&  (arguments->decomp    == NULL)
                                              &&  (arguments->header    == NULL)
                                              &&  (arguments->reference == NULL)) &&
             !((arguments->decomp    != NULL) &&  (arguments->analysis  == NULL)
                                              &&  (arguments->comp      == NULL)
                                              &&  (arguments->header    == NULL)
                                              &&  (arguments->reference == NULL)) &&
             !((arguments->header    != NULL) &&  (arguments->analysis  == NULL)
                                              &&  (arguments->comp      == NULL)
                                              &&  (arguments->decomp    == NULL)
                                              &&  (arguments->output    == NULL)
                                              &&  (arguments->reference == NULL)))
            {
              argp_error(state, "The User supplied options do not fit the "
                                "supported use cases.\n");
            }*/
          break;

      default:
          return ARGP_ERR_UNKNOWN;
    }
  return EXIT_SUCCESS;
}

// initialize the argp struct. Which will be used to parse and use the args.
static struct argp argp = {options, parse_opt, 0, doc};

/*================================================================================================*/
/**
 * @details This function provides the command-line interface for the BigWhoop compression 
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
  int i = 0;
  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  cli_arguments   arguments = {0};

  /*--------------------------------------------------------*\
  ! Initialize the arguments structure.                      !
  \*--------------------------------------------------------*/
  arguments.stream  = calloc(1, sizeof(bwc_stream));
  arguments.codec   = calloc(1, sizeof(bwc_codec));
  if((arguments.stream == NULL) ||
     (arguments.codec  == NULL))
    {
       // memory allocation error
       fprintf(stderr, MEMERROR);
       return EXIT_FAILURE;
    }

  /*--------------------------------------------------------*\
  ! Parse the cli arguments.                                 !
  \*--------------------------------------------------------*/
  argp_parse(&argp, argc, argv, 0, 0, &arguments);
  if(arguments.mode == cli_err)
    {
      return EXIT_FAILURE;
    }

  for(i = 0; i < 10; ++i)
    printf("bitrate %d:\t%f\n",i,arguments.bitrate[i]);



  //printf("ARG1:    %s", arguments.args[0]);
  //printf("\nVERBOSE: %s", arguments.verbose? "yes" : "no");
  //printf("\nOption1: %s", arguments.option1);
  //printf("\n");
}
