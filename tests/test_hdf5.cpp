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
 *        @file test_hdf5.cpp
 *
 *        This file defines unit tests for BigWhoop's HDF5 plugin.
 *                                                                                                */
/*  --------------------------------------------------------------------------------------------  *\
||  Copyright (c) 2025, High Performance Computing Center - University of Stuttgart               ||
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
// clang-format off
#include <algorithm>                                      //!< Algorithmic building blocks
#include <catch2/catch_test_macros.hpp>                   //!< Catch2 unit testing macros
#include <cstdint>                                        //!< Fixed-width integers
#include <cstdlib>                                        //!< Standard C library
#include <cmath>                                          //!< Math operations
#include <H5Cpp.h>                                        //!< HDF5 header
#include <iostream>                                       //!< Object oriented I/O
#include <limits>                                         //!< Numeric type limits
#include <numeric>                                        //!< Numeric operations
#include <string>                                         //!< String class template
#include <tuple>                                          //!< Heterogeneous return values
#include <vector>                                         //!< Vector class template

#ifdef __cplusplus
extern "C"
{
#endif
#include "../include/interfaces/HDF5/H5Zbwc.h"            //!< BWC HDF5 plugin
#ifdef __cplusplus
}
#endif

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||                                ____ _    _ ____ ____ ____ ____                                 ||
||                                |__| |    | |__| [__  |___ [__                                  ||
||                                |  | |___ | |  | ___] |___ ___]                                 ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
using DoubleVec = std::vector<double>;                    //!< Vector of doubles
using HSizeVec  = std::vector<hsize_t>;                   //!< Vector of integers
using UIntVec   = std::vector<unsigned int>;              //!< Vector of unsigned integers
using FieldSet  = std::vector<DoubleVec>;                 //!< Vector of vectours of doubles
using ResTuple  = std::tuple<hsize_t, FieldSet>;          //!< Tuple containing Comp. results

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||                           ____ ____ _  _ ____ ___ ____ _  _ ___ ____                           ||
||                           |    |  | |\ | [__   |  |__| |\ |  |  [__                            ||
||                           |___ |__| | \| ___]  |  |  | | \|  |  ___]                           ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
constexpr double PI = 3.14159265358979323846;             //!< Mathematical constant
constexpr double GAMMA = 1.4;                             //!< Specific heat ratio

constexpr size_t FILTER_PARAM_COUNT = 26;                 //!< Expected number of cd_values
constexpr size_t NUM_VARIABLES = 5;                       //!< Number of numerical variables
constexpr double PSNR_TOLERANCE = 0.01;                   //!< PSNR tolerance after compression
constexpr const char* VAR_NAMES[NUM_VARIABLES] = {"rho",
                                                  "rhou",
                                                  "rhov",
                                                  "rhow",
                                                  "rhoE"};//!< PSNR tolerance after compression
// clang-format on

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||           ___  ____ _ _  _ ____ ___ ____    ____ _  _ _  _ ____ ___ _ ____ _  _ ____           ||
||           |__] |__/ | |  | |__|  |  |___    |___ |  | |\ | |     |  | |  | |\ | [__            ||
||           |    |  \ |  \/  |  |  |  |___    |    |__| | \| |___  |  | |__| | \| ___]           ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*================================================================================================*/
/**
 * @details Generates the initial field of the Taylor Green Vortex test case.
 *
 * @param[in] dims Dimension of the grid
 *
 * @return 3 dimensional flow field containing variables rho, rhou, rhov, rhow, rhoE.
 */
/*================================================================================================*/
static FieldSet
init_3d_field(const HSizeVec& dims)
{
  if (dims.size() != 3)
    throw std::runtime_error("init_3d_field requires 3 dimensions.");

  hsize_t nx = dims[0], ny = dims[1], nz = dims[2];
  hsize_t domain_size = nx * ny * nz;

  double dX = (2.0 * PI) / static_cast<double>(nx);
  double dY = (2.0 * PI) / static_cast<double>(ny);
  double dZ = (2.0 * PI) / static_cast<double>(nz);

  FieldSet vars(5, DoubleVec(domain_size));

  for (hsize_t k = 0; k < nz; ++k)
    {
      for (hsize_t j = 0; j < ny; ++j) 
        {
          for (hsize_t i = 0; i < nx; ++i)
            {
              hsize_t n = i + (ny * (j + (nz * k)));

              double u = std::sin(static_cast<double>(i) * dX) *
                         std::cos(static_cast<double>(j) * dY) *
                         std::cos(static_cast<double>(k) * dZ);

              double v = -std::cos(static_cast<double>(i) * dX) *
                          std::sin(static_cast<double>(j) * dY) *
                          std::cos(static_cast<double>(k) * dZ);

              vars[0][n] = 1.0;
              vars[1][n] = u;
              vars[2][n] = v;
              vars[3][n] = 0.0;
              vars[4][n] = 1.0 / (GAMMA - 1.0) + (0.5 * (u * u + v * v));
           }
        }
    }
  return vars;
}

