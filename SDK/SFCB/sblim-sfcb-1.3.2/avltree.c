/*
 * avltree.c
 *
 * (C) Copyright IBM Corp. 2005
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:       Sebastian Seyrich <seyrich@de.ibm.com>
 *
 * Description:
 * AVL-Tree Datatype
 *
 * 
 *
*/


#include <stdlib.h>
#include "avltree.h"


static void freeAvl(AvlTree **t);
static void *insertAvl(AvlTree *this, void *item);
static void *findAvl(AvlTree *this, void *item);
static void *findMinAvl(AvlTree *this);
static void *deleteAvl(AvlTree *this, void *item);
static void *deleteMinAvl(AvlTree *this);


AvlTree *newAvlTree(int (* compar)(const void *, const void *)) {
    AvlTree *this;

    this = (AvlTree*)calloc(1,sizeof(AvlTree));
    this->root = NULL;
    this->compar = compar;
    this->stack = malloc(AVL_STACK_SIZE * sizeof(AvlNode *));
    this->pathInfo = malloc(AVL_STACK_SIZE * sizeof(signed char));
    this->n = 0;
	this->free = freeAvl;
    this->insert = insertAvl;
    this->find = findAvl;
    this->findMin = findMinAvl;
    this->delete = deleteAvl;
    this->deleteMin = deleteMinAvl;
    return this;
}


static void freeAvl(AvlTree **this) {	
	//printf("1\n");
    AvlTree * t = *this;
   // printf("2\n");
	    
    int tos;
    AvlNode *p, **stack;
	//printf("3\n");
    if(t->root) {
        //printf("4\n");
    	stack = t->stack;
		stack[0] = t->root;
        tos = 1;
        //printf("5\n");
    
		while(tos) {
			//printf("6\n");
	    
		    p = stack[--tos];
		    if(p->left) {
				stack[tos++] = p->left;
		    }
		    if(p->right) {
				stack[tos++] = p->right;
		    }
		    free(p);
		}
    }
//printf("7\n");
    
    free(t->stack);
//printf("8\n");
    free(t->pathInfo);
//printf("9\n");
    free(t);
//printf("10\n");
    
}


static void *insertAvl(AvlTree *this, void *item) {
	
    int (* compar)(const void *, const void *);
    int cmpres;
    void *res;
    AvlNode *x, *p, *q, *r, **attachX;
    AvlNode **stack;
    int tos, stackN;
    signed char *pathInfo;

    compar = this->compar;

    stack = this->stack;
    pathInfo = this->pathInfo;
    tos = 0;

    if((p = this->root)) {
        for(;;) {
		    stack[tos] = p;
		    cmpres = compar(item, p->item);
		    if(cmpres < 0) {
				pathInfo[tos] = -1;
		        tos++;
	            p = p->left;
				if(!p) {
				    attachX = &stack[tos-1]->left;
			    	break;
				}
		    }
		    else if(cmpres > 0) {
				pathInfo[tos] = 1;
		        tos++;
				p = p->right;
				if(!p) {
			    	attachX = &stack[tos-1]->right;
			    	break;
				}
		    }
		    else {
				return p->item;
		    }
		}
    }
    else {
		attachX = &this->root;
    }

    res = NULL;
    x = malloc(sizeof(AvlNode));
    x->left = x->right = NULL;
    x->balance = 0;
    x->item = item;

    *attachX = x;
    this->n++;
    	
    stackN = tos;
    while(tos) {
		p = stack[--tos];
		if(p->balance == 0) {
		    p->balance += pathInfo[tos];
		}
		else {
		    p->balance += pathInfo[tos];
		    if(p->balance == 0) {
				break;  
	    	}
			else {
				q = stack[tos + 1];
				if(pathInfo[tos] == pathInfo[tos + 1]) {

					if(pathInfo[tos] == 1) {
						p->right = q->left;
						q->left = p;
		    		}
		    		else {
						p->left = q->right;
						q->right = p;
		    		}

		    		p->balance = q->balance = 0;

		    		if(tos != 0) {
						if(pathInfo[tos-1] == 1) {
                            stack[tos-1]->right = q;
						}
						else {
						    stack[tos-1]->left = q;
						}
		    		}
		    		else {
						this->root = q;
		    		}
			
		    		break;
				}
				else {

			 	   if(tos + 2 != stackN) {
						r = stack[tos + 2];
			    	}
			    	else {
						r = x;
			    	}
			    
			    	if(pathInfo[tos] == 1) {
						q->left = r->right;
						p->right = r->left;
						r->right = q;
						r->left = p;  
						if(r->balance == 1) {
				    		p->balance = -1;
				    		q->balance = 0;
						}
						else if(r->balance == -1) {
				    		p->balance = 0;
				    		q->balance = 1;
						}
						else {
				    		p->balance = q->balance = 0;
						}
			    	}
			    	else {
						q->right = r->left;
						p->left = r->right;
						r->left = q;
						r->right = p;
						if(r->balance == 1) {
				   			p->balance = 0;
				    		q->balance = -1;
						}
						else if(r->balance == -1) {
				    		p->balance = 1;
				    		q->balance = 0;
						}
						else {
	                    	p->balance = q->balance = 0;
						}
			    	}
			    	r->balance = 0;
	
			    	if(tos != 0) {
						if(pathInfo[tos-1] == 1) {
	                    	stack[tos-1]->right = r;
						}
						else {
				    		stack[tos-1]->left = r;
						}
			    	}
			    	else {
						this->root = r;
			    	}
			    
			    	break;
				}
			}
		}
    }

    
    return res;
}


