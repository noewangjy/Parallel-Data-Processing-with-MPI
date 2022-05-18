#
# Created by 王金元 on 2022/5/16.
# This code is to generate test data with numpy and test dot product with numpy
#


import numpy as np
import argparse
import os
import time

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--matrix_1_rows", '-a', type=int, default=1000, required=False)
    parser.add_argument("--matrix_1_cols", '-b', type=int, default=1000, required=False)
    parser.add_argument("--matrix_2_rows", '-c', type=int, default=1000, required=False)
    parser.add_argument("--matrix_2_cols", '-d', type=int, default=1000, required=False)
    parser.add_argument("--data_path", '-p', type=str, default="../data", required=False)
    args = parser.parse_args()

    if args.matrix_1_cols != args.matrix_2_rows:
        raise ValueError("[Error] Matrix_1.cols != Matrix_2.rows")

    data_path = args.data_path
    matrix_1 = np.random.randn(args.matrix_1_rows, args.matrix_1_cols)
    matrix_2 = np.random.randn(args.matrix_2_rows, args.matrix_2_cols)
    t_begin = time.time()
    dot_result = np.dot(matrix_1, matrix_2)
    t_end = time.time()
    print(f"[Numpy] Total time: {t_end - t_begin}s")
    if not os.path.exists(data_path):
        os.makedirs(data_path)
    os.system("rm " + os.path.join(data_path, "*"))
    np.savetxt(os.path.join(data_path, "matrix_1.csv"), matrix_1, delimiter=",", fmt="%.6f")
    np.savetxt(os.path.join(data_path, "matrix_2.csv"), matrix_2, delimiter=",", fmt="%.6f")
    np.savetxt(os.path.join(data_path, "reference.csv"), dot_result, delimiter=",", fmt="%.6f")
