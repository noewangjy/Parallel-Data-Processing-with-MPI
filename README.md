# Parallel Data Processing with MPI
In this repository, we aim to use MPI to realize parallel data processing. 
This repo includes 3 tasks implemented with `cpp`:
- Calculate Pi
- Count prime numbers
- Accelerate MatMul

Please check the `README` in each folder for more details.


## Installing MPI

### Automatically
For Ubuntu, you can find `mpich` [here](https://packages.ubuntu.com/focal/mpich).

```shell
$ sudo apt-get install mpich
$ sudo make install
```

For MacOS, you can simply install `mpich` with homebrew
```shell
$ brew install mpich
```

Please Note that in each `CMakeLists.txt` file, you need to change the `CMAKE_CXX_COMPILER` to your
`mpicxx` path. 

```cmake
set(CMAKE_CXX_COMPILER "/usr/local/bin/mpicxx") # MacOS
set(CMAKE_CXX_COMPILER "/usr/bin/mpicxx") # Ubuntu
```



### Manually

Or manually download and compile `mpich`

```shell
$ wget https://www.mpich.org/static/downloads/4.0/mpich-4.0.tar.gz
$ tar -xvf mpich-4.0.tar.gz
$ ./configure
$ make
```

If `mpich` is manually installed，we need to configure `MPI_ROOT` 

```shell
export MPI_ROOT=$BUILD_DIR
export PATH=$MPI_ROOT/bin:$PATH
export MANPATH=$MPI_ROOT/man:$MANPATH
```

`$BUILD_DIR` is the directory where you compile `mpich`



Please Note that in each `CMakeLists.txt` file, you need to change the `CMAKE_CXX_COMPILER` to your
`mpicxx` path.

```cmake
set(CMAKE_CXX_COMPILER "YOUR_BUILD_PATH/mpicxx")
```








## Author
NoeWang at Shanghai Jiao Tong University
