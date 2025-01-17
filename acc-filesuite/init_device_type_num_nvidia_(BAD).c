#include "acc_testsuite.h"
#ifndef T1
//T1:init,runtime,V:2.5-3.2
int test1(){
    int err = 0;
    srand(SEED);
    int device_num = acc_get_device_num(acc_get_device_type());

    #pragma acc routine device_type(nvidia) device_num(device_num)

    return err;
}
#endif

int main(){
    int failcode = 0;
    int failed;
#ifndef T1
    failed = 0;
    for (int x = 0; x < NUM_TEST_CALLS; ++x){
        failed += test1();
    }
    if (failed){
        failcode += (1 << 0);
    }
#endif
    return failcode;
}
