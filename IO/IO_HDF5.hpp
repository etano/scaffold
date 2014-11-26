#ifndef SCAFFOLD_IO_CLASS
#define SCAFFOLD_IO_CLASS

#include "H5Cpp.h"
#include "../Matrix/Matrix.hpp"

namespace scaffold { namespace IO {

// Template to retrieve traits of any HDF5 object
template <class T>
struct hdf5_type_traits {
  static H5::PredType get_type(T& val);
  static inline size_t get_size(T& val);
  static inline void* get_addr(T& val);
  static inline const int get_dim(T& val, int d);
  static inline const int get_rank(T& val);
};

  // Specialization of hdf5_type_traits for primitive types
#define PRIMITIVE(Type, H5PredType, H5Type) \
        template<> \
        inline H5::PredType hdf5_type_traits<Type>::get_type(Type&) { return H5Type; } \
        template<> \
        inline size_t hdf5_type_traits<Type>::get_size(Type&) { return 1; } \
        template<> \
        inline void* hdf5_type_traits<Type>::get_addr(Type& val) { return &val; } \
        template<> \
        inline const int hdf5_type_traits<Type>::get_dim(Type& val, int d) { return 1; } \
        template<> \
        inline const int hdf5_type_traits<Type>::get_rank(Type& val) { return 0; }
  PRIMITIVE(char, H5::IntType, H5::PredType::NATIVE_CHAR);
  PRIMITIVE(std::string, H5::StrType, H5::PredType::NATIVE_CHAR);
  PRIMITIVE(short, H5::IntType, H5::PredType::NATIVE_SHORT);
  PRIMITIVE(int, H5::IntType, H5::PredType::NATIVE_INT);
  PRIMITIVE(long, H5::IntType, H5::PredType::NATIVE_LONG);
  PRIMITIVE(signed char, H5::IntType, H5::PredType::NATIVE_SCHAR);
  PRIMITIVE(unsigned char, H5::IntType, H5::PredType::NATIVE_UCHAR);
  PRIMITIVE(unsigned short, H5::IntType, H5::PredType::NATIVE_USHORT);
  PRIMITIVE(unsigned int, H5::IntType, H5::PredType::NATIVE_UINT);
  PRIMITIVE(unsigned long, H5::IntType, H5::PredType::NATIVE_ULONG);
  PRIMITIVE(unsigned long long, H5::IntType, H5::PredType::NATIVE_ULLONG);
  PRIMITIVE(bool, H5::IntType, H5::PredType::NATIVE_HBOOL);
  PRIMITIVE(std::complex<double>, H5::FloatType, H5::PredType::NATIVE_DOUBLE);
  PRIMITIVE(std::complex<float>, H5::FloatType, H5::PredType::NATIVE_FLOAT);
  PRIMITIVE(double, H5::FloatType, H5::PredType::NATIVE_DOUBLE);
  PRIMITIVE(float, H5::FloatType, H5::PredType::NATIVE_FLOAT);
#undef PRIMITIVE

// Specialization of hdf5_type_traits for armadillo types
#ifdef USE_ARMADILLO
  #define ARMATYPE(Type, ElemType, H5PredType, H5Type) \
          template<> \
          inline H5::PredType hdf5_type_traits<Type>::get_type(Type&) { return H5Type; } \
          template<> \
          inline size_t hdf5_type_traits<Type>::get_size(Type& val) { return val.size(); } \
          template<> \
          inline void* hdf5_type_traits<Type>::get_addr(Type& val) { return val.memptr(); } \
          template<> \
          inline const int hdf5_type_traits<Type>::get_dim(Type& val, int d) { if (d == 0) { return val.n_rows; } else { return val.n_cols; } } \
          template<> \
          inline const int hdf5_type_traits<Type>::get_rank(Type& val) { return 1; }
    ARMATYPE(matrix::vec<int>, int, H5::IntType, H5::PredType::NATIVE_INT);
    ARMATYPE(matrix::vec<float>, float, H5::FloatType, H5::PredType::NATIVE_FLOAT);
    ARMATYPE(matrix::vec<double>, double, H5::FloatType, H5::PredType::NATIVE_DOUBLE);
  #undef ARMATYPE

