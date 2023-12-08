#include "macros.hpp"
#include <complex>
#include <iostream>
#include <iomanip>
#include <random>
#include <sys/time.h>
#include <Eigen/Dense>

static inline double getETtime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + (double)tv.tv_usec * 1e-6;
}

using RealScalar = double;
using Scalar     = std::complex<RealScalar>;

int main(int argc, char** argv) {
	if(argc < 5) {
		std::cerr << "Usage: 0.(This) 1.(dimMin) 2.(dimMax) 3.(step) 4.(Nsample)\n";
		std::cerr << "argc = " << argc << std::endl;
		std::exit(EXIT_FAILURE);
	}
	std::cout << "# " << __FILE__ << std::endl;
	int const dimMin  = std::atoi(argv[1]);
	int const dimMax  = std::atoi(argv[2]);
	int const step    = std::atoi(argv[3]);
	int const Nsample = std::atoi(argv[4]);

	std::mt19937                         engine(0);
	std::normal_distribution<RealScalar> dist(0.0, 1.0);
	Eigen::MatrixXcd                     mat;

	for(auto dim = dimMin; dim <= dimMax; dim += step) {
		double          start, elapsed = 0.0;
		Eigen::VectorXd error(Nsample);
		for(auto i = 0; i < Nsample; ++i) {
			start = getETtime();
			mat   = Eigen::MatrixXcd::NullaryExpr(
                dim, dim, [&]() { return Scalar(dist(engine), dist(engine)); });
			Eigen::ComplexEigenSolver<decltype(mat)> solver(mat);
			elapsed += getETtime() - start;
			auto const& eigvecs = solver.eigenvectors();
			error(i) = (mat * eigvecs - eigvecs * solver.eigenvalues().asDiagonal()).norm();
		}
		elapsed /= double(Nsample);

		std::cout << std::setw(4) << dim << " " << std::scientific << elapsed << " " << error.mean()
		          << " " << error.maxCoeff() << std::endl;
	}

	return EXIT_SUCCESS;
}