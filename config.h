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

template<class T> using vec = arma::Col<T>;
template<class T> using mat = arma::Mat<T>;
template<class T> using cube = arma::Cube<T>;
template<class T> using field = arma::field<T>;
template<class T, class U>
inline U sum(T &val) { return arma::sum(val); }
template<class T, class U>
inline U det(T &val) { return arma::det(val); }
template<class T, class U>
inline U inv(T &val) { return arma::inv(val); }
template<class T>
inline RealType mag(T &val) { return arma::norm(val,2); }
template<class T>
inline ComplexType cdot(T val1, T val2) { return arma::cdot(val1,val2); }
template<class T>
inline RealType dot(T val1, T val2) { return arma::dot(val1,val2); }

typedef cube<RealType> Tcube;
typedef cube<ComplexType> Ccube;
typedef mat<RealType> Tmatrix;
typedef mat<ComplexType> Cmatrix;
typedef vec<RealType> Tvector;
typedef vec<ComplexType> Cvector;
typedef mat<IntType> Imatrix;
typedef vec<IntType> Ivector;
typedef mat<bool> Bmatrix;
typedef vec<bool> Bvector;

inline RealType cmag2 (const ComplexType &z1, const ComplexType &z2) { return (z1.real()*z2.real() + z1.imag()*z2.imag()); }
inline RealType cmag (const ComplexType &z1, const ComplexType &z2) { return (sqrt(cmag2(z1,z2))); }


#endif