  #define ARMATYPE(Type, ElemType, H5PredType, H5Type) \
          template<> \
          inline H5::PredType hdf5_type_traits<Type>::get_type(Type&) { return H5Type; } \
          template<> \
          inline size_t hdf5_type_traits<Type>::get_size(Type& val) { return val.size(); } \
          template<> \
          inline void* hdf5_type_traits<Type>::get_addr(Type& val) { return val.memptr(); } \
          template<> \
          inline const int hdf5_type_traits<Type>::get_dim(Type& val, int d) { if (d == 0) { return val.n_cols; } else { return val.n_rows; } } \
          template<> \
          inline const int hdf5_type_traits<Type>::get_rank(Type& val) { return 2; }
    ARMATYPE(matrix::mat<int>, int, H5::IntType, H5::PredType::NATIVE_INT);
    ARMATYPE(matrix::mat<float>, float, H5::FloatType, H5::PredType::NATIVE_FLOAT);
    ARMATYPE(matrix::mat<double>, double, H5::FloatType, H5::PredType::NATIVE_DOUBLE);
  #undef ARMATYPE

  #define ARMATYPE(Type, ElemType, H5PredType, H5Type) \
          template<> \
          inline H5::PredType hdf5_type_traits<Type>::get_type(Type&) { return H5Type; } \
          template<> \
          inline size_t hdf5_type_traits<Type>::get_size(Type& val) { return val.size(); } \
          template<> \
          inline void* hdf5_type_traits<Type>::get_addr(Type& val) { return val.memptr(); } \
          template<> \
          inline const int hdf5_type_traits<Type>::get_dim(Type& val, int d) { if (d == 0) { return val.n_slices; } else if (d == 1) { return val.n_cols; } else { return val.n_rows; } } \
          template<> \
          inline const int hdf5_type_traits<Type>::get_rank(Type& val) { return 3; }
    ARMATYPE(matrix::cube<int>, int, H5::IntType, H5::PredType::NATIVE_INT);
    ARMATYPE(matrix::cube<float>, float, H5::FloatType, H5::PredType::NATIVE_FLOAT);
    ARMATYPE(matrix::cube<double>, double, H5::FloatType, H5::PredType::NATIVE_DOUBLE);
  #undef ARMATYPE
#endif

// Specialization of hdf5_type_traits for armadillo types
#ifdef USE_EIGEN
  #define EIGENTYPE(Type, ElemType, H5PredType, H5Type) \
          template<> \
          inline H5::PredType hdf5_type_traits<Type>::get_type(Type&) { return H5Type; } \
          template<> \
          inline size_t hdf5_type_traits<Type>::get_size(Type& val) { return val.size(); } \
          template<> \
          inline void* hdf5_type_traits<Type>::get_addr(Type& val) { return val.data(); } \
          template<> \
          inline const int hdf5_type_traits<Type>::get_dim(Type& val, int d) { if (d == 0) { return val.rows(); } else { return val.cols(); } } \
          template<> \
          inline const int hdf5_type_traits<Type>::get_rank(Type& val) { return 1; }
    EIGENTYPE(matrix::vec<int>, int, H5::IntType, H5::PredType::NATIVE_INT);
    EIGENTYPE(matrix::vec<float>, float, H5::FloatType, H5::PredType::NATIVE_FLOAT);
    EIGENTYPE(matrix::vec<double>, double, H5::FloatType, H5::PredType::NATIVE_DOUBLE);
  #undef EIGENTYPE

