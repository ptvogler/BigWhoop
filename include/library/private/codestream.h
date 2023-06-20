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
||      File:  codestream.h                                                                                                          ||
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
||                            28.05.2018  Patrick Vogler     B87D120     V 0.1.0     header file created                            ||
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
#ifndef CODESTREAM_H
#define CODESTREAM_H

   /************************************************************************************************************\
   ||                                      _ _  _ ____ _    _  _ ___  ____                                     ||
   ||                                      | |\ | |    |    |  | |  \ |___                                     ||
   ||                                      | | \| |___ |___ |__| |__/ |___                                     ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   #include "types.h"

   /************************************************************************************************************\
   ||                                      _  _ ____ ____ ____ ____ ____                                       ||
   ||                                      |\/| |__| |    |__/ |  | [__                                        ||
   ||                                      |  | |  | |___ |  \ |__| ___]                                       ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   /*----------------------------------------------------------------------------------------------------------*\
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                These macros define stream manipulation operations to rewind, foward and get access         !
   !                to the current memory position of a bwc_stream.                                             !
   !                                                                                                            !
   !   Macros:                                                                                                  !
   !   -------                                                                                                  !
   !                Macro                                Description                                            !
   !                -----                                -----------                                            !
   !                rewind_stream                      - Rewinds a stream by a user specified amount (delta)    !
   !                                                     of bits.                                               !
   !                                                                                                            !
   !                foward_stream                      - Fowards a stream by a user specified amount (delta)    !
   !                                                     of bits.                                               !
   !                                                                                                            !
   !                get_access                         - Get an access pointer to the current memory position   !
   !                                                     of a bwc_stream.                                       !
   !                                                                                                            !
   !   DEVELOPMENT HISTORY:                                                                                     !
   !   --------------------                                                                                     !
   !                                                                                                            !
   !                Date        Author             Change Id   Release     Description Of Change                !
   !                ----        ------             ---------   -------     ---------------------                !
   !                07.08.2019  Patrick Vogler     B87D120     V 0.1.0     Macros created                       !
   \*----------------------------------------------------------------------------------------------------------*/
   #define rewind_stream(stream, delta)                        \
   {                                                           \
      stream->L -= delta;                                      \
   }
   
   #define forward_stream(stream, delta)                       \
   {                                                           \
      stream->L += delta;                                      \
   }
   
   #define get_access(stream) (uchar*)stream->memory + stream->L

   /************************************************************************************************************\
   ||                  ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                  ||
   ||                  |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__                   ||
   ||                  |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]                  ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void *test(void)                                                                          !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                DESCRIPTION NEEDED                                                                          !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   uchar
   assemble_main_header(bwc_field *const field);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void *test(void)                                                                          !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                DESCRIPTION NEEDED                                                                          !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   bwc_field*
   bwc_parse_main_header(bwc_data *const data,bwc_stream *const stream);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void *test(void)                                                                          !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                DESCRIPTION NEEDED                                                                          !
   !                                                                                                            !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   uchar
   codestream_write_aux(bwc_packed_stream *const header, bwc_packed_stream *const aux);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void *test(void)                                                                          !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                DESCRIPTION NEEDED                                                                          !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   uchar
   codestream_write_com(bwc_packed_stream *const header, bwc_packed_stream *const com);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: bwc_packed_stream* assemble_codestream(bwc_field *const field)                            !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                DESCRIPTION NEEDED                                                                          !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   bwc_packed_stream*
   assemble_codestream(bwc_field *const field);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: bwc_field* parse_codestream(bwc_data *const data)                                         !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                DESCRIPTION NEEDED                                                                          !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   bwc_field*
   parse_codestream(bwc_data *const data, uint8 const layer);
#endif