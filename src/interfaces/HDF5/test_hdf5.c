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
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "hdf5.h"
#include "H5Zbwc.h"

#include "bwc.h"

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
 * @details This function generates the initial field of the Taylor Green Vortex test case.
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
init_3d_field(hsize_t *dims,
              double **rho,
              double **rhou,
              double **rhov,
              double **rhow,
              double **rhoE)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  hsize_t                 domain_size;

  hsize_t                 n;
  hsize_t                 i, j, k;
  hsize_t                 x, y, z;

  const uint16_t          N = 256;

  /*-----------------------*\
  ! DEFINE FLOAT VARIABLES: !
  \*-----------------------*/
  double                  u, v;
  double                  dX, dY, dZ;

  const double            PI = 3.14159265358979323846f;
  const double            GAMMA = 1.4f;

  /* Initialize the grid variables                          */
  dX = (2.0f * PI) / dims[0];
  dY = (2.0f * PI) / dims[1];
  dZ = (2.0f * PI) / dims[2];

  domain_size = dims[0] * dims[1] * dims[2];

  /* Allocate the variable arrays                           */
  *rho  = (double *)calloc(domain_size, sizeof(double));
  *rhou = (double *)calloc(domain_size, sizeof(double));
  *rhov = (double *)calloc(domain_size, sizeof(double));
  *rhow = (double *)calloc(domain_size, sizeof(double));
  *rhoE = (double *)calloc(domain_size, sizeof(double));

  if (*rho == NULL || *rhou == NULL || *rhov == NULL 
                   || *rhow == NULL || *rhoE == NULL)
    { 
      if(*rho != NULL)
        free(*rho);
      if(*rhou != NULL)
        free(*rhou);
      if(*rhov != NULL)
        free(*rhov);
      if(*rhow != NULL)
        free(*rhow);
      if(*rhoE != NULL)
        free(*rhoE);

      fprintf(stderr, "o##########################################################o\n"\
                      "|                   ERROR: Out of memory                   |\n"\
                      "o##########################################################o\n");
      return EXIT_FAILURE; 
    }

  /* Populate the fields with their appropriate values.     */
  for (i = 0; i < dims[0]; i++)
    {
      for (j = 0; j < dims[1]; j++)
        {
          for (k = 0; k < dims[2]; k++)
            {
              n = i + (dims[1] * (j + (dims[2] *k)));

              u =  sin(i * dX) * cos(j * dY) * cos(k * dZ);
              v = -cos(i * dX) * sin(j * dY) * cos(k * dZ);

              (*rho)[n]  = 1.0f;
              (*rhou)[n] = u;
              (*rhov)[n] = v;
              (*rhow)[n] = 0.0f;
              (*rhoE)[n] = 1.0f / ((GAMMA - 1.0f)) + (0.5f * (u*u + v*v));
            }
        }
    }
  
  return EXIT_SUCCESS;
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
main(int    argc, 
     char **argv)
{
  /*-----------------------*\
  ! DEFINE INT VARIABLES:   !
  \*-----------------------*/
  int                     i, p;
  size_t                  ret_value = EXIT_SUCCESS;

  size_t                  cd_nelmts = 26;
  unsigned int            cd_values[26] = {0};

  hsize_t                 nDims   = 3;
  hsize_t                 nPoints = 0;
  hsize_t                 dims[] = {256, 256, 256};
  hsize_t                 chunk_dims[] = {256, 256, 256};

  hid_t                   file_id = -1;
  hid_t                   dataspace_id = -1;
  hid_t                   dataset_id = -1;
  hid_t                   dcpl_id = -1;

  /*-----------------------*\
  ! DEFINE FLOAT VARIABLES: !
  \*-----------------------*/
  char                   var_name[5][5] = {{"rho"}, {"rhou"}, {"rhov"}, {"rhow"}, {"rhoE"}};

  /*-----------------------*\
  ! DEFINE FLOAT VARIABLES: !
  \*-----------------------*/
  float                   bitrate = 0.01f;
  double                 *var[5]  = {NULL};
  double                 *varC[5] = {NULL};

  double                  MSE, PSNR;
  double                  peakVal;
  double                  sum;
  double                  minVal, maxVal;

  double                  MSER, PSNRR;
  double                 *compr, *decompr;
  bwc_stream             *stream = NULL;
  bwc_codec              *coder  = NULL;

  /* Initialize the HDF5 output file.                       */
  if((file_id = H5Fcreate("tmp.hf5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT)) < 0)
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }

  if((dataspace_id = H5Screate_simple(3, dims, 0)) < 0)
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }

  /* Generate the test data set and set up the HDF5 filter. */
  if(init_3d_field(dims, &var[0], &var[1], &var[2], &var[3], &var[4]) == EXIT_FAILURE)
    {
      ret_value = EXIT_FAILURE;
      goto done;
    }

  H5Pset_bwc_bitrate(bitrate, cd_nelmts, cd_values);
  H5Pset_bwc_qm(32, cd_nelmts, cd_values);

  dcpl_id = init_bwc_filter(cd_values, cd_nelmts, nDims, chunk_dims);

  /* Create the test dataset and write it to file.          */
  for(i = 0; i < 5; ++i)
    {
      if (0 > (dataset_id = H5Dcreate(file_id, var_name[i], H5T_NATIVE_DOUBLE, 
                                      dataspace_id, H5P_DEFAULT, dcpl_id, H5P_DEFAULT)))
        {
          ret_value = EXIT_FAILURE;
          goto done;
        }

      if (0 > H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, var[i]))
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
    }

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

  for(i = 0; i < 5; ++i)
    {
      if (0 > (dataset_id = H5Dopen2(file_id, var_name[i], H5P_DEFAULT)))
        {
          ret_value = EXIT_FAILURE;
          goto done;
        }

      if(0 > (dataspace_id = H5Dget_space(dataset_id)))
        {
          ret_value = EXIT_FAILURE;
          goto done;
        }

      if(0 == (nPoints = H5Sget_simple_extent_npoints(dataspace_id)))
        {
          ret_value = EXIT_FAILURE;
          goto done;
        }

      if (0 > H5Sclose(dataspace_id))
        {
          ret_value = EXIT_FAILURE;
          goto done;
        }
      dataspace_id = -1;

      if(NULL == (varC[i] = (double*)calloc(nPoints, sizeof(double))))
        {
          ret_value = EXIT_FAILURE;
          goto done;
        }

      if (0 > H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, varC[i]))
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
    }

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
          minVal  = MIN(minVal, (double)varC[p][i]);
          maxVal  = MAX(maxVal, (double)varC[p][i]);

          sum     = ((double)varC[p][i] - (double)var[p][i]);

          MSE    += sum * sum;
        }

      peakVal = MAX(peakVal, maxVal - minVal);
    }

  MSE /= (double)nPoints * 5;
  PSNR = 20 * log10(peakVal/(2 * sqrt(MSE)));

  if ((PSNR-75.16f) > 0.01) ret_value = EXIT_FAILURE;

  done:

    remove("tmp.hf5");

    for (i = 0; i < 5; ++i)
      {
        if (var[i])
          free(var[i]);

        if (varC[i])
          free(varC[i]);    
      }

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