  #define EIGENTYPE(Type, ElemType, H5PredType, H5Type) \
          template<> \
          inline H5::PredType hdf5_type_traits<Type>::get_type(Type&) { return H5Type; } \
          template<> \
          inline size_t hdf5_type_traits<Type>::get_size(Type& val) { return val.size(); } \
          template<> \
          inline void* hdf5_type_traits<Type>::get_addr(Type& val) { return val.data(); } \
          template<> \
          inline const int hdf5_type_traits<Type>::get_dim(Type& val, int d) { if (d == 0) { return val.cols(); } else { return val.rows(); } } \
          template<> \
          inline const int hdf5_type_traits<Type>::get_rank(Type& val) { return 2; }
    EIGENTYPE(matrix::mat<int>, int, H5::IntType, H5::PredType::NATIVE_INT);
    EIGENTYPE(matrix::mat<float>, float, H5::FloatType, H5::PredType::NATIVE_FLOAT);
    EIGENTYPE(matrix::mat<double>, double, H5::FloatType, H5::PredType::NATIVE_DOUBLE);
  #undef EIGENTYPE

  #define EIGENTYPE(Type, ElemType, H5PredType, H5Type) \
          template<> \
          inline H5::PredType hdf5_type_traits<Type>::get_type(Type&) { return H5Type; } \
          template<> \
          inline size_t hdf5_type_traits<Type>::get_size(Type& val) { return val.data.size(); } \
          template<> \
          inline void* hdf5_type_traits<Type>::get_addr(Type& val) { return val.data.data(); } \
          template<> \
          inline const int hdf5_type_traits<Type>::get_dim(Type& val, int d) { if (d == 0) { return val.n_rows; } else if (d == 1) { return val.n_cols; } else { return val.n_slices; } } \
          template<> \
          inline const int hdf5_type_traits<Type>::get_rank(Type& val) { return 3; }
    EIGENTYPE(matrix::cube<int>, int, H5::IntType, H5::PredType::NATIVE_INT);
    EIGENTYPE(matrix::cube<float>, float, H5::FloatType, H5::PredType::NATIVE_FLOAT);
    EIGENTYPE(matrix::cube<double>, double, H5::FloatType, H5::PredType::NATIVE_DOUBLE);
  #undef EIGENTYPE
#endif


class IOClass
{
public:
  std::string fileName;
  inline void load(std::string& tmpFileName)
  {
    fileName = tmpFileName;
  }

  inline void create()
  {
    // Create file
    H5::H5File* file = new H5::H5File(fileName, H5F_ACC_TRUNC);

    // Delete pointer
    delete file;
  }

  /// Gets for HDF5 traits
  // Get type
  template <class T>
  inline H5::AtomType GetHDF5Datatype(T &val) { return scaffold::IO::hdf5_type_traits<T>::get_type(val); }
  // Get address of first element
  template <class T>
  inline void* GetHDF5Addr(T &val) { return scaffold::IO::hdf5_type_traits<T>::get_addr(val); }
  // Get size of whole object
  template <class T>
  inline size_t GetHDF5Size(T &val) { return scaffold::IO::hdf5_type_traits<T>::get_size(val); }
  // Get size of given dimension d
  template <class T>
  inline const int GetHDF5Dim(T &val, int d) { return scaffold::IO::hdf5_type_traits<T>::get_dim(val,d); }
  // Get rank of object
  template <class T>
  inline const int GetHDF5Rank(T &val) { return scaffold::IO::hdf5_type_traits<T>::get_rank(val); }

  // Read
  template<class T>
  inline void Read(const std::string& dataset_name, T& data)
  {
    // Open file
    H5::H5File* file = new H5::H5File(fileName, H5F_ACC_RDONLY);

    // Do read
    H5::DataSet* dataset = new H5::DataSet(file->openDataSet(dataset_name));
    H5::DataSpace dataspace = dataset->getSpace();
    H5::AtomType datatype = GetHDF5Datatype(data);

    //try {
      dataset->read(GetHDF5Addr(data), datatype, dataspace, dataspace);
    //}

    // Delete pointers
    delete dataset;
    delete file;
  }

