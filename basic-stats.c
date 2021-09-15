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


int     add_function(function_list_t *flist, function_t new_function,
		     int *c, char *argv[])

{
    int             count = flist->count;
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
	flist->rows[count] = n;
	flist->cols[count] = 0;
    }
    else if ( strcmp(argv[*c + 1], "--col") == 0 )
    {
	flist->cols[count] = n;
	flist->rows[count] = 0;
    }
    else
	usage(argv);
    flist->functions[count] = new_function;
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
	if ( flist->functions[c] == MEDIAN )
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
	
	printf("%16.2f", x);
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
		printf("%s %u mean: %f\n", row_col_name, row_col_value,
			flist->sum[c] / flist->n[c]);
		break;
	    
	    case    MEDIAN:
		printf("%s %u median: %f\n", row_col_name, row_col_value,
			median(flist->nums[c], flist->nums_count[c]));
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
	
	case MEDIAN:
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
    }
}


double  median(double list[], size_t list_size)

{
    double  median;

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
