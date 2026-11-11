/* 华为测试long型表示程序 */ 
#include <stdio.h>
#include <limits.h>
int main(){
    long test_long =  (long)0x7ffffffffffffff; //7个半=8个字节 ; PC机中long 是4个字节 ；鲲鹏芯片64位的long是8个字节 
    printf("test_long low  = 0x%x\n", test_long);  //%x是4个字节，输出低4个字节 
    printf("test_long high = 0x%x\n", test_long>>32); //右移4个字节，输出高4个字节0x7ffffff
    printf("test_long long = %ld\n", test_long);  // 按长整型输出 
    long test_bb;
    test_bb = (long) (test_long *(double) 10);  //测试上面的test_long与double型10 浮点乘运算后，转为long型的结果。 
    printf("\n");
    printf("%lf",test_bb);
    printf("\n");
    printf("%ld", LONG_MAX);  // 输出可表示的 LONG_MAX 最大长整数 
    printf("\n");
    printf("%ld", LONG_MIN);  // 输出可表示的 LONG_MIN 最小长整数 
    printf("\n");
printf("sizeof long = %ld\n", sizeof(long));  //输出可表示的long型的字节数
return 0;
} 