  // Write
  template<class T>
  inline void Write(const std::string& dataset_name, T& data)
  {
    // Open file
    H5::H5File* file = new H5::H5File(fileName, H5F_ACC_RDWR);

    // Do write
    H5::AtomType datatype(GetHDF5Datatype(data));
    datatype.setOrder(H5T_ORDER_LE);
    hsize_t data_shape[GetHDF5Rank(data)];
    for (int i=0; i<GetHDF5Rank(data); ++i)
      data_shape[i] = GetHDF5Dim(data,i);
    H5::DataSpace dataspace(GetHDF5Rank(data), data_shape);
    H5::DataSet* dataset = new H5::DataSet(file->createDataSet(dataset_name, datatype, dataspace));
    dataset->write(GetHDF5Addr(data), datatype);

    // Delete pointers
    delete dataset;
    delete file;
  }

  // Rewrite
  template<class T>
  inline void Rewrite(const std::string& dataset_name, T& data)
  {
    // Open file
    H5::H5File* file = new H5::H5File(fileName, H5F_ACC_RDWR);

    // Do write
    H5::AtomType datatype(GetHDF5Datatype(data));
    datatype.setOrder(H5T_ORDER_LE);
    hsize_t data_shape[GetHDF5Rank(data)];
    for (int i=0; i<GetHDF5Rank(data); ++i)
      data_shape[i] = GetHDF5Dim(data,i);
    H5::DataSpace dataspace(GetHDF5Rank(data), data_shape);
    H5::DataSet* dataset = new H5::DataSet(file->openDataSet(dataset_name));
    dataset->write(GetHDF5Addr(data), datatype);

    // Delete pointers
    delete dataset;
    delete file;
  }

  // Create Group
  inline void CreateGroup(const std::string& group_name)
  {
    // Open file
    H5::H5File* file = new H5::H5File(fileName, H5F_ACC_RDWR);

    // Create group
    H5::Group group = file->createGroup(group_name);

    // Delete pointer
    delete file;
  }

  // Create extendable dataset
  template<class T>
  inline void CreateExtendableDataSet(const std::string& prefix, const std::string& dataset_name, T& data)
  {
    // Open file
    H5::H5File* file = new H5::H5File(fileName, H5F_ACC_RDWR);

    // Get data information
    int data_rank = GetHDF5Rank(data);
    hsize_t data_shape[GetHDF5Rank(data)];
    for (int i=0; i<GetHDF5Rank(data); ++i)
      data_shape[i] = GetHDF5Dim(data,i);
    H5::AtomType datatype = GetHDF5Datatype(data);

    // Create the data space with one unlimited dimension.
    int rank = data_rank + 1;
    hsize_t dims[rank];
    hsize_t maxdims[rank];
    dims[0] = 1;
    maxdims[0] = H5S_UNLIMITED;
    for (int i=1; i<rank; i++) {
      dims[i] = data_shape[i-1];
      maxdims[i] = data_shape[i-1];
    }
    H5::DataSpace mspace(rank, dims, maxdims);

    // Modify dataset creation properties, i.e. enable chunking.
    H5::DSetCreatPropList cparms;
    const hsize_t* chunk_dims = dims; // Default chunk size to 1 x shape(data)
    cparms.setChunk(rank, chunk_dims);

    // Set fill value for the dataset
    int fill_val = 0;
    cparms.setFillValue(datatype, &fill_val);

    // Create a new dataset within the file using cparms creation properties.
    std::string full_name = prefix + dataset_name;
    H5::DataSet* dataset = new H5::DataSet(file->createDataSet(full_name, datatype, mspace, cparms));

    // Extend the dataset.
    dataset->extend(dims);

    // Select a hyperslab.
    H5::DataSpace fspace = dataset->getSpace();
    hsize_t offset[rank];
    for (int i=0; i<rank; i++)
      offset[i] = 0;
    fspace.selectHyperslab(H5S_SELECT_SET, dims, offset);

    // Write original data into hyperslab
    dataset->write(GetHDF5Addr(data), datatype, mspace, fspace);

    // Delete pointers
    delete dataset;
    delete file;
  }

