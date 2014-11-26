#ifndef SCAFFOLD_RNG
#define SCAFFOLD_RNG

#include <iostream>
#include <random>
#include "../Matrix/Matrix.hpp"

namespace scaffold { namespace rand {

class RNG
{
private:
  std::mt19937 rng;  // make instance of random number generator
protected:
public:
  // Constructor
  RNG(int seed)
   : rng(seed), u_dist(0.,1.)
  {
    std::cout << "RNG seed: " << seed << "\n";
  }

  std::uniform_real_distribution<double> u_dist;
  std::normal_distribution<double> normal_dist;

  // Random Functions

  // Generate a random number between 0 and 1
  // return a uniform number in [0,1].
  inline double unifRand() { return u_dist(rng); }

  // Generate a random number in a real interval.
  // param a one end point of the interval
  // param b the other end of the interval
  // return a inform rand numberin [a,b].
  template <typename T>
  inline double unifRand(const T a, const T b) { return (b-a)*unifRand() + a; }

  // Generate a random integer between 1 and a given value.
  // param n the largest value return a uniform random value in [1,...,n]
  inline int unifRand (const int n) { return floor(unifRand()*n+1.); }

  // Generate a uniform random vector of length 1
  template <typename T>
  inline void unifRand(matrix::vec<T>& r)
  {
    for (unsigned int iD=0; iD<r.size(); iD++)
      r(iD) = unifRand(-1,1);
    r /= matrix::mag(r);
  }

  // Generate a uniform random vector of length l
  template <typename T>
  inline void unifRand(matrix::vec<T>& r, const T l)
  {
    unifRand(r);
    r *= l;
  }

  // Generate a normal distribution random number
  inline double normRand() { return normal_dist(rng); }

  // Generate a normal distribution random number of mean m and variance s
  template <typename T>
  inline double normRand(const T m, const T s) { return normal_dist(rng)*s + m; }

  // Generate a normal random vector of mean m and variance s
  template <typename T>
  inline void normRand(matrix::vec<T>& r, const T m, const T s)
  {
    normRand(r);
    r = r*s + m;
  }

  // Generate a normal random vector of length 1
  template <typename T>
  inline void normRand(matrix::vec<T>& r)
  {
    for (unsigned int iD=0; iD<r.size(); iD++)
      r(iD) = normRand();
  }

};

}} // namespace

#endif