static void *findAvl(AvlTree *this, void *item) {
    int (* compar)(const void *, const void *);
    int cmpres;
    AvlNode *p, *nextP;

    if((nextP = this->root)) {
         compar = this->compar;
	 
	    do {
		    p = nextP;
		    cmpres = compar(item, p->item);
		    if(cmpres < 0) {
	        	nextP = p->left;
		    }
		    else if(cmpres > 0) {
				nextP = p->right;
		    }
		    else {
				return p->item;
		    }
		} while(nextP);
    }

    return NULL;
}


void *findMinAvl(AvlTree *this) {
    AvlNode *p, *nextP;


    if((nextP = this->root)) {
    	do {
	    	p = nextP;
            nextP = p->left;
		} while(nextP);
    }
    else {
		return NULL;
    }
    
    return p->item;
}

static void *deleteAvl(AvlTree *this, void *item) {
    void *res;
    int (* compar)(const void *, const void *);
    int cmpres;
    AvlNode *p, *prevP;
    AvlNode *m, *prevM;
    AvlNode *q, *r;
    AvlNode **stack;
    int tos, stackP;
    signed char *pathInfo;


    if(!(p = this->root)) return NULL;    
    
    compar = this->compar;

    stack = this->stack;
    pathInfo = this->pathInfo;
    tos = 0;
    
    for(;;) {
		cmpres = compar(item, p->item);
		if(cmpres < 0) {
		    pathInfo[tos] = -1;
		    stack[tos++] = p;
		    p = p->left;
		}
		else if(cmpres > 0) {
		    pathInfo[tos] = 1;
		    stack[tos++] = p;
		    p = p->right;
		}
		else {
		    break;
		}
		if(!p) {
		    return NULL;
		}
    }


    if(!p->left) {
		if(tos == 0) {
	    	this->root = p->right;
		}
		else {
		    prevP = stack[tos-1];
		    if(p == prevP->left) {
		        prevP->left = p->right;
		    }
		    else {
		        prevP->right = p->right;
		    }
		}
    }
    else if(!p->right) {
		if(tos == 0) {
            this->root = p->left;
		}
		else {
		    prevP = stack[tos-1];
		    if(p == prevP->left) {
				prevP->left = p->left;
		    }
		    else {
				prevP->right = p->left;
		    }
		}
    }
    else {
    
		pathInfo[tos] = 1;
		stack[tos] = p;
		stackP = tos++;
	
		m = p->right;  
        do {
		    pathInfo[tos] = -1;
		    stack[tos++] = m;
		    m = m->left;
		} while(m);
		m = stack[--tos];

	
		if(stackP == 0) {
            this->root = m;
		}
        else {
		    prevP = stack[stackP - 1];
		    if(p == prevP->left) {
				prevP->left = m;
	    	}
	    	else {
				prevP->right = m;
	    	}
		}

		prevM = stack[tos-1];
        if(prevM != p) {
		    prevM->left = m->right;
		    m->right = p->right;
		}
        m->left = p->left;

		m->balance = p->balance;
		stack[stackP] = m;
    }

    res = p->item;
    free(p);
    this->n--;

    while(tos) {
		p = stack[--tos];
		if(p->balance == 0) {
		    p->balance -= pathInfo[tos];
		    break;
		}
		else {
	   		p->balance -= pathInfo[tos];

	    	if(p->balance != 0) {
				if(pathInfo[tos] == 1) {
		    		q = p->left;
				}
				else {
		    		q = p->right;
				}
				if(pathInfo[tos] != q->balance) {
		    
		   			if(pathInfo[tos] != 1) {
						p->right = q->left;
						q->left = p;
		    		}
		    		else {
						p->left = q->right;
						q->right = p;
		   			}

		    		if(tos != 0) {
						if(pathInfo[tos-1] == 1) {
                            stack[tos-1]->right = q;
						}
						else {
			   				 stack[tos-1]->left = q;
						}
		    		}
		    		else {
						this->root = q;
		    		}

		    		if(q->balance == 0) {
						p->balance = p->balance > 0 ? 1 : -1;
						q->balance = -p->balance;
						break;  
		    		}
		   			else {
                      	p->balance = q->balance = 0;
		    		}
		    
		    
				}
				else {
     
		   			if(pathInfo[tos] != 1) {
						r = q->left;
						q->left = r->right;
						p->right = r->left;
						r->right = q;
						r->left = p;  
						if(r->balance == 1) {
						    p->balance = -1;
						    q->balance = 0;
						}
						else if(r->balance == -1) {
						    p->balance = 0;
						    q->balance = 1;
						}
						else {
						    p->balance = q->balance = 0;
						}
		    		}
		    		else {
						r = q->right;
						q->right = r->left;
						p->left = r->right;
						r->left = q;
						r->right = p;
						if(r->balance == 1) {
						    p->balance = 0;
						    q->balance = -1;
						}
						else if(r->balance == -1) {
						    p->balance = 1;
						    q->balance = 0;
						}
						else {
			                            p->balance = q->balance = 0;
						}
		    		}
		    		r->balance = 0;
		    	
		    		if(tos != 0) {
						if(pathInfo[tos-1] == 1) {
                            stack[tos-1]->right = r;
						}
						else {
			    			stack[tos-1]->left = r;
						}
		    		}
		    		else {
						this->root = r;
		    		}
		    
		    
				}
	    	}
		}
    }
       
    return res;
}


