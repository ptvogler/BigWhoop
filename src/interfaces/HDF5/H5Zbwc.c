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
#include <hdf5.h>                                       //!< HDF5 public header
#include <stdlib.h>                                     //!< Standard C library

#include "bwc.h"                                        //!< BigWhoop public header


/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||                                 _  _ ____ ____ ____ ____ ____                                  ||
||                                 |\/| |__| |    |__/ |  | [__                                   ||
||                                 |  | |  | |___ |  \ |__| ___]                                  ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*================================================================================================*/
/**
 * @details Simple Macros defining core attributes of the HDF5 filter.
 */
/*===================================================|============================================*/
#define  H5Z_FILTER_BWC         33000                 //!< Filter id number

#define  H5Z_BWC_VERSION_MAJOR  0                     //!< H5Z_BWC Major Version
#define  H5Z_BWC_VERSION_MINOR  1                     //!< H5Z_BWC Minor Version
#define  H5Z_BWC_VERSION_PATCH  0                     //!< H5Z_BWC Patch Version

#define  H5Z_BWC_USER_NPARMS    15                    //!< Number of user adjustable parames.
#define  H5Z_BWC_TOTAL_NPARMS   26                    //!< Total number of parameters.

#define  H5Z_BWC_FG             0                     //!< Pos. of control flag in cd_values

#define  H5Z_BWC_NX             1                     //!< Pos. of sizeX in cd_values
#define  H5Z_BWC_NY             3                     //!< Pos. of sizeY in cd_values
#define  H5Z_BWC_NZ             5                     //!< Pos. of sizeZ in cd_values
#define  H5Z_BWC_NT             7                     //!< Pos. of sizeTS in cd_values
#define  H5Z_BWC_NP             9                     //!< Pos. of n.o. params in cd_values
#define  H5Z_BWC_DT             10                    //!< Pos. of data type in cd_values

#define  H5Z_BWC_ER             11                    //!< Pos. of err_res. flag in cd_values
#define  H5Z_BWC_BR             12                    //!< Pos. of bitrate in cd_values

#define  H5Z_BWC_CB             13                    //!< Pos. of codeblock size in cd_values
#define  H5Z_BWC_PC             14                    //!< Pos. of precinct in cd_values

#define  H5Z_BWC_TX             15                    //!< Pos. of tileSizeX in cd_values
#define  H5Z_BWC_TY             17                    //!< Pos. of tileSizeY in cd_values
#define  H5Z_BWC_TZ             19                    //!< Pos. of tileSizeZ in cd_values
#define  H5Z_BWC_TT             21                    //!< Pos. of tileSizeTS in cd_values

#define  H5Z_BWC_DL             23                    //!< Pos. of n.o. decomp. lvl in cd_values
#define  H5Z_BWC_QM             24                    //!< Pos. of QM value in cd_values

#define  H5Z_BWC_UL             25                    //!< Pos. of use layer in cd_values


/*================================================================================================*/
/**
 * @details Simple wrapper for the H5Epush_goto function used to push the appropriate ret_val.
 */
/*===================================================|============================================*/
#define BWC_GOTO_ERROR(MJ, MN, RV, MG)                \
do {                                                  \
      ret_value = RV;                                 \
      H5Epush_goto(_funcname_, H5E_ERR_CLS, MJ,       \
                                            MN,       \
                                            MG, done);\
} while(0)


/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||     ____ _  _ _  _ ____ ___ _ ____ _  _   ___  ____ ____ ___ ____ ___ _   _ ___  ____ ____     ||
||     |___ |  | |\ | |     |  | |  | |\ |   |__] |__/ |  |  |  |  |  |   \_/  |__] |___ [__      ||
||     |    |__| | \| |___  |  | |__| | \|   |    |  \ |__|  |  |__|  |    |   |    |___ ___]     ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
//==========|==========================|======================|======|=======|======================
static 
htri_t       H5Z__can_apply_bwc         (hid_t                                dcpl_id,
                                         hid_t                                type_id,
                                         hid_t                                space_id);
//==========|==========================|======================|======|=======|======================
static 
herr_t       H5Z__set_local_bwc         (hid_t                                dcpl_id,
                                         hid_t                                type_id,
                                         hid_t                                space_id);
