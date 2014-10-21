#ifndef CONFIG
#define CONFIG

#define ARMA_NO_DEBUG
#define ARMA_DONT_USE_WRAPPER
#define ARMA_USE_LAPACK
#define ARMA_USE_BLAS
#define ARMA_USE_CXX11
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

typedef arma::Cube<RealType> Tcube;
typedef arma::Cube<ComplexType> Ccube;
typedef arma::Mat<RealType> Tmatrix;
typedef arma::Mat<ComplexType> Cmatrix;
typedef arma::Col<RealType> Tvector;
typedef arma::Col<ComplexType> Cvector;
typedef arma::Mat<IntType> Imatrix;
typedef arma::Col<IntType> Ivector;
typedef arma::Mat<bool> Bmatrix;
typedef arma::Col<bool> Bvector;

//template<class T> using vec = arma::Col<T>;
//template<class T> using mat = arma::Mat<T>;
//template<class T> using cube = arma::Cube<T>;
template<class T> using field = arma::field<T>;

template<class T>
inline ComplexType cdet(T val) { return arma::det(val); }
template<class T>
inline RealType det(T val) { return arma::det(val); }
template<class T>
inline Cmatrix cinv(T val) { return arma::inv(val); }
template<class T>
inline Tmatrix inv(T val) { return arma::inv(val); }
template<class T>
inline ComplexType cdot(T val1, T val2) { return arma::cdot(val1,val2); }
template<class T>
inline RealType dot(T val1, T val2) { return arma::dot(val1,val2); }
inline RealType mag(Tvector &val) { return arma::norm(val,2); }

inline double cmag2 (const ComplexType &z1, const ComplexType &z2) { return (z1.real()*z2.real() + z1.imag()*z2.imag()); }
inline double cmag (const ComplexType &z1, const ComplexType &z2) { return (sqrt(cmag2(z1,z2))); }

#endif
