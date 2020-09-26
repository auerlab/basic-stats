/***************************************************************************
 *  Description:
 *      Command-line tool for quick, basic statistics.  Basic Unix tools
 *      don't facilitate stats very well and going to R for every little
 *      thing can really cramp your workflow, especially when using non-R
 *      tools for most of the work.
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
    // http://makemeanalyst.com/basic-statistics-for-data-analysis/
    static struct option longopts[] = {
	{ "help",               required_argument,  NULL,           'h' },
	{ "median",             required_argument,  NULL,           'm' },
	{ "average",            required_argument,  NULL,           'a' },
	{ "population-variance",required_argument,  NULL,           'V' },
	{ "sample-variance",    required_argument,  NULL,           'v' },
	{ "stddev",             required_argument,  NULL,           's' },
	{ "mode",               required_argument,  NULL,           'o' },
	{ "range",              required_argument,  NULL,           'r' },
	{ "iq-range",           required_argument,  NULL,           'i' },
	{ "box-plot",           required_argument,  NULL,           'b' },
	{ "z-scores",           required_argument,  NULL,           'z' },
	{ 0,                    0,                  0,               0  }
    };
    int     ch;
    
    if ( argc < 2 )
	usage(argv);
    
    while ((ch = getopt_long(argc, argv, "m:a:V:", longopts, NULL)) != -1)
    {
	switch(ch)
	{
	    case    'h':
		usage(argv);
	    case    'm':
		median();
		break;
	    case    'a':
		average();
		break;
	    case    'V':
		population_variance();
		break;
	    case    'v':
	    case    's':
	    case    'o':
	    case    'r':
	    case    'i':
	    case    'b':
	    case    'z':
		fprintf(stderr, "Not yet implemented.\n");
		return EX_UNAVAILABLE;
	    default:
		usage(argv);
	}
    }
    return EX_OK;
}


void    usage(char *argv[])

{
    fprintf(stderr, "Usage: %s function column [function column ...]\n", argv[0]);
    fprintf(stderr, "\nAt least one of the following functions is required:\n\n");
    fprintf(stderr,"  --median, -m column\n");
    fprintf(stderr,"  --average, -a column\n");
    fprintf(stderr,"  --population-variance, -V column\n");
    fprintf(stderr,"  --sample-variance, -v column\n");
    fprintf(stderr,"  --stddev, -s column\n");
    fprintf(stderr,"  --mode, -o column\n");
    fprintf(stderr,"  --range, -r column\n");
    fprintf(stderr,"  --iq-range, -i column\n");
    fprintf(stderr,"  --box-plot, -b column\n");
    fprintf(stderr,"  --z-scores, -z column\n");
    fputc('\n', stderr);
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


void    population_variance(void)

{
    size_t  list_size,
	    c;
    double  num,
	    total,
	    mean;
    FILE    *fp;
    
    fp = tmpfile();
    for (list_size = 0, total = 0.0; scanf("%lf", &num) == 1; ++list_size)
    {
	total += num;
	fprintf(fp, "%f\n", num);
    }
    mean = total / list_size;
    
    rewind(fp);
    for (c = 0, total = 0.0; c < list_size; ++c)
    {
	fscanf(fp, "%lf", &num);
	total += (num - mean) * (num - mean);
    }
    printf("%f\n", total / list_size);
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
