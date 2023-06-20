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
||      File:  macros.h                                                                                                             ||
||      -----                                                                                                                       ||
||                                                                                                                                  ||
||      DESCRIPTION:                                                                                                                ||
||      ------------                                                                                                                ||
||      DESCRIPTION NEEDED.                                                                                                         ||
||                                                                                                                                  ||
||      STRUCTS:                                                                                                                    ||
||      --------                                                                                                                    ||
||                                                                                                                                  ||
||      PUBLIC FUNCTIONS:                                                                                                           ||
||      -----------------                                                                                                           ||
||                                                                                                                                  ||
||      DEVELOPMENT HISTORY:                                                                                                        ||
||      --------------------                                                                                                        ||
||                                                                                                                                  ||
||                            Date        Author             Change Id   Release     Description Of Change                          ||
||                            ----        ------             ---------   -------     ---------------------                          ||
||                            29.05.2017  Patrick Vogler     B87D120     V 0.1.0     header file created                            ||
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
#ifndef BWC_MACROSS_H
#define BWC_MACROSS_H
   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                These Macros are used to calculate the maximum and minimum between two values.              !
   !                                                                                                            !
   !   Macros:                                                                                                  !
   !   -------                                                                                                  !
   !                Macro                                Description                                            !
   !                -----                                -----------                                            !
   !                MAX(x, y)                          - Returns the maximum value of two values.               !
   !                                                                                                            !
   !                MIN(x, y)                          - Returns the minimum value of two values.               !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                21.03.2018  Patrick Vogler     B87D120     V 0.1.0     Macros created                       !
   \*----------------------------------------------------------------------------------------------------------*/
   #define MAX(x, y)   (((x) < (y))?(y):(x))
   #define MIN(x, y)   (((x) > (y))?(y):(x))

   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This macro is used to evaluate the size of an array.                                        !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                16.09.2019  Patrick Vogler     B87D120     V 0.1.0     Macros created                       !
   \*----------------------------------------------------------------------------------------------------------*/
   #define GET_DIM(x) (sizeof(x)/sizeof(*(x)))

   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !                These Constants describe the codestream markers used to create the embedded codestream.     !
   !                                                                                                            !
   !   CONSTANTS:                                                                                               !
   !   -----------                                                                                              !
   !                Constant                             Description                                            !
   !                --------                             -----------                                            !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                01.12.2017  Patrick Vogler     B87D120     V 0.1.0     Constants created                    !
   \*----------------------------------------------------------------------------------------------------------*/
   #define MEMERROR "o##########################################################o\n"\
                    "|                   ERROR: Out of memory                   |\n"\
                    "o##########################################################o\n"

   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                                                                                                            !
   !                These Constants describe the codestream markers used to create the embedded codestream.     !
   !                                                                                                            !
   !   CONSTANTS:                                                                                               !
   !   -----------                                                                                              !
   !                Constant                             Description                                            !
   !                --------                             -----------                                            !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                01.12.2017  Patrick Vogler     B87D120     V 0.1.0     Constants created                    !
   \*----------------------------------------------------------------------------------------------------------*/
   #define CSERROR "o##########################################################o\n"\
                   "|                ERROR: Invalid Codestream                 |\n"\
                   "o##########################################################o\n"
#endif