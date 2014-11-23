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

//inline RealType cmag2 (const ComplexType &z1, const ComplexType &z2) { return (z1.real()*z2.real() + z1.imag()*z2.imag()); }
//inline RealType cmag (const ComplexType &z1, const ComplexType &z2) { return (sqrt(cmag2(z1,z2))); }

#ifdef USE_ARMADILLO
  #include "Matrix/armadillo.h"
#endif
#ifdef USE_EIGEN
  #define EIGEN_MATRIXBASE_PLUGIN "/Users/ethan/src/HPCTemplate/src/Utils/Matrix/eigen_matrix_additions.h"
  #include "Matrix/eigen.h"
#endif

#endif
