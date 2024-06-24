/*====================================================================================================================*\
||                                                                                                                    ||
||                 /$$$$$$$  /$$                  /$$      /$$ /$$                                                    ||
||                | $$__  $$|__/                 | $$  /$ | $$| $$                                                    ||
||                | $$  \ $$ /$$  /$$$$$$        | $$ /$$$| $$| $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$                 ||
||                | $$$$$$$ | $$ /$$__  $$       | $$/$$ $$ $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$                ||
||                | $$__  $$| $$| $$  \ $$       | $$$$_  $$$$| $$  \ $$| $$  \ $$| $$  \ $$| $$  \ $$                ||
||                | $$  \ $$| $$| $$  | $$       | $$$/ \  $$$| $$  | $$| $$  | $$| $$  | $$| $$  | $$                ||
||                | $$$$$$$/| $$|  $$$$$$$       | $$/   \  $$| $$  | $$|  $$$$$$/|  $$$$$$/| $$$$$$$/                ||
||                |_______/ |__/ \____  $$       |__/     \__/|__/  |__/ \______/  \______/ | $$____/                 ||
||                               /$$  \ $$                                                  | $$                      ||
||                              |  $$$$$$/                                                  | $$                      ||
||                               \______/                                                   |__/                      ||
||                                                                                                                    ||
||      DESCRIPTION:                                                                                                  ||
||      ------------                                                                                                  ||
||                   This is a simple command line tool that uses the Big Whoop library to (de)com-                   ||
||                   press a 2- to 4-dimensional IEEE 754 floating point array. For further infor-                    ||
||                   mation use the --help (-h) argument in the command-line or consult the appro-                    ||
||                   priate README file.                                                                              ||
||                                                                                                                    ||
||      FILE REFERENCES:                                                                                              ||
||      ----------------                                                                                              ||
||                                                                                                                    ||
||                   Name                  I/O                       Description                                      ||
||                   ----                  ---                       -----------                                      ||
||                 - input               - input                   - Input file that corresponds to                   ||
||                                                                   an uncompressed dataset, for a                   ||
||                                                                   compression run, or a com-                       ||
||                                                                   pressed bitstream, for a decom-                  ||
||                                                                   pression run.                                    ||
||                                                                                                                    ||
||                 - output              - Output                  - Output file that the com-                        ||
||                                                                   pressed bitstream, for a com-                    ||
||                                                                   pression run, or reconstructed                   ||
||                                                                   dataset, for a decompression                     ||
||                                                                   run, is written to.                              ||
||                                                                                                                    ||
||      FUNCTIONS:                                                                                                    ||
||      ----------                                                                                                    ||
||                                                                                                                    ||
||                   PRIVATE:                                        PUBLIC:                                          ||
||                   --------                                        -------                                          ||
||                 - get_digit_sep                                 - main                                             ||
||                 - get_size                                                                                         ||
||                 - get_dimension                                                                                    ||
||                 - get_prog_ord                                                                                     ||
||                 - get_quant_style                                                                                  ||
||                                                                                                                    ||
||      DEVELOPMENT HISTORY:                                                                                          ||
||      --------------------                                                                                          ||
||                                                                                                                    ||
||                   Date        Author             Change Id   Release     Description Of Change                     ||
||                   ----        ------             ---------   -------     ---------------------                     ||
||                   13.10.2017  Patrick Vogler     B87D120     V 0.1.0     source file created                       ||
||                   26.11.2020  Patrick Vogler     B87E7E4     V 0.1.0     Command line tool refac-                  ||
||                                                                          tored.                                    ||
||                                                                                                                    ||
||                                                                                                                    ||
||       ------------------------------------------------------------------------------------------------------       ||
||                                                                                                                    ||
||       Copyright (c) 2023, High Performance Computing Center - University of Stuttgart                              ||
||                                                                                                                    ||
||       Redistribution and use in source and binary forms, with or without modification, are permitted               ||
||       provided that the following conditions are met:                                                              ||
||                                                                                                                    ||
||          (1)   Redistributions of source code must retain the above copyright notice, this list of                 ||
||                conditions and the following disclaimer.                                                            ||
||                                                                                                                    ||
||          (2)   Redistributions in binary form must reproduce the above copyright notice, this list                 ||
||                of conditions and the following disclaimer in the documentation and/or other materials              ||
||                provided with the distribution.                                                                     ||
||                                                                                                                    ||
||       THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED       ||
||       WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A       ||
||       PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR       ||
||       ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT           ||
||       LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS          ||
||       INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR       ||
||       TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF         ||
||       ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                   ||
||                                                                                                                    ||
\*====================================================================================================================*/
/**********************************************************************************************************************\
||                                          _ _  _ ____ _    _  _ ___  ____                                           ||
||                                          | |\ | |    |    |  | |  \ |___                                           ||
||                                          | | \| |___ |___ |__| |__/ |___                                           ||
||                                                                                                                    ||
\**********************************************************************************************************************/
#include <assert.h>
#include <bwc.h>
#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include "bwccmdl.h"
#include "eas3.h"

/**********************************************************************************************************************\
||                    ____ _    ____ ___  ____ _       ____ ____ _  _ ____ ___ ____ _  _ ___ ____                     ||
||                    | __ |    |  | |__] |__| |       |    |  | |\ | [__   |  |__| |\ |  |  [__                      ||
||                    |__] |___ |__| |__] |  | |___    |___ |__| | \| ___]  |  |  | | \|  |  ___]                     ||
||                                                                                                                    ||
\**********************************************************************************************************************/
/*--------------------------------------------------------------------------------------------------------------------*\
!                                                                                                                      !
!   DESCRIPTION:                                                                                                       !
!   ------------                                                                                                       !
!                    This array defines the arguments supported by the bwc command line tool. Each                     !
!                    argument follows the following structure:                                                         !
!                                                                                                                      !
!                        | flag | long name | short name | opt | type | example | description |                        !
!                                                                                                                      !
!                    The flag signals if the argument is active, while the opt and type string def-                    !
!                    ine the argument type and wether it is optional. Finally, the example string                      !
!                    is 24 characters long and highlights how the argument is to be used while the                     !
!                    description should be no longer than 1024 and contain the argument description.                   !
!                    Additionally, the description needs to be subdivided into blocks of 50 charac-                    !
!                    ters for formating.                                                                               !
!                                                                                                                      !
!   DEVELOPMENT HISTORY:                                                                                               !
!   --------------------                                                                                               !
!                                                                                                                      !
!                    Date        Author             Change Id   Release     Description                                !
!                    ----        ------             ---------   -------     -----------                                !
!                    09.07.2019  Patrick Vogler     B87D120     V 0.1.0     Constant created.                          !
!                    03.05.2021  Patrick Vogler     B87E7E4     V 0.1.0     Description updated.                       !
!                                                                                                                      !
\*--------------------------------------------------------------------------------------------------------------------*/
static bwc_cmdl_args CMDL_ARGS[] = {
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "compress",                 "c",  "arg",    "str1",   "-c",                       "Compress a numerical dataset."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "decompress",               "d",  "arg",    "str1",   "-d",                       "Decompress a BigWhoop file."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "analysis",                 "al", "arg",    "lit",    "-al",                      "Analyze the Peak Signal to Noise Ratio (PSNR) and "
                                                                                        "Mean Square Error (MSE) of a decompressed dataset."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "info",                     "if", "arg",    "str1",   "-if",                      "Display the header information of a BigWhoop file."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "help",                     "h",  "arg",    "lit",    "-h",                       "Print this help message and exit."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "input",                    "i",  "arg",    "str1",   "-i /path/to/input",        "Valid path to a file that can be parsed/read by   "
                                                                                        "this command line tool. For a full list of the    "
                                                                                        "supported file extension see the bottom of this   "
                                                                                        "message."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "output",                   "o",  "arg",    "str1",   "-o /path/to/output",       "Valid path and filename that defines the output of"
                                                                                        "the (de)compressed dataset. If not defined, the   "
                                                                                        "[input] argument is used to generate the (de)com- "
                                                                                        "pressed output."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "reference",                "rf", "arg",    "str1",   "-rf /path/to/output",      "Valid path to a file that is used as a reference  "
                                                                                        "for the [analysis] option."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "verbose",                  "v",  "opt",    "num1",   "-v *",                     "Displays miscellaneous information. This option   "
                                                                                        "accepts the value 0 (default) for compression sta-"
                                                                                        "tistics and 1 for the applied compression param.  "},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "bitrate",                  "b",  "opt",    "numl",   "-b  *, *, *...",           "Defines the quality layers present in the code-   "
                                                                                        "stream as a function of the average bits per data "
                                                                                        "point. This option accepts real numbers in the    "
                                                                                        "range of 0 < * < 64."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "codeblock_size",           "cb", "opt",    "num4",   "-cb x=* y=* z=* ts=*",     "Defines the codeblocks size for all dimensions and"
                                                                                        "timesteps. This option accepts natural numbers in "
                                                                                        "log2 format in the range of 1 <= * <= 10. The sum "
                                                                                        "of all values has to lie within the range of      "
                                                                                        "4 < sum* < 20."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "compression_ratio",        "cr", "opt",    "num1",   "-cr *",                    "Defines the ratio between the uncompresssed and   "
                                                                                        "uncompressed file size. This option accepts all   "
                                                                                        "positive real numbers."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "decomposition_levels",     "dl", "opt",    "num4",   "-dl x=* y=* z=* ts=*",     "Deefines the number of wavelet decompositions ap- "
                                                                                        "plied to all dimensions and timesteps. This option"
                                                                                        "accepts natural numbers in the range of 1 <= * <= "
                                                                                        "63."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "number_of_threads",        "n",  "opt",    "num1",   "-n *",                     "Defines the number of OpenMP threads used to (de) "
                                                                                        "compress the [input] file. This option accepts    "
                                                                                        "natural numbers in the range of 1 <= * <= 255.    "},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "layer",                    "l",  "opt",    "num1",   "-l *",                     "Defines the quality layer used to generate the nu-"
                                                                                        "merical dataset from the bwc codestream. This op- "
                                                                                        "tion accepts natural numbers in the range of 0 <  "
                                                                                        "* <= number_of_quality_layers."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{0,   "wavelet_kernels",          "k",  "opt",    "str4",   "-k  x=* y=* z=* ts=*",     "Ddefines the wavelet kernels applied to all dimen-"
                                                                                        "sions and timesteps. This option accepts the      "
                                                                                        "strings CDF, LeGall and Haar."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{0,   "quantisation_step_size",   "q",  "opt",    "num1",   "-q  *",                    "Defines the step size of the quantization applied "
                                                                                        "to the wavelet coefficients. This option accepts  "
                                                                                        "real numbers in the range of 0 < * < 2."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "q_format_range",           "qm", "opt",    "num1",   "-qm *",                    "Defines the Q format's number of fractional bits  "
                                                                                        "used to transform the floating to a fixed point   "
                                                                                        "format. This option accepts natural numbers in the"
                                                                                        "range of 1 <= * <= 62."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "error_resilience",         "r",  "opt",    "lit",    "-r",                       "The [error_resilience] option is used to place    "
                                                                                        "special markers in the compressed bitstream that  "
                                                                                        "help with error detection and limit error propaga-"
                                                                                        "tion."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "tile_size",                "t",  "opt",    "num4",   "-t  x=* y=* z=* ts=*",     "Defines the tile size for all dimensions and time-"
                                                                                        "steps of a tile. This option accepts natural num- "
                                                                                        "bers in the range of 16 <= * <= domain size."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{1,   "precinct_size",            "p",  "opt",    "num4",   "-p  x=* y=* z=* ts=*",     "Defines the precinct size for all dimensions and  "
                                                                                        "timesteps. This option accepts natural numbers in "
                                                                                        "log2 format in the range of 1 <= * <= 15."},
//----|---------------------------|-----|---------|---------|---------------------------|--------------------------------------------------|
{0,   "",                         "",   "end",    "",       "",                         ""}};

/**********************************************************************************************************************\
||                     ___  ____ _ _  _ ____ ___ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                     ||
||                     |__] |__/ | |  | |__|  |  |___    |___ |  | |\ | |     |  | |  | |\ | [__                      ||
||                     |    |  \ |  \/  |  |  |  |___    |    |__| | \| |___  |  | |__| | \| ___]                     ||
||                                                                                                                    ||
\**********************************************************************************************************************/
/*----------------------------------------------------------------------------------------------------------*\
!                                                                                                            !
!   DESCRIPTION:                                                                                             !
!   ------------                                                                                             !
!                    This function converts the endianess of half, single or double precision values.        !
!                                                                                                            !
!   PARAMETERS:                                                                                              !
!   -----------                                                                                              !
!                    Variable                 Type                   Description                             !
!                    --------                 ----                   -----------                             !
!                    value                    void*                - Memory address of the parame-           !
!                                                                    ter to be converted.                    !
!                                                                                                            !
!   RETURN VALUE:                                                                                            !
!   -------------                                                                                            !
!                    Type                                            Description                             !
!                    ----                                            -----------                             !
!                    -                                               -                                       !
!                                                                                                            !
!   DEVELOPMENT HISTORY:                                                                                     !
!   --------------------                                                                                     !
!                                                                                                            !
!                    Date        Author             Change Id   Release     Description                      !
!                    ----        ------             ---------   -------     -----------                      !
!                    30.04.2019  Patrick Vogler     B87D120     V 0.1.0     function created                 !
!                    21.11.2019  Patrick Vogler     B87E7E4     V 0.1.0     functionality expanded           !
!                                                                           to 32 bit integers               !
!                    21.11.2019  Patrick Vogler     B87E7E4     V 0.1.0     functionality expanded           !
!                                                                           to 16 bit integers               !
!                                                                                                            !
\*----------------------------------------------------------------------------------------------------------*/
static void
endian_conversion(void          *value,
                  uint8_t const  accuracy)
{
  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(value);

  switch(accuracy)
    {
      case 2:
        {
          uint16_t *tmp  = (uint16_t*)value;

          *tmp           = (uint16_t)( *tmp << 8) | 
                           (uint16_t)( *tmp >> 8);
          break;
        }

      case 4:
        {
          uint32_t *tmp  = (uint32_t*)value;

          *tmp           = (uint32_t)((*tmp << 8) & 0xFF00FF00) |
                           (uint32_t)((*tmp >> 8) & 0x00FF00FF);

          *tmp           = (uint32_t)( *tmp << 16)              | 
                           (uint32_t)( *tmp >> 16);
          break;
        }

      case 8:
        {
          uint64_t *tmp  = (uint64_t*)value;

          *tmp           = (uint64_t)((*tmp << 8)  & 0xFF00FF00FF00FF00ULL) | 
                           (uint64_t)((*tmp >> 8)  & 0x00FF00FF00FF00FFULL);

          *tmp           = (uint64_t)((*tmp << 16) & 0xFFFF0000FFFF0000ULL) | 
                           (uint64_t)((*tmp >> 16) & 0x0000FFFF0000FFFFULL);

          *tmp           = (uint64_t)( *tmp << 32)                          | 
                           (uint64_t)( *tmp >> 32);
          break;
        }
      default:
        {
          break;
        }
    }
}

/*--------------------------------------------------------------------------------------------------------------------*\
!                                                                                                                      !
!   DESCRIPTION:                                                                                                       !
!   ------------                                                                                                       !
!                    This function takes a command line argument, checks if it is hyphenated and,                      !
!                    if true, returns a pointer to the first valid character in the string.                            !
!                                                                                                                      !
!   PARAMETERS:                                                                                                        !
!   -----------                                                                                                        !
!                    Variable                 Type                   Description                                       !
!                    --------                 ----                   -----------                                       !
!                    str                      char*                - String containing a command                       !
!                                                                    line argument.                                    !
!                                                                                                                      !
!   RETURN VALUE:                                                                                                      !
!   -------------                                                                                                      !
!                    Type                                            Description                                       !
!                    ----                                            -----------                                       !
!                    char*                                         - Pointer to a hyphenated com-                      !
!                                                                    mand line argument.                               !
!                                                                                                                      !
!   DEVELOPMENT HISTORY:                                                                                               !
!   --------------------                                                                                               !
!                                                                                                                      !
!                    Date        Author             Change Id   Release     Description                                !
!                    ----        ------             ---------   -------     -----------                                !
!                    03.05.2021  Patrick Vogler     B87D120     V 0.1.0     function created                           !
!                    04.05.2021  Patrick Vogler     B87E7E4     V 0.1.0     clean up                                   !
!                                                                                                                      !
\*--------------------------------------------------------------------------------------------------------------------*/
static char* 
get_opt(char* str)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  size_t trim;

  trim = strspn(str, " ");

  if((trim < strlen(str)) && (str[trim] == '-'))
    return &str[trim];
  else
    return NULL;
}

