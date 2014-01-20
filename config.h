#ifndef CONFIG
#define CONFIG

#include <armadillo>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <assert.h>
#include <vector>       // std::vector
#if USE_MPI
  #include <mpi.h>
#endif
#if USE_OPENMP
  #include <omp.h>
#endif

using namespace std;

#if PRECISION==double
typedef double RealType;
typedef complex<double> ComplexType;
#elif PRECISION==single
typedef float RealType;
typedef complex<float> ComplexType;
#endif
typedef int IntType;

typedef arma::Mat<RealType> Tmatrix;
typedef arma::Mat<ComplexType> Cmatrix;
typedef arma::Col<RealType> Tvector;
typedef arma::Col<ComplexType> Cvector;
typedef arma::Mat<IntType> Imatrix;
typedef arma::Col<IntType> Ivector;

template<class T>
inline ComplexType cdet(T val) { return arma::det(val); }
template<class T>
inline RealType det(T val) { return arma::det(val); }
template<class T>
inline Cmatrix cinv(T val) { return arma::inv(val); }
template<class T>
inline Tmatrix inv(T val) { return arma::inv(val); }


#endif
