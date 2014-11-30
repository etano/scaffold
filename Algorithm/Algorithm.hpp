#ifndef SCAFFOLD_ALGORITHM
#define SCAFFOLD_ALGORITHM

#include <vector>
#include <algorithm>
#include <map>

namespace scaffold { namespace algorithm {

  inline int factorial(const int x)
  {
    return (x == 1 ? x : x * factorial(x - 1));
  }
  
  inline int n_choose_k(const int n, const int k)
  {
    int n_factorial_over_n_minus_k_factorial = 1;
    for (int i=n; i>n-k; --i)
      n_factorial_over_n_minus_k_factorial *= i;
    return n_factorial_over_n_minus_k_factorial/factorial(k);
  }
  
  template <typename T>
  inline bool fequal(T a, T b, T tol)
  {
    return (fabs(a-b) < tol);
  }
  
  template <typename Iterator>
  inline bool next_combination(const Iterator first, Iterator k, const Iterator last)
  {
     /* Credits: Thomas Draper */
     if ((first == last) || (first == k) || (last == k))
        return false;
     Iterator itr1 = first;
     Iterator itr2 = last;
     ++itr1;
     if (last == itr1)
        return false;
     itr1 = last;
     --itr1;
     itr1 = k;
     --itr2;
     while (first != itr1)
     {
        if (*--itr1 < *itr2)
        {
           Iterator j = k;
           while (!(*itr1 < *j)) ++j;
           std::iter_swap(itr1,j);
           ++itr1;
           ++j;
           itr2 = k;
           std::rotate(itr1,j,last);
           while (last != j)
           {
              ++j;
              ++itr2;
           }
           std::rotate(k,itr2,last);
           return true;
        }
     }
     std::rotate(first,k,last);
     return false;
  }
  
  template <typename T>
  void genPerm(std::vector< std::vector<T> >& vs, std::vector<T> vals)
  {
    std::sort(vals.begin(), vals.end());
    do {
      vs.push_back(vals);
    } while(std::next_permutation(vals.begin(),vals.end()));
  
  }
  
  template <typename T>
  void genCombPermK(std::vector< std::vector<T> >& vs, std::vector<T> vals, int k, bool duplicates=false, bool permutations=false)
  {
    if (duplicates) {
      int n = vals.size();
      for (int i=0; i<k-1; i++)
        for (int j=0; j<n; j++)
          vals.push_back(vals[j]);
    }
    std::sort(vals.begin(), vals.end());
  
    do {
      std::vector<T> v;
      for (int i=0; i<k; ++i)
        v.push_back(vals[i]);
      std::sort(v.begin(), v.end());
      do {
        vs.push_back(v);
      } while(std::next_permutation(v.begin(),v.end()) && permutations);
    } while(next_combination(vals.begin(),vals.begin() + k,vals.end()));

  }

}}

#endif