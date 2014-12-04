#ifndef SCAFFOLD_COMMUNICATION
#define SCAFFOLD_COMMUNICATION

#if USE_MPI
  #include <mpi.h>
#endif
#if USE_OPENMP
  #include <omp.h>
#endif
#include "MPI_Datatype.hpp"

namespace scaffold { namespace parallel {

namespace COMM {
  #ifdef USE_MPI // Parallel version
    inline void Init (int argc, char **argv)
    {
      MPI_Init(&argc, &argv);
      int proc;
      MPI_Comm_rank(MPI_COMM_WORLD, &proc);
    }

    inline void Finalize() { MPI_Finalize(); }
    inline void BarrierSync() { MPI_Barrier(MPI_COMM_WORLD); }

    // Return process number
    inline int WorldProc()
    {
      int proc;
      MPI_Comm_rank(MPI_COMM_WORLD, &proc);
      return (proc);
    }

  #else // Serial version
    inline void Init (int argc, char **argv) {}
    inline void Finalize () {}
    inline int WorldProc() {return (0);}
  #endif
}

class CommunicatorClass
{
public:

  CommunicatorClass()
  {
    SetWorld();
  }

#if USE_MPI // Parallel version

  MPI_Comm MPIComm;

  /// Sets this communicator to be that of all the processes (i.e. MPI_WORLD)
  void SetWorld()
  {
    MPIComm = MPI_COMM_WORLD;
  }

  int MyProc()
  {
    int rank;
    MPI_Comm_rank(MPIComm, &rank);
    return rank;
  }

  int NumProcs()
  {
    int nProcs;
    MPI_Comm_size(MPIComm, &nProcs);
    return nProcs;
  }

  std::string MyHost()
  {
    int len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(hostname, &len);
    return hostname;
  }

  void BarrierSync()
  {
    MPI_Barrier(MPIComm);
  }

  void Split(int color, CommunicatorClass &newComm)
  {
    MPI_Comm_split(MPIComm, color, 0, &(newComm.MPIComm));
  }

  void Subset(matrix::vec<int> &ranks, CommunicatorClass &newComm)
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
  template <typename T>
  int AllGatherCols(matrix::mat<T> &buff)
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

  // Send
  template<class T>
  inline int Send(int toProc, T &val)
  {
    return MPI_Send(mpi_type_traits<T>::get_addr(val), mpi_type_traits<T>::get_size(val), mpi_type_traits<T>::get_type(val), toProc, 0, MPIComm);
  }

  // Receive
  template<class T>
  inline int Receive(int fromProc, T &val)
  {
    return MPI_Recv(mpi_type_traits<T>::get_addr(val), mpi_type_traits<T>::get_size(val), mpi_type_traits<T>::get_type(val), fromProc, 0, MPIComm, MPI_STATUS_IGNORE);
  }

  // Sendrecv
  template<class T>
  inline int SendReceive (int fromProc, T &fromBuff, int toProc, T &toBuff)
  {
    return MPI_Sendrecv(mpi_type_traits<T>::get_addr(fromBuff), mpi_type_traits<T>::get_size(fromBuff), mpi_type_traits<T>::get_type(fromBuff), fromProc, 1, mpi_type_traits<T>::get_addr(toBuff), mpi_type_traits<T>::get_size(toBuff), mpi_type_traits<T>::get_type(toBuff), toProc, 1, MPIComm, MPI_STATUS_IGNORE);
  }

  // Broadcast
  template<class T>
  inline int Broadcast(int fromProc, T &val)
  {
    return MPI_Bcast(mpi_type_traits<T>::get_addr(val), mpi_type_traits<T>::get_size(val), mpi_type_traits<T>::get_type(val), fromProc, MPIComm);
  }

  // Reduce
  template<class T>
  inline int Reduce(int toProc, T &fromBuff, T &toBuff, MPI_Op Op)
  {
    return MPI_Reduce(mpi_type_traits<T>::get_addr(fromBuff), mpi_type_traits<T>::get_addr(toBuff), mpi_type_traits<T>::get_size(fromBuff), mpi_type_traits<T>::get_type(fromBuff), Op, toProc, MPIComm);
  }

