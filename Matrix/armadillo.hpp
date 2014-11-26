#ifndef ARMADILLO_H
#define ARMADILLO_H

#define ARMA_NO_DEBUG
#define ARMA_DONT_USE_WRAPPER
#define ARMA_USE_LAPACK
#define ARMA_USE_BLAS
#define ARMA_USE_CXX11

#include <armadillo>

namespace scaffold { namespace matrix {

// Basic types
template<class T> using vec = arma::Col<T>;
template<class T> using mat = arma::Mat<T>;
template<class T> using cube = arma::Cube<T>;
template<class T> using field = arma::field<T>;

// Initialization
template<class T>
inline T identity(int i, int j) { return arma::eye<T>(i,j); }

template<class T>
inline T ones(int i) { return arma::ones<T>(i); }
template<class T>
inline T ones(int i, int j) { return arma::ones<T>(i,j); }
template<class T>
inline T ones(int i, int j, int k) { return arma::ones<T>(i,j,k); }

template<class T>
inline T random(int i) { return arma::randu<T>(i); }
template<class T>
inline T random(int i, int j) { return arma::randu<T>(i,j); }
template<class T>
inline T random(int i, int j, int k) { return arma::randu<T>(i,j,k); }

template<class T>
inline T zeros(int i) { return arma::zeros<T>(i); }
template<class T>
inline T zeros(int i, int j) { return arma::zeros<T>(i,j); }
template<class T>
inline T zeros(int i, int j, int k) { return arma::zeros<T>(i,j,k); }

// Functions
template <typename T>
auto sum(T&& val) -> decltype(arma::accu(std::forward<T>(val))) { return arma::accu(std::forward<T>(val)); }

template<class T>
auto det(T&& val) -> decltype(arma::det(std::forward<T>(val))) { return arma::det(std::forward<T>(val)); }

template<class T>
auto inv(T&& val) -> decltype(arma::inv(std::forward<T>(val))) { return arma::inv(std::forward<T>(val)); }

template<class T>
auto inv(T&& val1, T&& val2) -> decltype(arma::inv(std::forward<T>(val1),std::forward<T>(val2))) { return arma::inv(std::forward<T>(val1),std::forward<T>(val2)); }

template<class T>
auto mag(T&& val) -> decltype(arma::norm(std::forward<T>(val),2)) { return arma::norm(std::forward<T>(val),2); }

template<class T>
auto dot(T&& val1, T&& val2) -> decltype(arma::dot(std::forward(val1),std::forward(val2))) { return arma::dot(std::forward(val1),std::forward(val2)); }

template<class T>
auto solve(T&& val1, T&& val2) -> decltype(arma::solve(std::forward(val1),std::forward(val2))) { return arma::solve(std::forward(val1),std::forward(val2)); }

}} // namespace

#endif
