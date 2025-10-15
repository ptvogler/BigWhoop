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
 *        @file   hf5_test.c
 *
 *        Simple program used to test the BigWhoop's HDF5 plugin.
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
#include <hdf5.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "bwc.h"
#include "H5Zbwc.h"


/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||                                 _  _ ____ ____ ____ ____ ____                                  ||
||                                 |\/| |__| |    |__/ |  | [__                                   ||
||                                 |  | |  | |___ |  \ |__| ___]                                  ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*================================================================================================*/
/**
 * @details Macros to determine the minimum or maximum of two values.
 */
/*===========================================================================|====================*/
#define MAX(x, y)             (((x) < (y))?(y):(x))     // Returns maximum between two values
#define MIN(x, y)             (((x) > (y))?(y):(x))     // Returns minimum between two values

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||           ___  ____ _ _  _ ____ ___ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____           ||
||           |__] |__/ | |  | |__|  |  |___    |___ |  | |\ | |     |  | |  | |\ | [__            ||
||           |    |  \ |  \/  |  |  |  |___    |    |__| | \| |___  |  | |__| | \| ___]           ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*================================================================================================*/
/**
 * @details This function loads a dataset from a specified MPAS file.
 *
 * @param[out]  rho
 * @param[out]  rhou
 * @param[out]  rhov
 * @param[out]  rhow
 * @param[out]  rhoE
 *
 * @retval EXIT_FAILURE Failure
 * @retval EXIT_SUCCESS Success
 * 
 * @return  Error flag
 */
/*================================================================================================*/
static uint8_t
init_t2m_field(const char  *filename,
               const char  *dset_name,
               hsize_t     *nDims,
               hsize_t     *nPoints,
               hsize_t    **dims,
               float      **dataset)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  uint8_t                 ret_value = EXIT_FAILURE;

  hid_t                   file_id = -1;
  hid_t                   dataspace_id = -1;
  hid_t                   dataset_id = -1;
  hid_t                   dcpl_id = -1;

  /*-----------------------*\
  ! DEFINE FLOAT VARIABLES: !
  \*-----------------------*/


  if ((file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT)) < 0)
    goto done;

  if ((dataset_id = H5Dopen2(file_id, dset_name, H5P_DEFAULT)) < 0)
    goto done;

  if ((dataspace_id = H5Dget_space(dataset_id)) < 0)
    goto done;

  *nDims = H5Sget_simple_extent_ndims(dataspace_id);
  if ((*nDims < 2) || (*nDims > 3))
    goto done;

  if ((*nPoints = H5Sget_simple_extent_npoints(dataspace_id)) == 0)
    goto done;

  if((*dims = calloc(*nDims, sizeof(hsize_t))) == NULL)
    goto done;

  if (H5Sget_simple_extent_dims(dataspace_id, *dims, NULL) < 0)
    goto done;

  if ((*dataset = calloc(*nPoints, sizeof(float))) == NULL)
    goto done;

  if (H5Dread(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, *dataset))
    goto done;

  ret_value = EXIT_SUCCESS;
  
done:
  if (ret_value == EXIT_FAILURE)
    {
      if(*dims != NULL)
        {
          free(*dims);
          *dims = NULL;
        }

      if(*dataset != NULL)
        {
          free(*dataset);
          *dataset = NULL;
        }  
    }

  if (dataset_id > 0)
    H5Dclose(dataset_id);

  if (dcpl_id > 0)
    H5Pclose(dcpl_id);

  if (dataspace_id > 0)
    H5Sclose(dataspace_id);

  if (file_id > 0)
    H5Fclose(file_id);

  return ret_value;
}

/*================================================================================================*/
/**
 * @details This function initializes the BigWhoop HDF5 filter plug in
 *
 * @param[in]  cd_nelmts    Number of filter parameters.
 * @param[in]  cd_values    Filter-specific client data.
 * @param[in]  nDims        Number of dimensions.
 * @param[in]  chunk_dims   Dimensions defining chuncking.
 * 
 * @return Data set creation properties
 */
