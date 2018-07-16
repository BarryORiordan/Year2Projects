/*****************************************************
* Skeleton Code for the lab exercise. DO NOT add any 
* more printf statements other than those provided 
* in this skeleton code. Otherwise, your output may 
* not match with the desired output and you will be marked
* down. Only modify the values printing with the given 
* printf statements as appropriate. 
*
* Note: properly indent your code so it looks beautiful
* to read. Also add BRIEF comments explaining your code. 
* Poorly written code will be marked down.
***************************************************/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int main(int argc, char **argv)
{
  char *refStr, *tolStr;

  refStr = tolStr = NULL;

  //Add code to read arguments, in ANY order i.e. -r can come before or after -t  
 







  //Check if arguments were passed in correctly.
  if (argc != 5 || refStr == NULL || tolStr == NULL)
  {
    fprintf(stderr, "Usage: %s -r ref -t tol\n", argv[0]);
    exit(1);
  }

  //Add code to note start time and date and then print it. 
  //Right now printing just a default string





  char * tnd="Wed 15 Oct 2014 19:18:13 IST";
  printf("# Start time and date: %s", tnd );


  // Add rest of the functionality. 













  // The following print statement should be a part of a loop
  // Uncomment it tailor it to your code but ONLY the part that 
  // comes after:  \n",
 
  // fprintf(stdout, "r=%d, c=%d: %.6f\n", r, c, rows[r][c]);



  // output the final count of matches. Again, you may ONLY modify
  // the part of the print statement after:  \n",
  // fprintf(stdout, "Found %d approximate matches.\n", count);



  //finally, print out the end time and date. Right now only printing
  //a default string. 
  printf("# End time and date: %s", tnd);

  return 0;
}
