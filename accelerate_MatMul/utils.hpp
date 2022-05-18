//
// Created by 王金元 on 2022/5/16.
// This code is for block decomposition macros
//

#include <iostream>
#include <fstream>
#include <vector>
//#include "utils.cpp"
#include <Eigen/Eigen>




#define SLICE_MSG 1
#define RESULT_MSG 2


#define BLOCK_LOW(rank, world_size, n) (((rank)-1) * (n) / ((world_size)-1))
#define BLOCK_HIGH(rank, world_size, n) (BLOCK_LOW((rank)+1, (world_size), (n)) - 1)
#define BLOCK_SIZE(rank, world_size, n) (BLOCK_HIGH((rank), (world_size), (n)) - BLOCK_LOW((rank), (world_size), (n)) + 1) //



typedef struct {
    int first_row_size;
    int first_col_size;
    int second_row_size;
    int second_col_size;

} MetaInfo;

int load_matrix_from_file(
        const std::string &path,
        Eigen::MatrixXd &matrix,
        int &row_size,
        int &col_size
) {
    std::ifstream file;
    std::string line;
    std::vector<std::vector<double>> matrix_data;

    // Open file
    file.open(path, std::ios::in);

    // Loop for lines
    while (std::getline(file, line)) {
        std::stringstream ss_line(line);
        std::vector<double> line_data;
        std::string string_number;
        // Loop for numbers
        while (std::getline(ss_line, string_number, ',')) {
            line_data.push_back(std::stod(string_number));
        }
        matrix_data.push_back(line_data);

        // Size check
        if (matrix_data.size() > 1) {
            if (matrix_data[matrix_data.size() - 1].size() != matrix_data[0].size()) {
                file.close();
                printf("Unmatched line size for data_vector, expected %ld, but got %ld\n", matrix_data[0].size(),
                       matrix_data[matrix_data.size() - 1].size());
                return 1;
            }
        }
    }

    // Record matrix size
    row_size = (int) matrix_data.size();
    col_size = (int) matrix_data[0].size();

    matrix.resize((long) row_size, (long) col_size);

    // Load data
    for (auto row = 0; row < row_size; row++) {
        for (auto col = 0; col < col_size; col++) {
            matrix(row, col) = matrix_data[row][col];
        }
    }

    // Close file and return
    file.close();
    return 0;
}


int dump_matrix_to_file(
        const std::string &path,
        Eigen::MatrixXd &matrix,
        int &row_size,
        int &col_size
) {
    std::ofstream file;
    file.open(path, std::ios::out);
    for (auto row = 0; row < row_size; row++) {
        file << matrix(row, 0);
        for (auto col = 1; col < col_size; col++) {
            file << "," << matrix(row, col);
        }
        file << "\n";
    }
    return 0;
}