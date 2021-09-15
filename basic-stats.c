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
#include <string.h>
#include <math.h>
#include <xtend/dsv.h>
#include "basic-stats.h"

// http://makemeanalyst.com/basic-statistics-for-data-analysis/

int     main(int argc, char *argv[])

{
    char    *delims = "\t";
    int     c;
    function_list_t function_list;
    
    if ( argc < 4 )
	usage(argv);
    
    function_list_init(&function_list);
    for (c = 1; c < argc; ++c)
    {
	if ( strcmp(argv[c], "--delims") == 0 )
	{
	    if ( ++c < argc )
		delims = argv[c];
	    else
		usage(argv);
	}
	else if ( strcmp(argv[c],"--help") == 0 )
	    usage(argv);
	else if ( strcmp(argv[c],"median") == 0 )
	    add_function(&function_list, MEDIAN, &c, argv);
	else if ( strcmp(argv[c],"mean") == 0 )
	    add_function(&function_list, MEAN, &c, argv);
	else if ( strcmp(argv[c],"population-variance") == 0 )
	    add_function(&function_list, POPULATION_VARIANCE, &c, argv);
	else if ( strcmp(argv[c],"sample-variance") == 0 )
	    add_function(&function_list, SAMPLE_VARIANCE, &c, argv);
	else if ( strcmp(argv[c],"population-stddev") == 0 )
	    add_function(&function_list, POPULATION_STDDEV, &c, argv);
	else if ( strcmp(argv[c],"sample-stddev") == 0 )
	    add_function(&function_list, SAMPLE_STDDEV, &c, argv);
	else if ( strcmp(argv[c],"mode") == 0 )
	    add_function(&function_list, MODE, &c, argv);
	else if ( strcmp(argv[c],"range") == 0 )
	    add_function(&function_list, RANGE, &c, argv);
	else if ( strcmp(argv[c],"iq-range") == 0 )
	    add_function(&function_list, IQ_RANGE, &c, argv);
	else if ( strcmp(argv[c],"box-plot") == 0 )
	    add_function(&function_list, BOX_PLOT, &c, argv);
	else if ( strcmp(argv[c],"z-scores") == 0 )
	    add_function(&function_list, Z_SCORES, &c, argv);
	else
	    usage(argv);
    }
    
    return process_data(&function_list, delims);
}


void    usage(char *argv[])

{
    fprintf(stderr, "Usage: %s [--delim string] function1 --row row1 | --col col1 \\\n"
	    "        [function2 --row row2 | --col col2 ...]\n", argv[0]);
    fprintf(stderr, "\nAt least one of the following functions is required:\n\n");
    fprintf(stderr,"  median\n");
    fprintf(stderr,"  average\n");
    fprintf(stderr,"  population-variance\n");
    fprintf(stderr,"  sample-variance\n");
    fprintf(stderr,"  population-stddev\n");
    fprintf(stderr,"  sample-stddev\n");
    fprintf(stderr,"  mode\n");
    fprintf(stderr,"  range\n");
    fprintf(stderr,"  iq-range\n");
    fprintf(stderr,"  box-plot\n");
    fprintf(stderr,"  z-scores\n");
    fprintf(stderr,"\nDefault delimiter is TAB.  The delimiter string may be multiple characters.\n");
    exit(EX_USAGE);
}


int     add_function(function_list_t *function_list, function_t new_function,
		     int *c, char *argv[])

{
    int             count = function_list->count;
    unsigned long   n;
    char            *end;
    
    if ( *c == MAX_FUNCTIONS )
    {
	fprintf(stderr, "Error: Maximum functions for one run is %u.\n",
		MAX_FUNCTIONS);
	exit(EX_USAGE);
    }
    
    n = strtoul(argv[*c + 2], &end, 10);
    if ( *end != '\0' )
	usage(argv);
    if ( strcmp(argv[*c + 1], "--row") == 0 )
    {
	function_list->rows[count] = n;
	function_list->cols[count] = 0;
	fprintf(stderr, "%s %lu\n", argv[*c + 1], n);
    }
    else if ( strcmp(argv[*c + 1], "--col") == 0 )
    {
	function_list->cols[count] = n;
	function_list->rows[count] = 0;
    }
    else
	usage(argv);
    function_list->functions[count] = new_function;
    ++function_list->count;
    *c += 2;
    return 0;
}


int     process_data(function_list_t *function_list, const char *delims)

{
    unsigned    row,
		col;
    char        buff[MAX_DIGITS + 1],
		*end;
    size_t      len,
		f;
    double      x;
    int         ch;
    
    fprintf(stderr, "%u functions.\n", function_list->count);
    row = 1, col = 1;
    while ( (ch = dsv_read_field(stdin, buff, MAX_DIGITS, delims, &len)) != EOF )
    {
	x = strtod(buff, &end);
	if ( *end != '\0' )
	{
	    fprintf(stderr, "Invalid number: %s: row %u, col %u\n",
		    buff, row, col);
	    exit(EX_DATAERR);
	}
	for (f = 0; f < function_list->count; ++f)
	{
	    /*
	     *  Unspecified rows and cols are set to 0 in function_list and
	     *  row and col begin at 1, so checking for equality is all we
	     *  need to do.
	     */
	    
	    if ( row == function_list->rows[f] )
	    {
		switch(function_list->functions[f])
		{
		    case MEAN:
			function_list->sum[f] += x;
			++function_list->n[f];
			printf("x = %f, sum = %f, n = %u\n",
				x, function_list->sum[f], function_list->n[f]);
			break;
		    
		    default:
			break;
		}
	    }
	    else if ( col == function_list->cols[f] )
	    {
	    }
	    else
	    {
		fprintf(stderr, "Software error: neither row nor col set.\n");
		exit(EX_SOFTWARE);
	    }
	}
	if ( ch == '\n' )
	    ++row;
	else
	    ++col;
    }
    
    /*
     *  Finalize and print results
     */
    
    for (f = 0; f < function_list->count; ++f)
    {
	switch(function_list->functions[f])
	{
	    case    MEAN:
		printf("Mean: %f\n", function_list->sum[f] / function_list->n[f]);
		break;
	    
	    default:
		break;
	}
    }
    return EX_OK;
}


void    function_list_init(function_list_t *function_list)

{
    size_t  c;
    
    function_list->count = 0;
    for (c = 0; c < MAX_FUNCTIONS; ++c)
    {
	function_list->rows[c] = 0;
	function_list->cols[c] = 0;
	function_list->n[c] = 0;
	function_list->sum[c] = 0.0;
    }
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


double  variance(variance_t variance_type)

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
    return total / (list_size - variance_type);
}


double  average(void)

{
    size_t  list_size;
    double  num,
	    total;
    
    for (list_size = 0, total = 0.0; scanf("%lf", &num) == 1; ++list_size)
	total += num;
    return total / list_size;
}
