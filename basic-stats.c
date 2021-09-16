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
#include <xtend/mem.h>
#include "basic-stats.h"

// http://makemeanalyst.com/basic-statistics-for-data-analysis/

int     main(int argc, char *argv[])

{
    char    *delims = " \t";
    int     c;
    function_list_t flist;
    
    if ( argc < 4 )
	usage(argv);
    
    function_list_init(&flist);
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
	else if ( strcmp(argv[c],"quantile") == 0 )
	    add_function(&flist, QUANTILE, &c, argv);
	else if ( strcmp(argv[c],"median") == 0 )
	    add_function(&flist, MEDIAN, &c, argv);
	else if ( strcmp(argv[c],"mean") == 0 )
	    add_function(&flist, MEAN, &c, argv);
	else if ( strcmp(argv[c],"population-variance") == 0 )
	    add_function(&flist, POPULATION_VARIANCE, &c, argv);
	else if ( strcmp(argv[c],"sample-variance") == 0 )
	    add_function(&flist, SAMPLE_VARIANCE, &c, argv);
	else if ( strcmp(argv[c],"population-stddev") == 0 )
	    add_function(&flist, POPULATION_STDDEV, &c, argv);
	else if ( strcmp(argv[c],"sample-stddev") == 0 )
	    add_function(&flist, SAMPLE_STDDEV, &c, argv);
	else if ( strcmp(argv[c],"mode") == 0 )
	    add_function(&flist, MODE, &c, argv);
	else if ( strcmp(argv[c],"range") == 0 )
	    add_function(&flist, RANGE, &c, argv);
	else if ( strcmp(argv[c],"iq-range") == 0 )
	    add_function(&flist, IQ_RANGE, &c, argv);
	else if ( strcmp(argv[c],"box-plot") == 0 )
	    add_function(&flist, BOX_PLOT, &c, argv);
	else if ( strcmp(argv[c],"z-scores") == 0 )
	    add_function(&flist, Z_SCORES, &c, argv);
	else
	    usage(argv);
    }
    
    return process_data(&flist, delims);
}


void    usage(char *argv[])

{
    fprintf(stderr, "Usage: %s [--delim string] function1 [param1] --row row1 | --col col1 \\\n"
	    "        [function2 [param2] --row row2 | --col col2 ...]\n", argv[0]);
    fprintf(stderr, "\nAt least one of the following functions is required:\n\n");
    fprintf(stderr,"  mean\n");
    fprintf(stderr,"  quantile N (N = number of divisions)\n");
    fprintf(stderr,"  median (same as quantile 2)\n");
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


int     add_function(function_list_t *flist, function_t new_function,
		     int *c, char *argv[])

{
    int             count = flist->count;
    unsigned long   row_col;
    char            *end;
    
    if ( *c == MAX_FUNCTIONS )
    {
	fprintf(stderr, "Error: Maximum functions for one run is %u.\n",
		MAX_FUNCTIONS);
	exit(EX_USAGE);
    }
    
    flist->functions[count] = new_function;
    switch (new_function)
    {
	case    QUANTILE:
	    flist->quantile[count] = strtoul(argv[++*c], &end, 10);
	    if ( *end != '\0' )
	    {
		fprintf(stderr, "Invalid quantile partition count: %s\n", argv[*c]);
		usage(argv);
	    }
	    break;
	
	case    MEDIAN:
	    flist->functions[count] = QUANTILE;
	    flist->quantile[count] = 2;
	    break;
	
	default:
	    break;
    }
    
    row_col = strtoul(argv[*c + 2], &end, 10);
    if ( *end != '\0' )
    {
	fprintf(stderr, "Invalid row/col: %s\n", argv[*c + 2]);
	usage(argv);
    }
    if ( strcmp(argv[*c + 1], "--row") == 0 )
    {
	flist->rows[count] = row_col;
	flist->cols[count] = 0;
    }
    else if ( strcmp(argv[*c + 1], "--col") == 0 )
    {
	flist->cols[count] = row_col;
	flist->rows[count] = 0;
    }
    else
	usage(argv);
    ++flist->count;
    *c += 2;
    return 0;
}


int     process_data(function_list_t *flist, const char *delims)

{
    unsigned    row,
		col,
		row_col_value = 0;
    char        buff[MAX_DIGITS + 1],
		*end,
		*row_col_name = "";
    size_t      len,
		c;
    double      x;
    int         ch;

    for (c = 0; c < flist->count; ++c)
    {
	if ( flist->functions[c] == QUANTILE )
	{
	    // FIXME: Check malloc
	    flist->nums[c] = xt_malloc(1024, sizeof(*flist->nums[c]));
	    flist->array_size[c] = 1024;
	}
	else if ( (flist->functions[c] == POPULATION_VARIANCE) ||
		  (flist->functions[c] == SAMPLE_VARIANCE) )
	    flist->temp_file[c] = tmpfile();
    }
    
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
	
	printf("%8.2f", x);
	for (c = 0; c < flist->count; ++c)
	{
	    /*
	     *  Unspecified rows and cols are set to 0 in flist and
	     *  row and col begin at 1, so checking for equality is all we
	     *  need to do.
	     */
	    
	    if ( row == flist->rows[c] )
	    {
		process_val(flist, c, x);
	    }
	    
	    if ( col == flist->cols[c] )
	    {
		process_val(flist, c, x);
	    }
	}
	
	// End of input line?
	if ( ch == '\n' )
	{
	    ++row;
	    col = 1;
	    putchar('\n');
	}
	else
	    ++col;
    }
    
    /*
     *  Finalize and print results
     */
    
    for (c = 0; c < flist->count; ++c)
    {
	if ( flist->rows[c] != 0 )
	{
	    row_col_name = "Row";
	    row_col_value = flist->rows[c];
	}
	else
	{
	    row_col_name = "Col";
	    row_col_value = flist->cols[c];
	}
	switch(flist->functions[c])
	{
	    case    MEAN:
		printf("%s %u mean           %f\n", row_col_name, row_col_value,
			flist->sum[c] / flist->n[c]);
		break;
	    
	    case    QUANTILE:
		quantiles(flist, c, row_col_name, row_col_value);
		break;
	    
	    default:
		break;
	}
    }
    return EX_OK;
}


