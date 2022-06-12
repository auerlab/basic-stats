#ifndef _STDIO_H_
#include <stdio.h>
#endif

typedef enum
{
    STATSF_QUANTILE = 0,
    STATSF_MEDIAN,
    STATSF_QUARTILE,
    STATSF_MEAN,
    STATSF_MODE,
    STATSF_POP_VAR,
    STATSF_POP_STDDEV,
    STATSF_POP_Z_SCORES,
    STATSF_SAMPLE_VAR,
    STATSF_SAMPLE_STDDEV,
    STATSF_SAMPLE_Z_SCORES,
    STATSF_SAMPLE_STDERR,
    STATSF_T_SCORE,
    STATSF_RANGE,
    STATSF_IQ_RANGE,
    STATSF_BOX_PLOT,
    STATSF_CHI_FIT,
    STATSF_CHI_IND
}   statsf_code_t;

typedef struct
{
    statsf_code_t   code;       // MEAN, MEDIAN, etc.
    union
    {
	unsigned        row;    // 1-based row in input
	unsigned        *rows;  // List of rows for multirow (e.g. chi-sq)
    };
    union
    {
	unsigned        col;    // 1-based col in input
	unsigned        *cols;  // List of cols for multicol (e.g. chi-sq)
    };
    double          sum;        // Mean
    double          *nums;      // Array for sorting for quantiles
				// FIXME: Use temp file and Unix sort instead
    size_t          num_count;
    size_t          array_size;
    unsigned        partitions; // Quantile partitions

    // For variance use theorom sum[(x - u)^2] = sum[x^2] - (sum[x])^2 / n
    double          sum_x;
    double          sum_x_2;
    double          *row_sums;  // List of row sums for multirow (e.g. chi-sq)
    double          *col_sums;  // List of col sums for multirow (e.g. chi-sq)
    
    double          expected_mean;  // Parameter for t-scores

    // Pearson's r
    double          sum_y;
    double          sum_y_2;
    double          sum_x_y;
    
    FILE            *tmp_file;
}   statsf_t;


#include "statsf-rvs.h"
#include "statsf-accessors.h"
#include "statsf-mutators.h"

#include "statsf-protos.h"
#include "statsf-mutators-protos.h"
