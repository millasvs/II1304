/*Splay trees:

A splay tree is a self-adjusting binary search tree with the additional property that recently accessed elements are quick to access again.
The tree is reorganized after every lookup, as opposed to an ordinary BST, in order to keep the element accessed most often in the root. This means that elements that are often accessed
are found in the top of the tree.

splaying the tree for a certain element rearranges the tree so that the element is placed at the root of the tree
Frequently accessed elements are moved nearer the root where they can be accessed again more quickly
Splay trees do not need to store any bookkeeping data

Disadvantages:
the height can be linear, which happens for example after accessing all n elements in a non-decreasing order.

Operations:
when a node x is accessed, a splay operation is performed on x to move it to the root, by carrying out a sequence of splay steps, each of which moves x closer to the root.
Each step depends on three factors:
- whether x is the left or right child of its parent node, p
- whether p is the root or not, and of not
- whether p is the left or right child of its parent, g (the grandparent of x).

After any splay operations, the great-grandparent of x (gg) is set to point to x.
If gg is null, x is the root and must be updated as such.

zig step:
This step is performed when p is the root. This rotates x to the root, p to the right of x and the right child of x to the left child of p.
Condition: When the last accessed node (node x) is a child of the root
operation: single left or right rootation.

zig-zig step:
this step is done when p is not the root and x and p are either both right children or are both left children. This places x in the root, p as right child of x and g as a right child of p,
as well as rootating the right child of x to the left child of p and the right child of p to the left side of g.
Condition: When X is the left child of the parent P and P is the left child of the grandparent or if they are both right parents
Operation:

zig-zag step:
This step is performed when p is not the root and x is a right child and p is a left child or vice versa. This Places x in the root, g as a right child to x and p as a left child to x.
The left child of x is rotated to the right child of p, and the right child of x is rotated to the left child of g.
Condition: When X is the left child of P and P is the right child of G or vice versa
Operation: Double rootation

Retrieval:
Find node X using BST methods, then splay X up tp the root

Insertion:
Insert node X using BST methods, then splay X up to the root

How to run:
compile: gcc splayTree.c -o splayTree
run: ./splayTree
*/
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include<limits.h>

#define NUM_ITERATIONS 100
//#define DEBUG

struct node{
    double timestamp;
    struct node *left;
    struct node *right;
};

void preOrder(struct node *root);

/* thresholds for timestamp. */
double l_threshold, h_threshold;
/* seed value for timestamp generation */
long seed;

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



struct node *new_node(double timestamp){
    struct node* new_node = (struct node*) malloc(sizeof(struct node));
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

double generate_timestamp(){

	double timestamp;

	/* uniform distribution */
	timestamp = h_threshold * drand48();

	return timestamp;
}

/* generates a splay tree with the number of nodes spcified in size*/
struct node *generate(struct node *root, int size){

	for(int i = 0; i < size; i++){

    	double t = l_threshold + generate_timestamp();//drand48() * h_threshold;

    	root = insert(root, t);
	}
    return root;
}

/* Simulates a usage of the splay tree where one element
   is removed and another is inserted for a number of iterations.
   The average time taken for an insert + remove operation
   is returned */
double simulate(struct node *root){

	double t_start, t_end, timeElapsed;
	int i;
	t_start = read_timer();
	for(i = 0; i < NUM_ITERATIONS; i++){

        /* dequeue element with the highest priority */
		root = remove_node(root, root->timestamp);

		/* enqueue an element with a randomly generated timestamp */
		double t = l_threshold + generate_timestamp();//drand48() * h_threshold;

        root = insert(root, t);
	}


#ifdef DEBUG
    printf("Last print\n");
    preOrder(root);
    printf("\n\n");
#endif
	t_end = read_timer();
	timeElapsed = (t_end - t_start) / NUM_ITERATIONS;

	return timeElapsed;
}


// A utility function to print preorder traversal of the tree.
// The function also prints height of every node
void preOrder(struct node *root){

    if (root != NULL){
        printf("%g ", root->timestamp);
        preOrder(root->left);
        preOrder(root->right);
    }
}

/* Drier program to test above function*/
/*int main()
{
    struct node *root = new_node(100);
    root = insert(root, 50);
    root = insert(root, 200);
    root = insert(root, 40);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 25);
    printf("Preorder traversal of the modified Splay tree is \n");
    preOrder(root);
    printf("\n");

    remove_node(root, 25);
    printf("Preorder traversal of the modified Splay tree is \n");
    preOrder(root);
    printf("\n");
    return 0;
}*/
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
        preOrder(root);
    #endif
    printf("\n");

	double timeElapsed = simulate(root);

	printf("time elapsed: %g\n", timeElapsed);
	//printf("\t%d\t%g\n", size, timeElapsed);

	return 0;
}
