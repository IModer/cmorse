#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "morse.h"

Node * createnode(char c)
{
	Node * n = (Node *)malloc(sizeof(Node));
    if (n == NULL)
    {
        //Error is we could not init a Node
        fprintf(stderr, "Error: Not enough memory!\n");
    }
	if (n != NULL) {
        //Init the nodes values
		n->left = NULL;
		n->right = NULL;
		n->kod = 0;
		n->caracter = c;
	}
	return n;
}

Node * createtree(int layers)
{
	Node * tree = createnode('*');  //Create main node
	buildtree(tree, layers);        //Call build tree on it
    return tree;                    //Return the pointer to the built tree
}

void buildtree(Node * n, int layer)
{
    
    #ifdef DEBUG
    printf("----------------------------------------------\n");
    printf("buildtree:\t%p \t%d\n", n, layer);
	#endif

    //We have reached the end of the recursion
    if (layer == 0) 
	{
        //Create the last 2 nodes
		Node *l = createnode(' ');
        n->left = l;
        Node *r = createnode(' ');
        n->right = r;

        #ifdef DEBUG
        printf("Completed: %d\t\t with leaves \t%p\n\t\t\t\t\t%p\n", layer, l ,r);
		printf("----------------------------------------------\n");
        #endif
        
        return;
	}

    //Create child nodes with empty caracters and assign them
    Node *l = createnode(' ');
	n->left = l;
    Node *r = createnode(' ');
	n->right = r;

    #ifdef DEBUG
    printf("Building tree: %d\t with leaves \t%p\n\t\t\t\t\t%p\n", layer, l ,r);
    printf("----------------------------------------------\n");
	#endif

    //Recursively call buildtree
	buildtree(l, layer-1);
	buildtree(r, layer-1);
}

void addtotree(Node * n, char c, char * morse, unsigned int mkod, int i)
{
    
    #ifdef DEBUG
    printf("addtotree:\t%p \t%c \t%p \n", n, c, morse);
	#endif
	
    //Switch the current caracter of the morse code
    switch (*morse)
	{
        //End of the code
        case '\0':

            //Add the last caracter
            n->kod = mkod;
            n->caracter = c;

            #ifdef DEBUG
            printf("A node címe : \t%p\n",  n);
            printf("A karakter : \t%c\n",   n->caracter);
            printf("A kod : \t%d\n",        n->kod);
            printf("A mkod : \t%d\n",       mkod);
            printf("A morse : \t%s\n",      morse);
            #endif
            
            return;
		case '.':   //left

            //incode the morse code into mkod
			mkod += mpow(10, i); // 1 * 10^i, we increase the ith position of mkod 
		
        	#ifdef DEBUG
            printf("C:%c K:%d\n",*morse, mkod);
            #endif

            //Recursively call with the next caracter (morse+1)
            addtotree(n->left,  c,  morse+1,    mkod,   i+1);
			break;
		
        case '-':   //right
            
            //incode the morse code into mkod
            mkod += 2 * mpow(10, i); //2 * 10^i, we increase the ith position of mkod
			
            #ifdef DEBUG
            printf("C:%c K:%d\n",*morse, mkod);
            #endif
            
            //Recursively call with the next caracter (morse+1)
            addtotree(n->right, c,  morse+1,    mkod,   i+1);
			break;
	}
}

int mpow(int a, int n) //== a^n
{
    int pi = 1;
    for (int i = 0; i < n; i++)
    {
        pi *= a;
    }
    return pi;
}

char decodemorse(Node * n, char * morse)
{
	switch (*morse)
	{

        case '\0':

            #ifdef DEBUG
            printf("decodemorse: %d\n", n->kod);
            #endif
        
            return n->caracter;
		
        case '.':   //left
			decodemorse(n->left,   morse+1);
			break;
		
        case '-':   //right
			decodemorse(n->right,  morse+1);
			break;
	
    }
}

