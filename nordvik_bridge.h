#ifndef nordvik_bridge_h
#define nordvik_bridge_h

#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "sem_ops.h"
#define MAX_CARS 10
#define MAX_TRUCKS 5
#define WAIT_TEN 10
#define WAIT_FIVE 5
#define CROSSING_TIME 4

#endif

void cleanup();
void *car_entry_west( void * arg );
void *car_entry_east( void * arg );
void *truck_entry_west( void * arg );
void *truck_entry_east( void * arg );
void setup_bridge();
void setup_counter();
void setup_cars();
int main();


