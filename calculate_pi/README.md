# Calculate PI with MPI



We use the trapezoid method to calculate the following integral:

$$ \pi =\int_0^1\frac{4}{(1+x^2)} $$

which is the integral of 4/(1+x^2) for x from 0 to 1.

Calculate PI in 2 ways with MPI:
1. MPI_Reduce
2. MPI_SendRecv

## To run the code:
Automatically:
- Execute the script `run.sh`

Manually:
```shell
    $ cmake . & make
    $ mpirun -n 4 hello_word
    $ mpirun -n 4 calculate_pi
```

## Configuration
In `calculate_pi.cpp`, you can choose the way to calculate PI with Macros:
- `USE_MPI_REDUCE`
- `USE_MPI_SEND_RECV`

## Author 
NoeWang at Shanghai Jiao Tong University
