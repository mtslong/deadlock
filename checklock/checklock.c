/*
 * trace the filter function.
 */

#define _GNU_SOURCE 
#include <dlfcn.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <string.h>
#include <execinfo.h>

#define gettid() syscall(SYS_gettid)
static FILE *fp; /* main */

/*typedef struct {
	char name[255];
	void *addr;
} function_enty;

extern void a1(void);
extern void a2(void);
extern void a3(void);
extern void a4(void);
extern void add(void);

#define FUN_SIZE  5	
function_enty filter[FUN_SIZE]={
        {"add",add},
        {"a1",a1},
        {"a2",a2},
        {"a3",a3},
        {"a4",a4},
};*/

#define FUN_SIZE  12	
char filter[FUN_SIZE][250]={
        {"main"},
        {"thread1"},
        {"thread2"},
        {"thread3"},
        {"locka"},
        {"lockb"},
        {"lockc"},
        {"unlocka"},
        {"unlockb"},
        {"unlockc"},
        {"test"},
};


/* Function prototypes with attributes */
void main_constructor(void)
    __attribute__ ((no_instrument_function, constructor));

void main_destructor(void)
    __attribute__ ((no_instrument_function, destructor));

void __cyg_profile_func_enter(void *, void *)
    __attribute__ ((no_instrument_function));

void __cyg_profile_func_exit(void *, void *)
    __attribute__ ((no_instrument_function));


/* declare for no_instrument_function */
/*void filter_call(void *pt,char type)
    __attribute__ ((no_instrument_function));*/

void record_fun(void *pt,char type)
    __attribute__ ((no_instrument_function));

void get_ebp(unsigned long *ebp)
    __attribute__ ((no_instrument_function));


void get_ebp(unsigned long *ebp)
{
	__asm__ __volatile__("mov %%ebp, %0 \r\n":"=m"(*ebp)
			     ::"memory");

}

void record_fun(void *pt,char type)
{
        Dl_info mydl_info;
        dladdr(pt, &mydl_info);
        printf("->%p\t%s\t%s\n",pt,mydl_info.dli_sname,mydl_info.dli_fname);

        int i=0;
        for(i=0;i<FUN_SIZE;i++ ) {
                if ( strcmp(filter[i],mydl_info.dli_sname) == 0) {
                        if (fp == NULL) exit(-1);
                        printf("%ld\t%c\t%p\t%s\n",gettid(),type,pt,mydl_info.dli_sname);
                        fprintf(fp, "%ld\t%c\t%p\t%s\n",gettid(),type,pt,mydl_info.dli_sname); 
                        fflush(fp);
                        break;
                };
        }
}

/*void filter_call(void *pt,char type)
{
        int i=0;
        for(i=0;i<FUN_SIZE;i++ ) {
                if ( filter[i].addr == pt) {
                        if (fp == NULL) exit(-1);
                        [>printf("%ld\t%c\t%p\t%s\n",gettid(),type, filter[i].addr, filter[i].name);<]
                        fprintf(fp, "%ld\t%c\t%p\t%s\n",gettid(),type, filter[i].addr, filter[i].name);
                        fflush(fp);
                        break;
                };
        }
}*/


void main_constructor(void)
{
        /*printf("** %s\tdebug:tid=%ld\n",__func__,gettid());*/
	char t_name[50];
	sprintf(t_name, "trace-%ld.txt", gettid());
	fp = fopen(t_name, "w");
	if (fp == NULL) exit(-1);

        /* print filter function address */
        printf("*** Record Function List:\n");
        int i=0;
        for ( i=0; i<FUN_SIZE; i++ ) {
                printf("%ld\t%s\n",gettid(),filter[i]);
        };
        printf("*** Record Start......\n");
}

void main_destructor(void)
{
        /*printf("** %s\tdebug:tid=%ld\n",__func__,gettid());*/
        printf("*** Record End.\n");
	fclose(fp);
}

/*void __cyg_profile_func_enter(void *this, void *callsite) { filter_call(this,'E'); };
void __cyg_profile_func_exit(void *this, void *callsite) { filter_call(this,'X'); };*/

void __cyg_profile_func_enter(void *this, void *callsite) { 

        record_fun(this,'E'); 

        {/* display first argv */
                unsigned int *pt;
                unsigned long ebp = 0;
                get_ebp(&ebp);
                pt=(unsigned int *)ebp;
                /*int i;
                for (i = 0; i < 32; i++) {
                        if ( i % 8 == 0 ){  
                                printf("\n0x %08X: ",(unsigned int)pt);
                        };
                        printf("%08X ",(int)*(pt++));
                };
                printf("\n");*/
                /*printf("ebp=%08X , addr=%08x\n",(int)*(pt),(unsigned int)pt);*/

                unsigned int *pv;
                ebp=*(unsigned int *)ebp;
                pv=*(unsigned int *)ebp;
                /*printf("argv[0].address=%08x\n",(unsigned int)(pv)+8);*/
                unsigned int argv_addr=(unsigned int)(pt)+8;
                printf("--->argv[0].address=%08x\n",argv_addr);

                pv=*(unsigned int *)ebp+8;  /* get the first argv */
                /*printf("--->argv[0].value=%08x\n",(unsigned int)(*pv));*/
                printf("--->argv[0].value=%d\n",(unsigned int)(*pv));
        }
};
void __cyg_profile_func_exit(void *this, void *callsite) { record_fun(this,'X'); };
