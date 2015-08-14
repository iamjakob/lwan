extern "C" {
#include "bounded_queue.h"
}
#include <thread>

#define startat 0
#define maxsleep 100
#define test_val 1
#define end_val 10000
#define minsleep 0
#define sleepincr 10

bounded_queue shared_queue;

using namespace std;
using namespace std::chrono;

void sleep_micros(size_t mic) {
    this_thread::sleep_for(milliseconds(mic));
}

int do_pops() {
    void *st = 0;
    char *stlast = 0;
    int isgood = 1;
    size_t maxs = minsleep;
    for (;;) {
        if(pop(&shared_queue, &st)) {
            if (stlast++ != (char *)st)
                isgood = 0;
            else if (st == (void *)end_val)
                return isgood;
            maxs=minsleep; //should just be register mov, should be fine...
        }
        else {
            maxs += sleepincr;
            sleep_micros(maxs);
        }
    }
    return isgood;
}

void do_pushes() {
    size_t maxs = minsleep;
    for (size_t i = startat + 1; i <= end_val; i++) {
        if(push(&shared_queue, (void *)i)) {
            maxs=minsleep; //should just be register mov, should be fine...
        }
        else {
            maxs += sleepincr;
            sleep_micros(maxs);
        }
    }
}

int main() {
    
}