/*================================================================================================*/
static hid_t 
init_bwc_filter(unsigned int *cd_values,
                size_t        cd_nelmts,
                unsigned int  nDims,
                hsize_t      *chunk_dims)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  hid_t                   dcpl_id;

  /* Create the data set creation property list and define  */
  /* the appropriate chuncking.                             */
  if (0 > (dcpl_id = H5Pcreate(H5P_DATASET_CREATE)))
    {
      fprintf(stderr, "Failed to create data set.\n");
      return 1;
    }

  if (0 > H5Pset_chunk(dcpl_id, nDims, chunk_dims))
    {
      fprintf(stderr, "Failed to invoke H5Pcreate.\n");
      return 1;
    }

  /* Add filter to the pipeline via generic interface.      */
  if (0 > H5Pset_filter(dcpl_id, 33000, H5Z_FLAG_MANDATORY, cd_nelmts, cd_values))
    {
      fprintf(stderr, "Failed to set BigWhoop as filter.\n");
      return 1;
    }

  return dcpl_id;
}
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||             ___  _  _ ___  _    _ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____             ||
||             |__] |  | |__] |    | |       |___ |  | |\ | |     |  | |  | |\ | [__              ||
||             |    |__| |__] |___ | |___    |    |__| | \| |___  |  | |__| | \| ___]             ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*================================================================================================*/
/**
 * @details This function tests the H5Zbwc plugin by generating a synthetic field, writing it to
 *          an HDF5 utilizing the BigWhoop compressor and comparing the decompressed result to the
 *          original field.
 *
 * @param[in] argc  Number of strings pointed to by argv
 * @param[in] argv  Array of arguments
 * 
 * @retval EXIT_FAILURE Failure
 * @retval EXIT_SUCCESS Success
 * 
 * @return  Error flag
 */