//==========|==========================|======================|======|=======|======================
static 
size_t       H5Z__filter_bwc            (unsigned int                         flags,
                                         size_t                               cd_nelmts,
                                         unsigned int                 const   cd_values[],
                                         size_t                               nbytes, 
                                         size_t                              *buf_size, 
                                         void                               **buf);

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||      ____ _  _ ___ ____ ____ _  _ ____ _       ____ ____ _  _ ____ ___ ____ _  _ ___ ____      ||
||      |___  \/   |  |___ |__/ |\ | |__| |       |    |  | |\ | [__   |  |__| |\ |  |  [__       ||
||      |___ _/\_  |  |___ |  \ | \| |  | |___    |___ |__| | \| ___]  |  |  | | \|  |  ___]      ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*================================================================================================*/
/**
 * @details This message derives from H5Z.
 */
/*===================================================|============================================*/
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


/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||             ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____             ||
||             |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__              ||
||             |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]             ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*================================================================================================*/
/**
 * @details This function returns an HDF5 type identifying the implementation as a filter.
 * 
 * @return  HDF5 Type
 */
/*================================================================================================*/
H5PL_type_t
H5PLget_plugin_type(void)
{
  return H5PL_TYPE_FILTER;
}

/*================================================================================================*/
/**
 * @details This function returns the H5Z structure defining the BigWhoop HDF5 filter.
 * 
 * @return  H5Z struct
 */
/*================================================================================================*/
const void*
H5PLget_plugin_info(void)
{
  return &H5Z_BWC;
}

/*================================================================================================*/
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
/*================================================================================================*/
static htri_t
H5Z__can_apply_bwc(hid_t  __attribute__((unused)) dcpl_id, 
                   hid_t                          type_id, 
                   hid_t  __attribute__((unused)) space_id)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  htri_t                  ret_value = 1;

  hid_t                   nTypeID;
  hsize_t                 dims[H5S_MAX_RANK] = {0};

  size_t                  dtSize = {0};

  int                     nDims     = 0;
  unsigned int            valNoDims = 0;

  unsigned int            i;

  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  static char const *_funcname_ = "H5Z__can_apply_bwc";

  /*-----------------------*\
  ! DEFINE STRUCTS:         !
  \*-----------------------*/
  H5T_class_t             dType;
  H5T_order_t             dtOrder;
  H5T_order_t             ntOrder;

  /* Determines class and size of the data type and checks  *
   * whether they are valid for the BigWhoop library.       */
  if ((dType = H5Tget_class(type_id)) == H5T_NO_CLASS)
    BWC_GOTO_ERROR(H5E_PLINE, H5E_BADTYPE, -1,
      "bad datatype class");

  if (dType != H5T_FLOAT)
    BWC_GOTO_ERROR(H5E_ARGS, H5E_BADVALUE, 0, 
      "BigWhoop only supports the H5T_FLOAT datatype class");

  if ((dtSize = H5Tget_size(type_id)) == 0)
    BWC_GOTO_ERROR(H5E_PLINE, H5E_BADTYPE, -1, 
      "bad datatype size");

  if (dtSize != 4 && dtSize != 8)
    BWC_GOTO_ERROR(H5E_ARGS, H5E_BADVALUE, 0, 
      "BigWhoop only supports 4- or 8-byte floating point values");


  /* Determin the number of non-unity dimensions and check  *
   * whether this is valid for the BigWhoop library.        */
  if ((nDims = H5Sget_simple_extent_dims(space_id, dims, NULL)) < 0)
    BWC_GOTO_ERROR(H5E_PLINE, H5E_BADTYPE, -1, 
      "bad datatype dimension");

  for (i = 0, valNoDims = 0; i < (unsigned int) nDims; ++i)
    {
      if (dims[i] > 1)
        valNoDims++;
    }

  if ((valNoDims <= 1) || (valNoDims > 4))
    BWC_GOTO_ERROR(H5E_ARGS, H5E_BADVALUE, 0, 
      "BigWhoop only supports 2 to 4 non-unity dimensions");


  /* Make sure that the endianess is consistent between the *
   * datatype and native type.                              */
  if((nTypeID = H5Tget_native_type(type_id, H5T_DIR_ASCEND)) == H5I_INVALID_HID)
    BWC_GOTO_ERROR(H5E_PLINE, H5E_BADTYPE, -1, 
      "bad native datatype");

  if ((dtOrder = H5Tget_order(type_id)) == H5T_ORDER_ERROR)
    BWC_GOTO_ERROR(H5E_PLINE, H5E_BADTYPE, -1, 
      "can't retrieve datatype endianness order");

  if ((ntOrder = H5Tget_order(nTypeID)) == H5T_ORDER_ERROR)
    BWC_GOTO_ERROR(H5E_PLINE, H5E_BADTYPE, -1, 
      "can't retrieve native type endianness order");

  if (dtOrder != ntOrder)
    BWC_GOTO_ERROR(H5E_ARGS, H5E_BADVALUE, 0, 
      "BigWhoop does not allow for endian targetting");

