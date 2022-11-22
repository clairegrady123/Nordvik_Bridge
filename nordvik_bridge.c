/******************************************************************************
 * Created by Claire Grady on 27 / 7 / 2022.
 * nordvik_bridge.c
 * Program that demonstrates the use of semaphores to synchronize
 * parallel threads. There are 10 eastern and 10 western car threads
 * as well as 5 eastern and 5 western truck threads. A maximum of 5 cars
 * OR one truck are allowed on the bridge at any one time.
 *
 * All threads are printed to the terminal.
 *
 * Returns: 0 on Success
 *
 * Build:
 *     gcc -Wall -pedantic -pthread -o nordvik_bridge nordvik_bridge.c
 * Run Example:
 *    ./nordvik_bridge
 * ***************************************************************************/


#include "nordvik_bridge.h"

int cars_east, cars_west, counter_east, counter_west, bridge, east_counter, west_counter;

/*****************************************************************************
 * Function: cleanup
 *
 * Function to ensure semaphores are removed
 *
 * Returns: 0 on Success
 ****************************************************************************/
void cleanup() {   
  rm_sem(cars_east);
  rm_sem(cars_west);
  rm_sem(counter_east);
  rm_sem(counter_west);
  rm_sem(bridge);
  exit(0);
}

/*****************************************************************************
 * Function: *car_entry_west
 *
 * Thread entry point function that acquires and releases semaphores to ensure
 * no cars travel west when a truck or car traveling east is on the bridge
 * and to make sure that no more than 5 cars are traveling west on the bridge
 * at any one time
 * 
 * Parameters: 
 *          1. Void pointer that points to the id of the car_west thread
 *
 * Returns: 0 on Success
 ****************************************************************************/
void *car_entry_west( void * arg ) { 
  int *id; 
  id = (int *) arg; //car id
  if (sleep(rand() % WAIT_TEN) != 0){ //sleep random amount up to 10 secs
    perror("There was a problem with the random sleep call in car_entry_west");
    exit(EXIT_FAILURE);
  }
  P(cars_west); //acquire semaphore to ensure a max of 5 cars going west
  P(counter_west); //acquire semaphore to ensure only one thread can access counter
  west_counter++; //increment counter
  if (west_counter == 1)
    P(bridge); //if one car on bridge acquire semaphore to lock bridge
  V(counter_west); //release counter semaphore
  if (sleep(west_counter) != 0){ //sleep counter seconds so threads should exit in
  //the same order they entered
    perror("There was a problem with the west_counter sleep call in car_entry_west");
    exit(EXIT_FAILURE);
  }
  printf("Car %d is going west on the bridge!\n", *id);
  if (sleep(CROSSING_TIME) != 0){ //sleep crossing time
    perror("There was a problem with the CROSSING_TIME sleep call in car_entry_west");
    exit(EXIT_FAILURE);
  }
  printf("Car %d is going west off the bridge!\n", *id);
  P(counter_west); //acquire semaphore to ensure only one thread can access counter
  west_counter--; //decrememt counter
  if (west_counter == 0)
    V(bridge); //release bridge lock when all cars on bridge have exited
  V(counter_west); //release counter semaphore
  V(cars_west); //release car semaphore     
  pthread_exit( NULL ); //threads exit
}

/*****************************************************************************
 * Function: *car_entry_east
 *
 * Thread entry point function that acquires and releases semaphores to ensure
 * no cars travel east when a truck or car traveling west is on the bridge
 * and to make sure that no more than 5 cars are traveling east on the bridge
 * at any one time
 * 
 * Parameters: 
 *          1. Void pointer that points to the id of the car_east thread
 *
 * Returns: 0 on Success
 ****************************************************************************/