/*--------------------------------------------------------------------------------------------------------------------*\
!                                                                                                                      !
!   DESCRIPTION:                                                                                                       !
!   ------------                                                                                                       !
!                    This function takes an integer value, generates a version with proper digital                     !
!                    group separators and returns the string to the function caller.                                   !
!                                                                                                                      !
!   PARAMETERS:                                                                                                        !
!   -----------                                                                                                        !
!                    Variable                 Type                   Description                                       !
!                    --------                 ----                   -----------                                       !
!                    integer                  unsigned int(64 bit) - Integer value.                                    !
!                                                                                                                      !
!   RETURN VALUE:                                                                                                      !
!   -------------                                                                                                      !
!                    Type                                            Description                                       !
!                    ----                                            -----------                                       !
!                    char*                                         - Character array containing the                    !
!                                                                    group seperated integer value.                    !
!                                                                                                                      !
!   DEVELOPMENT HISTORY:                                                                                               !
!   --------------------                                                                                               !
!                                                                                                                      !
!                    Date        Author             Change Id   Release     Description                                !
!                    ----        ------             ---------   -------     -----------                                !
!                    03.02.2020  Patrick Vogler     B87D120     V 0.1.0     function created                           !
!                    04.05.2021  Patrick Vogler     B87E7E4     V 0.1.0     clean up                                   !
!                                                                                                                      !
\*--------------------------------------------------------------------------------------------------------------------*/
static char*
get_digit_sep(uint64_t integer)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint64_t    multiplier;
  uint16_t    buffI;
  uint8_t     length, shift;

  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  char        buffC[7] = {0};
  char       *str;

  /*--------------------------------------------------------*\
  ! Calculate the decimal power of the integer value, esti-  !
  ! mate the length of the group seperated string and alloc- !
  ! cate the corresponding character array.                  !
  \*--------------------------------------------------------*/
  shift   = (uint8_t)log10(integer);
  length  = (uint8_t)floor(shift/3.0f) + shift + 2;

  str     = calloc(length, sizeof(char));
  if(str == NULL)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return NULL;
   }

  /*--------------------------------------------------------*\
  ! Iterate over the integer value and assemble the group    !
  ! serparated string.                                       !
  \*--------------------------------------------------------*/
  do
    {
      /*--------------------------------------------------------*\
      ! If the first digits do not form a group, decrement the   !
      ! shift to the next group, extract the first digits from   !
      ! the integer and write them to the string.                !
      \*--------------------------------------------------------*/
      if(shift % 3 || shift == 0)
        {
          shift      -= (shift % 3);
          multiplier  = (uint64_t)pow(10, shift);
          buffI       = integer/multiplier;
          integer    -= (uint64_t)buffI * multiplier;

          sprintf(buffC, "%d", buffI);
          strcat(str, buffC);
          memset(buffC, '\0', 5);
        }
      /*--------------------------------------------------------*\
      ! If the next digits form a group, decrement the decimal   !
      ! shift, extract digits from the integer and write them    !
      ! to the string with the proper separator.                 !
      \*--------------------------------------------------------*/
      else
        {
          shift      -= 3;
          multiplier  = (uint64_t)pow(10, shift);
          buffI       = integer/multiplier;
          integer    -= (uint64_t)buffI * multiplier;
          if(strlen(str))
            {
              sprintf(buffC, ".%03d", buffI);
            }
          else
            {
              sprintf(buffC, "%03d", buffI);
            }
          strcat(str, buffC);
          memset(buffC, '\0', 5);
        }
    } while(shift);
  return str;
}

/*--------------------------------------------------------------------------------------------------------------------*\
!                                                                                                                      !
!   DESCRIPTION:                                                                                                       !
!   ------------                                                                                                       !
!                    This function takes an integer value and generates a version with the appropri-                   !
!                    ate byte unit in log2 format that is returned to the function caller.                             !
!                                                                                                                      !
!   PARAMETERS:                                                                                                        !
!   -----------                                                                                                        !
!                    Variable                 Type                   Description                                       !
!                    --------                 ----                   -----------                                       !
!                    integer                  unsigned int(64 bit) - Integer value.                                    !
!                                                                                                                      !
!   RETURN VALUE:                                                                                                      !
!   -------------                                                                                                      !
!                    Type                                            Description                                       !
!                    ----                                            -----------                                       !
!                    char*                                         - Character array containing                        !
!                                                                    the group seperated string.                       !
!                                                                                                                      !
!   DEVELOPMENT HISTORY:                                                                                               !
!   --------------------                                                                                               !
!                                                                                                                      !
!                    Date        Author             Change Id   Release     Description                                !
!                    ----        ------             ---------   -------     -----------                                !
!                    03.05.2019  Patrick Vogler     B87D120     V 0.1.0     function created                           !
!                    04.05.2021  Patrick Vogler     B87E7E4     V 0.1.0     clean up                                   !
!                                                                                                                      !
\*--------------------------------------------------------------------------------------------------------------------*/
static char*
get_size(uint64_t integer)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint64_t    multiplier;
  uint8_t     i;

  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  char       *sizes[] = { "EiB", "PiB", "TiB", "GiB", "MiB", "KiB", "B" };
  char       *str;

  /*--------------------------------------------------------*\
  ! Set up the multiplier used to evaluate the digital unit  !
  ! prefix and allocate the character array returned to the  !
  ! function caller.                                         !
  \*--------------------------------------------------------*/
  multiplier  =  1024ULL * 1024ULL * 1024ULL *
                 1024ULL * 1024ULL * 1024ULL;

  str     = calloc(10, sizeof(char));
  if(str == NULL)
   {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return NULL;
   }

  /*--------------------------------------------------------*\
  ! If larger than 0, iterate over the byte units until the  !
  ! integer is within its range and populate the char. array !
  ! with the appropriate information.                        !
  \*--------------------------------------------------------*/
  if(integer > 0)
    {
      for(i = 0; i < 7; ++i, multiplier /= 1024)
        {
          if(integer < multiplier)
            continue;
          if(integer % multiplier == 0)
            sprintf(str, "%" PRIu64 " %s", integer / multiplier, sizes[i]);
          else
            sprintf(str, "%.1f %s", floor((10.0 * integer) / multiplier) / 10.0, sizes[i]);
          break;
        }
    }
  else
    {
      strcpy(str, "0 B");
    }
  return str;
}


/*--------------------------------------------------------------------------------------------------------------------*\
!                                                                                                                      !
!   DESCRIPTION:                                                                                                       !
!   ------------                                                                                                       !
!                    This function is a variant of the DJB hash function that takes a string, con-                     !
!                    verts it to uppercase and returns the appropriate hash.                                           !
!                                                                                                                      !
!   PARAMETERS:                                                                                                        !
!   -----------                                                                                                        !
!                    Variable                 Type                   Description                                       !
!                    --------                 ----                   -----------                                       !
!                    str                      char*                - String used to generate a u-                      !
!                                                                    niquely identifiable hash.                        !
!                                                                                                                      !
!   RETURN VALUE:                                                                                                      !
!   -------------                                                                                                      !
!                    Type                                            Description                                       !
!                    ----                                            -----------                                       !
!                    uint64_t                                      - Uniquely identifiable hash.                       !
!                                                                                                                      !
!   DEVELOPMENT HISTORY:                                                                                               !
!   --------------------                                                                                               !
!                                                                                                                      !
!                    Date        Author             Change Id   Release     Description                                !
!                    ----        ------             ---------   -------     -----------                                !
!                    17.04.2019  Patrick Vogler     B87D120     V 0.1.0     function created                           !
!                    04.05.2021  Patrick Vogler     B87E7E4     V 0.1.0     clean up                                   !
!                                                                                                                      !
\*--------------------------------------------------------------------------------------------------------------------*/
static uint64_t
hash(char* str) 
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint64_t hash;  
  uint8_t  c;

  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(str);

  /*--------------------------------------------------------*\
  ! Initialize the hash with a magic number.                 !
  \*--------------------------------------------------------*/
  hash = 0x1505;

  /*--------------------------------------------------------*\
  ! Walk through all the characters in the string, convert   !
  ! them to uppercase and use them to generate the hash.     !
  \*--------------------------------------------------------*/
  while((c = *str++))
    {
      if((c >= 97) && 
         (c <= 122))
        {
          c = c - 32;
        }

      hash = (hash * 33) ^ c;
    }

  /*--------------------------------------------------------*\
  ! Return the hash.                                         !
  \*--------------------------------------------------------*/
  return hash;
}

/*--------------------------------------------------------------------------------------------------------------------*\
!                                                                                                                      !
!   DESCRIPTION:                                                                                                       !
!   ------------                                                                                                       !
!                    This function deallocates a util_arg_node linked list that is used to store                       !
!                    the bwc command line arguments.                                                                   !
!                                                                                                                      !
!   PARAMETERS:                                                                                                        !
!   -----------                                                                                                        !
!                    Variable                 Type                   Description                                       !
!                    --------                 ----                   -----------                                       !
!                    args                     util_arg_node*       - Arguments/options linked list                     !
!                                                                    for the bwc command-line tool.                    !
!                                                                                                                      !
!   RETURN VALUE:                                                                                                      !
!   -------------                                                                                                      !
!                    Type                                            Description                                       !
!                    ----                                            -----------                                       !
!                    -                                               -                                                 !
!                                                                                                                      !
!   DEVELOPMENT HISTORY:                                                                                               !
!   --------------------                                                                                               !
!                                                                                                                      !
!                    Date        Author             Change Id   Release     Description                                !
!                    ----        ------             ---------   -------     -----------                                !
!                    24.04.2019  Patrick Vogler     B87D120     V 0.1.0     Function created                           !
!                    04.05.2021  Patrick Vogler     B87E7E4     V 0.1.0     clean up                                   !
!                                                                                                                      !
\*--------------------------------------------------------------------------------------------------------------------*/
static void
bwc_kill_arg(bwc_cmdl_arg_node *args)
{
  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  bwc_cmdl_arg_node *temp;
  
  if(args != NULL)
    {
      /*--------------------------------------------------------*\
      ! Reset the arguments linked list to its root node.        !
      \*--------------------------------------------------------*/
      args = args->root;

      /*--------------------------------------------------------*\
      ! Walk through the arguments linked list and deallocate    !
      ! the lit_opt, num_ opt and util_arg_node structure.       !
      \*--------------------------------------------------------*/
      while(args != NULL)
        {
          temp = args;
          args = args->next;

          free(temp->lit_opt);
          free(temp->num_opt);
          free(temp);
        }
    }
}

