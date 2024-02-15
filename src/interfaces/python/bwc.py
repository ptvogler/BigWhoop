#/=====================================================================================================================\#
#|                                                                                                                     |#
#|                 /$$$$$$$  /$$                  /$$      /$$ /$$                                                     |#
#|                | $$__  $$|__/                 | $$  /$ | $$| $$                                                     |#
#|                | $$  \ $$ /$$  /$$$$$$        | $$ /$$$| $$| $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$                  |#
#!                | $$$$$$$ | $$ /$$__  $$       | $$/$$ $$ $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$                 |#
#|                | $$__  $$| $$| $$  \ $$       | $$$$_  $$$$| $$  \ $$| $$  \ $$| $$  \ $$| $$  \ $$                 |#
#|                | $$  \ $$| $$| $$  | $$       | $$$/ \  $$$| $$  | $$| $$  | $$| $$  | $$| $$  | $$                 |#
#|                | $$$$$$$/| $$|  $$$$$$$       | $$/   \  $$| $$  | $$|  $$$$$$/|  $$$$$$/| $$$$$$$/                 |#
#|                |_______/ |__/ \____  $$       |__/     \__/|__/  |__/ \______/  \______/ | $$____/                  |#
#|                               /$$  \ $$                                                  | $$                       |#
#|                              |  $$$$$$/                                                  | $$                       |#
#|                               \______/                                                   |__/                       |#
#|                                                                                                                     |#
#|      DESCRIPTION:                                                                                                   |#
#|      ------------                                                                                                   |#
#|                                                                                                                     |#
#|                   This file defines a Python interface for the Big Whoop compression library                        |#
#|                                                                                                                     |#
#|      FUNCTIONS:                                                                                                     |#
#|      ----------                                                                                                     |#
#|                 - bwc_free_field                                                                                    |#
#|                 - bwc_add_param                                                                                     |#
#|                 - bwc_set_tiles                                                                                     |#
#|                 - bwc_set_kernels                                                                                   |#
#|                 - bwc_set_decomp                                                                                    |#
#|                 - bwc_set_precincts                                                                                 |#
#|                 - bwc_set_codeblocks                                                                                |#
#|                 - bwc_set_progression                                                                               |#
#|                 - bwc_set_error_resilience                                                                          |#
#|                 - bwc_set_quantization_style                                                                        |#
#|                 - bwc_set_qm                                                                                        |#
#|                 - bwc_set_quantization_step_size                                                                    |#
#|                 - bwc_set_memory_limit                                                                              |#
#|                 - bwc_compress                                                                                      |#
#|                 - bwc_decompress                                                                                    |#
#|                                                                                                                     |#
#|                                                                                                                     |#
#|      DEVELOPMENT HISTORY:                                                                                           |#
#|      --------------------                                                                                           |#
#|                                                                                                                     |#
#|                   Date        Author             Change Id   Release     Description                                |#
#|                   ----        ------             ---------   -------     -----------                                |#
#|                   17.03.2021  Patrick Vogler     B87D120     V 0.1.0     Python Module created                      |#
#|                                                                                                                     |#
#|                                                                                                                     |#
#|       ------------------------------------------------------------------------------------------------------        |#
#|                                                                                                                     |#
#|       Copyright (c) 2023, High Performance Computing Center - University of Stuttgart                               |#
#|                                                                                                                     |#
#|       Redistribution and use in source and binary forms, with or without modification, are permitted                |#
#|       provided that the following conditions are met:                                                               |#
#|                                                                                                                     |#
#|          (1)   Redistributions of source code must retain the above copyright notice, this list of                  |#
#|                conditions and the following disclaimer.                                                             |#
#|                                                                                                                     |#
#|          (2)   Redistributions in binary form must reproduce the above copyright notice, this list                  |#
#|                of conditions and the following disclaimer in the documentation and/or other materials               |#
#|                provided with the distribution.                                                                      |#
#|                                                                                                                     |#
#|       THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED        |#
#|       WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A        |#
#|       PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR        |#
#|       ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT            |#
#|       LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS           |#
#|       INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR        |#
#|       TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF          |#
#|       ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                    |#
#|                                                                                                                     |#
#\=====================================================================================================================/#
#/=====================================================================================================================\#
#|                                          _ _  _ ____ _    _  _ ___  ____                                            |#
#|                                          | |\ | |    |    |  | |  \ |___                                            |#
#|                                          | | \| |___ |___ |__| |__/ |___                                            |#
#|                                                                                                                     |#
#\=====================================================================================================================/#
import  ctypes

from    numpy.ctypeslib import  ndpointer
from    enum            import  Enum

libbwc = ctypes.cdll.LoadLibrary("../../../lib/libbwc.so")

