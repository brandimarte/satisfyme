/**  *******************************************************  **/
/**                     ** SatisfyMe! **                      **/
/**                                                           **/
/**                        05/05/2012                         **/
/**                                                           **/
/**   Written by: Pedro Brandimarte (brandimarte@gmail.com)   **/
/**                                                           **/
/**  *******************************************************  **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CP.h"


/* *********************************************************** */
/* Check command line options:                                 */
/*  -n: prints the number of attributions satisfying the input */
/*  -N: prints the number and all satisfying attributes (in    */
/*      lexicographical order)                                 */
/*  -C: indicates that the input is an exact cover problem     */
/*      (for debugging/checking) and returns the number and    */
/*      all solutions                                          */
void getOptions (int argc, char *argv[], int *opt)
{
   if (argc == 1) /* no option */
      *opt = 0;
   else if (argv[1][0] == '-' && argv[1][1] == 'n') /* -n */
      *opt = 1;
   else if (argv[1][0] == '-' && argv[1][1] == 'N') /* -N */
      *opt = 2;
   else if (argv[1][0] == '-' && argv[1][1] == 'C') /* -C */
      *opt = 3;
   else {
      fprintf (stderr, "\n Invalid option: %s\n\n", argv[1]);
      exit(EXIT_FAILURE);
   }
   
} /* getOptions */


/* *********************************************************** */
/* Reads from file an exact cover problem from a set of 'n'    */
/* elements from 'm' subsets and create a data structure.      */
void readECP (column *root, line *row, int n, int m)
{
   int i, j;
   char *irow, *c, aux[8];
   column *p, *q;
   node *noh, *cur = NULL;

   /* Create the columns. */
   p = root;
   q = p;
   for (i = 0; i < n; i++) {
      q->next = checkMalloc (sizeof (*q));
      q = q->next;
      q->head.up = q->head.down = &q->head;
      q->len = 0;
      sprintf (q->name, "%d", i + 1);
      q->prev = p;
      p = q;
   }
   /* Close the columns' circular double linked list. */
   q->next = root;
   root->prev = q;

   /* Read and create the rows. */
   irow = checkMalloc ((4 * n + 3) * sizeof (char));
   fgets (irow, 4 * n + 3, stdin); /* first line */

   for (i = 0, q = root->next; i < m; i++) {
      p = q;

      /* Read another line from file. */
      fgets (irow, 4 * n + 3, stdin);
      if (irow[strlen(irow) - 1] != '\n') { /* check if size <= (4 * n + 3). */
	 fprintf (stderr, "\n Size of the row %d is too long!\n\n", i + 1);
	 exit(EXIT_FAILURE);
      }
      c = irow;

      while (*c != '\n') {
   	 if (*c == ' ')
	    c++;
   	 else {
	    /* Read the column name. */
	    for (j = 0; (*c != ' ') && (*c != '\n') && (j < max_name); j++, c++)
	       aux[j] = *c;
	    if (j == max_name) { /* check if the column name overcame max_name. */
	       fprintf (stderr, "\n Column name too long!\n\n");
	       exit(EXIT_FAILURE);
	    }
	    aux[j] = '\0';

	    /* Search the column. */
   	    for (j = 0; (j < n) && (strcmp (aux, p->name) != 0); j++)
   	       p = p->next;
	    if (j == n) {
	       fprintf (stderr, "\n The column %s from row %d does not exists!\n\n", aux, i + 1);
	       exit(EXIT_FAILURE);
	    }

	    /* Create node and refresh links. */
	    noh = p->head.up;
	    noh->down = checkMalloc (sizeof (*noh)); /* create node at the column end */
	    noh = noh->down;
	    noh->up = p->head.up;
	    p->head.up = noh;
	    noh->down = &p->head;
	    noh->col = p;
	    p->len = p->len + 1;

	    if (cur == NULL) { /* there is no nodes in this row yet */
	       cur = noh;
	       cur->left = cur->right = noh;
	    }
	    else { /* refresh row links */
	       noh->right = cur->right;
	       cur->right->left = noh;
	       cur->right = noh;
	       noh->left = cur;
	       cur = noh;
	    }
   	 }

      } /* while (*c != '\n') */

      if (cur == NULL) { /* check if empty row */
	 fprintf (stderr, "\n Row %d is empty!\n\n", i + 1);
	 exit(EXIT_FAILURE);
      }

      /* Assign row array. */
      row[i].nodeR = cur->right;
      sprintf (row[i].nameR, "%d", i + 1);

      cur = NULL;
      q = root->next;

   } /* for (i = 0, q = ... */

   /* Free memory. */
   free (irow);

} /* readECP */


