#include <iostream>
#include "timer.h"
#include "memory.h"

void add(float* a, int m, float* b, int n) {
    for(int i=0; i<m; i++) {
        for(int j=0; j<n; j++) {
            a[i] = a[i] + b[j];
        }
    }
}

void add_reorder(float* a, int m, float* b, int n) {
    for(int j=0; j<n; j++) {
        for(int i=0; i<m; i++) {
            a[i] = a[i] + b[j];
        }
    }
}

void add_tile_a(float* a, int m, float* b, int n, int tile) {
    for(int oi=0; oi<m; oi+=tile) {
        for(int j=0; j<n; j++) {
            for(int ii=oi; ii<tile && ii<m; ii++) {
                a[ii] = a[ii] + b[j];
            }
        }
    }
}

void add_tile_b(float* a, int m, float* b, int n, int tile) {
    for(int oj=0; oj<n; oj+=tile) {
        for(int i=0; i<m; i++) {
            for(int ij=oj; ij<tile && ij<n; ij++) {
                a[i] = a[i] + b[ij];
            }
        }
    }
}

int main() {
    int m=32*1024, n=32*1024;
    int tile = 64;

    float* a = new float[m];
    float* b = new float[n];

    memset(a, 0x0, sizeof(float)*m);
    memset(b, 0x0, sizeof(float)*n);

    TIMER_INIT;

for(int i=0; i<10; i++) {
    TIMER_RESTART;
    add(a, m, b, n);
    TIMER_MS_LOG("naive");
}

    TIMER_RESTART;
    add_reorder(a, m, b, n);
    TIMER_MS_LOG("reorder");

    TIMER_RESTART;
    add_tile_b(a, m, b, n, tile);
    TIMER_MS_LOG("tile b ");

    TIMER_RESTART;
    add_tile_a(a, m, b, n, tile);
    TIMER_MS_LOG("tile a ");

    return 0;
}