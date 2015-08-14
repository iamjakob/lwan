#include "bounded_queue.h"
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdatomic.h>
#define startat 0
#define maxsleep 100
#define test_val 1
#define end_val 1000000000
#define minsleep 0
#define sleepincr 10
#define pop_size 20
#define push_size 50

bounded_queue  shared_queue;
_Atomic size_t micro_slept;

void sleep_micros(size_t mic) {
    struct timespec mytime;
    mytime.tv_sec = 0;
    mytime.tv_nsec = mic*1000;
    nanosleep(&mytime, NULL); // sleep 1 sec
}

//simulate doing other stuff
void sleep_if(size_t i) {
    if (!(i%10000))
        sleep_micros(5);
}

int isgood = 1;

void *do_pops(void *data) {
    void *st = 0;
    size_t maxs = minsleep;
    for (;;) {
        if(pop(&shared_queue, &st)) { 
            if (st == (void *)end_val) {
                break;
            }
            maxs = minsleep;
        }
        else {
            maxs = maxs >= maxsleep? maxs : maxs + sleepincr;
            sleep_micros(maxs);
        }
    }
    return NULL;
}

void *do_pushes(void *indat) {
    size_t maxs = minsleep;
    for (size_t i = startat + 1; i <= end_val + 1;) {
        if(push(&shared_queue, (void *)i)) {
            i++;
            maxs=minsleep;
            sleep_if(i + 5000);
        }
        else {
            sleep_micros(maxs += sleepincr);
        }
    }
    return NULL;
}

void printtime(clock_t start) {
    double secs = (clock() - start)*1.0/CLOCKS_PER_SEC;
    double ptr_sec = end_val/secs;
    printf("Time spent was %f, %f pointers per second, %f ns/push-pop, %f mb/sec\n", secs, ptr_sec, 1e9 * 1.0 /ptr_sec, ptr_sec * sizeof(void *)/1000000.0);
}

int main() {
    pthread_t pusher, popper;
    init_queue(&shared_queue, 4096*8);
    clock_t myclock = clock();

    pthread_create(&pusher, NULL, do_pushes, NULL);
    sleep_micros(10);
    pthread_create(&popper, NULL, do_pops, NULL);

    pthread_join(pusher, NULL);
    pthread_join(popper, NULL);
    printf("Was the program valid? %d\n", isgood);
    printtime(myclock);
    clear_queue(&shared_queue);
}