static void *deleteMinAvl(AvlTree *this) {
    void *res;
    AvlNode *p;
    AvlNode *q, *r;
    AvlNode **stack;
    int tos;

    stack = this->stack;
    tos = 0;
    
    if((p = this->root)) {
        do {
	    	stack[tos++] = p;
            p = p->left;
		} while(p);
		p = stack[--tos];
    }
    else {
		return NULL;
    }

    if(tos == 0) {
        this->root = p->right;
    }
    else {
        stack[tos-1]->left = p->right;
    }
    res = p->item;
    free(p);
    this->n--;

    while(tos) {
		p = stack[--tos];
		if(p->balance == 0) {
		    p->balance++;;
		    break; 
		}
		else {
		    p->balance++;
	
		    
		    if(p->balance != 0) {
			
				q = p->right; 
				if(q->balance != -1) {
			   		p->right = q->left;
			    	q->left = p;
	
			    	if(tos != 0) {
						stack[tos-1]->left = q;
			    	}
			    	else {
						this->root = q;
			    	}
	
			    	if(q->balance == 0) {
						p->balance = 1;
						q->balance = -1;
						break;  
			    	}
			    	else {
	                	p->balance = q->balance = 0;
			    	}
			    
			    
				}
				else {
	            
			    	r = q->left;
				    q->left = r->right;
				    p->right = r->left;
				    r->right = q;
				    r->left = p;  
				    if(r->balance == 1) {
						p->balance = -1;
						q->balance = 0;
				    }
				    else if(r->balance == -1) {
						p->balance = 0;
						q->balance = 1;
				    }
				    else {
						p->balance = q->balance = 0;
				    }
				    r->balance = 0;
				    
				    if(tos != 0) {
						stack[tos-1]->left = r;
				    }
				    else {
						this->root = r;
				    }
			    
				}
	    	}
		}
    }    

    
    return res;
}


