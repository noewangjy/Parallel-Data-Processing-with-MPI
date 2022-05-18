//
// Created by 王金元 on 2022/5/12.
// This code is for block decomposition macros
// We only consider odd numbers
//

#define BLOCK_LOW(rank, world_size, n) (3 + 2 * ((rank) * (n) / (world_size)))  // rank0 starts from 3
#define BLOCK_HIGH(rank, world_size, n) (BLOCK_LOW((rank)+1, (world_size), (n)) - 2) // -2 for previous odd number
#define BLOCK_SIZE(rank, world_size, n) ((BLOCK_HIGH((rank), (world_size), (n)) - BLOCK_LOW((rank), (world_size), (n)))/2 + 1) // all odd numbers
