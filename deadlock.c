/*
 * demo for mutex lock 
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

int c1=1000;
int c2=2000;
int c3=3000;
#define gettid() syscall(SYS_gettid)
#define thread(x) thread##x


pthread_mutex_t ma;
pthread_mutex_t mb;
pthread_mutex_t mc;

volatile int ga=0;
volatile int gb=0;
volatile int gc=0;

void locka(void){pthread_mutex_lock(&ma);};
void lockb(void){pthread_mutex_lock(&mb);};
void lockc(void){pthread_mutex_lock(&mc);};

int test(int a,int b,int c){return a+b+c;};

void unlocka(void) {pthread_mutex_unlock(&ma);};
void unlockb(void) {pthread_mutex_unlock(&mb);};
void unlockc(void) {pthread_mutex_unlock(&mc);};

void add_ab(void){locka();ga++;lockb();gb++;unlockb();unlocka();};
void add_ac(void){locka();ga++;lockc();gb++;unlockc();unlocka();};
void add_cb(void){lockc();ga++;lockb();gb++;unlockb();unlockc();};
void add_ca(void){lockc();ga++;locka();gb++;unlocka();unlockc();};
void add_bc(void){lockb();ga++;lockc();gb++;unlockc();unlockb();};
void add_ba(void){lockb();ga++;locka();gb++;unlocka();unlockb();};

void add_a(void){ locka();ga++;unlocka();};
void add_b(void){ lockb();gb++;unlockb();};
void add_c(void){ lockc();gc++;unlockc();};

/* ---------- thread function -------------*/
void *thread1(void *arg)
{
	int i = (int) arg;
	printf("\t%s(%ld):%d\n", __func__, gettid(), i);
        locka();
        lockb();
        unlocka();
        lockc();
        unlockc();
        unlockb();
        
        lockb();
        unlockb();

        lockc();
        unlockc();


	return NULL;
}

void *thread2(void *arg)
{
	int i = (int) arg;
	printf("\t%s(%ld):%d\n", __func__, gettid(), i);
        sleep(1);
        lockb();
        lockc();
        locka();
        unlocka();
        unlockb();
        unlockc();
	return NULL;
}

void *thread3(void *arg)
{
	int i = (int) arg;
	printf("\t%s(%ld):%d\n", __func__, gettid(), i);
        add_ca();
	return NULL;
}

int main()
{
        pthread_t pthr[3];

        pthread_mutex_init(&ma, NULL);
        pthread_mutex_init(&mb, NULL);
        pthread_mutex_init(&mc, NULL);
        {
                pthread_create(&pthr[0], NULL, thread1, (void *) 1000);
                pthread_create(&pthr[1], NULL, thread2, (void *) 2000);
                pthread_create(&pthr[2], NULL, thread(3), (void *) 3000);
                usleep(10);
                locka();
                unlocka();
                lockc();
                unlockc();
                pthread_join(pthr[0], NULL);
                pthread_join(pthr[1], NULL);
                pthread_join(pthr[2], NULL);
        }
        pthread_mutex_destroy(&ma);
        pthread_mutex_destroy(&mb);
        pthread_mutex_destroy(&mc);

        test(0x11,0x22,0x33);

        return 0;
}