done:
    return ret_value;
}

/*================================================================================================*/
/**
 * @details This function sets the parameters needed for the BigWhoop compression.
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
/*================================================================================================*/
static herr_t
H5Z__set_local_bwc(hid_t dcpl_id,
                   hid_t type_id,
                   hid_t space_id)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  herr_t                  ret_value = 1;

  hsize_t                 dims[H5S_MAX_RANK]   = {0};
  hsize_t                 vdSize[H5S_MAX_RANK] = {0};

  size_t                  cd_nelmts = 26;

  size_t                  dtSize = 0;

  unsigned int            cd_values[H5Z_BWC_TOTAL_NPARMS] = {0};

  int                     nDims     = 0;
  unsigned int            valNoDims = 0;

  unsigned                flags;

  unsigned int            i,j;

  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  static char const *_funcname_ = "H5Z__set_local_bwc";

  /* Fetch the filter-specific client data and check that   *
   * the cd_values array has the required number of values. */
  if (H5Pget_filter_by_id(dcpl_id, H5Z_FILTER_BWC, &flags, &cd_nelmts, cd_values, 0, NULL, NULL) < 0)
    BWC_GOTO_ERROR(H5E_PLINE, H5E_CANTGET, -1, 
      "can't get bwc parameters");

  if (cd_nelmts < 26)
    BWC_GOTO_ERROR(H5E_PLINE, H5E_BADVALUE, -1, 
      "invalid value for cd_nelmts");

  /* Determines size of the data type, checks validity and  *
   * stores it in the cd_values array       .               */
  if ((dtSize = H5Tget_size(type_id)) == 0)
    BWC_GOTO_ERROR(H5E_PLINE, H5E_BADTYPE, -1, 
      "bad datatype size");

  if (dtSize != 4 && dtSize != 8)
    BWC_GOTO_ERROR(H5E_ARGS, H5E_BADVALUE, 0, 
      "BigWhoop only supports 4- or 8-byte floating point values");

  cd_values[H5Z_BWC_DT] = (unsigned int) dtSize;

  /* Determin the number of non-unity dimensions, check     *
   * validity and store the information in cd_values array. */
  if ((nDims = H5Sget_simple_extent_dims(space_id, dims, NULL)) < 0)
    BWC_GOTO_ERROR(H5E_PLINE, H5E_BADTYPE, -1, 
      "bad datatype dimension");

  for (i = 0; i < H5S_MAX_RANK; ++i)
    vdSize[i] = 1;

  for (i = 0, valNoDims = 0; i < (unsigned int) nDims; ++i)
    {
      if (dims[i] > 1)
        vdSize[valNoDims++] = dims[i];
    }

  if ((valNoDims <= 1) || (valNoDims > 4))
    BWC_GOTO_ERROR(H5E_ARGS, H5E_BADVALUE, 0, 
      "BigWhoop only supports 2 to 4 non-unity dimensions");

  cd_values[H5Z_BWC_NX + 0] = (unsigned int)(vdSize[0] & 0xFFFFFFFF);
  cd_values[H5Z_BWC_NX + 1] = (unsigned int)((vdSize[0] >> 32) & 0xFFFFFFFF);

  cd_values[H5Z_BWC_NY + 0] = (unsigned int)(vdSize[1] & 0xFFFFFFFF);
  cd_values[H5Z_BWC_NY + 1] = (unsigned int)((vdSize[1] >> 32) & 0xFFFFFFFF);

  cd_values[H5Z_BWC_NZ + 0] = (unsigned int)(vdSize[2] & 0xFFFFFFFF);
  cd_values[H5Z_BWC_NZ + 1] = (unsigned int)((vdSize[2] >> 32) & 0xFFFFFFFF);

  cd_values[H5Z_BWC_NT + 0] = (unsigned int)(vdSize[3] & 0xFFFFFFFF);
  cd_values[H5Z_BWC_NT + 1] = (unsigned int)((vdSize[3] >> 32) & 0xFFFFFFFF);

  cd_values[H5Z_BWC_NP]     = 1;

  /* Modify the filter's parameters for this dataset */
  if (H5Pmodify_filter(dcpl_id, H5Z_FILTER_BWC, flags, H5Z_BWC_TOTAL_NPARMS, cd_values) < 0)
    BWC_GOTO_ERROR(H5E_PLINE, H5E_CANTSET, -1, 
      "can't set local bwc parameters");