/*--------------------------------------------------------------------------------------------------------------------*\
!                                                                                                                      !
!   DESCRIPTION:                                                                                                       !
!   ------------                                                                                                       !
!                    This function is used to parse the command-line arguments, check if they are                      !
!                    valid arguments and options for the bwc command-line tool and save them in a                      !
!                    linked list.                                                                                      !
!                                                                                                                      !
!   PARAMETERS:                                                                                                        !
!   -----------                                                                                                        !
!                    Variable                 Type                   Description                                       !
!                    --------                 ----                   -----------                                       !
!                    argc                     int                  - Argument count                                    !
!                                                                                                                      !
!                    argv                     char**               - Argument vector                                   !
!                                                                                                                      !
!   RETURN VALUE:                                                                                                      !
!   -------------                                                                                                      !
!                    Type                                            Description                                       !
!                    ----                                            -----------                                       !
!                    util_arg_node*                                - Arguments/options linked list                     !
!                                                                    for the bwc command-line tool.                    !
!                                                                                                                      !
!   DEVELOPMENT HISTORY:                                                                                               !
!   --------------------                                                                                               !
!                                                                                                                      !
!                    Date        Author             Change Id   Release     Description                                !
!                    ----        ------             ---------   -------     -----------                                !
!                    17.04.2019  Patrick Vogler     B87D120     V 0.1.0     function created                           !
!                    04.05.2021  Patrick Vogler     B87E7E4     V 0.1.0     clean up                                   !
!                                                                                                                      !
\*--------------------------------------------------------------------------------------------------------------------*/
static bwc_cmdl_arg_node*
parse_arguments(int     argc, 
                char  **argv)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint64_t            buf;
  uint64_t           *hash_list;
  uint64_t           *tmp;

  uint16_t            l, lsiz;

  uint8_t             i, j;

  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  char               *token, *ptr;
  char               *str;

  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  bwc_cmdl_arg_node  *args;

  /*--------------------------------------------------------*\
  ! Allocate an integer array used to store the hash values  !
  ! for all active command line arguments and options.       !
  \*--------------------------------------------------------*/
  hash_list     = calloc(40, sizeof(uint64_t));
  if(hash_list == NULL)
    {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return NULL;
    }

  /*--------------------------------------------------------*\
  ! Loop through the command line argument list and store    !
  ! the hash value of the long and short name for all active !
  ! command line arguments in the hash list.                 !
  \*--------------------------------------------------------*/
  for(l = 0, lsiz = 20; strcmp(CMDL_ARGS[l].arg_type, "end"); ++l)
    {
      if(l >= lsiz)
        {
          lsiz         *= 2;
          hash_list     = realloc(hash_list, lsiz * 2 * sizeof(uint64_t));
          if(hash_list == NULL)
            {
              // memory allocation error
              fprintf(stderr, MEMERROR);
              return NULL;
            }
        }

      hash_list[2 * l]      = hash(CMDL_ARGS[l].arg_long);
      hash_list[2 * l + 1]  = hash(CMDL_ARGS[l].arg_short);
    }

  /*--------------------------------------------------------*\
  ! Save the total number of active command line arguments,  !
  ! reallocate the corresponding hash list and initialize    !
  ! the arguments linked list.                               !
  \*--------------------------------------------------------*/
  lsiz    = l;
  tmp     = realloc(hash_list, lsiz * 2 * sizeof(uint64_t));
  if(tmp == NULL)
    {
      // memory allocation error
      free(hash_list);
      fprintf(stderr, MEMERROR);
      return NULL;
    }
  else
    {
      hash_list = tmp;
    }

  args     = calloc(1, sizeof(bwc_cmdl_arg_node));
  if(args == NULL)
    {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      return NULL;
    }
  args->root = args;

  /*--------------------------------------------------------*\
  ! Walk through all the command-line arguments passed to    !
  ! main.                                                    !
  \*--------------------------------------------------------*/
  for(i = 0; i < argc; ++i)
    {
      /*--------------------------------------------------------*\
      ! Check if the command-line argument is hyphenated, indi-  !
      ! cating a bwc argument or option. If this is the case,    !
      ! save a pointer to the argument in a temporary variable.  !
      \*--------------------------------------------------------*/
      str = get_opt(argv[i]);

      if(str)
        {
          /*--------------------------------------------------------*\
          ! Generate a hash from the argument string.                !
          \*--------------------------------------------------------*/
          buf = hash(++str);

          /*--------------------------------------------------------*\
          ! Check if the command-line argument is a valid bwc argu-  !
          ! ment or option and if additional values are expected for !
          ! the current argument or option. The additional values    !
          ! are handled according to the type of the current argu-   !
          ! ment.                                                    !
          \*--------------------------------------------------------*/
          for(l = 0; l < lsiz; ++l)
            {
              if(buf == hash_list[2 * l] || buf == hash_list[2 * l + 1])
                {
                  switch(hash(CMDL_ARGS[l].type))
                    {
                      /*--------------------------------------------------------*\
                      ! This type of argument/option requires no additional in-  !
                      ! formation and the parser can advance to the next hyphen- !
                      ! ated command-line argument.                              !
                      \*--------------------------------------------------------*/
                      case 0x000000000B87DB14:
                        {
                          /*--------------------------------------------------------*\
                          ! Store the hash identifier in the linked list node.       !
                          \*--------------------------------------------------------*/
                          args->hash = hash_list[2 * l];
                          break;
                        }

                      /*--------------------------------------------------------*\
                      ! This type of argument/option accepts a single string     !
                      ! that is passed to the parser as a non-hyphenated         !
                      ! command-line argument.                                   !
                      \*--------------------------------------------------------*/
                      case 0x000000017C8A3F41:
                        {
                          /*--------------------------------------------------------*\
                          ! Store the hash identifier in the linked list node.       !
                          \*--------------------------------------------------------*/
                          args->hash = hash_list[2 * l];

                          /*--------------------------------------------------------*\
                          ! Check that the next command-line argument is non-hypenat-!
                          ! ed. If true, allocate the lit_opt character array and    !
                          ! store the arguments memory handle.                       !
                          \*--------------------------------------------------------*/
                          if(((i + 1) < argc) && (get_opt(argv[i + 1]) == NULL))
                            {
                              args->lit_opt     = calloc(args->count + 1, sizeof(char*));
                              if(args->lit_opt == NULL)
                                {
                                  // memory allocation error
                                  fprintf(stderr, MEMERROR);
                                  bwc_kill_arg(args);
                                  free(hash_list);
                                  return NULL;
                                }
                              args->lit_opt[args->count++] = argv[i + 1];
                            }
                          break;
                        }

                      /*--------------------------------------------------------*\
                      ! This type of argument/option accepts four additional     !
                      ! strings that are passed to parser as a non-hyphenated    !
                      ! command-line arguments.                                  !
                      \*--------------------------------------------------------*/
                      case 0x000000017C8A3F44:
                        {
                          /*--------------------------------------------------------*\
                          ! Store the hash identifier in the linked list node.       !
                          \*--------------------------------------------------------*/
                          args->hash = hash_list[2 * l];

                          /*--------------------------------------------------------*\
                          ! Loop through the next four command-line arguments, check !
                          ! that they are non-hyphenated and, if true, store their   !
                          ! memory handle in the list node.                          !
                          \*--------------------------------------------------------*/
                          for(j = i + 1; j < argc && (get_opt(argv[j]) == NULL); ++j)
                            {
                              /*--------------------------------------------------------*\
                              ! Allocate the lit_opt array for 4 character strings.      !
                              \*--------------------------------------------------------*/
                              if(args->lit_opt == NULL)
                                {
                                  args->lit_opt     = calloc(4, sizeof(char*));
                                  if(args->lit_opt == NULL)
                                    {
                                      // memory allocation error
                                      fprintf(stderr, MEMERROR);
                                      bwc_kill_arg(args);
                                      free(hash_list);
                                      return NULL;
                                    }
                                  args->count   = 0x04;
                                }

                              /*--------------------------------------------------------*\
                              ! Save the memory handle for the next argument in a tempo- !
                              ! rary variable and tokenize the string according to the   !
                              ! specified delimiters.                                    !
                              \*--------------------------------------------------------*/
                              str           = argv[j];
                              token         = strtok_r(str, ":=\t, ", &ptr);

                              /*--------------------------------------------------------*\
                              ! Check if a valid token has been generated for the first  !
                              ! spatial direction. If true, save the memory handle, set  !
                              ! the dimension flag and generate the next token.          !
                              \*--------------------------------------------------------*/
                              if(token && ((strchr(token, 'x') != NULL) || 
                                           (strchr(token, 'X') != NULL)))
                                {
                                  token            = strtok_r(NULL, ":=\t, ", &ptr);
                                  args->lit_opt[0] = token;
                                  args->dim       |= 0x01;
                                  token            = strtok_r(NULL, ":=\t, ", &ptr);
                                }

                              /*--------------------------------------------------------*\
                              ! Check if a valid token has been generated for the second !
                              ! spatial direction. If true, save the memory handle, set  !
                              ! the dimension flag and generate the next token.          !
                              \*--------------------------------------------------------*/
                              if(token && ((strchr(token, 'y') != NULL) || 
                                           (strchr(token, 'Y') != NULL)))
                                {
                                  token            = strtok_r(NULL, ":=\t, ", &ptr);
                                  args->lit_opt[1] = token;
                                  args->dim       |= 0x02;
                                  token            = strtok_r(NULL, ":=\t, ", &ptr);
                                }

                              /*--------------------------------------------------------*\
                              ! Check if a valid token has been generated for the third  !
                              ! spatial direction. If true, save the memory handle, set  !
                              ! the dimension flag and generate the next token.          !
                              \*--------------------------------------------------------*/
                              if(token && ((strchr(token, 'z') != NULL) ||
                                           (strchr(token, 'Z') != NULL)))
                                {
                                  token            = strtok_r(NULL, ":=\t, ", &ptr);
                                  args->lit_opt[2] = token;
                                  args->dim       |= 0x04;
                                  token            = strtok_r(NULL, ":=\t, ", &ptr);
                                }

                              /*--------------------------------------------------------*\
                              ! Check if a valid token has been generated for the first  !
                              ! temporal direction. If true, save the memory handle, set !
                              ! the dimension flag and generate the next token.          !
                              \*--------------------------------------------------------*/
                              if(token && ((strchr(token, 't') != NULL) ||
                                           (strchr(token, 'T') != NULL)))
                                {
                                  token            = strtok_r(NULL, ":=\t, ", &ptr);
                                  args->lit_opt[3] = token;
                                  args->dim       |= 0x08;
                                  token            = strtok_r(NULL, ":=\t, ", &ptr);
                                }
                            }
                          break;
                        }

                      /*--------------------------------------------------------*\
                      ! This type of argument/option accepts a single numerical  !
                      ! value that is passed to parser as a non-hyphenated com-  !
                      ! mand-line argument.                                      !
                      \*--------------------------------------------------------*/
                      case 0x000000017C82A8C2:
                        {
                          /*--------------------------------------------------------*\
                          ! Store the hash identifier in the linked list node.       !
                          \*--------------------------------------------------------*/
                          args->hash = hash_list[2 * l];

                          /*--------------------------------------------------------*\
                          ! Check that the next command-line argument is non-hypenat-!
                          ! ed and a valid floating point value. If true, allocate   !
                          ! the num_opt array and store the real value in it.        !
                          \*--------------------------------------------------------*/
                          if(((i + 1) < argc) && (get_opt(argv[i + 1]) == NULL))
                            {
                              str = argv[i + 1];

                              if(strtof(str, NULL) > 0.0f)
                                {
                                  args->num_opt     = calloc(args->count + 1, sizeof(double));
                                  if(args->num_opt == NULL)
                                    {
                                      // memory allocation error
                                      fprintf(stderr, MEMERROR);
                                      bwc_kill_arg(args);
                                      free(hash_list);
                                      return NULL;
                                    }
                                  args->num_opt[args->count++] = strtof(str, NULL);
                                }
                            }
                          break;
                        }

                      /*--------------------------------------------------------*\
                      ! This type of argument/option accepts four additional nu- !
                      ! merical values that are passed to parser as a non-hyphen-!
                      ! ated command-line arguments.                             !
                      \*--------------------------------------------------------*/
                      case 0x000000017C82A8C7:
                        {
                          /*--------------------------------------------------------*\
                          ! Store the hash identifier in the linked list node.       !
                          \*--------------------------------------------------------*/
                          args->hash = hash_list[2 * l];

                          /*--------------------------------------------------------*\
                          ! Loop through the next four command-line arguments, check !
                          ! that they are non-hyphenated and, if true, store their   !
                          ! numerical values handle in the list node.                !
                          \*--------------------------------------------------------*/
                          for(j = i + 1; j < argc && (get_opt(argv[j]) == NULL); ++j)
                            {
                              /*--------------------------------------------------------*\
                              ! Allocate the num_opt array for 4 real values.            !
                              \*--------------------------------------------------------*/
                              if(args->num_opt == NULL)
                                {
                                  args->num_opt     = calloc(4, sizeof(double));
                                  if(args->num_opt == NULL)
                                    {
                                      // memory allocation error
                                      fprintf(stderr, MEMERROR);
                                      bwc_kill_arg(args);
                                      free(hash_list);
                                      return NULL;
                                    }
                                  args->count   = 0x04;
                                }

                              /*--------------------------------------------------------*\
                              ! Save the memory handle for the next argument in a tempo- !
                              ! rary variable and tokenize the string according to the   !
                              ! specified delimiters.                                    !
                              \*--------------------------------------------------------*/
                              str           = argv[j];
                              token         = strtok_r(str, ":=\t, ", &ptr);

                              /*--------------------------------------------------------*\
                              ! Check if a valid token has been generated for the first  !
                              ! spatial direction. If true, save the numerical value,    !
                              ! set the dimension flag and generate the next token.      !
                              \*--------------------------------------------------------*/
                              if(token && ((strchr(token, 'x') != NULL) || 
                                           (strchr(token, 'X') != NULL)))
                                {
                                  token     = strtok_r(NULL, ":=\t, ", &ptr);
                                  if(token != NULL)
                                    {
                                      args->num_opt[0] = strtof(token, NULL);
                                      args->dim       |= 0x01;
                                    }
                                  token = strtok_r(NULL, ":=\t, ", &ptr);
                                }

                              /*--------------------------------------------------------*\
                              ! Check if a valid token has been generated for the second !
                              ! spatial direction. If true, save the numerical value,    !
                              ! set the dimension flag and generate the next token.      !
                              \*--------------------------------------------------------*/
                              if(token && ((strchr(token, 'y') != NULL) ||
                                           (strchr(token, 'Y') != NULL)))
                                {
                                  token     = strtok_r(NULL, ":=\t, ", &ptr);
                                  if(token != NULL)
                                    {
                                      args->num_opt[1] = strtof(token, NULL);
                                      args->dim       |= 0x02;
                                    }
                                  token = strtok_r(NULL, ":=\t, ", &ptr);
                                }

                              /*--------------------------------------------------------*\
                              ! Check if a valid token has been generated for the third  !
                              ! spatial direction. If true, save the numerical value,    !
                              ! set the dimension flag and generate the next token.      !
                              \*--------------------------------------------------------*/
                              if(token && ((strchr(token, 'z') != NULL) || 
                                           (strchr(token, 'Z') != NULL)))
                                {
                                  token     = strtok_r(NULL, ":=\t, ", &ptr);
                                  if(token != NULL)
                                    {
                                      args->num_opt[2] = strtof(token, NULL);
                                      args->dim       |= 0x04;
                                    }
                                  token = strtok_r(NULL, ":=\t, ", &ptr);
                                }

                              /*--------------------------------------------------------*\
                              ! Check if a valid token has been generated for the first  !
                              ! temporal direction. If true, save the numerical value,   !
                              ! set the dimension flag and generate the next token.      !
                              \*--------------------------------------------------------*/
                              if(token && ((strchr(token, 't') != NULL) ||
                                           (strchr(token, 'T') != NULL)))
                                {
                                  token     = strtok_r(NULL, ":=\t, ", &ptr);
                                  if(token != NULL)
                                    {
                                      args->num_opt[3] = strtof(token, NULL);
                                      args->dim       |= 0x08;
                                    }
                                  token = strtok_r(NULL, ":=\t, ", &ptr);
                                }
                            }
                          break;
                        }

                      /*--------------------------------------------------------*\
                      ! This type of argument/option accepts one ore more numeri-!
                      ! cal values that are passed to parser as a non-hyphenated !
                      ! command-line arguments.                                  !
                      \*--------------------------------------------------------*/
                      case 0x000000017C82A8BF:
                        {
                          /*--------------------------------------------------------*\
                          ! Store the hash identifier in the linked list node.       !
                          \*--------------------------------------------------------*/
                          args->hash = hash_list[2 * l];

                          /*--------------------------------------------------------*\
                          ! Loop through the next command-line arguments that are    !
                          ! non-hyphenated and store their numerical values handle   !
                          ! in the list node.                                        !
                          \*--------------------------------------------------------*/
                          for(j = i + 1; j < argc && (get_opt(argv[j]) == NULL); ++j)
                            {
                              /*--------------------------------------------------------*\
                              ! Save the memory handle for the next argument in a tempo- !
                              ! rary variable and tokenize the string according to the   !
                              ! specified delimiters.                                    !
                              \*--------------------------------------------------------*/
                              str           = argv[j];
                              token         = strtok_r(str, ", ", &ptr);

                              /*--------------------------------------------------------*\
                              ! Check if a valid token has been generated. If true, re-  !
                              ! size the num_opt array, save the numerical value, and    !
                              ! generate the next token.                                 !
                              \*--------------------------------------------------------*/
                              for(; (token != NULL); token = strtok_r(NULL, ", ", &ptr))
                                {
                                  if(token != NULL)
                                  {
                                    if(strtof(token, NULL) > 0.0f)
                                      {
                                        args->num_opt     = realloc(args->num_opt, (args->count + 1) * sizeof(double));
                                        if(args->num_opt == NULL) 
                                          {
                                            // memory allocation error
                                            fprintf(stderr, MEMERROR);
                                            bwc_kill_arg(args);
                                            free(hash_list);
                                            return NULL;
                                          }
                                        args->num_opt[args->count++] = strtof(token, NULL);
                                      }
                                    }
                                }
                            }
                          break;
                        }

                      default:
                        {
                          continue;
                        }
                    }
                  /*--------------------------------------------------------*\
                  ! Allocate and initialize the next linked list node.       !
                  \*--------------------------------------------------------*/
                  args->next     = calloc(1, sizeof(bwc_cmdl_arg_node));
                  if(args->next == NULL)
                    {
                      // memory allocation error
                      fprintf(stderr, MEMERROR);
                      bwc_kill_arg(args);
                      free(hash_list);
                      return NULL;
                    }
                  args->next->root = args->root;
                  args             = args->next;
                }
            }
        }
    }
  /*--------------------------------------------------------*\
  ! Free the command line argument hash list and return the  !
  ! linked list to the function caller.                      !
  \*--------------------------------------------------------*/
  free(hash_list);
  return args;
}

