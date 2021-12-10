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
#include "statsf-list.h"

// http://makemeanalyst.com/basic-statistics-for-data-analysis/

int     main(int argc, char *argv[])

{
    char    *delims = " \t";
    int     c,
	    flags = SFL_FLAG_NONE;
    statsf_list_t flist;

    if ( (argc == 2) && (strcmp(argv[1], "z-table") == 0) )
	return print_z_table();
    else if ( ((argc == 3) || (argc == 5)) && (strcmp(argv[1], "normal-cdf") == 0) )
	return print_normal_cdf(argc, argv);
    else if ( (argc == 5) && (strcmp(argv[1], "z-score") == 0) )
	return print_z_score(argc, argv);
    else if ( (argc == 5) && (strcmp(argv[1], "t-table") == 0) )
	return print_t_table();
    else if ( (argc == 6) && (strcmp(argv[1], "t-score") == 0) )
	return print_t_score(argc, argv);
    else if ( (argc == 4) && (strcmp(argv[1], "t-cdf") == 0) )
	return print_t_cdf(argc, argv);
    else if ( argc < 4 )
	usage(argv);
    
    statsf_list_init(&flist);
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
	else if ( strcmp(argv[c],"--verbose") == 0 )
	    flags |= SFL_FLAG_VERBOSE;
	else if ( strcmp(argv[c],"quantile") == 0 )
	    statsf_list_add_func(&flist, STATSF_QUANTILE, &c, argc, argv);
	else if ( strcmp(argv[c],"median") == 0 )
	    statsf_list_add_func(&flist, STATSF_MEDIAN, &c, argc, argv);
	else if ( strcmp(argv[c],"quartile") == 0 )
	    statsf_list_add_func(&flist, STATSF_QUARTILE, &c, argc, argv);
	else if ( strcmp(argv[c],"mean") == 0 )
	    statsf_list_add_func(&flist, STATSF_MEAN, &c, argc, argv);
	else if ( strcmp(argv[c],"pop-var") == 0 )
	    statsf_list_add_func(&flist, STATSF_POP_VAR, &c, argc, argv);
	else if ( strcmp(argv[c],"sample-var") == 0 )
	    statsf_list_add_func(&flist, STATSF_SAMPLE_VAR, &c, argc, argv);
	else if ( strcmp(argv[c],"pop-SD") == 0 )
	    statsf_list_add_func(&flist, STATSF_POP_STDDEV, &c, argc, argv);
	else if ( strcmp(argv[c],"pop-z-scores") == 0 )
	    statsf_list_add_func(&flist, STATSF_POP_Z_SCORES, &c, argc, argv);
	else if ( strcmp(argv[c],"sample-SD") == 0 )
	    statsf_list_add_func(&flist, STATSF_SAMPLE_STDDEV, &c, argc, argv);
	else if ( strcmp(argv[c],"sample-SE") == 0 )
	    statsf_list_add_func(&flist, STATSF_SAMPLE_STDERR, &c, argc, argv);
	else if ( strcmp(argv[c],"sample-z-scores") == 0 )
	    statsf_list_add_func(&flist, STATSF_SAMPLE_Z_SCORES, &c, argc, argv);
	else if ( strcmp(argv[c],"sample-t-score") == 0 )
	    statsf_list_add_func(&flist, STATSF_T_SCORE, &c, argc, argv);
	else if ( strcmp(argv[c],"mode") == 0 )
	    statsf_list_add_func(&flist, STATSF_MODE, &c, argc, argv);
	else if ( strcmp(argv[c],"range") == 0 )
	    statsf_list_add_func(&flist, STATSF_RANGE, &c, argc, argv);
	else if ( strcmp(argv[c],"iq-range") == 0 )
	    statsf_list_add_func(&flist, STATSF_IQ_RANGE, &c, argc, argv);
	else if ( strcmp(argv[c],"box-plot") == 0 )
	    statsf_list_add_func(&flist, STATSF_BOX_PLOT, &c, argc, argv);
	else if ( strcmp(argv[c],"chi-sq-fit") == 0 )
	    statsf_list_add_func(&flist, STATSF_CHI_FIT, &c, argc, argv);
	else if ( strcmp(argv[c],"chi-sq-ind") == 0 )
	    statsf_list_add_func(&flist, STATSF_CHI_IND, &c, argc, argv);
	else
	    usage(argv);
    }
    
    return statsf_list_process_stream(&flist, stdin, delims, flags);
}


void    usage(char *argv[])

