FIND_PATH(MATRIX_INCLUDE_DIR eigen3 $ENV{EIGEN_HOME}/include ${SCAFFOLD_INSTALL_DIR}/include)
IF(MATRIX_INCLUDE_DIR)
    INCLUDE_DIRECTORIES(${MATRIX_INCLUDE_DIR})
    MESSAGE(STATUS "MATRIX_INCLUDE_DIR=${MATRIX_INCLUDE_DIR}")
ELSE()
    ExternalProject_Add(
      eigen_project
      URL http://bitbucket.org/eigen/eigen/get/3.2.2.tar.gz
      SOURCE_DIR ${SCAFFOLD_DEPENDS_DIR}/eigen
      CMAKE_COMMAND cmake . -DCMAKE_INSTALL_PREFIX=${SCAFFOLD_INSTALL_DIR}
    )
    MESSAGE(STATUS "EIGEN_HOME not found. Installing in SCAFFOLD_DEPENDS_DIR.")
ENDIF()