  // AllReduce
  template<class T>
  inline int AllReduce(T &fromBuff, T &toBuff, MPI_Op Op)
  {
    return MPI_Allreduce(mpi_type_traits<T>::get_addr(fromBuff), mpi_type_traits<T>::get_addr(toBuff), mpi_type_traits<T>::get_size(fromBuff), mpi_type_traits<T>::get_type(fromBuff), Op, MPIComm);
  }

  // Sum
  template<class T>
  inline int Sum(int toProc, T &fromBuff, T &toBuff) { return Reduce(toProc,fromBuff,toBuff,MPI_SUM); }

  // AllSum
  template<class T>
  inline int AllSum(T &fromBuff, T &toBuff) { return AllReduce(fromBuff,toBuff,MPI_SUM); }

  // Product
  template<class T>
  inline int Product(int toProc, T &fromBuff, T &toBuff) { return Reduce(toProc,fromBuff,toBuff,MPI_PROD); }

  // AllProduct
  template<class T>
  inline int AllProduct(T &fromBuff, T &toBuff) { return AllReduce(fromBuff,toBuff,MPI_PROD); }

  // Gather
  template<class T>
  inline int Gather(int toProc, T &fromBuff, T &toBuff)
  {
    return MPI_Gather(mpi_type_traits<T>::get_addr(fromBuff), mpi_type_traits<T>::get_size(fromBuff), mpi_type_traits<T>::get_type(fromBuff), mpi_type_traits<T>::get_addr(toBuff), mpi_type_traits<T>::get_size(fromBuff), mpi_type_traits<T>::get_type(toBuff), toProc, MPIComm);
  }

  // Gatherv
  template<class T>
  inline int Gatherv(int toProc, T &fromBuff, T &toBuff, int* recvCounts, int* displacements)
  {
     return MPI_Gatherv(mpi_type_traits<T>::get_addr(fromBuff), mpi_type_traits<T>::get_size(fromBuff), mpi_type_traits<T>::get_type(fromBuff), mpi_type_traits<T>::get_addr(toBuff), recvCounts, displacements, mpi_type_traits<T>::get_type(toBuff), toProc, MPIComm);
  }

  // GatherCols
  template<class T>
  int GatherCols(int toProc, T &fromBuff, T &toBuff)
  {
    int nProcs = NumProcs();
    int myProc = MyProc();
#ifdef USE_ARMADILLO
    int rows = fromBuff.n_rows;
    int cols = fromBuff.n_cols;
#elif defined USE_EIGEN
    int rows = fromBuff.rows();
    int cols = fromBuff.cols();
#endif
    int displacements[nProcs];
    int recvCounts[nProcs];
    int currCol = 0;
    for (int proc=0; proc<nProcs; proc++) {
      int procCols = cols/nProcs + ((cols%nProcs)>proc);
      displacements[proc] = rows*currCol;
      recvCounts[proc] = rows*procCols;
      if (proc == myProc) {
        toBuff.set_size(rows,procCols);
      }
      currCol += procCols;
    }
    return Gather(toProc, fromBuff, toBuff, recvCounts, displacements);
  }

  // AllGather
  template<class T>
  inline int AllGather(T &fromBuff, T &toBuff)
  {
    return MPI_Gather(mpi_type_traits<T>::get_addr(fromBuff), mpi_type_traits<T>::get_size(fromBuff), mpi_type_traits<T>::get_type(fromBuff), mpi_type_traits<T>::get_addr(toBuff), mpi_type_traits<T>::get_size(fromBuff), mpi_type_traits<T>::get_type(toBuff), MPIComm);
  }

  // Scatter
  template<class T>
  inline int Scatter(int fromProc, T &fromBuff, T &toBuff)
  {
    return MPI_Scatter(mpi_type_traits<T>::get_addr(fromBuff), mpi_type_traits<T>::get_size(fromBuff), mpi_type_traits<T>::get_type(fromBuff), mpi_type_traits<T>::get_addr(toBuff), mpi_type_traits<T>::get_size(toBuff), mpi_type_traits<T>::get_type(toBuff), fromProc, MPIComm);
  }