/* *********************************************************** */
/* Recursive function that searches for solutions for an exact */
/* cover set problem (variation from Knuth's "Dancing Links X" */
/* https://www-cs-faculty.stanford.edu/~knuth/programs.html).  */
/* Receives 'root' (head of a circular double linked list of   */
/* type 'column') containing the problem structure and inserts */
/* the solutions if any in the linked list 'sol'.              */
/* The integer 'level' indicate the recursion level (i.e. the  */
/* depth in the search tree).                                  */
/* 'clmn' points to the column being covered.                  */
/* The array 'partial' contain the row addresses of a partial  */
/* solution.                                                   */
/* The array 'lin' contain the names and pointer to all the    */
/* problem rows.                                               */
/* Finally, 'cont' counts the # of solutions found.            */
void searchECP (int level, column *root, column *clmn,
		node **partial, line *row, int *cont, solution *sol)
{
   node *cur, *t, *aux;

   if (root->next == root) { /* solution found */
      (*cont)++; /* update counter */
      insertSol (level, partial, row, sol); /* insert solution */
      
      return ;
   }

   /* Choose the next column. */
   /* PS: since this part is for debugging/checking purposes of the GCP, */
   /* I'm not choosing the column with lesser nodes (which is optimal).  */
   clmn = root->next;
   cover (clmn);

   for (cur = clmn->head.down; cur != &(clmn->head); cur = cur->down) {
      aux = partial[level] = cur;

      /* Cover all the other columns from row 'cur'. */
      for (t = cur->right; t != cur; t = t->right)
   	 cover (t->col);

      /* Recursive call at 'level + 1'. */
      searchECP (level + 1, root, clmn, partial, row, cont, sol);

      /* Reassign previous values. */
      cur = aux;
      clmn = cur->col;

      /* Undo the cover from all other columns from row 'cur'. */
      for (t = cur->left; t != cur; t = t->left)
      	 uncover (t->col);

   }

   /* Undo the cover of column 'clmn' (returns to original structure). */
   uncover (clmn);

} /* searchECP */


/* *********************************************************** */
/* Reads a k-CNF problem (conjunctive normal form, formed by   */
/* "and" of clauses) from file with 'n' variables, 'k'         */
/* literals and 'm' clauses ("or" of literals). Creates the    */
/* data structure representing the k-CNF as a generalized      */
/* cover problem (GCP).                                        */
void readGCP (column *root, line *row, int k, int n, int m)
{
   int i, j, lt, veri;
   column *p, *q;
   node *noh, *cur = NULL;

   /* Create the columns. */
   p = root;
   q = p;
   for (i = 0; i < n; i++) { /* primaries */
      q->next = checkMalloc (sizeof (*q));
      q = q->next;
      q->head.up = q->head.down = &q->head;
      q->len = 0;
      sprintf (q->name, "X%d", i + 1);
      q->prev = p;
      p = q;
   }
   for (i = 0; i < m; i++) { /* secondaries */
      q->next = checkMalloc (sizeof (*q));
      q = q->next;
      q->head.up = q->head.down = &q->head;
      q->len = 0;
      sprintf (q->name, "C%d", i + 1);
      q->prev = p;
      p = q;
   }
   /* Close the columns' circular double linked list. */
   q->next = root;
   root->prev = q;

   /* Read and create the rows. */
   for (i = 0, q = root->next; i < n; i++) {
      for (j = 0; j < 2; j++) { /* the variables can be '0' or '1' (V or F) */
	 p = q;

	 /* Create node and refresh column links. */
	 noh = p->head.up;
	 noh->down = checkMalloc (sizeof (*noh)); /* create node at the column end */
	 noh = noh->down;
	 noh->up = p->head.up;
	 p->head.up = noh;
	 noh->down = &p->head;
	 noh->col = p;
	 p->len = p->len + 1;

	 if (cur == NULL) { /* there is no nodes in this row yet */
	    cur = noh;
	    cur->left = cur->right = noh;
	 }
	 else { /* refresh row links */
	    noh->right = cur->right;
	    cur->right->left = noh;
	    cur->right = noh;
	    noh->left = cur;
	    cur = noh;
	 }

	 if (cur == NULL) { /* check if empty row */
	    fprintf (stderr, "\n Row %d is empty!\n\n", i + 1);
	    exit(EXIT_FAILURE);
	 }

	 /* Assign row array. */
	 row[2*i+j].nodeR = cur->right;
	 sprintf (row[2*i+j].nameR, "%d", j);

	 cur = NULL;

      } /* for (j = 0, j < ... */

      q = q->next; /* next variable corresponds to the next column */

   } /* for (i = 0, q = ... */
   for (i = 0; i < m; i++) { /* clauses reading */
      for (j = 0; j < k; j ++) { /* each clause has k literals */
	 veri = scanf ("%d", &lt);
	 if (veri == 0 || veri == EOF) { /* check for reading error */
	    fprintf (stderr, "\n Error on input reading!\n\n");
	    exit(EXIT_FAILURE);
	 }
	 /* Create node and refresh column links. */
	 noh = q->head.up;
	 noh->down = checkMalloc (sizeof (*noh)); /* create node at the column end */
	 noh = noh->down;
	 noh->up = q->head.up;
	 q->head.up = noh;
	 noh->down = &q->head;
	 noh->col = q;
	 q->len = q->len + 1;

	 /* Refresh row links. */
	 if (lt < 0) /* case: not(X) */
	    cur = row[-2 * lt - 2].nodeR; /* -2 * lt - 2: zero + natural evens */
	 else /* lt > 0,  case: (X) */
	    cur = row[2 * lt - 1].nodeR; /* 2 * lt - 1: natural odds */

	 noh->left = cur->left; /* insert from the left */
	 cur->left->right = noh;
	 cur->left = noh;
	 noh->right = cur;
      } /* for (j = 0; j < k... */
      q = q->next;

   } /* for (i = 0; i < m... */

   veri = scanf ("%d", &lt);
   if (veri != -1 || veri != EOF) { /* check if input is longer */
      fprintf (stderr, "\n Incorrect input format!\n\n");
      exit(EXIT_FAILURE);
   }

} /* readGCP */


