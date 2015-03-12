#ifndef SCAFFOLD_MATRIX_MATRIX_H_
#define SCAFFOLD_MATRIX_MATRIX_H_

#ifdef USE_EIGEN
  #define EIGEN_MATRIXBASE_PLUGIN <Matrix/eigen_matrix_additions.h>
  #include "eigen.h"
#else
  #include "armadillo.h"
#endif

#endif // SCAFFOLD_MATRIX_MATRIX_H_
