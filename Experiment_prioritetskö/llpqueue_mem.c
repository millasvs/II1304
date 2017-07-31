/*
	Testing memory efficiency of a priority queue implemented with a 
	doubly-linked list where the events with the lowest timestamp
	have the highest priority.
	
	Usage in Linux:
	gcc llpqueue.c -o memory
	./memory
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

/* comment out the unwanted distributions */
double generate_timestamp(){

/* uniform distribution */
double timestamp = h_threshold * drand48();

return timestamp;

}


/*	measure size in bytes of the entire list */
double sizeofList() {

	event_t *temp = first;
	
	double size_list = sizeof(*temp);
	while(temp != NULL) {
		temp = temp->next;
		size_list += sizeof(*temp);
	}

	return size_list;
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


int main (int argc, char *argv[]){
	
//	printf("#Benchmark of a doubly linked priority queue\n");
//	printf("#\n\tsize\ttimeElapsed\n");

	int size = (argc > 1)? atoi(argv[1]) : 1000;
	l_threshold = 1;
	h_threshold = size;
	
	generate(size);

	double size_list = sizeofList();

	printf("%g\n", size_list);
	
	return 0;
}

