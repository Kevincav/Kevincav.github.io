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

#if defined(SLOWEST)
    void multiply (float **A, float **B, float **out, int size) {
        for (int row=0;row<size;row++)
            for (int col=0;col<size;col++)
                for (int in=0;in<size;in++)
                    out[row][col] += A[row][in] * B[in][col];
    }
#elif defined(SLOWER)
    void multiply (float *A, float *B, float *out, int size) {
        for (int row=0;row<size;row++)
            for (int col=0;col<size;col++)
                for (int in=0;in<size;in++)
                    out[row * size + col] += A[row * size + in] * B[in *size + col];
    }

#elif defined(SLOW)
    void multiply (float *A, float *B, float *out, int size) {
        for (int col=0;col<size;col++)
            for (int row=0;row<size;row++) {
                float curr = 0;
                for (int in=0;in<size;in++)
                    curr += A[row * size + in] * B[in *size + col];
                out[row * size + col] = curr;
            }
    }
#elif defined(MEDIUM)
    float dotProduct(float *A, float *B, int size) {
        float curr = 0;
        
        for (int i=0;i<size;i++)
            curr += A[i] * B[i];
        
        return curr;
    }
    void multiply (float *A, float *B, float *out, int size) {
        float *temp = new float[size];
        
        for (int col=0;col<size;col++) {
            for (int i=0;i<size;i++)
                temp[i] = B[i * size + col];
            for (int row=0;row<size;row++)
                out[row * size + col] = dotProduct(&A[row], temp, size);
        }
        
        delete[] temp;
    }
#elif defined(FAST)
    #include <xmmintrin.h>
    
    union vec {
        __m128 u;
        struct { float x, y, z, w; };
    };
    
    float dotProduct(float *A, float *B, int size) {
        vec v;
        v.u  = _mm_setzero_ps();
        const int end = size >> 2;

        int j = 0;
        for (int i=0;i<end;i++) {
            __m128 t1 = _mm_setr_ps(A[j], A[j+1], A[j+2], A[j+3]);
            __m128 t2 = _mm_setr_ps(B[j], B[j+1], B[j+2], B[j+3]);
            v.u = _mm_add_ps(_mm_add_ps(t1, t2), v.u);
            j += 4;
        }
        
        if (size - j != 0) {
            float outA[4] = {0};
            float outB[4] = {0};
            std::copy(A + j, A + size, outA);
            std::copy(B + j, B + size, outB);
            v.u = _mm_add_ps(_mm_add_ps(_mm_load_ps(outA), _mm_load_ps(outB)), v.u);
        }
        
        return v.x + v.y + v.z + v.w;
    }
    void multiply (float *A, float *B, float *out, int size) {
        float *temp = new float[size];
        
        for (int col=0;col<size;col++) {
            for (int i=0;i<size;i++)
                temp[i] = B[i * size + col];
            for (int row=0;row<size;row++)
                out[row * size + col] = dotProduct(&A[row], temp, size);
        }
        
        delete[] temp;
    }
#elif defined(FASTER)

#endif

int main (int argc, char *argv[]) {
    const int size = atoi(argv[1]);
    srand(time(NULL));
    
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