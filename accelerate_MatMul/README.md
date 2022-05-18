# Accelerating MatMul with MPI

This project aims to accelerate MatMul with MPI. 

## Idea
If the `world_size = 1`, then the only process will directly calculate the result.

If the `world_size > 1`, then we adopt a `allocator-calculator` design, 
where we use 1 process as `allocator` to allocate data and gather results, and other processes as `calculator`. 
We split the second matrix's columns into slices and multiply the first matrix with each slice in a dedicated process,
which is called `calculator_process`. 

In `allocator_process`, we broadcast the first matrix and meta information through broadcasting with `MPI_Bcast()`
and collect the results through asynchronous receive `MPI_Irecv()`. 

In `calculator_process`, we receive the first matrix through broadcasting and send the product result
of matrix 1 and matrix 2 slice to the `allocator_process` through `MPI_Send()`

In this project, we use the matrix library [Eigen](https://eigen.tuxfamily.org/dox/index.html), 
which is already included in the directory `./include/Eigen`.

## To run the code
### Automatically:
- Execute the script `run.sh`
```shell
$ bash run.sh
```
### Manually:
- Build
```shell
$ cmake . & make
```
- Generate test data
```shell
$ python3 ./utils/generate_data.py -a $A -b $B -c $C -d $D -p $DATA_PATH
```
- Run with `mpirun`
```shell
$ mpirun -n "$i" ./build/main $ROOT_RANK $MATRIX_PATH_1 $MATRIX_PATH_2 $RESULT_PATH
```



## Author
NoeWang at Shanghai Jiao Tong University