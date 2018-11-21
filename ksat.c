/**  *******************************************************  **/
/**                     ** SatisfyMe! **                      **/
/**                                                           **/
/**                        05/05/2012                         **/
/**                                                           **/
/**   Written by: Pedro Brandimarte (brandimarte@gmail.com)   **/
/**                                                           **/
/**  *******************************************************  **/
/**                                                           **/
/**  For implementation details refer to the documentation    **/
/**  docs folder.                                             **/
/**                                                           **/
/**  *******************************************************  **/

#include <stdio.h>
#include <stdlib.h>
#include "CP.h"


int main (int argc, char *argv[])
{
   int opt, k, n, m, cont = 0;
   column root; /* root from column list */
   column *clmn = NULL; /* pointer to the column being covered */
   node **partial = NULL; /* array of pointers to store the partial solutions */
   line *row; /* array of pointers to the rows */
   solution sol; /* store the solutions */

   /* Initialization. */
   sol.next = NULL;

   /* Get command line options. */
   getOptions (argc, argv, &opt);

   if (opt == 3) { /* exact cover problem */

      /* Read fist line 'n m', where 'n' = # columns and 'm' = # rows. */
      scanf ("%d%d", &n, &m);

      row = checkMalloc (m * sizeof (*row)); /* row array */
      readECP (&root, row, n, m); /* read and create structure from stdin */

      partial = checkMalloc (m * sizeof (*partial)); /* partial solutions array */
      searchECP (0, &root, clmn, partial, row, &cont, &sol); /* search and store solutions */

      writeSol (cont, sol, opt); /* print solutions if any */

   }
   else { /* k-sat problem */

      /* Read fist line 'k n m', where 'k' = # literals, 'n' = # variables */
      /* (primary columns) and 'm' = # clauses (secondary columns). */
      scanf ("%d%d%d", &k, &n, &m);

      row = checkMalloc (2 * n * sizeof (*row)); /* row array */
      readGCP (&root, row, k, n, m); /* read and create structure from stdin */

      partial = checkMalloc (n * sizeof (*partial)); /* partial solutions array */
      searchGCP (0, k, &root, clmn, partial, row, &cont, &sol); /* search and store solutions */

      writeSol (cont, sol, opt); /* print solutions if any */

   }

   /* Free memory. */
   free (partial);
   free (row);
   freeSol (&sol);

   return (0);

} /* main */


