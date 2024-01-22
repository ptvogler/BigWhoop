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
  use, intrinsic :: iso_c_binding, only: C_PTR, C_INT64_T, C_INT32_T, C_INT16_T, C_INT8_T, &
                                         C_INT, C_DOUBLE,  C_CHAR,    C_SIGNED_CHAR
  IMPLICIT NONE
  PRIVATE

  !************************************************************************************************!
  !|                          ____ ____ _  _ ____ ___ ____ _  _ ___ ____                          |!
  !|                          |    |  | |\ | [__   |  |__| |\ |  |  [__                           |!
  !|                          |___ |__| | \| ___]  |  |  | | \|  |  ___]                          |!
  !|                                                                                              |!
  !************************************************************************************************!
  ENUM, BIND(C)
    enumerator ::   bwc_dwt_9_7     = 0, &
                    bwc_dwt_5_3     = 1, &
                    bwc_dwt_haar    = 2
  END ENUM
  !*==============================================================================================*!
  ENUM, BIND(C)
    enumerator ::   bwc_prog_LRCP   = 0
  END ENUM
  !*==============================================================================================*!
  ENUM, BIND(C)
    enumerator ::   bwc_qt_none     = 0, &
                    bwc_qt_derived  = 1
  END ENUM
  !*==============================================================================================*!
  ENUM, BIND(C)
    enumerator ::   bwc_tile_sizeof = 0, &
                    bwc_tile_numbof = 1
  END ENUM

  !************************************************************************************************!
  !|            ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____            |!
  !|            |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__             |!
  !|            |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]            |!
  !|                                                                                              |!
  !************************************************************************************************!
  interface
    function initialize_data_f(field, nX, nY, nZ, nTS, nPar, file_extension) result(data) &
             BIND(C, NAME="bwc_initialize_data")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  field
      TYPE(C_PTR)                           ::  data
      
      !*-----------------------*!
      ! DEFINE INT VARIABLES:   !
      !*-----------------------*!
      INTEGER(KIND=C_INT16_T),      VALUE   ::  nX, nY, nZ
      INTEGER(KIND=C_INT8_T),       VALUE   ::  nTS, nPar
      
      !*-----------------------*!
      ! DEFINE CHAR VARIABLES:  !
      !*-----------------------*!
      CHARACTER(KIND=C_CHAR)                ::  file_extension(*)
    end function initialize_data_f
    !*============================================================================================*!
    subroutine add_param_f(data, name, sample, dim, precision) &
               BIND(C, NAME="bwc_add_param")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  data
      
      !*-----------------------*!
      ! DEFINE INT VARIABLES:   !
      !*-----------------------*!
      INTEGER(KIND=C_INT16_T),      VALUE   ::  sample
      INTEGER(KIND=C_INT8_T),       VALUE   ::  precision
      INTEGER(KIND=C_SIGNED_CHAR),  VALUE   ::  dim
      
      !*-----------------------*!
      ! DEFINE CHAR VARIABLES:  !
      !*-----------------------*!
      CHARACTER(KIND=C_CHAR)                ::  name(*)
    end subroutine add_param_f
    !*============================================================================================*!
    subroutine get_data_f(data, buffer, size) &
               BIND(C, NAME="bwc_get_data")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  data
      TYPE(C_PTR),                  VALUE   ::  buffer
      
      !*-----------------------*!
      ! DEFINE INT VARIABLES:   !
      !*-----------------------*!
      INTEGER(KIND=C_INT64_T),      Value   ::  size
    end subroutine get_data_f
    !*============================================================================================*!
    subroutine free_data_f(data) &
               BIND(C, NAME="bwc_free_data")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  data
    end subroutine free_data_f
    !*============================================================================================*!
    subroutine kill_compression_f(field) &
               BIND(C, NAME="bwc_kill_compression")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  field
    end subroutine kill_compression_f
    !*============================================================================================*!
    function initialize_field_f(data) result(field) &
             BIND(C, NAME="bwc_initialize_field")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  data
      TYPE(C_PTR)                           ::  field
    end function initialize_field_f
    !*============================================================================================*!
    subroutine set_error_resilience_f(field) &
               BIND(C, NAME="bwc_set_error_resilience")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  field
    end subroutine set_error_resilience_f
    !*============================================================================================*!
    subroutine set_quantization_style_f(field, quantization_style) &
               BIND(C, NAME="bwc_set_quantization_style")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  field

      !*-----------------------*!
      ! DEFINE INT VARIABLES:   !
      !*-----------------------*!
      INTEGER(KIND=C_INT),          VALUE   ::  quantization_style
    end subroutine set_quantization_style_f
    !*============================================================================================*!
    subroutine set_quantization_step_size_f(field, delta) &
               BIND(C, NAME="bwc_set_quantization_step_size")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  field

      !*-----------------------*!
      ! DEFINE FLOAT VARIABLES: !
      !*-----------------------*!
      REAL(KIND=C_DOUBLE),          VALUE   ::  delta
    end subroutine set_quantization_step_size_f
    !*============================================================================================*!
    subroutine set_progression_f(field, progression) &
               BIND(C, NAME="bwc_set_progression")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  field

      !*-----------------------*!
      ! DEFINE INT VARIABLES:   !
      !*-----------------------*!
      INTEGER(KIND=C_INT),          VALUE   ::  progression
    end subroutine set_progression_f
    !*============================================================================================*!
    subroutine set_kernels_f(field, KernelX, KernelY, KernelZ, KernelTS) &
               BIND(C, NAME="bwc_set_kernels")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  field

      !*-----------------------*!
      ! DEFINE INT VARIABLES:   !
      !*-----------------------*!
      INTEGER(KIND=C_INT),          VALUE   ::  KernelX, KernelY
      INTEGER(KIND=C_INT),          VALUE   ::  KernelZ, KernelTS
    end subroutine set_kernels_f
    !*============================================================================================*!
    subroutine set_decomp_f(field, decompX, decompY, decompZ, decompTS) &
               BIND(C, NAME="bwc_set_decomp")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  field

      !*-----------------------*!
      ! DEFINE INT VARIABLES:   !
      !*-----------------------*!
      INTEGER(KIND=C_INT8_T),       VALUE   ::  decompX, decompY
      INTEGER(KIND=C_INT8_T),       VALUE   ::  decompZ, decompTS
    end subroutine set_decomp_f
    !*============================================================================================*!
    subroutine set_precincts_f(field, pX, pY, pZ, pTS) &
               BIND(C, NAME="bwc_set_precincts")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  field

      !*-----------------------*!
      ! DEFINE INT VARIABLES:   !
      !*-----------------------*!
      INTEGER(KIND=C_INT8_T),       VALUE   ::  pX, pY
      INTEGER(KIND=C_INT8_T),       VALUE   ::  pZ, pTS
    end subroutine set_precincts_f
    !*============================================================================================*!
    subroutine set_codeblocks_f(field, cbX, cbY, cbZ, cbTS) &
               BIND(C, NAME="bwc_set_codeblocks")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  field

      !*-----------------------*!
      ! DEFINE INT VARIABLES:   !
      !*-----------------------*!
      INTEGER(KIND=C_INT8_T),       VALUE   ::  cbX, cbY
      INTEGER(KIND=C_INT8_T),       VALUE   ::  cbZ, cbTS
    end subroutine set_codeblocks_f
    !*============================================================================================*!
    subroutine set_qm_f(field, Qm) &
               BIND(C, NAME="bwc_set_qm")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  field

      !*-----------------------*!
      ! DEFINE INT VARIABLES:   !
      !*-----------------------*!
      INTEGER(KIND=C_INT8_T),       VALUE   ::  Qm
    end subroutine set_qm_f
    !*============================================================================================*!
    subroutine set_tiles_f(field, tilesX, tilesY, tilesZ, tilesTS, instr) &
               BIND(C, NAME="bwc_set_tiles")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  field

      !*-----------------------*!
      ! DEFINE INT VARIABLES:   !
      !*-----------------------*!
      INTEGER(KIND=C_INT64_T),      VALUE   ::  tilesX, tilesY, tilesZ
      INTEGER(KIND=C_INT16_T),      VALUE   ::  tilesTS
      INTEGER(KIND=C_INT),          VALUE   ::  instr
    end subroutine set_tiles_f
    !*============================================================================================*!
    #ifdef _OPENMP
      subroutine set_nThreads_f(field, nThreads) &
                 BIND(C, NAME="bwc_set_nThreads")
        IMPORT
        !*-----------------------*!
        ! DEFINE POINTERS:        !
        !*-----------------------*!
        TYPE(C_PTR),                VALUE   ::  field

        !*-----------------------*!
        ! DEFINE INT VARIABLES:   !
        !*-----------------------*!
        INTEGER(KIND=C_INT8_T),     VALUE   ::  nThreads
      end subroutine set_nThreads_f
    #endif
    !*============================================================================================*!
    function create_compression_f(field, rate_control) result(error_flag) &
             BIND(C, NAME="bwc_create_compression")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  field
      
      !*-----------------------*!
      ! DEFINE INT VARIABLES:   !
      !*-----------------------*!
      INTEGER(KIND=C_INT8_T)                ::  error_flag
      
      !*-----------------------*!
      ! DEFINE CHAR VARIABLES:  !
      !*-----------------------*!
      CHARACTER(KIND=C_CHAR)                ::  rate_control(*)
    end function create_compression_f
    !*============================================================================================*!
    function compress_f(field, data) result(error_flag) &
             BIND(C, NAME="bwc_compress")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  field
      TYPE(C_PTR),                  VALUE   ::  data

      !*-----------------------*!
      ! DEFINE INT VARIABLES:   !
      !*-----------------------*!
      INTEGER(KIND=C_INT8_T)                ::  error_flag
    end function compress_f
    !*============================================================================================*!
    function create_decompression_f(data, layer) result(field) &
             BIND(C, NAME="bwc_create_decompression")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR)                           ::  field
      TYPE(C_PTR),                  VALUE   ::  data

      !*-----------------------*!
      ! DEFINE INT VARIABLES:   !
      !*-----------------------*!
      INTEGER(KIND=C_INT8_T),       VALUE   ::  layer
    end function create_decompression_f
    !*============================================================================================*!
    function decompress_f(field, data) result(error_flag) &
             BIND(C, NAME="bwc_decompress")
      IMPORT
      !*-----------------------*!
      ! DEFINE POINTERS:        !
      !*-----------------------*!
      TYPE(C_PTR),                  VALUE   ::  field
      TYPE(C_PTR),                  VALUE   ::  data

      !*-----------------------*!
      ! DEFINE INT VARIABLES:   !
      !*-----------------------*!
      INTEGER(KIND=C_INT8_T)                ::  error_flag
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

  public ::   bwc_initialize_data,              &
              bwc_get_data,                     &
              bwc_free_data

  public ::   bwc_initialize_field,             &
              bwc_add_param,                    &
              bwc_kill_compression

  public ::   bwc_set_error_resilience,         &
              bwc_set_quantization_style,       &
              bwc_set_quantization_step_size,   &
              bwc_set_progression,              &
              bwc_set_kernels,                  &
              bwc_set_decomp,                   &
              bwc_set_precincts,                &
              bwc_set_codeblocks,               &
              bwc_set_qm,                       &
              bwc_set_tiles,                    &
              bwc_set_nThreads

  public ::   bwc_create_compression,           &
              bwc_compress,                     &
              bwc_create_decompression,         &
              bwc_decompress
end module bwc