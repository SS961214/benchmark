cmake_minimum_required(VERSION 3.14)
project(BIC CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Eigen3 REQUIRED)

find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)
message(${LAPACK_LIBRARIES})
string(FIND "${LAPACK_LIBRARIES}" "libiomp" USE_libiomp)
if("${USE_libiomp}" EQUAL "-1")
	set(USE_libiomp FALSE)
	message("libiomp is not found.")
else()
	set(USE_libiomp TRUE)
	message("libiomp is found. Using libiomp.")
	include_directories(SYSTEM "$ENV{MKLROOT}/include")
	message("MKLROOT/include = $ENV{MKLROOT}/include")
endif()

# For OpenMP
find_package(OpenMP REQUIRED)
if(USE_libiomp)
	set_property(TARGET OpenMP::OpenMP_CXX
		PROPERTY INTERFACE_LINK_LIBRARIES "")
endif()


include_directories("${PROJECT_SOURCE_DIR}")
set(modules
	ComplexDiagonalization
	SelfAdjointDiagonalization
)

foreach(name IN LISTS modules)
	add_executable(${name} ${name}.cpp)
	target_compile_options(${name} PRIVATE -Wall -Wextra -Wfatal-errors)
	target_link_libraries(${name}
		Eigen3::Eigen
		OpenMP::OpenMP_CXX
		LAPACK::LAPACK
		BLAS::BLAS
	)
endforeach(name)