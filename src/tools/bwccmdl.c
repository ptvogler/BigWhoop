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
 *        @file bwccmdl.c
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
#include <math.h>
#include <omp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "eas3.h"
#include "bwc.h"

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

#define TYPERROR  "o##########################################################o\n"\
                  "|                 ERROR: Invalid file type.                |\n"\
                  "o##########################################################o\n"

#define FINERROR  "o##########################################################o\n"\
                  "|       ERROR: Could not open specified input file.        |\n"\
                  "o##########################################################o\n"

#define FOUERROR  "o##########################################################o\n"\
                  "|       ERROR: Could not open specified output file.       |\n"\
                  "o##########################################################o\n"

#define RDERROR   "o##########################################################o\n"\
                  "|      ERROR: Invalid number of bytes read from file.      |\n"\
                  "o##########################################################o\n"
 
#define WRTERROR  "o##########################################################o\n"\
                  "|      ERROR: Invalid number of bytes written to file.     |\n"\
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
#define FLREF   0x0200   
#define OMPTH   0x0100
#define PRORD   0x0080
#define PRECS   0x0040
#define QFRMT   0x0020
#define QTSIZ   0x0010
#define QTSTL   0x0008
#define TILES   0x0004
#define USQLY   0x0002

/*================================================================================================*/
/**
 * @details Macros to determine the minimum or maximum of two values.
 */
/*===========================================================================|====================*/
#define MAX(x, y)             (((x) < (y))?(y):(x))     // Returns maximum between two values
#define MIN(x, y)             (((x) > (y))?(y):(x))     // Returns minimum between two values

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
 * @details String containing the header used for the cli output.
 */
/*================================================================================================*/
static char bwc_header_art[] = "==============================================================\n"\
                               "                                                              \n"\
                               "          .:-------------:           .:-------------:         \n"\
                               "         .+++++++++++++++=          :+++++++++++++++-         \n"\
                               "         :+++.        -++=          -++=                      \n"\
                               "         :+++.        -++=          -++=                      \n"\
                               "          -++++++++++++++=   -++=   -++=                      \n"\
                               "         .=++---------=++=   -++=   -++=                      \n"\
                               "         :+++         :++=   -++=   -++=                      \n"\
                               "         .+++=--------=+++---=+++---=+++------------:         \n"\
                               "          -=++++++++++++++++++++++++++++++++++++++++-         \n"\
                               "                                                              \n";

/*================================================================================================*/
/**
 * @details Character strings containing the cli version and bug report e-mail address.
 */