done:
    return ret_value;
}

/*================================================================================================*/
/**
 * @details This function applies the BigWhoop compression to the input buffer supplied
 *          by HDF5.
 *
 * @param[in]     flags     Flag defining a (de)compression run.
 * @param[in]     cd_nelmts Number of filter parameters.
 * @param[in]     cd_values Filter-specific client data.
 * @param[in]     nbytes    Number of bytes in the input buffer.
 * @param[in,out] buf_size  Pointer to I/O buffer size.
 * @param[in,out] buf       Pointer to I/O buffer.
 *
 * @retval  0  Error
 * @retval  >0 OK
 * @return  Returns the size of the filtered data chunk
 */
/*================================================================================================*/
static size_t
H5Z__filter_bwc(unsigned int          flags, 
                size_t                cd_nelmts,
                unsigned int  const   cd_values[], 
                size_t                nbytes,
                size_t               *buf_size,
                void                **buf)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  hsize_t                 nX, nY, nZ;
  hsize_t                 nTS;

  hsize_t                 dSize;

  size_t                  dtSize;
  size_t                  ret_value = 1;

  hsize_t                 tmpX, tmpY, tmpZ;
  hsize_t                 tmpTS;

  unsigned int            nPar;
  unsigned int            uLayer = 0;
  unsigned int            optFlags;

  /*-----------------------*\
  ! DEFINE CHAR VARIABLES:  !
  \*-----------------------*/
  char                    rate[20] = {0};

  static char const      *_funcname_ = "H5Z__filter_bwc";

  unsigned char          *input  = (unsigned char *)(*buf);
  unsigned char          *output = NULL;
  unsigned char          *tmpbuf = NULL;


  /*-----------------------*\
  ! DEFINE DER. VARIABLES:  !
  \*-----------------------*/
  bwc_header             *header;
  bwc_stream             *stream = NULL;
  bwc_codec              *coder  = NULL;

  /* Check the number of filter arguments.                  */
  if (cd_nelmts != 26)
    BWC_GOTO_ERROR(H5E_ARGS, H5E_BADVALUE, 0, 
      "invalid number of filter parameters");

  /* Filter invoked to decompress codestream.               */
  if (flags & H5Z_FLAG_REVERSE) 
    {
      /* Retrieve header information and allocate output buffer. */
      header = bwc_open_header(input);
      if (header == NULL)
        BWC_GOTO_ERROR(H5E_PLINE, H5E_CANTFILTER, 0, 
          "failed to parse main header");

      dSize = header->info.nX * header->info.nY * header->info.nZ *
              header->info.nTS * header->info.nPar;
      if(header->info.data_prec == bwc_precision_double)
        dSize *= sizeof(double);
      else if(header->info.data_prec == bwc_precision_single)
        dSize *= sizeof(float);

      bwc_close_header(header);

      output = calloc(dSize, sizeof(char));
      if(output == NULL)
        BWC_GOTO_ERROR(H5E_RESOURCE, H5E_NOSPACE, 0, 
          "memory allocation failed for bwc filter");

      /* If set, retreive the use layer option from cd_values.   */
      if ((cd_values[H5Z_BWC_FG] & (0x01 << 7)) != 0)
        uLayer = (unsigned int)cd_values[H5Z_BWC_UL];

      /* Initialize and run the decompression.                   */
      coder  = bwc_alloc_decoder();
      if(coder == NULL)
        BWC_GOTO_ERROR(H5E_RESOURCE, H5E_NOSPACE, 0, 
          "memory allocation failed for bwc filter");

      stream = bwc_init_stream(input, output, decomp);
      if(stream == NULL)
        BWC_GOTO_ERROR(H5E_RESOURCE, H5E_NOSPACE, 0, 
          "memory allocation failed for bwc filter");

      if (bwc_create_decompression(coder, stream, uLayer) == EXIT_FAILURE)
        BWC_GOTO_ERROR(H5E_PLINE, H5E_CANTFILTER, 0, 
          "failed to initialize the decompression");
      if (bwc_decompress(coder, stream) == EXIT_FAILURE)
        BWC_GOTO_ERROR(H5E_PLINE, H5E_CANTFILTER, 0, 
          "failed to decompress");

      /* Free the input buffer and set the return values.        */
      free(*buf);

      *buf      = output;
      output    = NULL;
      *buf_size = (size_t) dSize;
      ret_value = (size_t) dSize;
    }
  /* Filter invoked to compress data set.                   */  
  else 
    {
      /* Evaluate the input and output buffer size and initial- *
       * ize the BigWhoop data and coder structs.               */
      nX  = ((hsize_t)cd_values[H5Z_BWC_NX + 1] << 32) | cd_values[H5Z_BWC_NX];
      nY  = ((hsize_t)cd_values[H5Z_BWC_NY + 1] << 32) | cd_values[H5Z_BWC_NY];
      nZ  = ((hsize_t)cd_values[H5Z_BWC_NZ + 1] << 32) | cd_values[H5Z_BWC_NZ];
      nTS = ((hsize_t)cd_values[H5Z_BWC_NT + 1] << 32) | cd_values[H5Z_BWC_NT];
      
      nPar   = (unsigned int)cd_values[H5Z_BWC_NP];
      dtSize = (size_t)cd_values[H5Z_BWC_DT];

      dSize = nX * nY * nZ * nTS * nPar * dtSize;

      if (nbytes != dSize)
        BWC_GOTO_ERROR(H5E_PLINE, H5E_BADVALUE, -1, 
          "size of inbut buffer differs from specified domain size");

      output = calloc(dSize, sizeof(char));
      if(output == NULL)
        BWC_GOTO_ERROR(H5E_RESOURCE, H5E_NOSPACE, 0, 
          "memory allocation failed for bwc filter");

      stream = bwc_init_stream(input, output, comp);
      if(stream == NULL)
        BWC_GOTO_ERROR(H5E_RESOURCE, H5E_NOSPACE, 0, 
          "memory allocation failed for bwc filter");

      coder  = bwc_alloc_coder(nX, nY, nZ, nTS, nPar, dtSize);
      if(coder == NULL)
        BWC_GOTO_ERROR(H5E_RESOURCE, H5E_NOSPACE, 0, 
          "memory allocation failed for bwc filter");

      /* Get the option flags from the cd_values array and set  *
       * all user defined options in the coder structure.       */
      optFlags = (unsigned int)cd_values[H5Z_BWC_FG];

      if ((optFlags & (0x01 << 0)) != 0)
        bwc_set_error_resilience(coder);

      if ((optFlags & (0x01 << 1)) != 0)
        sprintf(rate, "%05.3f", *(float *)&cd_values[H5Z_BWC_BR]);
      else
        sprintf(rate, "%05.3f", (float)(8.0f * dtSize));

      if ((optFlags & (0x01 << 2)) != 0)
        {
          tmpX  = (hsize_t)((cd_values[H5Z_BWC_CB] >> 24) & 0xFF);
          tmpY  = (hsize_t)((cd_values[H5Z_BWC_CB] >> 16) & 0xFF);
          tmpZ  = (hsize_t)((cd_values[H5Z_BWC_CB] >>  8) & 0xFF);
          tmpTS = (hsize_t)(cd_values[H5Z_BWC_CB] & 0xFF);

          bwc_set_codeblocks(coder, tmpX, tmpY, tmpZ, tmpTS);
          tmpX = tmpY = tmpZ = tmpTS = 0;
        }

      if ((optFlags & (0x01 << 3)) != 0)
        {
          tmpX  = (hsize_t)((cd_values[H5Z_BWC_PC] >> 24) & 0xFF);
          tmpY  = (hsize_t)((cd_values[H5Z_BWC_PC] >> 16) & 0xFF);
          tmpZ  = (hsize_t)((cd_values[H5Z_BWC_PC] >>  8) & 0xFF);
          tmpTS = (hsize_t)(cd_values[H5Z_BWC_PC] & 0xFF);

          bwc_set_precincts(coder, tmpX, tmpY, tmpZ, tmpTS);
          tmpX = tmpY = tmpZ = tmpTS = 0;
        }

      if ((optFlags & (0x01 << 4)) != 0)
        {
          tmpX  = ((hsize_t)cd_values[H5Z_BWC_TX + 1] << 32) | cd_values[H5Z_BWC_TX];
          tmpY  = ((hsize_t)cd_values[H5Z_BWC_TY + 1] << 32) | cd_values[H5Z_BWC_TY];
          tmpZ  = ((hsize_t)cd_values[H5Z_BWC_TZ + 1] << 32) | cd_values[H5Z_BWC_TZ];
          tmpTS = ((hsize_t)cd_values[H5Z_BWC_TT + 1] << 32) | cd_values[H5Z_BWC_TT];

          bwc_set_tiles(coder, tmpX, tmpY, tmpZ, tmpTS, bwc_tile_sizeof);
          tmpX = tmpY = tmpZ = tmpTS = 0;
        }

      if ((optFlags & (0x01 << 5)) != 0)
        {
          tmpX  = (hsize_t)((cd_values[H5Z_BWC_DL] >> 24) & 0xFF);
          tmpY  = (hsize_t)((cd_values[H5Z_BWC_DL] >> 16) & 0xFF);
          tmpZ  = (hsize_t)((cd_values[H5Z_BWC_DL] >>  8) & 0xFF);
          tmpTS = (hsize_t)(cd_values[H5Z_BWC_DL] & 0xFF);

          bwc_set_decomp(coder, tmpX, tmpY, tmpZ, tmpTS);
          tmpX = tmpY = tmpZ = tmpTS = 0;
        }

      if ((optFlags & (0x01 << 6)) != 0)
        bwc_set_qm(coder, (unsigned int)cd_values[H5Z_BWC_QM]);

      /* Initialize and run the compression.                     */
      if (bwc_create_compression(coder, stream, rate) == EXIT_FAILURE)
        BWC_GOTO_ERROR(H5E_RESOURCE, H5E_NOSPACE, 0, 
            "memory allocation failed for bwc filter");

      dSize = bwc_compress(coder, stream);
      if (dSize == 0)
        BWC_GOTO_ERROR(H5E_PLINE, H5E_CANTFILTER, 0, 
          "failed to compress");

      if (dSize > nbytes)
        BWC_GOTO_ERROR(H5E_RESOURCE, H5E_OVERFLOW, 0,
          "buffer overrun");

      if (dSize < nbytes)
        {
          tmpbuf = realloc(output, dSize);
          if (tmpbuf == NULL)
            BWC_GOTO_ERROR(H5E_RESOURCE, H5E_NOSPACE, 0, 
              "failed to realloc output buffer ");
          else
            output = tmpbuf;
        }

      /* Free the input buffer and set the return values.        */
      free(*buf);

      *buf      = output;
      output    = NULL;
      tmpbuf    = NULL;
      *buf_size = (size_t) dSize;
      ret_value = (size_t) dSize;
    }

done:

  if (coder)
    bwc_free_codec(coder);

  if (stream)
    free(stream);

  if (output)
      free(output);
  return ret_value;
}
