#include <iostream>

void upsample(float* dst, float* src, int m, int n, int a, int b) {
    int mm = m*a;
    int nn = n*b;
    for(int i=0; i<m; i++) {
        for (int j=0; j<n; j++) {
            int ii = i*a;
            int jj = j*b;
            for(int k=0; k<a; k++) {
                for(int l=0; l<b; l++) {
                    dst[(ii+k)*nn+(jj+l)] = src[i*n + j];
                }
            }
        }
    }
}

void upsample2(float* dst, float* src, int m, int n, int a, int b) {
    int mm = m*a;
    int nn = n*b;
    for(int i=0; i<mm; i++) {
        for (int j=0; j<nn; j++) {
            int x = i/a;
            int y = j/b;
            dst[i*nn + j] = src[x*n+y];
        }
    }
}

// from caffe
void upsample3(float* dst, float* src, int N, int C, int H, int W, int scale) {
    int dH = H*scale;
    int dW = W*scale;

    for(int n=0; n<N; n++) {
        for(int c=0; c<C; c++) {
            for(int h=0; h<dH; h++) {
                for(int w=0; w<dW; w++) {
                    int sh = h/scale;
                    int sw = w/scale;
                    dst[n*C*dH*dW + c*dH*dW + h*dW + w] = src[n*C*H*W + c*H*W + sh*W + sw];
                }
            }
        }
    }
}


void downsample(float* dst, float* src, int m, int n, int a, int b) {
    int idx = 0;
    for(int i=0; i<m; i+=a) {
        for (int j=0; j<n; j+=b) {
            dst[idx++] = src[i*n+j];
        }
    }

    return;
}

void downsample2(float* dst, float* src, int m, int n, int a, int b) {
    int mm = (m+a-1) / a;
    int nn = (n+b-1) / b;
    int idx = 0;
    for(int i=0; i<mm; i++) {
        for (int j=0; j<nn; j++) {
            dst[i*nn+j] = src[i*a*n+j*b];
        }
    }

    return;
}

void ave_pool(float** dst_data, int* h, int* w, float* src, int N, int C, int H, int W, int step, int K, int p) {
    int dH = (H+2*p-K)/step+1;
    int dW = (W+2*p-K)/step+1;

    *h = dH;
    *w = dW;
    *dst_data = new float[dH*dW];
    float *dst = *dst_data;

    for(int n=0; n<N; n++) {
        for(int c=0; c<C; c++) {
            
            for(int h=0; h<dH; h++) {
                for(int w=0; w<dW; w++) { 
                    int d_idx = n*C*dH*dW + c*dH*dW + h*dW + w;
                    std::cout << "for d_idx: " << d_idx << std::endl;
                    int h_start = h*step - p;
                    int w_start = w*step - p;
                    int h_end = std::min(h_start+K, H+p);
                    int w_end = std::min(w_start+K, W+p);
                    int pool_size = (h_end-h_start) * (w_end-w_start);
                    h_start = std::max(0, h_start);
                    w_start = std::max(0, w_start);
                    h_end = std::min(h_end, H);
                    w_end = std::min(w_end, W);
                    std::cout << "   h_start: " << h_start << std::endl;
                    std::cout << "   w_start: " << w_start << std::endl;
                    std::cout << "   h_end: " << h_end << std::endl;
                    std::cout << "   w_end: " << w_end << std::endl;
                    for(int u=h_start; u<h_end; u++) {
                        for(int v=w_start; v<w_end; v++) {
                            int s_idx = n*C*H*W + c*H*W + u*W + v;
                            dst[d_idx] += src[s_idx];
                        }
                    }
                    std::cout << "all: " << dst[d_idx] << std::endl;
                    dst[d_idx] /= pool_size;
                }
            }
        }
    }
}


int main() {
    int m=4, n=4;
    int a=1, b=3;


    float* src = new float[m*n];
    float count = 1;
    for(int i=0; i<m*n; i++) {
        src[i] = count++;
    }

    float* dst = new float[m*a*n*b];

    upsample2(dst, src, m, n, a, b);

    for(int i=0; i<m*a*n*b; i++) {
        std::cout << dst[i] << " ";
        if((i+1) % (n*b) == 0) {
            std::cout << std::endl;
        }
    }

    std::cout << std::endl;

    float* src2 = new float[m*n];
    downsample2(src2, dst, m*a, n*b, a, b);

    for(int i=0; i<m*n; i++) {
        std::cout << src2[i] << " ";
        if((i+1) % n == 0) {
            std::cout << std::endl;
        }
    }

    std::cout << std::endl;
    

    int dh=0, dw=0;
    float * ave_dst = nullptr;
    ave_pool(&ave_dst, &dh, &dw, src, 1, 1, m, n, 2 , 2 , 1);

    for(int i=0; i<dh*dw; i++) {
        std::cout << ave_dst[i] << " ";
        if((i+1) % dw == 0) {
            std::cout << std::endl;
        }
    }

    std::cout << std::endl;

    return 0;
}