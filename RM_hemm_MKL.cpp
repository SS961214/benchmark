#include <vector>
#include <complex>
#include <random>

# undef   MKL_Complex8
# undef   MKL_Complex16
# define  MKL_Complex8  std::complex<float>
# define  MKL_Complex16 std::complex<double>
#include <mkl.h>

#include <iostream>
#include <iomanip>
#include <sys/time.h>
#ifdef NDEBUG
#  define debug_print(str)
#else
#  define debug_print(str) (std::cerr << str << std::endl)
#endif

static inline double getETtime() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec +(double)tv.tv_usec *1e-6;
}

int main(int argc, char** argv) {
  if(argc<1) std::exit(1);
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
      zheevd(&Jobs, &Uplo, &Dim, &*mat.begin(), &Dim, &*eigenValue.begin(), &*work.begin(), &lwork, &*rwork.begin(), &lrwork, &*iwork.begin(), &liwork, &info);
      lwork  = (int)std::real(work[0]);  work.resize(lwork);
      lrwork = (int)rwork[0];           rwork.resize(lrwork);
      liwork = (int)iwork[0];           iwork.resize(liwork);
      debug_print("(rep=" << rep << ") zheevd_(Diagonalization)");
      zheevd(&Jobs, &Uplo, &Dim, &*mat.begin(), &Dim, &*eigenValue.begin(), &*work.begin(), &lwork, &*rwork.begin(), &lrwork, &*iwork.begin(), &liwork, &info);
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
