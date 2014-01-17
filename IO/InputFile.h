#ifndef INPUTFILE_H
#define INPUTFILE_H

#include <string>
#include "../config.h"

class Input
{
public:
  std::string outputPrefix; // prefix for all generated output files
  int procsPerGroup; // # of processors per clone
  RealType beta, tau; // length of projection, timestep
  int N; // Number of basis states
  int M; // Number of electrons of either spin (a.k.a. Number of sites in Hubbard Model)
  int nInitWalkers, maxWalkers, minWalkers; // Initial, max, min number of walkers
  RealType maxWeight, minWeight; // Max and min possible weights
  RealType popScale; // Population rescaling factor
  RealType betaEquilibration; // Beta after which measurements begin
  int nStepsEnergy; // How often to measure the energy
  int nStabalize, nPopControl; // How often to stabalize wavefunction and perform population control
  void load(const std::string &filename);

  // Hubbard Model
  RealType t; // Hopping term
  RealType U; // On-site interaction
  int L1, L2; // Lattice dimensions
};

#endif
