#include <string.h>  
#include <execinfo.h>  
  
/* 获取ebp寄存器值 */  
void get_ebp(unsigned long *ebp)  
{  
        __asm__ __volatile__("mov %%ebp, %0 \r\n"  
                 :"=m"(*ebp)  
                 ::"memory");  
  
}  
  
int my_backtrace(void **stack, int size, unsigned long ebp)  
{  
        int layer = 0;  
    while(layer < size && ebp != 0 && *(unsigned long*)ebp != 0 && *(unsigned long *)ebp != ebp)  
    {  
            stack[layer++] = *(unsigned long *)(ebp+4);  
            ebp = *(unsigned long*)ebp;  
    }  
  
    return layer;  
}  
  
int func_3(int a, int b, int c)  
{  
       void *stack_addr[10];  
       int layer;  
       int i;  
       char **ppstack_funcs;  
  
       /* 通过调用libc函数实现 */  
       layer = backtrace(stack_addr, 10);  
       ppstack_funcs = backtrace_symbols(stack_addr, layer);  
       for(i = 0; i < layer; i++)  
             printf("\n%s:%p\n", ppstack_funcs[i], stack_addr[i]);  
  
       /* 自己实现 */  
       unsigned long ebp = 0;  
       get_ebp(&ebp);  
       memset(stack_addr, 0, sizeof(stack_addr));  
       layer = my_backtrace(stack_addr, 10, ebp);  
       for(i = 0; i < layer; i++)  
             printf("\nmy: %p\n", stack_addr[i]);  
  
     free(ppstack_funcs);  
     return 3;  
}  
  
int func_int(int a, int b, int c, int d)  
{  
        int aa,bb,cc;  
        int ret= func_3(aa,bb,cc);  
        return (a+ b+ c+ d + ret);  
}  
  
int func_str()  
{  
        int a = 1, b = 2;  
        int ret;  
  
        ret = func_int(a, a, b, b);  
  
        return ret;  
}  
  
int B(int c, int d)  
{  
        return c+d;  
}  
  
int A(int a, int b)  
{  
        int c = 0xff, d = 0xffff;  
        return B(c, d);  
}  
  
  
int main(int argc, char *argv[])  
{  
        int ret = func_str();  
        return 0;  
} 