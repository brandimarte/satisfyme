/**  *******************************************************  **/
/**                     ** SatisfyMe! **                      **/
/**                                                           **/
/**                        05/05/2012                         **/
/**                                                           **/
/**   Written by: Pedro Brandimarte (brandimarte@gmail.com)   **/
/**                                                           **/
/**  *******************************************************  **/

#define max_name 10

/**  *****************  Type definitions  ******************  **/

/* Each row from the input matrix is represented */
/* by a linked list of structures type 'node',   */
/* corresponding to the non zero row values.     */
typedef struct node_struct {
   struct node_struct *left, *right; /* predecessor and successor at row */
   struct node_struct *up, *down; /* predecessor and successor at column */
   struct col_struct *col; /* column containing this node */
} node;

/* Each column from the input matrix is      */
/* represented by a structure type 'column'. */
typedef struct col_struct {
   node head; /* list head (root) */
   int len; /* actual number of items in this column list (not including head) */
   char name[max_name]; /* symbolic identifier of the column for printing */
   struct col_struct *prev, *next; /* neighbor columns */
} column;

/* Structure to point and identify the problem rows. */
typedef struct line_struct {
   node *nodeR; /* first node of a row */
   char nameR[max_name]; /* row name */
} line;

/* Structure to store solutions. */
typedef struct sol_struct {
   line *rowSol; /* array of solution rows */
   int level; /* amount of rows from solution */
   struct sol_struct *next; /* pointer to the next solution */
} solution;

/**  ****************  Function prototypes  ****************  **/

/* Get command line options. */
void getOptions (int argc, char *argv[], int *opt);

/* Read and create structure for an exact cover problem. */
void readECP (column *root, line *row, int n, int m);

/* Search for solutions for an exact cover set problem. */
void searchECP (int level, column *root, column *clmn,
		node **partial, line *row, int *cont, solution *sol);

/* Read k-CNF problem and create data structure for GCP. */
void readGCP (column *root, line *row, int k, int n, int m);

/* Search for solutions of a generalized cover problem. */
void searchGCP (int level, int k, column *root, column *clmn,
		node **partial, line *row, int *cont, solution *sol);

/* Remove column 'c' and corresponding rows. */
void cover (column *c);

/* Reinsert column 'c' and corresponding rows. */
void uncover (column *c);

/* Remove column 'c', if not yet removed. */
void coverVar (column *c, int k);

/* Reinsert column 'c', case it has 'k - 1' elements. */
void uncoverVar (column *c, int k);

/* Insert solution in the solutions linked list. */
void insertSol (int k, node **partial, line *row, solution *sol);

/* Print solutions if any, otherwise prints '0'. */
void writeSol (int cont, solution sol, int opt);

/*  Free memory from the solutions linked list 'sol'. */
void freeSol (solution *sol);

/* Allocates a block of bytes if there are     */
/* enough memory, otherwise exits the program. */
void *checkMalloc (unsigned int nbytes);
