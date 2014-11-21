# Link dependency libs and includes
INCLUDE(ExternalProject)
INCLUDE_DIRECTORIES(${SCAFFOLD_INSTALL_DIR}/include)
LINK_DIRECTORIES(${SCAFFOLD_INSTALL_DIR}/lib)

# Find optimize linear algebra routines
INCLUDE(${SCAFFOLD_DIR}/CMake/FindLA.cmake)

# Find HDF5
INCLUDE(${SCAFFOLD_DIR}/CMake/FindHDF5.cmake)

# Find Armadillo
INCLUDE(${SCAFFOLD_DIR}/CMake/FindArmadillo.cmake)

# Set scaffold SRCS
SET (SRCS ${SRCS}
  ${SCAFFOLD_DIR}/Communication/Communication.cpp
  ${SCAFFOLD_DIR}/IO/InputClass.cpp
  ${SCAFFOLD_DIR}/IO/xmlParser.cpp
  ${SCAFFOLD_DIR}/RNG/RNGClass.cpp)

# Set scaffold libraries
SET (LIBS ${LIBS} ${LA_LIBS} ${HDF5_LIBS} ${EINSPLINE_LIBS} ${ARMADILLO_LIBS})
