/*
	Testing efficiency of a priority queue implemented with a 
	doubly-linked list where the events with the lowest timestamp
	have the highest priority.
	
	Usage in Linux:
	gcc llpqueue.c -o avg -lm
	./avg
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include<limits.h>

#define NUM_ITERATIONS 100

/* every element has a timestamp
and a pointer to the previous element
and the next element */
typedef struct event_t event_t;

struct event_t{
	double timestamp;
	event_t *next;
	event_t *prev;
};

/* first and last node in the list */
event_t *first;
event_t *last;

/* thresholds for timestamp. */
double l_threshold, h_threshold; 
/* seed value for timestamp generation */
long seed;

/* comment out the unwanted distributions */
double generate_timestamp(){

double timestamp;

/* exponential distribution */
//timestamp = -log(drand48());


/* uniform distribution */
timestamp = h_threshold * drand48();

/* biased distribution */
//timestamp = 0.9 + 0.2 * drand48();

/* bimodal distribution */
//double rand = drand48();
//timestamp = 0.95238 * rand + (rand < 0.1? 9.5238 : 0);

/* triangular distribution */
//timestamp = 1.5 * sqrt( drand48() );


return timestamp;

}



/* prints all elements in the list in forward traversal order */
void print_list() {
	event_t *temp = first;
	printf("Forward: ");
	while(temp != NULL) {
		printf("%g ",temp->timestamp);
		temp = temp->next;
	}
	printf("\n");
}

/* prints all elements in the list in forward traversal order */
void assert_list() {
	event_t *temp = first;
	while(temp != NULL) {
		if(temp->next != NULL)
			assert(temp->timestamp <= temp->next->timestamp);
		
		temp = temp->next;

	}
	printf("\n");
}


/* inserts an element in the list. begins
iteration from the start of the list */
void place_from_end(event_t *entry){

#ifdef DEBUG				
	printf("place from end\n");	
#endif
	
	if(first==NULL){ //if list is empty
		entry->next=NULL;
		entry->prev=NULL;
	
		first = entry;
		last = entry;
		return;
	}
	
	event_t *temp = last;
	
	if(temp->timestamp <= entry->timestamp){
		/* put entry last in the list */
		entry->prev = last;
		entry->next = NULL;
		last->next = entry;
		last = entry;
	} else {
	
	
	/* traverse the list to put new element in right place */
	while(temp != NULL && temp->timestamp > entry->timestamp)	
		temp = temp->prev;
	

	/* insert entry after temp */
	temp->next->prev = entry;				
	entry->next = temp->next;
	temp->next = entry;
	entry->prev = temp;
	}
}

/* inserts an element in the list. begins
iteration from the end of the list */
void place_from_beginning(event_t *entry){

#ifdef DEBUG				
	printf("place from beginning\n");	
#endif
	
	if(first==NULL){ //if list is empty
		entry->next=NULL;
		entry->prev=NULL;
	
		first = entry;
		last = entry;
		return;
	}
	
	event_t *temp = first;

	if(temp->timestamp > entry->timestamp){
	/* put entry first in the list */
	entry->next = first;
	entry->prev = NULL;
	first->prev = entry;
	first = entry;
	} else {
	
	/* traverse the list to put new element in right place */
	while(temp != NULL && temp->timestamp <= entry->timestamp)	
		temp = temp->next;
	
	
	/* insert entry before temp */
	temp->prev->next = entry;
	entry->prev = temp->prev;
	temp->prev = entry;
	entry->next = temp;	
	}
}

/* dequeue element with highest priority,
   which is the first element in the list,
   and update global lower threshold for timestamps */
void unlink_min(){

	if(first == NULL)
		return;
		
	event_t *evict = first;
	first = evict->next;
	l_threshold = evict->timestamp;
	free(evict);
}

/*	timer */
double read_timer() {

  static bool initialized = false;
  static struct timeval start;
  struct timeval end;
  if( !initialized )
  {
	  gettimeofday( &start, NULL );
	  initialized = true;
  }
  gettimeofday( &end, NULL );
  
  return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);

}

/* generates a sequence of events */
void generate(int size){

	first = NULL;
	last = NULL;
	
	for(int i = 0; i < size; i++){
	event_t *new = (event_t*)malloc(sizeof(event_t));
	
	double t = l_threshold + generate_timestamp();//drand48() * h_threshold;
	new->timestamp = t;
	
	if(first != NULL && (t < ((first->timestamp + last->timestamp) / 2)))
		place_from_beginning(new);
	else 
		place_from_end(new); 
	}
}


/* Simulates a usage of the priority queue where one element 
   is dequeued and another is enqueued for a number of iterations. 
   The average time taken for an enqueue + dequeue operation 
   is returned */ 
double simulate(){

	double t_start, t_end, timeElapsed;	
	int i;
	t_start = read_timer();
	for(i = 0; i < NUM_ITERATIONS; i++){
		/* dequeue element with the highest priority */
		unlink_min();
		
		/* enqueue an element with a randomly generated timestamp */
		event_t *new = (event_t*)malloc(sizeof(event_t));
		double t = l_threshold + generate_timestamp();//drand48() * h_threshold;
		new->timestamp = t;
#ifdef DEBUG				
		printf("new = %g\n", new->timestamp);
		printf("((first->timestamp + last->timestamp) / 2) = %g\n", ((first->timestamp + last->timestamp) / 2));
#endif
		if(new->timestamp < ((first->timestamp + last->timestamp) / 2))
			place_from_beginning(new);	
		else
			place_from_end(new);

#ifdef DEBUG		
		print_list();
	/* makes sure list is sorted */
		assert_list();
#endif
	}
	t_end = read_timer();
	timeElapsed = (t_end - t_start) / NUM_ITERATIONS	;

	return timeElapsed;
}


int main (int argc, char *argv[]){
	
	int size = (argc > 1)? atoi(argv[1]) : 1000;
	seed = (argc > 2)? atol(argv[2]) : 831970590;
	
	srand48(seed);
	
	l_threshold = 1;	
	h_threshold = size;
	
	generate(size);
	
#ifdef DEBUG
	print_list();
#endif


	double timeElapsed = simulate();
	printf("%g\n", timeElapsed);
	

	return 0;
}

