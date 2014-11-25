#ifndef SCAFFOLD_TYPES
#define SCAFFOLD_TYPES

#include <complex>

namespace scaffold {

#if PRECISION==double
  typedef double RealType;
  typedef std::complex<double> ComplexType;
#elif PRECISION==single
  typedef float RealType;
  typedef std::complex<float> ComplexType;
#endif
typedef int IntType;

} // namespace


#endif
