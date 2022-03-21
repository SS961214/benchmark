# LAPACK(zheevd)ベンチマーク

### system

* intel-linux:
    * Debian Linux 10.10 (buster) + kernel 4.19.0-17-amd64
    * Processor Name: Intel Xeon Platinum 9242
    * Processor Speed: 2.30GHz
    * Number of Processors:	1
    * Total Number of Cores:	96
    * Memory: 376 GB
* intel-mac:
    * MacBook Pro (13-inch, 2020, Four Thunderbolt 3 ports)
    * macOS Monterey 12.2.1
    * Processor Name: Quad-Core Intel Core i7
    * Processor Speed: 2.3 GHz
    * Number of Processors: 1
    * Total Number of Cores: 4
    * L2 Cache (per Core): 512 KB
    * L3 Cache: 8 MB
    * Hyper-Threading Technology: Enabled
    * Memory: 32 GB
* m1-mac:
    * MacBook Pro (13-inch, M1, 2020)
    * macOS Monterey 12.2.1
    * Chip: Apple M1
    * Total Number of Cores: 8 (4 performance and 4 efficiency)
    * Memory: 16 GB

### compiler
* intel: oneAPI 2021.3.0
* gcc: homebrew gcc 11.2
* clang: apple clang 12.0

### blas & lapack
* mkl: oneAPI 2021.3.0
* veclib: apple veclib
* openblas: homebrew openblas 0.3.20

### コンパイル方法
* intel + mkl
    * `icpc -O3 -DNDEBUG -o randomMatrix_hemm.out randomMatrix_hemm.cpp -mkl`
* gcc + veclib
    * `g++ -O3 -std=c++11 -DNDEBUG -o randomMatrix_hemm.out randomMatrix_hemm.cpp -llapack -lblas`
* clang + veclib
    * `clang++ -O3 -std=c++11 -DNDEBUG -o randomMatrix_hemm.out randomMatrix_hemm.cpp -llapack -lblas`
* gcc + openblas
    * `g++ -O3 -std=c++11 -DNDEBUG -o randomMatrix_hemm.out randomMatrix_hemm.cpp -I$HOMEBREW_PREFIX/opt/openblas/include -L$HOMEBREW_PREFIX/opt/openblas/lib -lopenblas`

### 行列の次元
* D=1000, 2000, 5000

### スレッド数の指定
* mkl:      環境変数 `OMP_NUM_THREADS`
* veclib:   環境変数 `VECLIB_MAXIMUM_THREADS`
* openblas: 環境変数 `OMP_NUM_THREADS`
