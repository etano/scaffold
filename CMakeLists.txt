# Find Lapack/Blas/Atlas/MKL/XLmass/ESSL/...
SET(LA_LIBS "")
IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "ppc") # This covers GNU and IBM Compilers on BGQ
  # Find Lapack/BLAS BGQ
  SET(LAPACK_LIBRARY /usr/local/tools/lapack/lib/liblapack.a)
  SET(BLAS_LIBRARY /usr/local/tools/blas/lib/libblas.a)
  SET(ESSL_LIBRARY /usr/local/tools/essl/5.1/lib/libesslsmpbg.a)
  SET(XLMASS_LIBRARY /opt/ibmcmp/xlmass/bg/7.3/bglib64/libmass.a
                     /opt/ibmcmp/xlmass/bg/7.3/bglib64/libmassv.a)
  SET(FORTRAN_LIBRARY /opt/ibmcmp/xlf/bg/14.1/bglib64/libxlf90_r.a
                      /opt/ibmcmp/xlf/bg/14.1/bglib64/libxl.a
                      /opt/ibmcmp/xlf/bg/14.1/bglib64/libxlfmath.a
                      /opt/ibmcmp/xlf/bg/14.1/bglib64/libxlfpmt4.a
                      /opt/ibmcmp/xlf/bg/14.1/bglib64/libxlfpmt8.a
                      /opt/ibmcmp/xlf/bg/14.1/bglib64/libxlopt.a)
  SET(LA_LIBS ${LAPACK_LIBRARY} ${BLAS_LIBRARY} ${ESSL_LIBRARY}
    ${XLMASS_LIBRARY} ${FORTRAN_LIBRARY} -ldl)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(CMAKE_CXX_FLAGS "-framework Accelerate ${CMAKE_CXX_FLAGS}")
  SET(CMAKE_C_FLAGS "-framework Accelerate ${CMAKE_C_FLAGS}")
  MESSAGE(STATUS "Using Mac OS Accelerate framework.")
ELSE()
    # Find MKL
    FIND_PATH(MKL_INCLUDE_DIR mkl.h)
    FIND_PATH(MKL_LINK_DIR libmkl_core.a)
    SET(MKL_FOUND false)
    IF(MKL_INCLUDE_DIR)
      SET(MKL_FOUND true)
    ELSE()
      FIND_PATH(MKL_INCLUDE_DIR mkl.h ${MKL_HOME} ${MKL_HOME}/include $ENV{MKL_HOME} $ENV{MKL_HOME}/include)
      FIND_PATH(MKL_LINK_DIR libmkl_core.a ${MKL_HOME} ${MKL_HOME}/lib/intel64  $ENV{MKL_HOME} $ENV{MKL_HOME}/lib/intel64)
      IF(MKL_INCLUDE_DIR)
        SET(MKL_FOUND true)
      ENDIF()
    ENDIF()
    IF(MKL_FOUND)
      SET(MKL_LIBS mkl_intel_lp64 mkl_intel_thread mkl_core iomp5 pthread m mkl_scalapack_lp64 mkl_blacs_intelmpi_lp64)
      #SET(MKL_LIBS "-L$ENV{MKL_HOME};mkl_intel_lp64;mkl_sequential;mkl_core")
      LINK_DIRECTORIES(/opt/intel/lib/intel64)
      LINK_DIRECTORIES(${MKL_LINK_DIR})
      SET(LA_LIBS ${MKL_LIBS})
    ELSE()
      FIND_LIBRARY(LAPACK_LIBS lapack /usr/lib /usr/local/lib)
      FIND_LIBRARY(BLAS_LIBS blas /usr/lib /usr/local/lib)
      IF(LAPACK_LIBS AND BLAS_LIBS)
        SET(LA_LIBS ${LAPACK_LIBS} ${BLAS_LIBS})
        LINK_LIBRARIES(${LA_LIBS})
      ENDIF()
    ENDIF()
    MESSAGE(STATUS "LA_LIBS=${LA_LIBS}")
ENDIF()

# Find HDF5
INCLUDE(EXTERNALPROJECT)
FIND_PATH(HDF5_INCLUDE_DIR hdf5.h $ENV{HDF5_HOME}/include ${SCAFFOLD_INSTALL_DIR}/include)
FIND_LIBRARY(HDF5_LIB hdf5 $ENV{HDF5_HOME}/lib $ENV{HDF5_HOME}/lib64 ${SCAFFOLD_INSTALL_DIR}/lib)
FIND_LIBRARY(HDF5_CPP_LIB hdf5_cpp $ENV{HDF5_HOME}/lib $ENV{HDF5_HOME}/lib64 ${SCAFFOLD_INSTALL_DIR}/lib)
SET(HDF5_LIBS ${HDF5_LIB} ${HDF5_CPP_LIB})
IF(HDF5_INCLUDE_DIR AND HDF5_LIBS)
    INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIR})
    LINK_LIBRARIES(${HDF5_LIBS})
    MESSAGE(STATUS "HDF5_INCLUDE_DIR=${HDF5_INCLUDE_DIR}")
    MESSAGE(STATUS "HDF5_LIBS=${HDF5_LIBS}")
