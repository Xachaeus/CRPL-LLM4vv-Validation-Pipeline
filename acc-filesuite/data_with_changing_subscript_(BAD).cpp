#include "acc_testsuite.h"
#ifndef T1
//T1:data,data-region,construct-independent,V:2.0-2.7
int test1(){
    int err = 0;
    srand(SEED);
    int length = n;
    real_t * a = new real_t[n];
    real_t * b = new real_t[n];
    real_t * c = new real_t[n];

    for (int x = 0; x < n; ++x){
        a[x] = rand() / (real_t)(RAND_MAX / 10);
        b[x] = rand() / (real_t)(RAND_MAX / 10);
        c[x] = 0.0;
    }

    #pragma acc parallel copyin(a[0:length], b[0:length]) copyout(c[0:length])
    {
        #pragma acc parallel
        {
            #pragma acc loop
            for (int x = 0; x < length; ++x){
                c[x] = a[x] + b[x];
            }
        }
        length = 1;
    }

    for (int x = 0; x < n; ++x){
        if (fabs(c[x] - (a[x] + b[x])) > PRECISION){
            err += 1;
        }
    }

    return err;
}
#endif

int main(){
    int failcode = 0;
    int failed;
#ifndef T1
    failed = 0;
    for (int x = 0; x < NUM_TEST_CALLS; ++x){
        failed = failed + test1();
    }
    if (failed != 0){
        failcode = failcode + (1 << 0);
    }
#endif
    return failcode;
}
