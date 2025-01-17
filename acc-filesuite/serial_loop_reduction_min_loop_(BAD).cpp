#include "acc_testsuite.h"
#ifndef T1
//T1:serial,loop,reduction,combined-constructs,V:2.6-3.2
int test1(){
    int err = 0;
    srand(SEED);
    real_t * a = new real_t[10 * n];
    real_t * b = new real_t[10 * n];
    real_t * c = new real_t[10 * n];
    real_t * minimum = new real_t[10];
    real_t temp = 0;

    for (int x = 0; x < 10 * n; ++x){
        a[x] = 1 + rand() / (real_t)(RAND_MAX/10);
        b[x] = 1 + rand() / (real_t)(RAND_MAX/10);
        c[x] = 0.0;
    }

    #pragma acc loop copyin(a[0:10*n], b[0:10*n]) copy(c[0:10*n], minimum[0:10])
    {
        #pragma acc serial 
	{    
	    
	#pragma acc loop gang private(temp)
        for (int x = 0; x < 10; ++x){
            temp = 1000;
            #pragma acc loop reduction(min:temp)
            for (int y = 0; y < n; ++y){
                temp = fmin(a[x * n + y] * b[x * n + y], temp);
            }
            minimum[x] = temp;
            #pragma acc loop
            for (int y = 0; y < n; ++y){
                c[x * n + y] = (a[x * n + y] * b[x * n + y]) / minimum[x];
            }
        }
	}
    }

    for (int x = 0; x < 10; ++x){
        temp = 1000;
        for (int y = 0; y < n; ++y){
            temp = fmin(temp, a[x * n + y] * b[x * n + y]);
        }
        if (fabs(temp - minimum[x]) > PRECISION){
            err += 1;
        }
        for (int y = 0; y < n; ++y){
            if (fabs(c[x * n + y] - (a[x * n + y] * b[x * n + y] / minimum[x])) > PRECISION){
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
    return failcode;
}
