#ifndef IO_H
#define IO_H

#include "../config.h"
#include "IO_Datatype.h"

class IOClass
{
public:
  string fileName;
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
  inline H5::AtomType GetHDF5Datatype(T &val) { return IO::hdf5_type_traits<T>::get_type(val); }
  // Get address of first element
  template <class T>
  inline void* GetHDF5Addr(T &val) { return IO::hdf5_type_traits<T>::get_addr(val); }
  // Get size of whole object
  template <class T>
  inline size_t GetHDF5Size(T &val) { return IO::hdf5_type_traits<T>::get_size(val); }
  // Get constant pointer to shape array
  template <class T>
  inline const hsize_t* GetHDF5Shape(T &val) { return IO::hdf5_type_traits<T>::get_shape(val); }
  // Get rank of object
  template <class T>
  inline const int GetHDF5Rank(T &val) { return IO::hdf5_type_traits<T>::get_rank(val); }

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
    dataset->read(GetHDF5Addr(data), datatype, dataspace, dataspace);

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
    H5::DataSpace dataspace(GetHDF5Rank(data), GetHDF5Shape(data));
    H5::DataSet* dataset = new H5::DataSet(file->createDataSet(dataset_name, datatype, dataspace));
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
    const hsize_t* data_shape = GetHDF5Shape(data);
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
    const hsize_t* data_shape = GetHDF5Shape(data);
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
  H5::DataSet* dataset = new H5::DataSet(file->openDataSet(dataset_name));
  H5::DataSpace dataspace = dataset->getSpace();
  H5::DataType datatype = dataset->getDataType();
  dataset->read(data, datatype, dataspace, dataspace);

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
  H5::StrType datatype(0, H5T_VARIABLE);
  H5::DataSpace dataspace(GetHDF5Rank(data), GetHDF5Shape(data));
  H5::DataSet* dataset = new H5::DataSet(file->createDataSet(dataset_name, datatype, dataspace));
  dataset->write(GetHDF5Addr(data), datatype);

  // Delete pointers
  delete dataset;
  delete file;
}

#endif