void *car_entry_east( void *arg ) {
  int *id;
  id = (int *) arg; //car id
  if (sleep(rand() % WAIT_TEN) != 0){ //sleep random amount up to 10 secs
    perror("There was a problem with the random sleep call in car_entry_east");
    exit(EXIT_FAILURE);
  }
  P(cars_east); //acquire semaphore to ensure a max of 5 cars going east
  P(counter_east); //acquire semaphore to ensure only one thread can access counter
  east_counter++; //increment counter
  if (east_counter == 1)
    P(bridge); //if one car on bridge acquire semaphore to lock bridge
  V(counter_east); //release counter semaphore
  if (sleep(east_counter) != 0){ //sleep counter seconds so threads should exit in
  //the same order they entered
    perror("There was a problem with the east_counter sleep call in car_entry_east");
    exit(EXIT_FAILURE);
  }
  printf("Car %d is going east on the bridge!\n", *id);
  if (sleep(CROSSING_TIME) != 0){ //sleep crossing time
    perror("There was a problem with the CROSSING_TIME sleep call in car_entry_east");
    exit(EXIT_FAILURE);
  }
  printf("Car %d is going east off the bridge!\n", *id);
  P(counter_east); //acquire semaphore to ensure only one thread can access counter
  east_counter--; //decrement counter
  if (east_counter == 0)
    V(bridge); //release bridge lock when all cars on bridge have exited
  V(counter_east); //release counter semaphore
  V(cars_east); //release car semaphore  
  pthread_exit( NULL ); //threads exit
}

/*****************************************************************************
 * Function: *truck_entry_west
 *
 * Thread entry point function that acquires and releases semaphores to ensure
 * a truck will only travel west when no other vehicles are on the bridge and 
 * only one truck will be on the bridge at any one time
 * 
 * Parameters: 
 *          1. Void pointer that points to the id of the truck_west thread
 *
 * Returns: 0 on Success
 ****************************************************************************/
void *truck_entry_west( void *arg ) {
  int *id;
  id = (int *) arg; //truck id
   if (sleep(rand() % WAIT_TEN) != 0){ //sleep random amount up to 10 secs
    perror("There was a problem with the random sleep call in truck_entry_west");
    exit(EXIT_FAILURE);
  }
  P(bridge); //if one truck on bridge acquire semaphore to lock bridge
  printf("Truck %d is going west on the bridge!\n", *id);
  if (sleep(CROSSING_TIME) != 0){ //sleep crossing time
    perror("There was a problem with the CROSSING_TIME sleep call in truck_entry_west");
    exit(EXIT_FAILURE);
  }
  printf("Truck %d is going west off the bridge!\n", *id);
  V(bridge); //release bridge lock
  pthread_exit( NULL ); //threads exit
}

/*****************************************************************************
 * Function: *truck_entry_east
 *
 * Thread entry point function that acquires and releases semaphores to ensure
 * a truck will only travel east when no other vehicles are on the bridge and 
 * only one truck will be on the bridge at any one time
 * 
 * Parameters: 
 *          1. Void pointer that points to the id of the truck_east thread
 *
 * Returns: 0 on Success
 ****************************************************************************/
void *truck_entry_east( void *arg ) { 
  int *id;
  id = (int *) arg; //truck id
   if (sleep(rand() % WAIT_TEN) != 0){ //sleep random amount up to 10 secs
    perror("There was a problem with the random sleep call in truck_entry_east");
    exit(EXIT_FAILURE);
  }
  P(bridge); //if one truck on bridge acquire semaphore to lock bridge
  printf("Truck %d is going east on the bridge!\n", *id);
  if (sleep(CROSSING_TIME) != 0){ //sleep crossing time
    perror("There was a problem with the CROSSING_TIME sleep call in truck_entry_east");
    exit(EXIT_FAILURE);
  }
  printf("Truck %d is going east off the bridge!\n", *id);
  V(bridge); //release bridge lock
  pthread_exit( NULL ); //threads exit
}

/*****************************************************************************
 * Function: setup_cars
 *
 * Function that creates and initialises the cars_east and cars_west 
 * semaphores
 *
 * Returns: 0 on Success
 ****************************************************************************/
void setup_cars() {
  cars_east = semtran( IPC_PRIVATE);
  cars_west = semtran( IPC_PRIVATE);
    V5(cars_east); //initialise to 5
    V5(cars_west); //initialise to 5
}

/*****************************************************************************
 * Function: setup_bridge
 *
 * Function that creates and initialises the bridge semaphore
 *
 * Returns: 0 on Success
 ****************************************************************************/
void setup_bridge(){
  bridge = semtran( IPC_PRIVATE );
  V(bridge); //initialise to 1
}

/*****************************************************************************
 * Function: setup_counter
 *
 * Function that creates and initialises the counter_east and counter_west
 * semaphores
 *
 * Returns: 0 on Success
 ****************************************************************************/
void setup_counter(){
  counter_west = semtran(IPC_PRIVATE);
  counter_east = semtran(IPC_PRIVATE);
  V(counter_west); //initialise to 1
  V(counter_east); //initialise to 1
}

