!*================================================================================================*!
!|                                                                                                |!
!|       /$$$$$$$  /$$                  /$$      /$$ /$$                                          |!
!|      | $$__  $$|__/                 | $$  /$ | $$| $$                                          |!
!|      | $$  \ $$ /$$  /$$$$$$        | $$ /$$$| $$| $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$       |!
!|      | $$$$$$$ | $$ /$$__  $$       | $$/$$ $$ $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$      |!
!|      | $$__  $$| $$| $$  \ $$       | $$$$_  $$$$| $$  \ $$| $$  \ $$| $$  \ $$| $$  \ $$      |!
!|      | $$  \ $$| $$| $$  | $$       | $$$/ \  $$$| $$  | $$| $$  | $$| $$  | $$| $$  | $$      |!
!|      | $$$$$$$/| $$|  $$$$$$$       | $$/   \  $$| $$  | $$|  $$$$$$/|  $$$$$$/| $$$$$$$/      |!
!|      |_______/ |__/ \____  $$       |__/     \__/|__/  |__/ \______/  \______/ | $$____/       |!
!|                     /$$  \ $$                                                  | $$            |!
!|                    |  $$$$$$/                                                  | $$            |!
!|                     \______/                                                   |__/            |!
!|                                                                                                |!
!|  DESCRIPTION:                                                                                  |!
!|  ------------                                                                                  |!
!|                                                                                                |!
!|        This file defines a Fortran interface for the Big Whoop compression library.            |!
!|                                                                                                |!
!|  --------------------------------------------------------------------------------------------  |!
!|  Copyright (c) 2023, High Performance Computing Center - University of Stuttgart               |!
!|                                                                                                |!
!|  Redistribution and use in source and binary forms, with or without modification, are          |!
!|  permitted provided that the following conditions are met:                                     |!
!|                                                                                                |!
!|     (1)   Redistributions of source code must retain the above copyright notice, this list of  |!
!|           conditions and the following disclaimer.                                             |!
!|                                                                                                |!
!|     (2)   Redistributions in binary form must reproduce the above copyright notice, this list  |!
!|           of conditions and the following disclaimer in the documentation and/or other         |!
!|           materials provided with the distribution.                                            |!
!|                                                                                                |!
!|  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS   |!
!|  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF               |!
!|  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE    |!
!|  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,     |!
!|  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF            |!
!|  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)        |!
!|  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR      |!
!|  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  |!
!|  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                            |!
!|                                                                                                |!
!*================================================================================================*!
module bwc
  !************************************************************************************************!
  !|                               _ _  _ ____ _    _  _ ___  ____                                |!
  !|                               | |\ | |    |    |  | |  \ |___                                |!
  !|                               | | \| |___ |___ |__| |__/ |___                                |!
  !|                                                                                              |!
  !************************************************************************************************!
  use, intrinsic :: iso_c_binding, only: c_ptr, c_int64_t, c_int32_t, c_int16_t, c_int8_t, &
                                         c_int, c_double,  c_char,    c_signed_char
  implicit none
  private

  !************************************************************************************************!
  !|                          ____ ____ _  _ ____ ___ ____ _  _ ___ ____                          |!
  !|                          |    |  | |\ | [__   |  |__| |\ |  |  [__                           |!
  !|                          |___ |__| | \| ___]  |  |  | | \|  |  ___]                          |!
  !|                                                                                              |!
  !************************************************************************************************!
  enum, bind(c)
    enumerator ::   bwc_dwt_9_7     = 0, &
                    bwc_dwt_5_3     = 1, &
                    bwc_dwt_haar    = 2
  end enum
  !*==============================================================================================*!
  enum, bind(c)
    enumerator ::   bwc_prog_lrcp   = 0
  end enum
  !*==============================================================================================*!
  enum, bind(c)
    enumerator ::   bwc_qt_none     = 0, &
                    bwc_qt_derived  = 1
  end enum
  !*==============================================================================================*!
  enum, bind(c)
    enumerator ::   bwc_tile_sizeof = 0, &
                    bwc_tile_numbof = 1
  end enum

  !************************************************************************************************!
  !|            ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____            |!
  !|            |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__             |!
  !|            |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]            |!
  !|                                                                                              |!
  !************************************************************************************************!
  interface
    function init_stream_f(inpbuf, outbuf, mode) result(stream) &
             bind(c, name="bwc_init_stream")
      import
      type(c_ptr)                           ::  stream
      type(c_ptr),                  value   ::  inpbuf
      type(c_ptr),                  value   ::  outbuf
      integer(kind=c_int),          value   ::  mode
    end function init_stream_f
    !*============================================================================================*!
    function alloc_coder_f(nx, ny, nz, nts, npar, prec) result(codec) &
             bind(c, name="bwc_alloc_coder")
      import
      type(c_ptr)                           ::  codec
      integer(kind=c_int64_t),      value   ::  nx, ny, nz, nts
      integer(kind=c_int8_t),       value   ::  npar
      integer(kind=c_int),          value   ::  prec
    end function alloc_coder_f
    !*============================================================================================*!
    function alloc_decoder_f() result(codec) &
             bind(c, name="bwc_alloc_decoder")
      import
      type(c_ptr)                           ::  codec
    end function alloc_decoder_f
    !*============================================================================================*!
    subroutine free_codec_f(codec) &
               bind(c, name="bwc_free_codec")
      import
      type(c_ptr),                  value   ::  codec
    end subroutine free_codec_f
    !*============================================================================================*!
    subroutine set_error_resilience_f(codec) &
               bind(c, name="bwc_set_error_resilience")
      import
      type(c_ptr),                  value   ::  codec
    end subroutine set_error_resilience_f
    !*============================================================================================*!
    subroutine set_decomp_f(codec, decompx, decompy, decompz, decompts) &
               bind(c, name="bwc_set_decomp")
      import
      type(c_ptr),                  value   ::  codec
      integer(kind=c_int8_t),       value   ::  decompx, decompy
      integer(kind=c_int8_t),       value   ::  decompz, decompts
    end subroutine set_decomp_f
    !*============================================================================================*!
    subroutine set_precincts_f(codec, px, py, pz, pts) &
               bind(c, name="bwc_set_precincts")
      import
      type(c_ptr),                  value   ::  codec
      integer(kind=c_int8_t),       value   ::  px, py
      integer(kind=c_int8_t),       value   ::  pz, pts
    end subroutine set_precincts_f
    !*============================================================================================*!
    subroutine set_codeblocks_f(codec, cbx, cby, cbz, cbts) &
               bind(c, name="bwc_set_codeblocks")
      import
      type(c_ptr),                  value   ::  codec
      integer(kind=c_int8_t),       value   ::  cbx, cby
      integer(kind=c_int8_t),       value   ::  cbz, cbts
    end subroutine set_codeblocks_f
    !*============================================================================================*!
    subroutine set_qm_f(codec, qm) &
               bind(c, name="bwc_set_qm")
      import
      type(c_ptr),                  value   ::  codec
      integer(kind=c_int8_t),       value   ::  qm
    end subroutine set_qm_f
    !*============================================================================================*!
    subroutine set_tiles_f(codec, tilesx, tilesy, tilesz, tilests, instr) &
               bind(c, name="bwc_set_tiles")
      import
      type(c_ptr),                  value   ::  codec
      integer(kind=c_int64_t),      value   ::  tilesx, tilesy, tilesz, tilests
      integer(kind=c_int),          value   ::  instr
    end subroutine set_tiles_f
    !*============================================================================================*!
    function create_compression_f(codec, stream, rate_control) result(error_flag) &
             bind(c, name="bwc_create_compression")
      import
      integer(kind=c_int8_t)                ::  error_flag
      type(c_ptr),                  value   ::  codec
      type(c_ptr),                  value   ::  stream
      character(kind=c_char)                ::  rate_control(*)
    end function create_compression_f
    !*============================================================================================*!
    function compress_f(codec, stream) result(error_flag) &
             bind(c, name="bwc_compress")
      import
      integer(kind=c_int8_t)                ::  error_flag
      type(c_ptr),                  value   ::  codec
      type(c_ptr),                  value   ::  stream
    end function compress_f
    !*============================================================================================*!
    function create_decompression_f(codec, stream, layer) result(error_flag) &
             bind(c, name="bwc_create_decompression")
      import
      integer(kind=c_int8_t)                ::  error_flag
      type(c_ptr),                  value   ::  codec
      type(c_ptr),                  value   ::  stream
      integer(kind=c_int8_t),       value   ::  layer
    end function create_decompression_f
    !*============================================================================================*!
    function decompress_f(codec, stream) result(error_flag) &
             bind(c, name="bwc_decompress")
      import
      integer(kind=c_int8_t)                ::  error_flag
      type(c_ptr),                  value   ::  codec
      type(c_ptr),                  value   ::  stream
    end function decompress_f
  end interface

  !************************************************************************************************!
  !|                           _ _  _ ___ ____ ____ ____ ____ ____ ____                           |!
  !|                           | |\ |  |  |___ |__/ |___ |__| |    |___                           |!
  !|                           | | \|  |  |___ |  \ |    |  | |___ |___                           |!
  !|                                                                                              |!
  !************************************************************************************************!
  public  ::  bwc_dwt_9_7,                      &
              bwc_dwt_5_3,                      &
              bwc_dwt_haar

  public ::   bwc_prog_LRCP
  
  public ::   bwc_qt_none,                      &
              bwc_qt_derived
  
  public ::   bwc_tile_sizeof,                  &
              bwc_tile_numbof 

  public ::   bwc_init_stream,                  &
              bwc_alloc_coder,                  &
              bwc_alloc_decoder,                &
              bwc_free_codec

  public ::   bwc_set_error_resilience,         &
              bwc_set_decomp,                   &
              bwc_set_precincts,                &
              bwc_set_codeblocks,               &
              bwc_set_qm,                       &
              bwc_set_tiles

  public ::   bwc_create_compression,           &
              bwc_compress,                     &
              bwc_create_decompression,         &
              bwc_decompress
end module bwc