int encodemorse(Node * n, char c, char * ret)
{
    
    #ifdef DEBUG
    printf("encodemorse:\t%p\t%c=?=%c\n", n, c, n->caracter);
    printf("----------------------------------------------\n");
	#endif

    //Found the caracter
    if (n->caracter == c)
    {
        int a = n->kod; //pl 1122 -> --..
        int i = 0;
        //Decode kod to a morse code
        while (a != 0)
        {
            if (a % 2 == 0)   
            {
                ret[i] = '-';
            }
            else
            {
                ret[i] = '.';
            }  
            
            #ifdef DEBUG
            printf("a osztas elott=%d\n",a);
            #endif
            
            a /= 10;

            #ifdef DEBUG
            printf("a osztas utan=%d\n",a);
            #endif
            
            i++;

            #ifdef DEBUG
            printf("encode returned %d\n", n->kod);
            printf("%c => %d\n", c, a);
            printf("A=%d, I = %d\n", a, i);
            #endif
        }

        ret[i] = '\0';
        return n->kod;
    }

    if (n->left != NULL && n->right != NULL)
    {
        encodemorse(n->left,  c, ret);
        encodemorse(n->right, c, ret);
    }
}

void add_morse(Node * n, char * morse, char c)
{

    #ifdef DEBUG
    printf("add_morse:\t%p\t%p\t%c\n", n, morse, c);
    printf("----------------------------------------------\n");
    printf("\t\t[n]\t\t\t[c]\t[morse]\t\t\t[mkod]\t\t\t[mkod->s]\t[i]\n");
    #endif
    
    addtotree(n, c, morse, 0, 0);

}

void printtree(Node * n)
{
    if (n->caracter != ' ')
    {
        printf("Kod: %p\t Karakter: %c\n", n->kod, n->caracter);
    }
    if (n->left != NULL)
    {
        printtree(n->left);
    }
    if ( n->right != NULL)
    {
        printtree(n->right);
    }
}

int main(int argc, char **argv)
{
    /* usage : morse.exe [MOD] "[MESSAGE]" */
    //Handle argv

    #ifdef DEBUG
    printf("argv[0] = %s,argv[1] = %s,argv[2] = %s\n", argv[0], argv[1],argv[2]);
    #endif
    
    if (argc != 3)
    {
        printf("Error: Wrong number of argoments provided\nusage : morse.exe [MODE] \"[MESSAGE]\"\nMode: \t1 - decode\n\t2 - encode");
        return 1;
    }
    
    if ((!atoi(argv[1])) || atoi(argv[1]) < 1 || atoi(argv[1]) > 2) 
    {
        printf("Error: Wrong Mode provided \nusage : morse.exe [MODE] \"[MESSAGE]\"\nMode: \t1 - decode\n\t2 - encode");
        return 2;
    }
    
    int mode = atoi(argv[1]);
    char * input = argv[2];
    
    //Create tree
    Node *r;
    r = createtree(5);
    
    //Read the file, build the tree
    FILE *morse_file = fopen("morse.txt", "r");
    
    for (int i = 0; i < ALPHABET_LENGTH; ++i)
    {
        char * morse_kod[10];
        char morse_caracter;
        fscanf(morse_file, "%s %c", morse_kod, &morse_caracter);
        
        #ifdef DEBUG
        printf("read : %s %c\n", morse_kod, morse_caracter);
        #endif
        
        add_morse(r, morse_kod, morse_caracter);
    }
    
    fclose(morse_file);
    
    printf("Parsing the input string '%s'\n", input);
    const char delimiter[2] = " ";
    char *token;
   
    token = strtok(input, delimiter);
   
    /* walk through other tokens */
    while( token != NULL ) {

        #ifdef DEBUG
        printf( "%c\n", *token );
        printf( "%s\n", token );
        #endif
        
        if (mode == 1)
        {
            char dec = decodemorse(r, token);
            printf("%c ", dec);
        }
        else
        {
            char enc[6];
            int a = encodemorse(r, *token, enc);
            printf("%s ", enc);
        }
        token = strtok(NULL, delimiter);
    }
    printf("\n");

	return 0;
}