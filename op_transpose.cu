#include <cuda.h>
#include <cuda_runtime.h>
#include <iostream>


__global__ void nchw_to_nhwc(float* dst, float* src, int num, int N, int C, int H, int W) {
    int num_td = gridDim.x * blockDim.x;
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    for(int i=idx; idx<num; idx+=num_td) {
        int n = i/(C*H*W);
        int c = i/(H*W)%C;
        int h = i%(H*W)/W;
        int w = i%W;

        dst[n*H*W*C + h*W*C + w*C + c] = src[i];
    }

    return;
}

__global__ void nchw_to_nhwc_naive(float* dst, float* src, int num, int N, int C, int H, int W) {
    int n = blockIdx.x;
    int c = blockIdx.y;
    int h = threadIdx.x;
    int w = threadIdx.y;

    int src_idx = n*H*W*C + c*H*W + h*W + w;
    if(src_idx < num) {
        dst[n*H*W*C + h*W*C + w*C + c] = src[src_idx];
    }

    return;
}

void print_matrix(float* data, int N, int C, int H, int W) {
    int len = N*C*H*W;
    for(int i=0; i<len; i++) {
        std::cout << data[i] << " ";

        // std::cout << "aa : " << (i+1)  << std::endl;
        if((i+1) % W == 0) {
            std::cout << std::endl;
        }

        if((i+1) % (H*W) == 0) {
            std::cout << std::endl;
        }

        if((i+1) % (C*H*W) == 0) {
            std::cout << "-------------" << std::endl;
        }
    }

    return;
}

int main() {
    int N = 1;
    int C = 2;
    int H = 3;
    int W = 3;

    int len = N*C*H*W;
    float *src = new float[len];

    for(int i=0; i<len; i++) {
        src[i] = i+1;
    }

    for(int i=0; i<len; i++) {
        std::cout << src[i] << " ";
    }
    std::cout << std::endl;

    float *dst = new float[len];

    float *d_src = nullptr;
    float *d_dst = nullptr;

    cudaMalloc((void**)&d_src, len*sizeof(float));
    cudaMalloc((void**)&d_dst, len*sizeof(float));
    cudaMemcpy(d_src, src, len*sizeof(float), cudaMemcpyHostToDevice);
    // int td_num = 512;
    // int block_num = (len+512-1)/512;

    // nchw_to_nhwc<<<block_num, td_num>>>(d_dst, d_src, len, N, C, H, W);

    dim3 grid(N,C);
    dim3 block(H,W);
    nchw_to_nhwc_naive<<<grid, block>>>(d_dst, d_src, len, N, C, H, W);

    cudaMemcpy(dst, d_dst, len*sizeof(float), cudaMemcpyDeviceToHost);
    

    for(int i=0; i<len; i++) {
        std::cout << dst[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "src ====>" << std::endl;
    print_matrix(src, N, C, H, W);
    std::cout << "dst ====>" << std::endl;
    print_matrix(dst, N, H, W, C);

    return 0;
}