/*================================================================================================*/
int 
main(int  __attribute__((unused))   argc, 
     char __attribute__((unused)) **argv)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  int                     p;
  size_t                  ret_value = EXIT_SUCCESS;

  size_t                  cd_nelmts = 26;
  unsigned int            cd_values[26] = {0};

  hsize_t                 i;
  hsize_t                 nDims   = 0;
  hsize_t                 nPoints = 0;
  hsize_t                *dims    = NULL;

  hid_t                   file_id = -1;
  hid_t                   dataspace_id = -1;
  hid_t                   dataset_id = -1;
  hid_t                   dcpl_id = -1;

  /*-----------------------*\
  ! DEFINE FLOAT VARIABLES: !
  \*-----------------------*/
  float                   bitrate = 1.1f;
  float                  *dataset = NULL;
  float                  *cdset   = NULL;

  double                  MSE, PSNR;
  double                  peakVal;
  double                  sum;
  double                  minVal, maxVal;

  /* Ingest the test data setGenerate the test data set.    */
  if(init_t2m_field("/mnt/Data/HLRS/Codes/Data/mpas_surface_t2m_10KM_3days.nc", "t2m", &nDims, &nPoints, &dims, &dataset) == EXIT_FAILURE)
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }

  /* Initialize the HDF5 output file.                       */
  if((file_id = H5Fcreate("tmp.hf5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT)) < 0)
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }

  if((dataspace_id = H5Screate_simple(nDims, dims, 0)) < 0)
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }

  //H5Pset_bwc_error_resilience(cd_nelmts, cd_values);
  H5Pset_bwc_bitrate(bitrate, cd_nelmts, cd_values);
  //H5Pset_bwc_codeblocks(4, cd_nelmts, cd_values);
  //H5Pset_bwc_codeblocks_d(2, 3, 4, 0, cd_nelmts, cd_values);
  //H5Pset_bwc_precinct(14, cd_nelmts, cd_values);
  //H5Pset_bwc_precinct_d(11, 12, 13, 0, cd_nelmts, cd_values);
  //H5Pset_bwc_tile(128, cd_nelmts, cd_values);
  //H5Pset_bwc_tile_d(32, 64, 128, 1, cd_nelmts, cd_values);
  //H5Pset_bwc_decomp(1, cd_nelmts, cd_values);
  H5Pset_bwc_decomp_d(6, 22, 0, 0, cd_nelmts, cd_values);
  //H5Pset_bwc_qm(32, cd_nelmts, cd_values);

  dcpl_id = init_bwc_filter(cd_values, cd_nelmts, nDims, dims);

  if (0 > (dataset_id = H5Dcreate(file_id, "t2m", H5T_NATIVE_FLOAT, 
                                  dataspace_id, H5P_DEFAULT, dcpl_id, H5P_DEFAULT)))
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }

  if (0 > H5Dwrite(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, dataset))
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }

  if (0 > H5Dclose(dataset_id))
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }
  dataset_id = -1;

  if ((H5Pclose(dcpl_id) < 0)     ||
      (H5Sclose(dataspace_id) < 0)||
      (H5Fclose(file_id) < 0))
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }

  dcpl_id       =
  dataspace_id  =
  file_id       = -1;

  /* Read the HDF5 file.                                    */
  if((file_id = H5Fopen("tmp.hf5", H5F_ACC_RDONLY, H5P_DEFAULT)) < 0)
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }

  if (0 > (dataset_id = H5Dopen2(file_id, "t2m", H5P_DEFAULT)))
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }

  if(NULL == (cdset = (float*)calloc(nPoints, sizeof(float))))
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }

  if (0 > H5Dread(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, cdset))
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }

  if (0 > H5Dclose(dataset_id))
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }
  dataset_id = -1;

  if ((H5Fclose(file_id) < 0))
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }
  file_id       = -1;


  /* Read the HDF5 file.                                    */
  if((file_id = H5Fopen("/mnt/Data/HLRS/Codes/Data/compression_20250916/Lossy/BigWhoop/Data/compressed_data_t2m_bwc_crate_test.nc", H5F_ACC_RDWR, H5P_DEFAULT)) < 0)
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }

  if (0 > (dataset_id = H5Dopen2(file_id, "t2m", H5P_DEFAULT)))
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }

  if (0 > H5Dwrite(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, cdset))
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }

  if (0 > H5Dclose(dataset_id))
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }
  dataset_id = -1;

  if ((H5Fclose(file_id) < 0))
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }
  file_id       = -1;

  /* Output the peak signal-to-noise ratio.                 */
  peakVal = -1.7976931348623157e+308;
  PSNR    =
  MSE     = 0;

  for(p = 0; p < 5; ++p)
    {
      minVal = 1.7976931348623157e+308;
      maxVal = -1.7976931348623157e+308;

      for(i = 0; i < nPoints; ++i)
        {
          minVal  = MIN(minVal, (double)dataset[i]);
          maxVal  = MAX(maxVal, (double)dataset[i]);

          sum     = ((double)cdset[i] - (double)dataset[i]);

          MSE    += sum * sum;
        }

      peakVal = MAX(peakVal, maxVal - minVal);
    }

  MSE /= (double)nPoints * 5;
  PSNR = 20 * log10(peakVal/(2 * sqrt(MSE)));

  printf("==============================================================\n");
  printf("  Mean Square Error:             %*.2e\n", 22, MSE);
  printf("  Peak Signal-to-Noise Ratio:    %*.2f\n", 22, PSNR);
  printf("==============================================================\n");


  done:

    remove("tmp.hf5");

    if (dataset_id > 0)
      H5Dclose(dataset_id);

    if (dcpl_id > 0)
      H5Pclose(dcpl_id);

    if (dataspace_id > 0)
      H5Sclose(dataspace_id);

    if (file_id > 0)
      H5Fclose(file_id);

    H5close();

    return ret_value;
}