/*--------------------------------------------------------------------------------------------------------------------*\
!                                                                                                                      !
!   DESCRIPTION:                                                                                                       !
!   ------------                                                                                                       !
!                    This function is used to parse the util_arg_node linked list and, if it is pre-                   !
!                    sent in linked list, return the memory handle to the bwc argument/option short-                   !
!                    /long-name supplied by the function.                                                              !
!                                                                                                                      !
!   PARAMETERS:                                                                                                        !
!   -----------                                                                                                        !
!                    Variable                 Type                   Description                                       !
!                    --------                 ----                   -----------                                       !
!                    args                     util_arg_node*       - Arguments/options linked list                     !
!                                                                    for the bwc command-line tool.                    !
!                                                                                                                      !
!                    str                      char*                - Long-/short-name of a bwc com-                    !
!                                                                    mand-line argument or option.                     !
!                                                                                                                      !
!   RETURN VALUE:                                                                                                      !
!   -------------                                                                                                      !
!                    Type                                            Description                                       !
!                    ----                                            -----------                                       !
!                    util_arg_nodes*                               - Memory handle bwc argument                        !
!                                                                    linked list node.                                 !
!                                                                                                                      !
!   DEVELOPMENT HISTORY:                                                                                               !
!   --------------------                                                                                               !
!                                                                                                                      !
!                    Date        Author             Change Id   Release     Description                                !
!                    ----        ------             ---------   -------     -----------                                !
!                    24.04.2019  Patrick Vogler     B87D120     V 0.1.0     function created                           !
!                    04.05.2021  Patrick Vogler     B87E7E4     V 0.1.0     clean up                                   !
!                                                                                                                      !
\*--------------------------------------------------------------------------------------------------------------------*/
static bwc_cmdl_arg_node*
retrieve_arg(bwc_cmdl_arg_node *const args, 
             char*                    name)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint64_t            arg_hash;

  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  bwc_cmdl_arg_node  *temp;

  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(args);
  assert(name);

  /*--------------------------------------------------------*\
  ! Generate a hash from the argument/option name and save   !
  ! the root node memory address in a temporary variable.    !
  \*--------------------------------------------------------*/
  arg_hash = hash(name);
  temp     = args->root;

  /*--------------------------------------------------------*\
  ! Advance through the linked list until the argument/op-   !
  ! tion, corresponding to the supplied name, has been found !
  ! or the end of the list has been reached.                 !
  \*--------------------------------------------------------*/
  while(temp->hash != arg_hash  && temp->next != NULL)
    temp = temp->next;

  /*--------------------------------------------------------*\
  ! If found, return the memory handle to the argument/op-   !
  ! option to the function caller.                           !
  \*--------------------------------------------------------*/
  if((temp->next != NULL) && (temp->hash == arg_hash))
    return temp;
  else
    return NULL;
}

/*--------------------------------------------------------------------------------------------------------------------*\
!                                                                                                                      !
!   DESCRIPTION:                                                                                                       !
!   ------------                                                                                                       !
!                    This function prints the help page to the standard output.                                        !
!                                                                                                                      !
!   PARAMETERS:                                                                                                        !
!   -----------                                                                                                        !
!                    Variable                 Type                   Description                                       !
!                    --------                 ----                   -----------                                       !
!                    -                        -                      -                                                 !
!                                                                                                                      !
!   RETURN VALUE:                                                                                                      !
!   -------------                                                                                                      !
!                    Type                                            Description                                       !
!                    ----                                            -----------                                       !
!                    -                                               -                                                 !
!                                                                                                                      !
!   DEVELOPMENT HISTORY:                                                                                               !
!   --------------------                                                                                               !
!                                                                                                                      !
!                    Date        Author             Change Id   Release     Description                                !
!                    ----        ------             ---------   -------     -----------                                !
!                    14.05.2019  Patrick Vogler     B87D120     V 0.1.0     function created                           !
!                    03.05.2021  Patrick Vogler     B87E7E4     V 0.1.0     clean up                                   !
!                                                                                                                      !
\*--------------------------------------------------------------------------------------------------------------------*/
static void
print_help(void)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint16_t          i;

  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  char             *buf;

  /*--------------------------------------------------------*\
  ! Write the head of the help message to the standard out-  !
  ! put.                                                     !
  \*--------------------------------------------------------*/
  printf("Usage: bwc [arguments] [file] [options]                   (De)compress a specified [file] using [options].  \n"\
         "                                                                                                            \n"\
         "e.g.: bwc -c -i [input] -o [output] [options]             Compress an [input] file with the specified [op-  \n"\
         "                                                          tions] and save the compressed bitstream in the   \n"\
         "                                                          [output] file.                                    \n"\
         "                                                                                                            \n"\
         "e.g.: bwc -c [input] [options]                            Compress an [input] file with the specified [op-  \n"\
         "                                                          tions] and save the compressed bitstream.         \n"\
         "                                                                                                            \n"\
         "e.g.: bwc -d -i [input] -o [output] [options]             Decompress an [input] file with the specified [op-\n"\
         "                                                          tions] and save the decompressed bitstream in the \n"\
         "                                                          [output] file.                                    \n"\
         "                                                                                                            \n"\
         "e.g.: bwc -d [input] [options]                            Decompress an [input] file with the specified [op-\n"\
         "                                                          tions] and save the decompressed bitstream.       \n"\
         "                                                                                                            \n"\
         "e.g.: bwc -al -i [input] -ref [reference]                 Evaluate the Peak-Signal-to-Noise-Ratio of the    \n"\
         "                                                          [input] file with regards to the [reference] file \n"\
         "                                                          and print the result to the standard output.      \n"\
         "                                                                                                            \n"\
         "e.g.: bwc -if -i [input]                                  Print the header information of the compressed    \n"\
         "                                                          [input] file to the standard output.              \n"\
         "                                                                                                            \n"\
         "                                                                                                            \n"\
         "Arguments:                                                                                                  \n"\
         "                                                                                                            \n"\
         "   [Type]                         [Usage]                 [Description]                                     \n"\
         "                                                                                                            \n");

  /*--------------------------------------------------------*\
  ! Loop through the command line arguments list and write   !
  ! the short and long name, usage example and description   !
  ! for all mandatory (arg) command line arguments to stdout.!
  \*--------------------------------------------------------*/
  for(i = 0; strcmp(CMDL_ARGS[i].arg_type, "end"); ++i)
    {
      if(strcmp(CMDL_ARGS[i].arg_type, "arg") == 0 && CMDL_ARGS[i].active)
        {
          printf("   -%s,\t",  CMDL_ARGS[i].arg_short);
          printf("--%-24s",    CMDL_ARGS[i].arg_long);
          printf("%-24s",      CMDL_ARGS[i].usage);
          printf("%.50s\n",    CMDL_ARGS[i].definition);

          for(buf = CMDL_ARGS[i].definition + 50; strcmp(buf, ""); buf += 50)
            {
              printf("%58s%.50s\n", "", buf);
            }

          printf("\n");
        }
    }

  /*--------------------------------------------------------*\
  ! Write the head of the optional arguments list to the     !
  ! standard output.                                         !
  \*--------------------------------------------------------*/
  printf("Options:                                                                                                    \n"\
         "                                                                                                            \n"\
         "   [Type]                         [Usage]                 [Description]                                     \n"\
         "                                                                                                            \n");

  /*--------------------------------------------------------*\
  ! Loop through the command line arguments list and write   !
  ! the short and long name, usage example and description   !
  ! for all optional (arg) command line arguments to stdout. !
  \*--------------------------------------------------------*/
  for(i = 0; strcmp(CMDL_ARGS[i].arg_type, "end"); ++i)
    {
      if(strcmp(CMDL_ARGS[i].arg_type, "opt") == 0 && CMDL_ARGS[i].active)
        {
          printf("   -%s,\t",  CMDL_ARGS[i].arg_short);
          printf("--%-24s",    CMDL_ARGS[i].arg_long);
          printf("%-24s",      CMDL_ARGS[i].usage);
          printf("%.50s\n",    CMDL_ARGS[i].definition);

          for(buf = CMDL_ARGS[i].definition+50; strcmp(buf, ""); buf += 50)
            {
              printf("%58s%.50s\n", "", buf);
            }
          
          printf("\n");
        }
    }
}

