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

    T& operator()(const int i, const int j, const int k)
    {
      return data(i,k*n_cols + j);
    }

    friend std::ostream& operator<<(std::ostream& out, cube<T> &val)
    {
      out << val.data;
      return out;
    }

    size_t size() { return n_slices*n_rows*n_cols; }

    mat<T> data;
    int n_slices, n_rows, n_cols;
};

template<class T>
class field {
  
  public:
    field () {}

    field (int _n_rows)
      : n_rows(_n_rows), n_cols(1), data_1d(_n_rows), dim(1)
    {}

    field (int _n_rows, int _n_cols)
      : n_rows(_n_rows), n_cols(_n_cols), data_2d(_n_rows), dim(2)
    {
      for (int i=0; i<n_rows; ++i)
        data_2d[i].resize(n_cols);
    }

    T& operator()(const int i)
    {
      return data_1d[i];
    }

    T& operator()(const int i, const int j)
    {
      return data_2d[i][j];
    }

    void set_size(const int _n_rows)
    {
      dim = 1;
      n_rows = _n_rows;
      data_1d.resize(n_rows);
    }

    void set_size(const int _n_rows, const int _n_cols)
    {
      dim = 2;
      n_rows = _n_rows;
      n_cols = _n_cols;
      data_2d.resize(n_rows);
      for (int i=0; i<n_rows; ++i)
        data_2d[i].resize(n_cols);
    }

    size_t size() { return n_rows*n_cols; }

    friend std::ostream& operator<<(std::ostream& out, field<T> &val)
    {
      for (int i=0; i<val.n_rows; ++i) {
        if (val.dim>1) {
          for (int j=0; j<val.n_cols; ++j) {
            out << val(i,j);
          }
        } else {
          out << val(i);
        }
      }
      return out;
    }

    std::vector<T> data_1d;
    std::vector< std::vector<T> > data_2d;
    int n_rows, n_cols, dim;

};

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
auto dot(T& val1, T& val2) -> decltype(val1.dot(val2)) { return val1.dot(val2); }

template<class T>
inline T solve(T &val1, T &val2) { return val1.fullPivLu().solve(val2); }

#endif
