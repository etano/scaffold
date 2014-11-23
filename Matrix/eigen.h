#ifndef EIGEN_H
#define EIGEN_H

#include <eigen3/Eigen/Eigen>

// Basic types
template<class T> using vec = Eigen::Matrix<T, Eigen::Dynamic, 1>;
template<class T> using mat = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;
template<class T>
class cube {
  public:
    cube (int _n_rows, int _n_cols, int _n_slices)
      : n_slices(_n_slices), n_rows(_n_rows), n_cols(_n_cols),data(_n_rows,_n_slices*_n_cols)
    {}

    T operator()(const int i, const int j, const int k)
    {
      return data(i,k*n_cols + j);
    }

    friend std::ostream& operator<<(std::ostream& out, cube<T> &val)
    {
      out << val.data;
      return out;
    }

    mat<T> data;
    int n_slices, n_rows, n_cols;
};

template<class T> using field = std::vector<T>;

// Initialization
template<class T>
inline T identity(int i, int j) { return T::Identity(i,j); }

template<class T>
inline T ones(int i) { return T::Constant(i,1); }
template<class T>
inline T ones(int i, int j) { return T::Constant(i,j,1); }

template<class T>
inline T random(int i) { return T::Random(i); }
template<class T>
inline T random(int i, int j) { return T::Random(i,j); }

template<class T>
inline T zeros(int i) { return T::Zero(i); }
template<class T>
inline T zeros(int i, int j) { return T::Zero(i,j); }

// Functions
template <typename T>
auto sum(T&& val) -> decltype(std::forward<T>(val).sum()) { return std::forward<T>(val).sum(); }

template<class T>
auto det(T&& val) -> decltype(std::forward<T>(val).determinant()) { return std::forward<T>(val).determinant(); }

template<class T>
auto inv(T&& val) -> decltype(std::forward<T>(val).inverse()) { return std::forward<T>(val).inverse(); }

template<class T>
inline bool inv(T &val1, T &val2) { val1 = val2.inverse(); return false; }

template<class T>
auto mag(T&& val) -> decltype(std::forward<T>(val).norm()) { return std::forward<T>(val).norm(); }

template<class T>
auto dot(T&& val1, T&& val2) -> decltype(std::forward(val1).dot(std::forward(val2))) { return std::forward(val1).dot(std::forward(val2)); }

template<class T>
inline T solve(T &val1, T &val2) { return val1.fullPivLu().solve(val2); }

#endif
