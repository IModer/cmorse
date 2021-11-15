#ifndef MORSE_H
#define MORSE_H

//Constants
#define ALPHABET_LENGTH 26

//The node struct
//arguably tree could have its own type
typedef struct node {
	char caracter;
	unsigned int kod;
	//  ^
	//  |
	//  ---- this encodes the morse code of the caracter
	// 1122 <=> --..
	// 1121 <=> .-..
	// ones represent dots
	// twos the dots 
	struct node *left;
	struct node *right;
} Node;

//Functions to create and init nodes and the tree

//Creates and inits a nodes, returns a pointer to it.
Node * createnode(char c);

/*Creates the tree,
inits one node with caracter '*'
and calls buildtree() on it.*/
Node * createtree(int layers);

//Functions of the Tree

//Builds the tree to n with layer number of layers
void buildtree(Node * n, int layer);
//Prints the tree with dfs (?)
void printtree(Node * n);

void addtotree(Node * n, char c, char * morse, unsigned int kod, int i);

void add_morse(Node * n, char * morse, char c);


char decodemorse(Node * n, char * morse);
int encodemorse(Node * n, char c, char * ret);

#endif