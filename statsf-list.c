#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <math.h>
#include <xtend/mem.h>
#include <xtend/dsv.h>
#include "statsf-list.h"
// FIXME: Define usage() somewhere else
#include "basic-stats-protos.h"

int     statsf_list_add_func(statsf_list_t *flist, statsf_code_t new_code,
		     int *c, int argc, char *argv[])

{
    int             count = flist->count;
    unsigned long   row_col;
    char            *end;
    
    if ( *c == SFL_MAX_FUNCTIONS )
    {
	fprintf(stderr, "Error: Maximum functions for one run is %u.\n",
		SFL_MAX_FUNCTIONS);
	exit(EX_USAGE);
    }
    
    STATSF_SET_CODE(&flist->functions[count], new_code);
    switch (new_code)
    {
	case    STATSF_QUANTILE:
	    STATSF_SET_PARTITIONS(&flist->functions[count], strtoul(argv[++*c], &end, 10));
	    if ( *end != '\0' )
	    {
		fprintf(stderr, "Invalid quantile partition count: %s\n", argv[*c]);
		exit(EX_USAGE);
	    }
	    break;
	
	case    STATSF_MEDIAN:
	    // Overwrite median code since it's a special case of quantile
	    STATSF_SET_CODE(&flist->functions[count], STATSF_QUANTILE);
	    STATSF_SET_PARTITIONS(&flist->functions[count], 2);
	    break;
	
	case    STATSF_QUARTILE:
	    // Overwrite quartile code since it's a special case of quantile
	    STATSF_SET_CODE(&flist->functions[count], STATSF_QUANTILE);
	    STATSF_SET_PARTITIONS(&flist->functions[count], 4);
	    break;

	default:
	    break;
    }
    
    if ( argc < *c + 2 )
	usage(argv);
    row_col = strtoul(argv[*c + 2], &end, 10);
    if ( *end != '\0' )
    {
	fprintf(stderr, "Invalid row/col: %s\n", argv[*c + 2]);
	exit(EX_USAGE);
    }
    if ( strcmp(argv[*c + 1], "--row") == 0 )
    {
	STATSF_SET_ROW(&flist->functions[count], row_col);
	STATSF_SET_COL(&flist->functions[count], 0);
    }
    else if ( strcmp(argv[*c + 1], "--col") == 0 )
    {
	STATSF_SET_ROW(&flist->functions[count], 0);
	STATSF_SET_COL(&flist->functions[count], row_col);
    }
    else
	usage(argv);
    ++flist->count;
    *c += 2;
    return 0;
}


int     statsf_list_process_stream(statsf_list_t *flist, FILE *stream,
				   const char *delims, int flags)

