#*================================================================================================*#
#|                                                                                                |#
#|       /$$$$$$$  /$$                  /$$      /$$ /$$                                          |#
#|      | $$__  $$|__/                 | $$  /$ | $$| $$                                          |#
#|      | $$  \ $$ /$$  /$$$$$$        | $$ /$$$| $$| $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$       |#
#|      | $$$$$$$ | $$ /$$__  $$       | $$/$$ $$ $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$      |#
#|      | $$__  $$| $$| $$  \ $$       | $$$$_  $$$$| $$  \ $$| $$  \ $$| $$  \ $$| $$  \ $$      |#
#|      | $$  \ $$| $$| $$  | $$       | $$$/ \  $$$| $$  | $$| $$  | $$| $$  | $$| $$  | $$      |#
#|      | $$$$$$$/| $$|  $$$$$$$       | $$/   \  $$| $$  | $$|  $$$$$$/|  $$$$$$/| $$$$$$$/      |#
#|      |_______/ |__/ \____  $$       |__/     \__/|__/  |__/ \______/  \______/ | $$____/       |#
#|                     /$$  \ $$                                                  | $$            |#
#|                    |  $$$$$$/                                                  | $$            |#
#|                     \______/                                                   |__/            |#
#|                                                                                                |#
#|  DESCRIPTION:                                                                                  |#
#|  ------------                                                                                  |#
#|                                                                                                |#
#|        This file defines a Python interface for the Big Whoop compression library.             |#
#|                                                                                                |#
#|  --------------------------------------------------------------------------------------------  |#
#|  Copyright (c) 2023, High Performance Computing Center - University of Stuttgart               |#
#|                                                                                                |#
#|  Redistribution and use in source and binary forms, with or without modification, are          |#
#|  permitted provided that the following conditions are met:                                     |#
#|                                                                                                |#
#|     (1)   Redistributions of source code must retain the above copyright notice, this list of  |#
#|           conditions and the following disclaimer.                                             |#
#|                                                                                                |#
#|     (2)   Redistributions in binary form must reproduce the above copyright notice, this list  |#
#|           of conditions and the following disclaimer in the documentation and/or other         |#
#|           materials provided with the distribution.                                            |#
#|                                                                                                |#
#|  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS   |#
#|  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF               |#
#|  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE    |#
#|  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,     |#
#|  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF            |#
#|  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)        |#
#|  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR      |#
#|  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  |#
#|  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                            |#
#|                                                                                                |#
#*================================================================================================*#
#**************************************************************************************************#
#|                                _ _  _ ____ _    _  _ ___  ____                                 |#
#|                                | |\ | |    |    |  | |  \ |___                                 |#
#|                                | | \| |___ |___ |__| |__/ |___                                 |#
#|                                                                                                |#
#**************************************************************************************************#
import  ctypes

from    numpy.ctypeslib import  ndpointer
from    enum            import  Enum

libbwc = ctypes.cdll.LoadLibrary("../../../lib/libbwc.so")

#**************************************************************************************************#
#|                           ____ ____ _  _ ____ ___ ____ _  _ ___ ____                           |#
#|                           |    |  | |\ | [__   |  |__| |\ |  |  [__                            |#
#|                           |___ |__| | \| ___]  |  |  | | \|  |  ___]                           |#
#|                                                                                                |#
#**************************************************************************************************#
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

#**************************************************************************************************#
#|             ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____             |#
#|             |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__              |#
#|             |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]             |#
#|                                                                                                |#
#**************************************************************************************************#
def init_stream(inpbuf, outbuf, mode):
  fun = libbwc.bwc_init_stream
  fun.restype  = ctypes.c_void_p
  fun.argtypes = [ndpointer(ctypes.c_double, flags="C_CONTIGUOUS"),
                  ndpointer(ctypes.c_double, flags="C_CONTIGUOUS"),
                  ctypes.c_char_p]
  return ctypes.c_void_p(fun(inpbuf, outbuf, mode.encode('utf-8')))
