export N=2000000000

if [ ! -d "./build" ]; then
mkdir build
fi
cd build && cmake .. && make && cd ..


echo '============== Test of prime_sieve =============='
for i in $(seq 1 8)
do
  mpirun -n "$i" ./build/prime_sieve $N
done
echo '=================== End of test ==================='