/* *********************************************************** */
/* Recursive function that searches for solutions for a k-CNF  */
/* problem (conjunctive normal form, formed by "and" of        */
/* clauses with 'k' literals).                                 */
/* Receives a data structure pointed by 'root' representing    */
/* the k-CNF problem as a generalized cover problem (GCP),     */
/* where the primary columns correspond to the 'n' variables   */
/* (which have to be exactly covered) and the secondary        */
/* columns corresponding to the 'm' ("or" of 'k' literals).    */
/* Therefore, the problem is solved by a variation of Knuth's  */
/* "Dancing Links X" algorithm                                 */
/* (https://www-cs-faculty.stanford.edu/~knuth/programs.html)  */
/* and the solutions, if any, are inserted in the linked list  */
/* 'sol'.                                                      */
/* The integer 'level' indicate the recursion level (i.e. the  */
/* depth in the search tree).                                  */
/* 'clmn' points to the column being covered.                  */
/* The array 'partial' contain the row addresses of a partial  */
/* solution.                                                   */
/* The array 'lin' contain the names and pointers to all the   */
/* problem rows.                                               */
/* Finally, 'cont' counts the # of solutions found.            */
void searchGCP (int level, int k, column *root, column *clmn,
		node **partial, line *row, int *cont, solution *sol)
{
   node *cur, *t, *aux;

   if (root->next == root) { /* solution found */
      (*cont)++; /* update counter */
      insertSol (level, partial, row, sol); /* insert solution */
      
      return ;
   }

   /* Choose the next column. */
   clmn = root->next;
   cover (clmn);

   for (cur = clmn->head.down; cur != &(clmn->head); cur = cur->down) {
      aux = partial[level] = cur;

      /* Cover all the other columns from row 'cur'. */
      for (t = cur->right; t != cur; t = t->right)
	 coverVar (t->col, k);

      /* Recursive call at 'level + 1'. */
      searchGCP (level + 1, k, root, clmn, partial, row, cont, sol);

      /* Reassign previous values. */
      cur = aux;
      clmn = cur->col;

      /* Undo the cover from all other columns from row 'cur'. */
      for (t = cur->left; t != cur; t = t->left)
	 uncoverVar (t->col, k);
   }

   /* Undo the cover of column 'clmn' (returns to original structure). */
   uncover (clmn);

} /* searchGCP */


/* *********************************************************** */
/* Cover the column 'c'. Remove the column head and, from top  */
/* to bottom, remove the rows 'rr' from column 'c',            */
/* corresponding to the 1's from this column.                  */
void cover (column *c)
{
   column *l, *r;
   node *rr, *nn, *uu, *dd;

   /* Remove column 'c' from columns list. */
   l = c->prev; r = c->next;
   l->next = r; r->prev = l;

   /* Remove the rows 'rr', from top to bottom. */
   for (rr = c->head.down; rr != &(c->head); rr = rr->down)
      /* Remove each element 'nn' from row 'rr', from left to right. */
      for (nn = rr->right; nn != rr; nn = nn->right) {
	 uu = nn->up; dd = nn->down;
	 uu->down = dd; dd->up = uu;
      }

} /* cover */