/*--------------------------------------------------------------------------------------------------------------------*\
!                                                                                                                      !
!   DESCRIPTION:                                                                                                       !
!   ------------                                                                                                       !
!                    This function prints the header information of a compressed dataset to the                        !
!                    standard output.                                                                                  !
!                                                                                                                      !
!   PARAMETERS:                                                                                                        !
!   -----------                                                                                                        !
!                    Variable                 Type                   Description                                       !
!                    --------                 ----                   -----------                                       !
!                    args                     util_arg_node*       - Arguments/options linked list                     !
!                                                                    for the bwc command-line tool.                    !
!                                                                                                                      !
!                    name                     char*                - Long-/short-name of a bwc com-                    !
!                                                                    mand-line argument or option.                     !
!                                                                                                                      !
!   RETURN VALUE:                                                                                                      !
!   -------------                                                                                                      !
!                    Type                                            Description                                       !
!                    ----                                            -----------                                       !
!                    -                                               -                                                 !
!                                                                                                                      !
!   DEVELOPMENT HISTORY:                                                                                               !
!   --------------------                                                                                               !
!                                                                                                                      !
!                    Date        Author             Change Id   Release     Description                                !
!                    ----        ------             ---------   -------     -----------                                !
!                    12.09.2019  Patrick Vogler     B87D120     V 0.1.0     function created                           !
!                    17.05.2021  Patrick Vogler     B87E7E4     V 0.1.0     clean up                                   !
!                                                                                                                      !
\*--------------------------------------------------------------------------------------------------------------------*/
static void
output_info(bwc_cmdl_arg_node *const args, 
            char*                    name)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint64_t            data_points;

  uint64_t            Ld;
  uint32_t            Ls;
  uint16_t            Lh;

  uint32_t            t;
  uint8_t             l, p;

  uint16_t            marker;
  uint8_t             index;

  /*-----------------------*\
  ! DEFINE FLOAT VARIABLES: !
  \*-----------------------*/
  bwc_float           minVal, maxVal;

  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  char               *buff;
  uchar               status;

  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  bwc_field          *field;
  bwc_data           *data;

  bwc_gl_ctrl        *control;
  bwc_gl_inf         *info;

  bwc_param_ctrl     *param_ctrl;
  bwc_param_inf      *param_info;

  bwc_cmdl_arg_node  *temp;

  struct stat         buf;

  /*-----------------------*\
  ! DEFINE FILE POINTER:    !
  \*-----------------------*/
  FILE             *fp;

  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(args);

  /*--------------------------------------------------------*\
  ! Retrieve the file argument from the linked list and con- !
  ! firm that only one file has been specified and that it   !
  ! is a regular file. If true, open the file for reading.   !
  \*--------------------------------------------------------*/
  temp = retrieve_arg(args, name);
  if((temp              == NULL) || 
     (temp->lit_opt     == NULL) || 
     (temp->lit_opt[0]  == NULL))
    {
      fprintf(stderr, "o##########################################################o\n"\
                      "|   ERROR: File                                            |\n"\
                      "|                                                          |\n"\
                      "|          No File has been specified.                     |\n"\
                      "|                                                          |\n"\
                      "o##########################################################o\n");
      return;
    }

  if(temp->count > 1)
    {
      fprintf(stderr, "o##########################################################o\n"\
                      "|   ERROR: File                                            |\n"\
                      "|                                                          |\n"\
                      "|          More than one file has been specified.          |\n"\
                      "|                                                          |\n"\
                      "o##########################################################o\n");
      return;
    }

  stat(temp->lit_opt[0], &buf);
  if(!S_ISREG(buf.st_mode))
    {
      fprintf(stderr, "o##########################################################o\n"\
                      "|   ERROR: file                                            |\n"\
                      "|                                                          |\n"\
                      "|          The specified input is not a regular file.      |\n"\
                      "|                                                          |\n"\
                      "o##########################################################o\n");
      return;
    }

  fp     = fopen(temp->lit_opt[0], "rb");
  if(fp == NULL)
    {
      // error opening file
      fprintf(stderr, "o##########################################################o\n"\
                      "|   ERROR: Could not open or read %-25s|\n"\
                      "o##########################################################o\n", temp->lit_opt[0]);
      return;
    }

  /*--------------------------------------------------------*\
  ! Initialize the codestream status and reading index var-  !
  ! iables and parse the codestream to evaluate the size of  !
  ! the main header.                                         !
  \*--------------------------------------------------------*/
  status = CODESTREAM_OK;
  index = 0;

  while(status == 0)
   {
      Ld = fread(&marker, sizeof(uint16_t), 1, fp);
      endian_conversion(&marker, 2);
      if((Ld != 1) || (marker < 0xFF00))
        {
          // Invalid Codestream
          fprintf(stderr, CSTERROR);
          fclose(fp);
          return;
        }

      switch(marker)
        {
          case SOC:
            {
              break;
            }

          case COM:
          case SGC:
          case SGI:
          default:
            {
              if(fread(&Lh, sizeof(uint16_t), 1, fp) != 1)
                {
                  // Invalid Codestream
                  fprintf(stderr, CSTERROR);
                  fclose(fp);
                  return;
                }

              endian_conversion(&Lh, 2);
              fseek(fp, Lh - 2, SEEK_CUR);

              break;
            }

          case SAX:
            {
              if(fread(&Ls, sizeof(uint32_t), 1, fp) != 1)
                {
                  // Invalid Codestream
                  fprintf(stderr, CSTERROR);
                  fclose(fp);
                  return;
                }

              endian_conversion(&Ls, 4);
              fseek(fp, Ls - 4, SEEK_CUR);

              break;
            }

          case EOH:
            {
              if(fread(&Lh, sizeof(uint16_t), 1, fp) != 1)
                {
                  // Invalid Codestream
                  fprintf(stderr, CSTERROR);
                  fclose(fp);
                  return;
                }

              endian_conversion(&Lh, 2);
              fseek(fp, Lh - 2, SEEK_CUR);

              status |= CODESTREAM_READ;
              break;
            }
        }
      index++;
    }

  /*--------------------------------------------------------*\
  ! Determine the size of the main header present in the bwc !
  ! codestream and allocate the data structure, packed code- !
  ! stream sub-structure and its memory block.               !
  \*--------------------------------------------------------*/
  Ld = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  data     = calloc(1, sizeof(bwc_data));
  if(data == NULL)
    {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      fclose(fp);
      return;
    }

  data->codestream.data     = calloc(1, sizeof(bwc_stream));
  if(data->codestream.data == NULL)
    {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      bwc_free_data(data);
      fclose(fp);
      return;
    }

  data->codestream.data->memory     = calloc(Ld, sizeof(uchar));
  if(data->codestream.data->memory == NULL)
    {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      bwc_free_data(data);
      fclose(fp);
      return;
    }

  /*--------------------------------------------------------*\
  ! Read the main header from the packed codestream of the   !
  ! user specified bcw file and set up the data structure.   !
  \*--------------------------------------------------------*/
  if(fread(data->codestream.data->memory, sizeof(uchar), Ld, fp) != Ld)
    {
      // invalid read
      fprintf(stderr, RDERROR);
      bwc_free_data(data);
      fclose(fp);
      return;
    }

  data->codestream.data->access   = data->codestream.data->memory;
  data->codestream.data->size     = Ld;
  data->codestream.data->position = 0;

  /*--------------------------------------------------------*\
  ! Initialize the bitstream, parse the main header and set  !
  ! up the field structure for the current dataset.          !
  \*--------------------------------------------------------*/
  field     = bwc_create_decompression(data, 0);
  if(field == NULL)
    {
      bwc_free_data(data);
      fclose(fp);
      return;
    }
  control = &field->control;
  info    =  field->info;

  /*--------------------------------------------------------*\
  ! Write the help message to the standard output.           !
  \*--------------------------------------------------------*/
  printf("/================================================================================\\\n"\
         "|                                                                                |\n"\
         "|                   .:-------------:           .:-------------:                  |\n"\
	       "|                  .+++++++++++++++=          :+++++++++++++++-                  |\n"\
	       "|                  :+++.        -++=          -++=                               |\n"\
	       "|                  :+++.        -++=          -++=                               |\n"\
	       "|                   -++++++++++++++=   -++=   -++=                               |\n"\
	       "|                  .=++---------=++=   -++=   -++=                               |\n"\
	       "|                  :+++         :++=   -++=   -++=                               |\n"\
	       "|                  .+++=--------=+++---=+++---=+++------------:                  |\n"\
	       "|                   -=++++++++++++++++++++++++++++++++++++++++-                  |\n"\
         "|                                                                                |\n"\
         "|------------------------------ General Information -----------------------------|\n"\
         "|                                                                                |\n");
  
  /*--------------------------------------------------------*\
  ! Print the original file size and format.                 !
  \*--------------------------------------------------------*/
  data_points = (uint64)info->nX  * info->nY * info->nZ *
                        info->nTS * info->nPar;
  buff        = get_size(data_points * 8);

  printf("|    Original size:                %42s    |\n"\
         "|    Original file format:         %42s    |\n"\
         "|%80s|\n", buff, info->f_ext, " ");

  free(buff);

  /*--------------------------------------------------------*\
  ! Print the file size and compression ratio.               !
  \*--------------------------------------------------------*/
  fseek(fp, 0L, SEEK_END);
  Ld   = ftell(fp);
  buff = get_size(Ld);
  fclose(fp);

  printf("|    Size on Disk:                 %42s    |\n"\
         "|    Comp. Ratio:                  %42.2f    |\n"\
         "|%80s|\n", buff, ((double)data_points * 8.0f/Ld), " ");

  free(buff);

  /*--------------------------------------------------------*\
  ! Print the number of datapoints.                          !
  \*--------------------------------------------------------*/
  buff = get_digit_sep(data_points);
  printf("|    Number of Datapoints:         %42s    |\n"\
         "|%80s|\n", buff, " ");
  free(buff);
            
  buff = get_digit_sep(info->nX);
  printf("|      -1st Dim.:                  %42s    |\n", buff);
  free(buff);

  buff = get_digit_sep(info->nY);
  printf("|      -2nd Dim.:                  %42s    |\n", buff);
  free(buff);

  buff = get_digit_sep(info->nZ);
  printf("|      -3nd Dim.:                  %42s    |\n", buff);
  free(buff);

  buff = get_digit_sep(info->nTS);
  printf("|      -Time Steps:                %42s    |\n"\
         "|      -No. Params:                %42d    |\n"\
         "|%80s|\n", buff, info->nPar, " ");
  free(buff);

  /*--------------------------------------------------------*\
  ! Print the numerical Datapoints.                          !
  \*--------------------------------------------------------*/
  printf("|----------------------------- Numerical Parameters -----------------------------|\n"\
         "|                                                                                |\n");

  for(p = 0; p < info->nPar; ++p)
    {
      param_ctrl = &field->tile[0].parameter[p].control;
      param_info = &field->tile[0].parameter[p].info;

      minVal     = param_info->parameter_min;
      maxVal     = param_info->parameter_max;

      for(t = 0; t <= control->nTiles; ++t)
        {
          minVal = MIN(minVal, param_info->parameter_min);
          maxVal = MIN(maxVal, param_info->parameter_max);
        }

      buff = param_info->name + strlen(param_info->name) - 1;
      while(buff > param_info->name && isspace((unsigned char)*buff)) buff--;
      buff[1] = '\0';

      if(p != 0)
        {
            printf("|    ........................................................................    |\n"
                   "|%80s|\n"," ");
        }

      printf("|    Name:            %55s    |\n"\
             "|    Minimum value:   %55.2e    |\n"\
             "|    Maximum value:   %55.2e    |\n",  param_info->name, 
                                                      minVal,
                                                      maxVal);
    }
  printf("|%80s|\n"," ");
  /*--------------------------------------------------------*\
  ! Print the quality layers.                                !
  \*--------------------------------------------------------*/
  printf("|-------------------------------- Quality Layers --------------------------------|\n"\
         "|                                                                                |\n");

  for(l = 0; l < control->nLayers; ++l)
    {
      buff = get_size(data_points * control->bitrate[l]);

      printf("|                                                                                |\n"\
             "|    Quality Layer Nr. %d:        %40.2f Bpd    |\n"\
             "|                                %44s    |\n", l + 1, control->bitrate[l], buff);
      free(buff);
    }

  printf("|                                                                                |\n"\
          "\\--------------------------------------------------------------------------------/\n");

  /*--------------------------------------------------------*\
  ! Free the field structure.                                !
  \*--------------------------------------------------------*/
  bwc_kill_compression(field);
  bwc_free_data(data);
}

