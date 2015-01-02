scaffold
========

Utilities for fast parallel communication and IO wrapped neatly in C++

## Dependencies

* HDF5 (with c++ API) - http://www.hdfgroup.org/HDF5/
* Armadillo c++ - http://arma.sourceforge.net/ OR Eigen http://eigen.tuxfamily.org/

These dependencies will automatically be installed by cmake. However, if you'd like to install them yourself, be sure to set the environmental variables:

    export HDF5_HOME=/hdf5/install/directory
    export ARMADILLO_HOME=/armadillo/install/directory
    export EIGEN_HOME=/eigen/install/directory

## Usage

For an example usage and tests, please see the [HPCTemplate](https://github.com/etano/HPCTemplate) repository.