/*****************************************************************************
 * Function: main
 *
 * Main process to create and join multiple threads, pass them to their entry
 * point functions and call the cleanup semaphore function before exiting
 *
 * Returns: 0 on Success
 ****************************************************************************/
int main() {
  //define counter, semaphore, thread and id variables
  int i = cars_east = cars_west = east_counter = west_counter = 0;
  pthread_t tid_cars_east[MAX_CARS];
  pthread_t tid_cars_west[MAX_CARS];
  pthread_t tid_trucks_east[MAX_TRUCKS];
  pthread_t tid_trucks_west[MAX_TRUCKS];
  int car_west_id[MAX_CARS];
  int car_east_id[MAX_CARS];
  int truck_west_id[MAX_TRUCKS];
  int truck_east_id[MAX_TRUCKS];

  //call setup semaphore functions
  setup_cars();
  setup_bridge();
  setup_counter();

  //Setup car east ids
  for( i=0; i < MAX_CARS; i++ ){ 
    car_east_id[i] = i;
  }
  //Setup car west ids
  for( i=0; i < MAX_CARS; i++ ){ 
    car_west_id[i] = i;
  }
  //Setup truck east ids
  for( i=0; i < MAX_TRUCKS; i++ ){ 
    truck_east_id[i] = i;
  }
  //Setup truck west ids
  for( i=0; i < MAX_TRUCKS; i++ ){ 
    truck_west_id[i] = i;
  }
  //create 5 west/east car/truck threads
  for( i = 0; i < MAX_TRUCKS; i++ ){
    sleep(rand() % WAIT_FIVE);
    if (pthread_create( &tid_trucks_east[i], NULL, truck_entry_east, (void *) &truck_east_id[i])){
      perror("There was a problem creating an east truck thread");
			exit(EXIT_FAILURE);
    }
    sleep(rand() % WAIT_FIVE);
    if (pthread_create( &tid_trucks_west[i], NULL, truck_entry_west, (void *) &truck_west_id[i])){
      perror("There was a problem creating a west truck thread");
			exit(EXIT_FAILURE);
    }
    sleep(rand() % WAIT_FIVE);
    if(pthread_create( &tid_cars_west[i], NULL, car_entry_west, (void *) &car_west_id[i])){
      perror("There was a problem creating a west car thread");
			exit(EXIT_FAILURE);
    }
    sleep(rand() % WAIT_FIVE);
    if(pthread_create( &tid_cars_east[i], NULL, car_entry_east, (void *) &car_east_id[i] )){
      perror("There was a problem creating an east car thread");
			exit(EXIT_FAILURE);
    }

  }
  //create another 5 car east/west threads
   for( i = 5; i < MAX_CARS; i++ ){
    sleep(rand() % WAIT_FIVE);
    if(pthread_create( &tid_cars_west[i], NULL, car_entry_west, (void *) &car_west_id[i])){
      perror("There was a problem creating a west car thread");
			exit(EXIT_FAILURE);
    }
    sleep(rand() % WAIT_FIVE);
    if(pthread_create( &tid_cars_east[i], NULL, car_entry_east, (void *) &car_east_id[i] )){
      perror("There was a problem creating an east car thread");
			exit(EXIT_FAILURE);
    }
  }

  //join east truck threads
  for (i = 0; i < MAX_TRUCKS; i++){
    if (pthread_join( tid_trucks_east[i], NULL)){
      perror("2.There was a problem joining an east truck thread");
			exit(EXIT_FAILURE);
    }
  }

  //join west truck threads
  for (i = 0; i < MAX_TRUCKS; i++){
    if (pthread_join( tid_trucks_west[i], NULL)){
      perror("1.There was a problem joining a west truck thread");
      exit(EXIT_FAILURE);
    }
  }
  //join east car threads
    for( i = 0; i < MAX_CARS; i++ ){
    if (pthread_join( tid_cars_east[i], NULL)){
      perror("4.There was a problem joining an east car thread");
			exit(EXIT_FAILURE);
    }
  }

  //join west car threads
    for( i = 0; i < MAX_CARS; i++ ){
    if (pthread_join( tid_cars_west[i], NULL)){
      perror("3.There was a problem joining a west car thread");
			exit(EXIT_FAILURE);
    }
  }
  //call cleanup function
  cleanup();
  return 0;
}

