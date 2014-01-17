#include "InputFile.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

// Loads settings structure from the specified XML file
void Input::load(const std::string &filename)
{
  // Create an empty property tree object
  using boost::property_tree::ptree;
  ptree pt;

  // Load the XML file into the property tree. If reading fails
  // (cannot open file, parse error), an exception is thrown.
  read_xml(filename, pt);

  // Inputs
  outputPrefix = pt.get<std::string>("Input.IO.OutputPrefix");
  procsPerGroup = pt.get<int>("Input.Parallel.ProcsPerGroup", 1);
  beta = pt.get<RealType>("Input.System.beta");
  tau = pt.get<RealType>("Input.System.tau");
  //N = pt.get<int>("Input.System.nStates");
  M = pt.get<int>("Input.System.nParticles");
  nInitWalkers = pt.get<int>("Input.Walkers.nInitial");
  maxWalkers = pt.get<int>("Input.Walkers.nMax");
  minWalkers = pt.get<int>("Input.Walkers.nMin");
  maxWeight = pt.get<RealType>("Input.Walkers.weightMax");
  minWeight = pt.get<RealType>("Input.Walkers.weightMin");
  popScale = pt.get<RealType>("Input.Algorithm.popRescaling");
  betaEquilibration = pt.get<RealType>("Input.Algorithm.betaEquilibration");
  nStepsEnergy = pt.get<int>("Input.Algorithm.nStepsEnergy");
  nStabalize = pt.get<int>("Input.Algorithm.nStabalize");
  nPopControl = pt.get<int>("Input.Algorithm.nPopulation");

  // 2D Hubbard Model
  t = pt.get<RealType>("Input.System.t");
  U = pt.get<RealType>("Input.System.U");
  L1 = pt.get<int>("Input.System.L1");
  L2 = pt.get<int>("Input.System.L2");
}