ELSE()
    ExternalProject_Add(
      hdf5_project
      URL http://www.hdfgroup.org/ftp/HDF5/current/src/hdf5-1.8.13.tar.gz
      SOURCE_DIR ${SCAFFOLD_DEPENDS_DIR}/hdf5
      CONFIGURE_COMMAND ${SCAFFOLD_DEPENDS_DIR}/hdf5/configure --prefix=${SCAFFOLD_INSTALL_DIR} --enable-cxx
      BUILD_COMMAND make
    )
    SET(HDF5_LIBS hdf5 hdf5_cpp)
    MESSAGE(STATUS "HDF5_HOME not found. Installing in SCAFFOLD_DEPENDS_DIR.")
ENDIF()

# Find Einspline
FIND_PATH(EINSPLINE_INCLUDE_DIR einspline $ENV{EINSPLINE_HOME}/include ${SCAFFOLD_INSTALL_DIR}/include)
FIND_LIBRARY(EINSPLINE_LIBS einspline $ENV{EINSPLINE_HOME}/lib $ENV{EINSPLINE_HOME}/lib64 ${SCAFFOLD_INSTALL_DIR}/lib)
IF(EINSPLINE_INCLUDE_DIR AND EINSPLINE_LIBS)
    INCLUDE_DIRECTORIES(${EINSPLINE_INCLUDE_DIR})
    LINK_LIBRARIES(${EINSPLINE_LIBS})
    MESSAGE(STATUS "EINSPLINE_INCLUDE_DIR=${EINSPLINE_INCLUDE_DIR}")
    MESSAGE(STATUS "EINSPLINE_LIBS=${EINSPLINE_LIBS}")
ELSE()
    ExternalProject_Add(
      meinspline_project
      URL https://github.com/etano/meinspline/archive/master.tar.gz
      SOURCE_DIR ${SCAFFOLD_DEPENDS_DIR}/meinspline
      CONFIGURE_COMMAND ${SCAFFOLD_DEPENDS_DIR}/meinspline/configure --prefix=${SCAFFOLD_INSTALL_DIR}
      BUILD_COMMAND make
    )
    SET(EINSPLINE_LIBS einspline)
    MESSAGE(STATUS "EINSPLINE_HOME not found. Installing in SCAFFOLD_DEPENDS_DIR.")
ENDIF()

# Find Armadillo
FIND_PATH(ARMADILLO_INCLUDE_DIR armadillo $ENV{ARMADILLO_HOME}/include ${SCAFFOLD_INSTALL_DIR}/include)
FIND_LIBRARY(ARMADILLO_LIBS armadillo $ENV{ARMADILLO_HOME}/lib $ENV{ARMADILLO_HOME}/lib64 ${SCAFFOLD_INSTALL_DIR}/lib)
IF(ARMADILLO_INCLUDE_DIR AND ARMADILLO_LIBS)
    INCLUDE_DIRECTORIES(${ARMADILLO_INCLUDE_DIR})
    LINK_LIBRARIES(${ARMADILLO_LIBS})
    MESSAGE(STATUS "ARMADILLO_INCLUDE_DIR=${ARMADILLO_INCLUDE_DIR}")
    MESSAGE(STATUS "ARMADILLO_LIBS=${ARMADILLO_LIBS}")
ELSE()
    ExternalProject_Add(
      armadillo_project
      URL http://sourceforge.net/projects/arma/files/armadillo-4.450.4.tar.gz
      SOURCE_DIR ${SCAFFOLD_DEPENDS_DIR}/armadillo
      CMAKE_COMMAND cmake . -DCMAKE_INSTALL_PREFIX=${SCAFFOLD_INSTALL_DIR}
    )
    SET(ARMADILLO_LIBS armadillo)
    MESSAGE(STATUS "ARMADILLO_HOME not found. Installing in SCAFFOLD_DEPENDS_DIR.")
ENDIF()

INCLUDE_DIRECTORIES(${SCAFFOLD_INSTALL_DIR}/include)
LINK_DIRECTORIES(${SCAFFOLD_INSTALL_DIR}/lib)

# Set scaffold SRCS
SET (SRCS ${SRCS}
  ${SCAFFOLD_DIR}/Communication/Communication.cpp
  ${SCAFFOLD_DIR}/IO/InputClass.cpp
  ${SCAFFOLD_DIR}/IO/xmlParser.cpp
  ${SCAFFOLD_DIR}/RNG/RNGClass.cpp)

# Set scaffold libraries
SET (LIBS ${LIBS} ${LA_LIBS} ${HDF5_LIBS} ${EINSPLINE_LIBS} ${ARMADILLO_LIBS})
