#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include<limits.h>
#include <string.h>
#include "print_splay.h"
#define DEBUG
#ifdef DEBUG
    #define NUM_ITERATIONS 10
#else
    #define NUM_ITERATIONS 100
#endif




void preOrder(struct node *root);
//void print_ascii_tree(struct node * t);

/* thresholds for timestamp. */
double l_threshold, h_threshold;
/* seed value for timestamp generation */
long seed;

struct node* new_node(double timestamp){
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->timestamp = timestamp;
    new_node->left = NULL;
    new_node->right = NULL;
    return (new_node);
}

struct node *right_rotate(struct node *x){
    struct node *y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

struct node *left_rotate(struct node *x){
    struct node *y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

struct node *splay(struct node *root, double timestamp){

    //base case as this is a reqursive function
    if(root == NULL || root->timestamp == timestamp) return root;

    if(root->timestamp > timestamp){
        if (root->left == NULL) {
            return root;
        }
        //zig-zig
        if (root->left->timestamp > timestamp) {

            //recursively call splay to bring the timestamp as root of left left
            root->left->left = splay(root->left->left, timestamp);

            //rotate right the first time
            root = right_rotate(root);
        }
        //zig-zag
        else if (root->left->timestamp < timestamp) {
            root->left->right = splay(root->left->right, timestamp);
            if (root->left->right != NULL){
                root->left = left_rotate(root->left);
            }
        }
        return (root->left == NULL)? root: right_rotate(root);
    }
    else /*if(root->timestamp < timestamp)*/{
        if (root->right == NULL){
            return root;
        }

        //zig-zag
        if (root->right->timestamp > timestamp) {
            root->right->left = splay(root->right->left, timestamp);
            if (root->right->left != NULL){
                root->right = right_rotate(root->right);
            }
        }
        //zig-zig
        else if(root->right->timestamp < timestamp){
            root->right->right = splay(root->right->right, timestamp);
            root = left_rotate(root);
        }
        return (root->right == NULL)? root: left_rotate(root);
    }
}

struct node *insert(struct node *root, double timestamp){

    //If tree is empty, just create the new node
    if (root == NULL) {
        return new_node(timestamp);
    }
    //Splay the current node to the root
    root = splay(root, timestamp);

    struct node *node = new_node(timestamp);

    //If the timestamp is less than the root timestamp. In this case the old root should be the right child of the new node
    //Insert new node at root
    if (root->timestamp > timestamp) {

        //Copy the left chiold of the root to the new node
        node->left = root->left;

        //Make the root the right child if the new node
        node->right = root;

        //The root no longer has a left child
        root->left = NULL;
    }
    //If the timestamp is greater than the root, the root should be the left child of the new node
    // Insert new node at root
    else if(root->timestamp < timestamp){

        //Make the right child of the root the right child of the new node
        node->right = root->right;

        //Set the root to the left child of the new node
        node->left = root;

        //The root no longer has a right child
        root->right = NULL;
    }
    //root->timestamp == timestamp
    else{
        return root;
    }
    //the new node becomes the new root
    return node;
}

//Deletes the node with the specified timestamp
struct node *remove_node(struct node *root, double timestamp){

    //The tree is empty, just return
    if (root == NULL) return root;

    //Splay the node we want to delete to the root
    root = splay(root, timestamp);

    if (root->timestamp == timestamp) {

        //If the root doesn't have any left children
        if (root->left == NULL) {

            //Update the root to the right child of the root. This way the new root,
            //i.e. the node we want to delete that was splayed to the root, is deleted.
            root = root->right;
        }
        //Otherwise
        else{

            //Store the right child of the root in a temporary node
            struct node *temp = root->right;

            //Set the root to the node's left child. Here the root is deleted.
            root = root->left;

            //Splay the root to the top
            root = splay(root, timestamp);

            //Set the right child of the new node to the right child of the old root
            root->right = temp;
        }
    }
    return root;
    //else: it wasn't in the tree to remove
}

/*find the node with the highest priority, i.e. the node with the smalles timestamo*/
struct node *find_min(struct node *root){

    //The tree is empty, return NULL
    if (root == NULL){
        return NULL;
    }
    //The root has no left children, which means that the root has the smallest timestamp
    else if (root->left == NULL){
        return root;
    }
    else{
        //Recursively call find_min to get to the node to the farthest left, i.e. the node with the smallest timestamp
        return find_min(root->left);
    }
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

/* generates a splay tree with the number of nodes spcified in size*/
struct node *generate(struct node *root, int size){

	for(int i = size-1; i >= 0; i--){

    	root = insert(root, i);
	}
    return root;
}

/* Simulates a usage of the splay tree where one element
   is removed and another is inserted for a number of iterations.
   The average time taken for an insert + remove operation
   is returned */
double simulate(struct node *root, int size){

	double t_start, t_end, timeElapsed;
	int i;
	t_start = read_timer();
	for(i = 0; i < NUM_ITERATIONS; i++){

		root = remove_node(root, find_min(root)->timestamp);

        #ifdef DEBUG
            printf("After remove\n");
            print_ascii_tree(root);
            printf("\n\n");
        #endif

        root = insert(root, 0);

        #ifdef DEBUG
            printf("After insert\n");
            print_ascii_tree(root);
            printf("\n\n");
        #endif
	}


#ifdef DEBUG
    printf("Last print\n");
    print_ascii_tree(root);
    printf("\n\n");
#endif
	t_end = read_timer();
	timeElapsed = (t_end - t_start) / NUM_ITERATIONS;

	return timeElapsed;
}

int main (int argc, char *argv[]){

	int size = (argc > 1)? atoi(argv[1]) : 1000;
	seed = (argc > 2)? atol(argv[2]) : 831970590;

    struct node *root = NULL;

	srand48(seed);

	l_threshold = 1;
	h_threshold = size;

	root = generate(root, size);
    #ifdef DEBUG
        printf("First print\n" );
        print_ascii_tree(root);
    #endif
    printf("\n");

	double timeElapsed = simulate(root, size);

	printf("time elapsed: %g\n", timeElapsed);
	//printf("\t%d\t%g\n", size, timeElapsed);

	return 0;
}