#/=====================================================================================================================\#
#|                                     ____ ____ _  _ ____ ___ ____ _  _ ___ ____                                      |#
#|                                     |    |  | |\ | [__   |  |__| |\ |  |  [__                                       |#
#|                                     |___ |__| | \| ___]  |  |  | | \|  |  ___]                                      |#
#|                                                                                                                     |#
#\=====================================================================================================================/#
class _const():
  class DWT(Enum):
    CDF_9_7     = 0
    LEGALL_5_3  = 1
    HAAR        = 2
  
  class PROG(Enum):
    LRCP        = 0

  class QT(object):
    NONE        = 0
    DERIVED     = 1

  class TILE(Enum):
    SIZEOF      = 0
    NUMBOF      = 1
const = _const()

#/=====================================================================================================================\#
#|                       ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____                        |#
#|                       |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__                         |#
#|                       |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]                        |#
#|                                                                                                                     |#
#\=====================================================================================================================/#
def free_data(data):
  fun = libbwc.bwc_free_data
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p]
  fun(data)
#=======================================================================================================================#
def initialize_data(data, nX, nY, nZ, nTS, nPar, file_extension):
  fun = libbwc.bwc_initialize_data
  fun.restype  = ctypes.c_void_p
  fun.argtypes = [ndpointer(ctypes.c_double, flags="C_CONTIGUOUS"),
                  ctypes.c_uint64,
                  ctypes.c_uint64,
                  ctypes.c_uint64,
                  ctypes.c_uint16,
                  ctypes.c_uint8,
                  ctypes.c_char_p]
  return ctypes.c_void_p(fun(data, nX, nY, nZ, nTS, nPar, file_extension.encode('utf-8')))
#=======================================================================================================================#
def get_data(data, buffer, size):
  fun = libbwc.bwc_get_data
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p,
                  ndpointer(ctypes.c_double, flags="C_CONTIGUOUS"),
                  ctypes.c_uint64]
  fun(data, buffer, size)
#=======================================================================================================================#
def add_param(data, name, sample, dim, precision):
  fun = libbwc.bwc_add_param
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p,
                  ctypes.c_char_p,
                  ctypes.c_uint16,
                  ctypes.c_uint8,
                  ctypes.c_uint8]
  fun(data, name.encode('utf-8'), sample, dim, precision)
#=======================================================================================================================#
def kill_compression(field):
  fun = libbwc.bwc_kill_compression
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p]
  fun(field)
#=======================================================================================================================#
def initialize_field(data):
  fun = libbwc.bwc_initialize_field
  fun.restype  = ctypes.c_void_p
  fun.argtypes = [ctypes.c_void_p]
  return ctypes.c_void_p(fun(data))
#=======================================================================================================================#
def set_codeblocks(field, cbX, cbY, cbZ, cbTS):
  fun = libbwc.bwc_set_codeblocks
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p, ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8]
  fun(field, cbX, cbY, cbZ, cbTS)
#=======================================================================================================================#
def set_decomp(field, decompX, decompY, decompZ, decompTS):
  fun = libbwc.bwc_set_decomp
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p, ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8]
  fun(field, decompX, decompY, decompZ, decompTS)
#=======================================================================================================================#
def set_qm(field, Qm):
  fun = libbwc.bwc_set_qm
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p, ctypes.c_int8]
  fun(field, Qm)
#=======================================================================================================================#
def set_tiles(field, tilesX, tilesY, tilesZ, tilesTS, instr):
  fun = libbwc.bwc_set_tiles
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p, ctypes.c_uint64, ctypes.c_uint64, ctypes.c_uint64, ctypes.c_uint64, ctypes.c_char_p]
  fun(field, tilesX, tilesY, tilesZ, tilesTS, instr.encode('utf-8'))
#=======================================================================================================================#
def set_precincts(field, pX, pY, pZ, pTS):
  fun = libbwc.bwc_set_precincts
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p, ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8, ctypes.c_uint8]
  fun(field, pX, pY, pZ, pTS)
#=======================================================================================================================#
def create_compression(field, rate_control):
  fun = libbwc.bwc_create_compression
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p, ctypes.c_char_p]
  fun(field, rate_control.encode('utf-8'))
#=======================================================================================================================#
def compress(field, data):
  fun = libbwc.bwc_compress
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
  fun(field, data)
#=======================================================================================================================#
def create_decompression(field, data):
  fun = libbwc.bwc_create_decompression
  fun.restype  = ctypes.c_void_p
  fun.argtypes = [ctypes.c_void_p, ctypes.c_uint8]
  return ctypes.c_void_p(fun(field, data))
#=======================================================================================================================#
def decompress(field, data):
  fun = libbwc.bwc_decompress
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
  fun(field, data)