#==================================================================================================#
def alloc_coder(nX, nY, nZ, nTS, nPar, prec):
  fun = libbwc.bwc_alloc_coder
  fun.restype  = ctypes.c_void_p
  fun.argtypes = [ctypes.c_uint64,
                  ctypes.c_uint64,
                  ctypes.c_uint64,
                  ctypes.c_uint8,
                  ctypes.c_char_p]
  return ctypes.c_void_p(fun(nX, nY, nZ, nTS, nPar, prec.encode('utf-8')))
#==================================================================================================#
def alloc_decoder():
  fun = libbwc.bwc_alloc_decoder
  fun.restype  = ctypes.c_void_p
  fun.argtypes = []
  return ctypes.c_void_p(fun())
#==================================================================================================#
def free_codec(codec):
  fun = libbwc.bwc_free_codec
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p]
  fun(codec)
#==================================================================================================#
def set_codeblocks(codec, cbX, cbY, cbZ, cbTS):
  fun = libbwc.bwc_set_codeblocks
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p,
                  ctypes.c_uint8,
                  ctypes.c_uint8,
                  ctypes.c_uint8,
                  ctypes.c_uint8]
  fun(codec, cbX, cbY, cbZ, cbTS)
#==================================================================================================#
def set_decomp(codec, decompX, decompY, decompZ, decompTS):
  fun = libbwc.bwc_set_decomp
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p,
                  ctypes.c_uint8,
                  ctypes.c_uint8,
                  ctypes.c_uint8,
                  ctypes.c_uint8]
  fun(codec, decompX, decompY, decompZ, decompTS)
#==================================================================================================#
def set_qm(codec, Qm):
  fun = libbwc.bwc_set_qm
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p,
                  ctypes.c_uint8]
  fun(codec, Qm)
#==================================================================================================#
def set_tiles(codec, tilesX, tilesY, tilesZ, tilesTS, instr):
  fun = libbwc.bwc_set_tiles
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p,
                  ctypes.c_uint64,
                  ctypes.c_uint64,
                  ctypes.c_uint64,
                  ctypes.c_uint64,
                  ctypes.c_char_p]
  fun(codec, tilesX, tilesY, tilesZ, tilesTS, instr.encode('utf-8'))
#==================================================================================================#
def set_precincts(codec, pX, pY, pZ, pTS):
  fun = libbwc.bwc_set_precincts
  fun.restype  = None
  fun.argtypes = [ctypes.c_void_p,
                  ctypes.c_uint8,
                  ctypes.c_uint8,
                  ctypes.c_uint8,
                  ctypes.c_uint8]
  fun(codec, pX, pY, pZ, pTS)
#==================================================================================================#
def create_compression(codec, stream, rate_control):
  fun = libbwc.bwc_create_compression
  fun.restype  = ctypes.c_uchar
  fun.argtypes = [ctypes.c_void_p,
                  ctypes.c_void_p,
                  ctypes.c_char_p]
  return ctypes.c_uchar(fun(codec, stream, rate_control.encode('utf-8')))
#==================================================================================================#
def compress(codec, stream):
  fun = libbwc.bwc_compress
  fun.restype  = ctypes.c_size_t
  fun.argtypes = [ctypes.c_void_p,
                  ctypes.c_void_p]
  return ctypes.c_size_t(fun(codec, stream))
#==================================================================================================#
def create_decompression(codec, stream, layer):
  fun = libbwc.bwc_create_decompression
  fun.restype  = ctypes.c_uchar
  fun.argtypes = [ctypes.c_void_p,
                  ctypes.c_void_p,
                  ctypes.c_uint8]
  return ctypes.c_uchar(fun(codec, stream, layer))
#==================================================================================================#
def decompress(codec, stream):
  fun = libbwc.bwc_decompress
  fun.restype  = ctypes.c_uchar
  fun.argtypes = [ctypes.c_void_p,
                  ctypes.c_void_p]
  return ctypes.c_uchar(fun(codec, stream))