{
    unsigned    row,
		col,
		row_col_value = 0;
    char        buff[SFL_MAX_FIELD_LEN + 1],
		*end,
		*row_col_name = "";
    size_t      len,
		c;
    double      x, ss, var, stddev, mean, se;
    int         ch;

    for (c = 0; c < flist->count; ++c)
    {
	if ( STATSF_CODE(&flist->functions[c]) == STATSF_QUANTILE )
	{
	    // FIXME: Check malloc
	    STATSF_SET_NUMS(&flist->functions[c],
		    xt_malloc(SFL_INITIAL_LIST_SIZE,
			      sizeof(*STATSF_NUMS(&flist->functions[c]))));
	    STATSF_SET_ARRAY_SIZE(&flist->functions[c], SFL_INITIAL_LIST_SIZE);
	}
    }
    
    row = 1, col = 1;
    while ( (ch = dsv_read_field(stream, buff, SFL_MAX_FIELD_LEN, delims,
		    &len)) != EOF )
    {
	if ( flags & SFL_FLAG_VERBOSE )
	    printf("%8s", buff);
	for (c = 0; c < flist->count; ++c)
	{
	    /*
	     *  Unspecified rows and cols are set to 0 in flist and
	     *  row and col begin at 1, so checking for equality is all we
	     *  need to do.
	     */
	    
	    if ( (row == STATSF_ROW(&flist->functions[c])) ||
		 (col == STATSF_COL(&flist->functions[c])) )
	    {
		x = strtod(buff, &end);
		if ( *end != '\0' )
		{
		    fprintf(stderr, "Invalid number: %s: row %u, col %u\n",
			    buff, row, col);
		    exit(EX_DATAERR);
		}
		statsf_process_val(&flist->functions[c], x);
	    }
	}
	
	// End of input line?
	if ( ch == '\n' )
	{
	    ++row;
	    col = 1;
	    if ( flags & SFL_FLAG_VERBOSE)
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
	if ( STATSF_ROW(&flist->functions[c]) != 0 )
	{
	    row_col_name = "Row";
	    row_col_value = STATSF_ROW(&flist->functions[c]);
	}
	else
	{
	    row_col_name = "Col";
	    row_col_value = STATSF_COL(&flist->functions[c]);
	}
	mean = STATSF_SUM(&flist->functions[c]) /
		STATSF_NUM_COUNT(&flist->functions[c]);
	switch(STATSF_CODE(&flist->functions[c]))
	{
	    case    STATSF_MEAN:
		printf("%s %u mean           %f\n", row_col_name,
			row_col_value, mean);
		break;
	    
	    case    STATSF_QUANTILE:
		statsf_quantiles(&flist->functions[c],
				 row_col_name, row_col_value);
		break;
	    
	    /*
	     *  Use theorom sum[(x - u)^2] = sum[x^2] - (sum[x])^2 / n
	     */
	    case    STATSF_POP_VAR:
	    case    STATSF_POP_STDDEV:
	    case    STATSF_POP_Z_SCORES:
		ss = statsf_sum_squares(&flist->functions[c]);
		var = ss / STATSF_NUM_COUNT(&flist->functions[c]);
		stddev = sqrt(var);
		printf("%s %u sum-squares    %f\n", row_col_name,
			row_col_value, ss);
		printf("%s %u pop-var        %f\n", row_col_name,
			row_col_value, var);
		printf("%s %u pop-stddev     %f\n", row_col_name,
			row_col_value, stddev);
		printf("%s %u pop-mean       %f\n", row_col_name,
			row_col_value, mean);

		if ( STATSF_CODE(&flist->functions[c]) == STATSF_POP_Z_SCORES )
		{
		    // FIXME: Use accessor for tmp_file
		    rewind(flist->functions[c].tmp_file);
		    printf("%s %u z-scores:\n%16s %16s %16s\n", row_col_name, row_col_value,
			    "Val", "z-score", "CDF = P(x < Val)");
		    while ( fscanf(flist->functions[c].tmp_file, "%lf", &x) == 1 )
			printf("%16f %16f %16f\n", x,
				z_score(x, mean, stddev),
				z_percentile(x, mean, stddev));
		}
		break;
		
	    case    STATSF_SAMPLE_VAR:
	    case    STATSF_SAMPLE_STDDEV:
	    case    STATSF_SAMPLE_STDERR:
	    case    STATSF_SAMPLE_Z_SCORES:
		ss = statsf_sum_squares(&flist->functions[c]);
		var = ss / (STATSF_NUM_COUNT(&flist->functions[c]) - 1);
		stddev = sqrt(var);
		se = sqrt(var) / sqrt(STATSF_NUM_COUNT(&flist->functions[c]));
		printf("%s %u sum-squares    %f\n", row_col_name,
			row_col_value, ss);
		printf("%s %u sample-var     %f\n", row_col_name,
			row_col_value, var);
		printf("%s %u sample-stddev  %f\n", row_col_name,
			row_col_value, stddev);
		printf("%s %u sample-stderr  %f\n", row_col_name,
			row_col_value, se);
		printf("%s %u sample-mean    %f\n", row_col_name,
			row_col_value, mean);
		printf("%s %u 95%%-CI-2SE     %f->%f\n", row_col_name,
			row_col_value, mean - 2.0*se, mean + 2.0*se);

		if ( STATSF_CODE(&flist->functions[c]) == STATSF_SAMPLE_Z_SCORES )
		{
		    // FIXME: Use accessor for tmp_file
		    rewind(flist->functions[c].tmp_file);
		    printf("%s %u z-scores:\n%16s %16s %16s\n", row_col_name, row_col_value,
			    "Val", "z-score", "CDF = P(x < Val)");
		    while ( fscanf(flist->functions[c].tmp_file, "%lf", &x) == 1 )
			printf("%16f %16f %16f\n", x,
				z_score(x, mean, stddev),
				z_percentile(x, mean, stddev));
		}
		break;
	    
	    default:
		break;
	}
    }
    return EX_OK;
}


void    statsf_list_init(statsf_list_t *flist)

{
    size_t  c;
    
    flist->count = 0;
    for (c = 0; c < SFL_MAX_FUNCTIONS; ++c)
	statsf_init(&flist->functions[c]);
}
