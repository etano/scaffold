FIND_PATH(MATRIX_INCLUDE_DIR armadillo $ENV{ARMADILLO_HOME}/include ${SCAFFOLD_INSTALL_DIR}/include)
FIND_LIBRARY(MATRIX_LIBS armadillo $ENV{ARMADILLO_HOME}/lib $ENV{ARMADILLO_HOME}/lib64 ${SCAFFOLD_INSTALL_DIR}/lib)
IF(MATRIX_INCLUDE_DIR AND MATRIX_LIBS)
    INCLUDE_DIRECTORIES(${MATRIX_INCLUDE_DIR})
    IF(BUILD_STATIC)
    ELSE()
        LINK_LIBRARIES(${MATRIX_LIBS})
        MESSAGE(STATUS "MATRIX_LIBS=${MATRIX_LIBS}")
    ENDIF()
    MESSAGE(STATUS "MATRIX_INCLUDE_DIR=${MATRIX_INCLUDE_DIR}")
ELSE()
    ExternalProject_Add(
      armadillo_project
      URL http://downloads.sourceforge.net/project/arma/armadillo-4.450.4.tar.gz
      SOURCE_DIR ${SCAFFOLD_DEPENDS_DIR}/armadillo
      CMAKE_COMMAND cmake . -DCMAKE_INSTALL_PREFIX=${SCAFFOLD_INSTALL_DIR} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_FC_COMPILER=${CMAKE_FC_COMPILER}
    )
    SET(MATRIX_LIBS "")
    MESSAGE(STATUS "ARMADILLO_HOME not found. Installing in SCAFFOLD_DEPENDS_DIR.")
ENDIF()
