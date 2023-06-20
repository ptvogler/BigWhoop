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
||      File:  bwc_bitstream.h                                                                                                      ||
||      -----                                                                                                                       ||
||                                                                                                                                  ||
||      DESCRIPTION:                                                                                                                ||
||      ------------                                                                                                                ||
||                    This file describes a set of functions that can be used to create, manipulate and terminate                   ||
||                    a bitstream. These functions facilitate the creation or reading of a compressed bwc code-                     ||
||                    stream and can emit/extract information on a per bit, symbol (64-bit) or string basis.                        ||
||                                                                                                                                  ||
||                                                                                                                                  ||
||      STRUCTS:                                                                                                                    ||
||      --------                                                                                                                    ||
||      -                                                                                                                           ||
||                                                                                                                                  ||
||      PUBLIC FUNCTIONS:                                                                                                           ||
||      -----------------                                                                                                           ||
||      - bwc_init_stream                                                                                                           ||
||      - bwc_emit_chunck                                                                                                           ||
||      - bwc_emit_symbol                                                                                                           ||
||      - bwc_emit_bit                                                                                                              ||
||      - bwc_get_chunck                                                                                                            ||
||      - bwc_get_symbol                                                                                                            ||
||      - bwc_get_bit                                                                                                               ||
||      - bwc_terminate_stream                                                                                                      ||
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
#ifndef BITSTREAM_H
#define BITSTREAM_H

   /************************************************************************************************************\
   ||                                      _ _  _ ____ _    _  _ ___  ____                                     ||
   ||                                      | |\ | |    |    |  | |  \ |___                                     ||
   ||                                      | | \| |___ |___ |__| |__/ |___                                     ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   #include "types.h"

   /************************************************************************************************************\
   ||                  ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                  ||
   ||                  |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__                   ||
   ||                  |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]                  ||
   ||                                                                                                          ||
   \************************************************************************************************************/
   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: uint32 bytes_used(bwc_stream *const stream)                                               !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function is used to evaluate the number of bytes that have already been                !
   !                written to the allocated bitstream memory block.                                            !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   uint64 
   bytes_used(bwc_stream *const stream);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: bwc_stream* bwc_init_stream(uchar* memory, uint32 size, char instr)                       !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function is used to initialize a bwc bitstream. For encoding, a null pointer           !
   !                is passed as a memory handle and the function will allocate a memory block with the         !
   !                specified stream size. For decoding, a valid memory handle, passed by the function          !
   !                caller, will be stored in the bwc_stream structure. The byte buffer counter t,              !
   !                stream size Lmax and size increment are initialized with their appropriate values.          !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   bwc_stream* 
   bwc_init_stream(uchar* memory, uint32 size, char instr);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void *test(void)                                                                          !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                DESCRIPTION NEEDED                                                                          !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   void
   bwc_emit_chunck(bwc_stream *const stream, const uchar* chunck, const uint64 size);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void *test(void)                                                                          !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                DESCRIPTION NEEDED                                                                          !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   void
   bwc_emit_symbol(bwc_stream *const stream, const uint64 symbol, const uint8 size);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void *test(void)                                                                          !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                DESCRIPTION NEEDED                                                                          !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   void 
   bwc_emit_bit(bwc_stream *const stream, const uint64 bit);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void *test(void)                                                                          !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                DESCRIPTION NEEDED                                                                          !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   void 
   flush_stream(bwc_stream *const stream);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void *test(void)                                                                          !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                DESCRIPTION NEEDED                                                                          !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   uchar*
   bwc_get_chunck(bwc_stream *const stream, const uint64 length);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void *test(void)                                                                          !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                DESCRIPTION NEEDED                                                                          !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   uint64
   bwc_get_symbol(bwc_stream *const stream, const uint8 length);

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
   bwc_get_bit(bwc_stream *const stream);

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
   bwc_terminate_stream(bwc_stream *stream, bwc_packed_stream *const packed_stream);

   /*----------------------------------------------------------------------------------------------------------*\
   !   FUNCTION NAME: void release_packed_stream(bwc_packed_stream *stream)                                     !
   !   --------------                                                                                           !
   !                                                                                                            !
   !   DESCRIPTION:                                                                                             !
   !   ------------                                                                                             !
   !                This function is used to release all the information stored in a packed bitstream           !
   !                and reset the parameters for another (de)compression run.                                   !
   !                                                                                                            !
   \*----------------------------------------------------------------------------------------------------------*/
   void
   release_packed_stream(bwc_packed_stream *stream);
#endif