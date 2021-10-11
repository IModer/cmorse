#ifndef MORSE_H
#define MORSE_H

typedef struct node {
	char caracter;
	//char * kod;
	unsigned int kod;
	struct node *left;
	struct node *right;
} Node;

Node * createnode(char c);
Node * createtree(int layers);

void buildtree(Node * n, int layer);
void printtree(Node * n);
void addtotree(Node * n, char c, char * morse, unsigned int kod, int i);
void add_morse(Node * n, char * morse, char c);


char decodemorse(Node * n, char * morse);
void encodemorse(Node * n, char c, char * ret);

#endif