#ifndef RNG_H
#define RNG_H

#include "../config.h"
#include <random>

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

  std::uniform_real_distribution<RealType> u_dist;
  std::uniform_int_distribution<IntType> uint_dist;
  std::normal_distribution<RealType> normal_dist;

  // Random Functions
  RealType unifRand();
  RealType unifRand(const RealType a, const RealType b);
  long unifRand(const long n);
  void unifRand(Tvector& r);
  void unifRand(Tvector& r, const RealType l);
  RealType normRand(const RealType m, const RealType s);
  void normRand(Tvector& r, const RealType m, const RealType s);
};

// Generate a random number between 0 and 1
// return a uniform number in [0,1].
inline RealType RNG::unifRand()
{
  return u_dist(rng);
}

// Generate a random number in a real interval.
// param a one end point of the interval
// param b the other end of the interval
// return a inform rand numberin [a,b].
inline RealType RNG::unifRand(const RealType a, const RealType b)
{
  return (b-a)*unifRand() + a;
}

// Generate a random integer between 1 and a given value.
// param n the largest value return a uniform random value in [1,...,n]
inline long RNG::unifRand (const long n)
{
  return floor(unifRand()*n+1.);
}

// Generate a uniform random vector of length 1
inline void RNG::unifRand(Tvector& r)
{
  for (unsigned int iD=0; iD<r.n_elem; iD++)
    r(iD) = unifRand(-1,1);
  r /= norm(r,2);
}

// Generate a uniform random vector of length l
inline void RNG::unifRand(Tvector& r, const RealType l)
{
  unifRand(r);
  r *= l;
}

// Generate a normal distribution random number
inline RealType RNG::normRand(const RealType m, const RealType s)
{
  /* mean m, standard deviation s */
  return normal_dist(rng)*s + m;
}

// Generate a normal random vector of length 1
inline void RNG::normRand(Tvector& r, const RealType m, const RealType s)
{
  for (unsigned int iD=0; iD<r.n_elem; iD++)
    r(iD) = normRand(m,s);
}

#endif
