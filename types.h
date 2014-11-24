#ifndef TYPES_H
#define TYPES_H

#include <complex>
#if PRECISION==double
  typedef double RealType;
  typedef std::complex<double> ComplexType;
#elif PRECISION==single
  typedef float RealType;
  typedef std::complex<float> ComplexType;
#endif
typedef int IntType;

#ifdef USE_EIGEN
  #define EIGEN_MATRIXBASE_PLUGIN "/Users/ethan/src/simpimc/src/Utils/Matrix/eigen_matrix_additions.h"
  #include "Matrix/eigen.h"
#else
  #include "Matrix/armadillo.h"
#endif


#endif