/*================================================================================================*/
const char *argp_program_version      = "bwc 0.1.0";
const char *argp_program_bug_address  = "patrick.vogler@hlrs.de";

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
// clang-format off
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
// clang-format on

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

  char                       *in, *out, *ref;           //!< Name of the in/out/ref file

  bool                        erresilience;             //!< Flag signalling error resilience
  bool                        verbose;                  //!< Flag signalling verbose output

  uint64_t                    tileSize[4];              //!< Spatial/Temporal tile size
  uint8_t                     precSize[4];              //!< Spatial/Temporal precinct size
  uint8_t                     cblkSize[4];              //!< Spatial/Temporal codeblock size

  bwc_dwt_filter              dwtKernel[4];             //!< Spatial/Temporal wavelet kernels

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
      if (decimal == true)
        {
          if (!isdigit(*parse) && *parse != ' ' && *parse != '.')
            return EXIT_FAILURE;
        }
      else
        {
          if (!isdigit(*parse) && *parse != ' ')
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
          if (arguments->mode != cli_ini)
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
              arguments->in   =  arg;
            }
          break;
        }

      /* Ingest decompression argument.                       */
      case 'D':
        {
          if (arguments->mode != cli_ini)
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
              arguments->in   =  arg;
            }
          break;
        }

      /* Ingest analysis argument.                            */
      case 'A':
        {
          if (arguments->mode != cli_ini)
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
              arguments->in   =  arg;
            }

          break;
        }

      /* Ingest header info argument.                         */
      case 'H':
        {
          if (arguments->mode != cli_ini)
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
              arguments->in   =  arg;
            }

          break;
        }

      /* Ingest output argument.                              */
      case 'O':
        {
          if (arguments->optSet & FLOUT)
            {
              argp_error(state, "The output file can only be defined once.\n");
            }

          if ((arg == NULL) || (arg[0] == '-'))
            {
              argp_error(state, "file names that start with an '-'"
                                " are not supported.\n");
            }
          else
            {
              arguments->out     = arg;
              arguments->optSet |= FLOUT;
            }
          break;
        }

      /* Ingest reference argument.                           */
      case 'R':
        {
          if (arguments->optSet & FLREF)
            {
              argp_error(state, "The reference file can only be defined once.\n");
            }

          if ((arg == NULL) || (arg[0] == '-'))
            {
              argp_error(state, "file names that start with an '-'"
                                " are not supported.\n");
            }
          else
            {
              arguments->ref     = arg;
              arguments->optSet |= FLREF;
            }
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
          if (arguments->optSet & BITRT)
            {
              argp_error(state, "The bitrate can only be defined once.\n");
            }
          else if (arguments->optSet & CMPRT)
            {
              argp_error(state, "The size reduction requires to be specified "
                                "either by a bit rate or a compression ratio, "
                                "never both.\n");
            }

          if (verify_opt(arg, ',', true) == EXIT_FAILURE)
            {
              argp_error(state, "Invalid deliminator in the bitrate option\n");
            }

          for(bitrate = strtod(arg, &end), i = 0; arg != end && i < 10;
              bitrate = strtod(arg, &end), i++)
            {
              arg = end;

              if (bitrate > 0 && bitrate <= 64 && errno != ERANGE)
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
          if (arguments->optSet & CBLKS)
            {
              argp_error(state, "The codeblock size can only be defined once.\n");
            }

          if (verify_opt(arg, ',', false) == EXIT_FAILURE)
            {
              argp_error(state, "Invalid deliminator in the codeblock"
                                " size option.\n");
            }

          for(buff = strtoll(arg, &end, 10), i = 0; arg != end && i < 4;
              buff = strtoll(arg, &end, 10), i++)
            {
              arg = end;

              if (buff >= 0 && buff <= 10 && errno != ERANGE)
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

          if (i == 1)
            {
              arguments->cblkSize[1] =
              arguments->cblkSize[2] =
              arguments->cblkSize[3] = arguments->cblkSize[0];
            }
          else if (i != 4)
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
          if (arguments->optSet & CMPRT)
            {
              argp_error(state, "The compression ratio can only be defined once.\n");
            }
          else if (arguments->optSet & BITRT)
            {
              argp_error(state, "The size reduction requires to be specified "
                                "either by a bit rate or a compression ratio, "
                                "never both.\n");
            }

          if (verify_opt(arg, ',', true) == EXIT_FAILURE)
            {
              argp_error(state, "Invalid deliminator in the compression"
                                " ratio option \n");
            }

          for(compRatio = strtod(arg, &end), i = 0; arg != end && i < 10;
              compRatio = strtod(arg, &end), i++)
            {
              arg = end;

              if (compRatio > 0 && compRatio < 65536 && errno != ERANGE)
                {
                  arguments->rate[i] = (double) compRatio;
                }
              else
                {
                  errno = 0;
                  argp_error(state, "The specified compression ratio (%f) is "
                                    "out of the supported range.\n", compRatio);
                }
            }

          arguments->optSet |= CMPRT;
          break;
        }

      /* Ingest decomposition level.                          */
      case 'd':
        {
          if (arguments->optSet & DCLVL)
            {
              argp_error(state, "The compression ratio can only be defined once.\n");
            }

          if (verify_opt(arg, ',', false) == EXIT_FAILURE)
            {
              argp_error(state, "Invalid deliminator in the decomposition"
                                " level option.\n");
            }

          for(buff = strtoll(arg, &end, 10), i = 0; arg != end && i < 4;
              buff = strtoll(arg, &end, 10), i++)
            {
              arg = end;

              if (buff > 0 && buff <= 63 && errno != ERANGE)
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

          if (i == 1)
            {
              arguments->decompLevel[1] =
              arguments->decompLevel[2] =
              arguments->decompLevel[3] = arguments->decompLevel[0];
            }
          else if (i != 4)
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
          if (arguments->optSet & OMPTH)
            {
              argp_error(state, "The number of OpenMP threads can only be "
                                "defined once.\n");
            }

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
          if (arguments->optSet & USQLY)
            {
              argp_error(state, "The quality layer used for decompression"
                                " can only be defined once.\n");
            }

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
      case 'k':
        {
          if (arguments->optSet & DWTKL)
            {
              argp_error(state, "The wavelet kernels can only be defined once.\n");
            }

          for(token =  strtok_r(arg, ",", &ptr), i = 0;
              token != NULL && i < 4;
              token = strtok_r(NULL, ",", &ptr), i++)
            {
              if (strcasecmp(token, "leGall") == 0)
                arguments->dwtKernel[i] =  bwc_dwt_5_3;
              else if (strcasecmp(token, "CDF") == 0)
                arguments->dwtKernel[i] = bwc_dwt_9_7;
              else if (strcasecmp(token, "Haar") == 0)
                arguments->dwtKernel[i] = bwc_dwt_haar;
              else
                argp_error(state, "Waveket kernel %s is unknown "
                                  "to the library.\n", token);
            }

          if (i == 1)
            {
              arguments->dwtKernel[1] =
              arguments->dwtKernel[2] =
              arguments->dwtKernel[3] = arguments->dwtKernel[0];
            }
          else if (i != 4)
            {
              argp_error(state, "The wavelet kernel argument expects either a "
                                "single global or 4 directional values\n");
            }

          arguments->optSet |= DWTKL;
          break;
        }

      /* Ingest quantization step size.                       */
      case 'q':
        {
          if (arguments->optSet & QTSIZ)
            {
              argp_error(state, "The quantization step size can only be "
                                "defined once.\n");
            }

          qt_step_size = strtod(arg, &end);

          if (qt_step_size > 0 && qt_step_size < 2 && errno != ERANGE)
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
          if (arguments->optSet & QFRMT)
            {
              argp_error(state, "The Q format range can only be defined once.\n");
            }

          buff = strtoll(arg, &end, 10);
          if (buff > 0 && buff < 63 && errno != ERANGE)
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
          if (arguments->optSet & TILES)
            {
              argp_error(state, "The tile size can only be defined once.\n");
            }

          if (verify_opt(arg, ',', false) == EXIT_FAILURE)
            {
              argp_error(state, "Invalid deliminator in the tile"
                                " size option.\n");
            }

          for(buff = strtoll(arg, &end, 10), i = 0; arg != end && i < 4;
              buff = strtoll(arg, &end, 10), i++)
            {
              arg = end;

              if (buff >= 16 && errno != ERANGE)
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

          if (i == 1)
            {
              arguments->tileSize[1] =
              arguments->tileSize[2] =
              arguments->tileSize[3] = arguments->tileSize[0];
            }
          else if (i != 4)
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
          if (arguments->optSet & PRECS)
            {
              argp_error(state, "The precinct size can only be defined once.\n");
            }

          if (verify_opt(arg, ',', false) == EXIT_FAILURE)
            {
              argp_error(state, "Invalid deliminator in the precinct"
                                " size option.\n");
            }

          for(buff = strtoll(arg, &end, 10), i = 0; arg != end && i < 4;
              buff = strtoll(arg, &end, 10), i++)
            {
              arg = end;

              if (buff >= 0 && buff <= 15 && errno != ERANGE)
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

          if (i == 1)
            {
              arguments->precSize[1] =
              arguments->precSize[2] =
              arguments->precSize[3] = arguments->precSize[0];
            }
          else if (i != 4)
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
          if (((arguments->mode == cli_cmp                                                        ||
                arguments->mode == cli_dcp) && (arguments->in == NULL || arguments->ref != NULL)) ||
               (arguments->mode == cli_inf  && (arguments->in == NULL || arguments->ref != NULL
                                                                      || arguments->out != NULL)) ||
               (arguments->mode == cli_anl  && (arguments->in == NULL || arguments->ref == NULL 
                                                                      || arguments->out != NULL)))
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
        {
          if (arguments->mode == cli_ini)
            argp_usage (state);
          break;
        }

      /* Return error if key is unknown.                      */
      default:
        return ARGP_ERR_UNKNOWN;
    }
  return EXIT_SUCCESS;
}

void
printheaderIO(char const *input, char const *output, char const *reference)
{
  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  char                    cli_buffer[1024]  = {0};
  char                    cli_verbose[4096] = {0};

  strcat(cli_verbose, bwc_header_art);
  strcat(cli_verbose, "---------------------------   I/O   --------------------------\n\n");

  sprintf(cli_buffer,"  Input:  %s \n", input);
  strcat(cli_verbose, cli_buffer);
  memset(cli_buffer, '0', sizeof(char) * 1024);

  sprintf(cli_buffer,"  Output: %s \n", output);
  strcat(cli_verbose, cli_buffer);
  memset(cli_buffer, '0', sizeof(char) * 1024);

  sprintf(cli_buffer,"  Reference: %s \n", reference);
  strcat(cli_verbose, cli_buffer);
  memset(cli_buffer, '0', sizeof(char) * 1024);
  printf("%s", cli_verbose);
}

void
printctrl(bwc_gl_ctrl *const control)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint8_t                 i;

  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  char                    cli_buffer[1024]  = {0};
  char                    cli_verbose[4096] = {0};

  strcat(cli_verbose, "\n");
  strcat(cli_verbose, "-----------------   Compression Parameters   -----------------\n");
  strcat(cli_verbose, "\n");

  strcat(cli_verbose,"  Tile Size:                          \n");
  sprintf(cli_buffer,"       - Samples in 1.D:         %27ld\n"\
                     "       - Samples in 2.D:         %27ld\n"\
                     "       - Samples in 3.D:         %27ld\n"\
                     "       - Samples in 4.D:         %27ld\n", control->tileSizeX,
                                                                 control->tileSizeY,
                                                                 control->tileSizeZ,
                                                                 control->tileSizeTS);
  strcat(cli_verbose, cli_buffer);
  memset(cli_buffer, '0', sizeof(char) * 1024);

  strcat(cli_verbose, "  __________________________________________________________\n");
  strcat(cli_verbose, "\n");
  strcat(cli_verbose, "                                       1.D | 2.D | 3.D | 4.D\n");
  sprintf(cli_buffer, "  Decomposition Levels: %18d |%4d |%4d |%4d\n",
                                                control->decompX,
                                                control->decompY,
                                                control->decompZ,
                                                control->decompTS);
  strcat(cli_verbose, cli_buffer);
  memset(cli_buffer, '0', sizeof(char) * 1024);

  sprintf(cli_buffer, "  Precincts  [log2]:    %18d |%4d |%4d |%4d\n",
                                                control->precSizeX,
                                                control->precSizeY,
                                                control->precSizeZ,
                                                control->precSizeTS);
  strcat(cli_verbose, cli_buffer);
  memset(cli_buffer, '0', sizeof(char) * 1024);

  sprintf(cli_buffer, "  Codeblocks [log2]:    %18d |%4d |%4d |%4d\n",
                                                control->cbX,
                                                control->cbY,
                                                control->cbZ,
                                                control->cbTS);
  strcat(cli_verbose, cli_buffer);
  memset(cli_buffer, '0', sizeof(char) * 1024);

  strcat(cli_verbose, "  __________________________________________________________\n");
  strcat(cli_verbose, "\n");

  sprintf(cli_buffer, "  Q Number Format:               %27d\n", control->Qm);
  strcat(cli_verbose, cli_buffer);
  memset(cli_buffer, '0', sizeof(char) * 1024);

  if (control->error_resilience)
    {
      sprintf(cli_buffer, "  Error Resilience:              %27s\n", "true");
    }
  else
    {
      sprintf(cli_buffer, "  Error Resilience:              %27s\n", "false");
    }
  strcat(cli_verbose, cli_buffer);
  memset(cli_buffer, '0', sizeof(char) * 1024);

  strcat(cli_verbose, "  __________________________________________________________\n");
  strcat(cli_verbose, "\n");

  for(i = 0; i < control->nLayers; ++i)
    {
      sprintf(cli_buffer, "  Quality Layer Nr. %d: %33.2f bpd\n", i,
                                      control->bitrate[i]);
      strcat(cli_verbose, cli_buffer);
    }

  memset(cli_buffer, '0', sizeof(char) * 1024);
  strcat(cli_verbose, "\n==============================================================\n");
  printf("%s", cli_verbose);
}

static unsigned char
output_analysis(eas3_data *const ref_data, eas3_data *const org_data)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint64_t            size;
  uint64_t            i;

  uint8_t             p;

  /*-----------------------*\
  ! DEFINE FLOAT VARIABLES: !
  \*-----------------------*/
  double              MSE, PSNR;
  double              peakVal;
  double              sum;

  double              *dOrig, *dRef;
  float               *fOrig, *fRef;

  bwc_float           minVal, maxVal;

  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  eas3_param_names    *param_name;

  dOrig = org_data->field.d;
  dRef  = ref_data->field.d;

  fOrig = org_data->field.f;
  fRef  = ref_data->field.f;

  if(org_data->params.ndim1 == ref_data->params.ndim1 &&
     org_data->params.ndim2 == ref_data->params.ndim2 &&
     org_data->params.ndim3 == ref_data->params.ndim3 &&
     org_data->params.nts   == ref_data->params.nts   &&
     org_data->params.npar  == ref_data->params.npar)
    {
      size = (uint64_t)ref_data->params.ndim1 * ref_data->params.ndim2 *
                       ref_data->params.ndim3 * ref_data->params.nts;

      peakVal = -1.7976931348623157e+308;
      PSNR    =
      MSE     = 0;

      if(ref_data->param_names)
        {
          param_name = ref_data->param_names->root;
          p = 0;
          while(param_name != NULL)
            {
              minVal = 1.7976931348623157e+308;
              maxVal = -1.7976931348623157e+308;

              if(ref_data->params.accuracy == 2)
                {
                  for(i = 0; i < size; ++i)
                    {
                      minVal  = MIN(minVal, (double)dRef[i + p * size]);
                      maxVal  = MAX(maxVal, (double)dRef[i + p * size]);

                      sum     = ((double)dRef[i + p * size] - (double)dOrig[i + p * size]);

                      MSE    += sum * sum;
                    }
                }
              else if(ref_data->params.accuracy == 1)
                {
                  for(i = 0; i < size; ++i)
                    {
                      minVal  = MIN(minVal, (double)fRef[i + p * size]);
                      maxVal  = MAX(maxVal, (double)fRef[i + p * size]);

                      sum     = ((double)fRef[i + p * size] - (double)fOrig[i + p * size]);

                      MSE    += sum * sum;
                    }
                }
              peakVal = MAX(peakVal, maxVal - minVal);
              param_name = param_name->next;
              p++;
            }
        }

      MSE /= (double)size * ref_data->params.npar;
      PSNR = 20 * log10(peakVal/(2 * sqrt(MSE)));

      printf("==============================================================\n");
      printf("  Mean Square Error:             %*.2e\n", 22, MSE);
      printf("  Peak Signal-to-Noise Ratio:    %*.2f\n", 22, PSNR);
      printf("==============================================================\n");

    }

  return EXIT_SUCCESS;
}

/*================================================================================================*/
/**
 * @details Initialize the argp struct. used to parse the command line arguments
 */
/*================================================================================================*/
static struct argp argp = {options, parse_opt, 0, doc, 0, 0, 0};

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
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint64_t                size = 0;
  uint64_t                root, Lfield;
  uint8_t                 i;
  uint8_t                 error_handle = EXIT_SUCCESS;

  /*-----------------------*\
  ! DEFINE REAL VARIABLES:  !
  \*-----------------------*/
  double                  rtype, exp;

  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  char                    rate[200] = {0};

  char                   *cli_output = NULL;
  char                   *buffer = NULL;

  unsigned char          *input  = NULL;
  unsigned char          *output = NULL;

  /*-----------------------*\
  ! DEFINE FILE POINTER:    !
  \*-----------------------*/
  FILE                   *fp = NULL;

  /*-----------------------*\
  ! DEFINE DER. VARIABLES:  !
  \*-----------------------*/
  bwc_precision           precision;
  bwc_header             *header;
  bwc_stream             *stream = NULL;
  bwc_codec              *coder  = NULL;

  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  eas3_data              *data = NULL;
  eas3_data              *ref_data = NULL;
  cli_arguments           arguments = {0};

  /* Parse the command line arguments and invoke the appro- *
   * priate bwccmdl mode.                                   */
  if (argp_parse(&argp, argc, argv, 0, 0, &arguments) == EXIT_FAILURE)
    {
      error_handle = EXIT_FAILURE;
      goto OUT;
    }

  /* Set the number of OpenMP threads.                      */
  #if defined (_OPENMP) 
    if (arguments.optSet & OMPTH)
      omp_set_num_threads((int)arguments.nThreads);
  #endif

  /* Compress the user supplied data set.                   */
  if (arguments.mode == cli_cmp)
    {
      /* Evaluate the appropriate output file.                  */
      if ((arguments.optSet & FLOUT) == 0)
        arguments.out = arguments.in;
      if ((buffer = strrchr(arguments.out, '.')) == NULL)
        {
          error_handle = EXIT_FAILURE;
          printf(TYPERROR);
          goto OUT;
        }
      cli_output = calloc(strlen(arguments.out) - strlen(buffer) + 5, sizeof(char));
      if (cli_output == NULL)
        {
          error_handle = EXIT_FAILURE;
          printf(MEMERROR);
          goto OUT;
        }
      sprintf(cli_output, "%.*s.bwc", (int)(strlen(arguments.out) - strlen(buffer)), arguments.out);

      if (arguments.verbose == true)
        {
          printheaderIO(arguments.in, cli_output, arguments.ref);
        }

      // TODO: Define a universal data structure and implement a reader
      //       that ingests different file formats
      if ((data = read_eas3(arguments.in)) == NULL)
        {
          error_handle = EXIT_FAILURE;
          goto OUT;
        }

      /* Evaluate the input and output buffer size and initial- *
       * size the BigWhoop data and coder structs.              */
      size = data->params.ndim1 * data->params.ndim2 *
             data->params.ndim3 * data->params.nts   *
             data->params.npar;
 
      if (data->params.accuracy == 1)
        {
          precision = bwc_precision_single;
          input     = (unsigned char*)data->field.f;
          output    = calloc(size, sizeof(float));
        }
      else if (data->params.accuracy == 2)
        {
          precision = bwc_precision_double;
          input     = (unsigned char*)data->field.d;
          output    = calloc(size, sizeof(double));
        }

      stream = bwc_init_stream(input, output, comp);
      coder  = bwc_alloc_coder(data->params.ndim1,
                               data->params.ndim2,
                               data->params.ndim3,
                               data->params.nts,
                               data->params.npar,
                               precision);
      bwc_set_aux(stream, (char*)data->aux.ptr, data->aux.len);

      if ((arguments.optSet & TILES) != 0)
        {
          bwc_set_tiles(coder, arguments.tileSize[0],
                               arguments.tileSize[1],
                               arguments.tileSize[2],
                               arguments.tileSize[3], bwc_tile_sizeof);

        }

      // TODO: Implement the kernel setter as a global function
      //       and define the proper verbose output layout.
      /*if ((arguments.optSet & DWTKL) != 0)
        {
          bwc_set_kernels(coder, arguments.dwtKernel[0],
                                 arguments.dwtKernel[1],
                                 arguments.dwtKernel[2],
                                 arguments.dwtKernel[3]);
        }*/

      if ((arguments.optSet & DCLVL) != 0)
        {
          bwc_set_decomp(coder, arguments.decompLevel[0],
                                arguments.decompLevel[1],
                                arguments.decompLevel[2],
                                arguments.decompLevel[3]);
        }

      if ((arguments.optSet & PRECS) != 0)
        {
          bwc_set_precincts(coder, arguments.precSize[0],
                                   arguments.precSize[1],
                                   arguments.precSize[2],
                                   arguments.precSize[3]);
        }

      if ((arguments.optSet & CBLKS) != 0)
        {
          bwc_set_codeblocks(coder, arguments.cblkSize[0],
                                    arguments.cblkSize[1],
                                    arguments.cblkSize[2],
                                    arguments.cblkSize[3]);
        }

      if ((arguments.optSet & QFRMT) != 0)
        {
          bwc_set_qm(coder, arguments.Qm);
        }

      if (arguments.erresilience == true)
        {
          bwc_set_error_resilience(coder);

        }

      /* Initialize the rate control string according to the  *
       * specified bit rate/compression ratio.                */
      if ((arguments.optSet & BITRT) != 0)
        {
          rtype = 1.0;
          exp   = 1.0;
        }
      else if ((arguments.optSet & CMPRT) != 0)
        {
          rtype = 64.0;
          exp   = -1.0;
        }
      else
        {
          rtype = 1.0;
          exp   = 1.0;
          arguments.rate[0] = 64;
        }

      for(i = 0; i < 10 && strlen(rate) < 192; ++i)
        {
          if (arguments.rate[i] > 0)
            {
              sprintf(rate + strlen(rate), "%05.3f,", pow(arguments.rate[i]/rtype, exp));
            }
        }
      rate[strlen(rate) - 1] = '0';

      if (bwc_create_compression(coder, stream, rate) == EXIT_FAILURE)
        {
          error_handle = EXIT_FAILURE;
          goto OUT;
        }

      if(arguments.verbose == true)
        {
          printctrl(&coder->control);
        }

      size = bwc_compress(coder, stream);

      /* Write the codestream to the speicifed file.          */
      fp     = fopen(cli_output, "wb");
      if (fp == NULL)
        {
          error_handle = EXIT_FAILURE;
          printf(FOUERROR);
          goto OUT;
        }

      if (fwrite(stream->out, sizeof(unsigned char), size, fp) != size)
        {
          error_handle = EXIT_FAILURE;
          printf(WRTERROR);
          goto OUT;
        }
    }
  else if (arguments.mode == cli_dcp)
    {
      /* Evaluate the appropriate output file.                  */
      if ((arguments.optSet & FLOUT) == 0)
        arguments.out = arguments.in;
      if ((buffer = strrchr(arguments.out, '.')) == NULL)
        {
          error_handle = EXIT_FAILURE;
          printf(TYPERROR);
          goto OUT;
        }
      cli_output = calloc(strlen(arguments.out) - strlen(buffer) + 5, sizeof(char));
      if (cli_output == NULL)
        {
          error_handle = EXIT_FAILURE;
          printf(MEMERROR);
          goto OUT;
        }
      sprintf(cli_output, "%.*s.eas", (int)(strlen(arguments.out) - strlen(buffer)), arguments.out);

      if (arguments.verbose == true)
        {
          printheaderIO(arguments.in, cli_output, arguments.ref);
        }

      if ((fp = fopen(arguments.in, "r")) == NULL)
        {
          error_handle = EXIT_FAILURE;
          printf(FINERROR);
          goto OUT;
        }

      root   = ftell(fp);
      fseek(fp, 0L,   SEEK_END);
      Lfield = ftell(fp) - root;
      fseek(fp, root, SEEK_SET);

      /* Read the compressed data from the input file. */
      input = calloc(Lfield, sizeof(unsigned char));
      if (fread(input, sizeof(unsigned char), Lfield, fp) != Lfield)
        {
          error_handle = EXIT_FAILURE;
          printf(RDERROR);
          goto OUT;
        }

      /* Retrieve header information and allocate output buffer. */
      header = bwc_open_header(input);
      size = header->info.nX * header->info.nY * header->info.nZ *
             header->info.nTS * header->info.nPar;
      if(header->info.data_prec == bwc_precision_double)
        {
          output = calloc(size, sizeof(double));
        }
      else if(header->info.data_prec == bwc_precision_single)
        {
          output = calloc(size, sizeof(float));
        }
      bwc_close_header(header);

      printctrl(&header->control);

      /* Initialize and run the decompression. */
      stream = bwc_init_stream(input, output, comp);
      coder  = bwc_alloc_decoder();
      if (bwc_create_decompression(coder, stream, 0) == EXIT_FAILURE)
        {
          error_handle = EXIT_FAILURE;
          goto OUT;
        }
      bwc_decompress(coder, stream);

      /* Parse decompressed data into eas3 data structure *
       * and write to the output file.                    */
      data = calloc(1, sizeof(eas3_data));
      bwc_to_eas3(stream, data);
      write_eas3(data, cli_output);
      
      goto OUT;
    }
  else if (arguments.mode == cli_anl)
    {
      if (arguments.verbose == true)
        {
          printheaderIO(arguments.in, cli_output, arguments.ref);
        }

      /* Ingest the reference data input. */
      if ((ref_data = read_eas3(arguments.ref)) == NULL)
        {
          error_handle = EXIT_FAILURE;
          goto OUT;
        }

      /* Ingest the compressed data input. */
      if ((fp = fopen(arguments.in, "r")) == NULL)
        {
          error_handle = EXIT_FAILURE;
          printf(FINERROR);
          goto OUT;
        }

      root   = ftell(fp);
      fseek(fp, 0L,   SEEK_END);
      Lfield = ftell(fp) - root;
      fseek(fp, root, SEEK_SET);

      /* Read the compressed data from the input file. */
      input = calloc(Lfield, sizeof(unsigned char));
      if (fread(input, sizeof(unsigned char), Lfield, fp) != Lfield)
        {
          error_handle = EXIT_FAILURE;
          printf(RDERROR);
          goto OUT;
        }

      /* Retrieve header information and allocate output buffer. */
      if((header = bwc_open_header(input)) == NULL)
      {
        error_handle = EXIT_FAILURE;
        goto OUT;
      }
      size = header->info.nX * header->info.nY * header->info.nZ *
             header->info.nTS * header->info.nPar;
      if(header->info.data_prec == bwc_precision_double)
        {
          output = calloc(size, sizeof(double));
        }
      else if(header->info.data_prec == bwc_precision_single)
        {
          output = calloc(size, sizeof(float));
        }
      bwc_close_header(header);

      if(arguments.verbose == true)
        {
          printctrl(&header->control);
        }

      /* Initialize and run the decompression. */
      stream = bwc_init_stream(input, output, comp);
      coder  = bwc_alloc_decoder();
      if (bwc_create_decompression(coder, stream, 0) == EXIT_FAILURE)
        {
          error_handle = EXIT_FAILURE;
          goto OUT;
        }
      bwc_decompress(coder, stream);

      /* Parse decompressed data into eas3 data structure *
       * and write to the output file.                    */
      data = calloc(1, sizeof(eas3_data));
      bwc_to_eas3(stream, data);

      output_analysis(ref_data, data);

      goto OUT;
    }
  else if (arguments.mode == cli_inf)
    {
      printheaderIO(arguments.in, cli_output, arguments.ref);

      if ((fp = fopen(arguments.in, "r")) == NULL)
        {
          error_handle = EXIT_FAILURE;
          printf(FINERROR);
          goto OUT;
        }

      root   = ftell(fp);
      fseek(fp, 0L,   SEEK_END); 
      Lfield = ftell(fp) - root;
      fseek(fp, root, SEEK_SET);

      /* Read the compressed data from the input file. */
      input = calloc(Lfield, sizeof(unsigned char));
      if (fread(input, sizeof(unsigned char), Lfield, fp) != Lfield)
        {
          error_handle = EXIT_FAILURE;
          printf(RDERROR);
          goto OUT;
        }

      /* Retrieve header information. */
      header = bwc_open_header(input);
      size = header->info.nX * header->info.nY * header->info.nZ *
             header->info.nTS * header->info.nPar;
      if(header->info.data_prec == bwc_precision_double)
        {
          output = calloc(size, sizeof(double));
        }
      else if(header->info.data_prec == bwc_precision_single)
        {
          output = calloc(size, sizeof(float));
        }
      bwc_close_header(header);

      printctrl(&header->control);
    }
  else
    {
      return EXIT_FAILURE;
    }

OUT:
  if (coder != NULL)
    bwc_free_codec(coder);

  if (data != NULL)
    eas3_free_data(data);

  if (ref_data != NULL)
    eas3_free_data(ref_data);

  if (stream !=NULL)
    free(stream);

  if (output != NULL)
    free(output);

  if (cli_output != NULL)
    free(cli_output);

  if (fp != NULL)
    fclose(fp);

  return error_handle;
}
