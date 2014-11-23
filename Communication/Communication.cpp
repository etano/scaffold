#include "Communication.h"

#ifdef USE_MPI

#if PRECISION==double
MPI_Datatype MPI_RealType = MPI_DOUBLE;
#elif PRECISION==single
MPI_Datatype MPI_RealType = MPI_FLOAT;
#endif

/// Sets this communicator to be that of all the processes (i.e. MPI_WORLD)
void CommunicatorClass::SetWorld()
{
  MPIComm = MPI_COMM_WORLD;
}

int CommunicatorClass::MyProc()
{
  int rank;
  MPI_Comm_rank(MPIComm, &rank);
  return rank;
}

int CommunicatorClass::NumProcs()
{
  int nProcs;
  MPI_Comm_size(MPIComm, &nProcs);
  return nProcs;
}

std::string CommunicatorClass::MyHost()
{
  int len;
  char hostname[MPI_MAX_PROCESSOR_NAME];
  MPI_Get_processor_name(hostname, &len);
  return hostname;
}

void CommunicatorClass::BarrierSync()
{
  MPI_Barrier(MPIComm);
}

void CommunicatorClass::Split(int color, CommunicatorClass &newComm)
{
  MPI_Comm_split(MPIComm, color, 0, &(newComm.MPIComm));
}

void CommunicatorClass::Subset(vec<int> &ranks, CommunicatorClass &newComm)
{
  MPI_Group myGroup, newGroup;
  MPI_Comm_group (MPIComm, &myGroup);
#ifdef USE_ARMADILLO
  MPI_Group_incl(myGroup, ranks.size(), ranks.memptr(), &newGroup);
#endif
#ifdef USE_EIGEN
  MPI_Group_incl(myGroup, ranks.size(), ranks.data(), &newGroup);
#endif
  MPI_Comm_create(MPIComm, newGroup, &(newComm.MPIComm));
}

/** Gathers vectors from subprocesses and puts them in a matrix
 * @param toProc ID for receiving processor
 * @param fromBuff Reference to sending vector
 * @param toBuff Reference to receiving matrix
 * return the MPI status
 */
int CommunicatorClass::AllGatherCols(mat<RealType> &buff)
{
  int nProcs = NumProcs();
  int myProc = MyProc();
#ifdef USE_ARMADILLO
  int rows = buff.n_rows;
  int cols = buff.n_cols;
#endif
#ifdef USE_EIGEN
  int rows = buff.rows();
  int cols = buff.cols();
#endif
  int displacements[nProcs];
  int receiveCounts[nProcs];
  int sendCount;
  void *sendBuf, *receiveBuf;
  int currCol = 0;
  for (int proc=0; proc<nProcs; proc++) {
    int procCols = cols/nProcs + ((cols%nProcs)>proc);
    displacements[proc] = rows*currCol;
    receiveCounts[proc] = rows*procCols;
    if (proc == myProc) {
      sendBuf = &(buff(0,currCol));
      sendCount = rows*procCols;
    }
    currCol += procCols;
  }
#ifdef USE_ARMADILLO
  receiveBuf = buff.memptr();
#endif
#ifdef USE_EIGEN
  receiveBuf = buff.data();
#endif
  return MPI_Allgatherv(sendBuf, sendCount, MPI_DOUBLE, receiveBuf, receiveCounts, displacements, MPI_DOUBLE, MPIComm);
}

#endif