/*--------------------------------------------------------------------------------------------------------------------*\
!                                                                                                                      !
!   DESCRIPTION:                                                                                                       !
!   ------------                                                                                                       !
!                    This function opens a bwc file, checks it for its validity and loads its con-                     !
!                    tent into a properly set up bwc_data structure.                                                   !
!                                                                                                                      !
!   PARAMETERS:                                                                                                        !
!   -----------                                                                                                        !
!                    Variable                 Type                   Description                                       !
!                    --------                 ----                   -----------                                       !
!                    filename                 char*                - Name of the bwc file that is                      !
!                                                                    to be opened and read.                            !
!                                                                                                                      !
!   RETURN VALUE:                                                                                                      !
!   -------------                                                                                                      !
!                    Type                                            Description                                       !
!                    ----                                            -----------                                       !
!                    bwc_data*                                     - Structure containing a code-                      !
!                                                                    stream/numerical dataset.                         !
!                                                                                                                      !
!   DEVELOPMENT HISTORY:                                                                                               !
!   --------------------                                                                                               !
!                                                                                                                      !
!                    Date        Author             Change Id   Release     Description                                !
!                    ----        ------             ---------   -------     -----------                                !
!                    20.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                           !
!                    17.05.2021  Patrick Vogler     B87E7E4     V 0.1.0     clean up                                   !
!                                                                                                                      !
\*--------------------------------------------------------------------------------------------------------------------*/
static bwc_data*
read_bwc(char *const filename)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint64_t  Lfield;
  uint16_t  root;

  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  bwc_data *file;

  /*-----------------------*\
  ! DEFINE FILE POINTER:    !
  \*-----------------------*/
  FILE     *fp;

  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(filename);

  /*--------------------------------------------------------*\
  ! Open the specified file for reading and, if succesful,   !
  ! determine the size of the codestream. If the file could  !
  ! not be opened, print an error message to stderr and exit.!
  \*--------------------------------------------------------*/
  fp     = fopen(filename, "rb");
  if(fp != NULL)
    {
      root   = ftell(fp);
      fseek(fp, 0L,   SEEK_END); 
      Lfield = ftell(fp) - root;
      fseek(fp, root, SEEK_SET);
    }
  else
    {
      // error opening file
      fprintf(stderr, "o##########################################################o\n"\
                      "|   ERROR: Could not open or read %-25s|\n"\
                      "o##########################################################o\n", filename);
      return NULL;
    }

  /*--------------------------------------------------------*\
  ! Allocate the data and packed codestream structure as     !
  ! well as the memory block that will hold the codestream.  !
  \*--------------------------------------------------------*/
  file     = calloc(1,  sizeof(bwc_data));
  if(file == NULL)
    {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      fclose(fp);
      return NULL;
    }

  file->codestream.data     = calloc(1,  sizeof(bwc_stream));
  if(file->codestream.data == NULL)
    {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      bwc_free_data(file);
      fclose(fp);
      return NULL;
    }

  file->codestream.data->memory     = calloc(Lfield, sizeof(uchar));
  if(file->codestream.data->memory == NULL)
    {
      // memory allocation error
      fprintf(stderr, MEMERROR);
      bwc_free_data(file);
      fclose(fp);
      return NULL;
    }

  /*--------------------------------------------------------*\
  ! Read the compressed bitstream from the file and, if suc- !
  ! essfull, properly set up the packed codestream and re-   !
  ! turn the bwc_data structre to the function caller.       !
  \*--------------------------------------------------------*/
  if(fread(file->codestream.data->memory, sizeof(uchar), Lfield, fp) != Lfield)
    {
      // invalid read
      fprintf(stderr, RDERROR);
      bwc_free_data(file);
      fclose(fp);
      return NULL;
    }

  file->codestream.data->access   = file->codestream.data->memory;
  file->codestream.data->size     = Lfield;
  file->codestream.data->position = 0;

  fclose(fp);
  return file;
}

