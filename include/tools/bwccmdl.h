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
||      STRUCTS:                                                                                                      ||
||      --------                                                                                                      ||
||                   DESCRIPTION NEEDED.                                                                              ||
||                                                                                                                    ||
||      DEVELOPMENT HISTORY:                                                                                          ||
||      --------------------                                                                                          ||
||                                                                                                                    ||
||                   Date        Author             Change Id   Release     Description                               ||
||                   ----        ------             ---------   -------     -----------                               ||
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
#ifndef BWC_CMDL_H
#define BWC_CMDL_H

  /********************************************************************************************************************\
  ||                                         _ _  _ ____ _    _  _ ___  ____                                          ||
  ||                                         | |\ | |    |    |  | |  \ |___                                          ||
  ||                                         | | \| |___ |___ |__| |__/ |___                                          ||
  ||                                                                                                                  ||
  \********************************************************************************************************************/
  #include <inttypes.h>

  /********************************************************************************************************************\
  ||                                          _  _ ____ ____ ____ ____ ____                                           ||
  ||                                          |\/| |__| |    |__/ |  | [__                                            ||
  ||                                          |  | |  | |___ |  \ |__| ___]                                           ||
  ||                                                                                                                  ||
  \********************************************************************************************************************/
  /*------------------------------------------------------------------------------------------------------------------*\
  !   DESCRIPTION:                                                                                                     !
  !   ------------                                                                                                     !
  !                                                                                                                    !
  !                   These macros define minimum and maximum operators as well as an operator used                    !
  !                   to evaluate the size of an array.                                                                !
  !                                                                                                                    !
  !   MACROS:                                                                                                          !
  !   -------                                                                                                          !
  !                   Name                                            Description                                      !
  !                   ----                                            -----------                                      !
  !                   MAX(x, y)                                     - Returns the maximum value of                     !
  !                                                                   two values.                                      !
  !                                                                                                                    !
  !                   MIN(x, y)                                     - Returns the minimum value of                     !
  !                                                                   two values.                                      !
  !                                                                                                                    !
  !                   GET_LEN(x)                                    - Returns the size of an array.                    !
  !                                                                                                                    !
  !   DEVELOPMENT HISTORY:                                                                                             !
  !   --------------------                                                                                             !
  !                                                                                                                    !
  !                   Date        Author             Change Id   Release     Description                               !
  !                   ----        ------             ---------   -------     -----------                               !
  !                   21.03.2018  Patrick Vogler     B87D120     V 0.1.0     macros created                            !
  !                   16.09.2019  Patrick Vogler     B87E7E4     V 0.1.0     Added GET_LEN(X) macro.                   !
  !                                                                                                                    !
  \*------------------------------------------------------------------------------------------------------------------*/
  #define MAX(x, y)               (((x) < (y))?(y):(x))
  #define MIN(x, y)               (((x) > (y))?(y):(x))

  /*------------------------------------------------------------------------------------------------------------------*\
  !   DESCRIPTION:                                                                                                     !
  !   ------------                                                                                                     !
  !                                                                                                                    !
  !                   These Constants define codestream markers used to create the embedded code-                      !
  !                   stream.                                                                                          !
  !                                                                                                                    !
  !   MACROS:                                                                                                          !
  !   -------                                                                                                          !
  !                   Name                                            Description                                      !
  !                   ----                                            -----------                                      !
  !                   SOC                                           - Start of code-stream                             !
  !                   SGI                                           - Global data-set information                      !
  !                   SGC                                           - Global control parameters                        !
  !                   SGR                                           - Global register containing tile                  !
  !                                                                   bitstream size information                       !
  !                   SAX                                           - Auxiliary data-set information                   !
  !                   TLM                                           - Packet lengths: main header                      !
  !                   PLM                                           - Packet lengths: tile-part                        !
  !                   PPM                                           - Quantization default                             !
  !                   COM                                           - Comment                                          !
  !                   EOH                                           - End of header                                    !
  !                   PLT                                           - Packed packet headers: main header               !
  !                   PPT                                           - Packed packet headers: tile-part                 !
  !                   SOT                                           - Start of tile                                    !
  !                   SOP                                           - Start of packet                                  !
  !                   EPH                                           - End of packet header                             !
  !                   SOD                                           - Start of data                                    !
  !                   EOC                                           - End of code-stream                               !
  !                                                                                                                    !
  !   DEVELOPMENT HISTORY:                                                                                             !
  !   --------------------                                                                                             !
  !                                                                                                                    !
  !                   Date        Author             Change Id   Release     Description                               !
  !                   ----        ------             ---------   -------     -----------                               !
  !                   01.12.2017  Patrick Vogler     B87D120     V 0.1.0     macros created                            !
  !                                                                                                                    !
  \*------------------------------------------------------------------------------------------------------------------*/
  #define SOC                     0xFF50
  #define SGI                     0xFF51
  #define SGC                     0xFF52
  #define SAX                     0xFF53
  #define TLM                     0xFF54
  #define PLM                     0xFF55
  #define PPM                     0xFF56
  #define COM                     0xFF57
  #define EOH                     0xFF58
  #define PLT                     0xFF60
  #define PPT                     0xFF61
  #define SOT                     0xFF90
  #define SOP                     0xFF91
  #define EPH                     0xFF92
  #define SOD                     0xFF93
  #define EOC                     0xFFFF

  /*------------------------------------------------------------------------------------------------------------------*\
  !   DESCRIPTION:                                                                                                     !
  !   ------------                                                                                                     !
  !                                                                                                                    !
  !                   These macros define flags used for codestream parsing.                                           !
  !                                                                                                                    !
  !   MACROS:                                                                                                          !
  !   -------                                                                                                          !
  !                   Name                                            Description                                      !
  !                   ----                                            -----------                                      !
  !                   CODESTREAM_OK                                 - No errors detected in Codestream                 !
  !                                                                                                                    !
  !                   CODESTREAM_READ                               - Codestream has been fully read.                  !
  !                                                                                                                    !
  !   DEVELOPMENT HISTORY:                                                                                             !
  !   --------------------                                                                                             !
  !                                                                                                                    !
  !                   Date        Author             Change Id   Release     Description                               !
  !                   ----        ------             ---------   -------     -----------                               !
  !                   01.08.2019  Patrick Vogler     B87D120     V 0.1.0     macros created                            !
  !                                                                                                                    !
  \*------------------------------------------------------------------------------------------------------------------*/
  #define CODESTREAM_OK           0x00
  #define CODESTREAM_READ         0x80

  /*------------------------------------------------------------------------------------------------------------------*\
  !   DESCRIPTION:                                                                                                     !
  !   ------------                                                                                                     !
  !                                                                                                                    !
  !                   These Constants define common error messages used throughout the bwc library.                    !
  !                                                                                                                    !
  !   MACROS:                                                                                                          !
  !   -------                                                                                                          !
  !                   Name                                            Description                                      !
  !                   ----                                            -----------                                      !
  !                   CSTERROR                                      - Codestream parser has encoun-                    !
  !                                                                   tered invalid marker.                            !
  !                                                                                                                    !
  !                   MEMERROR                                      - Allocation has returned a NULL                   !
  !                                                                   pointer due to limited memory.                   !
  !                                                                                                                    !
  !                   RDERROR                                       - Invalid number of bytes read                     !
  !                                                                   from file.                                       !
  !                                                                                                                    !
  !                   WRTERROR                                      - Invalid number of bytes writ-                    !
  !                                                                   ten to file.                                     !
  !                                                                                                                    !
  !   DEVELOPMENT HISTORY:                                                                                             !
  !   --------------------                                                                                             !
  !                                                                                                                    !
  !                   Date        Author             Change Id   Release     Description                               !
  !                   ----        ------             ---------   -------     -----------                               !
  !                   01.12.2017  Patrick Vogler     B87D120     V 0.1.0     macros created                            !
  !                                                                                                                    !
  \*------------------------------------------------------------------------------------------------------------------*/
  #define CSTERROR  "o##########################################################o\n"\
                    "|                ERROR: Invalid Codestream                 |\n"\
                    "o##########################################################o\n"

  #define MEMERROR  "o##########################################################o\n"\
                    "|                   ERROR: Out of Memory                   |\n"\
                    "o##########################################################o\n"
  
  #define RDERROR   "o##########################################################o\n"\
                    "|      ERROR: Invalid Number of Bytes Read from File.      |\n"\
                    "o##########################################################o\n"
 
  #define WRTERROR  "o##########################################################o\n"\
                    "|      ERROR: Invalid Number of Bytes Written to File.     |\n"\
                    "o##########################################################o\n"

  #define GET_DIM(x) (sizeof(x)/sizeof(*(x)))
  /********************************************************************************************************************\
  ||                         ___  ____ ____ ____ _ _  _ ____ ___     ___ _   _ ___  ____ ____                         ||
  ||                         |  \ |___ |__/ |__/ | |  | |___ |  \     |   \_/  |__] |___ [__                          ||
  ||                         |__/ |___ |  \ |  \ |  \/  |___ |__/     |    |   |    |___ ___]                         ||
  ||                                                                                                                  ||
  \********************************************************************************************************************/
  /*------------------------------------------------------------------------------------------------------------------*\
  !   DESCRIPTION:                                                                                                     !
  !   ------------                                                                                                     !
  !                                                                                                                    !
  !                   This structure defines the attributes of a single argument supported by the bwc                  !
  !                   command line tool.                                                                               !
  !                                                                                                                    !
  !   PARAMETERS:                                                                                                      !
  !   -----------                                                                                                      !
  !                   Name                  Type                      Description                                      !
  !                   ----                  ----                      -----------                                      !
  !                   active                char                    - Flag indicating if the argu-                     !
  !                                                                   ment is active.                                  !
  !                                                                                                                    !
  !                   arg_long              char                    - Long form of the argument name.                  !
  !                                                                                                                    !
  !                   arg_short             char                    - Short form of the argument                       !
  !                                                                   name.                                            !
  !                                                                                                                    !
  !                   arg_type              char                    - Flag signaling if the argument                   !
  !                                                                   is optional.                                     !
  !                                                                                                                    !
  !                   type                  char                    - Flag signaling the argument                      !
  !                                                                   type.                                            !
  !                                                                                                                    !
  !                   usage                 char                    - A string of 24 characters de-                    !
  !                                                                   scribing the argument usage.                     !
  !                                                                                                                    !
  !                   definition            char                    - A string of 1024 characters                      !
  !                                                                   containing the argument de-                      !
  !                                                                   scription.                                       !
  !                                                                                                                    !
  !   DEVELOPMENT HISTORY:                                                                                             !
  !   --------------------                                                                                             !
  !                                                                                                                    !
  !                   Date        Author             Change Id   Release     Description                               !
  !                   ----        ------             ---------   -------     -----------                               !
  !                   14.02.2019  Patrick Vogler     B87D120     V 0.1.0     struct created                            !
  !                   26.11.2020  Patrick Vogler     B87E7E4     V 0.1.0     clean up                                  !
  !                                                                                                                    !
  \*------------------------------------------------------------------------------------------------------------------*/
  typedef struct
  {
    char          active;
    char          arg_long[25];
    char          arg_short[3];
    char          arg_type[4];
    char          type[5];
    char          usage[25];
    char          definition[1024];
  } bwc_cmdl_args;

  /*------------------------------------------------------------------------------------------------------------------*\
  !   DESCRIPTION:                                                                                                     !
  !   ------------                                                                                                     !
  !                   This structure describes a linked list which stores all the arguments and their                  !
  !                   attributes supplied to the command line tool by the user.                                        !
  !                                                                                                                    !
  !   PARAMETERS:                                                                                                      !
  !   -----------                                                                                                      !
  !                   Name                  Type                      Description                                      !
  !                   ----                  ----                      -----------                                      !
  !                   hash                  unsigned int(64 bit)    - Uniquely identifiable hash that                  !
  !                                                                   corresponds to the arg/opt name.                 !
  !                                                                                                                    !
  !                   count                 unsigned int(8 bit)     - Counter that signifies the num-                  !
  !                                                                   ber of modifier values stored                    !
  !                                                                   in the linked list node.                         !
  !                                                                                                                    !
  !                   dim                   unsigned int(8 bit)     - Dimension(s) for which the mod-                  !
  !                                                                   ifiers have been defined                         !
  !                                                                                                                    !
  !                   active                char                    - Flag indicating if the arg/opt                   !
  !                                                                   is active.                                       !
  !                                                                                                                    !
  !                   num_opt               double*                 - Array of numerical modifier                      !
  !                                                                   values.                                          !
  !                                                                                                                    !
  !                   lit_opt               char**                  - Character array of literal mod-                  !
  !                                                                   ifier values.                                    !
  !                                                                                                                    !
  !   DEPENDENCIES:                                                                                                    !
  !   -------------                                                                                                    !
  !                   Name                                            TYPE                                             !
  !                   ----                                            ----                                             !
  !                   next                                            opt*                                             !
  !                                                                                                                    !
  !                   root                                            opt*                                             !
  !                                                                                                                    !
  !   DEVELOPMENT HISTORY:                                                                                             !
  !   --------------------                                                                                             !
  !                                                                                                                    !
  !                   Date        Author             Change Id   Release     Description                               !
  !                   ----        ------             ---------   -------     -----------                               !
  !                   26.04.2019  Patrick Vogler     B87D120     V 0.1.0     struct created                            !
  !                   26.11.2020  Patrick Vogler     B87E7E4     V 0.1.0     clean up                                  !
  !                                                                                                                    !
  \*------------------------------------------------------------------------------------------------------------------*/
  typedef struct arg
  {
    uint64_t      hash;
    uint8_t       count;
    uint8_t       dim;
    char          active;
    double       *num_opt;
    char        **lit_opt;
    struct arg   *next;
    struct arg   *root;
  } bwc_cmdl_arg_node;
#endif