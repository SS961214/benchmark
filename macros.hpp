#pragma once

#if __has_include(<mkl.h>)
	#ifndef MKL
		#define MKL
	#endif
	#ifndef EIGEN_USE_MKL_ALL
		#define EIGEN_USE_MKL_ALL
	#endif
#else
	#if __has_include(<Accelerate/Accelerate.h>)
		#ifndef ACCELERATE
			#define ACCELERATE
		#endif
		#ifndef EIGEN_USE_BLAS
			#define EIGEN_USE_BLAS
		#endif
	#endif
#endif

#if defined(NDEBUG) || defined(__CUDA_ARCH__)
	#define DEBUG(arg)
#else
	#define DEBUG(arg) (arg)
#endif