/*--------------------------------------------------------------------------------------------------------------------*\
!                                                                                                                      !
!   DESCRIPTION:                                                                                                       !
!   ------------                                                                                                       !
!                    This function opens a bwc file, checks it for its validity and writes the code-                   !
!                    stream, provided by the function caller, to the file.                                             !
!                                                                                                                      !
!   PARAMETERS:                                                                                                        !
!   -----------                                                                                                        !
!                    Variable                 Type                   Description                                       !
!                    --------                 ----                   -----------                                       !
!                    filename                 char*                - Name of the bwc file that is                      !
!                                                                    to be created.                                    !
!                                                                                                                      !
!   RETURN VALUE:                                                                                                      !
!   -------------                                                                                                      !
!                    Type                                            Description                                       !
!                    ----                                            -----------                                       !
!                    unsigned char                                 - Returns an unsigned char for                      !
!                                                                    error handling.                                   !
!                                                                                                                      !
!   DEVELOPMENT HISTORY:                                                                                               !
!   --------------------                                                                                               !
!                                                                                                                      !
!                    Date        Author             Change Id   Release     Description                                !
!                    ----        ------             ---------   -------     -----------                                !
!                    20.06.2018  Patrick Vogler     B87D120     V 0.1.0     function created                           !
!                    17.05.2021  Patrick Vogler     B87E7E4     V 0.1.0     clean up                                   !
!                                                                                                                      !
\*--------------------------------------------------------------------------------------------------------------------*/
static uchar
write_bwc(bwc_data  *const data, 
          char      *const filename)
{
  /*-----------------------*\
  ! DEFINE FILE POINTER:    !
  \*-----------------------*/
  FILE   *fp;

  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(data);
  assert(filename);

  /*--------------------------------------------------------*\
  ! Open the specified file for writing  If the file could   !
  ! not be opened, print an error message to stderr and exit.!
  \*--------------------------------------------------------*/
  fp     = fopen(filename, "wb");
  if(fp == NULL)
    {
      // error opening file
      fprintf(stderr, "o##########################################################o\n"\
                      "|   ERROR: Could not open %-33s|\n"\
                      "o##########################################################o\n", filename);
      return EXIT_FAILURE;
    }

  /*--------------------------------------------------------*\
  ! Write the compressed codestream to file and, if success- !
  ! ful, close the file ptr and return to the fun. caller.   !
  \*--------------------------------------------------------*/
  if(fwrite(data->codestream.data->memory, sizeof(uchar), data->codestream.data->size, fp) != 
                                                          data->codestream.data->size)
    {
      // error opening file
      fprintf(stderr, "o##########################################################o\n"\
                      "|   ERROR: Could not write to %-29s|\n"\
                      "o##########################################################o\n", filename);
      fclose(fp);
      return EXIT_FAILURE;
    }

  fclose(fp);
  return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------------------------------------*\
!                                                                                                                      !
!   DESCRIPTION:                                                                                                       !
!   ------------                                                                                                       !
!                    This function is a wrapper that is used to invoke the appropriate read func-                      !
!                    tion according to the file extension of the user supplied filename. The data-                     !
!                    set is stored in the bwc_data structure and passed onto the function caller.                      !
!                                                                                                                      !
!   PARAMETERS:                                                                                                        !
!   -----------                                                                                                        !
!                    Variable                 Type                   Description                                       !
!                    --------                 ----                   -----------                                       !
!                    args                     util_arg_node*       - Arguments/options linked list                     !
!                                                                    for the bwc command-line tool.                    !
!                                                                                                                      !
!                    str                      char*                - Long-/short-name of a bwc com-                    !
!                                                                    mand-line argument or option.                     !
!                                                                                                                      !
!   RETURN VALUE:                                                                                                      !
!   -------------                                                                                                      !
!                    Type                                            Description                                       !
!                    ----                                            -----------                                       !
!                    bwc_data*                                     - Structure containing a code-                      !
!                                                                    stream/numerical dataset.                         !
!                                                                                                                      !
!   DEVELOPMENT HISTORY:                                                                                               !
!   --------------------                                                                                               !
!                                                                                                                      !
!                    Date        Author             Change Id   Release     Description                                !
!                    ----        ------             ---------   -------     -----------                                !
!                    14.05.2019  Patrick Vogler     B87D120     V 0.1.0     function created                           !
!                    03.05.2021  Patrick Vogler     B87E7E4     V 0.1.0     clean up                                   !
!                                                                                                                      !
\*--------------------------------------------------------------------------------------------------------------------*/
static bwc_data*
read_file(bwc_cmdl_arg_node *const args, 
          char*                    name)
{
  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  char               *str;

  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  bwc_cmdl_arg_node  *temp;
  bwc_data           *file;
  struct stat         buf;

  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(args);
  assert(name);

  /*--------------------------------------------------------*\
  ! Retrieve the file argument from the linked list and con- !
  ! firm that only one file has been specified and that it   !
  ! is a regular file.                                       !
  \*--------------------------------------------------------*/
  temp = retrieve_arg(args, name);
  if((temp              == NULL)  ||
     (temp->lit_opt     == NULL)  ||
     (temp->lit_opt[0]  == NULL))
    {
      fprintf(stderr, "o##########################################################o\n"\
                      "|   ERROR: File                                            |\n"\
                      "|                                                          |\n"\
                      "|          No File has been specified.                     |\n"\
                      "|                                                          |\n"\
                      "o##########################################################o\n");
      return NULL;
    }

  if(temp->count > 1)
    {
      fprintf(stderr, "o##########################################################o\n"\
                      "|   ERROR: File                                            |\n"\
                      "|                                                          |\n"\
                      "|          More than one file has been specified.          |\n"\
                      "|                                                          |\n"\
                      "o##########################################################o\n");
      return NULL;
    }

  stat(temp->lit_opt[0], &buf);
  if(!S_ISREG(buf.st_mode))
    {
      fprintf(stderr, "o##########################################################o\n"\
                      "|   ERROR: file                                            |\n"\
                      "|                                                          |\n"\
                      "|          The specified input is not a regular file.      |\n"\
                      "|                                                          |\n"\
                      "o##########################################################o\n");
      return NULL;
    }

  /*--------------------------------------------------------*\
  ! Extract the file extension from the input string and use !
  ! it to invoke the appropriate read function.              !
  \*--------------------------------------------------------*/
  str = strrchr(temp->lit_opt[0], '.');
  switch(hash(str))
    {
      case 0x000000017C4DC25C:
        {
          file     = read_eas3(temp->lit_opt[0]);
          if(file == NULL)
            {
              return NULL;
            }

          return file;
        }

      case 0x000000017C4DAF1D:
        {
          file     = read_bwc(temp->lit_opt[0]);
          if(file == NULL)
            {
              return NULL;
            }

          return file;
        }

      default:
        {
          fprintf(stderr, "o##########################################################o\n"\
                          "|   ERROR: File                                            |\n"\
                          "|                                                          |\n"\
                          "|          The format of the specified file is currently   |\n"\
                          "|          not supported.                                  |\n"\
                          "|                                                          |\n"\
                          "o##########################################################o\n");
          return NULL;
        }
    }
}

/*--------------------------------------------------------------------------------------------------------------------*\
!                                                                                                                      !
!   DESCRIPTION:                                                                                                       !
!   ------------                                                                                                       !
!                    This function is a wrapper that is used to invoke the appropriate write func-            	       !
!                    tion according to the file extension supplied by the user or codestream.                 	       ! 
!                                                                                                                      !
!   PARAMETERS:                                                                                                        !
!   -----------                                                                                                        !
!                    Variable                 Type                   Description                                       !
!                    --------                 ----                   -----------                                       !
!                    args                     util_arg_node*       - Arguments/options linked list                     !
!                                                                    for the bwc command-line tool.                    !
!                                                                                                                      !
!                    str                      char*                - Long-/short-name of a bwc com-                    !
!                                                                    mand-line argument or option.                     !
!                                                                                                                      !
!                    data                     bwc_data*            - Structure containing a code-                      !
!                                                                    stream/numerical dataset.                         !
!                                                                                                                      !
!   RETURN VALUE:                                                                                                      !
!   -------------                                                                                                      !
!                    Type                                            Description                                       !
!                    ----                                            -----------                                       !
!                    unsigned char                                 - Returns an unsigned char for                      !
!                                                                    error handling.                                   !
!                                                                                                                      !
!   DEVELOPMENT HISTORY:                                                                                               !
!   --------------------                                                                                               !
!                                                                                                                      !
!                    Date        Author             Change Id   Release     Description                                !
!                    ----        ------             ---------   -------     -----------                                !
!                    14.05.2019  Patrick Vogler     B87D120     V 0.1.0     function created                           !
!                    17.05.2021  Patrick Vogler     B87E7E4     V 0.1.0     clean up                                   !
!                                                                                                                      !
\*--------------------------------------------------------------------------------------------------------------------*/
static uchar
write_file(bwc_cmdl_arg_node  *const  args, 
           bwc_data           *const  data,
           char*                      name)
{
  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  uint64_t            ext_hash;
  uint16_t            new_len;

  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  bwc_cmdl_arg_node  *temp;

  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  char               *str;

  /*-----------------------*\
  ! DEFINE ASSERTIONS:      !
  \*-----------------------*/
  assert(data);
  assert(args);
  assert(name);

  /*--------------------------------------------------------*\
  ! Retrieve the file argument from the linked list and con- !
  ! firm that only one file has been specified.              !
  \*--------------------------------------------------------*/
  temp = retrieve_arg(args, name);
  if((temp              == NULL) || 
     (temp->lit_opt     == NULL) || 
     (temp->lit_opt[0]  == NULL))
    {
      fprintf(stderr, "o##########################################################o\n"\
                      "|   ERROR: File                                            |\n"\
                      "|                                                          |\n"\
                      "|          No File has been specified.                     |\n"\
                      "|                                                          |\n"\
                      "o##########################################################o\n");
      return EXIT_FAILURE;
    }

   if(temp->count > 1)
    {
      fprintf(stderr, "o##########################################################o\n"\
                      "|   ERROR: File                                            |\n"\
                      "|                                                          |\n"\
                      "|          More than one file has been specified.          |\n"\
                      "|                                                          |\n"\
                      "o##########################################################o\n");
      return EXIT_FAILURE;
    }

  /*--------------------------------------------------------*\
  ! If the output is a packed codestream, replace the user   !
  ! supplied with the propper '.bwc' file extension to en-   !
  ! sure uniformity.                                         !
  \*--------------------------------------------------------*/
  if((retrieve_arg(args, "compress") != NULL) && (data->codestream.data != NULL))
    {                
      /*--------------------------------------------------------*\
      ! Extract the filename in the output argument, remove its  !
      ! file extension and calculate the new string length with  !
      ! the '.bwc' extension.                                    !
      \*--------------------------------------------------------*/
      str     = strrchr(temp->lit_opt[0], '.');
      str[1]  = '\0';

      new_len = (strlen(temp->lit_opt[0]) + 4) * sizeof(char);

      /*--------------------------------------------------------*\
      ! Resize the filename string in the output argument and    !
      ! ammend the new file extension.                           !
      \*--------------------------------------------------------*/
      str     = calloc(new_len, sizeof(char));
      if(str == NULL)
        {
          // memory allocation error
          fprintf(stderr, MEMERROR);
          return EXIT_FAILURE;
        }
      sprintf(str, "%s%s", temp->lit_opt[0],"bwc");

      /*--------------------------------------------------------*\
      ! Write the codestream to the filesystem.                  !
      \*--------------------------------------------------------*/
      if(write_bwc(data, str) == EXIT_FAILURE)
        {
          free(str);
          return EXIT_FAILURE;
        }
    }
  /*--------------------------------------------------------*\
  ! If the output is a numerical dataset, generate the ap-   !
  ! propriate filename and write the information to file.    !
  \*--------------------------------------------------------*/
  else if((retrieve_arg(args, "decompress") != NULL) && ((data->field.d != NULL) || 
                                                         (data->field.f != NULL)))
    {
      /*--------------------------------------------------------*\
      ! If no output file has been specified, assemble the file- !
      ! name using the input name and file extension assoc. with !
      ! the codestream. Generate the ext. hash accordingly.      !
      \*--------------------------------------------------------*/
      if(strcmp(name, "decompress") == 0)
        {
          ext_hash = hash(data->info.f_ext);
          str      = strrchr(temp->lit_opt[0], '.');
          str[0]   = '\0';

          new_len  = (strlen(temp->lit_opt[0]) + 
                      strlen(data->info.f_ext) + 7) * sizeof(char);

          str     = calloc(new_len, sizeof(char));
          if(str == NULL)
            {
              // memory allocation error
              fprintf(stderr, MEMERROR);
              return EXIT_FAILURE;
            }
          sprintf(str, "%s%s%s", temp->lit_opt[0], "_d.", data->info.f_ext);
        }
      /*--------------------------------------------------------*\
      ! If an output file has been specified, extract the file   !
      ! extension, evaluate the corresponding hash and check     !
      ! that the codestream supports the file format.            !
      \*--------------------------------------------------------*/
      else
        {
          ext_hash     = hash(strrchr(temp->lit_opt[0], '.') + 1);
          if(ext_hash != hash(data->info.f_ext))
            {
              fprintf(stderr, "o##########################################################o\n"\
                              "|   ERROR: Incompatible File Format                        |\n"\
                              "|                                                          |\n"\
                              "|          The format of the specified file is incom-      |\n"\
                              "|          patible with the information stored in the      |\n"\
                              "|          codestream.                                     |\n"\
                              "|                                                          |\n"\
                              "o##########################################################o\n");
              return EXIT_FAILURE;
            }
          
          str     = calloc(strlen(temp->lit_opt[0]) + 1, sizeof(char));
          if(str == NULL)
            {
              // memory allocation error
              fprintf(stderr, MEMERROR);
              return EXIT_FAILURE;
            }
          strcpy(str, temp->lit_opt[0]);
        }

      /*--------------------------------------------------------*\
      ! Use the extension hash to invoke the appropriate write   !
      ! function.                                                !
      \*--------------------------------------------------------*/
      switch (ext_hash)
        {
          case 0X0000000000B87A592:
            {
              if(write_eas3(data, str) == EXIT_FAILURE)
                {
                  free(str);
                  return EXIT_FAILURE;
                }
              break;
            }

          default:
            {
                fprintf(stderr, "o##########################################################o\n"\
                                "|   ERROR: File                                            |\n"\
                                "|                                                          |\n"\
                                "|          The format of the specified file is currently   |\n"\
                                "|          not supported.                                  |\n"\
                                "|                                                          |\n"\
                                "o##########################################################o\n");
                free(str);
                return EXIT_FAILURE;
            }
        }
    }
  /*--------------------------------------------------------*\
  ! If no apporprite in- or output argument has been speci-  !
  ! fied return an error handle to the function caller.      !
  \*--------------------------------------------------------*/
  else
    {
      fprintf(stderr, "o##########################################################o\n"\
                      "|   ERROR: Data                                            |\n"\
                      "|                                                          |\n"\
                      "|          Output can not be generated because no valid    |\n"\
                      "|          data has been specified.                        |\n"\
                      "|                                                          |\n"\
                      "o##########################################################o\n");
      return EXIT_FAILURE;
    }

  free(str);
  return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------------------------------------*\
!                                                                                                                      !
!   DESCRIPTION:                                                                                                       !
!   ------------                                                                                                       !
!                   |                                                                              |                   !
!                    This function calculates the Peak-Signal-to-Noise-Ratio of a compressed file                      !
!                    in relation to the original reference file. The mean square error and peak                        !
!                    signal to noise ratio are written to the standard output.                                         !
!                                                                                                                      !
!   PARAMETERS:                                                                                                        !
!   -----------                                                                                                        !
!                    Variable                 Type                   Description                                       !
!                    --------                 ----                   -----------                                       !
!                    args                     util_arg_node*       - Arguments/options linked list                     !
!                                                                    for the bwc command-line tool.                    !
!                                                                                                                      !
!                    name                     char*                - Long-/short-name of a bwc com-                    !
!                                                                    mand-line argument or option.                     !
!                                                                                                                      !
!   RETURN VALUE:                                                                                                      !
!   -------------                                                                                                      !
!                    Type                                            Description                                       !
!                    ----                                            -----------                                       !
!                    unsigned char                                 - Returns an unsigned char for                      !
!                                                                    error handling.                                   !
!                                                                                                                      !
!   DEVELOPMENT HISTORY:                                                                                               !
!   --------------------                                                                                               !
!                                                                                                                      !
!                    Date        Author             Change Id   Release     Description                                !
!                    ----        ------             ---------   -------     -----------                                !
!                    12.09.2019  Patrick Vogler     B87D120     V 0.1.0     function created                           !
!                    17.05.2021  Patrick Vogler     B87E7E4     V 0.1.0     clean up                                   !
!                                                                                                                      !
\*--------------------------------------------------------------------------------------------------------------------*/
static uchar
output_analysis(bwc_cmdl_arg_node *const args)
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
  bwc_data           *file, *ref;

  bwc_cmd_opts_ll    *param;

  /*--------------------------------------------------------*\
  !  Load the input and reference files into the appropriate !
  !  file structures.                                        !
  \*--------------------------------------------------------*/
  file     = read_file(args, "input");
  if(file == NULL)
    {
      return EXIT_FAILURE;
    }

  ref     = read_file(args, "reference");
  if(ref == NULL)
    {
      bwc_free_data(file);
      return EXIT_FAILURE;
    }

  dOrig = file->field.d;
  dRef  = ref->field.d;

  fOrig = file->field.f;
  fRef  = ref->field.f;

  if(file->info.nX   == ref->info.nX   &&
     file->info.nY   == ref->info.nY   &&
     file->info.nZ   == ref->info.nZ   &&
     file->info.nTS  == ref->info.nTS  &&
     file->info.nPar == ref->info.nPar)
    {
      size = (uint64_t)ref->info.nX * ref->info.nY  * 
      ref->info.nZ * ref->info.nTS;

      peakVal = -1.7976931348623157e+308;
      PSNR    =
      MSE     = 0;

      if(ref->info.parameter)
        {
          param = ref->info.parameter->root;
          p = 0;
          while(param != NULL)
            {
              minVal = 1.7976931348623157e+308;
              maxVal = -1.7976931348623157e+308;

              if(param->precision == 8)
                {
                  for(i = 0; i < size; ++i)
                    {
                      minVal  = MIN(minVal, (double)dRef[i + p * size]);
                      maxVal  = MAX(maxVal, (double)dRef[i + p * size]);

                      sum     = ((double)dRef[i + p * size] - (double)dOrig[i + p * size]);

                      MSE    += sum * sum;
                    }
                }
              else if(param->precision == 4)
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
              param = param -> next;
              p++;
            }
        }

      MSE /= (double)size * ref->info.nPar;
      PSNR = 20 * log10(peakVal/(2 * sqrt(MSE)));

      printf("==============================================================\n");
      printf("  Mean Square Error:             %*.2f\n", 22, MSE);
      printf("  Peak Signal-to-Noise Ratio:    %*.2f\n", 22, PSNR);
      printf("==============================================================\n");

    }
  /*--------------------------------------------------------*\
  !  Load the input and reference files into the appropriate !
  !  file structures.                                        !
  \*--------------------------------------------------------*/
  bwc_free_data(file);
  bwc_free_data(ref);

  return EXIT_SUCCESS;
}

/**********************************************************************************************************************\
||                       ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                       ||
||                       |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__                        ||
||                       |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]                       ||
||                                                                                                                    ||
\**********************************************************************************************************************/
/*--------------------------------------------------------------------------------------------------------------------*\
!                                                                                                                      !
!   DESCRIPTION:                                                                                                       !
!   ------------                                                                                                       !
!                    This function provides a command-line interface for the BigWhoop compression                      !
!                    library.                                                                                          !
!                                                                                                                      !
!   PARAMETERS:                                                                                                        !
!   -----------                                                                                                        !
!                    Variable                 Type                   Description                                       !
!                    --------                 ----                   -----------                                       !
!                    argc                     int                  - Number of strings pointed to                      !
!                                                                    by argv.                                          !
!                                                                                                                      !
!                    argv                     char**               - Array of arguments.                               !
!                                                                                                                      !
!   RETURN VALUE:                                                                                                      !
!   -------------                                                                                                      !
!                    Type                                            Description                                       !
!                    ----                                            -----------                                       !
!                    int                                           - Return value signaling a normal                   !
!                                                                    or abnormal process exit.                         !
!                                                                                                                      !
!   DEVELOPMENT HISTORY:                                                                                               !
!   --------------------                                                                                               !
!                                                                                                                      !
!                    Date        Author             Change Id   Release     Description                                !
!                    ----        ------             ---------   -------     -----------                                !
!                    17.04.2019  Patrick Vogler     B87D120     V 0.1.0     function created                           !
!                    15.05.2021  Patrick Vogler     B880CA2     V 0.1.0     Clean up.                                  !
!                                                                                                                      !
\*--------------------------------------------------------------------------------------------------------------------*/
int 
main(int    argc, 
     char  *argv[])
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint64_t            size=0;
  uint64_t            i;
  uint8_t             error_handle;

  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  char               *csSize = NULL;
  char               *fdSize = NULL;
  char                buff[200];
  char                rate[10];

  /*-----------------------*\
  ! DEFINE FLOAT VARIABLES: !
  \*-----------------------*/
  double              comp_ratio;
  double              bpd;

  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  bwc_field          *field = NULL;
  bwc_data           *file  = NULL;

  bwc_gl_ctrl        *control;

  bwc_dwt_filter      filter[4];
  bwc_cmdl_arg_node  *args, *temp;

  bwc_cmd_opts_ll    *param;

  /*--------------------------------------------------------*\
  ! Initialize the field and args structures for proper er-  !
  ! ror handling, as well as the error handle itself.        !
  \*--------------------------------------------------------*/
  field        = NULL;
  args         = NULL;

  error_handle = EXIT_SUCCESS;

  /*--------------------------------------------------------*\
  ! Assemble the arg. linked list using the command-line ar- !
  ! guments passed by the function caller.                   !
  \*--------------------------------------------------------*/
  args     = parse_arguments(argc, argv);
  if(args == NULL)
    {
      error_handle = EXIT_FAILURE;
      print_help();
      goto OUT;
    }

  /*--------------------------------------------------------*\
  !                                                          !
  \*--------------------------------------------------------*/
  if(retrieve_arg(args, "compress") != NULL)
    {
      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      if(retrieve_arg(args, "input"))
        file   = read_file(args, "input");
      else
        file   = read_file(args, "compress");

      if(file == NULL)
        {
          error_handle = EXIT_FAILURE;
          goto OUT;
        }

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      /*--------------------------------------------------------*\
      ! Save frequently used variables/structures to temporary   !
      ! variables to make the code more readable.                !
      \*--------------------------------------------------------*/
      field     = bwc_initialize_field(file);
      if(field == NULL)
        {
          error_handle = EXIT_FAILURE;
          goto OUT;
        }
      control = &field->control;

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      temp = retrieve_arg(args, "q_format_range");
      if((temp != NULL) && (temp->count == 1))
        {
          bwc_set_qm(field, (uint8_t)temp->num_opt[0]);
        }

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      // temp = retrieve_arg(args, "wavelet_kernels");
      // if((temp != NULL) && (temp->count == 4) && (temp->dim != 0x00))
      //   {
      //     for(i = 0; i < temp->count; ++i)
      //       {
      //         switch(hash(temp->lit_opt[i]))
      //           {
      //             case 0x000000000B87CF64:
      //               {
      //                 filter[i] = bwc_dwt_9_7;
      //                 break;
      //               }
      //             case 0x00000652AB15772A:
      //               {
      //                 filter[i] = bwc_dwt_5_3;
      //                 break;
      //               }
      //             case 0x000000017C858EFF:
      //               {
      //                 filter[i] = bwc_dwt_5_3;
      //                 break;
      //               }
      //             default:
      //               {
      //                 filter[i] = bwc_dwt_9_7;
      //                 break;
      //               }
      //           }
      //       }
      //     bwc_set_kernels(field,  filter[0], filter[1],
      //                             filter[2], filter[3]);
      //   }

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      temp = retrieve_arg(args, "decomposition_levels");
      if((temp != NULL) && (temp->count == 4) && (temp->dim != 0x00))
        {
          bwc_set_decomp(field, (uint8_t)temp->num_opt[0], (uint8_t)temp->num_opt[1],
                                (uint8_t)temp->num_opt[2], (uint8_t)temp->num_opt[3]);
        }

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      temp = retrieve_arg(args, "tile_size");
      if((temp != NULL) && (temp->count == 4) && (temp->dim != 0x00))
        {
          bwc_set_tiles(field,  (uint64_t)temp->num_opt[0], (uint64_t)temp->num_opt[1],
                                (uint64_t)temp->num_opt[2], (uint64_t)temp->num_opt[3], bwc_tile_sizeof);
        }

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      temp = retrieve_arg(args, "precinct_size");
      if((temp != NULL) && (temp->count == 4) && (temp->dim != 0x00))
        {
          bwc_set_precincts(field,  (uint8_t)temp->num_opt[0], (uint8_t)temp->num_opt[1],
                                    (uint8_t)temp->num_opt[2], (uint8_t)temp->num_opt[3]);
        }

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      temp = retrieve_arg(args, "codeblock_size");
      if((temp != NULL) && (temp->count == 4) && (temp->dim != 0x00))
        {
          bwc_set_codeblocks(field, (uint8_t)temp->num_opt[0], (uint8_t)temp->num_opt[1],
                                    (uint8_t)temp->num_opt[2], (uint8_t)temp->num_opt[3]);
        }

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      // temp = retrieve_arg(args, "quantisation_style");
      // if((temp != NULL) && (temp->count == 1))
      //   {
      //     if(strcmp(temp->lit_opt[0], "NONE"))
      //       bwc_set_quant_style(field, bwc_qt_none);
      //     else
      //       bwc_set_quant_style(field, bwc_qt_derived);
      //   }

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      // temp = retrieve_arg(args, "quantisation_step_size");
      // if((temp != NULL) && (temp->count == 1))
      //   {
      //     bwc_set_quant_step_size(field, temp->num_opt[0]);
      //   }

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      memset(buff, 0, 200 * sizeof(char));

      temp = retrieve_arg(args, "bitrate");
      if((temp != NULL) && (temp->count > 0))
        {
          for(i = 0; i < temp->count && strlen(buff) < 192; ++i)
            {
              sprintf(rate, "%05.3f,", temp->num_opt[i]);
              strcat(buff, rate);
            }
          buff[strlen(buff) - 1] = '0';
        }

      temp = retrieve_arg(args, "compression_ratio");
      if((temp != NULL) && (temp->count == 1))
        {
          sprintf(buff, "%05.3f", ((double)PREC_BIT + 1.0f)/temp->num_opt[0]);
        }

      if(strlen(buff) == 0)
        {
          sprintf(buff, "-");
        }

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      #if defined (_OPENMP)
        temp = retrieve_arg(args, "number_of_threads");
        if((temp != NULL) && (temp->count == 1))
          {
            omp_set_num_threads((uint8_t)temp->num_opt[0]);
          }
      #endif

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      if(retrieve_arg(args, "error_resilience"))
        {
          bwc_set_error_resilience(field);
        }

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      if(bwc_create_compression(field, buff))
        {
          error_handle = EXIT_FAILURE;
          goto OUT;
        }

      if(bwc_compress(field, file))
        {
          error_handle = EXIT_FAILURE;
          goto OUT;
        }


      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      if(retrieve_arg(args, "output"))
        {
          if(write_file(args, file, "output"))
            {
              error_handle = EXIT_FAILURE;
              goto OUT;
            }
        }
      else
        {
          if(write_file(args, file, "compress"))
            {
              error_handle = EXIT_FAILURE;
              goto OUT;
            }
        }
      temp     = retrieve_arg(args, "verbose");
      if(temp != NULL)
        {
          /*--------------------------------------------------------*\
          ! Calculate the original field size, compression ratio and !
          ! bits per datapoint and print the miscellaneous compres-  !
          ! sion information to the standard output.                 !
          \*--------------------------------------------------------*/
          if((temp->count == 1) && (temp->num_opt[0] > 1) && (control->CSsgc != 0))
            {
              printf("==============================================================\n");
              printf("                                                              \n"\
                     "          .:-------------:           .:-------------:         \n"\
                     "         .+++++++++++++++=          :+++++++++++++++-         \n"\
                     "         :+++.        -++=          -++=                      \n"\
                     "         :+++.        -++=          -++=                      \n"\
                     "          -++++++++++++++=   -++=   -++=                      \n"\
                     "         .=++---------=++=   -++=   -++=                      \n"\
                     "         :+++         :++=   -++=   -++=                      \n"\
                     "         .+++=--------=+++---=+++---=+++------------:         \n"\
                     "          -=++++++++++++++++++++++++++++++++++++++++-         \n"\
                     "                                                              \n");
              printf("-----------------   Compression Parameters   -----------------\n\n");
              if((control->CSsgc &0x200) != 0)
                {
                  printf("  Number of Tiles:               %27d\n",  control->nTiles);
                  printf("       - Samples in 1.D:         %27ld\n", control->tileSizeX);
                  printf("       - Samples in 2.D:         %27ld\n", control->tileSizeY);
                  printf("       - Samples in 3.D:         %27ld\n", control->tileSizeZ);
                  printf("       - Timesteps:              %27d\n",  control->tileSizeTS);
                  printf("  ..........................................................\n");
                  printf("\n");
                }
        
              if((control->CSsgc &0xE0) != 0)
                {
                  printf("                                        1.D | 2.D | 3.D | TS\n");
                  if((control->CSsgc &0x20) != 0)
                    {
                        printf("  Decomposition Levels:                  %2d |  %2d |  %2d | %2d\n", control->decompX
                                                                                                   , control->decompY
                                                                                                   , control->decompZ
                                                                                                   , control->decompTS);
                    }
                  if((control->CSsgc &0x40) != 0)
                    {
                        printf("  Precincts  [log2]:                     %2d |  %2d |  %2d | %2d\n", control->precSizeX
                                                                                                   , control->precSizeY
                                                                                                   , control->precSizeZ
                                                                                                   , control->precSizeTS);

                    }
                  if((control->CSsgc &0x80) != 0)
                    {
                        printf("  Codeblocks [log2]:                     %2d |  %2d |  %2d | %2d\n", control->cbX
                                                                                                   , control->cbY
                                                                                                   , control->cbZ
                                                                                                   , control->cbTS);
                    }
                  printf("  ..........................................................\n");
                  printf("\n");
                }

              if((control->CSsgc &0x101) != 0)
                {
                  if((control->CSsgc &0x100) != 0)
                    printf("  Q Number Format:               %27d\n", control->Qm);
                  if((control->CSsgc &0x1) != 0)
                    printf("  Error Resilience:              %27s\n", ((control->error_resilience > 0) ? "True" : "False"));

                  printf("  ..........................................................\n");
                  printf("\n");
                }

              for(i = 0; i < control->nLayers; ++i)
                {
                  printf("  Quality Layer Nr.  %2ld:         %23.2f bpd\n", i + 1, control->bitrate[i]/8.0f);
                }

              printf("\n");
              printf("----------------   Compression Performance   -----------------\n\n");
            }
          else
            {
              printf("==============================================================\n");
            }
        }
    }
  /*--------------------------------------------------------*\
  !                                                          !
  \*--------------------------------------------------------*/
  else if(retrieve_arg(args, "decompress") != NULL)
    {
      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      if(retrieve_arg(args, "input"))
        file   = read_file(args, "input");
      else
        file   = read_file(args, "decompress");

      if(file == NULL)
        {
          error_handle = EXIT_FAILURE;
          goto OUT;
        }

      if(file->codestream.data == NULL)
        {
          fprintf(stderr, "o##########################################################o\n"\
                          "|   ERROR: File                                            |\n"\
                          "|                                                          |\n"\
                          "|          Input file does not contain a packed code-      |\n"\
                          "|          stream.                                         |\n"\
                          "|                                                          |\n"\
                          "o##########################################################o\n");
          
          error_handle = EXIT_FAILURE;
          goto OUT;
        }

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      temp      = retrieve_arg(args, "layer");
      if((temp != NULL) && (temp->count == 1))
        field = bwc_create_decompression(file, (uint8_t)temp->num_opt[0]);
      else
        field = bwc_create_decompression(file, 0);

      if(field == NULL)
        {
          error_handle = EXIT_FAILURE;
          goto OUT;
        }

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      #if defined (_OPENMP)
        temp = retrieve_arg(args, "number_of_threads");
        if((temp != NULL) && (temp->count == 1))
          {
            omp_set_num_threads((uint8_t)temp->num_opt[0]);
          }
      #endif

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      if(bwc_decompress(field, file))
        {
          error_handle = EXIT_FAILURE;
          goto OUT;
        }

      /*--------------------------------------------------------*\
      !                                                          !
      \*--------------------------------------------------------*/
      if(retrieve_arg(args, "output"))
        {
          if(write_file(args, file, "output"))
            {
              error_handle = EXIT_FAILURE;
              goto OUT;
            }
        }
      else
        {
          if(write_file(args, file, "decompress"))
            {
              error_handle = EXIT_FAILURE;
              goto OUT;
            }
        }
    }
  /*--------------------------------------------------------*\
  !                                                          !
  \*--------------------------------------------------------*/
  else if(retrieve_arg(args, "info") != NULL)
    {
      if(retrieve_arg(args, "input"))
        output_info(args, "input");
      else
        output_info(args, "info");
    }
  /*--------------------------------------------------------*\
  !                                                          !
  \*--------------------------------------------------------*/
  else if(retrieve_arg(args, "analysis") != NULL)
    {
      if(output_analysis(args) == EXIT_FAILURE)
        {
          error_handle = EXIT_FAILURE;
          goto OUT;
        }
    }
  /*--------------------------------------------------------*\
  !                                                          !
  \*--------------------------------------------------------*/
  else
    {
      print_help();
      goto OUT;
    }

OUT:
  if(field != NULL)
    {
      bwc_kill_compression(field);
    }

  if(file != NULL)
    {
      bwc_free_data(file);
    }

  if(args != NULL)
    {
      bwc_kill_arg(args);
    }

  fclose(stdin);
  fclose(stdout);
  fclose(stderr);

  return error_handle;
}