export DATA_PATH="data/"
export A=1000
export B=500
export C=500
export D=1000
export MATRIX_PATH_1=$DATA_PATH"matrix_1.csv"
export MATRIX_PATH_2=$DATA_PATH"matrix_2.csv"
export RESULT_PATH=$DATA_PATH"result.csv"
export ROOT_RANK=0



if [ ! -d "./build" ]; then
mkdir build
fi
cd build && cmake .. && make && cd ..


echo '============== Generate Data and Test Numpy ================='
python3 ./utils/generate_data.py -a $A -b $B -c $C -d $D -p $DATA_PATH
echo '=================== Test of MPI ==================='

for i in $(seq 1 5)
do
  mpirun -n "$i" ./build/MatMul $ROOT_RANK $MATRIX_PATH_1 $MATRIX_PATH_2 $RESULT_PATH
done


echo '=================== End of test ==================='