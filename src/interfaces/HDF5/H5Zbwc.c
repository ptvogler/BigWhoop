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
 *        This code aims to expose the HDF5 library to the compression functionality of
 *        BigWhoop. The work is based on the internal filter options of HDF5 and is not
 *        intended to plagiarize, but to respect its coding paradigm.
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
#include "H5Zmodule.h"                                  //!< H5Z module
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

#include "H5Zbwc_sup.h"

#ifdef H5_HAVE_FILTER_BWC
  
  #ifdef H5_HAVE_BWC_H
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
  htri_t       H5Z__can_apply_bwc         (hid_t                                dcpl_id,
                                           hid_t                                type_id,
                                           hid_t                                space_id);
  //==========|==========================|======================|======|=======|====================
  static 
  herr_t       H5Z__set_local_bwc         (hid_t                                dcpl_id,
                                           hid_t                                type_id,
                                           hid_t                                space_id);
  //==========|==========================|======================|======|=======|====================
  static 
  size_t       H5Z__filter_bwc            (unsigned int                         flags,
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
    H5Z__can_apply_bwc,                                 //!< The "can apply" callback
    H5Z__set_local_bwc,                                 //!< The "set local" callback
    H5Z__filter_bwc,                                    //!< The actual filter function
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
  H5Z__can_apply_bwc(hid_t H5_ATTR_UNUSED dcpl_id, 
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
    size_t                  dtype_size;

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
        "BigWhoop only supports the H5T_FLOAT datatype class")

    if ((dtype_size = H5Tget_size(type_id)) == 0)
      HGOTO_ERROR(H5E_PLINE, H5E_BADTYPE, FAIL, 
        "bad datatype size")

    if (dtype_size != 4 && dtype_size != 8)
      HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FALSE, 
        "BigWhoop only supports 4- or 8-byte floating point values")


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
        "BigWhoop only supports 2 to 4 non-unity dimensions")


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
  H5Z__set_local_bwc(hid_t dcpl_id,
                     hid_t type_id,
                     hid_t space_id)
  {
    /*-----------------------*\
    ! DEFINE INT VARIABLES:   !
    \*-----------------------*/
    herr_t                  ret_value = SUCCEED;

    size_t                  cd_nelmts = H5Z_SZIP_USER_NPARMS;
    unsigned int            cd_values[H5Z_SZIP_TOTAL_NPARMS];

    hid_t                   ntype_id;
    hsize_t                 dtype_dims[H5S_MAX_RANK];

    unsigned int            dtype_ndims;
    unsigned int            dtype_udims;
    unsigned int            dtype_size;

    unsigned int            i;

    /*-----------------------*\
    ! DEFINE STRUCTS:         !
    \*-----------------------*/

    const H5T_t    *type;                             /* Datatype */
    const H5S_t    *ds;                               /* Dataspace */
    unsigned        flags;                            /* Filter flags */
    hsize_t         dims[H5O_LAYOUT_NDIMS];           /* Dataspace (i.e. chunk) dimensions */
    size_t          dtype_size;                       /* Datatype's size (in bits) */
    size_t          dtype_precision;                  /* Datatype's precision (in bits) */
    hsize_t         scanline;                         /* Size of dataspace's fastest changing dimension */

    FUNC_ENTER_PACKAGE


    /* Determines class and size of the data type and checks  *
     * whether they are valid for the BigWhoop library.       */
    if ((dtype_class = H5Tget_class(type_id)) == H5T_NO_CLASS)
      HGOTO_ERROR(H5E_PLINE, H5E_BADTYPE, FAIL,
        "bad datatype class")

    if (dtype_class != H5T_FLOAT)
      HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, 
        "BigWhoop only supports the H5T_FLOAT datatype class")

    if ((dtype_size = H5Tget_size(type_id)) == 0)
      HGOTO_ERROR(H5E_PLINE, H5E_BADTYPE, FAIL, 
        "bad datatype size")

    if (dtype_size != 4 && dtype_size != 8)
      HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, 
        "BigWhoop only supports 4- or 8-byte floating point values")


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
      HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, 
        "BigWhoop only supports 2 to 4 non-unity dimensions")


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
      HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, 
        "BigWhoop does not allow for endian targetting")


    /* Determines class and size of the data type and checks  *
     * whether they are valid for the BigWhoop library.       */
    if (H5Pget_filter_by_id(dcpl_id, H5Z_FILTER_BWC, &flags, &cd_nelmts, cd_values, 0, NULL, NULL) < 0)
      HGOTO_ERROR(H5E_PLINE, H5E_CANTGET, FAIL, 
        "can't get bwc parameters");

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
  H5Z__filter_bwc(unsigned int          flags, 
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
