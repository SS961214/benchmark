#include "macros.hpp"
#include <complex>
#include <iostream>
#include <random>
#include <sys/time.h>
#include <Eigen/Dense>

static inline double getETtime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + (double)tv.tv_usec * 1e-6;
}

using RealScalar = double;
using Scalar = std::complex<RealScalar>;

int main(int argc, char** argv) {
	if(argc < 3) {
		std::cerr << "Usage: 0.(This) 1.(dim) 2.(Nsample)\n";
		std::cerr << "argc = " << argc << std::endl;
		std::exit(EXIT_FAILURE);
	}
	int const dim = std::atoi(argv[1]);
	int const Nsample = std::atoi(argv[1]);

	std::mt19937 engine(0);
	std::normal_distribution<RealScalar> dist(0.0, 1.0);
	Eigen::MatrixXcd mat;

	double start, elapsed = 0.0;
	for(auto i = 0;i < Nsample; ++i) {start = getETtime();
		mat = Eigen::MatrixXcd::NullaryExpr(dim, dim, [&]() { return Scalar(dist(engine), dist(engine)); });
		Eigen::ComplexEigenSolver<decltype(mat)> solver(mat);
		elapsed += getETtime() - start;
	}
	elapsed /= double(Nsample);

	std::cout << dim << " " << elapsed << std::endl;

	return EXIT_SUCCESS;
}