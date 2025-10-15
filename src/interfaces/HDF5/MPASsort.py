#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Sep 30 10:43:44 2025

@author: pvogler
"""

import h5py
import numpy as np

# ----------------------------------------------------------------------
# 1️⃣  Path to your HDF5 file and the name of the dataset you want to sort
# ----------------------------------------------------------------------
h5_path   = "/mnt/Data/HLRS/Codes/Data/mpas_surface_t2m_10KM_3days_sorted_min.nc"          # <-- change to your file
dset_name = "t2m"          # <-- change to the dataset inside the file

# ----------------------------------------------------------------------
# 2️⃣  Open the file in read‑write mode ('r+') – this lets us modify it
# ----------------------------------------------------------------------
with h5py.File(h5_path, 'r+') as f:
    # --------------------------------------------------------------
    # 3️⃣  Load the dataset into memory as a NumPy array
    # --------------------------------------------------------------
    data = f[dset_name][...]          # [:] also works; [...] forces a copy

    # --------------------------------------------------------------
    # 4️⃣  Sort the data
    # --------------------------------------------------------------
    keys = np.min(data, axis=1)      # !!!! Use this command to change sorting (e.g. keys = np.median(data, axis=1))
    perm = np.argsort(keys)
    
    sorted_data = data[perm]


    # --------------------------------------------------------------
    # 5️⃣  Replace the original dataset with the sorted version
    # --------------------------------------------------------------
    if dset_name in f:
        del f[dset_name]               # free the old dataset

    f.create_dataset(
        dset_name,
        data=sorted_data,
        dtype=sorted_data.dtype,
    )

    # --------------------------------------------------------------
    # 6️⃣  (Optional) Flush to ensure data is written before the file closes
    # --------------------------------------------------------------
    f.flush()

f.close()

print(f"Dataset '{dset_name}' in '{h5_path}' has been sorted and saved.")
del(data, dset_name, f, h5_path, keys, perm, sorted_data)
