#include <stdlib.h>
#include <math.h>
#include <xtend/mem.h>
#include <xtend/math.h> // xt_double_cmp()
#include "statsf.h"

void    statsf_process_val(statsf_t *function, double x)

{
    // Always compute mean since it's cheap and necessary for SE, etc.
    function->sum += x;
    switch(function->code)
    {
	case STATSF_MEAN:
	    break;
	
	case STATSF_QUANTILE:
	    // FIXME: Check malloc
	    if ( function->num_count == function->array_size )
		function->nums = xt_realloc(function->nums,
			       function->array_size *= 2,
			       sizeof(*function->nums));
	    function->nums[function->num_count] = x;
	    break;

	case STATSF_POP_Z_SCORES:
	case STATSF_SAMPLE_Z_SCORES:
	    fprintf(function->tmp_file, "%f\n", x);
	    // Fall-through, no break
	case STATSF_POP_VAR:
	case STATSF_SAMPLE_VAR:
	case STATSF_POP_STDDEV:
	case STATSF_SAMPLE_STDDEV:
	case STATSF_SAMPLE_STDERR:
	case STATSF_T_SCORE:
	    function->sum_x += x;
	    function->sum_x_2 += x * x;
	    break;

	default:
	    break;
    }
    ++function->num_count;
}


void    statsf_init(statsf_t *function)

{
    function->row = 0;
    function->col = 0;
    function->rows = NULL;
    function->cols = NULL;
    function->sum = 0.0;
    function->nums = NULL;
    function->num_count = 0;
    function->array_size = 0;
    function->partitions = 0;
    function->sum_x = 0.0;
    function->sum_x_2 = 0.0;
    function->row_sums = NULL;
    function->col_sums = NULL;
    function->tmp_file = tmpfile();
}


void    statsf_quantiles(statsf_t *function,
		  const char *row_col_name, unsigned row_col_value)

{
    double  *list = function->nums,
	    quantile,
	    p,
	    a;
    size_t  list_size = function->num_count,
	    partitions = function->partitions,
	    k,
	    c;
    
    qsort(list, list_size, sizeof(double),
	  (int (*)(const void *, const void *))xt_double_cmp);
    
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


double  statsf_sum_squares(statsf_t *function)

{
    return  function->sum_x_2 -
	    function->sum_x * function->sum_x / function->num_count;
}


/***************************************************************************
 *  Library:
 *      #include <>
 *      -l
 *
 *  Description:
 *      z-score = (x - mean) / stddev
 *  
 *  Arguments:
 *
 *  Returns:
 *
 *  Examples:
 *
 *  Files:
 *
 *  Environment
 *
 *  See also:
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-10-27  Jason Bacon Begin
 ***************************************************************************/

double  z_score(double x, double mean, double stddev)

{
    return (x - mean) / stddev;
}


/***************************************************************************
 *  Library:
 *      #include <>
 *      -l
 *
 *  Description:
 *      z-score = (x - mean) / stddev
 *      equivalent score = z-score * new_stddev + new_mean
 *  
 *  Arguments:
 *
 *  Returns:
 *
 *  Examples:
 *
 *  Files:
 *
 *  Environment
 *
 *  See also:
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-10-27  Jason Bacon Begin
 ***************************************************************************/

double  z_equivalent(double x, double mean, double stddev,
		     double new_mean, double new_stddev)

{
    return z_score(x, mean, stddev) * new_stddev + new_mean;
}


/***************************************************************************
 *  Library:
 *      #include <>
 *      -l
 *
 *  Description:
 *  
 *  Arguments:
 *
 *  Returns:
 *
 *  Examples:
 *
 *  Files:
 *
 *  Environment
 *
 *  See also:
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-10-27  Jason Bacon Begin
 ***************************************************************************/

double  normal_cdf(double x, double mean, double stddev)

{
    return 0.5 * (1.0 + erf((x - mean) / (stddev * M_SQRT2)));
}


/***************************************************************************
 *  Library:
 *      #include <>
 *      -l
 *
 *  Description:
 *      t-score = (x-bar - mean) / (s / sqrt(n))
 *  
 *  Arguments:
 *
 *  Returns:
 *
 *  Examples:
 *
 *  Files:
 *
 *  Environment
 *
 *  See also:
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-10-27  Jason Bacon Begin
 ***************************************************************************/

double  t_score(double x, double expected_mean, double sample_stddev, unsigned n)

{
    return (x - expected_mean) / (sample_stddev / sqrt((double)n));
}


/***************************************************************************
 *  Use auto-c2man to generate a man page from this comment
 *
 *  Library:
 *      #include <>
 *      -l
 *
 *  Description:
 *      https://en.wikipedia.org/wiki/Student%27s_t-distribution
 *      CDF involves hypergeometric function, which is complicated and not
 *      in standard libraries.
 *      PDF is simpler and only requires gamma function.  Can be applied
 *      with trapezoid rule for estimating integrals.  Probably not the
 *      most efficient approach, but should suffice for now.
 *  
 *  Arguments:
 *
 *  Returns:
 *
 *  Examples:
 *
 *  Files:
 *
 *  Environment
 *
 *  See also:
 *
 *  History: 
 *  Date        Name        Modification
 *  2021-11-03  Jason Bacon Begin
 ***************************************************************************/

double  t_pdf(double x, unsigned n)

{
    double  v = n - 1,
	    pdf;
    
    // n is unsigned, so don't try to negate it before dividing
    pdf = (tgamma(n / 2.0) / (sqrt(v * M_PI) * tgamma(v / 2.0)))
	* pow(1.0 + x * x / v, -(n / 2.0));
    return pdf;
}


double  t_cdf(double x, unsigned n)

{
    double  slice_width = 1.0 / 1024.0,
	    c,
	    area,
	    tolerance = 1e-10,
	    cdf = 0.0;
    
    /*
     *  Estimate area of trapezoid as PDF height at center * slice_width.
     *  Start 1/2 slice left of x and work backwards until slices have
     *  inconsequential area.
     *
     *  For positive X, compute area for -X and return 1-area, to save
     *  time.
     */
    
    if ( x < 0 )
	c = x - slice_width / 2.0;
    else
	c = -x - slice_width / 2.0;
    
    while ( (area = t_pdf(c, n) * slice_width) > tolerance )
    {
	cdf += area;
	c -= slice_width;
    }
    if ( x < 0 )
	return cdf;
    else
	return 1.0 - cdf;
}
