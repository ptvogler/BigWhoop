# coding: utf-8
# /*##########################################################################
#
# Copyright (c) 2016-2022 European Synchrotron Radiation Facility
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# ###########################################################################*/
"""BWC filter ID"""
BWC_ID = 33000

class BWC(h5py.filters.FilterRefBase):
    """``h5py.Group.create_dataset``'s compression arguments for using BWC filter.

    .. code-block:: python

        f = h5py.File('test.h5', 'w')
        f.create_dataset(
            'bwc',
            data=numpy.random.random(100),
            compression=hdf5plugin.BWC())
        f.close()
    """

    filter_name = "bwc"
    filter_id = BWC_ID

    def __init__(self,
                 errorRes=False,
                 bitrate=None, 
                 codeblockSize=None,
                 precinctSize=None,
                 tileSize=None,
                 numberofLayers=None,
                 Qm=None,
                 useLayer=None):
        flag = 0

        if errorRes:
            flag = flag | (0x01 << 0)
            errorRes = struct.unpack('I', struct.pack('i', int(1)))[0]
            logger.info("BWC error resilience mode is set")
        else:
            errorRes = 0

        if bitrate is not None:
            flag = flag | (0x01 << 1)
            bitrate = struct.unpack('I', struct.pack('f', float(bitrate)))[0]
            logger.info("BWC bitrate is set")
        else:
            bitrate = 0

        if codeblockSize is not None and codeblockSize >= 0 and codeblockSize < 512:
            flag = flag | (0x01 << 2)
            codeblockSize = ((0xFF & codeblockSize) << 24)  |\
                            ((0xFF & codeblockSize) << 16)  |\
                            ((0xFF & codeblockSize) << 8)   |\
                            ((0xFF & codeblockSize))
            codeblockSize = struct.unpack('I', struct.pack('i', int(codeblockSize)))[0]
            logger.info("BWC codeblockSize is set")
        else:
            codeblockSize = 0

        if precinctSize is not None and precinctSize >= 0 and precinctSize < 512:
            flag = flag | (0x01 << 3)
            precinctSize = ((0xFF & precinctSize) << 24)  |\
                           ((0xFF & precinctSize) << 16)  |\
                           ((0xFF & precinctSize) << 8)   |\
                           ((0xFF & precinctSize))
            precinctSize = struct.unpack('I', struct.pack('i', int(precinctSize)))[0]
            logger.info("BWC precinctSize is set")
        else:
            precinctSize = 0

        if tileSize is not None:
            flag = flag | (0x01 << 4)
            tileXHigh,  tileXLow  = struct.unpack('II', struct.pack('Q', int(tileSize)))
            tileYHigh,  tileYLow  = struct.unpack('II', struct.pack('Q', int(tileSize)))
            tileZHigh,  tileZLow  = struct.unpack('II', struct.pack('Q', int(tileSize)))
            tileTSHigh, tileTSLow = struct.unpack('II', struct.pack('Q', int(tileSize)))
            logger.info("BWC tileSize is set")
        else:
            tileXHigh = tileXLow = tileYHigh = tileYLow = \
            tileZHigh = tileZLow = tileTSHigh = tileTSLow = 0

        if numberofLayers is not None and numberofLayers >= 0 and numberofLayers < 255:
            flag = flag | (0x01 << 5)
            numberofLayers = ((0xFF & numberofLayers) << 24)  |\
                             ((0xFF & numberofLayers) << 16)  |\
                             ((0xFF & numberofLayers) << 8)   |\
                             ((0xFF & numberofLayers))
            numberofLayers = struct.unpack('I', struct.pack('i', int(numberofLayers)))[0]
            logger.info("BWC precinctSize is set")
        else:
            numberofLayers = 0

        if Qm is not None:
            flag = flag | (0x01 << 6)
            Qm = struct.unpack('I', struct.pack('i', int(Qm)))[0]
            logger.info("BWC Qm is set")
        else:
            Qm = 0

        if useLayer is not None:
            flag = flag | (0x01 << 7)
            useLayer = struct.unpack('I', struct.pack('i', int(useLayer)))[0]
            logger.info("BWC useLayer is set")
        else:
            useLayer = 0


        self.filter_options = flag,                 \
                              0, 0,                 \
                              0, 0,                 \
                              0, 0,                 \
                              0, 0,                 \
                              0,                    \
                              0,                    \
                              errorRes,             \
                              bitrate,              \
                              codeblockSize,        \
                              precinctSize,         \
                              tileXHigh,  tileXLow, \
                              tileYHigh,  tileYLow, \
                              tileZHigh,  tileZLow, \
                              tileTSHigh, tileTSLow,\
                              numberofLayers,       \
                              Qm,                   \
                              useLayer

        logger.info(f"filter options = {self.filter_options}")

FILTER_CLASSES = Bitshuffle, Blosc, Blosc2, BWC, BZip2, FciDecomp, LZ4, Sperr, SZ, SZ3, Zfp, Zstd
