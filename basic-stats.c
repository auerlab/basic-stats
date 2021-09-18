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
    int     c;
    statsf_list_t flist;
    
    if ( argc < 4 )
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
	else if ( strcmp(argv[c],"quantile") == 0 )
	    statsf_list_add_func(&flist, STATSF_QUANTILE, &c, argv);
	else if ( strcmp(argv[c],"median") == 0 )
	    statsf_list_add_func(&flist, STATSF_MEDIAN, &c, argv);
	else if ( strcmp(argv[c],"quartile") == 0 )
	    statsf_list_add_func(&flist, STATSF_QUARTILE, &c, argv);
	else if ( strcmp(argv[c],"mean") == 0 )
	    statsf_list_add_func(&flist, STATSF_MEAN, &c, argv);
	else if ( strcmp(argv[c],"pop-var") == 0 )
	    statsf_list_add_func(&flist, STATSF_POP_VAR, &c, argv);
	else if ( strcmp(argv[c],"sample-var") == 0 )
	    statsf_list_add_func(&flist, STATSF_SAMPLE_VAR, &c, argv);
	else if ( strcmp(argv[c],"pop-stddev") == 0 )
	    statsf_list_add_func(&flist, STATSF_POP_STDDEV, &c, argv);
	else if ( strcmp(argv[c],"sample-stddev") == 0 )
	    statsf_list_add_func(&flist, STATSF_SAMPLE_STDDEV, &c, argv);
	else if ( strcmp(argv[c],"mode") == 0 )
	    statsf_list_add_func(&flist, STATSF_MODE, &c, argv);
	else if ( strcmp(argv[c],"range") == 0 )
	    statsf_list_add_func(&flist, STATSF_RANGE, &c, argv);
	else if ( strcmp(argv[c],"iq-range") == 0 )
	    statsf_list_add_func(&flist, STATSF_IQ_RANGE, &c, argv);
	else if ( strcmp(argv[c],"box-plot") == 0 )
	    statsf_list_add_func(&flist, STATSF_BOX_PLOT, &c, argv);
	else if ( strcmp(argv[c],"z-scores") == 0 )
	    statsf_list_add_func(&flist, STATSF_Z_SCORES, &c, argv);
	else
	    usage(argv);
    }
    
    return statsf_list_process_stream(&flist, stdin, delims);
}


void    usage(char *argv[])

{
    fprintf(stderr, "Usage: %s [--delim string] function1 [param1] --row row1 | --col col1 \\\n"
	    "        [function2 [param2] --row row2 | --col col2 ...]\n", argv[0]);
    fprintf(stderr, "\nAt least one of the following functions is required:\n\n");
    fprintf(stderr,"  mean\n");
    fprintf(stderr,"  quantile N (N = number of divisions)\n");
    fprintf(stderr,"  median (same as quantile 2)\n");
    fprintf(stderr,"  quartile (same as quantile 4)\n");
    fprintf(stderr,"  pop-var\n");
    fprintf(stderr,"  sample-var\n");
    fprintf(stderr,"  pop-stddev\n");
    fprintf(stderr,"  sample-stddev\n");
    fprintf(stderr,"  mode\n");
    fprintf(stderr,"  range\n");
    fprintf(stderr,"  iq-range\n");
    fprintf(stderr,"  box-plot\n");
    fprintf(stderr,"  z-scores\n");
    fprintf(stderr,"\nDefault delimiter is TAB.  The delimiter string may be multiple characters.\n");
    exit(EX_USAGE);
}
