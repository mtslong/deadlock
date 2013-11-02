#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

void get_ebp(unsigned long *ebp)
{
	__asm__ __volatile__("mov %%ebp, %0 \r\n":"=m"(*ebp)
			     ::"memory");

}

/* Obtain a backtrace and print it to stdout. */
void print_trace(int a,int b,int c)
{
	void *array[10];
	size_t size;
	char **strings;
	size_t i;

        printf("a+b+c=%d=0x%x\n",a+b+c,a+b+c);
	size = backtrace(array, 10);
	strings = backtrace_symbols(array, size);

	printf("Obtained %zd stack frames.\n", size);

	for (i = 0; i < size; i++)
		printf("%s: %08X\n", strings[i],(unsigned int)array[i]);


	unsigned int *pt;
        pt=(unsigned int *)array[0];
	for (i = 0; i < 32; i++) {
                if ( i % 8 == 0 ){  
                        printf("\n0x %08X: ",(unsigned int)pt);
                };
                printf("%08X ",(int)*(pt++));
        };
        printf("\n");

	unsigned long ebp = 0;
	get_ebp(&ebp);
        pt=(unsigned int *)ebp;
        printf("ebp=%08X , addr=%08x\n",(int)*(pt),(unsigned int)pt);

        unsigned int *pv;
        pv=*(unsigned int *)ebp;
        printf("argv[0].address=%08x\n",(unsigned int)(pv)+8);

        pv=*(unsigned int *)ebp+8;  /* get the first argv */
        printf("argv[0].value=%08x\n",(unsigned int)(*pv));

        /*printf("argv[0]=%x,addr=%08x\n",(*pv)+8,(unsigned int)(ebp+8));*/
	for (i = 0; i < 64; i++) {
                if ( i % 8 == 0 ){  
                        printf("\n0x %08X: ",(unsigned int)pt);
                };
                printf("%08X ",(int)*(pt++));
        };
        printf("\n");

	free(strings);
}

void test(int a,int b)
{
        printf("a+b=%d\n",a+b);
        print_trace(0x11,0x22,0x33);
}

int main()
{
        test(11,22);
        return 0;
}