/*================================================================================================*/
/**
 * @details Applies the BigWhoop compression to the original data (varOrig) according to the user
 *          supplied compression parameters and returns the decompressed version to the function
 *          caller.
 *
 * @param[in] varOrig   Reference to original data
 * @param[in] dims      Dimension of the grid
 * @param[in] cd_values Filter-specific client data.
 *
 * @return Processed data
 */
/*================================================================================================*/
static ResTuple
hdf5_apply_bwc(const FieldSet& varOrig,
               const HSizeVec& dims,
               const UIntVec&  cd_values)
{
  /* Check arguments for correctness.                       */
  if (dims.empty())
    throw std::runtime_error("Dimensions cannot be empty.");

  hsize_t domain_size = std::accumulate(dims.begin(), dims.end(), 1.0, std::multiplies<hsize_t>());

  bool shapes_match = (varOrig.size() == NUM_VARIABLES);
  if (shapes_match == true)
    {
      for (size_t i = 0; i < NUM_VARIABLES; ++i)
        {
          if (varOrig[i].size() != domain_size)
            {
              shapes_match = false;
              break;
            }
        }
    }

  if (shapes_match == false)
    throw std::runtime_error("Size and shape of input does not match supplied dimensions.");

  if (cd_values.size() != FILTER_PARAM_COUNT) 
    {
      throw std::runtime_error("BigWhoop filter requires exactly " + 
                               std::to_string(FILTER_PARAM_COUNT) + 
                               " configuration parameters. Got " + 
                                std::to_string(cd_values.size()) + ".");
    }

  /* Initialize Filter                                      */
  H5::DSetCreatPropList dcpl_id;
  dcpl_id.setChunk(static_cast<unsigned int>(dims.size()), dims.data());
  dcpl_id.setFilter(33000, H5Z_FLAG_MANDATORY, cd_values.size(), cd_values.data());

  /* Write Phase                                            */
  {
    H5::H5File file("tmp.hf5", H5F_ACC_TRUNC);
    H5::DataSpace dataspace(static_cast<unsigned int>(dims.size()), dims.data());
    for (size_t i = 0; i < NUM_VARIABLES; ++i)
    {
      H5::DataSet dataset = file.createDataSet(VAR_NAMES[i],
                                               H5::PredType::NATIVE_DOUBLE,
                                               dataspace,
                                               dcpl_id);
      dataset.write(varOrig[i].data(), H5::PredType::NATIVE_DOUBLE);
    }
  }

  /* Read Phase                                             */
  FieldSet varComp(NUM_VARIABLES, DoubleVec(domain_size));
  hsize_t fieldSize = 0;
  {
    H5::H5File file("tmp.hf5", H5F_ACC_RDONLY);
    for (size_t i = 0; i < NUM_VARIABLES; ++i)
      {
        H5::DataSet dataset = file.openDataSet(VAR_NAMES[i]);
        fieldSize += dataset.getStorageSize();
        dataset.read(varComp[i].data(), H5::PredType::NATIVE_DOUBLE);
      }
  }

  std::remove("tmp.hf5"); 
  return std::make_tuple(fieldSize, varComp);
}

/*================================================================================================*/
/**
 * @details This function calculates the Peak Signa-to-Noise Ratio (PSNR) of a test array after
 *          lossy comrpession.
 *
 * @param[in] varOrig    Reference to original data
 * @param[in] varComp  Reference to compressed data
 *
 * @return Peak Signal-to-Noise Ratio
 */
