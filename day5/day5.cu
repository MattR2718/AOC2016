#include <cuda_runtime.h>
#include <stdint.h>
#include "day5.cuh"

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ranges>
#include <algorithm>

#define MD5_BLOCK_SIZE 64
#define MD5_HASH_SIZE 16

// MD5 constants
__constant__ uint32_t k[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

__constant__ uint32_t r[64] = {
    7,12,17,22, 7,12,17,22, 7,12,17,22, 7,12,17,22,
    5,9,14,20, 5,9,14,20, 5,9,14,20, 5,9,14,20,
    4,11,16,23, 4,11,16,23, 4,11,16,23, 4,11,16,23,
    6,10,15,21, 6,10,15,21, 6,10,15,21, 6,10,15,21
};

__device__ __forceinline__ uint32_t leftrotate(uint32_t x, uint32_t c) {
    return (x << c) | (x >> (32 - c));
}

__device__ __forceinline__ uint32_t F(uint32_t x, uint32_t y, uint32_t z) { return (x & y) | (~x & z); }
__device__ __forceinline__ uint32_t G(uint32_t x, uint32_t y, uint32_t z) { return (x & z) | (y & ~z); }
__device__ __forceinline__ uint32_t H(uint32_t x, uint32_t y, uint32_t z) { return x ^ y ^ z; }
__device__ __forceinline__ uint32_t I(uint32_t x, uint32_t y, uint32_t z) { return y ^ (x | ~z); }

__device__ void lock(int *mutex) {
    while (atomicCAS(mutex, 0, 1) != 0) {
        // spin-wait
    }
}

__device__ void unlock(int *mutex) {
    atomicExch(mutex, 0);
}

__global__ void md5_kernel_variable_length(
    const uint8_t* base_str,
    const uint32_t base_str_len,
    const uint32_t start_index,
    uint8_t* __restrict__ output_hashes,
    size_t num_inputs,
    bool* valid_flags
) {

    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= num_inputs) return;

    uint32_t num = idx + start_index;
    uint32_t digits = 1;
    while (num >= 10) {
        num /= 10;
        digits++;
    }
    uint32_t len = base_str_len + digits;

    uint8_t str[32] = {0};
    for (uint32_t i = 0; i < base_str_len && i < 32; i++) {
        str[i] = base_str[i];
    }

    int temp = idx + start_index;
    for (int i = len - 1; i >= (int)base_str_len; i--) {
        str[i] = '0' + (temp % 10);
        temp /= 10;
    }

    // Prepare single 64-byte block with padding
    uint8_t block[MD5_BLOCK_SIZE];

    // Copy input string
    #pragma unroll
    for (int i = 0; i < 32; i++) {
        block[i] = (i < len) ? str[i] : 0;
    }
    // Append 0x80 byte after message
    if (len < 32) block[len] = 0x80;

    // Zero out remaining bytes until 56
    #pragma unroll
    for (int i = len + 1; i < 56; i++) {
        block[i] = 0;
    }

    // Append length in bits (little-endian) at block[56..63]
    uint64_t bit_len = (uint64_t)len * 8;
    block[56] = (uint8_t)(bit_len & 0xff);
    block[57] = (uint8_t)((bit_len >> 8) & 0xff);
    block[58] = (uint8_t)((bit_len >> 16) & 0xff);
    block[59] = (uint8_t)((bit_len >> 24) & 0xff);
    block[60] = (uint8_t)((bit_len >> 32) & 0xff);
    block[61] = (uint8_t)((bit_len >> 40) & 0xff);
    block[62] = (uint8_t)((bit_len >> 48) & 0xff);
    block[63] = (uint8_t)((bit_len >> 56) & 0xff);

    // Prepare 16 uint32 words from block (little endian)
    uint32_t M[16];
    #pragma unroll
    for (int i = 0; i < 16; i++) {
        M[i] = (uint32_t)block[i*4] |
              ((uint32_t)block[i*4 + 1] << 8) |
              ((uint32_t)block[i*4 + 2] << 16) |
              ((uint32_t)block[i*4 + 3] << 24);
    }

    // Initialize MD5 state
    uint32_t a0 = 0x67452301;
    uint32_t b0 = 0xefcdab89;
    uint32_t c0 = 0x98badcfe;
    uint32_t d0 = 0x10325476;

    uint32_t A = a0, B = b0, C = c0, D = d0;

    // MD5 main loop
    #pragma unroll
    for (int i = 0; i < 64; i++) {
        uint32_t F_val, g;
        if (i < 16) {
            F_val = F(B, C, D);
            g = i;
        } else if (i < 32) {
            F_val = G(B, C, D);
            g = (5 * i + 1) % 16;
        } else if (i < 48) {
            F_val = H(B, C, D);
            g = (3 * i + 5) % 16;
        } else {
            F_val = I(B, C, D);
            g = (7 * i) % 16;
        }

        uint32_t temp = D;
        D = C;
        C = B;
        B = B + leftrotate((A + F_val + k[i] + M[g]), r[i]);
        A = temp;
    }

    a0 += A;
    b0 += B;
    c0 += C;
    d0 += D;

    // Store hash output (little endian)
    uint8_t* out = output_hashes + idx * MD5_HASH_SIZE;
    #pragma unroll
    for (int i = 0; i < 4; i++) {
        out[i]      = (a0 >> (8 * i)) & 0xff;
        out[i + 4]  = (b0 >> (8 * i)) & 0xff;
        out[i + 8]  = (c0 >> (8 * i)) & 0xff;
        out[i + 12] = (d0 >> (8 * i)) & 0xff;
    }

    if(!(*out) && !(*(out + 1)) && *(out + 2) < (uint8_t)16){
        *(valid_flags + idx) = true;
    }

}

