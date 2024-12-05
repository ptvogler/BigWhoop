/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||                                                                                                ||
||       /$$$$$$$  /$$                  /$$      /$$ /$$                                          ||
||      | $$__  $$|__/                 | $$  /$ | $$| $$                                          ||
||      | $$  \ $$ /$$  /$$$$$$        | $$ /$$$| $$| $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$       ||
||      | $$$$$$$ | $$ /$$__  $$       | $$/$$ $$ $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$      ||
||      | $$__  $$| $$| $$  \ $$       | $$$$_  $$$$| $$  \ $$| $$  \ $$| $$  \ $$| $$  \ $$      ||
||      | $$  \ $$| $$| $$  | $$       | $$$/ \  $$$| $$  | $$| $$  | $$| $$  | $$| $$  | $$      ||
||      | $$$$$$$/| $$|  $$$$$$$       | $$/   \  $$| $$  | $$|  $$$$$$/|  $$$$$$/| $$$$$$$/      ||
||      |_______/ |__/ \____  $$       |__/     \__/|__/  |__/ \______/  \______/ | $$____/       ||
||                     /$$  \ $$                                                  | $$            ||
||                    |  $$$$$$/                                                  | $$            ||
||                     \______/                                                   |__/            ||
||                                                                                                ||
\*  --------------------------------------------------------------------------------------------  */
/**                                                                                               
 *        @file   H5Zbwc.c
 *
 *       |                                                                                |
 *        DETAILED DESCRIPTION NEEDED
 *                                                                                                */
