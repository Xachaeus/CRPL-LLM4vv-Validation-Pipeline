#include "acc_testsuite.h"
#ifndef T1
//T1:kernels,data,data-region,V:1.0-2.7
int test1(){
    int err = 0;
    srand(SEED);
    real_t * a = new real_t[n];
    real_t * b = new real_t[n];
    real_t * c = new real_t[n];

    for (int x = 0; x < n; ++x){
        a[x] = rand() / (real_t)(RAND_MAX / 10);
        b[x] = rand() / (real_t)(RAND_MAX / 10);
        c[x] = 1;
    }
    #pragma acc parallel copyin(a[0:n], b[0:n])
    {
        #pragma acc kernels copy(c[0:n])
        {
            #pragma acc loop
            for (int x = 0; x < n; ++x){
                c[x] += a[x] + b[x];
            }
        }
    }

    for (int x = 0; x < n; ++x){
        if (fabs(c[x] - (1 + a[x] + b[x])) > PRECISION) {
            err += 1;
        }
    }

    return err;
}
#endif

#ifndef T2
//T2:kernels,data,data-region,devonly,reference-counting,V:2.5-2.7
int test2(){
    int err = 0;
    srand(SEED);
    int * devtest = (int *)malloc(sizeof(int));
    real_t * a = new real_t[n];
    real_t * b = new real_t[n];
    real_t * c = new real_t[n];
    devtest[0] = 1;
    #pragma acc enter data copyin(devtest[0:1])
    #pragma acc kernels present(devtest[0:1])
    {
      devtest[0] = 0;
    }

    if (devtest[0] == 1){
        for (int x = 0; x < n; ++x){
          a[x] = rand() / (real_t)(RAND_MAX / 10);
          b[x] = rand() / (real_t)(RAND_MAX / 10);
          c[x] = 1;
        }
        #pragma acc data copyin(a[0:n], b[0:n], c[0:n])
        {
            #pragma acc kernels copy(c[0:n])
            {
                #pragma acc loop
                for (int x = 0; x < n; ++x){
                    c[x] += a[x] + b[x];
                }
            }
        }

        for (int x = 0; x < n; ++x){
            if (fabs(c[x] - 1) > PRECISION && (a[x] + b[x]) > PRECISION) {
                err += 1;
            }
        }
    }

    return err;
}
#endif

#ifndef T3
//T3:kernels,data,data-region,devonly,reference-counting,V:2.5-2.7
int test3(){
    int err = 0;
    srand(SEED);
    int * devtest = (int *)malloc(sizeof(int));
    real_t * a = new real_t[n];
    real_t * b = new real_t[n];
    real_t * c = new real_t[n];
    devtest[0] = 1;
    #pragma acc enter data copyin(devtest[0:1])
    #pragma acc kernels present(devtest[0:1])
    {
      devtest[0] = 0;
    }

    if (devtest[0] == 1){
        for (int x = 0; x < n; ++ x){
            a[x] = rand() / (real_t)(RAND_MAX / 10);
            b[x] = rand() / (real_t)(RAND_MAX / 10);
            c[x] = 1;
        }

        #pragma acc data copyin(a[0:n], b[0:n]) copy(c[0:n])
        {
            for (int x = 0; x < n; ++x){
                c[x] = 0;
            }
            #pragma acc kernels copy(c[0:n])
            {
                #pragma acc loop
                for (int x = 0; x < n; ++x){
                    c[x] += a[x] + b[x];
                }
            }
            for (int x = 0; x < n; ++x){
                if (fabs(c[x]) > PRECISION) {
                    err += 1;
                }
            }
        }

        for (int x = 0; x < n; ++x){
            if (fabs(c[x] - (a[x] + b[x] + 1)) > PRECISION){
                err += 1;
            }
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
#ifndef T2
    failed = 0;
    for (int x = 0; x < NUM_TEST_CALLS; ++x){
        failed = failed + test2();
    }
    if (failed != 0){
        failcode = failcode + (1 << 1);
    }
#endif
#ifndef T3
    failed = 0;
    for (int x = 0; x < NUM_TEST_CALLS; ++x){
        failed = failed + test3();
    }
    if (failed != 0){
        failcode = failcode + (1 << 2);
    }
#endif
    return failcode;
}
