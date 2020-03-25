/***************************************************************************
 *  Description:
 *  
 *  Arguments:
 *
 *  Returns:
 *
 *  History: 
 *  Date        Name        Modification
 *  2020-03-24  Jason Bacon Begin
 ***************************************************************************/

#include <stdio.h>
#include <sysexits.h>
#include <getopt.h>
#include <stdlib.h>
#include "basic-stats.h"

int     main(int argc, char *argv[])

{
    /* options descriptor */
    static struct option longopts[] = {
	{ "median",     required_argument,  NULL,           'm' },
	{ "average",    required_argument,  NULL,           'a' },
	{ 0,            0,                  0,              0 }
    };
    int     ch;
    
    while ((ch = getopt_long(argc, argv, "m:a:", longopts, NULL)) != -1)
    {
	switch(ch)
	{
	    case    'm':
		median();
		break;
	    case    'a':
		average();
		break;
	    default:
		usage(argv);
	}
    }
    return EX_OK;
}


void    usage(char *argv[])

{
    fprintf(stderr, "Usage: %s [-m|--median column] [-a|--average column]\n",
	    argv[0]);
    exit(EX_USAGE);
}


double  median(void)

{
    double  *list,
	    median;
    size_t  array_size = INITIAL_LIST_SIZE,
	    list_size;
    
    list = MALLOC(array_size, double);
    for (list_size = 0; scanf("%lf", list + list_size) == 1; ++list_size)
    {
	if ( list_size == array_size - 1 )
	{
	    array_size += array_size;
	    list = REALLOC(list, array_size, double);
	}
    }
    qsort(list, list_size, sizeof(double),
	  (int (*)(const void *, const void *))double_cmp);
    if ( list_size % 2 == 0 )
	median = (list[list_size / 2 - 1] + list[list_size / 2]) / 2.0;
    else
	median = list[list_size / 2];
    printf("%f\n", median);
    return median;
}


int     double_cmp(const double *d1, const double *d2)

{
    if ( *d1 > *d2 )
	return 1;
    else if ( *d1 < *d2 )
	return -1;
    else
	return 0;
}


void    average(void)

{
    size_t  list_size;
    double  num,
	    total;
    
    for (list_size = 0, total = 0.0; scanf("%lf", &num) == 1; ++list_size)
	total += num;
    printf("%f\n", total / list_size);
}
