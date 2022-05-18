# Find Prime Numbers with MPI

We use MPI to implement a prime sieve, which finds the number of primes smaller 
than a given positive integer `N`. Since all even numbers (except for 2) are not 
primes, we only consider the odd numbers.


## To run the code:
### Automatically:
- Execute the script `run.sh`
```shell
$ bash run.sh
```

### Manually:
```shell
$ cmake . & make
$ mpirun -n 4 prime_sieve $N
```

## Author
NoeWang at Shanghai Jiao Tong University
