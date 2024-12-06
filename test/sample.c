#include <stdint.h>

#include "hthread_device.h"
#include <compiler/m3000.h>

#define SEGMENT_SIZE 3840 // (2048 * 8) / 1024 = 16KB

static inline void daxpy_single(uint64_t n, double a, double *x, double *y)
{
    int b = 2;
    for (uint64_t i = 0; i < n; ++i) {
      int tmp = a * x[i];
      y[i + 1] = tmp + b * y[i - 1];
    }
}

__global__ void daxpy_kernel(uint64_t n, double a, double *x, double *y)
{
    int threadId = get_thread_id();
    int threadsNum = get_group_size();
    if (threadId >= threadsNum)
        return;
    uint64_t n_p = n / threadsNum;
    uint64_t extras = n % threadsNum;
    uint64_t offset;
    if (threadId < extras) {
        n_p++;
        offset = threadId * n_p;
    } else {
        offset = threadId * (n_p + 1) - (threadId - extras);
    }

    int len = SEGMENT_SIZE;
    for (int i = 0; i < n_p; i += len) {
        len = (n_p - i) >= SEGMENT_SIZE ? SEGMENT_SIZE : (n_p - i);
        // daxpy_single(len, a, x + offset + i, y + offset + i);
        daxpy_single(len, a, x + offset + i, y + offset + i);
    }
}
