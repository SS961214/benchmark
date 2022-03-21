#include <vector>
#include <complex>
#include <random>

#include <iostream>
#include <iomanip>
#include <sys/time.h>
#ifdef NDEBUG
#  define debug_print(str)
#else
#  define debug_print(str) (std::cerr << str << std::endl)
#endif

extern "C" {
  void zheevd_(char const& JOBZ, char const& UPLO,
               int const& N, std::complex<double>* A, int const& LDA,
               double* W, std::complex<double>* WORK, int const& LWORK,
               double* RWORK, int const& LRWORK,
               int* IWORK, int const& LIWORK,
               int& INFO);
};
static inline double getETtime() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec +(double)tv.tv_usec *1e-6;
}

int main(int argc, char** argv) {
  if(argc != 2) std::exit(1);
  size_t const D = std::atoi(argv[1]); // Size of a matrix
  int const Dim = D;
  constexpr size_t repMax = 10; // number of matrices to diagonalize

  std::mt19937 mt;
  std::normal_distribution<> normalDist(0.0, 1.0/sqrt(2.0*D));
  std::vector<double> eigenValue(D, 0.0);
  std::vector< std::complex<double> > mat(D*D, 0.0);

  int info, lwork=-1, lrwork=-1, liwork=-1;
  std::vector< std::complex<double> > work(1);
  std::vector<double> rwork(1);
  std::vector<int> iwork(1);
  char Jobs = 'V', Uplo='L';

  double Tcstr=0, Tdiag=0, Ttemp=0;

  for(size_t rep = 0; rep < repMax; ++rep) {
    Ttemp = getETtime();
      for(size_t j = 0;j < D; ++j) {
        mat.at(j+D*j) = std::complex<double>(sqrt(2.0)*normalDist(mt), 0.0);
        for(size_t k = 0;k < j; ++k) {
          mat.at(j+D*k) = std::complex<double>(normalDist(mt), normalDist(mt));
          mat.at(k+D*j) = std::conj(mat[j+D*k]);
        }
      }
    Tcstr += getETtime() - Ttemp;
    debug_print("(rep=" << rep << ") Generated a matrix.");

    Ttemp = getETtime();
      lwork=-1, lrwork=-1, liwork=-1;
      debug_print("(rep=" << rep << ") zheevd_(Query)");
      zheevd_(Jobs, Uplo, Dim, &*mat.begin(), Dim, &*eigenValue.begin(), &*work.begin(), lwork, &*rwork.begin(), lrwork, &*iwork.begin(), liwork, info);
      lwork  = (int)std::real(work[0]);  work.resize(lwork);
      lrwork = (int)rwork[0];           rwork.resize(lrwork);
      liwork = (int)iwork[0];           iwork.resize(liwork);
      debug_print("(rep=" << rep << ") zheevd_(Diagonalization)");
      zheevd_(Jobs, Uplo, Dim, &*mat.begin(), Dim, &*eigenValue.begin(), &*work.begin(), lwork, &*rwork.begin(), lrwork, &*iwork.begin(), liwork, info);
    Tdiag += getETtime() - Ttemp;

    #ifndef NDEBUG
      debug_print("print eigenvalues.");
      for(size_t j = 0;j < eigenValue.size(); ++j) std::cout << eigenValue[j] << std::endl;
    #endif
  }

  std::cerr << std::setprecision(6)
            << "\tTcstr = " << Tcstr << " sec\n"
            << "\tTdiag = " << Tdiag << " sec\n";
  return info;
}
