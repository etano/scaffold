#ifndef SCAFFOLD_MATRIX
#define SCAFFOLD_MATRIX

#ifdef USE_EIGEN
  #define EIGEN_MATRIXBASE_PLUGIN SCAFFOLD_DIR "/Matrix/eigen_matrix_additions.h"
  #include "eigen.hpp"
#else
  #include "armadillo.hpp"
#endif

#endif
