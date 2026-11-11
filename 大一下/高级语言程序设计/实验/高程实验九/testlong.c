#include <stdio.h>
#include <limits.h>
int main(){
    long test_long =  (long)0x7ffffffffffffff;
    printf("test_long low  = 0x%x\n", test_long);
    printf("test_long high = 0x%x\n", test_long>>32);
    printf("test_long long = %ld\n", test_long);
    long test_bb;
    test_bb = (long) (test_long *(double) 10);
    printf("\n");
    printf("%lf",test_bb);
    printf("\n");
    printf("%ld", LONG_MAX);
    printf("\n");
    printf("%ld", LONG_MIN);
    printf("\n");
printf("sizeof long = %ld\n", sizeof(long));
return 0;
} 