{
    fprintf(stderr, "\nSimple functions:\n");
    fprintf(stderr, "   %s z-table\n", argv[0]);
    fprintf(stderr, "   %s z-score x mean SD|SEM\n", argv[0]);
    fprintf(stderr, "   %s z-score x mean SSD n\n", argv[0]);
    fprintf(stderr, "   %s normal-cdf x [mean SD] (defaults: 0 1)\n\n", argv[0]);
    fprintf(stderr, "   %s t-score x-bar expected-mean SD n\n", argv[0]);
    fprintf(stderr, "   %s t-cdf x-bar n\n", argv[0]);
    
    fprintf(stderr, "Tabular data:\n");
    fprintf(stderr, "   %s [--verbose] [--delim 'string'] \\\n"
		    "       function1 [param1] --row|--col N1 \\\n"
		    "       [function2 [param2] --row|--col N2 ...] \\\n"
		    "       < input-file\n", argv[0]);
    fprintf(stderr, "\nFor tabular data at least one of the following functions is required:\n\n");
    fprintf(stderr, "  mean\n");
    fprintf(stderr, "  quantile N (N = number of divisions)\n");
    fprintf(stderr, "  median (same as quantile 2)\n");
    fprintf(stderr, "  quartile (same as quantile 4)\n");
    fprintf(stderr, "  pop-var\n");
    fprintf(stderr, "  pop-SD\n");
    fprintf(stderr, "  pop-z-scores\n");
    fprintf(stderr, "  sample-var\n");
    fprintf(stderr, "  sample-SD\n");
    fprintf(stderr, "  sample-z-scores\n");
    fprintf(stderr, "  sample-SE\n");
    fprintf(stderr, "  sample-t-score expected-mean\n");
    fprintf(stderr, "  mode\n");
    fprintf(stderr, "  range\n");
    fprintf(stderr, "  iq-range\n");
    fprintf(stderr, "  box-plot\n");
    fprintf(stderr, "\nDefault delimiter is TAB.  The delimiter string may be multiple characters.\n");
    fprintf(stderr, "Multiple consecutive spaces are treated as a single delimiter.\n");
    fprintf(stderr, "\nExample: basic-stats sample-SD --col 9 pop-var --col 10 < sample.tsv\n");
    fprintf(stderr, "Example: echo '3 6 1 8 2' | basic-stats mean --row 1\n\n");
    exit(EX_USAGE);
}


int     print_z_table(void)

{
    double  row, col;

    printf("      ");
    for (col = 0.00; col < 0.099; col += 0.01)
	printf("%6.2f ", col);
    putchar('\n');
    for (row = -3.0; row < 3.1; row += 0.1)
    {
	printf("%4.1f  ", row);
	for (col = 0.00; col < 0.099; col += 0.01)
	    printf("%.4f ", normal_cdf(row + (row < 0 ? -col : col), 0.0, 1.0));
	putchar('\n');
    }
    return EX_OK;
}


int     print_z_score(int argc, char *argv[])

{
    double  x, mean, stddev, score, cdf;
    char    *end;
    
    x = strtod(argv[2], &end);
    mean = strtod(argv[3], &end);
    stddev = strtod(argv[4], &end);
    score = z_score(x, mean, stddev);
    printf("z-score = %f\n", score);
    printf("P(x < %f) = %f\n", x, cdf = normal_cdf(x, mean, stddev));
    printf("1 - P = %f\n", 1.0 - cdf);
    return EX_OK;
}


int     print_normal_cdf(int argc, char *argv[])

{
    double  x, mean, stddev, cdf;
    char    *end;
    
    if ( argc == 3 )
    {
	x = strtod(argv[2], &end);
	if ( *end != '\0' )
	{
	    fprintf(stderr, "Invalid z-score: %s\n", argv[1]);
	    usage(argv);
	}
	printf("P(x < %f) = %f\n", x, cdf = normal_cdf(x, 0.0, 1.0));
	printf("1 - P = %f\n", 1.0 - cdf);
    }
    else if ( argc == 5 )
    {
	x = strtod(argv[2], &end);
	mean = strtod(argv[3], &end);
	stddev = strtod(argv[4], &end);
	printf("P(x < %f) = %f\n", x, cdf = normal_cdf(x, mean, stddev));
	printf("1 - P = %f\n", 1.0 - cdf);
    }
    else
	usage(argv);
    return EX_OK;
}


int     print_t_table(void)

{
    fprintf(stderr, "Not yet iumplemented.\n");
    return EX_OK;
}


int     print_t_score(int argc, char *argv[])

{
    double      x_bar, expected_mean, stddev, score, cdf;
    unsigned    n;
    char        *end;
    
    x_bar = strtod(argv[2], &end);
    expected_mean = strtod(argv[3], &end);
    stddev = strtod(argv[4], &end);
    n = strtoul(argv[5], &end, 10);
    score = t_score(x_bar, expected_mean, stddev, n);
    printf("t(%u) = %f\n", n - 1, score);
    printf("P(t(%u) < %f) = %f\n", n - 1, score, cdf = t_cdf(score, n));
    printf("1 - P = %f\n", 1.0 - cdf);
    return EX_OK;
}


int     print_t_cdf(int argc, char *argv[])

{
    double      x_bar, cdf;
    unsigned    n;
    char        *end;
    
    x_bar = strtod(argv[2], &end);
    n = strtoul(argv[3], &end, 10);
    printf("P(x_bar < %f) = %f\n", x_bar, cdf = t_cdf(x_bar, n));
    printf("1 - P = %f\n", 1.0 - cdf);
    
    return EX_OK;
}