std::string md5_bytes_to_hex_string(const uint8_t* digest, size_t length = 16) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (size_t i = 0; i < length; ++i) {
        oss << std::setw(2) << static_cast<int>(digest[i]);
    }
    return oss.str();
}

void md5(const str& input, str& p1, str& p2) {
    constexpr int num_inputs = 30000000;
    int starting_index = 0;

    std::cout<<"MAPPING TO DEVICE MEMORY\n";

    // Map data into device memory
    uint8_t* d_input_str;
    uint8_t* d_output_hashes;
    bool* d_valid_flags;

    ANS p1_ans, p2_ans;

    
    cudaMalloc(&d_input_str, input.size);
    cudaMalloc(&d_output_hashes, num_inputs * MD5_HASH_SIZE);
    cudaMalloc(&d_valid_flags, num_inputs * sizeof(bool));

    cudaMemcpy(d_input_str, input.data, input.size, cudaMemcpyHostToDevice);
    cudaMemset(d_valid_flags, 0, num_inputs * sizeof(bool));

    std::cout<<"LAUNCHING KERNEL\n";

    // Launch kernel
    int blockSize = 256;
    int numBlocks = (num_inputs + blockSize - 1) / blockSize;
    md5_kernel_variable_length<<<numBlocks, blockSize>>>(
        d_input_str,
        input.size,
        starting_index,
        d_output_hashes,
        num_inputs,
        d_valid_flags
    );

    cudaDeviceSynchronize();

    std::cout<<"COPYING OUTPUT HASHES BACK TO HOST\n";

    // Copy output hashes back to host
    std::vector<uint8_t> output_hashes(num_inputs * MD5_HASH_SIZE);
    cudaMemcpy(output_hashes.data(), d_output_hashes, output_hashes.size(), cudaMemcpyDeviceToHost);

    std::vector<uint8_t> valid_flags(num_inputs);
    cudaMemcpy(valid_flags.data(), d_valid_flags, valid_flags.size(), cudaMemcpyDeviceToHost);

    std::cout<<"FREEING DEVICE MEMORY\n";

    // Free device memory
    cudaFree(d_output_hashes);
    cudaFree(d_input_str);
    cudaFree(d_valid_flags);

    std::cout<<"DEVICE MEMORY FREED\n";

    // Process output_hashes to produce final results
    std::string p1_str = md5_bytes_to_hex_string((uint8_t*)output_hashes.data());            // first hash
    std::string p2_str = md5_bytes_to_hex_string((uint8_t*)output_hashes.data() + 16);       // second hash


    //for(int i = 0; i < output_hashes.size(); i += 16){
        //if(!(*(output_hashes.data() + i)) && !(*(output_hashes.data() + i + 1)) && *(output_hashes.data() + i + 2) < (uint8_t)16){
        //    std::cout << "\tFound matching hash: " << md5_bytes_to_hex_string((uint8_t*)output_hashes.data() + i) << '\n';
        //}
    //}
     for(int i = 0; i < valid_flags.size(); i++){

         if(valid_flags[i]){
             std::cout << "\tFound matching hash: " << md5_bytes_to_hex_string((uint8_t*)output_hashes.data() + i * MD5_HASH_SIZE) << '\n';
         }

     }


    p1.init(p1_str.c_str(), p1_str.size());
    p2.init(p2_str.c_str(), p2_str.size());

}