  // Scatter
  template<class T>
  inline int Scatterv(int fromProc, T &fromBuff, T &toBuff, int* sendCounts, int* displacements)
  {
     return MPI_Scatterv(mpi_type_traits<T>::get_addr(fromBuff), sendCounts, displacements, mpi_type_traits<T>::get_type(fromBuff), mpi_type_traits<T>::get_addr(toBuff), mpi_type_traits<T>::get_size(toBuff), mpi_type_traits<T>::get_type(toBuff), fromProc, MPIComm);
  }

  // ScatterCols
  template<class T>
  int ScatterCols(int fromProc, T &fromBuff, T &toBuff)
  {
    int nProcs = NumProcs();
    int myProc = MyProc();
#ifdef USE_ARMADILLO
    int rows = fromBuff.n_rows;
    int cols = fromBuff.n_cols;
#elif defined USE_EIGEN
    int rows = fromBuff.rows();
    int cols = fromBuff.cols();
#endif
    int displacements[nProcs];
    int sendCounts[nProcs];
    int currCol = 0;
    for (int proc=0; proc<nProcs; proc++) {
      int procCols = cols/nProcs + ((cols%nProcs)>proc);
      displacements[proc] = rows*currCol;
      sendCounts[proc] = rows*procCols;
      if (proc == myProc) {
        toBuff.set_size(rows,procCols);
      }
      currCol += procCols;
    }
    return Scatterv(fromProc, fromBuff, toBuff, sendCounts, displacements);
  }

#else   // Serial version
  inline void SetWorld(){}
  inline int MyProc() {return 0;}
  inline int NumProcs() {return 1;}
  inline std::string MyHost() {return "0";}
  inline void BarrierSync() {}
  inline void Split(int color, CommunicatorClass &newComm) {}
  inline void Subset(matrix::vec<int> &ranks, CommunicatorClass &newComm)
  {
    if (ranks.size() != 1) {
      std::cerr << "Serial verion of code does not support nontrivial subsets. Exiting.\n";
      abort();
    }
  }

  template<class T>
  inline int Send(int toProc, T &val) {}
  template<class T>
  inline int Receive(int fromProc, T &val) {}
  template<class T>
  inline int SendReceive (int fromProc, T &fromBuff, int toProc, T &toBuff) {toBuff = fromBuff;}
  template<class T>
  inline int Broadcast(int fromProc, T &val) {}
  template<class T>
  inline int Sum(int toProc, T &fromBuff, T &toBuff) {toBuff = fromBuff;}
  template<class T>
  inline int AllSum(T &fromBuff, T &toBuff) {toBuff = fromBuff;}
  template<class T>
  inline int Product(int toProc, T &fromBuff, T &toBuff) {toBuff = fromBuff;}
  template<class T>
  inline int AllProduct(T &fromBuff, T &toBuff) {toBuff = fromBuff;}
  template<class T>
  inline int Gather(int toProc, T &fromBuff, T &toBuff) {toBuff = fromBuff;}
  template<class T>
  inline int Gatherv(int toProc, T &fromBuff, T &toBuff, int* recvCounts, int* displacements) {toBuff = fromBuff;}
  template<class T>
  inline int GatherCols(int toProc, T &fromBuff, T &toBuff) {toBuff = fromBuff;}
  template<class T>
  inline int AllGather(T &fromBuff, T &toBuff) {toBuff = fromBuff;}
  template<class T>
  inline int Scatter(int fromProc, T &fromBuff, T &toBuff) {toBuff = fromBuff;}
  template<class T>
  inline int Scatterv(int fromProc, T &fromBuff, T &toBuff, int* sendCounts, int* displacements) {toBuff = fromBuff;}
  template<class T>
  inline int ScatterCols(int fromProc, T &fromBuff, T &toBuff) {toBuff = fromBuff;}
  template<class T>
  inline int AllGatherCols (matrix::mat<T> &buff) {}
#endif
};

}}

#endif