/* *********************************************************** */
/* Uncover the column 'c'. Reinsert the column 'c' in the      */
/* reverse order from which if has been covered. Reinsert the  */
/* rows 'rr' from column 'c', from bottom to top, and after    */
/* reinsert the column head on the columns list.               */
void uncover (column *c)
{
   column *l, *r;
   node *rr, *nn, *uu, *dd;

   /* Reinsert the rows 'rr', from bottom to top. */
   for (rr = c->head.up; rr != &(c->head); rr = rr->up)
      /* Reinsert each element 'nn' from row 'rr', from right to left. */
      for (nn = rr->left; nn != rr; nn = nn->left) {
	 uu = nn->up; dd = nn->down;
	 uu->down = dd->up = nn;
      }

   /* Reinsert column 'c' in the columns list. */
   l = c->prev; r = c->next;
   l->next = r->prev = c;

} /* uncover */


/* *********************************************************** */
/* Remove column 'c', if not yet removed, i.e., if it has 'k'  */
/* elements. Otherwise, decreases the amount of elements.      */
void coverVar (column *c, int k)
{
   column *l, *r;

   if (c->len == k) { /* Remove column 'c' from columns list. */     
      l = c->prev; r = c->next;
      l->next = r; r->prev = l;
   }
   c->len--;

} /* coverVar */

/* *********************************************************** */
/* Reinsert column 'c' if it has 'k - 1' elements. Otherwise,  */
/* only increases the amount of elements.                      */
void uncoverVar (column *c, int k)
{
   column *l, *r;

   if (c->len == k - 1) { /* Reinsert column 'c' in columns list. */
      l = c->prev; r = c->next;
      l->next = r->prev = c;
   }
   c->len++;

} /* uncoverVar */


/* *********************************************************** */
/* Insert solution in the solutions linked list. Receive the   */
/* amount 'k' of rows that comprises the solution, an array of */
/* pointers 'partial' with the addresses to these rows, an     */
/* array 'lin' with names and pointers for all the problem     */
/* and a linked list 'sol' of solutions (that is updated).     */
void insertSol (int k, node **partial, line *row, solution *sol)
{
   int i, j;
   solution *s;
   
   s = sol;
   while (s->next != NULL) /* insert new solution at the end */
      s = s->next;
   s->next = checkMalloc (sizeof (*sol));
   s = s->next;
   s->next = NULL;

   /* Assign the rows comprising the solution. */
   s->level = k; /* amount of rows */
   s->rowSol = checkMalloc (k * sizeof (*row));
   j = 0;
   for (i = 0; i < k; i++) { /* find the corresponding row */
      while (partial[i] != row[j].nodeR)
	 j++;
      s->rowSol[i] = row[j];
   }

} /* insertSol */


/* *********************************************************** */
/* Prints '0' if there is no solution. Otherwise:              */
/*  - if 'opt = 0' prints '1';                                 */
/*  - if 'opt = 1' (i.e. '-n') prints the amount of solutions; */
/*  - otherwise (i.e. '-N' or '-C'), prints the amount of      */
/*    solutions and all of them in lexicographical order.      */
void writeSol (int cont, solution sol, int opt)
{
   int i;
   solution *s;

   if (sol.next == NULL)
      printf ("0\n");
   else
      if (opt == 0)
	 printf ("1\n");
      else
	 if (opt == 1)
	    printf ("%d\n", cont);
	 else {
	    printf ("%d\n", cont);
	    for (s = sol.next; s != NULL; s = s->next) {
	       for (i = 0; i < s->level; i++)
		  printf ("%s ", s->rowSol[i].nameR);
	       printf ("\n");
	    }
	 }

} /* writeSol */


/* *********************************************************** */
/* Free memory from the solutions linked list 'sol'.           */
void freeSol (solution *sol)
{
   solution *dead;

   while (sol->next != NULL) {
      dead = sol->next;
      sol->next = dead->next;
      free (dead);
   }

} /* freeSol */


/* *********************************************************** */
/* Allocates a block of bytes if there are enough memory.      */
/* Otherwise returns an error message and exits the program.   */
void *checkMalloc (unsigned int nbytes)
{
   void *ptr;
   ptr = malloc (nbytes);

   if (ptr == NULL) {    
      fprintf (stderr, "\n\n Insufficient memory.\n\n");
      exit (EXIT_FAILURE);
   }

   return ptr;

} /* checkMalloc */
