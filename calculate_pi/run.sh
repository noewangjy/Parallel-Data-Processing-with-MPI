if [ ! -d "./build" ]; then
mkdir build
fi
cd build && cmake .. && make && cd ..

echo '============== Testing for hello_world =============='
mpirun -n 4 ./build/hello_world
echo '=================== End of test ==================='


echo '============== Test for calculate_pi =============='
for i in $(seq 1 8)
do
  mpirun -n "$i" ./build/calculate_pi
done
echo '=================== End of test ==================='