#include <iostream>
#include <array>
#include "timer.h"

// void nhwc_to_nchw_naive(float* dst, float* src, int H, int W) {
//     int p = 0;
//     for(int h=0; h<H; h++) {
//         for(int w=0; w<W; w++) {
//             float* p_dst = dst+ h*W+w;
//             p_dst[0] = src[p++];
//             p_dst += H*W;
//             p_dst[0] = src[p++];
//             p_dst += H*W;
//             p_dst[0] = src[p++];
//         }
//     }
// }


void nhwc_to_nchw_naive(float* dst, float* src, int H, int W) {
    uint32_t p = 0;
    int n_per_chan = H*W;
    auto pc2 = dst + H*W;
    auto pc3 = pc2 + H*W;

    int index = 0;
    int base = 0;
    for(int h=0; h<H; h++) {
        for(int w=0; w<W; w++) {
            dst[index] = src[p++];
            pc2[index] = src[p++];
            pc3[index] = src[p++];
            index++;
        }
    }
}

void nhwc_to_nchw_array(float* dst, float* src, int H, int W) {
    auto p = (std::array<float, 3>*)src;
    auto pc2 = dst + H*W;
    auto pc3 = pc2 + H*W;
    int index = 0;
    for(int h=0; h<H; h++) {
        for(int w=0; w<W; w++) {
            
            index = h*W + w;
            dst[index] = (*p)[0];
           
            pc2[index] = (*p)[1];
   
            pc3[index] = (*p)[2];
            p++;
        }
    }
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
    int C = 3;
    int H = 1024;
    int W = 1024;

    int len = N*C*H*W;
    float *src = new float[len];

    for(int i=0; i<len; i++) {
        src[i] = i+1;
    }

    // for(int i=0; i<len; i++) {
    //     std::cout << src[i] << " ";
    // }
    // std::cout << std::endl;


    float *dst = new float[len];
    TIMER_INIT;

    for(int i=0; i<5; i++) {
        TIMER_RESTART;
        nhwc_to_nchw_naive(dst, src, H, W);
        TIMER_MS_LOG("naive");
    }

    for(int i=0; i<5; i++) {
        TIMER_RESTART;
        nhwc_to_nchw_array(dst, src, H, W);
        TIMER_MS_LOG("array");
    }

    // for(int i=0; i<len; i++) {
    //     std::cout << dst[i] << " ";
    // }
    // std::cout << std::endl;

    // std::cout << "src ====>" << std::endl;
    // print_matrix(src, N, C, H, W);
    // std::cout << "dst ====>" << std::endl;
    // print_matrix(dst, N, H, W, C);

    return 0;
}