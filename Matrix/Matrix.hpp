#ifndef SCAFFOLD_MATRIX
#define SCAFFOLD_MATRIX

#include "../types.hpp"

#ifdef USE_EIGEN
  #define EIGEN_MATRIXBASE_PLUGIN "/Users/ethan/src/simpimc/src/Utils/Matrix/eigen_matrix_additions.h"
  #include "eigen.hpp"
#else
  #include "armadillo.hpp"
#endif

#endif