/*================================================================================================*/
static double 
calculate_psnr(const FieldSet& varOrig,
               const FieldSet& varComp)
{
  bool shapes_match = (varOrig.size() == varComp.size());
  if (shapes_match == true)
    {
      for (size_t i = 0; i < varOrig.size(); ++i)
        {
          if (varOrig[i].size() != varComp[i].size())
            {
              shapes_match = false;
              break;
            }
        }
    }

  if (shapes_match == false)
    throw std::runtime_error("Size and shape of input arrays do not match.");

  double peakVal = std::numeric_limits<double>::lowest();
  double mseSum = 0.0;

  hsize_t totalParams  = varOrig.size();
  hsize_t totalPoints  = 0;

  for (int p = 0; p < totalParams; ++p)
    {
      double minVal = std::numeric_limits<double>::max();
      double maxVal = std::numeric_limits<double>::lowest();

      const auto& orig = varOrig[p];
      const auto& comp = varComp[p];

      totalPoints = orig.size();

      for (hsize_t i = 0; i < totalPoints; ++i)
        {
          double valComp = comp[i];
          double valOrig = orig[i];

          minVal = std::min(minVal, valOrig);
          maxVal = std::max(maxVal, valOrig);

          double diff = valOrig - valComp;
          mseSum += diff * diff;
        }

      peakVal = std::max(peakVal, maxVal - minVal);
    }

  double mse = mseSum / (static_cast<double>(totalPoints) * static_cast<double>(totalParams));
  
  if (mse == 0.0)
    return 100.0;
  else
    return 20.0 * std::log10(peakVal / (2.0 * std::sqrt(mse)));
}

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*\
||                           _  _ _  _ _ ___    ___ ____ ____ ___ ____                            ||
||                           |  | |\ | |  |      |  |___ [__   |  [__                             ||
||                           |__| | \| |  |      |  |___ ___]  |  ___]                            ||
||                                                                                                ||
\*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
TEST_CASE("BigWhoop HDF5 Plugin - 3D Field", "[bwc][hdf5][compression]") 
{
  const double  TOLERANCE = 0.1f;
  const float   BITRATE = 0.1f;

  UIntVec       cd_values(FILTER_PARAM_COUNT, 0);
  unsigned int  cd_nelmts = static_cast<unsigned int>(cd_values.size());
  HSizeVec      dims(3, 128);

  FieldSet varOrig = init_3d_field(dims);

  SECTION("Full Fixed Precision")
    {
      const double  EXPECTED_PSNR = 95.1f;

      H5Pset_bwc_bitrate (BITRATE, cd_nelmts, cd_values.data());
      // H5Pset_bwc_qm (32, cd_nelmts, cd_values.data());
      
      ResTuple result = hdf5_apply_bwc(varOrig, dims, cd_values);
      hsize_t fieldSizeComp = std::get<0>(result);
      FieldSet varComp = std::get<1>(result);

      hsize_t domain_size = std::accumulate(dims.begin(), dims.end(), 1.0, std::multiplies<hsize_t>());
      hsize_t fieldSize   = NUM_VARIABLES * domain_size;

      REQUIRE((static_cast<double>(fieldSizeComp)/fieldSize) < BITRATE);

      double psnr = calculate_psnr(varOrig, varComp);

      REQUIRE(std::abs(EXPECTED_PSNR - psnr) < TOLERANCE);

      INFO("PSNR: " << psnr);
      INFO("Compressed Size: " << fieldSizeComp << " bytes");
      INFO("Uncompressed Size: " << domain_size << " bytes");
      INFO("Compression Ratio: " << (static_cast<double>(domain_size) / fieldSizeComp));
    }

  SECTION("Half Fixed Precision")
    {
      const double  EXPECTED_PSNR = 92.2;

      H5Pset_bwc_bitrate (BITRATE, cd_nelmts, cd_values.data());
      H5Pset_bwc_qm (32, cd_nelmts, cd_values.data());
      
      ResTuple result = hdf5_apply_bwc(varOrig, dims, cd_values);
      hsize_t fieldSizeComp = std::get<0>(result);
      FieldSet varComp = std::get<1>(result);

      hsize_t domain_size = std::accumulate(dims.begin(), dims.end(), 1.0, std::multiplies<hsize_t>());
      hsize_t fieldSize   = NUM_VARIABLES * domain_size;

      REQUIRE((static_cast<double>(fieldSizeComp)/fieldSize) < BITRATE);

      double psnr = calculate_psnr(varOrig, varComp);

      REQUIRE(std::abs(EXPECTED_PSNR - psnr) < TOLERANCE);

      INFO("PSNR: " << psnr);
      INFO("Compressed Size: " << fieldSizeComp << " bytes");
      INFO("Uncompressed Size: " << domain_size << " bytes");
      INFO("Compression Ratio: " << (static_cast<double>(domain_size) / fieldSizeComp));
    }
}