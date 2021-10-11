#include <stdio.h>
#include <stdlib.h>
#include "morse.h"
#include <math.h>
#define ALPHABET_LENGTH 26

Node * createnode(char c)
{
	Node * n = (Node *)malloc(sizeof(Node));
    if (n == NULL)
    {
        //unreachable
        printf("OHNO!!\n");
    }
	if (n != NULL) {
		n->left = NULL;
		n->right = NULL;
		n->kod = 0;
		n->caracter = c;
	}
	return n;
}

Node * createtree(int layers)
{
	Node * tree = createnode('*');
	buildtree(tree, layers);
    return tree;
}

void buildtree(Node * n, int layer)
{
    #ifdef DEBUG
    printf("----------------------------------------------\n");
    printf("buildtree:\t%p \t%d\n", n, layer);
	#endif
    if (layer == 0)
	{
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
    Node *l = createnode(' ');
	n->left = l;
    Node *r = createnode(' ');
	n->right = r;
    #ifdef DEBUG
    printf("Building tree: %d\t with leaves \t%p\n\t\t\t\t\t%p\n", layer, l ,r);
    printf("----------------------------------------------\n");
	#endif
	buildtree(l, layer-1);
	buildtree(r, layer-1);
}
//	 híváskor:|-a fa     
//			  |			|-car  
//			  |			|		|-pointer a stringre ahonnan olvasunk
//			  |			|		|			  -int ahova gyüjtjük a kódot (1='.' | 2='-')
//			  |			|		|			  |		pl 0112 = -..
//			  V			V		V 			  V 	pl 1122 = --..
//addtotree  [n]       [c]     [morse]       kod        i <- hanyadok sorban vagyunk, a kodhoz kell
void addtotree(Node * n, char c, char * morse, unsigned int mkod, int i)
{
    //debug
    #ifdef DEBUG
    printf("addtotree:\t%p \t%c \t%p \n", n, c, morse);
	#endif
	switch (*morse)
	{
        case '\0':
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
			mkod += (int)pow((double)10, (double)i); // 1 * 10^i vagyis 1-el növeljük az i-edik jegyet 
			addtotree(n->left,  c,  morse+1,    mkod,   i+1); //pointer magic
			break;
		case '-':   //right
            mkod += 2 * (int)pow((double)10, (double)i); //2-el növeljük az i-edik jegyet
			addtotree(n->right, c,  morse+1,    mkod,   i+1); //pointer magic
			break;
	}
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
			decodemorse(n->left,   morse+1); //pointer magic
			break;
		case '-':   //right
			decodemorse(n->right,  morse+1); //pointer magic
			break;
	}
}

int encode = 0;

void encodemorse(Node * n, char c, char * ret)
{
    #ifdef DEBUG
    printf("encodemorse:\t%p\t%c=?=%c\n", n, c, n->caracter);
    printf("----------------------------------------------\n");
	#endif
    if (n->caracter == c)
    {
        #ifdef DEBUG
        printf("encode returned %d\n", n->kod);
        #endif
        //encode the int into a stringre
        
        //int length = ((n->kod) / 10) + 1;
        /*
        int length = 0;
        int a = n->kod;
        while (a != 0)
        {
            a /= 10;
            length++;
        }
        */
        #ifdef DEBUG
        //printf("length = %d\n", length);
        #endif
        int a = n->kod;
        int i = 0;
        //for (; i < length; ++i)
        while (a != 0)
        {
            if (a % 2 == 0)   // osztható e kettővel, egye akkor az első számjegye 2 vagyis -
            {
                //encod[i] = '-';
                ret[i] = '-';
            }
            else
            {
                //encod[i] = '.';
                ret[i] = '.';
            }  
            a /= 10;
            i++;
            //printf("A=%d, I = %d\n", a, i);
        }
        //encod[i] = '\0';
        ret[i] = '\0';
        //printf("encod = %p\n", encod);
        //ret = encod;
        //printf("ret = %p\n", ret);
    }
    //printf("----------------------------------------------\n");
    if (n->left != NULL && n->right != NULL)
    {
        encodemorse(n->left,  c, ret);
        encodemorse(n->right, c, ret);
    }
    return;
}

void add_morse(Node * n, char * morse, char c)
{
    
    //debug
    //printf("add_morse:\t%p\t%p\t%c\n", n, morse, c);
    //printf("----------------------------------------------\n");
    //printf("\t\t[n]\t\t\t[c]\t[morse]\t\t\t[mkod]\t\t\t[mkod->s]\t[i]\n");
    //adjuk meg melyin hosszú a morse
    addtotree(n, c, morse, 0, 0);
    //printf("----------------------------------------------\n");
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
    //printf("argv[0] = %s,argv[1] = %s,argv[2] = %s\n", argv[0], argv[1],argv[2]);
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
    
    //create tree
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
            encodemorse(r, *token, enc);
            printf("%s ", enc);
        }
        token = strtok(NULL, delimiter);
    }

	return 0;

    /*
    printf("Elindult\n\n");
    node * r = createtree(5); //morsetree
    printf("fa kesz\n\n");

    Node *n1 = createnode('A');
    Node *n3 = createnode('C');
    Node *n2 = createnode('B');
    Node *n4 = createnode('D');
    n1->left=n2;
    n1->right=n3;
    n2->left=n4;

    printf("%p\n", n1->left->left);
    
    printf("Before calling add_morse: %p\n", r);
 
    add_morse(r, ".-",      'A');
    add_morse(r, "-...",    'B' );
    add_morse(r, "-.-.",    'C' );
    add_morse(r, "-..",     'D');
    add_morse(r, ".",       'E');
    add_morse(r, "..-.",    'F');
    add_morse(r, "--.",     'G');
    add_morse(r, "....",    'H');
    add_morse(r, "..",      'I');
    add_morse(r, ".---",    'J');
    add_morse(r, "-.-",     'K');
    add_morse(r, ".-..",    'L');
    add_morse(r, "--",      'M');
    add_morse(r, "-.",      'N');
    add_morse(r, "---",     'O');
    add_morse(r, ".--.",    'P');
    add_morse(r, "--.-",    'Q');
    add_morse(r, ".-.",     'R');
    add_morse(r, "...",     'S');
    add_morse(r, "-",       'T');
    add_morse(r, "..-",     'U');
    add_morse(r, "...-",    'V');
    add_morse(r, ".--",     'W');
    add_morse(r, "-..-",    'X');
    add_morse(r, "-.--",    'Y');
    add_morse(r, "--..",    'Z');

    
    
    
    
    char * kod = "..";
    char i1 = decodemorse(r, kod);
    printf("This should be a b => %c\n", i1);
    
    printf("%c\n", r->caracter);
    printf("%c\n", r->right->right->caracter);
    printf("%c\n", r->left->left->caracter);
    
    printtree(r);
    
    
    char i2[5];
    encodemorse(r, 'I', i2);
    printf("This should be \"..\" => %s\n", i2);
    */
}