void    process_val(function_list_t *flist, size_t c, double x)

{
    switch(flist->functions[c])
    {
	case MEAN:
	    flist->sum[c] += x;
	    ++flist->n[c];
	    break;
	
	case QUANTILE:
	    // FIXME: Check malloc
	    if ( flist->nums_count[c] == flist->array_size[c] )
		flist->nums[c] = xt_realloc(flist->nums[c],
			       flist->array_size[c] *= 2,
			       sizeof(*flist->nums[c]));
	    flist->nums[c][flist->nums_count[c]] = x;
	    ++flist->nums_count[c];
	    break;
	
	default:
	    break;
    }
}


void    function_list_init(function_list_t *flist)

{
    size_t  c;
    
    flist->count = 0;
    for (c = 0; c < MAX_FUNCTIONS; ++c)
    {
	flist->rows[c] = 0;
	flist->cols[c] = 0;
	flist->n[c] = 0;
	flist->sum[c] = 0.0;
	flist->nums[c] = NULL;
	flist->array_size[c] = 0;
	flist->nums_count[c] = 0;
	flist->temp_file[c] = NULL;
	flist->quantile[c] = 0;
    }
}


void    quantiles(function_list_t *flist, size_t c,
		  const char *row_col_name, unsigned row_col_value)

{
    double  *list = flist->nums[c],
	    quantile,
	    p,
	    a;
    size_t  list_size = flist->nums_count[c],
	    partitions = flist->quantile[c],
	    k;
    
    qsort(list, list_size, sizeof(double),
	  (int (*)(const void *, const void *))double_cmp);
    
    printf("%s %u low            %f\n", row_col_name, row_col_value, list[0]);
    for (c = 1; c < partitions; ++c)
    {
	/*
	 *  Using Wikipedia quartile method 4, which can be generalized
	 *  to any quantile
	 */
	p = 1.0 / partitions * c;
	k = p * (list_size + 1);
	a = p * (list_size + 1) - k;
	//printf("p = %f  k = %zu  a = %f  ", p, k, a);
	// Method is 1-based, so adjust for C arrays
	quantile = list[k - 1] + a * (list[k] - list[k - 1]);
	printf("%s %u quantile(%0.2f) %f\n", row_col_name, row_col_value,
		p, quantile);
    }
    printf("%s %u high           %f\n", row_col_name, row_col_value, list[list_size - 1]);
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


double  variance(variance_t variance_adjust)

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
    return total / (list_size - variance_adjust);
}
