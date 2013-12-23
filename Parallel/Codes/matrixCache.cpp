#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>

// Uncomment one to test.

// #define SLOWEST
// #define SLOWER
// #define SLOW
// #define MEDIUM
// #define FAST
// #define FASTER

// base case
#if defined(SLOWEST)
    void multiply (float **A, float **B, float **out, int size) {
        for (int row=0;row<size;row++)
            for (int col=0;col<size;col++)
                for (int in=0;in<size;in++)
                    out[row][col] += A[row][in] * B[in][col];
    }
// Takes in 1-D arrays, same as before.
#elif defined(SLOWER)
    void multiply (float *A, float *B, float *out, int size) {
        for (int row=0;row<size;row++)
            for (int col=0;col<size;col++)
                for (int in=0;in<size;in++)
                    out[row * size + col] += A[row * size + in] * B[in * size + col];
    }
// Flips first and second loops
#elif defined(SLOW)
    void multiply (float *A, float *B, float *out, int size) {
        for (int col=0;col<size;col++)
            for (int row=0;row<size;row++) {
                float curr = 0;  // prevents from calculating position each time through
                for (int in=0;in<size;in++)
                    curr += A[row * size + in] * B[in *size + col];
                out[row * size + col] = curr;
            }
    }
#elif defined(MEDIUM)
    // Keeps it organized for future codes.
    float dotProduct(float *A, float *B, int size) {
        float curr = 0;
        
        for (int i=0;i<size;i++)
            curr += A[i] * B[i];
        
        return curr;
    }
    void multiply (float *A, float *B, float *out, int size) {
        float *temp = new float[size];
        
        for (int col=0;col<size;col++) {
            for (int i=0;i<size;i++)  // stores column into sequential array
                temp[i] = B[i * size + col];
            for (int row=0;row<size;row++)
                out[row * size + col] = dotProduct(&A[row], temp, size);  // uses function above for dot product.
        }
        
        delete[] temp;
    }
#elif defined(FAST)

#elif defined(FASTER)

#endif

int main (int argc, char *argv[]) {
    const int size = atoi(argv[1]);
    srand(time(NULL));
    
// 2-D matrices
#if defined(SLOWEST)
    float **mA = new float*[size];
    float **mB = new float*[size];
    float **mC = new float*[size];
    
    for (int i=0;i<size;i++) {
        mA[i] = new float[size];
        mB[i] = new float[size];
        mC[i] = new float[size]();
        for (int j=0;j<size;j++) {
            mA[i][j] = (float) rand() / RAND_MAX;
            mB[i][j] = (float) rand() / RAND_MAX;
        }
    }
// 1-D matrices
#else
    const int nxn = size * size;
    float *mA = new float[nxn];
    float *mB = new float[nxn];
    float *mC = new float[nxn]();
    for (int i=0;i<nxn;i++) {
        mA[i] = (float) rand() / RAND_MAX;
        mB[i] = (float) rand() / RAND_MAX;
    }
#endif
    
    // This stuff is just for timing.
    struct timeval tp;
    int rtn = gettimeofday(&tp, NULL);
    double start = (double) tp.tv_sec+(1.e-6)*tp.tv_usec;
    
    multiply(mA, mB, mC, size);
    
    rtn = gettimeofday(&tp, NULL);
    double stop = (double) tp.tv_sec+(1.e-6)*tp.tv_usec;
    
    std::cout << "Multiplied matrix A and B in " << stop - start << " seconds.\n";

#if defined(SLOWEST)
    for (int i=0;i<size;i++) {
        delete[] mA[i];
        delete[] mB[i];
        delete[] mC[i];
    }
#endif
    delete[] mA;
    delete[] mB;
    delete[] mC;
    return 0;
}