/*  --------------------------------------------------------------------------------------------  *\
||  Copyright (c) 2023, High Performance Computing Center - University of Stuttgart               ||
||                                                                                                ||
||  Redistribution and use in source and binary forms, with or without modification, are          ||
||  permitted provided that the following conditions are met:                                     ||
||                                                                                                ||
||     (1)   Redistributions of source code must retain the above copyright notice, this list of  ||
||           conditions and the following disclaimer.                                             ||
||                                                                                                ||
||     (2)   Redistributions in binary form must reproduce the above copyright notice, this list  ||
||           of conditions and the following disclaimer in the documentation and/or other         ||
||           materials provided with the distribution.                                            ||
||                                                                                                ||
||  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS   ||
||  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF               ||
||  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE    ||
||  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,     ||
||  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF            ||
||  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)        ||
||  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR      ||
||  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  ||
||  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                            ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||                                _ _  _ ____ _    _  _ ___  ____                                 ||
||                                | |\ | |    |    |  | |  \ |___                                 ||
||                                | | \| |___ |___ |__| |__/ |___                                 ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#include "H5private.h"                                  //!< Generic Functions
#include "H5Eprivate.h"                                 //!< Error handling
#include "H5Fprivate.h"                                 //!< File access
#include "H5Iprivate.h"                                 //!< IDs
#include "H5MMprivate.h"                                //!< Memory management
#include "H5Oprivate.h"                                 //!< Object headers
#include "H5Pprivate.h"                                 //!< Property lists
#include "H5Sprivate.h"                                 //!< Dataspaces
#include "H5Tprivate.h"                                 //!< Datatypes
#include "H5Zpkg.h"                                     //!< Data filters

#ifdef H5_HAVE_FILTER_SZIP
  
  #ifdef H5_HAVE_SZLIB_H
    #include "bwc.h"
  #endif

  /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
  ||                                _  _ ____ ____ ____ ____ ____                                 ||
  ||                                |\/| |__| |    |__/ |  | [__                                  ||
  ||                                |  | |  | |___ |  \ |__| ___]                                 ||
  ||                                                                                              ||
  \*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
  /*==============================================================================================*/
  /**
   * @details Simple Macros defining core attributes of the HDF5 filter.
   */
  /*===================================================|==========================================*/
  #define  H5Z_FILTER_BWC 33033                         //!< Filter id number


  /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
  ||    ____ _  _ _  _ ____ ___ _ ____ _  _   ___  ____ ____ ___ ____ ___ _   _ ___  ____ ____    ||
  ||    |___ |  | |\ | |     |  | |  | |\ |   |__] |__/ |  |  |  |  |  |   \_/  |__] |___ [__     ||
  ||    |    |__| | \| |___  |  | |__| | \|   |    |  \ |__|  |  |__|  |    |   |    |___ ___]    ||
  ||                                                                                              ||
  \*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
  //==========|==========================|======================|======|=======|====================
  static 
  htri_t       H5Z_can_apply_bwc          (hid_t                                dcpl_id,
                                           hid_t                                type_id,
                                           hid_t                                space_id);
  //==========|==========================|======================|======|=======|====================
  static 
  herr_t       H5Z_set_local_bwc          (hid_t                                dcpl_id,
                                           hid_t                                type_id,
                                           hid_t                                space_id);
  //==========|==========================|======================|======|=======|====================
  static 
  size_t       H5Z_filter_bwc             (unsigned int                         flags,
                                           size_t                               cd_nelmts,
                                           unsigned int                 const   cd_values[],
                                           size_t                               nbytes, 
                                           size_t                              *buf_size, 
                                           void                               **buf);

  /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
  ||     ____ _  _ ___ ____ ____ _  _ ____ _       ____ ____ _  _ ____ ___ ____ _  _ ___ ____     ||
  ||     |___  \/   |  |___ |__/ |\ | |__| |       |    |  | |\ | [__   |  |__| |\ |  |  [__      ||
  ||     |___ _/\_  |  |___ |  \ | \| |  | |___    |___ |__| | \| ___]  |  |  | | \|  |  ___]     ||
  ||                                                                                              ||
  \*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
  /*==============================================================================================*/
  /**
   * @details This message derives from H5Z.
   */
  /*===================================================|==========================================*/
  H5Z_class2_t H5Z_BWC[1] = 
  {{
    H5Z_CLASS_T_VERS,                                   //!< H5Z_class_t version
    H5Z_FILTER_BWC,                                     //!< Filter id number
    1,                                                  //!< Encoder present flag (set to true)
    1,                                                  //!< Decoder present flag (set to true)
    "bwc",                                              //!< Filter name for debugging
    H5Z_can_apply_bwc,                                  //!< The "can apply" callback
    H5Z_set_local_bwc,                                  //!< The "set local" callback
    H5Z_filter_bwc,                                     //!< The actual filter function
  }};


  /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
  ||            ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____            ||
  ||            |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__             ||
  ||            |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]            ||
  ||                                                                                              ||
  \*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
  /*==============================================================================================*/
  /**
   * @details This function probes the dataset defined by the supplied identifiers and
   *          evaluates weather the BigWhoop compressor can be applied.
   *
   * @param[in]   dcpl_id   Dataset creation property list identifier.
   * @param[in]   type_id   Type identifier.
   * @param[in]   space_id  Dataspace identifiers.
   *
   * @retval -1  Failure
   * @retval  0  False
   * @retval  1  True
   * 
   * @return  Three valued boolean type
   */
  /*==============================================================================================*/
  static htri_t
  H5Z_can_apply_bwc(hid_t H5_ATTR_UNUSED dcpl_id, 
                    hid_t                type_id, 
                    hid_t H5_ATTR_UNUSED space_id)
  {
    /*-----------------------*\
    ! DEFINE INT VARIABLES:   !
    \*-----------------------*/
    hid_t                   ntype_id;
    hsize_t                 dtype_dims[H5S_MAX_RANK];
    htri_t                  ret_value = TRUE;

    unsigned int            dtype_ndims;
    unsigned int            dtype_udims;
    unsigned int            dtype_size;

    unsigned int            i;

    /*-----------------------*\
    ! DEFINE STRUCTS:         !
    \*-----------------------*/
    H5T_class_t             dtype_class;
    H5T_order_t             dtype_order;
    H5T_order_t             ntype_order;

    FUNC_ENTER_NOAPI(FAIL)


    /* Determines class and size of the data type and checks  *
     * whether they are valid for the BigWhoop library.       */
    if ((dtype_class = H5Tget_class(type_id)) == H5T_NO_CLASS)
      HGOTO_ERROR(H5E_PLINE, H5E_BADTYPE, FAIL, 
        "bad datatype class")

    if (dtype_class != H5T_FLOAT)
      HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FALSE, 
        "only supports the H5T_FLOAT datatype class")

    if ((dtype_size = H5Tget_size(type_id)) == 0)
      HGOTO_ERROR(H5E_PLINE, H5E_BADTYPE, FAIL, 
        "bad datatype size")

    if (dtype_size != 4 && dtype_size != 8)
      HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FALSE, 
        "only supports 4- or 8-byte floating point values")


    /* Determin the number of non-unity dimensions and check  *
     * whether this is valid for the BigWhoop library.        */
    if ((dtype_ndims = H5Sget_simple_extent_dims(space_id, dtype_dims, NULL)) < 0)
      HGOTO_ERROR(H5E_PLINE, H5E_BADTYPE, FAIL, 
        "bad datatype dimension")

    for (i = 0, dtype_udims = 0; i < dtype_ndims; ++i)
      {
        if (dtype_dims[i] > 1)
          dtype_udims++;
      }

    if ((dtype_udims <= 1) || (dtype_udims > 4))
      HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FALSE, 
        "only supports 2 to 4 non-unity dimensions")


    /* Make sure that the endianess is consistent between the *
     * datatype and native type.                              */
    if((ntype_id = H5Tget_native_type(type_id, H5T_DIR_ASCEND)) == H5I_INVALID_HID)
      HGOTO_ERROR(H5E_PLINE, H5E_BADTYPE, FAIL, 
        "bad native datatype")
    
    if ((dtype_order = H5Tget_order(type_id)) == H5T_ORDER_ERROR)
      HGOTO_ERROR(H5E_PLINE, H5E_BADTYPE, FAIL, 
        "can't retrieve datatype endianness order")

    if ((ntype_order = H5Tget_order(ntype_id)) == H5T_ORDER_ERROR)
      HGOTO_ERROR(H5E_PLINE, H5E_BADTYPE, FAIL, 
        "can't retrieve native type endianness order")

    if (dtype_order != ntype_order)
      HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FALSE, 
        "BigWhoop does not allow for endian targetting")

  done:
      FUNC_LEAVE_NOAPI(ret_value)
  }

  /*==============================================================================================*/
  /**
   * @details This function sets the "local" parameters for the BigWhoop compression.
   *
   * @param[in]   dcpl_id   Dataset creation property list identifier.
   * @param[in]   type_id   Type identifier.
   * @param[in]   space_id  Dataspace identifiers.
   *
   * @retval -1  Failure
   * @retval  1  Success
   * 
   * @return  Error Flag
   */
  /*==============================================================================================*/
  static herr_t
  H5Z_set_local_bwc(hid_t dcpl_id,
                    hid_t type_id,
                    hid_t space_id)
  {
    /*-----------------------*\
    ! DEFINE INT VARIABLES:   !
    \*-----------------------*/
    herr_t                  ret_value = SUCCEED;

    /*-----------------------*\
    ! DEFINE STRUCTS:         !
    \*-----------------------*/

  if (0 > (dclass = H5Tget_class(type_id)))
        H5Z_ZFP_PUSH_AND_GOTO(H5E_ARGS, H5E_BADTYPE, -1, "not a datatype");

    if (0 == (dsize = H5Tget_size(type_id)))
        H5Z_ZFP_PUSH_AND_GOTO(H5E_ARGS, H5E_BADTYPE, -1, "not a datatype");

    if (0 > (ndims = H5Sget_simple_extent_dims(chunk_space_id, dims, 0)))
        H5Z_ZFP_PUSH_AND_GOTO(H5E_ARGS, H5E_BADTYPE, -1, "not a data space");

    /* setup zfp data type for meta header */
    if (dclass == H5T_FLOAT)
    {
        if (dsize == sizeof(float))
            zt = zfp_type_float;
        else if (dsize == sizeof(double))
            zt = zfp_type_double;
        else
            H5Z_ZFP_PUSH_AND_GOTO(H5E_ARGS, H5E_BADTYPE, -1, "invalid datatype size");
    }
    else
    {
        H5Z_ZFP_PUSH_AND_GOTO(H5E_PLINE, H5E_BADTYPE, 0,
            "datatype class must be H5T_FLOAT or H5T_INTEGER");
    }

    /* computed used (e.g. non-unity) dimensions in chunk */
    for (i = 0; i < ndims; i++)
    {
        if (dims[i] <= 1) continue;
        dims_used[ndims_used] = dims[i];
        ndims_used++;
    }

    /* set up dummy zfp field to compute meta header */
    switch (ndims_used)
    {
        case 1: dummy_field = Z zfp_field_1d(0, zt, dims_used[0]); break;
        case 2: dummy_field = Z zfp_field_2d(0, zt, dims_used[1], dims_used[0]); break;
        case 3: dummy_field = Z zfp_field_3d(0, zt, dims_used[2], dims_used[1], dims_used[0]); break;
#if ZFP_VERSION_NO >= 0x0540
        case 4: dummy_field = Z zfp_field_4d(0, zt, dims_used[3], dims_used[2], dims_used[1], dims_used[0]); break;
#endif
#if ZFP_VERSION_NO < 0x0530
        default: H5Z_ZFP_PUSH_AND_GOTO(H5E_PLINE, H5E_BADVALUE, 0,
                     "chunks may have only 1...3 non-unity dims");
#else
        default: H5Z_ZFP_PUSH_AND_GOTO(H5E_PLINE, H5E_BADVALUE, 0,
                     "chunks may have only 1...4 non-unity dims");
#endif
    }
    if (!dummy_field)
        H5Z_ZFP_PUSH_AND_GOTO(H5E_RESOURCE, H5E_NOSPACE, 0, "zfp_field_Xd() failed");

    /* get current cd_values and re-map to new cd_value set */
    if (0 > H5Pget_filter_by_id(dcpl_id, H5Z_FILTER_ZFP, &flags, &mem_cd_nelmts, mem_cd_values, 0, NULL, NULL))
        H5Z_ZFP_PUSH_AND_GOTO(H5E_PLINE, H5E_CANTGET, 0, "unable to get current ZFP cd_values");

    /* Handle default case when no cd_values are passed by using ZFP library defaults. */
    if (mem_cd_nelmts == 0)
    {
        /* check for filter controls in the properites */
        if (0 < H5Pexist(dcpl_id, "zfp_controls"))
        {
            if (0 > H5Pget(dcpl_id, "zfp_controls", &ctrls))
                H5Z_ZFP_PUSH_AND_GOTO(H5E_PLINE, H5E_CANTGET, 0, "unable to get ZFP controls");
            have_zfp_controls = 1;
        }
        else /* just use ZFP library defaults */
        {
            mem_cd_nelmts = H5Z_ZFP_CD_NELMTS_MEM;
            H5Pset_zfp_expert_cdata(ZFP_MIN_BITS, ZFP_MAX_BITS, ZFP_MAX_PREC, ZFP_MIN_EXP, mem_cd_nelmts, mem_cd_values);
        }
    }
        
    /* Into hdr_cd_values, we encode ZFP library and H5Z-ZFP plugin version info at
       entry 0 and use remaining entries as a tiny buffer to write ZFP native header. */
    hdr_cd_values[0] = (unsigned int) ((ZFP_VERSION_NO<<16) | (ZFP_CODEC<<12) | H5Z_FILTER_ZFP_VERSION_NO);
    if (0 == (dummy_bstr = B stream_open(&hdr_cd_values[1], sizeof(hdr_cd_values))))
        H5Z_ZFP_PUSH_AND_GOTO(H5E_RESOURCE, H5E_NOSPACE, 0, "stream_open() failed");

    if (0 == (dummy_zstr = Z zfp_stream_open(dummy_bstr)))
        H5Z_ZFP_PUSH_AND_GOTO(H5E_RESOURCE, H5E_NOSPACE, 0, "zfp_stream_open() failed");

    /* Set the ZFP stream mode from zfp_control properties or mem_cd_values[0] */
    if (have_zfp_controls)
    {
        switch (ctrls.mode)
        {
            case H5Z_ZFP_MODE_RATE:
                Z zfp_stream_set_rate(dummy_zstr, ctrls.details.rate, zt, ndims_used, 0);
                break;
            case H5Z_ZFP_MODE_PRECISION:
#if ZFP_VERSION_NO < 0x0510
                Z zfp_stream_set_precision(dummy_zstr, ctrls.details.prec, zt);
#else
                Z zfp_stream_set_precision(dummy_zstr, ctrls.details.prec);
#endif
                break;
            case H5Z_ZFP_MODE_ACCURACY:
#if ZFP_VERSION_NO < 0x0510
                Z zfp_stream_set_accuracy(dummy_zstr, ctrls.details.acc, zt);
#else
                Z zfp_stream_set_accuracy(dummy_zstr, ctrls.details.acc);
#endif
                break;
            case H5Z_ZFP_MODE_EXPERT:
                Z zfp_stream_set_params(dummy_zstr, ctrls.details.expert.minbits,
                    ctrls.details.expert.maxbits, ctrls.details.expert.maxprec,
                    ctrls.details.expert.minexp);
                break;
#if ZFP_VERSION_NO >= 0x0550
            case H5Z_ZFP_MODE_REVERSIBLE:
                Z zfp_stream_set_reversible(dummy_zstr);
                break;
#endif
            default:
                H5Z_ZFP_PUSH_AND_GOTO(H5E_PLINE, H5E_BADVALUE, 0, "invalid ZFP mode");
        }
    }
    else
    {
        switch (mem_cd_values[0])
        {
            case H5Z_ZFP_MODE_RATE:
                Z zfp_stream_set_rate(dummy_zstr, *((double*) &mem_cd_values[2]), zt, ndims_used, 0);
                break;
            case H5Z_ZFP_MODE_PRECISION:
#if ZFP_VERSION_NO < 0x0510
                Z zfp_stream_set_precision(dummy_zstr, mem_cd_values[2], zt);
#else
                Z zfp_stream_set_precision(dummy_zstr, mem_cd_values[2]);
#endif
                break;
            case H5Z_ZFP_MODE_ACCURACY:
#if ZFP_VERSION_NO < 0x0510
                Z zfp_stream_set_accuracy(dummy_zstr, *((double*) &mem_cd_values[2]), zt);
#else
                Z zfp_stream_set_accuracy(dummy_zstr, *((double*) &mem_cd_values[2]));
#endif
                break;
            case H5Z_ZFP_MODE_EXPERT:
                Z zfp_stream_set_params(dummy_zstr, mem_cd_values[2], mem_cd_values[3],
                    mem_cd_values[4], (int) mem_cd_values[5]);
                break;
#if ZFP_VERSION_NO >= 0x0550
            case H5Z_ZFP_MODE_REVERSIBLE:
                Z zfp_stream_set_reversible(dummy_zstr);
                break;
#endif
            default:
                H5Z_ZFP_PUSH_AND_GOTO(H5E_PLINE, H5E_BADVALUE, 0, "invalid ZFP mode");
        }
    }

    /* Use ZFP's write_header method to write the ZFP header into hdr_cd_values array */
    if (0 == (hdr_bits = Z zfp_write_header(dummy_zstr, dummy_field, ZFP_HEADER_FULL)))
        H5Z_ZFP_PUSH_AND_GOTO(H5E_PLINE, H5E_CANTINIT, 0, "unable to write header");

    /* Flush the ZFP stream */
    Z zfp_stream_flush(dummy_zstr);

    /* compute necessary hdr_cd_values size */
    hdr_bytes     = 1 + ((hdr_bits  - 1) / 8);
    hdr_cd_nelmts = 1 + ((hdr_bytes - 1) / sizeof(hdr_cd_values[0]));
    hdr_cd_nelmts++; /* for slot 0 holding version info */

    if (hdr_cd_nelmts > H5Z_ZFP_CD_NELMTS_MAX)
        H5Z_ZFP_PUSH_AND_GOTO(H5E_PLINE, H5E_BADVALUE, -1, "buffer overrun in hdr_cd_values");

    /* Now, update cd_values for the filter */
    if (0 > H5Pmodify_filter(dcpl_id, H5Z_FILTER_ZFP, flags, hdr_cd_nelmts, hdr_cd_values))
        H5Z_ZFP_PUSH_AND_GOTO(H5E_PLINE, H5E_BADVALUE, 0,
            "failed to modify cd_values");

    /* cleanup the dummy ZFP stuff we used to generate the header */
    Z zfp_field_free(dummy_field); dummy_field = 0;
    Z zfp_stream_close(dummy_zstr); dummy_zstr = 0;
    B stream_close(dummy_bstr); dummy_bstr = 0;

    retval = 1;

done:

    if (dummy_field) Z zfp_field_free(dummy_field);
    if (dummy_zstr) Z zfp_stream_close(dummy_zstr);
    if (dummy_bstr) B stream_close(dummy_bstr);
    return retval;
  done:
      FUNC_LEAVE_NOAPI(ret_value)
  }

  /*==============================================================================================*/
  /**
   * @details DESCRIPTION NEEDED
   *
   * @param[in]     name  DESCRIPTION NEEDED.
   * @param[out]    name  DESCRIPTION NEEDED.
   * @param[in,out] name  DESCRIPTION NEEDED.
   *
   * @retval -1  Error
   * @retval  0  OK
   * @return  DESCRIPTION NEEDED
   */
  /*==============================================================================================*/
  static size_t
  H5Z_filter_szip(unsigned int          flags, 
                  size_t                cd_nelmts,
                  unsigned int  const   cd_values[], 
                  size_t                nbytes,
                  size_t               *buf_size,
                  void                **buf)
  {


  done:
      if (outbuf)
          H5MM_xfree(outbuf);
      FUNC_LEAVE_NOAPI(ret_value)
  }
#endif
