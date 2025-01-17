#include "acc_testsuite.h"
#ifndef T1
//T1:serial,loop,reduction,combined-constructs,V:2.6-2.7
int test1(){
    int err = 0;
    int multiplicitive_n = 128;
    srand(SEED);
    real_t * a = new real_t[multiplicitive_n];
    real_t * b = new real_t[multiplicitive_n];

    real_t multiplied_total = 1.0;

    for (int x = 0; x < multiplicitive_n; ++x){
        a[x] = rand() / (real_t) RAND_MAX;
        b[x] = rand() / (real_t) RAND_MAX;
    }

    #pragma acc data copyin(a[0:multiplicitive_n], b[0:multiplicitive_n]) copy(multiplied_total)
    {
        #pragma acc serial loop reduction (*:multiplied_total)
        for (int x = 0; x < multiplicitive_n; ++x){
            multiplied_total *= a[x] + b[x];
        }
    }

    for (int x = 0; x < multiplicitive_n; ++x){
        multiplied_total /= (a[x] + b[x]);
    }
    if (fabs(multiplied_total - 1) > PRECISION * (2 * multiplicitive_n - 1)){
        err = 1;
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
    if (failed != 0)
        failcode = failcode + (1 << 0);
    }
#endif
    return failcode;
}
