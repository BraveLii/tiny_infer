#include <iostream>

void nchw_to_nhwc(float* dst, float* src, int N, int C, int H, int W) {
    int p = 0;
    for(int n=0; n<N; n++) {
        for(int c=0; c<C; c++) {
            for(int h=0; h<H; h++) {
                for(int w=0; w<W; w++) {
                    int dst_idx = n*H*W*C + h*W*C + w*C + c;
                    int src_idx = n*C*H*W + c*H*W + h*W + w;
                    // std::cout << dst_idx << " " << src_idx << std::endl;
                    dst[dst_idx] = src[src_idx];
                }
            }
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
    nchw_to_nhwc(dst, src, N, C, H, W);

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