#ifndef SCAFFOLD_MATRIX
#define SCAFFOLD_MATRIX

#ifdef USE_EIGEN
  #define EIGEN_MATRIXBASE_PLUGIN <Matrix/eigen_matrix_additions.h>
  #include "eigen.hpp"
#else
  #include "armadillo.hpp"
#endif

#endif