  // Extend dataset
  template<class T>
  inline void AppendDataSet(const std::string& prefix, const std::string& dataset_name, T& data)
  {
    // Open file
    H5::H5File* file = new H5::H5File(fileName, H5F_ACC_RDWR);

    // Get data information
    int data_rank = GetHDF5Rank(data);
    hsize_t data_shape[GetHDF5Rank(data)];
    for (int i=0; i<GetHDF5Rank(data); ++i)
      data_shape[i] = GetHDF5Dim(data,i);
    H5::AtomType datatype = GetHDF5Datatype(data);

    // Open data set
    std::string full_name = prefix + dataset_name;
    H5::DataSet* dataset = new H5::DataSet(file->openDataSet(full_name));

    // Get old dataspace properties
    H5::DataSpace fspace = dataset->getSpace();
    int rank = data_rank + 1;
    hsize_t dims_old[rank], maxdims[rank];
    fspace.getSimpleExtentDims(dims_old, maxdims);

    // Extend the dataset.
    hsize_t dims_new[rank];
    dims_new[0] = dims_old[0]+1;
    for (int i=1; i<rank; i++)
      dims_new[i] = data_shape[i-1];
    dataset->extend(dims_new);

    // Select a hyperslab.
    fspace = dataset->getSpace();
    hsize_t offset[rank], dims_orig[rank];
    offset[0] = dims_old[0];
    dims_orig[0] = 1;
    for (int i=1; i<rank; i++) {
      offset[i] = 0;
      dims_orig[i] = data_shape[i-1];
    }
    fspace.selectHyperslab(H5S_SELECT_SET, dims_orig, offset);

    // Define memory space
    H5::DataSpace mspace(rank, dims_orig);

    // Write the data to the hyperslab.
    dataset->write(GetHDF5Addr(data), datatype, mspace, fspace);

    // Delete pointers
    delete dataset;
    delete file;
  }

};

// Specialization for strings

// Read
template<>
inline void IOClass::Read(const std::string& dataset_name, std::string& data)
{
  // Open file
  H5::H5File* file = new H5::H5File(fileName, H5F_ACC_RDONLY);

  // Do read
  const int MAX_NAME_LENGTH = 1024;
  char t_data[MAX_NAME_LENGTH];
  H5::DataSet* dataset = new H5::DataSet(file->openDataSet(dataset_name));
  H5::DataSpace dataspace = dataset->getSpace();
  H5::DataType datatype = dataset->getDataType();
  dataset->read(t_data, datatype, dataspace, dataspace);
  data = std::string(t_data);

  // Delete pointers
  delete dataset;
  delete file;
}

// Write
template<>
inline void IOClass::Write(const std::string& dataset_name, std::string& data)
{
  // Open file
  H5::H5File* file = new H5::H5File(fileName, H5F_ACC_RDWR);

  // Do write
  const int MAX_NAME_LENGTH = 1024;
  char t_data[MAX_NAME_LENGTH];
  strcpy(t_data, data.c_str());
  H5::StrType datatype(H5::PredType::C_S1, MAX_NAME_LENGTH);
  int data_rank = 1;
  hsize_t data_shape[1];
  data_shape[0] = 1;
  H5::DataSpace dataspace(data_rank, data_shape);
  H5::DataSet* dataset = new H5::DataSet(file->createDataSet(dataset_name, datatype, dataspace));
  dataset->write(t_data, datatype);

  // Delete pointers
  delete dataset;
  delete file;
}

// Rewrite
template<>
inline void IOClass::Rewrite(const std::string& dataset_name, std::string& data)
{
  // Open file
  H5::H5File* file = new H5::H5File(fileName, H5F_ACC_RDWR);

  // Do write
  const int MAX_NAME_LENGTH = 1024;
  char t_data[MAX_NAME_LENGTH];
  strcpy(t_data, data.c_str());
  H5::StrType datatype(H5::PredType::C_S1, MAX_NAME_LENGTH);
  int data_rank = 1;
  hsize_t data_shape[1];
  data_shape[0] = 1;
  H5::DataSpace dataspace(data_rank, data_shape);
  H5::DataSet* dataset = new H5::DataSet(file->openDataSet(dataset_name));
  dataset->write(t_data, datatype);

  // Delete pointers
  delete dataset;
  delete file;
}

}}

#endif
